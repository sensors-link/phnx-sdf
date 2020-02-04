/*****************************************
 * @file            iom.h
 * @brief           GPIO Model Header file
 * @version         v0.0
 * @date            2019/05/06
 * @author          tangbifei
 * @note      应用于Keil平台
 * Copyright(C) 2019,  FanHai Data Tech Corp.
 * All rights reserved
 ******************************************/

#ifndef _IOM_
#define _IOM_

#ifdef __cplusplus

extern "C" {
	
#endif
	
#include <stddef.h>
#include <stdint.h>


#define  GPIO_P00       0
#define  GPIO_P01       1
#define  GPIO_P02       2
#define  GPIO_P03       3
#define  GPIO_P04       4
#define  GPIO_P05       5
#define  GPIO_P06       6
#define  GPIO_P07       7
#define  GPIO_P08       8
#define  GPIO_P09       9
#define  GPIO_P10      10
#define  GPIO_P11      11
#define  GPIO_P12      12
#define  GPIO_P13      13
#define  GPIO_P14      14
#define  GPIO_P15      15
#define  GPIO_P16      16
#define  GPIO_P17      17
#define  GPIO_P18      18
#define  GPIO_P19      19
                

#define  GPIO_DATA_msk    0x000fffff

//@{ gpio register declare start
//GPIO_DATA
#define  GPIO_HIGH   1
#define  GPIO_LOW    0

//GPIO_DIR
#define  GPIO_DIR_IN        0
#define  GPIO_DIR_OUT       1

//GPIO_PU
#define  GPIO_PU_ENABLE      1
#define  GPIO_PU_DISENALE    0  

//GPIO_PD
#define  GPIO_PD_ENABLE      1
#define  GPIO_PD_DISENALE    0  

//GPIO_OTYPE
#define  GPIO_OTYPE_OPEN_D      1
#define  GPIO_OTYPE_CMOS        0  

//GPIO_ADS
#define  GPIO_ADS_SEL_ANALOG    1
#define  GPIO_ADS_SEL_DIGITAL   0

//GPIO_DRS
#define  GPIO_DRS_SEL_STRONG    1
#define  GPIO_DRS_SEL_WEAK      0

//GPIO_AF0
#define  GPIO_AF0_P00_SEL_GPIO            (0ul<<0)
#define  GPIO_AF0_P00_SEL_UART1_RX        (1ul<<0) //def

#define  GPIO_AF0_P01_SEL_GPIO            (0ul<<2)
#define  GPIO_AF0_P01_SEL_UART1_TX        (1ul<<2) //def 

#define  GPIO_AF0_P02_SEL_GPIO            (0ul<<4)
#define  GPIO_AF0_P02_SEL_UART2_RX        (1ul<<4) //def

#define  GPIO_AF0_P03_SEL_GPIO            (0ul<<6)
#define  GPIO_AF0_P03_SEL_UART2_TX        (1ul<<6) //def

#define  GPIO_AF0_P04_SEL_GPIO            (0ul<<8)
#define  GPIO_AF0_P04_SEL_SWD_CLK         (1ul<<8) //def

#define  GPIO_AF0_P05_SEL_GPIO            (0ul<<10)
#define  GPIO_AF0_P05_SEL_SWD_IO          (1ul<<10) //def

#define  GPIO_AF0_P06_SEL_GPIO            (0ul<<12)
#define  GPIO_AF0_P06_SEL_I2C_SDA         (1ul<<12)

#define  GPIO_AF0_P07_SEL_GPIO            (0ul<<14)
#define  GPIO_AF0_P07_SEL_I2C_SCL         (1ul<<14)

#define  GPIO_AF0_P08_SEL_GPIO            (0ul<<16)
#define  GPIO_AF0_P08_SEL_TMR0_TOG        (1ul<<16)

#define  GPIO_AF0_P09_SEL_GPIO            (0ul<<18)
#define  GPIO_AF0_P09_SEL_TMR0_TOGN       (1ul<<18)

#define  GPIO_AF0_P10_SEL_GPIO            (0ul<<20)
#define  GPIO_AF0_P10_SEL_TMR1_TOG        (1ul<<20)

#define  GPIO_AF0_P11_SEL_GPIO            (0ul<<22)
#define  GPIO_AF0_P11_SEL_TMR1_TOGN       (1ul<<22)

#define  GPIO_AF0_P12_SEL_GPIO            (0ul<<24)
#define  GPIO_AF0_P12_SEL_SPI_CS          (1ul<<24)

#define  GPIO_AF0_P13_SEL_GPIO            (0ul<<26)
#define  GPIO_AF0_P13_SEL_SPI_SCK         (1ul<<26)

#define  GPIO_AF0_P14_SEL_GPIO            (0ul<<28)
#define  GPIO_AF0_P14_SEL_SPI_MOSI        (1ul<<28)

#define  GPIO_AF0_P15_SEL_GPIO            (0ul<<30)
#define  GPIO_AF0_P15_SEL_SPI_MISO        (1ul<<30) 

//GPIO_AF1
#define  GPIO_AF1_P16_SEL_GPIO            (0<<0)
#define  GPIO_AF1_P16_SEL_NO              (1<<0) 

#define  GPIO_AF1_P17_SEL_GPIO            (0<<2)
#define  GPIO_AF1_P17_SEL_NO              (1<<2) 

#define  GPIO_AF1_P18_SEL_GPIO            (0<<4)
#define  GPIO_AF1_P18_SEL_NO              (1<<4) 

#define  GPIO_AF1_P19_SEL_GPIO            (0<<8)
#define  GPIO_AF1_P19_SEL_NO              (1<<8) 

//GPIO_INT_TYPE
#define  GPIO_INT_TYPE_LEVEL              1
#define  GPIO_INT_TYPE_EDAGE              0

//GPIO_INT_POLARITY
#define  GPIO_INT_POLARITY_HIGH           1
#define  GPIO_INT_POLARITY_LOW            0

//GPIO_EXT_INTE
#define  GPIO_EXT_INTE_ENABLE             1
#define  GPIO_EXT_INTE_DISABLE            0 

//GPIO_INTF
#define  GPIO_INTF_msk      0x000fffff

//GPIO_CTL
#define  GPIO_CTL_INT_EN          (1<<0)
#define  GPIO_CTL_INT_DEBOUNCE_EN (1<<1)
#define  GPIO_CTL_LS_SYNC_EN      (1<<2)
#define  GPIO_SetCTLBit(x)          {IOM->GPIO_CTL |= (x);}
#define  GPIO_ClrCTLBit(x)          {IOM->GPIO_CTL &= (x);}
//@}  regist declare end

#define     GPIO_IntGlobleEnable()        {IOM -> GPIO_CTL |= GPIO_CTL_INT_EN;}
#define     GPIO_IntGlobleDisable()        {IOM -> GPIO_CTL &= ~GPIO_CTL_INT_EN;}


typedef struct _GPIOPinCfg{
    uint32_t wPin;
    uint32_t wAnalog;
    uint32_t wPinFun; 
    uint32_t wDir;
    uint32_t wPullUp;
    uint32_t wPullDown;
    uint32_t wOutType;
    uint32_t wOutDrive;
    uint32_t wIntEnable;
    uint32_t wIntType;
    uint32_t wIntPolarity;
}GPIOPinCfg_Type;

typedef void (*GPIOSignalEvent_Type) (uint32_t wEvent);  ///< Pointer to GPIO Event.

//function declare
void GPIO_PinConfig(GPIOPinCfg_Type * psPinCfg,GPIOSignalEvent_Type cbFunc);
void GPIO_SetPin(uint32_t wPinNo);
void GPIO_ClrPin(uint32_t wPinNo);
uint32_t GPIO_GetPin(uint32_t wPinNo);
uint32_t GPIO_GetData(void);
void GPIO_SetData(uint32_t wValue);
uint32_t GPIO_GetIntFlag(void);
void GPIO_ClrIntFlag(uint32_t wValue);


#ifdef __cplusplus
}
#endif


#endif
