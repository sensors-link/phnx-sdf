/*****************************************
 * @file            main.c
 * @brief           test project
 * @version         v0.0
 * @date            2019/08/15
 * @author          tangbifei
 * @note      Ӧ����Keilƽ̨
 * Copyright(C) 2019,  FanHai Data Tech Corp.
 * All rights reserved
 ******************************************/
#include "FH_phoenix01.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "iom.h"
#include "uart.h"
#include "sysc.h"
#include "pmu.h"
#include "lptimer.h"
#include "wdt.h"
#include "efc.h"
#include "anac.h"


#define  TEST_PIN       13
#define  REST_DEF_CFG   12
#define  ALARM_EXIT_PIN    9

#ifdef _PIN_CTL
#define  PULSE_PIN      8
#endif

#define  PULSE_CNT      1          
#define  PULSE_DEL     100            //100us
#ifdef  TIM_P 
#define  LED_EN_DEL       40         //us
#endif

#ifdef _LED_ALARM
#define   LED_ALARM_PIN   10
#define 		LED_ALARM_SET()      {IOM->GPIO_DATA |= (1<<LED_ALARM_PIN);}
#define 		LED_ALARM_CLR()      {IOM->GPIO_DATA &= ~(1<<LED_ALARM_PIN);}
#endif

//led anac cfg
#define  LED_CUR_R       3      //N*10mA
#define  ANAC_CLK_DIV    2      //set:2/3...32
#define  ADC_CLK_DIV     8      //set:2 4 6 8 ...32
#define  SENSOR_GEAN_VALUE  0x0f         //0 max,0x0f min

//def param 
#define  LPT_WK_DIV           99         // 4ms*(N+1)
#define  WDT_OV_DIV         9         // 4ms*( 1<<(N+1))
#define  UART_BAUD         9600 

#define  SMOKE_ALARM_TIMES     5
#define  TMP_ALARM_TIMES       5

#define  SMOKE_ALARM_CNT_MAX     10
#define  TEMP_ALARM_CNT_MAX     10


#define  SMOKE_ALARM_VALUE    0x100                                 //�����޸�
#define  TMP_ALARM_VALUE      0x100



#define  COMP_NOISE_VALUE      0x70

#define  COMP_SUM_ADJUST_VALUE 0x200

#define  COMP_STEP_VALUE     0x20



#define  ACK       0x06
#define  NACK      0xff

#define  EFC_EEPROM_BASE      0x10180000
#define  EFC_EEPROM_END       0x10180400 //0x101803ff    //1k


#define  THEPMD_EN()            {ANAC_WRPROTECT_CLOSE();ANAC->ANAC_CFG |= 1<<15;}
#define  THEPMD_DIS()            {ANAC_WRPROTECT_CLOSE();ANAC->ANAC_CFG &= ~(1<<15);}

#define  ADC_SWITCH_TMP()     {ANAC_WRPROTECT_CLOSE();ANAC->ADC_CFG &= ~(0x0f<<7);ANAC_WRPROTECT_CLOSE();ANAC->ADC_CFG |= (0x0e<<7);}
#define  ADC_SWITCH_SENS()    {ANAC_WRPROTECT_CLOSE();ANAC->ADC_CFG &= ~(0x0f<<7);ANAC_WRPROTECT_CLOSE();ANAC->ADC_CFG |= (0x0c<<7);}
#define  ADC_SWITCH_CH0()    {ANAC_WRPROTECT_CLOSE();ANAC->ADC_CFG &= ~(0x0f<<7);ANAC_WRPROTECT_CLOSE();ANAC->ADC_CFG |= (0x00<<7);}


#pragma pack(1)
typedef struct _Record{
	uint8_t bPhotoElecFaultFlag:1;
	uint8_t bLowVolFlag:1;
	uint8_t bOpenedFlag:1;
	uint8_t bAlmFlag:1;
    uint8_t bRsv:4;
	uint8_t  hwYear;
	uint8_t  chMon;
	uint8_t  chDay;
	uint8_t  chHour;
	uint8_t  chMinu;
	uint8_t  chSec;
    uint8_t  Rsv;
}sRecordType_Def;


typedef struct _E2PCfgTable{          //e2prom  1K
    uint32_t wBaud;
	uint8_t u8WDTDiv;   
	uint8_t u8I2cAddr;
//	uint32_t wI2cClk;
	uint16_t hwLPTDiv;       
	uint16_t hwAlmVal;
	uint16_t hwAlmValN;
	uint16_t hwTmpAlmVal;
	uint16_t hwTmpAlmValN;
	uint16_t hwStaticVal;
	uint16_t hwStaticValN;
	uint16_t hwCompValue;
	uint16_t hwCompValueN;
	sRecordType_Def sRecord[10];
	uint8_t u8NewRcdPos;
	uint8_t iSmokeAlarmTimes;
	uint8_t iTmpAlarmTimes;
    uint8_t Rsv;
    
	uint32_t wCfgCHK;
}sE2PCfgTableType_Def;

typedef struct _GVAR{          //e2prom  1K
	uint16_t hwSmokeVal;
	uint16_t hwTmpVal;
	uint16_t hwBatVolt;

	uint32_t wI2CRdFlag;
	uint32_t wI2CWrFlag;
	uint32_t wI2CRcvCmdFlag;

	uint16_t wAddr;
	uint32_t  wDatLen;
	uint8_t  u8I2CCmd;
	uint32_t wPos;	

	uint32_t wTimeCnt;

}sGVarType_Def;

#pragma pack()

typedef struct _TIM_EVENT{
    uint32_t  wStartFlag;
    uint32_t  wCurTimeCnt;
    uint32_t  wEndTimeCnt;
}sTimEVType_Def;

sTimEVType_Def  gsIncLEDEV,gsLdCfgEV,gsCompEV,gsAlarmLed;

//globle varible declare
sE2PCfgTableType_Def  gsCfgVar;
// __align(4) sE2PCfgTableType_Def  gsCfgVar;
volatile sGVarType_Def  gsVar;
int gAlarmCnt=0,gTmpAlarmCnt = 0,gAlarmSmokeFlag=0,gAlarmTmpFlag = 0;
int bLoadCFGValueFlag=0,bReCalStaticFlag = 0,bCompFlag = 0,bRecordFlag =0;
uint8_t gCmd,gCmdN;
uint32_t gAddr,gData;

//extern function declare
extern void DelayNus(uint32_t wDel);

void INCDelayNus(uint32_t  wDel)     //8M
{
// Loop MOVS R1,#1
//      NOP
//      NOP
//      NOP
//      NOP
//      SUBS R0,R0,R1
//      BNE  Loop
//      BX   LR
}


uint32_t CheckSum(uint8_t *pDat,int iLen)
{
    uint32_t wSum = 0;
    while(iLen--)
    {
        wSum += *pDat++;
    }
    return wSum;
}


void Test(void)
{
#ifdef _FW_DEBUG
    printf("lpt int\r\n");
#endif
}


void McuInit(void)
{
	EFC->EFC_WPT = 0xc3;
	EFC->EFC_WPT = 0x3c;
	EFC->EFC_CR &= ~(1<<11);	

	SYSC_SetCLKENCFG(SYSC_CLKENCFG_LPTIM_PCKEN | SYSC_CLKENCFG_PMU_PCKEN);
	PMU_WRPROTECT_CLOSE();
	PMU_SetPMU_CR(1<<PMU_CR_LPCLKEN_pos);
    SYSC_SetCLKENCFG(SYSC_CLKENCFG_LPWDT_PCKEN);
#ifdef _WDT_EN
	PMU_WRPROTECT_CLOSE();
	PMU_SetPMU_CR(1<<PMU_CR_WDTCLKEN_pos);
    SYSC_SetCLKENCFG(SYSC_CLKENCFG_LPWDT_PCKEN);
	WDT_Init(gsCfgVar.u8WDTDiv,WDT_WDTCR_RST_MODE,NULL);     //reset mode
	WDT_ClrCnt();
#endif

//pin variable etc
	SYSC_SetCLKENCFG(SYSC_CLKENCFG_IOM_PCKEN);
	IOM->GPIO_ADS &= ~(1<<TEST_PIN);  //enter test pin
	IOM->GPIO_DIR &= ~(1<<TEST_PIN);
	IOM->GPIO_PU  |=  (1<<TEST_PIN);

	IOM->GPIO_ADS &= ~(1<<REST_DEF_CFG);  //rest def cfg pin
	IOM->GPIO_DIR &= ~(1<<REST_DEF_CFG);
	IOM->GPIO_PU  |=  (1<<REST_DEF_CFG);

	IOM->GPIO_ADS &= ~(1<<ALARM_EXIT_PIN);  
	IOM->GPIO_DIR &= ~(1<<ALARM_EXIT_PIN);
	IOM->GPIO_PU  |=  (1<<ALARM_EXIT_PIN);
#ifdef _LED_ALARM
    IOM->GPIO_ADS &= ~(1<<LED_ALARM_PIN);  
	IOM->GPIO_DIR |=  (1<<LED_ALARM_PIN);
	IOM->GPIO_DATA &= ~(1<<LED_ALARM_PIN);
#endif

//anan adc div
	SYSC->ANCLKDIV =  ((ANAC_CLK_DIV-1)<<4) | ((ADC_CLK_DIV>>1)-1) ;
//adc cfg
	SYSC_SetCLKENCFG(SYSC_CLKENCFG_ANAC_PCKEN);
	ANAC_WRPROTECT_CLOSE();
	ANAC->ADC_CFG = 0;
	ANAC->ADC_CTL = 0;
	ANAC_WRPROTECT_CLOSE();
	ANAC->ME_CTL = 0x01;
	ANAC_WRPROTECT_CLOSE();
	ANAC->ANAC_CFG |= (ANAC_ANAC_CFG_BGR_EN);
	DelayNus(1000);
	ANAC_WRPROTECT_CLOSE();
	ANAC->ANAC_CFG |= (ANAC_ANAC_CFG_ALDO4A_EN);
	DelayNus(1000);

	ANAC_WRPROTECT_CLOSE();
	ANAC->ADC_CFG = 0;
	ANAC_WRPROTECT_CLOSE();
	ANAC->ADC_CFG = (0x0c<<7) | (1<<6) | (2<<1) | (1<<0);
	ANAC->ADC_CTL =0x02;
    DelayNus(100);
    ANAC->ADC_CTL = 0;	
	ANAC->ADC_CTL = (3<<6) | (1<<3);
	ANAC->ADC_CTL |= (1<<0);
	//sensor cfg
	ANAC_WRPROTECT_CLOSE();
	ANAC->PDSENS_CFG = 0;
	ANAC_WRPROTECT_CLOSE();
	ANAC->PDSENS_CFG |= (SENSOR_GEAN_VALUE<<2);
	ANAC_WRPROTECT_CLOSE();
	ANAC->PDSENS_CFG |= 0x01;
#ifdef _PIN_CTL
    IOM->GPIO_ADS &= ~(1<<PULSE_PIN);
    IOM->GPIO_DIR |= (1<<PULSE_PIN);
    IOM->GPIO_DATA &= ~(1<<PULSE_PIN);
#else
	//dc dc
	ANAC_WRPROTECT_CLOSE();
	ANAC->DC_CFG |= 0x01;
//    while( ( ANAC->ANAC_FLAG & (1<<5) ) == 0);
	//led cfg
	ANAC_WRPROTECT_CLOSE();
	ANAC->LED_CFG = 0;
	ANAC_WRPROTECT_CLOSE();
	ANAC->LED_CFG = (LED_CUR_R<<7) | (LED_CUR_R<<1);
//ANAC_WRPROTECT_CLOSE();
//ANAC->LED_CFG |= (1<<6) | (1<<0);
#endif   
    //even init 
    gsIncLEDEV.wStartFlag = 1;
	gsIncLEDEV.wCurTimeCnt=gsVar.wTimeCnt;
	gsIncLEDEV.wEndTimeCnt=8000/400;        //8s ��˸һ��

    gsCompEV.wStartFlag = 1;
	gsCompEV.wCurTimeCnt=gsVar.wTimeCnt;
	gsCompEV.wEndTimeCnt=8000/400;        //8s  comp handler 	
	
}

void PutChar(uint8_t chChar)
{
#if defined(_UART2_PRINT)
	UART2 -> THR = chChar;
	while( (UART2->LSR & (1ul<<6)) == 0);	
#else
	UART1 -> THR = chChar;
	while( (UART1->LSR & (1ul<<6)) == 0);
#endif
}

uint8_t GetChar(uint32_t wOT,uint8_t *pDat)
{
	if(!wOT)
	{
#if defined(_UART2_PRINT)
		while( (UART2->LSR & (1ul<<0)) == 0);
		*pDat = UART2 -> RBR & 0xff;	
#else
		while( (UART1->LSR & (1ul<<0)) == 0);
		*pDat = UART1 -> RBR & 0xff;	
#endif
		return 1;
	}
	else
	{
#if defined(_UART2_PRINT)
		while( ( (UART2->LSR & (1ul<<0)) == 0) && (--wOT>0) );
		if( wOT>0)
			*pDat = UART2 -> RBR & 0xff;
		else
			return 0;
#else
		while( ((UART1->LSR & (1ul<<0)) == 0) && (--wOT>0) );
		if( wOT>0)
			*pDat = UART1 -> RBR & 0xff;	
		else
			return 0;
#endif
			return 1;
	}


}

void TestModeHandler(void)
{
	if( (IOM->GPIO_DATA & (0x1<<TEST_PIN)) == 0)
	{
		int i=1000;
		do{
			if( (IOM->GPIO_DATA & (0x01<<TEST_PIN))!=0 )
				return;
			__NOP();__NOP();__NOP();__NOP();
		}
		while(--i);//DelayNus(10000);
		if( (IOM->GPIO_DATA & (0x1<<TEST_PIN)) == 0)
		{
			while(1)                                           //UARTЭ��
			{
            uint8_t chChar;
ReStart:
#ifdef _WDT_EN
		WDT_ClrCnt();
#endif

                GetChar(0,&chChar);
				if( chChar == 0xcc)
				{
					int i;
                    if( !GetChar(100000,&gCmd) )
                        goto ReStart;
                    if( !GetChar(100000,&gCmdN) )
                        goto ReStart;
					for(i=0;i<4;i++)
					{
						if( !GetChar(100000,(uint8_t*)&gAddr + i ) )
							goto ReStart;
					}
					for(i=0;i<4;i++)
					{
						if( !GetChar(100000,(uint8_t*)&gData + i) )
							goto ReStart;
					}                    
                    gCmdN = ~gCmdN;
					if( gCmd == gCmdN)
					{
						switch(gCmd)
						{
						//eeprom
						case 0x01:
							{        
                                //gData &= 0xff;
                                gData |= REG32(gAddr) & (~0xff);
								if( EFC_PageErase(gAddr))
								{
									PutChar(gCmd);
									PutChar(NACK);
									break;
								}
								if( EFC_SingleProgram(EFC_PRG_BYTE,gAddr,gData))
								{
									PutChar(gCmd);
									PutChar(NACK);
									break;
								}
							PutChar(gCmd);
							PutChar(ACK);							
							}
							break;
						case 0x02:  
							{
                                ////gData &= 0xffff;
                                gData |= REG32(gAddr) & (~0xffff);
								if( EFC_PageErase(gAddr))
								{
									PutChar(gCmd);
									PutChar(NACK);
									break;
								}
								if( EFC_SingleProgram(EFC_PRG_HWORD,gAddr,gData))
								{
									PutChar(gCmd);
									PutChar(NACK);
									break;
								}
								PutChar(gCmd);
								PutChar(ACK);	

							}
							break;
						case 0x03:
							{								
								if( EFC_PageErase(gAddr))
								{
									PutChar(gCmd);
									PutChar(NACK);
									break;
								}
								if( EFC_SingleProgram(EFC_PRG_WORD,gAddr,gData))
								{
									PutChar(gCmd);
									PutChar(NACK);
									break;
								}
								PutChar(gCmd);
								PutChar(ACK);	
							}
							break;
						case 0x04:
							{
								gData = REG32(gAddr);
								PutChar(gCmd);
								PutChar(gData&0xff);
								PutChar((gData>>8)&0xff);
                                PutChar((gData>>16)&0xff);
                                PutChar((gData>>24)&0xff);
							}
                            break;
					    //sram
						case 0x11:
							{
								REG8(gAddr) = gData;
								PutChar(gCmd);
								PutChar(ACK);	
							}
							break;
						case 0x12:
							{
								REG16(gAddr) = gData;
								PutChar(gCmd);
								PutChar(ACK);	
							}
							break;					
						case 0x13:
							{
								REG32(gAddr) = gData;
								PutChar(gCmd);
								PutChar(ACK);	
							}
							break;
						case 0x14:
							{
								gData = REG32(gAddr);
								PutChar(gCmd);
                                PutChar(gData&0xff);
								PutChar((gData>>8)&0xff);
                                PutChar((gData>>16)&0xff);
                                PutChar((gData>>24)&0xff);
							}
							break;
                        //control
						case 0x21:          //pcb 
							{
								ANAC_WRPROTECT_CLOSE();
								ANAC->LED_CFG |= (1<<12);
								DelayNus(100000);
								ANAC_WRPROTECT_CLOSE();
								ANAC->LED_CFG &= ~(1<<12);
								//other 
								
							}
                            PutChar(gCmd);
							PutChar(ACK);
							break;
						case 0x22:
							{
								PMU_WRPROTECT_CLOSE();
								__ISB();	
								PMU_SoftDigtalReset();								
							}
						break;
						case 0x23:
							{
								PMU_WRPROTECT_CLOSE();
								__ISB();	
								PMU_SoftChipReset();
							}
						break;
						
						
						}
						
					}
				}

			}
		}

	}
}
#ifdef _TIM_P
void TIM0Int(void)
{
	ANAC->WPROT = 0x5a5a;ANAC->WPROT = 0xa5a5;	
	ANAC->LED_CFG &= ~((1<<6) | (1<<0));
	TIMER0->CR = 0;
}
#endif
void ADC_Sample(int iChn)
{
	int i;
	if( iChn)
	{
		ADC_SWITCH_SENS();
#ifdef TIM_P
		DelayNus(500);
		{
			TIMInitType_Def sTimInit;
			sTimInit.wIntSel = TIM_INT_EN;
			sTimInit.wReloadValue = (PULSE_DEL*8)-1;
			sTimInit.wRunMode = TIM_RUN_MODE_DEFINE;
			sTimInit.wPwmSel = TIM_PWM_DIS;
			TIM_Init(TIMER0,&sTimInit,(TIMSignalEvent_Type)TIM0Int);
			TIM_ClrIntFlag(TIMER0);
			NVIC_EnableIRQ(TIMER0_IRQn);
			ANAC->WPROT = 0x5a5a;ANAC->WPROT = 0xa5a5;
			ANAC->LED_CFG |= (1<<6) | (1<<0);
			INCDelayNus(LED_EN_DEL); 
			ANAC->ADC_CTL |= (1<<2);
		}
#else
		DelayNus(1000);
		ANAC->ADC_CTL |= (1<<2);
		for(i=0;i<PULSE_CNT;++i)
		{
#ifdef _PIN_CTL
            IOM->GPIO_DATA |= (1<<PULSE_PIN); 
            INCDelayNus(PULSE_DEL);   
            IOM->GPIO_DATA &= ~(1<<PULSE_PIN); 
            INCDelayNus(PULSE_DEL);   
#else
			ANAC->WPROT = 0x5a5a;ANAC->WPROT = 0xa5a5;
			ANAC->LED_CFG |= (1<<6) | (1<<0);
            INCDelayNus(PULSE_DEL);
			ANAC->WPROT = 0x5a5a;ANAC->WPROT = 0xa5a5;	
			ANAC->LED_CFG &= ~((1<<6) | (1<<0));
            INCDelayNus(PULSE_DEL);
#endif
		}
#endif
		while( (ANAC->ANAC_FLAG & (1<<3) ) == 0);	
        ANAC->ANAC_FLAG = (1<<3);
	}
	else
	{
		THEPMD_EN();
		ADC_SWITCH_TMP();
		DelayNus(1000);		
		ANAC->ADC_CTL |= (1<<2);
		while( (ANAC->ANAC_FLAG & (1<<2) ) == 0);
        ANAC->ANAC_FLAG = (1<<2);
        THEPMD_DIS();
	}
}

void Sort(uint16_t *pDat,uint32_t wLen)
{
    int i,j,k;
    for(i=0;i<wLen;++i)
    {
        k = 0;
        for(j=0;j<wLen-i;++j)
        {
            if( pDat[k] < pDat[j])
            {
                k = j;
            }
        }
        {
            uint16_t hwTmp = pDat[wLen-1-i];
            pDat[wLen-1-i] = pDat[k];
            pDat[k] = hwTmp;
        }        
    }

}

void SmokeTmpSample(void)
{
	uint16_t hwTmp[8];
	int i,j;
	//read tmp
	for(i=0;i<8;++i)
	{
		uint32_t wSum=0;
		uint32_t *pAdcAddr = (uint32_t *) &(ANAC->TEMP_VAL0);
		ADC_Sample(0);	
		for(j=0;j<8;++j)
		{
			wSum += *pAdcAddr;
#if _FW_DEBUG
//			printf("%x  ",*pAdcAddr);
#endif
			++pAdcAddr;
		}
		hwTmp[i] = wSum>>3;
	}
	Sort(hwTmp,8);
	gsVar.hwTmpVal = ( (uint32_t)hwTmp[3]+hwTmp[4])>>1;
#if _FW_DEBUG
	printf("\r\ntmp val:%x,",gsVar.hwTmpVal);
#endif
	//read sensor
	for(i=0;i<8;++i)
	{
		uint32_t wSum=0;
		uint32_t *pAdcAddr = (uint32_t *) &(ANAC->ADC_VAL0);
		ADC_Sample(1);	
		for(j=0;j<8;++j)
		{
			wSum += *pAdcAddr;
#if _FW_DEBUG
//			printf("%x  ",*pAdcAddr);
#endif
			++pAdcAddr;
		}
		hwTmp[i] = wSum>>3;
	}
	Sort(hwTmp,8);
	gsVar.hwSmokeVal= ( (uint32_t)hwTmp[3]+hwTmp[4])>>1;
#if _FW_DEBUG
	printf("smoke:%x\r\n",gsVar.hwSmokeVal);
#endif

}

void WriteCFGToE2PROM(void)
{
	int i;
	uint32_t *pDat = (uint32_t*) &gsCfgVar;
	for(i=0;i<sizeof(sE2PCfgTableType_Def)/4;i+=1)
	{
		int iRetryTimes = 5;
		do{
		if( EFC_PageErase(EFC_EEPROM_BASE+(i<<2)) == EFC_SUCCESS )
			break;
		}while(--iRetryTimes);
		if(iRetryTimes == 0)
		{
#if _FW_DEBUG
			printf("Erase Fail");
#endif		
			while(1);
		}
		iRetryTimes = 5;
		do{
		if( EFC_SingleProgram(EFC_PRG_WORD,EFC_EEPROM_BASE+(i<<2),*pDat++) == EFC_SUCCESS )
			break;
		}while(--iRetryTimes);
		if(iRetryTimes == 0)
		{
#if _FW_DEBUG
			printf("Program Fail");
#endif		
			while(1);
		}		
	}

}
void SmokeCompHandler(void)
{
	int i,j;
	if(bReCalStaticFlag)
	{
		uint16_t hwTmp[10];
		bReCalStaticFlag = 0;
		for(i=0;i<10;++i)
		{
			uint32_t wSum=0;
			uint32_t *pAdcAddr = (uint32_t *) &(ANAC->ADC_VAL0);
            ADC_Sample(1);
			for(j=0;j<8;++j)
			{
				wSum += *pAdcAddr;
#if _FW_DEBUG
	//			printf("%x  ",*pAdcAddr);
#endif
				++pAdcAddr;
			}
			hwTmp[i] = wSum>>3;
		}
		Sort(hwTmp,10);
		gsCfgVar.hwStaticVal = ( (uint32_t)hwTmp[3]+hwTmp[4]+hwTmp[5]+hwTmp[6])>>2;	
		gsCfgVar.hwStaticValN = ~gsCfgVar.hwStaticVal;
#if _FW_DEBUG
        printf("hwStaticVal:%x  ",gsCfgVar.hwStaticVal);
#endif        
		gsCfgVar.wCfgCHK = CheckSum((uint8_t*)&gsCfgVar,sizeof(sE2PCfgTableType_Def)-4);
		WriteCFGToE2PROM();
	}

	if( bCompFlag )
	{
		static uint32_t  wSumValue=0;
#if _FW_DEBUG
        printf("Comp handler,wSumValue:%x\r\n",wSumValue);
#endif         
		bCompFlag = 0;
		if( gsVar.hwSmokeVal - (gsCfgVar.hwStaticVal + gsCfgVar.hwCompValue) > 0)
		{
			if( gsVar.hwSmokeVal - (gsCfgVar.hwStaticVal + gsCfgVar.hwCompValue) > COMP_NOISE_VALUE)
			{
				if( gsVar.hwSmokeVal - (gsCfgVar.hwStaticVal + gsCfgVar.hwCompValue) < gsCfgVar.hwAlmVal)
				{						
					wSumValue += gsVar.hwSmokeVal - (gsCfgVar.hwStaticVal + gsCfgVar.hwCompValue);
				}
#if _FW_DEBUG
				else{
			   		printf("up line ov\r\n");
				}
#endif					
			}				
		}
		else if( (gsCfgVar.hwStaticVal + gsCfgVar.hwCompValue)- gsVar.hwSmokeVal < COMP_NOISE_VALUE)
		{
			if( (gsCfgVar.hwStaticVal + gsCfgVar.hwCompValue)- gsVar.hwSmokeVal >= gsCfgVar.hwAlmVal)
			{
#if _FW_DEBUG
			   printf("down line ov\r\n");
#endif				
				gsCfgVar.hwCompValue = 0;
				bReCalStaticFlag = 1;      
			}
			else
			{
				if( wSumValue > (gsCfgVar.hwStaticVal + gsCfgVar.hwCompValue) - gsVar.hwSmokeVal)
					wSumValue -=  (gsCfgVar.hwStaticVal + gsCfgVar.hwCompValue) - gsVar.hwSmokeVal;
				else
					wSumValue = 0;
			}
		}
		if( wSumValue > COMP_SUM_ADJUST_VALUE)
		{
#if _FW_DEBUG
			printf("Comp adjust\r\n");
#endif 
			wSumValue = 0;
			gsCfgVar.hwCompValue+= COMP_STEP_VALUE;
            gsCfgVar.hwCompValueN = ~gsCfgVar.hwCompValue;
		}	
	}
	
}

void TimerEventHandler(void)
{
	if( gsIncLEDEV.wStartFlag == 1)
	{
		if( gsVar.wTimeCnt - gsIncLEDEV.wCurTimeCnt > gsIncLEDEV.wEndTimeCnt)
        {
            gsIncLEDEV.wCurTimeCnt = gsVar.wTimeCnt;
//				ANAC_WRPROTECT_CLOSE();
//				if(ANAC->LED_CFG & (1<<12))
//					ANAC->LED_CFG &= ~(1<<12);
//				else
//					ANAC->LED_CFG |= (1<<12);
            ANAC_WRPROTECT_CLOSE();
            ANAC->LED_CFG |= (1<<12);
            DelayNus(100000);
            ANAC_WRPROTECT_CLOSE();
            ANAC->LED_CFG &= ~(1<<12);
        }
	}
	
	if( gsLdCfgEV.wStartFlag == 1)
	{
		if( gsVar.wTimeCnt - gsLdCfgEV.wCurTimeCnt >= gsLdCfgEV.wEndTimeCnt)
        {
            gsLdCfgEV.wCurTimeCnt = gsVar.wTimeCnt;
			bLoadCFGValueFlag = 1;
		}
	}
	
	if( gsCompEV.wStartFlag == 1)
	{
		if( gsVar.wTimeCnt - gsCompEV.wCurTimeCnt >= gsCompEV.wEndTimeCnt)
        {
            gsCompEV.wCurTimeCnt = gsVar.wTimeCnt;

			bCompFlag= 1;
		}
	}
	if( gsAlarmLed.wStartFlag == 1)
	{
		if( gsVar.wTimeCnt - gsAlarmLed.wCurTimeCnt >= gsAlarmLed.wEndTimeCnt)
        {
            gsAlarmLed.wCurTimeCnt = gsVar.wTimeCnt;
			if(IOM->GPIO_DATA & (1<<LED_ALARM_PIN))
			{
				LED_ALARM_CLR();
			}
			else
			{
				LED_ALARM_SET();
			}
		}		
	}
	
}



void RecordHandler(void)
{
    if( bRecordFlag )
	{
        bRecordFlag = 0;
		//��ǰʱ��д�뵱ǰλ�ü�¼

		//
        
		++gsCfgVar.u8NewRcdPos;
        if( gsCfgVar.u8NewRcdPos == 10)
            gsCfgVar.u8NewRcdPos = 0;
		CheckSum((uint8_t*)&gsCfgVar,sizeof(sE2PCfgTableType_Def)-4);
		WriteCFGToE2PROM();
	}
    
}

void FireAlarmHandler(void)
{
    static uint32_t wTimeCntTmp,wTTimeCntTmp;
	if( ( gsCfgVar.hwAlmVal != (uint16_t)(~gsCfgVar.hwAlmValN)) ||  ( gsCfgVar.hwTmpAlmVal != (uint16_t)(~gsCfgVar.hwTmpAlmValN)) \
		||( gsCfgVar.hwStaticVal != (uint16_t)(~gsCfgVar.hwStaticValN)) || ( gsCfgVar.hwCompValue != (uint16_t)(~gsCfgVar.hwCompValueN)))
    {
        bLoadCFGValueFlag = 1;
    }
    else
    {
        if( gsVar.hwSmokeVal > gsCfgVar.hwAlmVal + gsCfgVar.hwStaticVal + gsCfgVar.hwCompValue)
        {
            if( wTimeCntTmp + 1 == gsVar.wTimeCnt){
                if( gAlarmCnt < SMOKE_ALARM_CNT_MAX)
                    ++gAlarmCnt;
                wTimeCntTmp = gsVar.wTimeCnt;
                if( gAlarmCnt > gsCfgVar.iSmokeAlarmTimes)
                {
                    gAlarmSmokeFlag = 1;
                }
            }
            else
            {
                wTimeCntTmp = gsVar.wTimeCnt;
            	if(gAlarmCnt != 0)
                	--gAlarmCnt;
            }           
        }
        else
        {
            if( gsVar.wTimeCnt > wTimeCntTmp+SMOKE_ALARM_TIMES )
            {
                wTimeCntTmp = gsVar.wTimeCnt;
                	gAlarmCnt=0;
            }
        }
        if( gsVar.hwTmpVal <gsCfgVar.hwTmpAlmVal )
        {
            if( wTTimeCntTmp + 1 == gsVar.wTimeCnt){
                if( gAlarmCnt < TEMP_ALARM_CNT_MAX)
                    ++gTmpAlarmCnt;
                wTTimeCntTmp = gsVar.wTimeCnt;
                if( gTmpAlarmCnt > gsCfgVar.iTmpAlarmTimes)
                {
 //                   gAlarmTmpFlag = 1;
                }
            }
            else
            {
                wTTimeCntTmp = gsVar.wTimeCnt;
            	if(gTmpAlarmCnt != 0)
                	--gTmpAlarmCnt;
            }           
        }
        else
        {
            if( gsVar.wTimeCnt > wTTimeCntTmp+TMP_ALARM_TIMES )
            {
                wTTimeCntTmp = gsVar.wTimeCnt;
                gTmpAlarmCnt=0;
            }            
        }
#ifdef _FW_DEBUG
        printf("tmp times:%x,:smoke:%x\r\n",gTmpAlarmCnt,gAlarmCnt); 
#endif        
		if( gAlarmSmokeFlag || gAlarmTmpFlag )//if( gAlarmSmokeFlag && gAlarmTmpFlag )
			{
				gAlarmSmokeFlag = 0;
				gAlarmTmpFlag = 0;
#if 0
				while(1){
#ifdef _WDT_EN
                WDT_ClrCnt();
#endif

#ifdef _LED_ALARM
				LED_ALARM_SET();
                DelayNus(100000);
                LED_ALARM_CLR();
                DelayNus(100000);    
#endif
				if( (IOM->GPIO_DATA & (1<<ALARM_EXIT_PIN)) == 0)
					{
                        DelayNus(10000);
                        if( (IOM->GPIO_DATA & (1<<ALARM_EXIT_PIN)) == 0){
                            gAlarmCnt=gTmpAlarmCnt =gAlarmSmokeFlag=gAlarmTmpFlag = 0;
                            LED_ALARM_CLR();
                            break;
                        }
					}
				
				}
#endif
				gsCfgVar.hwLPTDiv = 100/4-1;  //100ms wake
				gsCfgVar.wCfgCHK = CheckSum((uint8_t*)&gsCfgVar,sizeof(sE2PCfgTableType_Def)-4);
				gsCfgVar.sRecord[gsCfgVar.u8NewRcdPos].bAlmFlag = 1;
				bRecordFlag = 1;
				gsAlarmLed.wStartFlag = 1;
				gsAlarmLed.wCurTimeCnt = gsVar.wTimeCnt;
				gsAlarmLed.wEndTimeCnt = 100/100;

			}
    }
}

void GotoPowerDown(void)
{
	LPT_Init(gsCfgVar.hwLPTDiv,LPT_CNTMODE_PIT,LPT_INT_EN,(LPTSignalEvent_Type)&Test);
	NVIC_EnableIRQ(LPT_IRQn);          
//	__disable_irq();
//	PMU->PMU_WPT = 0xc3;
//	PMU->PMU_WPT = 0x3c;
//	__enable_irq();
//	PMU->PMU_CR |= 0x0a;
//	__ISB();
//	__WFI();
//	__NOP();
//	__NOP();
//	__NOP();
//	__NOP();
    SCB->SCR |= (1<<2);
    __ISB();
    __NOP();
    __NOP();
    __WFI();
    __NOP();
    __NOP();    
}

uint32_t LoadCfgValue(void)
{
	int i;
	uint32_t *pdstAddr = (uint32_t*)&gsCfgVar;
	uint32_t *psrcAddr = (uint32_t*)EFC_EEPROM_BASE;
	uint32_t *psumAddr=(uint32_t*)(pdstAddr+(sizeof(sE2PCfgTableType_Def)/4)-1);
	for(i=0;i<sizeof(sE2PCfgTableType_Def)/4;++i)
	{
		*pdstAddr++ = *psrcAddr++;
	}
	if( CheckSum((uint8_t*)&gsCfgVar,sizeof(sE2PCfgTableType_Def)-4) != *psumAddr)
	{
		return 0;
	}
	else
		return 1;
}

void SetDefCfgValue(void)
{
	memset(&gsCfgVar,0,sizeof(sE2PCfgTableType_Def));	

	gsCfgVar.hwAlmVal = SMOKE_ALARM_VALUE;                    
	gsCfgVar.hwAlmValN=~SMOKE_ALARM_VALUE;   
	gsCfgVar.hwTmpAlmVal=TMP_ALARM_VALUE;
	gsCfgVar.hwTmpAlmValN=~TMP_ALARM_VALUE;
	gsCfgVar.hwStaticVal=0x020;
	gsCfgVar.hwStaticValN=~0x020;
	gsCfgVar.hwCompValue=0x00;
	gsCfgVar.hwCompValueN=~0x00;

	gsCfgVar.wBaud = UART_BAUD;
	gsCfgVar.hwLPTDiv = LPT_WK_DIV;
	gsCfgVar.u8WDTDiv = WDT_OV_DIV;

	gsCfgVar.iSmokeAlarmTimes = SMOKE_ALARM_TIMES;
	gsCfgVar.iTmpAlarmTimes   = TMP_ALARM_TIMES;
	
	gsCfgVar.wCfgCHK = CheckSum((uint8_t*)&gsCfgVar,sizeof(sE2PCfgTableType_Def));
}

int LoadCfg(void)
{
	if( ( IOM->GPIO_DATA & (1<<REST_DEF_CFG)) == 0)
	{
		DelayNus(10000);
		if( ( IOM->GPIO_DATA & (1<<REST_DEF_CFG)) == 0)
		{
			SetDefCfgValue();
		}
	}
//	else if( (PMU->PMU_RSTSTS & (1<<5)) && ( (PMU->PMU_RSTSTS & PMU_RSTSTS_EXTRST) ==0) )
	else if( (PMU->PMU_RSTSTS & (1<<5)) || (PMU->PMU_RSTSTS & PMU_RSTSTS_EXTRST) )
	{
		int i;
		PMU->PMU_RSTSTS = PMU->PMU_RSTSTS;
		bReCalStaticFlag = 1;
		memset((void*)&gsVar,0,sizeof(sGVarType_Def));
		memset((void*)&gsCfgVar,0,sizeof(sE2PCfgTableType_Def));
		for(i=0;i<10;++i)   
		{
			if( LoadCfgValue() )
			{
#ifdef _FW_DEBUG
				printf("load cfg success\r\n");
#endif
				return 1;
			}
		}
		if( i==10 )
		{
			SetDefCfgValue();
#ifdef _FW_DEBUG
			printf("ld cfg fail:set def\r\n");
#endif
		}
	}

	return 0;
}

void UartInit(void)
{
#ifdef _UART1_PRINT
	/*
	 * Initialize UART1 pin connect
	*/
	 SYSC_SetCLKENCFG(SYSC_CLKENCFG_UART1_PCKEN);
	{
		GPIOPinCfg_Type sU1PinCfg;
		memset(&sU1PinCfg,0,sizeof(sU1PinCfg));
		sU1PinCfg.wPin = GPIO_P00;
		sU1PinCfg.wAnalog = GPIO_ADS_SEL_DIGITAL;
		sU1PinCfg.wPinFun = GPIO_AF0_P00_SEL_UART1_RX;
		sU1PinCfg.wDir = GPIO_DIR_IN;
		GPIO_PinConfig(&sU1PinCfg,NULL);
		sU1PinCfg.wPin = GPIO_P01;
		sU1PinCfg.wPinFun = GPIO_AF0_P01_SEL_UART1_TX;
		sU1PinCfg.wDir = GPIO_DIR_OUT;
		GPIO_PinConfig(&sU1PinCfg,NULL);
	}
	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 9600bps   //clk = 8M
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	 {
		UART_InitType_Def sU1Init;
		sU1Init.wClkDiv = SYSC_GetPCLK()/16/gsCfgVar.wBaud;    //0x0034;
		sU1Init.wDataLen = UART_LCR_DLS_8BIT;
		sU1Init.wStopBit = UART_LCR_STOP_1BIT;
		UART_CfgParityCheck(UART1,UART_LCR_EPS_ODD,0);
		UART_Init(UART1,&sU1Init,NULL);
	 }
#else
	/*
	 * Initialize UART2 pin connect
	 */
	SYSC_SetCLKENCFG(SYSC_CLKENCFG_UART2_PCKEN);
	{
		GPIOPinCfg_Type sU2PinCfg;
		memset(&sU2PinCfg,0,sizeof(sU2PinCfg));
		sU2PinCfg.wPin = GPIO_P02;
		sU2PinCfg.wAnalog = GPIO_ADS_SEL_DIGITAL;
		sU2PinCfg.wPinFun = GPIO_AF0_P02_SEL_UART2_RX;
		sU2PinCfg.wDir = GPIO_DIR_IN;
		GPIO_PinConfig(&sU2PinCfg,NULL);
		sU2PinCfg.wPin = GPIO_P03;
		sU2PinCfg.wPinFun = GPIO_AF0_P03_SEL_UART2_TX;
		sU2PinCfg.wDir = GPIO_DIR_OUT;
		GPIO_PinConfig(&sU2PinCfg,NULL);
	}
	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 9600bps  //clk = 8M
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	{
		UART_InitType_Def sU2Init;
		sU2Init.wClkDiv = SYSC_GetPCLK()/16/gsCfgVar.wBaud; //0x0034;
		sU2Init.wDataLen = UART_LCR_DLS_8BIT;
		sU2Init.wStopBit = UART_LCR_STOP_1BIT;
		UART_CfgParityCheck(UART2,UART_LCR_EPS_ODD,0);
		UART_Init(UART2,&sU2Init,NULL);
	}	
#endif
}
void LoadCfgHandler(void)
{
	if( bLoadCFGValueFlag )
	{
		int i;
        bLoadCFGValueFlag = 0;
		for(i=0;i<10;++i)	
		{
			if( LoadCfgValue() )
			{
#ifdef _FW_DEBUG
				printf("load cfg success\r\n");
#endif
                break;
			}       
		}
        if( i==10)
        {
#ifdef _FW_DEBUG
            printf("load cfg fail\r\n");
#endif
             while(1);//wait wdt reset
        }

		
	}

}

void KeyHandler(void)
{
	if( (IOM->GPIO_DATA & (1<<ALARM_EXIT_PIN)) == 0)
		{
			DelayNus(10000);
			if( (IOM->GPIO_DATA & (1<<ALARM_EXIT_PIN)) == 0){
				gAlarmCnt=gTmpAlarmCnt =gAlarmSmokeFlag=gAlarmTmpFlag = 0;
				gsCfgVar.hwLPTDiv = 400/4-1;  //400ms wake
				gsCfgVar.wCfgCHK = CheckSum((uint8_t*)&gsCfgVar,sizeof(sE2PCfgTableType_Def)-4);
				gsCfgVar.sRecord[gsCfgVar.u8NewRcdPos].bAlmFlag = 0;
				gsAlarmLed.wStartFlag = 0;
			}
		}

}
int main (void)
{	
    SCB->VTOR = 0x10100000; 
	UART1->LCR = (1<<7);
	UART1->DLL = (8000000/16/UART_BAUD)&0xff;
	UART1->DLH = (8000000/16/UART_BAUD)>>8;
	UART1->LCR = 0x03; 
	LoadCfg(); 
    UartInit();
#ifdef _FW_DEBUG
    printf("uart test\r\n");
#endif 
	McuInit();
	//main loop	
	while(1)
	{
		TestModeHandler();
        ++gsVar.wTimeCnt;        
#ifdef _WDT_EN
		WDT_ClrCnt();
#endif
		LoadCfgHandler();
		SmokeTmpSample();
		SmokeCompHandler();
		TimerEventHandler();
		RecordHandler();		
		FireAlarmHandler();
		KeyHandler();
        printf("static:%03x,tmp:%03x,smk:%03x,Ttimes:%x,Stimes:%x\r\n",gsCfgVar.hwStaticVal,gsVar.hwTmpVal,gsVar.hwSmokeVal,gTmpAlarmCnt,gAlarmCnt);      
		GotoPowerDown();
	}
	
	
}



