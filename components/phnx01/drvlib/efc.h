/*****************************************
 * @file        efc.h
 * @brief       efc  header file
 * @version     v0.0
 * @date        2019/05/06
 * @author      tangbifei
 * @note      ???Keil??
 * Copyright(C) 2019,  FanHai Data Tech Corp.
 * All rights reserved
 ******************************************/
#ifndef _EFC_H_


#define _EFC_H_
#include <stddef.h>
#include <stdint.h>

//efc_cr   wr protect
#define  EFC_CR_CDINTEN_pos           0
#define  EFC_CR_FCINTEN_pos           1
#define  EFC_CR_PVEINTEN_pos          2
#define  EFC_CR_EVEINTEN_pos          3
#define  EFC_CR_ADDREINTEN_pos        4
#define  EFC_CR_FTTEINTEN_pos         5
#define  EFC_CR_ATTEINTEN_pos         6
#define  EFC_CR_ATDEINTEN_pos         7
#define  EFC_CR_LVDWARNEN_pos         8
#define  EFC_CR_PMRDEN_pos            9
#define  EFC_CR_EMRDEN_pos            10
#define  EFC_CR_NRDDLY_pos            11
#define  EFC_CR_MRDCYC_4BIT(x)        ((x)<<12) //def:9
#define  EFC_CR_MRDCYC_4BIT_pos       12

#define  EFC_CR_INTEN_mask     0xff


#define  EFC_WRPROTECT_CLOSE()       {uint32_t tmp = __get_PRIMASK(); __set_PRIMASK(1); EFC->EFC_WPT = 0xc3;EFC->EFC_WPT = 0x3c;__set_PRIMASK(tmp);}


//efc_opr
#define  EFC_OPRMODE_SIG_PRG          0      //main/nvr/e2
#define  EFC_OPRMODE_ROW_PRG          1     //main/nvr
#define  EFC_OPRMODE_PAGE_ERASE       2     //main/nvr/e2
#define  EFC_OPRMODE_CHIP_ERASE       3     //main/nvr/e2
#define  EFC_OPRMODE_PAGE_LOAD        4     //main/nvr
#define  EFC_OPRMODE_PAGE_ERASE_VERIFY  5  //main/nvr
#define  EFC_OPRMODE_PAGE_PRG_VERIFY    6  //main/nvr


#define  EFC_SetOPRMODE(x)     {uint32_t tmp = __get_PRIMASK(); __set_PRIMASK(1);EFC->EFC_OPR = x;EFC->EFC_OPR = (7<<4) | x;EFC->EFC_OPR = (9<<4) | x;EFC->EFC_OPR = (0x0c<<4) | x;__set_PRIMASK(tmp);} 



//efc_pvev
#define  EFC_PVEV_EVCNT_7BIT_pos       8
#define  EFC_PVEV_PVCNT_7BIT_pos       0


//efc_sts
#define  EFC_STS_CD                (1<<0)
#define  EFC_STS_FCE               (1<<1)
#define  EFC_STS_PVE               (1<<2)
#define  EFC_STS_EVE               (1<<3)
#define  EFC_STS_ADDRE             (1<<4)
#define  EFC_STS_FTTE              (1<<5)
#define  EFC_STS_ATTE              (1<<6)
#define  EFC_STS_ATDE              (1<<7)

#define  EFC_GetStatusFlag(x)      {x = EFC->EFC_STS;}
#define  EFC_ClrStatusFlag(x)      { EFC->EFC_STS = x;}

//efc_sften   wr protect

//efc_sftctl
#define  EFC_SFTCTL_CEb_pos           2
#define  EFC_SFTCTL_WEb_pos           3
#define  EFC_SFTCTL_PRG_pos           4
#define  EFC_SFTCTL_PRG2_pos          5
#define  EFC_SFTCTL_ERASE_pos         6
#define  EFC_SFTCTL_SEL_NVR_pos       0
#define  EFC_SFTCTL_SEL_EEPROM_pos    1
#define  EFC_SFTCTL_SEL_CHIP_pos      7
#define  EFC_SFTCTL_BYTE0_pos         8
#define  EFC_SFTCTL_BYTE1_pos         9
#define  EFC_SFTCTL_BYTE2_pos         10
#define  EFC_SFTCTL_BYTE3_pos         11

#define  EFC_SFTCTL_CEb          (1<<2 )
#define  EFC_SFTCTL_WEb          (1<<3 )
#define  EFC_SFTCTL_PRG          (1<<4 )
#define  EFC_SFTCTL_PRG2         (1<<5 )
#define  EFC_SFTCTL_ERASE        (1<<6 )
#define  EFC_SFTCTL_SEL_NVR      (1<<0 )
#define  EFC_SFTCTL_SEL_EEPROM   (1<<1 )
#define  EFC_SFTCTL_SEL_CHIP     (1<<7 )
#define  EFC_SFTCTL_BYTE0        (1<<8 )
#define  EFC_SFTCTL_BYTE1        (1<<9 )
#define  EFC_SFTCTL_BYTE2        (1<<10)
#define  EFC_SFTCTL_BYTE3        (1<<11)

#define  EFC_SetSFTCTL(x)     {EFC->EFC_SFTCTL |= x;}
#define  EFC_ClrSFTCTL(x)     {EFC->EFC_SFTCTL &=~(x);}


//2M
#define  EFC_TNVS_VAL     0x0d                  //  def 0x0d
#define  EFC_TPROG_VAL    0x0d                  //  def 0x0d 
#define  EFC_TPGS_VAL     0x0c                  //  def 0x0c 
#define  EFC_TRCV_VAL     ((0x0c<<16)|(0x78<<9)|0x1e0)      //  def ((0x0c<<16)|(0x78<<9)|0x1e0)
#define  EFC_TERASE_VAL   ((0x09<<7)|0x46)      //  def ((0x09<<7)|0x46)

#define  EFC_CR_VAL       ((0x09<<12)|(1<<11))  //  def ((0x09<<12)|(1<<11)) 


#define  EFC_FLASH_ADDR_BASE  0x10100000
#define  EFC_FLASH_ADDR_END   0x10120000  //0x1011ffff   //128k

#define  EFC_NVR_ADDR_BASE    0x10140000
#define  EFC_NVR_ADDR_END     0x10141000 //0x10140fff    //4k

#define  EFC_EEPROM_BASE      0x10180000
#define  EFC_EEPROM_END       0x10180400 //0x101803ff    //1k

#define  EFC_PAGE_BUFF_BASE   0x101c0000 
#define  EFC_PAGE_BUFF_END    0x101c0200  //0x101c01ff     //512B


#define  EFC_PRG_WORD        0
#define  EFC_PRG_BYTE        1
#define  EFC_PRG_HWORD       2

#define  EFC_PAGE_SIZE   128
#define  EFC_FLASH_SIZE  (128*1024)

typedef enum _ReturnType{
	EFC_SUCCESS=0,
	EFC_FAIL_PRG,
	EFC_FAIL_RPRG,
	EFC_FAIL_PAGE_ERASE,	
	EFC_FAIL_CHIP_ERASE,
	EFC_FAIL_PV,
	EFC_FAIL_EV = 7,
	EFC_FAIL_FLASH_ADDR_OV,
	EFC_FAIL_FLASH_WR,
	EFC_FAIL_FLASH_VF,

}eReturnType_Def;

typedef struct _EFC_CFG{
    uint32_t wTnvs;
    uint32_t wTprog;
    uint32_t wTpgs;
    uint32_t wTrcv;
    uint32_t wTerase;
	uint32_t wCR;
}EFCInitType_Def;

typedef void (*EFCSignalEvent_Type)(uint32_t wEvent);



//function declare
void EFC_Init(EFCInitType_Def *psInit,EFCSignalEvent_Type cbFunc);
eReturnType_Def EFC_SingleProgram(uint32_t wByteSel,int iAddr,uint32_t wData);
eReturnType_Def EFC_PageProgram(int iAddr);
eReturnType_Def EFC_PageErase(int iAddr);
eReturnType_Def EFC_ChipErase(int iAddr);
eReturnType_Def EFC_PageLoad(int iAddr);
eReturnType_Def EFC_PageEraseVerify(int iAddr);
eReturnType_Def EFC_PageProgramVerify(int iAddr);

eReturnType_Def EFC_FlashWrite(int iAddr,uint32_t *pwData,uint32_t wWordNum,uint32_t wWordPrg);






#endif
