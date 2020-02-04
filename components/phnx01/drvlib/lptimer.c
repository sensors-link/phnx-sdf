
/*****************************************
 * @file        lptimer.c
 * @brief       lptimer source file
 * @version     v0.0
 * @date        2019/05/06
 * @author      tangbifei
 * @note      ???Keil??
 * Copyright(C) 2019,  FanHai Data Tech Corp.
 * All rights reserved
 ******************************************/
#include "FH_phoenix01.h"
#include "assert.h"
#include "lptimer.h"



static LPTSignalEvent_Type cbLptIntFunc;


uint16_t LPT_GetCnt(void)
{
    int i;
    uint16_t hwVal1,hwVal2;
    for(i=0;i<3;++i)
    {
	hwVal1 = LPT->LPTCNT;
	hwVal2 = LPT->LPTCNT;
	if(hwVal1 == hwVal2)
	    break;
    }
    if(i<3)
	return hwVal1;
    else
	return 0;
}

__inline uint32_t LPT_GetIntFlag(void)
{
    return LPT->LPTINTSTS;
}



void LPT_Init(uint32_t wDiv,uint32_t wCntMode,uint32_t wIntEn,LPTSignalEvent_Type cbFunc)
{
	ASSERT( wDiv > 0xffff);
	ASSERT( (wCntMode==LPT_CNTMODE_PIT)?(wDiv<3):(wDiv==0));
	ASSERT(wCntMode > 1);
	ASSERT(wIntEn > 1);
	LPT->LPTCR = 0;
	LPT->LPTCFG = wDiv;
	LPT_SetLPTCR( (wCntMode<<LPT_LPTCR_PITE_pos) | (wIntEn<<LPT_LPTCR_IE_pos));
	if( wIntEn == LPT_INT_EN)
		{
			cbLptIntFunc = cbFunc;
		}
	LPT_SetLPTCR( 1<<LPT_LPTCR_LPTEN_pos );
}


void LPT_IrqHandler(void)
{
	uint32_t wStatus = LPT_GetIntFlag();
	cbLptIntFunc(wStatus);
	LPT_ClrIntFlag();
}


