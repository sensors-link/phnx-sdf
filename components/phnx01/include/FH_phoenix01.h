/**************************************************************************//**
 * @file     FH_phoenix01.h
 * @brief    CMSIS Cortex-M0+ Core Peripheral Access Layer Header File for
 *           Device FH_phoenix01
 * @version  V1.00
 * @date     02. Jan 2019
 *
 * @note
 * Copyright (C) 2019 FanHai Data Tech Corp. Limited. All rights reserved.
 *
 * @par
 * FanHai DATA is supplying this software for use with Cortex-M0+
 * processor based microcontrollers.  This file can be freely distributed
 * within development tools that are supporting such ARM based processors.
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/


#ifndef  FH_phoenix01_H

#define FH_Phoenix01_H

#ifdef __cplusplus
 extern "C" {
#endif

/* ToDo: replace 'CMSDK_CM0plus' with your device name; add your doxyGen comment   */
/** @addtogroup CMSDK_CM0plus_Definitions CMSDK_CM0plus Definitions
  This file defines all structures and symbols for CMSDK_CM0plus:
    - registers and bitfields
    - peripheral base address
    - peripheral ID
    - Peripheral definitions
  @{
*/

/******************************************************************************/
/*                Processor and Core Peripherals                              */
/******************************************************************************/
/** @addtogroup CMSDK_CM0plus_CMSIS Device CMSIS Definitions
  Configuration of the Cortex-M0+ Processor and Core Peripherals
  @{
*/

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */
typedef enum IRQn
{
/******  Cortex-M0+ Processor Exceptions Numbers **************************************************/

  NonMaskableInt_IRQn           = -14,      /*!<  2 Non Maskable Interrupt                        */
  HardFault_IRQn                = -13,      /*!<  3 Cortex-M0+ Hard Fault Interrupt               */
  SVCall_IRQn                   = -5,       /*!< 11 Cortex-M0+ SV Call Interrupt                  */
  PendSV_IRQn                   = -2,       /*!< 14 Cortex-M0+ Pend SV Interrupt                  */
  SysTick_IRQn                  = -1,       /*!< 15 Cortex-M0+ System Tick Interrupt              */

/******  CMSDK Specific Interrupt Numbers *******************************************************/
  PMU_IRQn		                = 0,       	/*!< PMU Interrupt		                         		*/
  LPT_IRQn						= 1,       	/*!< LPT Interrupt 		                              	*/
  TIMER0_IRQn	                = 2,       	/*!< TIMER1 Interrupt	                              	*/																				 /*!< IRQ2 - Unused                               */
  TIMER1_IRQn	    		    = 3,	   	/*!< TIMER2 Interrupt									*/
  TIMER2_IRQn 	              	= 4,	   	/*!< TIMER3 Interrupt								  	*/
  TIMER3_IRQn 				    = 5,	   	/*!< TIMER4 Interrupt		   						  	*/ 
  UART1_IRQn       			    = 6,       	/*!< UART1 Interrupt                       			  	*/
  UART2_IRQn					= 7,	   	/*!< UART2 Interrupt								  	*/
  SPI_IRQn					  	= 8,	   	/*!< SPI Interrupt								      	*/
  ANAC_IRQn					  	= 9,	   	/*!< ANAC Interrupt								   	 	*/
  EFC_IRQn					  	= 10,		/*!< EFC Interrupt								   	  	*/
  IOM_IRQn               		= 11,       /*!< IOM Interrupt                                  	*/
  I2C_IRQn             		    = 12,       /*!< I2C Interrupt                                  	*/
  CRC_IRQn              		= 13,      	/*!< CRC Interrupt                              	  	*/                                                                            
} IRQn_Type;

/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M0+ Processor and Core Peripherals */
#define __CM0_REV                 0x0000    /*!< Core Revision r2p1                               */
#define __NVIC_PRIO_BITS          2         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */
#define __MPU_PRESENT             0         /*!< MPU present or not                               */
#define __VTOR_PRESENT            1         /*!< Cortex-M0+ can support the VTOR                  */

/*@}*/ /* end of group CMSDK_CM0plus_CMSIS */


#include "core_cm0plus.h"                   /* Cortex-M0+ processor and core peripherals         */
//#include "system_phoenix01.h"               /* CMSDK_CM0plus System  include file                */


/* ========================================  Start of section using anonymous unions  ======================================== */
#if   defined (__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined (__ICCARM__)
  #pragma language=extended
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wc11-extensions"
  #pragma clang diagnostic ignored "-Wreserved-id-macro"
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning 586
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif

/*--------------------------- Micro Trace Buffer -----------------------------*/
typedef struct
{
  __IO uint32_t POSITION;                    /*!< Offset: 0x000 (R/W) MTB Position Register                  */
  __IO uint32_t MASTER;                      /*!< Offset: 0x004 (R/W) MTB Master Register                    */
  __IO uint32_t FLOW;                        /*!< Offset: 0x008 (R/w) MTB Flow Register                      */
  __I  uint32_t BASE;                        /*!< Offset: 0x00C (R/ ) MTB Base Register                      */
       uint32_t RESERVED0[956];
  __IO uint32_t ITCTRL;                      /*!< Offset: 0xF00 (R/W) MTB Integration Mode Control Register  */
       uint32_t RESERVED1[39];
  __IO uint32_t CLAIMSET;                    /*!< Offset: 0xFA0 (R/W) MTB Claim Set Register                 */
  __IO uint32_t CLAIMCLR;                    /*!< Offset: 0xFA4 (R/W) MTB Claim Clear Register               */
       uint32_t RESERVED2[2];
  __IO uint32_t LOCKACCESS;                  /*!< Offset: 0xFB0 (R/W) MTB Lock Access Register               */
  __I  uint32_t LOCKSTATUS;                  /*!< Offset: 0xFB4 (R/ ) MTB Lock Status Register               */
  __I  uint32_t AUTHSTATUS;                  /*!< Offset: 0xFB8 (R/ ) MTB Authentication Status Register     */
  __I  uint32_t DEVARCH;                     /*!< Offset: 0xFBC (R/ ) MTB Device Architecture Register       */
       uint32_t RESERVED3[2];
  __I  uint32_t DEVID;                       /*!< Offset: 0xFC8 (R/ ) MTB Device Configuration Register      */
  __I  uint32_t DEVTYPE;                     /*!< Offset: 0xFCC (R/ ) MTB Device Type Register               */
  __I  uint32_t PID4;                        /*!< Offset: 0xFD0 (R/ ) CoreSight register                     */
  __I  uint32_t PID5;                        /*!< Offset: 0xFD4 (R/ ) CoreSight register                     */
  __I  uint32_t PID6;                        /*!< Offset: 0xFD8 (R/ ) CoreSight register                     */
  __I  uint32_t PID7;                        /*!< Offset: 0xFDC (R/ ) CoreSight register                     */
  __I  uint32_t PID0;                        /*!< Offset: 0xFE0 (R/ ) CoreSight register                     */
  __I  uint32_t PID1;                        /*!< Offset: 0xFE4 (R/ ) CoreSight register                     */
  __I  uint32_t PID2;                        /*!< Offset: 0xFE8 (R/ ) CoreSight register                     */
  __I  uint32_t PID3;                        /*!< Offset: 0xFEC (R/ ) CoreSight register                     */
  __I  uint32_t CID0;                        /*!< Offset: 0xFF0 (R/ ) CoreSight register                     */
  __I  uint32_t CID1;                        /*!< Offset: 0xFF4 (R/ ) CoreSight register                     */
  __I  uint32_t CID2;                        /*!< Offset: 0xFF8 (R/ ) CoreSight register                     */
  __I  uint32_t CID3;                        /*!< Offset: 0xFFC (R/ ) CoreSight register                     */
} MTB_Type;



/*------------- Universal Asynchronous Receiver Transmitter (UART) -----------*/
/** @addtogroup CMSDK_UART CMSDK Universal Asynchronous Receiver/Transmitter
  memory mapped structure for CMSDK_UART
  @{
*/
typedef struct
{
	union
	{
		__I 	uint32_t	RBR;		//0x00
		__O 	uint32_t	THR;
		__IO	uint32_t	DLL;
	};
	union
	{
		__IO	uint32_t	DLH;		//0x04
		__IO	uint32_t	IER;
	};
	union
	{
		__I 	uint32_t	IIR;		//0x08
		__O 	uint32_t	FCR;
	};
	__IO		uint32_t	LCR;		//0x0c
	__IO		uint32_t	MCR;		//0x10
	__IO		uint32_t	LSR;		//0x14
	__I 		uint32_t	MSR;		//0x18
	__IO		uint32_t	TR; 		//0x1c
	__IO		uint32_t	LPDLL;		//0x20
	__IO		uint32_t	LPDLH;		//0x24
	__I			uint32_t	RESERVED0[2];	//0x28, 0x2c
	union
	{
		__I 	uint32_t	SRBR[16];	//0x30 - 0x6c
		__O 	uint32_t	STHR[16];
	};
	__IO		uint32_t	FACR;		//0x70
	__I 		uint32_t	TFIFOR; 	//0x74
	__O 		uint32_t	RFIFOW; 	//0x78
	__I 		uint32_t	USR;		//0x7c
	__I 		uint32_t	TFL;		//0x80
	__I 		uint32_t	RFL;		//0x84
	__O 		uint32_t	SRR;		//0x88
	__IO		uint32_t	SRTS;		//0x8c
	__IO		uint32_t	SBC;		//0x90
	__IO		uint32_t	SDMAE;		//0x94
	__IO		uint32_t	SFE;		//0x98
	__IO		uint32_t	SRFR;		//0x9c
	__IO		uint32_t	STER;		//0xa0
	__IO		uint32_t	HTX;		//0xa4
	__O 		uint32_t	DMASA;		//0xa8
	__IO		uint32_t	TCR;		//0xac
	__IO		uint32_t	DER;		//0xb0
	__IO		uint32_t	RER;		//0xb4
	__IO		uint32_t	DET;		//0xb8
	__IO		uint32_t	TAT;		//0xbc
	__IO		uint32_t	DFR;		//0xc0
	__IO		uint32_t	RAR;		//0xc4
	__IO		uint32_t	TAR;		//0xc8
	__IO		uint32_t	ELCR;		//0xcc
	__I 		uint32_t	RESERVED[9];
	__I 		uint32_t	CPR;		//0xf4
	__I 		uint32_t	CPV;		//0xf8
	__I 		uint32_t	CPTR;		//0xfc

} UART_TypeDef;


/*----------------------------- Timer (TIMER) -------------------------------*/
/** @addtogroup CMSDK_TIMER CMSDK Timer
  @{
*/
typedef struct
{
  __IO   uint32_t  LPTCR;          	//0x00
  __IO    uint32_t  LPTCFG;        		//0x04
  __I   uint32_t  LPTCNT;          	//0x08
  __IO 	 uint32_t  LPTINTSTS;          	//0x0c
} LPT_TypeDef;


typedef struct
{
  __IO   uint32_t  LC1R;          	//0x00
  __I    uint32_t  CVR;        		//0x04
  __IO   uint32_t  CR;          	//0x08
  __I 	 uint32_t  SICR;          	//0x0c
  __I    uint32_t  SISR;          	//0x10
} TIMER_TypeDef;

typedef struct
{
	__I    uint32_t  ISR;	        //0xa0
	__I    uint32_t  ICR;	        //0xA4
	__I    uint32_t  RISR;          //0xA8
	__I    uint32_t  CPV;	        //0xAC
	__IO   uint32_t  LC2R_0;        //0xB0
	__IO   uint32_t  LC2R_1;        //0xB4
	__IO   uint32_t  LC2R_2;        //0xB8
	__IO   uint32_t  LC2R_3;        //0xBC
} TIMER_COMMON_TypeDef;




/*------------- System Control (SYSC) --------------------------------------*/
/** @addtogroup CMSDK_SYSC CMSDK System Control
  @{
*/
typedef struct
{
  __IO   uint32_t  SYSCTRCFG;      		//0x00
  __IO   uint32_t  WRPROCFG; 			//0x04
  __IO   uint32_t  CLKENCFG;			//0x08
  __IO   uint32_t  MSFTRSTCFG;	        //0x0c
  __IO	 uint32_t  NVCLKDIV;			//0x10
  __IO	 uint32_t  TESTCKSEL;			//0x14
  __IO	 uint32_t  ANCLKDIV ;			//0x14
} SYSC_TypeDef;


/*@}*/ /* end of group CMSDK_SYSCON */



/*------------------- Watchdog ----------------------------------------------*/
/** @addtogroup CMSDK_Watchdog CMSDK Watchdog
  @{
*/
typedef struct
{
	__IO		uint32_t	WDTRST;											//0x00
	__IO		uint32_t	WDTCR;											//0x04
	__I			uint32_t	CCV;											//0x08
	__O			uint32_t	CRR;											//0x0C
	__I			uint32_t	ISR;											//0x10
	__I			uint32_t	ICR;											//0x14

} WDT_TypeDef;

/*@}*/ /* end of group  CMSDK_Watchdog */


/*------------------- Flash Control --------------------------------------*/
/** @addtogroup Flash Control
  @{
*/

typedef struct
{
		__IO		uint32_t	EFC_CR;							//0x00
		__IO		uint32_t	EFC_Tnvs;						//0x04
		__IO		uint32_t	EFC_Tprog;						//0x08
		__IO		uint32_t	EFC_Tpgs;						//0x0C
		__IO		uint32_t	EFC_Trcv;						//0x10
		__IO		uint32_t	EFC_Terase;						//0x14
		__IO		uint32_t	EFC_WPT;						//0x18
		__IO		uint32_t	EFC_OPR;						//0x1C
		__IO		uint32_t	EFC_PVEV;						//0x20
		__IO		uint32_t	EFC_STS;						//0x24
		__IO		uint32_t	EFC_SFTEN;						//0x28
		__IO		uint32_t	EFC_SFTCTL;						//0x2C
		__IO		uint32_t	EFC_SFTADR;						//0x30
		__IO		uint32_t	EFC_SFTDAT;						//0x34
} EFC_TypeDef;


/*@}*/ /* end of group Flash Control */


/*------------------- IOM --------------------------------------*/
/** @addtogroup IOM
  @{
*/

typedef struct
{
		__IO		uint32_t	GPIO_DATA;			//0x00
		__IO		uint32_t	GPIO_DIR;			//0x04
		__IO		uint32_t	GPIO_PU;			//0x08
		__IO		uint32_t	GPIO_PD;			//0x0C
		__IO		uint32_t	GPIO_OTYPE;			//0x10
		__IO		uint32_t	GPIO_ADS;			//0x14
		__IO		uint32_t	GPIO_DRS;			//0x18
		__IO		uint32_t	GPIO_AF0;			//0x1C
		__IO		uint32_t	GPIO_AF1;			//0x20
		__IO		uint32_t	GPIO_INT_TYPE;		//0x24
		__IO		uint32_t	GPIO_INT_POLARITY;	//0x28
		__IO		uint32_t	GPIO_EXT_INTE;		//0x2C
		__IO		uint32_t	GPIO_INTF;			//0x30
		__IO		uint32_t	GPIO_CTL;			//0x34

} IOM_TypeDef;

/*@}*/ /* end of group EEPROM Control */


/*------------------- I2C --------------------------------------*/
/** @addtogroup I2C
  @{
*/

typedef struct
{
		__IO		uint32_t	CON;									//0x00
		__IO		uint32_t	TAR;									//0x04
		__IO		uint32_t	SAR;									//0x08
		__IO		uint32_t	HS_MADDR;							//0x0c
		__IO		uint32_t	DATACMD;							//0x10
		__IO		uint32_t	SSHCNT;					//0x14
		__IO		uint32_t	SSLCNT;					//0x18
		__IO		uint32_t	FS_SCL_HCNT;					//0x1c
		__IO		uint32_t	FS_SCL_LCNT;					//0x20
		__IO		uint32_t	HS_SCL_HCNT;					//0x24
		__IO		uint32_t	HS_SCL_LCNT;					//0x28
		__I			uint32_t	INTRSTAT;						//0x2c
		__IO		uint32_t	INTRMSK;						//0x30
		__I			uint32_t	RAW_INTR_MASK;				//0x34
		__IO		uint32_t	RX_TL;								//0x38
		__IO		uint32_t	TX_TL;								//0x3c
		__I			uint32_t	CLRINTR;							//0x40
		__I			uint32_t	CLRRUND;					//0x44
		__I			uint32_t	CLRRXOVER;					//0x48
		__I			uint32_t	CLRTXOVER;					//0x4c
		__I			uint32_t	CLRRDREQ;						//0x50
		__I			uint32_t	CLRTXARBT;					//0x54
		__I			uint32_t	CLRRXDONE;					//0x58
		__I			uint32_t	CLRACTIVITY;					//0x5c
		__I			uint32_t	CLRSTOPDET;					//0x60
		__I			uint32_t	CLRSTARTDET;				//0x64
		__I			uint32_t	CLRGENCALL;					//0x68
		__IO		uint32_t	ENABLE;								//0x6c
		__I			uint32_t	STATUS;								//0x70
		__IO		uint32_t	TXFLR;								//0x74
		__IO		uint32_t	RXFLR;								//0x78
		__IO		uint32_t	SDAHOLD;							//0x7c
		__I			uint32_t	ABRTSTUS;				//0x80
		__IO		uint32_t	RESERVED0[4];					
		__IO        uint32_t    SDASETUP;                  //94
		__IO		uint32_t	ACKGENELCALL;			//0x98
		__I			uint32_t	I2CENSTAS;				//0x9c		
		__IO		uint32_t	FSSPKLEN;						//0xa0
		__IO		uint32_t	HS_SPKLEN;						//0xa4
		__I			uint32_t	CLR_RESTART_DET;			//0xa8
		__I			uint32_t	RESERVED1[18];				//0xac
		__I			uint32_t	COMP_PARAM;						//0xf4

} I2C_TypeDef;


/*------------------- SPI --------------------------------------*/
/** @addtogroup SPI
  @{
*/

typedef struct
{
		__IO		uint32_t	CONTROL0;							//0x00
		__IO		uint32_t	CONTROL1;							//0x04
		__IO		uint32_t	ENABLE;								//0x08
		__IO		uint32_t	MWCR;		//0x0c
		__IO		uint32_t	SER;					//0x10
		__IO		uint32_t	BAUDR;					//0x14
		__IO		uint32_t	TFT;			//0x18
		__IO		uint32_t	RFT;			//0x1c		
		__IO		uint32_t	TFL;				//0x20
		__IO		uint32_t	RFL;				//0x24
		__IO		uint32_t	SR;								//0x28
		__IO		uint32_t	IMR;						//0x2c
		__IO		uint32_t	ISR;					//0x30
//		__IO		uint32_t	RISR;			//0x34
	  __I     uint32_t  RSV[11];
		__IO		uint32_t	DR0;			//0x60
		
} SPI_TypeDef;


/*------------------- PMU --------------------------------------*/
/** @addtogroup PMU
  @{
*/

typedef struct
{
		__IO		uint32_t	PMU_CR;			//0x00
		__IO		uint32_t	PMU_WKCFG;		//0x04
		__O			uint32_t	PMU_SOFTRST;	//0x08
		__IO		uint32_t	PMU_WKSTS;		//0x0C
		__IO		uint32_t	PMU_RSTSTS;		//0x10
		__IO		uint32_t	PMU_INTEN;		//0x14
		__O			uint32_t	PMU_WPT;		//0x18
		__IO		uint32_t	PMU_BAKRx[32];	//0x20
} PMU_TypeDef;


/*------------------- swap registers --------------------------------------*/
/** @addtogroup swap registers
  @{
*/
/* RAM reserved for communication between TB and SOFTWARE					*/
typedef struct
{
		__IO		uint32_t	TC_NUM;			//0x00
		__IO		uint32_t	TC_PARA1;		//0x04
		__IO		uint32_t	TC_PARA2;		//0x08
		__IO		uint32_t	TC_PARA3;		//0x0C
		__IO		uint32_t	TC_PARA4;		//0x10
		__IO		uint32_t	TC_PARA5;		//0x14
		__IO		uint32_t	TC_PARA6;		//0x28
		__IO		uint32_t	TC_PARA7;		//0x1C
		__IO		uint32_t	TC_PARA8;		//0x20
} TCSWAP_TypeDef;


/*@}*/ /* end of group Hibernate Control */

/*------------------- Analog Controller --------------------------------------*/
/** @addtogroup Analog Controller
  @{
*/

typedef struct
{
		__IO		uint32_t	RES_CFG;			//0x00
		__IO		uint32_t	LED_CFG;			//0x04
		__IO		uint32_t	PDSENS_CFG;			//0x08
		__IO		uint32_t	DC_CFG;				//0x0C
		__IO		uint32_t	CMP_CFG;			//0x10
		__IO		uint32_t	ADC_CFG;			//0x14
		__IO		uint32_t	ADC_CTL;			//0x18
		__IO		uint32_t	ANAC_CFG;			//0x1C
		__IO		uint32_t	LVD_CFG;			//0x20
		__IO		uint32_t	CLK_CFG;			//0x24
		__IO		uint32_t	HRC_TRIM;			//0x28
		__IO		uint32_t	LRC_TRIM;			//0x2c
		__IO		uint32_t	CMP_TRIM;			//0x30
		__IO		uint32_t	BGR_TRIM;			//0x34
		__IO		uint32_t	LDO_TRIM;			//0x38
		__IO		uint32_t	ANA_RES0;			//0x3C
		__IO		uint32_t	ANA_RES1;			//0x40
		__IO		uint32_t	ANA_RES2;			//0x44
		__IO		uint32_t	DLY_CFG;			//0x48
		__IO		uint32_t	ME_CTL;				//0x4C
		__IO		uint32_t	ANAC_FLAG;			//0x50
		__IO		uint32_t	ANAC_TEST;			//0x54	
		__IO		uint32_t	WPROT;				//0x58	
		__IO		uint32_t	RES_CFG1;			//0x5C
		__IO		uint32_t	RES_CFG2;			//0x60
		__IO		uint32_t	RES_CFG3;			//0x64
		__IO		uint32_t	RES_CFG4;			//0x68
		__IO		uint32_t	RES_CFG5;			//0x6C
		__IO		uint32_t	RES_CFG6;			//0x70
		__IO		uint32_t	RES_CFG7;			//0x74
		__IO		uint32_t	RES_CFG8;			//0x78
		__IO		uint32_t	RES_CFG9;			//0x7C
		__IO		uint32_t	ADC_VAL0;			//0x80
		__IO		uint32_t	ADC_VAL1;			//0x84
		__IO		uint32_t	ADC_VAL2;			//0x88
		__IO		uint32_t	ADC_VAL3;			//0x8C
		__IO		uint32_t	ADC_VAL4;			
		__IO		uint32_t	ADC_VAL5;			
		__IO		uint32_t	ADC_VAL6;			
		__IO		uint32_t	ADC_VAL7;			//
		__IO		uint32_t	TEMP_VAL0;			//0xa0~0xbc
		__IO		uint32_t	TEMP_VAL1;			//
		__IO		uint32_t	TEMP_VAL2;			//
		__IO		uint32_t	TEMP_VAL3;			//
		__IO		uint32_t	TEMP_VAL4;			//
		__IO		uint32_t	TEMP_VAL5;			//	
		__IO		uint32_t	TEMP_VAL6;			//
		__IO		uint32_t	TEMP_VAL7;			//
		__IO		uint32_t	DARKC_VAL0;			////0xc0~0xdc
		__IO		uint32_t	DARKC_VAL1;			//
		__IO		uint32_t	DARKC_VAL2;			//
		__IO		uint32_t	DARKC_VAL3;			//
		__IO		uint32_t	DARKC_VAL4;			//
		__IO		uint32_t	DARKC_VAL5;			//		
		__IO		uint32_t	DARKC_VAL6;			//
		__IO		uint32_t	DARKC_VAL7;			//
		__IO		uint32_t	SMOKE_VAL0;			////0xe0~0xfc
		__IO		uint32_t	SMOKE_VAL1;			//
		__IO		uint32_t	SMOKE_VAL2;			//
		__IO		uint32_t	SMOKE_VAL3;			//
		__IO		uint32_t	SMOKE_VAL4;			//
		__IO		uint32_t	SMOKE_VAL5;			//	
		__IO		uint32_t	SMOKE_VAL6;			//
		__IO		uint32_t	SMOKE_VAL7;			//	
} ANAC_TypeDef;

/*@}*/ /* end of group Smoke Detect */



/* =========================================  End of section using anonymous unions  ========================================= */
#if   defined (__CC_ARM)
  #pragma pop
#elif defined (__ICCARM__)
  /* leave anonymous unions enabled */
#elif (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic pop
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning restore
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif

/*@}*/ /* end of group CMSDK_CM0plus_Peripherals */


/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/* ToDo: add here your device peripherals base addresses
         following is an example for timer                                    */
/** @addtogroup CMSDK_CM0plus_MemoryMap CMSDK_CM0plus Memory Mapping
  @{
*/

/* Peripheral and SRAM base address */
#define FLASH_BASE 			(0x10100000UL)     /*!< (FLASH     ) Base Address */
#define NVR_BASE   			(0x10140000UL)     /*!< (NVR       ) Base Address */
#define EEPROM_BASE			(0x10180000UL)     /*!< (EEPROM    ) Base Address */
#define PGBUF_BASE 			(0x101C0000UL)     /*!< (PageBuf   ) Base Address */
#define SRAM_BASE  			(0x20000000UL)     /*!< (SRAM      ) Base Address */

#define AHB_BASE    		(0x40000000UL)
#define APB_BASE    		(0x40010000UL)
#define MTBSFR_BASE 		(0xF0200000UL)
#define MTBSRAM_BASE		(0xF0210000UL)


/* APB peripherals                                                           */
#define TIMER0_BASE     	(APB_BASE + 0x0000UL)
#define TIMER1_BASE     	(APB_BASE + 0x0014UL)
#define TIMER2_BASE     	(APB_BASE + 0x0028UL)
#define TIMER3_BASE     	(APB_BASE + 0x003CUL)
#define TIMER_COMMON_BASE   (APB_BASE + 0x00A0UL)

#define I2C_BASE       		(APB_BASE + 0x0400UL)
#define SPI_BASE	      	(APB_BASE + 0x0800UL)
#define UART2_BASE        	(APB_BASE + 0x0C00UL)
#define UART1_BASE        	(APB_BASE + 0x1000UL)
#define ANAC_BASE        	(APB_BASE + 0x1C00UL)
#define SYSC_BASE        	(APB_BASE + 0x2000UL)
#define IOM_BASE        	(APB_BASE + 0x2400UL)
#define PMU_BASE        	(APB_BASE + 0x2C00UL)
#define LPT_BASE        	(APB_BASE + 0x3000UL)
#define WDT_BASE        	(APB_BASE + 0x3400UL)

/* AHB peripherals                                                           */
#define EFC_BASE			(AHB_BASE + 0x0000UL)
#define EEC_BASE			(AHB_BASE + 0x0400UL)
#define SRAMC_BASE			(AHB_BASE + 0x0800UL)
#define CRC_BASE			(AHB_BASE + 0x0C00UL)

#define TCSWAP_BASE			(SRAM_BASE + 0x1800UL)

/*@}*/ /* end of group CMSDK_CM0plus_MemoryMap */

#define	SET_BIT(v, n)		(v) |= (1U << (n))
#define	CLR_BIT(v, n)		(v) &= ~(1U << (n))
#define	GET_BIT(v, n)		(v) &= (1U << (n))

#define SYS_CLK_FREQ		(8) //(32) //(8)

/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/

/** @addtogroup CMSDK_CM0plus_PeripheralDecl CMSDK_CM0plus Peripheral Declaration
  @{
*/

#define TIMER0         ((TIMER_TypeDef    *) TIMER0_BASE  )
#define TIMER1         ((TIMER_TypeDef    *) TIMER1_BASE  )
#define TIMER2         ((TIMER_TypeDef    *) TIMER2_BASE  )
#define TIMER3         ((TIMER_TypeDef    *) TIMER3_BASE  )
#define TIMER_COMMON    ((TIMER_COMMON_TypeDef  *) TIMER_COMMON_BASE  )


#define I2C             ((I2C_TypeDef       *) I2C_BASE     )
#define SPI             ((SPI_TypeDef       *) SPI_BASE     )
#define UART2           ((UART_TypeDef      *) UART2_BASE   )
#define UART1           ((UART_TypeDef      *) UART1_BASE   )
#define ANAC            ((ANAC_TypeDef      *) ANAC_BASE    )
#define SYSC            ((SYSC_TypeDef      *) SYSC_BASE    )
#define IOM             ((IOM_TypeDef       *) IOM_BASE     )
#define PMU             ((PMU_TypeDef       *) PMU_BASE     )
#define LPT             ((LPT_TypeDef       *) LPT_BASE     )
#define WDT             ((WDT_TypeDef       *) WDT_BASE     )

#define EFC           	((EFC_TypeDef       *) EFC_BASE    	)
//#define CRC             ((CRC_TypeDef       *) CRC_BASE     )

#define TCSWAP			((TCSWAP_TypeDef	*) TCSWAP_BASE	)

/*@}*/ /* end of group CMSDK_CM0plus_PeripheralDecl */

/******************************************************************************/
/*                         Declaration                                        */
/******************************************************************************/
#define MTB				((MTB_Type 			*) MTBSFR_BASE	)

/*@}*/ /* end of group CMSDK_CM0plus_Definitions */

#ifdef __cplusplus
}
#endif


#define REG32(addr)		(*(volatile unsigned int   * )(addr))
#define REG16(addr)		(*(volatile unsigned short * )(addr))
#define REG8(addr)		(*(volatile unsigned char  * )(addr))

#endif  /* CMSDK_CM0plus_H */
