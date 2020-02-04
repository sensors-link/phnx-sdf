/*****************************************
 * @file        i2c.h
 * @brief       i2c header file
 * @version     v0.0
 * @date        2019/05/06
 * @author      tangbifei
 * @note      ???Keil??
 * Copyright(C) 2019,  FanHai Data Tech Corp.
 * All rights reserved
 ******************************************/

#ifndef _I2C_

#define  _I2C_

#include <stddef.h>
#include <stdint.h>


//CON         cfg  i2c_enable = 0
#define  I2C_CON_MODE_MASTER      (1ul<<0)
#define  I2C_CON_MODE_SLAVE       (0ul<<0)

#define  I2C_CON_SPEED_STD        (1ul<<1)
#define  I2C_CON_SPEED_FAST       (2ul<<1)
#define  I2C_CON_SPEED_HIGH       (3ul<<1)
#define  I2C_CON_RESTART          (1ul<<5)

#define  I2C_CON_SLV_DIS          (1ul<<6)
#define  I2C_CON_SLV_EN           (0ul<<6)

#define  I2C_CON_STOP_DET_IFADDR  (1ul<<7)
#define  I2C_CON_TX_EPT_SEND_OVER (1ul<<8)


//TAR   i2c_enable = 1
#define  I2C_TAR_ADDR(x)          ((x)<<0)    def:55
#define  I2C_TAR_START_BYTE       (1ul<<10)
#define  I2C_TAR_GENERAT_CALL     (0ul<<10)
#define  I2C_TAR_SPECIAL_MODE     (1ul<<11)
#define  I2C_TAR_NORMAL_MODE      (0ul<<11)  //ignore bit10

#define  I2C_SetTAR(x)    {I2C->TAR = (x);}


//SAR    i2c_enable = 0
#define  I2C_SAR_ADDR(x)          ((x)<<0)

//DATACMD
#define  I2C_DATACMD_RESTART       (1ul<<10)
#define  I2C_DATACMD_STOP          (1ul<<9)
#define  I2C_DATACMD_CMD_READ      (1ul<<8)
#define  I2C_DATACMD_CMD_WRITE     (0ul<<8)
#define  I2C_DATACMD_DATA(X)       ((X)<<0)

#define  I2C_SetDataCmd(x)     {I2C->DATACMD = (x);}
#define  I2C_GetData(x)        {x = I2C->DATACMD;}
//SSHCNT
#define  I2C_SSHCNT_16BIT(x)       ((x)<<0)
//SSLCNT
#define  I2C_SSLCNT_16BIT(x)       ((x)<<0)


//INTRSTAT
#define  I2C_INTRSTAT_RX_UNDER    (1ul<<0)
#define  I2C_INTRSTAT_RX_OVER     (1ul<<1)
#define  I2C_INTRSTAT_RX_FULL     (1ul<<2)
#define  I2C_INTRSTAT_TX_OVER     (1ul<<3)
#define  I2C_INTRSTAT_TX_EMPTY    (1ul<<4)
#define  I2C_INTRSTAT_RD_REQ      (1ul<<5)
#define  I2C_INTRSTAT_TX_ABRT     (1ul<<6)
#define  I2C_INTRSTAT_RX_DONE     (1ul<<7)
#define  I2C_INTRSTAT_ACTIVITY    (1ul<<8)
#define  I2C_INTRSTAT_STOP_DEC    (1ul<<9)
#define  I2C_INTRSTAT_START_DEC   (1ul<<10)
#define  I2C_INTRSTAT_GEN_CALL    (1ul<<11)

#define  I2C_GetINTRSTAT(x)       {x = I2C->INTRSTAT;}
//INTRMSK
#define  I2C_INTRMSK_RX_UNDER    (1ul<<0)
#define  I2C_INTRMSK_RX_OVER     (1ul<<1)
#define  I2C_INTRMSK_RX_FULL     (1ul<<2)
#define  I2C_INTRMSK_TX_OVER     (1ul<<3)
#define  I2C_INTRMSK_TX_EMPTY    (1ul<<4)
#define  I2C_INTRMSK_RD_REQ      (1ul<<5)
#define  I2C_INTRMSK_TX_ABRT     (1ul<<6)
#define  I2C_INTRMSK_RX_DONE     (1ul<<7)
#define  I2C_INTRMSK_ACTIVITY    (1ul<<8)
#define  I2C_INTRMSK_STOP_DEC    (1ul<<9)
#define  I2C_INTRMSK_START_DEC   (1ul<<10)
#define  I2C_INTRMSK_GEN_CALL    (1ul<<11)

#define  I2C_INTRMSK_msk         0x0fff

//RAWINTRSTAT
#define  I2C_RAWINTRSTAT_RX_UNDER    (1ul<<0)
#define  I2C_RAWINTRSTAT_RX_OVER     (1ul<<1)
#define  I2C_RAWINTRSTAT_RX_FULL     (1ul<<2)
#define  I2C_RAWINTRSTAT_TX_OVER     (1ul<<3)
#define  I2C_RAWINTRSTAT_TX_EMPTY    (1ul<<4)
#define  I2C_RAWINTRSTAT_RD_REQ      (1ul<<5)
#define  I2C_RAWINTRSTAT_TX_ABRT     (1ul<<6)
#define  I2C_RAWINTRSTAT_RX_DONE     (1ul<<7)
#define  I2C_RAWINTRSTAT_ACTIVITY    (1ul<<8)
#define  I2C_RAWINTRSTAT_STOP_DEC    (1ul<<9)
#define  I2C_RAWINTRSTAT_START_DEC   (1ul<<10)
#define  I2C_RAWINTRSTAT_GEN_CALL    (1ul<<11)


//ENABLE
#define  I2C_ENABLE_ABRT        (1ul<<1)
#define  I2C_ENABLE_TX_BLK_DIS  (1ul<<2)

#define  I2C_Enable()    {I2C->ENABLE |= 1ul;}
#define  I2C_Disalbe()    {I2C->ENABLE &= ~(1ul);}


//STATUS
#define  I2C_STATUS_ACTIVITY      (1ul<<0)
#define  I2C_STATUS_TX_FULL       (1ul<<1)
#define  I2C_STATUS_TX_EMPTY      (1ul<<2)
#define  I2C_STATUS_RX_EMPTY      (1ul<<3)
#define  I2C_STATUS_RX_FULL       (1ul<<4)
#define  I2C_STATUS_MST_ACTIVITY  (1ul<<5)
#define  I2C_STATUS_SLV_ACTIVITY  (1ul<<6)
#define  I2C_GetSTATUS(x)       {x = I2C->STATUS;}

//SDAHOLD  //i2c_enable = 0
#define  I2C_SDAHOLD_RX_8BIT(x)     ((x)<<16)
#define  I2C_SDAHOLD_TX_16BIT(x)     ((x)<<0)

//ABRTATUS
#define  I2C_ABRTATUS_7BIT_ADDR_NOACK        (1ul<<0)
#define  I2C_ABRTATUS_10BIT_ADDR1_NOACK      (1ul<<1)
#define  I2C_ABRTATUS_10BIT_ADDR2_NOACK      (1ul<<2)
#define  I2C_ABRTATUS_TX_DATA_NOACK          (1ul<<3)
#define  I2C_ABRTATUS_GCALL_NOACK            (1ul<<4)
#define  I2C_ABRTATUS_GCALL_READ             (1ul<<5)
#define  I2C_ABRTATUS_SBYTE_ACKDET           (1ul<<7)
#define  I2C_ABRTATUS_SBYTE_NORSTRT          (1ul<<9)
#define  I2C_ABRTATUS_10BIT_RD_NORSTRT       (1ul<<10)
#define  I2C_ABRTATUS_MASTER_DIS             (1ul<<11)
#define  I2C_ABRTATUS_LOST                   (1ul<<12)
#define  I2C_ABRTATUS_SLVFLUSH_TXFIFO        (1ul<<13)
#define  I2C_ABRTATUS_SLV_TRS_LOST           (1ul<<14)
#define  I2C_ABRTATUS_SLVRD_INTX             (1ul<<15)
#define  I2C_ABRTATUS_USER_ABRT              (1ul<<16)
#define  I2C_ABRTATUS_TX_FLUSH_CNT_msk       (7ul<<23)


//SDASETUP
#define  I2C_SDASETUP_TX_8BIT(x)      ((x)<<0)

//ACKGENCALL
#define  I2C_ACKGENCALL_CTRL          (1ul<<0)

//I2CENSTAS
#define  I2C_I2CENSTAS_SLV_RX_DATA_LOST (1ul<<2)
#define  I2C_I2CENSTAS_SLV_DIS_OF_BUSY  (1ul<<1)
#define  I2C_I2CENSTAS_I2C_EN           (1ul<<0)

//FSSPKLEN
#define  I2C_FSSPKLEN_1BIT(x)          ((x)<<0)


//define mcro fuction
#define  I2C_SetSSLCNT(x)         {I2C->SSLCNT  = (x);}
#define  I2C_SetSSHCNT(x)         {I2C->SSHCNT  = (x);}
//#define  I2C_SetRXTL(x)           {I2C->RXTL    = (x);}
//#define  I2C_SetTXTL(x)           {I2C->TXTL    = (x);}
//#define  I2C_SetRXFLR(x)          {I2C->RXFLR   = (x);}
//#define  I2C_SetTXFLR(x)          {I2C->TXFLR   = (x);}
#define  I2C_SetSDAHoldT(x)       {I2C->SDAHOLD = (x);}
#define  I2C_SetSDASetupT(x)      {I2C->SDASETUP = (x);}
#define  I2C_SetFSSPKLEN(x)       {I2C->FSSPKLEN = (x);}


#define  I2C_ClrAllIntFlag()   {int tmp = I2C->CLRINTR; (void)tmp;}
#define  I2C_ClrRxUnderFlag()  {int tmp = I2C->RCLRUND; (void)tmp;}
#define  I2C_ClrRxOverFlag()   {int tmp = I2C->CLRRXOVER; (void)tmp;}
#define  I2C_ClrTxOverFlag()   {int tmp = I2C->CLRTXOVER; (void)tmp;}
#define  I2C_ClrRdReqFlag()   {int tmp = I2C->CLRRDREQ; (void)tmp;}
#define  I2C_ClrTxAbrt()      {int tmp = I2C->CLRTXABRT; (void)tmp;}
#define  I2C_ClrRxDone()      {int tmp = I2C->CLRRXDONE; (void)tmp;}
#define  I2C_ClrActivety()    {int tmp = I2C->CLRACTIVITY; (void)tmp;}
#define  I2C_ClrStopDet()     {int tmp = I2C->CLRSTOPDET ; (void)tmp;}
#define  I2C_ClrStartDet()    {int tmp = I2C->CLRSTARTDET; (void)tmp;}
#define  I2C_ClrGenCall()     {int tmp = I2C->CLRGENCALL ; (void)tmp;}


typedef void (*I2CSignalEvent_Type)(uint32_t wEvent);

typedef struct _I2C_Init{
    int wMode;
    int wSpeed;
    int wClkHighCnt;
    int wClkLowCnt;
    int wSlaveAddr;
	int wTAddr;
    int wIntMask;
}I2C_InitType_Def;


void I2C_Init(I2C_InitType_Def *psInit,I2CSignalEvent_Type cbFunc);
uint32_t I2C_MasterSendData(uint8_t * pbyValue, uint32_t wLen);
uint32_t I2C_MasterRecieveByte(uint8_t * pbyValue, uint32_t wLen);

extern uint8_t * pbyI2CTxBuff;
extern uint8_t * pbyI2CRxBuff;
extern uint32_t  wI2CDataLen;


#endif
