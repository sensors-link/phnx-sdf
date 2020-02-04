/*****************************************
 * @file        iom.c
 * @brief       iom source file
 * @version     v0.0
 * @date        2019/05/06
 * @author      tangbifei
 * @note      应用于Keil平台
 * Copyright(C) 2019,  FanHai Data Tech Corp.
 * All rights reserved
 ******************************************/
#include "FH_phoenix01.h"
#include "iom.h"
#include "assert.h"


// static varible declare
static GPIOSignalEvent_Type cbPortInt;


/*****************************************
 * @brief             Pin Configure
 * @param[in]  GPIOPinCfg_Type * psPinCfg,GPIOSignalEvent_Type cbFunc 
 * @return         void
 * @note
 ******************************************/
void GPIO_PinConfig(GPIOPinCfg_Type * psPinCfg,GPIOSignalEvent_Type cbFunc)
{
    ASSERT(psPinCfg == NULL);
    ASSERT(psPinCfg -> wPin > GPIO_P19);
    IOM -> GPIO_ADS &= ~(1ul<<psPinCfg->wPin); 
    if( psPinCfg -> wAnalog == ( GPIO_ADS_SEL_ANALOG << psPinCfg -> wPin) ) //analog pin
    {
	IOM -> GPIO_ADS |= GPIO_ADS_SEL_ANALOG << psPinCfg -> wPin;
    }
    else               //digital pin
    {
        if( psPinCfg -> wPin > GPIO_P15)
	{
	    IOM -> GPIO_AF1 &=~(3ul<<(psPinCfg->wPin<<1));
	    IOM -> GPIO_AF1 |= psPinCfg -> wPinFun;
	}
	else
	{
			IOM -> GPIO_AF0 &=~(3ul<<(psPinCfg->wPin<<1));
	    IOM -> GPIO_AF0 |= psPinCfg -> wPinFun;
	}	    

	IOM -> GPIO_DIR &=~(1ul<<psPinCfg->wPin);

	if( psPinCfg -> wDir == GPIO_DIR_IN) 
	{
	    if( psPinCfg -> wIntEnable == GPIO_EXT_INTE_ENABLE)
	    {
		IOM -> GPIO_EXT_INTE |= 1ul<<psPinCfg->wPin;

		IOM -> GPIO_INT_TYPE &=~(1ul<<psPinCfg->wPin);
		IOM -> GPIO_INT_POLARITY &=~(1ul<<psPinCfg->wPin);

		IOM -> GPIO_INT_TYPE |= psPinCfg -> wIntType << psPinCfg->wPin;
		IOM -> GPIO_INT_POLARITY |= psPinCfg -> wIntPolarity << psPinCfg->wPin;
		ASSERT(cbFunc == NULL);
		cbPortInt = cbFunc;  
	    }
	    else
	    {
		IOM -> GPIO_EXT_INTE &=~(1ul<<psPinCfg->wPin);
		cbPortInt = NULL;
	    }
	}
	else
	{
		IOM -> GPIO_DIR |= 1ul<<psPinCfg->wPin;
	    IOM -> GPIO_OTYPE &=~(1ul<<psPinCfg->wPin);
	    IOM -> GPIO_DRS &=~(1ul<<psPinCfg->wPin);
	    IOM -> GPIO_OTYPE |= psPinCfg->wOutType << psPinCfg->wPin;
	    IOM -> GPIO_DRS |= psPinCfg->wOutType << psPinCfg->wPin;
	}
    IOM->GPIO_PU &=~(1ul<<psPinCfg->wPin);
    IOM->GPIO_PD &=~(1ul<<psPinCfg->wPin);
    IOM->GPIO_PU |= psPinCfg->wPullUp << psPinCfg->wPin;
    IOM->GPIO_PD |= psPinCfg->wPullDown << psPinCfg->wPin;
}

}


/*****************************************
 * @brief    set PIN to high
 * @param[in]  GPIO_P00  GPIO_P01...P19 
 * @return     void
 * @note       
 ******************************************/
__inline void GPIO_SetPin(uint32_t wPinNo)
{
    ASSERT(wPinNo>GPIO_P19);
    IOM->GPIO_DATA = (1ul<<wPinNo); 
}


/*****************************************
 * @brief    set PIN to low
 * @param[in]  GPIO_P00  GPIO_P01...P19 
 * @return     void
 * @note       
 ******************************************/
__inline void GPIO_ClrPin(uint32_t wPinNo)
{
    ASSERT(wPinNo>GPIO_P19);
    IOM->GPIO_DATA &=~(1<<wPinNo);
}

/*****************************************
 * @brief       get pin level
 * @param[in]   GPIO_P00 ...GPIO_P19
 * @return      GPIO_LOW(0) or GPIO_HIGH(1)
 * @note
 ******************************************/
__inline uint32_t GPIO_GetPin(uint32_t wPinNo)
{ 
    ASSERT(wPinNo>GPIO_P19);
    return( (IOM->GPIO_DATA & (1ul<<wPinNo) )>>wPinNo );
}

/*****************************************
 * @brief       get port data
 * @param[in]   void
 * @return      
 * @note
 ******************************************/

__inline uint32_t GPIO_GetData(void)
{
	return IOM->GPIO_DATA&GPIO_DATA_msk;
}

/*****************************************
 * @brief       set port data
 * @param[in]   void
 * @return      
 * @note
 ******************************************/

__inline void GPIO_SetData(uint32_t wValue)
{
	ASSERT( (wValue & ~GPIO_DATA_msk) != 0);
	IOM->GPIO_DATA = wValue;
}


/*****************************************
 * @brief     get int flag bit
 * @param[in]  void
 * @return      GPIO_LOW(0) or GPIO_HIGH(1)
 * @note       
 ******************************************/

__inline uint32_t GPIO_GetIntFlag(void)
{
	return( IOM->GPIO_INTF );
}

/*****************************************
 * @brief    clr int flag bit
 * @param[in]  set bits 1 
 * @return     void
 * @note       
 ******************************************/

__inline void GPIO_ClrIntFlag(uint32_t wValue)
{
    IOM->GPIO_INTF = wValue;	
}

/*****************************************
 * @brief      IOM interupt handler function
 * @param[in]  void
 * @return     void
 * @note
 ******************************************/
void IOM_IrqHandler(void)
{
    uint32_t wState = IOM->GPIO_INTF;
    cbPortInt(wState);
}




