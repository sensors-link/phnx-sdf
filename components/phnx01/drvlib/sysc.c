#include "FH_phoenix01.h"
#include "assert.h"
#include "sysc.h"
#include "anac.h"
#include "pmu.h"
#include "efc.h"



uint32_t gdwCoreClk = 8000000;



void SystemInit(void)
{

#if (SYSC_CLK_SRC_SEL == F_XTH)
	SYSC_SetCLKENCFG(SYSC_CLKENCFG_ANAC_PCKEN);
	ANAC_WRPROTECT_CLOSE();
	ANAC->CLK_CFG |= ANAC_CLK_CFG_XTH_EN;
	for(i=10000;i>0;i--);
#if (XTAL == 32000000)
	EFC_WRPROTECT_CLOSE();
	EFC->EFC_CR |= (1<<EFC_CR_NRDDLY_pos);
#endif
	SYSC_WRPROTECT_CLOSE();
	SYSC_SetSYSCTRCFG( SYSC_SYSCTRCFG_APB_CLK_DIV_7BIT(SYSC_PCLK_DIV) | SYSC_SYSCTRCFG_SYS_CLK_DIV_7BIT(SYSC_HCLK_DIV) | SYSC_SYSCTRCFG_SYS_CLK_SEL_XTH  );
	
#elif (SYSC_CLK_SRC_SEL == F_XTL)
	SYSC_SetCLKENCFG(SYSC_CLKENCFG_IOM_PCKEN);
	IOM->GPIO_PU &= ~(3<<2);
	IOM->GPIO_ADS |= (3<<2);
	SYSC_SetCLKENCFG(SYSC_CLKENCFG_ANAC_PCKEN);
	ANAC_WRPROTECT_CLOSE();
	ANAC->CLK_CFG |= ANAC_CLK_CFG_XTL_XTL_EN;
	for(i=10000;i>0;i--);	
	SYSC_WRPROTECT_CLOSE();
	SYSC_SetSYSCTRCFG( SYSC_SYSCTRCFG_APB_CLK_DIV_7BIT(SYSC_PCLK_DIV) | SYSC_SYSCTRCFG_SYS_CLK_DIV_7BIT(SYSC_HCLK_DIV) | SYSC_SYSCTRCFG_SYS_CLK_SEL_XTL  );
#elif (SYSC_CLK_SRC_SEL == F_LRC)
	SYSC_SetSYSCTRCFG( SYSC_SYSCTRCFG_APB_CLK_DIV_7BIT(SYSC_PCLK_DIV) | SYSC_SYSCTRCFG_SYS_CLK_DIV_7BIT(SYSC_HCLK_DIV) | SYSC_SYSCTRCFG_SYS_CLK_SEL_LRC  );
#else 
	ANAC_WRPROTECT_CLOSE();
	ANAC->CLK_CFG |= (SYSC_CLK_SRC_SEL)<<2;
	//load trim value ....
	
	//def hrc  set div
	SYSC_SetSYSCTRCFG( SYSC_SYSCTRCFG_APB_CLK_DIV_7BIT(SYSC_PCLK_DIV) | SYSC_SYSCTRCFG_SYS_CLK_DIV_7BIT(SYSC_HCLK_DIV) );
#endif
	CoreClkUpdate();

}



void CoreClkUpdate(void)
{
	uint32_t dwTmp;
#if (SYSC_CLK_SRC_SEL == F_XTH)
	gdwCoreClk = XTAL;
#elif (SYSC_CLK_SRC_SEL == F_XTL)
	gdwCoreClk = 32768;
#elif (SYSC_CLK_SRC_SEL == F_LRC)
	gdwCoreClk = 8000;
#else
	dwTmp = ANAC->CLK_CFG & (0x07<<2);
	if((dwTmp >> 2) == 1)
		gdwCoreClk = 1000000;
	else if((dwTmp >> 2) == 2)
		gdwCoreClk = 2000000;
	else if((dwTmp >> 2) == 3)
		gdwCoreClk = 4000000;
	else if((dwTmp >> 2) == 4)
		gdwCoreClk = 8000000;
	else if((dwTmp >> 2) == 5)
		gdwCoreClk = 16000000;
	else if((dwTmp >> 2) == 6)
		gdwCoreClk = 24000000;
	else if((dwTmp >> 2) == 7)
		gdwCoreClk = 32000000;
#endif


}

