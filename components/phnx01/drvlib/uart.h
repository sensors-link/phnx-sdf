/*****************************************
 * @file            uart.h
 * @brief           CMSIS Cortex-M0 Device Header File for UART
 * @version         v0.0
 * @date            2019/05/06
 * @author          tangbifei
 * @note      应用于Keil平台
 * Copyright(C) 2019,  FanHai Data Tech Corp.
 * All rights reserved
 ******************************************/

#ifndef  _UART_
#define _UART_


#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#include <stdint.h>

//rbr
#define UART_RBR_msk           0x1ff

//thr
#define UART_THR_msk           0x1ff

//ier
#define UART_IER_RCV_DATA_pos                0
#define UART_IER_THRE_pos                1
#define UART_IER_RLS_pos                2
#define UART_IER_MODEM_pos                3
#define UART_IER_PT_pos                7


//iir
#define UART_IIR_IID_MODEM                   0
#define UART_IIR_IID_NO_INT                  1
#define UART_IIR_IID_THR_EMP                 2
#define UART_IIR_IID_RCV_DATA                4
#define UART_IIR_IID_RCV_LIN_STATUS          6
#define UART_IIR_IID_BUSY                    7
#define UART_IIR_IID_CHAR_TIME_O             12
#define UART_IIR_IID_msk                0x0f

#define UART_IIR_FIFOEN_2BIT_pos           6

#define UART_IIR_FIFOEN_2BIT_EN           3
#define UART_IIR_FIFOEN_2BIT_DIS          0



//lcr
#define UART_LCR_DLS_pos            0
#define UART_LCR_STOP_pos           2
#define UART_LCR_PEN_pos            3
#define UART_LCR_EPS_pos            4
#define UART_LCR_SP_pos             5
#define UART_LCR_BRK_pos            6
#define UART_LCR_DLAB_pos           7

#define UART_LCR_DLS_5BIT          0
#define UART_LCR_DLS_6BIT          1
#define UART_LCR_DLS_7BIT          2
#define UART_LCR_DLS_8BIT          3

#define UART_LCR_STOP_1BIT        0
#define UART_LCR_STOP_2BIT        1

#define UART_LCR_EPS_ODD          1
#define UART_LCR_EPS_EVEN         0

//lsr
#define UART_LSR_DATA_NO_EMPTY       (1ul<<0)
#define UART_LSR_OVER_ERROR          (1ul<<1)
#define UART_LSR_PE                  (1ul<<2)
#define UART_LSR_FE                  (1ul<<3)
#define UART_LSR_BREAK               (1ul<<4)
#define UART_LSR_THRE                (1ul<<5)
#define UART_LSR_TX_IDLE             (1ul<<6)
#define UART_LSR_RFIFO_ERROR         (1ul<<7)
#define UART_LSR_RCV_BIT9_ADDR       (1ul<<8)

#define UART_LSR_msk               0x1ff
typedef struct _UARTInit{
    uint16_t wClkDiv;
    uint32_t wDataLen;
    uint32_t wStopBit;
}UART_InitType_Def;


typedef void (*UARTSignalEvent_Type)(uint32_t wEvent);


//function declare
uint32_t UART_GetRBR(UART_TypeDef *pUart);
void UART_SetTHR(UART_TypeDef *pUart,uint32_t wVal);
void UART_SetIER(UART_TypeDef *pUart,uint32_t wValMask);
void UART_ClrIER(UART_TypeDef *pUart,uint32_t wValMask);
uint32_t UART_GetIIR(UART_TypeDef *pUart);
uint32_t UART_GetLSR(UART_TypeDef *pUart);
void UART_SetLCR(UART_TypeDef *pUart,uint32_t wValMask);
void UART_ClrLCR(UART_TypeDef *pUart,uint32_t wValMask);


void UART_CfgParityCheck(UART_TypeDef *pUart,uint32_t wOddEvenSel,uint32_t wEnable);

void UART_Init(UART_TypeDef *pUart,UART_InitType_Def  *pInit,UARTSignalEvent_Type cbFunc);

void UART_Send(UART_TypeDef *pUart, uint16_t byChar);
int16_t UART_Receive(UART_TypeDef *pUart);

void UART_IrqHandler(UART_TypeDef *pUART);


#ifdef __cplusplus
}
#endif

#endif 
