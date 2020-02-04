/*****************************************
 * @file        lptimer.h
 * @brief       lptimer header file
 * @version     v0.0
 * @date        2019/05/06
 * @author      tangbifei
 * @note      ???Keil??
 * Copyright(C) 2019,  FanHai Data Tech Corp.
 * All rights reserved
 ******************************************/


#ifndef _LPTIMER_

#define _LPTIMER_
#include <stdint.h>
#include <stddef.h>



//lptcr
#define  LPT_LPTCR_LPTEN_pos       0
#define  LPT_LPTCR_PITE_pos        2
#define  LPT_LPTCR_IE_pos          3

#define  LPT_CNTMODE_PIT      1
#define  LPT_CNTMODE_NORMAL    0

#define  LPT_INT_EN        1
#define  LPT_INT_DIS       0

#define  LPT_SetLPTCR(x)     {LPT->LPTCR |= x;}
#define  LPT_ClrLPTCR(x)     {LPT->LPTCR &= x;}



//lptcfg

#define  LPT_LPTCFG_CFG_16BIT(x)     (x<<0) //pit >3  def:0xfff0

#define  LPT_ClrIntFlag()      {LPT->LPTINTSTS = 1;}

typedef void (*LPTSignalEvent_Type)(uint32_t wEvent);     



uint16_t LPT_GetCnt(void);
void LPT_Init(uint32_t wDiv,uint32_t wCntMode,uint32_t wIntEn,LPTSignalEvent_Type cbFunc);






















#endif
