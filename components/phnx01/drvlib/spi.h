/*****************************************
 * @file            spi.h
 * @brief           SPIM Model Header file
 * @version         v0.0
 * @date            2019/05/06
 * @author          tangbifei
 * @note      应用于Keil平台
 * Copyright(C) 2019,  FanHai Data Tech Corp.
 * All rights reserved
 ******************************************/

#ifndef _SPI_
#define _SPI_

#ifdef __cplusplus
extern "C" {
#endif
	
#include <stddef.h>
#include <stdint.h>



//CONTROL0      该寄存器必须在ssi_en=0时配置
#define  SPI_CONTROL0_DFS_1BYTE       7ul
#define  SPI_CONTROL0_DFS_2BYTE       15ul
#define  SPI_CONTROL0_DFS_pos         0      //def:7

#define  SPI_CONTROL0_FRF_MOTOROLA    0ul
//#define  SPI_CONTROL0_FRF_TEXAS       1ul
//#define  SPI_CONTROL0_FRF_NATIONAL    2ul
#define  SPI_CONTROL0_FRF_pos         4

#define  SPI_CONTROL0_SCPH_pos        6

#define  SPI_CONTROL0_SCPOL_pos       7

#define  SPI_CONTROL0_TMOD_TRS_RCV    0ul
//#define  SPI_CONTROL0_TMOD_TRS        1ul
//#define  SPI_CONTROL0_TMOD_RCV        2ul
//#define  SPI_CONTROL0_TMOD_EEP_RD     3ul
#define  SPI_CONTROL0_TMOD_pos         8

#define  SPI_CONTROL0_SLV_OE_EN      0ul
#define  SPI_CONTROL0_SLV_OE_DIS     1ul
#define  SPI_CONTROL0_SLV_OE_pos     10



////CONTROL1  //ssi_en = 0 cfg
//#define  SPI_CONTROL1_NDF_pos       0


//ENABLE
#define  SPI_ENABLE_SSI_EN       (1<<0)

////MWCR
//#define  SPI_MWCR_MWMOD_SEQ_TRSF    (1<<0)

//#define  SPI_MWCR_MWMOD_MDD_TRSF    1ul
//#define  SPI_MWCR_MWMOD_MDD_RCV     0ul
//#define  SPI_MWCR_MWMOD_MDD_pos     1

//#define  SPI_MWCR_MHS_EN            (1ul<<2)

//SER
#define  SPI_SER_EN       (1<<0)

//BAUDR
#define  SPI_BAUDR_SCKDV_16B(x)       (x<<0)  //ensures an even

////TFT
//#define  SPI_TFT_3B(x)        (x<<0)

////RFT
//#define  SPI_RFT_3B(x)        (x<<0)

////TFL
//#define  SPI_TFL_3B(x)        (x<<0)

////RFL
//#define  SPI_RFL_3B(x)         (x<<0)

//SR
#define  SPI_SR_BUSY_pos      0
//#define  SPI_SR_TFNF_pos      1
#define  SPI_SR_TFE_pos       2
#define  SPI_SR_RFNE_pos      3
//#define  SPI_SR_RFF_pos       4
//#define  SPI_SR_DCOL_pos      6

//IMR
#define  SPI_IMR_TXEIM       (1ul<<0)
//#define  SPI_IMR_TXOIM       (1ul<<1)
//#define  SPI_IMR_RXUIM       (1ul<<2)
//#define  SPI_IMR_RXOIM       (1ul<<3)
#define  SPI_IMR_RXFIM       (1ul<<4)
//#define  SPI_IMR_MSTIM       (1ul<<5)

//ISR
#define  SPI_ISR_TXEIS_pos       0
//#define  SPI_ISR_TXOIS_pos       1
//#define  SPI_ISR_RXUIS_pos       2
//#define  SPI_ISR_RXOIS_pos       3
#define  SPI_ISR_RXFIS_pos       4
//#define  SPI_ISR_MSTIS_pos       5

//RISR
//#define  SPI_RISR_TXEIR_pos       0
//#define  SPI_RISR_TXOIR_pos       1
//#define  SPI_RISR_RXUIR_pos       2
//#define  SPI_RISR_RXOIR_pos       3
//#define  SPI_RISR_RXFIR_pos       4
//#define  SPI_RISR_MSTIR_pos       5


//define initial data type
typedef struct _SPI_INIT{
    int iClkDiv;
    int iDataFrameLen;
//    int iFrameFormat;
    int iClkPhase;
    int iClkPolarity;
//    int iTransferMode;	
	int iIntMask;
}SPI_InitType_Def;

#ifdef __cplusplus
}
#endif


typedef void (*SPISignalEvent_Type) (uint32_t wEvent);  ///< Pointer to GPIO Event.

void SPI_WriteData(uint32_t wData);
uint32_t SPI_ReadData(void);
void SPI_SlaveCSCmd(int32_t wEnable);
void SPI_Cmd(int32_t wEnable);

void SPI_Init(SPI_InitType_Def *psInit, SPISignalEvent_Type cbFunc);
uint32_t SPI_TransferRecieveData(uint32_t wData);



#endif











