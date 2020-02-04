
/*****************************************
 * @file        wdt.c
 * @brief       wdt source file
 * @version     v0.0
 * @date        2019/05/06
 * @author      tangbifei
 * @note      应用于Keil平台
 * Copyright(C) 2019,  FanHai Data Tech Corp.
 * All rights reserved
 ******************************************/
#include "FH_phoenix01.h"
#include "wdt.h"
#include "assert.h"


// static varible declare
static WDTSignalEvent_Type cbWDTInt;



__inline void WDT_SetOverTime(uint32_t wVal)
{
    ASSERT( ( wVal & ~WDT_WDTCR_WOV_msk) != 0);
    WDT->WDTCR &= ~WDT_WDTCR_WOV_msk;
    WDT->WDTCR |= wVal;
}


__inline void WDT_SetMode(uint32_t wVal)
{
    ASSERT( wVal > 1);
    WDT->WDTCR &= ~(1<<WDT_WDTCR_WINTEN_pos);
    WDT->WDTCR |= wVal<<WDT_WDTCR_WINTEN_pos;
}


__inline uint32_t WDT_GetFlag(void)
{
    return WDT->WDTCR;
}

void WDT_Init(uint32_t wDiv,uint32_t wMode,WDTSignalEvent_Type cbFunc)
{
    WDT_SetOverTime(wDiv);
    WDT_SetMode(wMode);
    if(wMode == WDT_WDTCR_INT_MODE)
    {
    	ASSERT(cbFunc == NULL);
		cbWDTInt = cbFunc;
    }	
}
void NMI_Handler(void)
{
    uint32_t wStatus = WDT_GetFlag();
    cbWDTInt(wStatus);
	WDT_ClrCnt();
}



