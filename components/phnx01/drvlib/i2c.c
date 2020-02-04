/*****************************************
 * @file        i2c.c
 * @brief       i2c source file
 * @version     v0.0
 * @date        2019/05/06
 * @author      tangbifei
 * @note      ???Keil??
 * Copyright(C) 2019,  FanHai Data Tech Corp.
 * All rights reserved
 ******************************************/

#include "FH_phoenix01.h"
#include "i2c.h"
#include "assert.h"

static I2CSignalEvent_Type cbI2cIntFunc;

void I2C_Init(I2C_InitType_Def *psInit, I2CSignalEvent_Type cbFunc) {
    ASSERT(psInit == NULL);
    SYSC->MSFTRSTCFG = 0x01;
    I2C_Disalbe();
    I2C->CON = psInit->wMode | psInit->wSpeed | (1 << 5);
    I2C_SetSSHCNT(psInit->wClkHighCnt);
    I2C_SetSSLCNT(psInit->wClkLowCnt);
    I2C_SetTAR(psInit->wTAddr);
    I2C->SAR = psInit->wSlaveAddr;
    if ((psInit->wIntMask & I2C_INTRMSK_msk) != 0) {
        I2C->INTRMSK = psInit->wIntMask;
        ASSERT(cbFunc == NULL);
        cbI2cIntFunc = cbFunc;
    }
    I2C_Enable();
}

uint32_t I2C_MasterSendData(uint8_t *pbyValue, uint32_t wLen) {
    uint32_t wStatus, i = 60000, j;
    ASSERT((pbyValue == NULL) || (wLen == 0));
    if (wLen == 1) {
        I2C_ClrAllIntFlag();
        I2C_SetDataCmd(I2C_DATACMD_DATA(*pbyValue++) | I2C_DATACMD_CMD_WRITE |
                       I2C_DATACMD_STOP);
        i = 60000;
        do {
            I2C_GetINTRSTAT(wStatus);
        } while (((wStatus & I2C_INTRSTAT_TX_EMPTY) == 0) && (--i != 0));
        I2C_GetINTRSTAT(wStatus);
        if ((i == 0) || ((wStatus & I2C_INTRSTAT_TX_ABRT) != 0))
            return 0;
        return 1;
    }
    I2C_ClrAllIntFlag();
    I2C_SetDataCmd(I2C_DATACMD_DATA(*pbyValue++) | I2C_DATACMD_CMD_WRITE);
    do {
        I2C_GetINTRSTAT(wStatus);
    } while (((wStatus & I2C_INTRSTAT_TX_EMPTY) == 0) && (--i != 0));
    I2C_GetINTRSTAT(wStatus);
    if ((i == 0) || ((wStatus & I2C_INTRSTAT_TX_ABRT) != 0))
        return 0;
    for (j = 0; j < wLen - 2; ++j) {
        I2C_ClrAllIntFlag();
        I2C_SetDataCmd(I2C_DATACMD_DATA(*pbyValue++) | I2C_DATACMD_CMD_WRITE);
        i = 60000;
        do {
            I2C_GetINTRSTAT(wStatus);
        } while (((wStatus & I2C_INTRSTAT_TX_EMPTY) == 0) && (--i != 0));
        if (i == 0)
            return 0;
    }
    I2C_ClrAllIntFlag();
    I2C_SetDataCmd(I2C_DATACMD_DATA(*pbyValue++) | I2C_DATACMD_CMD_WRITE |
                   I2C_DATACMD_STOP);
    do {
        I2C_GetINTRSTAT(wStatus);
    } while (((wStatus & I2C_INTRSTAT_TX_EMPTY) == 0) && (--i != 0));
    I2C_GetINTRSTAT(wStatus);
    if ((i == 0) || ((wStatus & I2C_INTRSTAT_TX_ABRT) != 0))
        return 0;
    return 1;
}

uint32_t I2C_MasterRecieveByte(uint8_t *pbyValue, uint32_t wLen) {
    uint32_t wStatus, i = 60000, j;
    ASSERT((pbyValue == NULL) || (wLen == 0));
    if (wLen == 1) {
        I2C_ClrAllIntFlag();
        I2C_SetDataCmd(I2C_DATACMD_DATA(0) | I2C_DATACMD_CMD_READ |
                       I2C_DATACMD_STOP);
        i = 60000;
        do {
            I2C_GetINTRSTAT(wStatus);
        } while (((wStatus & I2C_INTRSTAT_RX_FULL) == 0) && (--i != 0));
        if (i == 0)
            return 0;
        I2C_GetData(*pbyValue);
        return 1;
    }
    I2C_ClrAllIntFlag();
    I2C_SetDataCmd(I2C_DATACMD_DATA(0) | I2C_DATACMD_CMD_READ);
    do {
        I2C_GetINTRSTAT(wStatus);
    } while (((wStatus & I2C_INTRSTAT_RX_FULL) == 0) && (--i != 0));
    if (i == 0)
        return 0;
    I2C_GetData(*pbyValue++);
    for (j = 0; j < wLen - 2; ++j) {
        I2C_ClrAllIntFlag();
        I2C_SetDataCmd(I2C_DATACMD_DATA(0) | I2C_DATACMD_CMD_READ);
        i = 60000;
        do {
            I2C_GetINTRSTAT(wStatus);
        } while (((wStatus & I2C_INTRSTAT_RX_FULL) == 0) && (--i != 0));
        if (i == 0)
            return 0;
        I2C_GetData(*pbyValue++);
    }
    I2C_ClrAllIntFlag();
    I2C_SetDataCmd(I2C_DATACMD_DATA(0) | I2C_DATACMD_CMD_READ |
                   I2C_DATACMD_STOP);
    do {
        I2C_GetINTRSTAT(wStatus);
    } while (((wStatus & I2C_INTRSTAT_RX_FULL) == 0) && (--i != 0));
    I2C_GetINTRSTAT(wStatus);
    if ((i == 0) || ((wStatus & I2C_INTRSTAT_TX_ABRT) != 0))
        return 0;
    I2C_GetData(*pbyValue);
    return 1;
}

void I2C_IrqHandler(void) {
    uint32_t wStatus = 0;
    cbI2cIntFunc(wStatus);
    I2C_ClrAllIntFlag();
}
