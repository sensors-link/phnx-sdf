
/*****************************************
 * @file        spi.c
 * @brief       spi master source file
 * @version     v0.0
 * @date        2019/05/06
 * @author      tangbifei
 * @note      应用于Keil平台
 * Copyright(C) 2019,  FanHai Data Tech Corp.
 * All rights reserved
 ******************************************/
#include "FH_phoenix01.h"
#include "spi.h"
#include "assert.h"


// static varible declare
static SPISignalEvent_Type cbSPIInt;




void SPI_Init(SPI_InitType_Def *psInit, SPISignalEvent_Type cbFunc)
{
	ASSERT(psInit == NULL);
	SPI->ENABLE = 0;
	SPI->CONTROL0 = (psInit->iClkPhase << SPI_CONTROL0_SCPH_pos) |
		            (psInit->iClkPolarity << SPI_CONTROL0_SCPOL_pos) |
//		            (psInit->iFrameFormat << SPI_CONTROL0_FRF_pos) |
//		            (psInit->iTransferMode << SPI_CONTROL0_TMOD_pos) |
		            (psInit->iDataFrameLen << SPI_CONTROL0_DFS_pos); 
	SPI->BAUDR = psInit->iClkDiv;
	if(psInit->iIntMask & (SPI_IMR_TXEIM | SPI_IMR_RXFIM))
		{
			ASSERT(cbFunc == NULL);
			SPI->IMR = psInit->iIntMask;
			cbSPIInt = cbFunc;
		}
	
}

/*****************************************
 * @brief    set spi slave cs sigal enable
 * @param[in]  SPI_SER_EN or !SPI_SER_EN
 * @return     void
 * @note       
 ******************************************/

__inline void SPI_SlaveCSCmd(int32_t wEnable)
{
	ASSERT(wEnable>1);
	{
	uint32_t tmp = SPI->ENABLE;
	SPI->ENABLE = 0;
	SPI->SER = wEnable;
	SPI->ENABLE = tmp;
	}
}


/*****************************************
 * @brief    set spi enable
 * @param[in]  SPI_ENABLE_SSI_EN or !SPI_ENABLE_SSI_EN
 * @return     void
 * @note       
 ******************************************/
__inline void SPI_Cmd(int32_t wEnable)
{
	ASSERT(wEnable>1);
	SPI->ENABLE = wEnable;
}


__inline void SPI_WriteData(uint32_t wData)
{
	SPI->DR0 = wData;
}

__inline uint32_t SPI_ReadData(void)
{
	return SPI->DR0;
}

__inline uint32_t SPI_GetStatus(void)
{
	return SPI->SR;
}

//__inline uint32_t SPI_GetRawStatus(void)
//{
//	return SPI->RISR;
//}

//__inline void SPI_ClrRawFlags(uint32_t wData)
//{
//	SPI->RISR = wData;
//}





uint32_t SPI_TransferRecieveData(uint32_t wData)
{
	uint32_t wRst;
	while( (SPI_GetStatus() & (1<<SPI_SR_TFE_pos)) == 0);	
	SPI_WriteData(wData);
	while( (SPI_GetStatus() & (1<<SPI_SR_RFNE_pos) ) == 0);
	wRst = SPI_ReadData();
	return wRst;
}




void SPI_IrqHandler(void)
{
	uint32_t wStatus = SPI_GetStatus();
	cbSPIInt(wStatus);
}
































