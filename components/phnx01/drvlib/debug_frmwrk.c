/**********************************************************************
* $Id$		debug_frmwrk.c				2019-05-06
*//**
* @file		debug_frmwrk.c
* @brief	Contains some utilities that used for debugging through UART
* @version	1.0
* @date		06. 05. 2019
* @author	FHSJDZ MCU SW Application Team
*
* Copyright(C) 2019, FHSJDZ Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* FHSJDZ Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. FHSJDZ Semiconductors
* reserves the right to make changes in the software without
* notification. FHSJDZ Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
**********************************************************************/

#include "debug_frmwrk.h"
#include "uart.h"
#include "iom.h"


#ifdef _FW_DEBUG

/* Debug framework */
void (*_db_msg)(UART_TypeDef *UARTx, const void *s);
void (*_db_msg_)(UART_TypeDef *UARTx, const void *s);
void (*_db_char)(UART_TypeDef *UARTx, uint8_t ch);
void (*_db_dec)(UART_TypeDef *UARTx, uint8_t decn);
void (*_db_dec_16)(UART_TypeDef *UARTx, uint16_t decn);
void (*_db_dec_32)(UART_TypeDef *UARTx, uint32_t decn);
void (*_db_hex)(UART_TypeDef *UARTx, uint8_t hexn);
void (*_db_hex_16)(UART_TypeDef *UARTx, uint16_t hexn);
void (*_db_hex_32)(UART_TypeDef *UARTx, uint32_t hexn);
uint8_t (*_db_get_char)(UART_TypeDef *UARTx);


/*********************************************************************//**
 * @brief		Puts a character to UART port
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	ch		Character to put
 * @return		None
 **********************************************************************/
void UARTPutChar (UART_TypeDef *UARTx, uint8_t ch)
{
	UART_Send(UARTx, ch);
}


/*********************************************************************//**
 * @brief		Get a character to UART port
 * @param[in]	UARTx	Pointer to UART peripheral
 * @return		character value that returned
 **********************************************************************/
uint8_t UARTGetChar (UART_TypeDef *UARTx)
{
	uint8_t tmp = 0;
	tmp = UART_Receive(UARTx);
	return(tmp);
}


/*********************************************************************//**
 * @brief		Puts a string to UART port
 * @param[in]	UARTx 	Pointer to UART peripheral
 * @param[in]	str 	string to put
 * @return		None
 **********************************************************************/
void UARTPuts(UART_TypeDef *UARTx, const void *str)
{
	uint8_t *s = (uint8_t *) str;

	while (*s)
	{
		UARTPutChar(UARTx, *s++);
	}
}


/*********************************************************************//**
 * @brief		Puts a string to UART port and print new line
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	str		String to put
 * @return		None
 **********************************************************************/
void UARTPuts_(UART_TypeDef *UARTx, const void *str)
{
	UARTPuts (UARTx, str);
	UARTPuts (UARTx, "\n\r");
}


/*********************************************************************//**
 * @brief		Puts a decimal number to UART port
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	decnum	Decimal number (8-bit long)
 * @return		None
 **********************************************************************/
void UARTPutDec(UART_TypeDef *UARTx, uint8_t decnum)
{
	uint8_t c1=decnum%10;
	uint8_t c2=(decnum/10)%10;
	uint8_t c3=(decnum/100)%10;
	UARTPutChar(UARTx, '0'+c3);
	UARTPutChar(UARTx, '0'+c2);
	UARTPutChar(UARTx, '0'+c1);
}

/*********************************************************************//**
 * @brief		Puts a decimal number to UART port
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	decnum	Decimal number (8-bit long)
 * @return		None
 **********************************************************************/
void UARTPutDec16(UART_TypeDef *UARTx, uint16_t decnum)
{
	uint8_t c1=decnum%10;
	uint8_t c2=(decnum/10)%10;
	uint8_t c3=(decnum/100)%10;
	uint8_t c4=(decnum/1000)%10;
	uint8_t c5=(decnum/10000)%10;
	UARTPutChar(UARTx, '0'+c5);
	UARTPutChar(UARTx, '0'+c4);
	UARTPutChar(UARTx, '0'+c3);
	UARTPutChar(UARTx, '0'+c2);
	UARTPutChar(UARTx, '0'+c1);
}

/*********************************************************************//**
 * @brief		Puts a decimal number to UART port
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	decnum	Decimal number (8-bit long)
 * @return		None
 **********************************************************************/
void UARTPutDec32(UART_TypeDef *UARTx, uint32_t decnum)
{
	uint8_t c1=decnum%10;
	uint8_t c2=(decnum/10)%10;
	uint8_t c3=(decnum/100)%10;
	uint8_t c4=(decnum/1000)%10;
	uint8_t c5=(decnum/10000)%10;
	uint8_t c6=(decnum/100000)%10;
	uint8_t c7=(decnum/1000000)%10;
	uint8_t c8=(decnum/10000000)%10;
	uint8_t c9=(decnum/100000000)%10;
	uint8_t c10=(decnum/1000000000)%10;
	UARTPutChar(UARTx, '0'+c10);
	UARTPutChar(UARTx, '0'+c9);
	UARTPutChar(UARTx, '0'+c8);
	UARTPutChar(UARTx, '0'+c7);
	UARTPutChar(UARTx, '0'+c6);
	UARTPutChar(UARTx, '0'+c5);
	UARTPutChar(UARTx, '0'+c4);
	UARTPutChar(UARTx, '0'+c3);
	UARTPutChar(UARTx, '0'+c2);
	UARTPutChar(UARTx, '0'+c1);
}

/*********************************************************************//**
 * @brief		Puts a hex number to UART port
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	hexnum	Hex number (8-bit long)
 * @return		None
 **********************************************************************/
void UARTPutHex (UART_TypeDef *UARTx, uint8_t hexnum)
{
	uint8_t nibble, i;

	UARTPuts(UARTx, "0x");
	i = 1;
	do {
		nibble = (hexnum >> (4*i)) & 0x0F;
		UARTPutChar(UARTx, (nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble));
	} while (i--);
}


/*********************************************************************//**
 * @brief		Puts a hex number to UART port
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	hexnum	Hex number (16-bit long)
 * @return		None
 **********************************************************************/
void UARTPutHex16 (UART_TypeDef *UARTx, uint16_t hexnum)
{
	uint8_t nibble, i;

	UARTPuts(UARTx, "0x");
	i = 3;
	do {
		nibble = (hexnum >> (4*i)) & 0x0F;
		UARTPutChar(UARTx, (nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble));
	} while (i--);
}

/*********************************************************************//**
 * @brief		Puts a hex number to UART port
 * @param[in]	UARTx	Pointer to UART peripheral
 * @param[in]	hexnum	Hex number (32-bit long)
 * @return		None
 **********************************************************************/
void UARTPutHex32 (UART_TypeDef *UARTx, uint32_t hexnum)
{
	uint8_t nibble, i;

	UARTPuts(UARTx, "0x");
	i = 7;
	do {
		nibble = (hexnum >> (4*i)) & 0x0F;
		UARTPutChar(UARTx, (nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble));
	} while (i--);
}


/*********************************************************************//**
 * @brief		Initialize Debug frame work through initializing UART port
 * @param[in]	None
 * @return		None
 **********************************************************************/
void debug_frmwrk_init(void)
{
#if (USED_UART_DEBUG_PORT==1)
	/*
	 * Initialize UART1 pin connect
	 */
	GPIOPinCfg_Type sU1PinCfg;
	memset(&sU1PinCfg,0,sizeof(sU1PinCfg));
	sU1PinCfg.wPin = GPIO_P00;
	sU1PinCfg.wAnalog = GPIO_ADS_SEL_DIGITAL;
	sU1PinCfg.wPinFun = GPIO_AF0_P00_SEL_UART1_RX;
	sU1PinCfg.wDir = GPIO_DIR_IN;
	GPIO_PinConfig(&sU1PinCfg,NULL);
	sU1PinCfg.wPin = GPIO_P01;
	sU1PinCfg.wPinFun = GPIO_AF0_P01_SEL_UART1_TX;
	sU1PinCfg.wDir = GPIO_DIR_OUT;
	GPIO_PinConfig(&sU1PinCfg,NULL);
	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 9600bps   //clk = 8M
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	 {
	UART_InitType_Def sU1Init;
	sU1Init.wClkDiv = 0x0034;
	sU1Init.wDataLen = UART_LCR_DLS_8BIT;
	sU1Init.wStopBit = UART_LCR_STOP_1BIT;
	UART_CfgParityCheck(UART1,UART_LCR_EPS_ODD,0);
	UART_Init(UART1,&sU1Init,NULL);
	 }
#elif (USED_UART_DEBUG_PORT==2)
	/*
	 * Initialize UART2 pin connect
	 */
	GPIOPinCfg_Type sU2PinCfg;
	memset(&sU2PinCfg,0,sizeof(sU2PinCfg));
	sU2PinCfg.wPin = GPIO_P02;
	sU2PinCfg.wAnalog = GPIO_ADS_SEL_DIGITAL;
	sU2PinCfg.wPinFun = GPIO_AF0_P02_SEL_UART2_RX;
	sU2PinCfg.wDir = GPIO_DIR_IN;
	GPIO_PinConfig(&sU2PinCfg,NULL);
	sU2PinCfg.wPin = GPIO_P03;
	sU2PinCfg.wPinFun = GPIO_AF0_P03_SEL_UART2_TX;
	sU2PinCfg.wDir = GPIO_DIR_OUT;
	GPIO_PinConfig(&sU2PinCfg,NULL);
	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 9600bps  //clk = 8M
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	UART_InitType_Def sU2Init;
	sU2Init.wClkDiv = 0x0034;
	sU2Init.wDataLen = UART_LCR_DLS_8BIT;
	sU2Init.wStopBit = UART_LCR_STOP_1BIT;
	UART_CfgParityCheck(UART2,UART_LCR_EPS_ODD,0);
	UART_Init(UART2,&sU2Init,NULL);
	
#endif
	_db_msg	= UARTPuts;
	_db_msg_ = UARTPuts_;
	_db_char = UARTPutChar;
	_db_hex = UARTPutHex;
	_db_hex_16 = UARTPutHex16;
	_db_hex_32 = UARTPutHex32;
	_db_dec = UARTPutDec;
	_db_dec_16 = UARTPutDec16;
	_db_dec_32 = UARTPutDec32;
	_db_get_char = UARTGetChar;
}
#endif /*_DBGFWK */


/* --------------------------------- End Of File ------------------------------ */
