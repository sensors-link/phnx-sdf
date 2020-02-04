/*****************************************
 * @file            timer.h
 * @brief           timer Model Header file
 * @version         v0.0
 * @date            2019/05/06
 * @author          tangbifei
 * @note      应用于Keil平台
 * Copyright(C) 2019,  FanHai Data Tech Corp.
 * All rights reserved
 ******************************************/

#ifndef _TIMER_H_


#define _TIMER_H_

#include <stddef.h>
#include <stdint.h>




#define   TIM_CR_EN_pos            0
#define   TIM_CR_MODE_pos          1
#define   TIM_CR_INT_EN_pos        2
#define   TIM_CR_PWM_EN_pos        3


#define   TIM_PWM_EN     1
#define   TIM_PWM_DIS    0

#define   TIM_INT_EN     0
#define   TIM_INT_DIS    1

#define   TIM_RUN_MODE_NORMAL      0
#define   TIM_RUN_MODE_DEFINE      1

#define  TIM_CR_msk      0x0f



//function micro


#define  TIM_GetALLIntFlag(x)   {x = TIMER_COMMON->ISR;}
#define  TIM_ClrAllINTFlag()    {uint32_t wTmp = TIMER_COMMON ->ICR;}
  

#define  TIM_SetPWM0HighCnt(x) {TIMER_COMMON -> LC2R_0 = x;}
#define  TIM_SetPWM1HighCnt(x) {TIMER_COMMON -> LC2R_1 = x;}
#define  TIM_SetPWM2HighCnt(x) {TIMER_COMMON -> LC2R_2 = x;}
#define  TIM_SetPWM3HighCnt(x) {TIMER_COMMON -> LC2R_3 = x;}



typedef void (*TIMSignalEvent_Type)(uint32_t wEvent);

typedef struct _TimInit{
	uint32_t wRunMode;
	uint32_t wPwmSel;
	uint32_t wIntSel;
	uint32_t wReloadValue;
	uint32_t wPwmHighValue;
}TIMInitType_Def;


//function declare
void  TIM_SetReloadValue(TIMER_TypeDef * psTim,uint32_t wVal);
uint32_t TIM_GetCurCnt(TIMER_TypeDef *psTim);
uint32_t TIM_GetIntFlag(TIMER_TypeDef *psTim);
uint32_t TIM_GetRIntFlag(void);
void TIM_ClrIntFlag(TIMER_TypeDef *psTim);
void TIM_SetCR(TIMER_TypeDef *psTim,uint32_t wCtrlMask);
void TIM_ClrCR(TIMER_TypeDef *psTim,uint32_t wCtrlMask);

void TIM_Init(TIMER_TypeDef *psTim,TIMInitType_Def *pInit,TIMSignalEvent_Type cbFunc);







#endif
