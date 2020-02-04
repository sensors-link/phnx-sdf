/*****************************************
 * @file        efc.c
 * @brief       efc source file
 * @version     v0.0
 * @date        2019/05/06
 * @author      tangbifei
 * @note      ???Keil??
 * Copyright(C) 2019,  FanHai Data Tech Corp.
 * All rights reserved
 ******************************************/
#include "FH_phoenix01.h"
#include "assert.h"
#include "efc.h"


static EFCSignalEvent_Type cbEFCIntFunc;



void EFC_Init(EFCInitType_Def *psInit,EFCSignalEvent_Type cbFunc)
{
    EFC_WRPROTECT_CLOSE();
    EFC->EFC_Tnvs = psInit->wTnvs;
	EFC_WRPROTECT_CLOSE();
	EFC->EFC_Tprog = psInit->wTprog;
	EFC_WRPROTECT_CLOSE();
	EFC->EFC_Trcv = psInit->wTrcv;
	EFC_WRPROTECT_CLOSE();
	EFC->EFC_Terase = psInit->wTerase;
	if(psInit->wCR & EFC_CR_INTEN_mask)
		{
			ASSERT(cbFunc == NULL);
			cbEFCIntFunc = cbFunc;
		}
	EFC_WRPROTECT_CLOSE();
	EFC->EFC_CR = psInit->wCR;
}


eReturnType_Def EFC_SingleProgram(uint32_t wByteSel,int iAddr,uint32_t wData)
{
	ASSERT( iAddr<EFC_FLASH_ADDR_BASE);
	ASSERT( (iAddr>=EFC_FLASH_ADDR_END)&&(iAddr<EFC_NVR_ADDR_BASE));
	ASSERT( (iAddr>=EFC_NVR_ADDR_END)&&(iAddr<EFC_EEPROM_BASE));
	ASSERT( iAddr >= EFC_EEPROM_END);
	ASSERT(wByteSel>EFC_PRG_HWORD);
	uint32_t wStatus;
	EFC_GetStatusFlag(wStatus);
	EFC_ClrStatusFlag(wStatus);
	EFC_SetOPRMODE(EFC_OPRMODE_SIG_PRG);
	if( wByteSel == EFC_PRG_BYTE)
		{
				REG8(iAddr) = wData & 0xFF;				
		}
	else if(wByteSel == EFC_PRG_HWORD)
		{
				REG16(iAddr) = wData & 0xFFFF;
		}
	else
		{
			REG32(iAddr) = wData;			
		}
	do{
		EFC_GetStatusFlag(wStatus);
		}
	while(!wStatus);
	if( (wStatus)!=0x01)
		{
			return EFC_FAIL_PRG;
		}
	else
		{
			return EFC_SUCCESS;
		}
}



eReturnType_Def EFC_PageProgram(int iAddr)
{
	ASSERT( iAddr<EFC_FLASH_ADDR_BASE);
	ASSERT( (iAddr>=EFC_FLASH_ADDR_END)&&(iAddr<EFC_NVR_ADDR_BASE));
	ASSERT( iAddr>=EFC_NVR_ADDR_END);	
	uint32_t wStatus;
	EFC_GetStatusFlag(wStatus);
	EFC_ClrStatusFlag(wStatus);
	EFC_SetOPRMODE(EFC_OPRMODE_ROW_PRG);
	REG32(iAddr) = 0;
	do{
		EFC_GetStatusFlag(wStatus);
		}
	while(!wStatus);
	if( (wStatus)!=0x01)
		{
			return EFC_FAIL_RPRG;
		}
	
	EFC_GetStatusFlag(wStatus);
	EFC_ClrStatusFlag(wStatus);
	EFC_SetOPRMODE(EFC_OPRMODE_ROW_PRG);
	REG32(iAddr+256) = 0;
	do{
		EFC_GetStatusFlag(wStatus);
		}
	while(!wStatus);
	if( (wStatus)!=0x01)
		{
			return EFC_FAIL_RPRG;
		}
	return EFC_SUCCESS;	
}


eReturnType_Def EFC_PageErase(int iAddr)     //eeprom 1 word
{
	ASSERT( iAddr<EFC_FLASH_ADDR_BASE);
	ASSERT( (iAddr>=EFC_FLASH_ADDR_END)&&(iAddr<EFC_NVR_ADDR_BASE));
	ASSERT( (iAddr>=EFC_NVR_ADDR_END)&&(iAddr<EFC_EEPROM_BASE));
	ASSERT( iAddr >= EFC_EEPROM_END);
	uint32_t wStatus;
	EFC_GetStatusFlag(wStatus);
	EFC_ClrStatusFlag(wStatus);
	EFC_SetOPRMODE(EFC_OPRMODE_PAGE_ERASE);
	REG32(iAddr) = 0;
	do{
		EFC_GetStatusFlag(wStatus);
		}
	while(!wStatus);
	EFC_ClrStatusFlag(wStatus);	
	if( (wStatus)!=0x01)
		{
			return EFC_FAIL_PAGE_ERASE;
		}
	return EFC_SUCCESS;		
}



eReturnType_Def EFC_ChipErase(int iAddr)           //nvr addr:main+nvr1-7, no nvr8
{
	ASSERT( iAddr<EFC_FLASH_ADDR_BASE);
	ASSERT( (iAddr>=EFC_FLASH_ADDR_END)&&(iAddr<EFC_NVR_ADDR_BASE));
	ASSERT( (iAddr>=EFC_NVR_ADDR_END)&&(iAddr<EFC_EEPROM_BASE));
	ASSERT( iAddr >= EFC_EEPROM_END);
	uint32_t wStatus;
	EFC_GetStatusFlag(wStatus);
	EFC_ClrStatusFlag(wStatus);
	EFC_SetOPRMODE(EFC_OPRMODE_CHIP_ERASE);
	REG32(iAddr) = 0;
	do{
		EFC_GetStatusFlag(wStatus);
		}
	while(!wStatus);
	if( (wStatus)!=0x01)
		{
			return EFC_FAIL_CHIP_ERASE;
		}
	return EFC_SUCCESS;		
}


eReturnType_Def EFC_PageLoad(int iAddr)
{
	ASSERT( iAddr<EFC_FLASH_ADDR_BASE);
	ASSERT( (iAddr>=EFC_FLASH_ADDR_END)&&(iAddr<EFC_NVR_ADDR_BASE));
	ASSERT( iAddr>=EFC_NVR_ADDR_END);	
	uint32_t wStatus;
	EFC_GetStatusFlag(wStatus);
	EFC_ClrStatusFlag(wStatus);
	EFC_SetOPRMODE(EFC_OPRMODE_PAGE_LOAD);
	REG32(iAddr) = 0;
	do{
		EFC_GetStatusFlag(wStatus);
		}
	while(!wStatus);
	if( (wStatus)!=0x01)
		{
			return EFC_FAIL_RPRG;
		}
	return EFC_SUCCESS;	
}


eReturnType_Def EFC_PageEraseVerify(int iAddr)
{
	ASSERT( iAddr<EFC_FLASH_ADDR_BASE);
	ASSERT( (iAddr>=EFC_FLASH_ADDR_END)&&(iAddr<EFC_NVR_ADDR_BASE));
	ASSERT( iAddr>=EFC_NVR_ADDR_END);	
	uint32_t wStatus;
	EFC_GetStatusFlag(wStatus);
	EFC_ClrStatusFlag(wStatus);
	EFC_SetOPRMODE(EFC_OPRMODE_PAGE_ERASE_VERIFY);
	REG32(iAddr) = 0;
	do{
		EFC_GetStatusFlag(wStatus);
		}
	while(!wStatus);
	if( (wStatus)!=0x01)
		{
			return EFC_FAIL_EV;
		}
	return EFC_SUCCESS;	
}


eReturnType_Def EFC_PageProgramVerify(int iAddr)
{
	ASSERT( iAddr<EFC_FLASH_ADDR_BASE);
	ASSERT( (iAddr>=EFC_FLASH_ADDR_END)&&(iAddr<EFC_NVR_ADDR_BASE));
	ASSERT( iAddr>=EFC_NVR_ADDR_END);	
	uint32_t wStatus;
	EFC_GetStatusFlag(wStatus);
	EFC_ClrStatusFlag(wStatus);
	EFC_SetOPRMODE(EFC_OPRMODE_PAGE_PRG_VERIFY);
	REG32(iAddr) = 0;
	do{
		EFC_GetStatusFlag(wStatus);
		}
	while(!wStatus);
	if( (wStatus)!=0x01)
		{
			return EFC_FAIL_PV;
		}
	return EFC_SUCCESS;	
}



eReturnType_Def EFC_FlashWrite(int iAddr,uint32_t *pwData,uint32_t wWordNum,uint32_t wWordPrg)
{
	ASSERT( iAddr<EFC_FLASH_ADDR_BASE);
	ASSERT( iAddr>=EFC_FLASH_ADDR_END);
	ASSERT(wWordNum > EFC_PAGE_SIZE);
	ASSERT( (iAddr&0x03)!=0);
	if( iAddr+ (wWordNum<<2) > EFC_FLASH_SIZE+EFC_FLASH_ADDR_BASE)
		return EFC_FAIL_FLASH_ADDR_OV;
	EFC_PageErase(iAddr);
	if(wWordPrg == 1)
		{
			int i;
			for(i=0;i<wWordNum;++i)
				{
					if(EFC_SingleProgram(EFC_PRG_WORD,iAddr+(i<<2),*pwData) != EFC_SUCCESS)
						return EFC_FAIL_FLASH_WR;
					if( REG32(iAddr+(i<<2)) != *pwData)
						return EFC_FAIL_FLASH_VF;
					++pwData;
				}
		}
	else
		{
			int i;
			for(i=0;i<wWordNum;++i)
				{
					REG32(EFC_PAGE_BUFF_BASE+(i<<2)) = *pwData++;
				}
			if( EFC_PageProgram(iAddr) != EFC_SUCCESS)
				return EFC_FAIL_FLASH_WR;
			if(EFC_PageProgramVerify(iAddr) != EFC_SUCCESS)
				return EFC_FAIL_FLASH_VF;
		}
	return EFC_SUCCESS;
}



void EFC_IrqHandler(void)
{
	uint32_t wStatus;
	EFC_GetStatusFlag(wStatus);
	cbEFCIntFunc(wStatus);
	EFC_ClrStatusFlag(wStatus);
}


