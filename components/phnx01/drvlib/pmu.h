/*****************************************
 * @file        pmu.h
 * @brief       pmu header file
 * @version     v0.0
 * @date        2019/05/06
 * @author      tangbifei
 * @note      应用于Keil平台
 * Copyright(C) 2019,  FanHai Data Tech Corp.
 * All rights reserved
 ******************************************/

#ifndef _PMU_H_

#define  _PMU_H_
#include <stddef.h>
#include <stdint.h>


//pmu_cr     pmu_wpt
#define  PMU_CR_PDEN               0x0a      
#define  PMU_CR_LVDCHIPRSTEN_pos    4

#define  PMU_CR_LPCLKSEL_pos        5
#define  PMU_CR_LPCLKSEL_LRC        0
#define  PMU_CR_LPCLKSEL_XTL        1

#define  PMU_CR_LPCLKEN_pos          6
#define  PMU_CR_WDTCLKEN_pos         7

#define  PMU_CR_VDDLPCFG_0_8V        3
#define  PMU_CR_VDDLPCFG_1_0V        2
#define  PMU_CR_VDDLPCFG_1_2V        1
#define  PMU_CR_VDDLPCFG_1_5V        0

#define  PMU_CR_VDDLPCFG_pos         8


#define  PMU_SetPMU_CR(x)        {PMU->PMU_CR |= x;}
#define  PMU_ClrPMU_CR(x)        {PMU->PMU_CR &=~(x);}



//pmu_wkcfg
#define  PMU_WKCFG_GPIOWKEN_GPIO16_pos   0
#define  PMU_WKCFG_GPIOWKEN_GPIO17_pos   1
#define  PMU_WKCFG_GPIOWKEN_GPIO18_pos   2
#define  PMU_WKCFG_GPIOWKEN_GPIO19_pos   3

#define  PMU_WKCFG_GPIOWKLEV_GPIO16_pos   4     //powerdown only low level wk
#define  PMU_WKCFG_GPIOWKLEV_GPIO17_pos   5
#define  PMU_WKCFG_GPIOWKLEV_GPIO18_pos   6
#define  PMU_WKCFG_GPIOWKLEV_GPIO19_pos   7

#define  PMU_WKMODE_pos                   9
#define  PMU_WKMODE_SYNC                  0
#define  PMU_WKMODE_ASYNC                 1  //only deepsleep wk

#define  PMU_SetWKMODE(x)        {PMU->PMU_WKCFG |= x;}
#define  PMU_ClrWKMODE(x)        {PMU->PMU_WKCFG &=~(x);}

//pmu_softrst  pmu_wpt protect

#define  PMU_SoftDigtalReset()     {PMU->PMU_SOFTRST = 0x02;}
#define  PMU_SoftChipReset()       {PMU->PMU_SOFTRST = 0x01;}


//pmu_wksts
#define  PMU_WKSTS_GPIO16         (1<<0)
#define  PMU_WKSTS_GPIO17         (1<<1)
#define  PMU_WKSTS_GPIO18         (1<<2)
#define  PMU_WKSTS_GPIO19         (1<<3)
#define  PMU_WKSTS_WDT         (1<<4)
#define  PMU_WKSTS_LPT         (1<<5)

#define  PMU_ClrWKStatusFlag(x)      {PMU->PMU_WKSTS = x;}
#define  PMU_GetWKStatusFlag(x)      {x = PMU->PMU_WKSTS;}

//pmu_rststs

#define PMU_RSTSTS_LVD             (1<<0)
#define PMU_RSTSTS_WDT             (1<<1)
#define PMU_RSTSTS_EXTRST          (1<<2)
#define PMU_RSTSTS_SOFTCHIP        (1<<3)
#define PMU_RSTSTS_SOFTDIGIST      (1<<4)
#define PMU_RSTSTS_POR             (1<<5)

#define  PMU_ClrRSTStatusFlag(x)      {PMU->PMU_RSTSTS = x;}
#define  PMU_GetRSTStatusFlag(x)      {x = PMU->PMU_RSTSTS;}


//pmu_inten
#define  PMU_INTEN_LVDRST_pos         0
#define  PMU_INTEN_WDTRST_pos         1
#define  PMU_INTEN_EXTRST_pos         2
#define  PMU_INTEN_SOFTCHIP_pos       3
#define  PMU_INTEN_SOFTDIG_pos        4
#define  PMU_INTEN_PORRST_pos         5
#define  PMU_INTEN_WDTWK_pos          6
#define  PMU_INTEN_LPTWK_pos          7
#define  PMU_INTEN_GPIO16_pos         8
#define  PMU_INTEN_GPIO17_pos         9
#define  PMU_INTEN_GPIO18_pos         10
#define  PMU_INTEN_GPIO19_pos         11
 

#define  PMU_SetPMU_INTEN(x)        {PMU->PMU_INTEN |= x;}
#define  PMU_ClrPMU_INTEN(x)        {PMU->PMU_INTEN &=~(x);}



//pmu_wpt
#define  PMU_WRPROTECT_CLOSE()       {uint32_t tmp = __get_PRIMASK(); __set_PRIMASK(1); PMU->PMU_WPT = 0xc3;PMU->PMU_WPT = 0x3c;__set_PRIMASK(tmp);}








#endif
