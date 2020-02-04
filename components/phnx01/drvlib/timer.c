/*****************************************
 * @file            timer.c
 * @brief           timer Model Header file
 * @version         v0.0
 * @date            2019/05/06
 * @author          tangbifei
 * @note      应用于Keil平台
 * Copyright(C) 2019,  FanHai Data Tech Corp.
 * All rights reserved
 ******************************************/


#include <stdint.h>
#include "FH_phoenix01.h"
#include "assert.h"
#include "timer.h"


// static varible declare
static TIMSignalEvent_Type cbTIMInt[4];


__inline void  TIM_SetReloadValue(TIMER_TypeDef * psTim,uint32_t wVal)
{
    ASSERT(psTim == NULL);
    psTim -> LC1R = wVal;
}


__inline uint32_t TIM_GetCurCnt(TIMER_TypeDef *psTim)
{
    ASSERT(psTim == NULL);
    return psTim -> CVR;
}

__inline void TIM_SetCR(TIMER_TypeDef *psTim,uint32_t wCtrlMask)
{
    ASSERT(psTim == NULL);
    ASSERT(wCtrlMask&~TIM_CR_msk); 
    psTim -> CR |= wCtrlMask;
}


__inline void TIM_ClrCR(TIMER_TypeDef *psTim,uint32_t wCtrlMask)
{
    ASSERT(psTim == NULL);
    ASSERT(wCtrlMask&~TIM_CR_msk); 
    psTim -> CR &= ~wCtrlMask;
}

__inline void TIM_ClrIntFlag(TIMER_TypeDef *psTim)
{
    uint32_t tmp;
    ASSERT(psTim == NULL);
    tmp = psTim -> SICR;
	tmp = tmp;
}
__inline uint32_t TIM_GetIntFlag(TIMER_TypeDef *psTim)
{
    ASSERT(psTim == NULL);
    return psTim -> SISR;
}
__inline uint32_t TIM_GetRIntFlag(void)
{
    return TIMER_COMMON->RISR;
}

void TIM_Init(TIMER_TypeDef *psTim,TIMInitType_Def *pInit,TIMSignalEvent_Type cbFunc)
{
	ASSERT(psTim == NULL);
	ASSERT(pInit == NULL);
	psTim ->CR = 0;
	if(pInit -> wPwmSel == TIM_PWM_EN )	
		{
			if( psTim == TIMER0){
				TIM_SetPWM0HighCnt(pInit->wPwmHighValue);}
			else if(psTim == TIMER1){
				TIM_SetPWM1HighCnt(pInit->wPwmHighValue);}				
			else if(psTim == TIMER2){
				TIM_SetPWM2HighCnt(pInit->wPwmHighValue);}			
			else if(psTim == TIMER3){
				TIM_SetPWM3HighCnt(pInit->wPwmHighValue);}		
		}
	if( pInit -> wIntSel == TIM_INT_EN)
		{
			ASSERT(cbFunc == NULL);
			if( psTim == TIMER0)
				cbTIMInt[0] = cbFunc;
			else if(psTim == TIMER1)
				cbTIMInt[1] = cbFunc;			
			else if(psTim == TIMER2)
				cbTIMInt[2] = cbFunc;		
			else if(psTim == TIMER3)
				cbTIMInt[3] = cbFunc;
		}

	TIM_SetReloadValue(psTim,pInit->wReloadValue);
	psTim ->CR = (pInit->wRunMode << TIM_CR_MODE_pos) | ( pInit->wIntSel << TIM_CR_INT_EN_pos) | ( pInit->wPwmSel << TIM_CR_PWM_EN_pos);
	psTim ->CR |= 1<<TIM_CR_EN_pos;
}


void TIMER1_IrqHandler(void)
{
	uint32_t wStatus = TIM_GetIntFlag(TIMER0);
	cbTIMInt[0](wStatus);	
	TIM_ClrIntFlag(TIMER0);
}

void TIMER2_IrqHandler(void)
{
	uint32_t wStatus = TIM_GetIntFlag(TIMER1);
	cbTIMInt[1](wStatus);
	TIM_ClrIntFlag(TIMER1);

}

void TIMER3_IrqHandler(void)
{
	uint32_t wStatus = TIM_GetIntFlag(TIMER2);
	cbTIMInt[2](wStatus);
	TIM_ClrIntFlag(TIMER2);

}

void TIMER4_IrqHandler(void)
{
	uint32_t wStatus = TIM_GetIntFlag(TIMER3);
	cbTIMInt[3](wStatus);
	TIM_ClrIntFlag(TIMER3);

}



