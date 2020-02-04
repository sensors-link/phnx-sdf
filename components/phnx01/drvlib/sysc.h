/*****************************************
 * @file            sysc.h
 * @brief           system control Model Header file
 * @version         v0.0
 * @date            2019/05/06
 * @author          tangbifei
 * @note      应用于Keil平台
 * Copyright(C) 2019,  FanHai Data Tech Corp.
 * All rights reserved
 ******************************************/


#ifndef  _SYSC_
#define  _SYSC_


#include <stddef.h>
#include <stdint.h>


/*
BASE ADDR:0x4001_2000  
  reg          pos    r/w type bits      reset vlaue
CLKCTRCFG	0x00	R/W	32	0xA900_0000	SYSC系统时钟控制寄存器
WRPROCFG	0x04	W	16	0x0	SYSC系统保护寄存器
CLKENCFG	0x08	R/W	17	0x6800	SYSC模块时钟门控寄存器
MSFTRSTCFG	0x0C	W	9	0x0	SYSC模块软复位寄存器
NVCLKDIV	0x10	R/W	18	0x3_CF0F	SYSC EFC时钟分频寄存器
TESTCKSEL	0x14	R/W	8	0xFF	SYSC测试时钟输出选择寄存器
ANCLKDIV	0x18	R/W	8	0xF0	SYSC ANAC时钟分频寄存器

*/

//SYSCTRCFG      //protect
#define  SYSC_SYSCTRCFG_SYS_CLK_SEL_HRC     (0<<4)     //def
#define  SYSC_SYSCTRCFG_SYS_CLK_SEL_LRC     (1<<4)
#define  SYSC_SYSCTRCFG_SYS_CLK_SEL_XTH     (2<<4)
#define  SYSC_SYSCTRCFG_SYS_CLK_SEL_XTL     (3<<4)


#define  SYSC_SYSCTRCFG_SYS_CLK_DIV_7BIT(x)      (x<<8)          //  fsrc/(div+1) 

#define  SYSC_SYSCTRCFG_EFC_BUSY_CLOSE_CPU_CLK    (1<<15)

#define  SYSC_SYSCTRCFG_APB_CLK_DIV_7BIT(x)      (x<<16)        // fsrc/(div+1)

#define  SYSC_SYSCTRCFG_DEEPSLEEP_WKUP_DLY_9BIT(x)    (x<<23)


#define  SYSC_SetSYSCTRCFG(x)          {SYSC->SYSCTRCFG |= x;}
#define  SYSC_ClrSYSCTRCFG(x)          {SYSC->SYSCTRCFG &=~x;}



//WRPROCFG
#define  SYSC_WRPROTECT_CLOSE()   {uint32_t tmp = __get_PRIMASK(); __set_PRIMASK(1);SYSC->WRPROCFG = 0x5a5a;SYSC->WRPROCFG = 0xa5a5;__set_PRIMASK(tmp);}

//CLKENCFSYSC_G
#define  SYSC_CLKENCFG_EST_PCKEN        (1ul<<0)
#define  SYSC_CLKENCFG_I2C_PCKEN        (1ul<<1)
#define  SYSC_CLKENCFG_SPI_PCKEN        (1ul<<2)
#define  SYSC_CLKENCFG_UART1_PCKEN      (1ul<<3)
#define  SYSC_CLKENCFG_UART2_PCKEN      (1ul<<4)
#define  SYSC_CLKENCFG_TIMER_PCKEN      (1ul<<5)
#define  SYSC_CLKENCFG_TIMER1_PCKEN     (1ul<<6)
#define  SYSC_CLKENCFG_TIMER2_PCKEN     (1ul<<7)
#define  SYSC_CLKENCFG_TIMER3_PCKEN     (1ul<<8)
#define  SYSC_CLKENCFG_TIEMR4_PCKEN     (1ul<<9)
#define  SYSC_CLKENCFG_IOM_PCKEN        (1ul<<10)
#define  SYSC_CLKENCFG_DCLK_HCKEN       (1ul<<11)  //def:1
#define  SYSC_CLKENCFG_ANAC_PCKEN       (1ul<<12)  
#define  SYSC_CLKENCFG_PMU_PCKEN        (1ul<<13)  //def:1
#define  SYSC_CLKENCFG_LPWDT_PCKEN      (1ul<<14)  //def:1
#define  SYSC_CLKENCFG_LPTIM_PCKEN      (1ul<<15)
#define  SYSC_CLKENCFG_CRC_HCKEN        (1ul<<16)


#define  SYSC_SetCLKENCFG(x)          {SYSC->CLKENCFG |= x;}
#define  SYSC_ClrCLKENCFG(x)          {SYSC->CLKENCFG &=~x;}


//MSFTRSTCFG  protect
#define  SYSC_MSFTRSTCFG_I2C           (1ul<<0)
#define  SYSC_MSFTRSTCFG_SPI           (1ul<<1)
#define  SYSC_MSFTRSTCFG_UART1         (1ul<<2)
#define  SYSC_MSFTRSTCFG_UART2         (1ul<<3)
#define  SYSC_MSFTRSTCFG_TIMER         (1ul<<4)
#define  SYSC_MSFTRSTCFG_CRC           (1ul<<5)
#define  SYSC_MSFTRSTCFG_HCLK          (1ul<<6)
#define  SYSC_MSFTRSTCFG_ANAC          (1ul<<7)
#define  SYSC_MSFTRSTCFG_EST           (1ul<<8)

#define  SYSC_SetMSFTRSTCFG(x)      {SYSC->MSFTRSTCFG = x;}

//NVCLKDIV
#define    SYSC_NVCLKDIV_DIV_TO_2M_5BIT(x)       (x<<0)    //fsrc = 32M  def:0xf      fsrc/(div+1) no div=0 
#define    SYSC_NVCLKDIV_DIV_TO_2K_10BIT(x)       (x<<8)    //fsrc = 2M   def:3ff      fsrc/(div+1) no div=0

#define  SYSC_SetNVCLKDIV(x)      {SYSC->NVCLKDIV |= x;}

//TESTCKSEL
#define   SYSC_TESTCKSEL_EN            (1<<7)
#define   SYSC_TESTCKSEL_DIV_7BIT(x)   (x<<0)

#define  SYSC_SetTESTCKSEL(x)         {SYSC->TESTCKSEL |= x;}

//ANCLKDIV
#define  SYSC_ANCLKDIV_TO_500K_5BIT(x)         (x<<4)           // fsrc/(div+1)  no div=0
#define  SYSC_ANCLKDIV_SCLK_DIV_4BIT(x)        (x<<0)          //fsrc/(2*(div+1)) 

#define  SYSC_SetANCLKDIV(x)       {SYSC->ANCLKDIV |= x;}


extern uint32_t gdwCoreClk;

//user verify xth
#define XTAL      32000000


#define F_XTH   -1
#define F_XTL    0
#define F_HRC1M  1
#define F_HRC2M  2
#define F_HRC4M  3
#define F_HRC8M  4
#define F_HRC16M 5
#define F_HRC24M 6
#define F_HRC32M 7
#define F_LRC    8


//user configure  
#define  SYSC_CLK_SRC_SEL  F_HRC8M

enum _DIV{
	DIV1 = 1,
	DIV2,
	DIV3,
	DIV4,
	DIV5,
	DIV6,
	DIV7,
	DIV8,
	DIV9,
	DIV10,
	DIV11,
	DIV12,
	DIV13,
	DIV14,
	DIV15,
	DIV16,
	DIV17,
	DIV18,
	DIV19,
	DIV20,
	DIV21,
	DIV22,
	DIV23,
	DIV24,
	DIV25,
	DIV26,
	DIV27,
	DIV28,
	DIV29,
	DIV30,
	DIV31,
	DIV32,
	DIV33,
	DIV34,
	DIV35,
	DIV36,
	DIV37,
	DIV38,
	DIV39,
	DIV40,
	DIV41,
	DIV42,
	DIV43,
	DIV44,
	DIV45,
	DIV46,
	DIV47,
	DIV48,
	DIV49,
	DIV50,
	DIV51,
	DIV52,
	DIV53,
	DIV54,
	DIV55,
	DIV56,
	DIV57,
	DIV58,
	DIV59,
	DIV60,
	DIV61,
	DIV62,
	DIV63,
	DIV64,
	DIV65,
	DIV66,
	DIV67,
	DIV68,
	DIV69,
	DIV70,
	DIV71,
	DIV72,
	DIV73,
	DIV74,
	DIV75,
	DIV76,
	DIV77,
	DIV78,
	DIV79,
	DIV80,
	DIV81,
	DIV82,
	DIV83,
	DIV84,
	DIV85,
	DIV86,
	DIV87,
	DIV88,
	DIV89,
	DIV90,
	DIV91,
	DIV92,
	DIV93,
	DIV94,
	DIV95,
	DIV96,
	DIV97,
	DIV98,
	DIV99,
	DIV100,
	DIV101,
	DIV102,
	DIV103,
	DIV104,
	DIV105,
	DIV106,
	DIV107,
	DIV108,
	DIV109,
	DIV110,
	DIV111,
	DIV112,
	DIV113,
	DIV114,
	DIV115,
	DIV116,
	DIV117,
	DIV118,
	DIV119,
	DIV120,
	DIV121,
	DIV122,
	DIV123,
	DIV124,
	DIV125,
	DIV126,
	DIV127,
	DIV128,
};              
//user configure  
#define SYSC_HCLK_DIV     DIV1
#define SYSC_PCLK_DIV     DIV1     



#define  SYSC_GetHCLK()       (gdwCoreClk/SYSC_HCLK_DIV)
#define  SYSC_GetPCLK()       (gdwCoreClk/SYSC_HCLK_DIV/SYSC_PCLK_DIV)

//extern function declare
void CoreClkUpdate(void);

#endif
