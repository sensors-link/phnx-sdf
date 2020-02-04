#include "FH_phoenix01.h"
#include "sysc.h"



#define   FPCLK      8000000


#if   1

void DelayNus(uint32_t  wDel)
{
	uint32_t wCnt;
    SYSC_SetCLKENCFG(SYSC_CLKENCFG_TIMER_PCKEN | SYSC_CLKENCFG_TIEMR4_PCKEN);
	TIMER3->CR = 0;
	wCnt = wDel * (FPCLK/1000000);
	TIMER3->LC1R = wCnt;
	TIMER3->CR = 0x01;
	wCnt = 1000000000;
	while( ((TIMER3->SISR & (1<<0) ) == 0) && (--wCnt > 0) );
	wCnt = TIMER3->SICR;
}

#else 

void DelayInit(void)
{
	SYSC_SetCLKENCFG(SYSC_CLKENCFG_TIMER_PCKEN | SYSC_CLKENCFG_TIEMR4_PCKEN);
	TIMER3->CR = 0;
	TIMER3->LC1R = 0;
	TIMER3->CR = 0x05;
}

void DelayNus(uint32_t wDel)      //  Fpclk=32M  max:134s       >10us
{
	uint32_t wTmp1,wTmp2,wTmp3,wCnt;
	wCnt = wDel * (FPCLK/1000000);
	__disable_irq();
	wTmp1 = TIMER3->CVR;
	__enable_irq();
	do{
		__disable_irq();
		wTmp2 = TIMER3->CVR;
		__enable_irq();
		wTmp3 = wTmp1-wTmp2;
		if( __get_APSR() & (1<<29))
		{
			wTmp3 = wTmp1 + (0xffffffff-wTmp2);
		}
	}while( wTmp3<wCnt);
}

void DelayNCount(uint32_t wDel)      //ms  Fpclk=32M  max:134s
{
	uint32_t wTmp1,wTmp2,wTmp3;
	__disable_irq();
	wTmp1 = TIMER3->CVR;
	__enable_irq();
	do{
		__disable_irq();
		wTmp2 = TIMER3->CVR;
		__enable_irq();
		wTmp3 = wTmp1-wTmp2;
		if( __get_APSR() & (1<<29))
		{
			wTmp3 = wTmp1 + (0xffffffff-wTmp2);
		}
	}while( wTmp3<wDel);
}
#endif

