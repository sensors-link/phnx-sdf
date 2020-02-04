/*****************************************
 * @file        wdt.h
 * @brief       wdt header file
 * @version     v0.0
 * @date        2019/05/06
 * @author      tangbifei
 * @note      应用于Keil平台
 * Copyright(C) 2019,  FanHai Data Tech Corp.
 * All rights reserved
 ******************************************/


#ifndef  _WDT_
#define _WDT_
#include <stddef.h>
#include <stdint.h>


#define   WDT_ClrCnt()     { int32_t wTmp = __get_PRIMASK();__set_PRIMASK(1);WDT->WDTRST = 0x69;WDT->WDTRST = 0x96;__set_PRIMASK(wTmp);}  


//wdtcr                                                                      
#define  WDT_WDTCR_WOV_4BIT(x)       ( x<<0)     
#define  WDT_WDTCR_WDTR_FLAG         (1ul<<4)
#define  WDT_WDTCR_WINTEN_pos        5
#define  WDT_WDTCR_INT_MODE        1
#define  WDT_WDTCR_RST_MODE        0

#define  WDT_WDTCR_WOV_msk      0x0f

#define  WDT_WDTCR_WDTINTFLAG        (1ul<<7)

typedef void (*WDTSignalEvent_Type)(uint32_t wEvent);


//function declare

void WDT_Init(uint32_t wDiv,uint32_t wMode,WDTSignalEvent_Type cbFunc);




#endif
