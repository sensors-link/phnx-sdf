#include "FH_phoenix01.h"
#include "assert.h"
#include "anac.h"




__inline void ANAC_LedCfg(uint32_t wRegVal)
{
	ANAC_WRPROTECT_CLOSE();
	ANAC->LED_CFG = wRegVal;
}

__inline void ANAC_PDsensCfg(uint32_t wRegVal)
{
	ANAC_WRPROTECT_CLOSE();
	ANAC->PDSENS_CFG = wRegVal;
}

__inline void ANAC_DCCfg(uint32_t wRegVal)
{
	ANAC_WRPROTECT_CLOSE();
	ANAC->DC_CFG = wRegVal;
}

__inline void ANAC_CMPCfg(uint32_t wRegVal)
{
	ANAC_WRPROTECT_CLOSE();
	ANAC->DC_CFG = wRegVal;
}

__inline void ANAC_ADCCfg(uint32_t wRegVal)
{
	ANAC_WRPROTECT_CLOSE();
	ANAC->ADC_CFG = wRegVal;
}

__inline void ANAC_ANACCfg(uint32_t wRegVal)
{
	ANAC_WRPROTECT_CLOSE();
	ANAC->ANAC_CFG = wRegVal;
}

__inline void ANAC_LVDCfg(uint32_t wRegVal)
{
	ANAC_WRPROTECT_CLOSE();
	ANAC->LVD_CFG = wRegVal;
}

__inline void ANAC_CLKCfg(uint32_t wRegVal)
{
	ANAC_WRPROTECT_CLOSE();
	ANAC->CLK_CFG = wRegVal;
}

__inline void ANAC_DLYCfg(uint32_t wRegVal)
{
	ANAC_WRPROTECT_CLOSE();
	ANAC->DLY_CFG = wRegVal;
}

__inline uint32_t ANAC_GetADCValue(uint32_t wChnNo)
{
	volatile uint32_t * pReg = &(ANAC->ADC_VAL0);
	ASSERT(wChnNo>7);
	return pReg[wChnNo];
}

__inline uint32_t ANAC_GetTempValue(uint32_t wChnNo)
{
	volatile uint32_t * pReg = &(ANAC->TEMP_VAL0);
	ASSERT(wChnNo>7);
	return pReg[wChnNo];	
}

__inline uint32_t ANAC_GetDarkCurValue(uint32_t wChnNo)
{
	volatile uint32_t * pReg = &(ANAC->DARKC_VAL0);
	ASSERT(wChnNo>7);
	return pReg[wChnNo];	
}

__inline uint32_t ANAC_GetSmokeValue(uint32_t wChnNo)
{
	volatile uint32_t * pReg = &(ANAC->SMOKE_VAL0);
	ASSERT(wChnNo>7);
	return pReg[wChnNo];	
}


