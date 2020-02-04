
/*****************************************
 * @file		uart.c
 * @brief		CMSIS Cortex-M0 Device Source File for UART
 * @version 	v0.0
 * @date		2019/05/06
 * @author		tangbifei
 * @note	  应用于Keil平台
 * Copyright(C) 2019,  FanHai Data Tech Corp.
 * All rights reserved
 ******************************************/

#include <stdint.h>
#include "FH_phoenix01.h"
#include "assert.h"
#include "uart.h"

static UARTSignalEvent_Type cbUartIntFunc;

__inline uint32_t UART_GetRBR(UART_TypeDef *pUart)
{
    ASSERT( (pUart != UART1)&&(pUart != UART2));
    return (pUart->RBR)&UART_RBR_msk;
}

__inline void UART_SetTHR(UART_TypeDef *pUart,uint32_t wVal)
{
    ASSERT( (pUart != UART1)&&(pUart != UART2));
    ASSERT( (wVal & ~UART_THR_msk) != 0)
    pUart->THR = wVal;
}


__inline void UART_SetIER(UART_TypeDef *pUart,uint32_t wValMask)
{
    ASSERT( (pUart != UART1)&&(pUart != UART2));
    pUart-> IER |= wValMask;
}
__inline void UART_ClrIER(UART_TypeDef *pUart,uint32_t wValMask)
{
    ASSERT( (pUart != UART1)&&(pUart != UART2));
    pUart-> IER &= ~wValMask;
}

__inline uint32_t UART_GetIIR(UART_TypeDef *pUart)
{
    ASSERT( (pUart != UART1)&&(pUart != UART2));
    return (pUart->IIR)&UART_IIR_IID_msk;
}

__inline uint32_t UART_GetLSR(UART_TypeDef *pUart)
{
    ASSERT( (pUart != UART1)&&(pUart != UART2));
    return (pUart->LSR)&UART_LSR_msk;
}

//__inline void UART_SetFIFOEnable(UART_TypeDef *pUart,uint32_t wVal)
//{
//    ASSERT( (pUart != UART1)&&(pUart != UART2));
//    pUart->IIR &= ~(3<<UART_IIR_FIFOEN_2BIT_pos);
//    pUart->IIR |= wVal<<UART_IIR_FIFOEN_2BIT_pos;
//}

__inline  void UART_CfgParityCheck(UART_TypeDef *pUart,uint32_t wOddEvenSel,uint32_t wEnable)
{
    ASSERT( wOddEvenSel > 1);
    ASSERT( wEnable > 1);
    pUart->LCR &= ~( (1<<UART_LCR_PEN_pos) | (1<<UART_LCR_EPS_pos) );
    pUart->LCR |=  ( wEnable << UART_LCR_PEN_pos) | ( wOddEvenSel << UART_LCR_EPS_pos);
}


__inline void UART_SetLCR(UART_TypeDef *pUart,uint32_t wValMask)
{
    ASSERT( (pUart != UART1)&&(pUart != UART2));
    pUart-> LCR |= wValMask;
}

__inline void UART_ClrLCR(UART_TypeDef *pUart,uint32_t wValMask)
{
    ASSERT( (pUart != UART1)&&(pUart != UART2));
    pUart-> LCR &= ~wValMask;
}


void UART_Init(UART_TypeDef *pUart,UART_InitType_Def  *pInit,UARTSignalEvent_Type cbFunc)
{
    ASSERT( (pUart != UART1)&&(pUart != UART2));
    ASSERT( pInit == NULL);
    pUart -> LCR = 0;
    pUart -> LCR = pInit-> wDataLen | (pInit -> wStopBit << UART_LCR_SP_pos);
    UART_SetLCR(pUart,1<<UART_LCR_DLAB_pos);
    pUart -> DLL = pInit-> wClkDiv & 0xff;
    pUart -> DLH = pInit-> wClkDiv >> 8;
    UART_ClrLCR(pUart,1<<UART_LCR_DLAB_pos);
    cbUartIntFunc = cbFunc;
}

void UART_Send(UART_TypeDef *pUart, uint16_t byChar)
{
    ASSERT( (pUart != UART1)&&(pUart != UART2));
    ASSERT( (byChar & ~UART_RBR_msk) != 0);
    pUart -> THR = byChar;
    while( (UART_GetLSR(pUart) & UART_LSR_TX_IDLE) == 0);
}


int16_t UART_Receive(UART_TypeDef *pUart)
{
    ASSERT( (pUart != UART1)&&(pUart != UART2));
    while( (UART_GetLSR(pUart) & UART_LSR_DATA_NO_EMPTY) == 0);
    return pUart -> RBR & UART_RBR_msk;
}


void UART_IrqHandler(UART_TypeDef *pUART)
{
    uint32_t wStatus;
    wStatus = UART_GetLSR(pUART);
    cbUartIntFunc(wStatus);
	UART_GetIIR(pUART);   
}


