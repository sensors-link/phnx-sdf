#ifndef _ANAC_H_
#define _ANAC_H_
#include <stddef.h>
#include <stdint.h>


//led_cfg
#define ANAC_LED_CFG_LED_SEL_BLUE     (1<<13)
#define ANAC_LED_CFG_LED_SEL_IRLED    (0<<13)

#define ANAC_LED_CFG_INDLED_EN        (1<<12)
#define ANAC_LED_CFG_INDLED_DIS       (0<<12)

#define ANAC_LED_CFG_IBLUE_CC_5BIT(x) (x<<7)  //0-31:0-310mA  step:10mA
#define ANAC_LED_CFG_IBLUE_EN         (1<<6)
#define ANAC_LED_CFG_IBLUE_DIS        (0<<6)

#define ANAC_LED_CFG_IIRLED_CC_5BIT(x) (x<<1)  //0-31:0-310mA  step:10mA
#define ANAC_LED_CFG_IIRLED_EN         (1<<0)
#define ANAC_LED_CFG_IIRLED_DIS        (0<<0)

//pdsens_cfg
#define ANAC_PDSENS_CFG_PDSENS_CC_1BIT(x)   (x<<6)
#define ANAC_PDSENS_CFG_PDSENS_GC_4BIT(x)   (x<<2)
#define ANAC_PDSENS_CFG_PDSENS_RST_1BIT(x)  (x<<1)
#define ANAC_PDSENS_CFG_PDSENS_EN           (1<<0)
#define ANAC_PDSENS_CFG_PDSENS_DIS          (0<<0)

//dc_cfg
#define ANAC_DC_CFG_DCDC_EN                        (1<<0)
#define ANAC_DC_CFG_DCDC_DIS                       (0<<0)
#define ANAC_DC_CFG_DCDC_VSL_2BIT(x)               (x<<3)  //0 3.6v,1 4.0v,2 4.4v,3 4.8v
#define ANAC_DC_CFG_DCDC_IPK_2BIT(x)               (x<<1) //0 1.1mA,

//cmp_cfg
#define ANAC_CMP_CFG_CMP_FLT_4BIT(x)               (x<<10)
#define ANAC_CMP_CFG_CMP_HYSSEL_2BIT(x)            (x<<8)
#define ANAC_CMP_CFG_CMP_NINSEL_3BIT(x)            (x<<5)
#define ANAC_CMP_CFG_CMP_PINSEL_3BIT(x)            (x<<2)
#define ANAC_CMP_CFG_CMP_INV_1BIT(x)               (x<<1)
#define ANAC_CMP_CFG_CMP_EN                        (1<<0)
#define ANAC_CMP_CFG_CMP_DIS                       (1<<0)

#define ANAC_CMP_CFG_HYSSEL_50MV         3
#define ANAC_CMP_CFG_HYSSEL_25MV         2
#define ANAC_CMP_CFG_HYSSEL_12P5MV       1
#define ANAC_CMP_CFG_HYSSEL_0MV          0

#define ANAC_CMP_CFG_NINSEL_VBGS         7
#define ANAC_CMP_CFG_NINSEL_VBGA         6
#define ANAC_CMP_CFG_NINSEL_V1P5S        5
#define ANAC_CMP_CFG_NINSEL_V1P5D        4
#define ANAC_CMP_CFG_NINSEL_VREF_VO      3
#define ANAC_CMP_CFG_NINSEL_VC1N2        2
#define ANAC_CMP_CFG_NINSEL_VC1N1        1
#define ANAC_CMP_CFG_NINSEL_VC1N0        0

#define ANAC_CMP_CFG_PINSEL_BVS_VO       7
#define ANAC_CMP_CFG_PINSEL_VC1P6        6
#define ANAC_CMP_CFG_PINSEL_VC1P5        5
#define ANAC_CMP_CFG_PINSEL_VC1P4        4
#define ANAC_CMP_CFG_PINSEL_VC1P3        3
#define ANAC_CMP_CFG_PINSEL_VC1P2        2
#define ANAC_CMP_CFG_PINSEL_VC1P1        1
#define ANAC_CMP_CFG_PINSEL_VC1P0        0

//adc_cfg
#define ANAC_ADC_CFG_ADC_CHSEL_4BIT(x)    (x<<7)
#define ANAC_ADC_CFG_ADC_BUFEN            (1<<6)
#define ANAC_ADC_CFG_ADC_BUFDIS           (0<<6)
#define ANAC_ADC_CFG_ADC_VRSEL_2BIT(x)    (x<<4)
#define ANAC_ADC_CFG_ADC_VRVOL_3BIT(x)    (x<<1)
#define ANAC_ADC_CFG_ADC_VREN             (1<<0)
#define ANAC_ADC_CFG_ADC_VRDIS            (0<<0)

#define ANAC_ADC_CFG_VRVOL_2P5     0
#define ANAC_ADC_CFG_VRVOL_1P5     1
#define ANAC_ADC_CFG_VRVOL_1P2     2
#define ANAC_ADC_CFG_VRVOL_1P1     3
#define ANAC_ADC_CFG_VRVOL_3P7     4
#define ANAC_ADC_CFG_VRVOL_4P3     5
#define ANAC_ADC_CFG_VRVOL_0P55    6
#define ANAC_ADC_CFG_VRVOL_RVS     7

#define ANAC_ADC_CFG_VRSEL_IINVREF     0
#define ANAC_ADC_CFG_VRSEL_EXTADC_VR   1
#define ANAC_ADC_CFG_VRSEL_ADC_VCC     2

#define ANAC_ADC_CFG_CHSEL_TSENS       0x0e
#define ANAC_ADC_CFG_CHSEL_BVS_VO      0x0d
#define ANAC_ADC_CFG_CHSEL_PDSENS_VO   0x0c
#define ANAC_ADC_CFG_CHSEL_AIN11       0x0b
#define ANAC_ADC_CFG_CHSEL_AIN10       0x0a
#define ANAC_ADC_CFG_CHSEL_AIN9        0x09
#define ANAC_ADC_CFG_CHSEL_AIN8        0x08
#define ANAC_ADC_CFG_CHSEL_AIN7        0x07
#define ANAC_ADC_CFG_CHSEL_AIN6        0x06
#define ANAC_ADC_CFG_CHSEL_AIN5        0x05
#define ANAC_ADC_CFG_CHSEL_AIN4        0x04
#define ANAC_ADC_CFG_CHSEL_AIN3        0x03
#define ANAC_ADC_CFG_CHSEL_AIN2        0x02
#define ANAC_ADC_CFG_CHSEL_AIN1        0x01
#define ANAC_ADC_CFG_CHSEL_AIN0        0x00

//adc_ctl
#define ANAC_ADC_CTL_SMP_TIMES_1       0
#define ANAC_ADC_CTL_SMP_TIMES_2       1
#define ANAC_ADC_CTL_SMP_TIMES_4       2
#define ANAC_ADC_CTL_SMP_TIMES_8       3

#define ANAC_ADC_CTL_SMP_TIMES_pos     6

#define ANAC_ADC_CTL_SMP_CYCLE_16      0
#define ANAC_ADC_CTL_SMP_CYCLE_12      1
#define ANAC_ADC_CTL_SMP_CYCLE_8       2
#define ANAC_ADC_CTL_SMP_CYCLE_4       3
#define ANAC_ADC_CTL_SMP_CYCLE_pos     4

#define ANAC_ADC_CTL_MULTI_MODE_pos    3

#define ANAC_ADC_CTL_SST_pos           2

#define ANAC_ADC_CTL_ADC_RST_pos       1

#define ANAC_ADC_CTL_ADC_EN_pos        0

#define ANAC_SetADC_CTL(x)      {ANAC->ADC_CTL |= x;}
#define ANAC_ClrADC_CTL(x)      {ANAC->ADC_CTL &=~(x);}

//anac_cfg
#define ANAC_ANAC_CFG_SLDO_VRSEL_2BIT(x)        (x<<0)
#define ANAC_ANAC_CFG_ALDO4D_EN                 (1<<2)
#define ANAC_ANAC_CFG_ALDO4D_DIS                (0<<2)
#define ANAC_ANAC_CFG_ALDO4D_LPEN               (1<<3)
#define ANAC_ANAC_CFG_ALDO4D_LPDIS              (0<<3)
#define ANAC_ANAC_CFG_ALDO4D_IBSEL_2BIT(x)      (x<<4)
#define ANAC_ANAC_CFG_BGR_EN                    (1<<6)
#define ANAC_ANAC_CFG_BGR_DIS                   (0<<6)
#define ANAC_ANAC_CFG_ALDO4A_EN                 (1<<7)
#define ANAC_ANAC_CFG_ALDO4A_DIS                (0<<7)
#define ANAC_ANAC_CFG_ALDO4A_VOSEL_2BIT(x)      (x<<8)
#define ANAC_ANAC_CFG_BVS_ISNKSEL_4BIT(x)       (x<<11)
#define ANAC_ANAC_CFG_THERMD_EN                 (1<<15)
#define ANAC_ANAC_CFG_THERMD_DIS                (0<<15)
#define ANAC_ANAC_CFG_INTF_ISOEN                (1<<16)
#define ANAC_ANAC_CFG_INTF_ISODIS               (0<<16)

//lvd_cfg
#define ANAC_LVD_CFG_LVD_DIS                    (0<<0)
#define ANAC_LVD_CFG_LVD_EN                     (1<<0)
#define ANAC_LVD_CFG_LVD_SRCSEL_2BIT(x)         (x<<1)
#define ANAC_LVD_CFG_LVD_VTSET_4BIT(x)          (x<<3)
#define ANAC_LVD_CFG_LVD_FLT_8BIT(x)            (x<<8)

//clk_cfg
#define ANAC_CLK_CFG_LRC_DIS                   (0<<0)
#define ANAC_CLK_CFG_LRC_EN                    (1<<0)
#define ANAC_CLK_CFG_HRC_DIS                   (0<<1)
#define ANAC_CLK_CFG_HRC_EN                    (1<<1)
#define ANAC_CLK_CFG_HRC_FSEL_3BIT(x)          (x<<2)
#define ANAC_CLK_CFG_XTL_XTL_DIS               (0<<5)
#define ANAC_CLK_CFG_XTL_XTL_EN                (1<<5)
#define ANAC_CLK_CFG_XTL_CSET_6BIT(x)          (x<<6)
#define ANAC_CLK_CFG_XTL_RTRIM_2BIT(x)         (x<<12)
#define ANAC_CLK_CFG_XTH_DIS                   (0<<14)
#define ANAC_CLK_CFG_XTH_EN                    (1<<14)

#define ANAC_CLK_CFG_HRC_FSEL_1M               1
#define ANAC_CLK_CFG_HRC_FSEL_2M               2
#define ANAC_CLK_CFG_HRC_FSEL_4M               3
#define ANAC_CLK_CFG_HRC_FSEL_8M               4
#define ANAC_CLK_CFG_HRC_FSEL_16M               5
#define ANAC_CLK_CFG_HRC_FSEL_24M               6
#define ANAC_CLK_CFG_HRC_FSEL_32M               7

//ana_res1
#define ANAC_ANA_RES1_LVDI1              (1<<19)
#define ANAC_ANA_RES1_AIN0               (1<<19)
#define ANAC_ANA_RES1_VPTST              (1<<18)
#define ANAC_ANA_RES1_AIN1               (1<<18)
#define ANAC_ANA_RES1_PDTST              (1<<17)
#define ANAC_ANA_RES1_ADC_VREF           (1<<17)
#define ANAC_ANA_RES1_LVDIO              (1<<16)
#define ANAC_ANA_RES1_AIN2               (1<<16)
#define ANAC_ANA_RES1_VC1P5              (1<<15)
#define ANAC_ANA_RES1_AIN3               (1<<15)
#define ANAC_ANA_RES1_VC1P4              (1<<14)
#define ANAC_ANA_RES1_AIN4               (1<<14)
#define ANAC_ANA_RES1_VC1P3              (1<<13)
#define ANAC_ANA_RES1_AIN5               (1<<13)
#define ANAC_ANA_RES1_VC1P2              (1<<12)
#define ANAC_ANA_RES1_AIN6               (1<<12)
#define ANAC_ANA_RES1_VC1N0              (1<<11)
#define ANAC_ANA_RES1_AIN7               (1<<11)
#define ANAC_ANA_RES1_VC1O0              (1<<10)
#define ANAC_ANA_RES1_VC1P1              (1<<9)
#define ANAC_ANA_RES1_VC1N1              (1<<8)
#define ANAC_ANA_RES1_VCIO1              (1<<7)
#define ANAC_ANA_RES1_AIN8               (1<<7)
#define ANAC_ANA_RES1_VC1P0              (1<<6)
#define ANAC_ANA_RES1_VC1N2              (1<<5)
#define ANAC_ANA_RES1_AIN9               (1<<5)
#define ANAC_ANA_RES1_AIN10              (1<<4)
#define ANAC_ANA_RES1_VC1P6              (1<<3)
#define ANAC_ANA_RES1_XTLI               (1<<3)
#define ANAC_ANA_RES1_XTLO               (1<<2)
#define ANAC_ANA_RES1_AIN11              (1<<1)

#define ANAC_SetANA_RES1(x)      {ANAC->ANA_RES1 |= x;}
#define ANAC_ClrANA_RES1(x)      {ANAC->ANA_RES1 &=~(x);}


//dly_cfg
#define ANAC_DLY_CFG_BGR_DLY_2BIT(x)         (x<<0)
#define ANAC_DLY_CFG_ALDO4A_DLY_2BIT(x)      (x<<2)
#define ANAC_DLY_CFG_DCDC_DLY_2BIT(x)        (x<<4)
#define ANAC_DLY_CFG_ADC_DLY_2BIT(x)         (x<<6)
#define ANAC_DLY_CFG_TEMP_DLY_2BIT(x)        (x<<8)
#define ANAC_DLY_CFG_PDSENS_RST_DLY_2BIT(x)  (x<<10)
#define ANAC_DLY_CFG_LED_SMOKE_DLY_8BIT(x)   (x<<12)
#define ANAC_DLY_CFG_LED_PW_8BIT(x)          (x<<28)


//me_ctl
#define ANAC_ME_CTL_ANAC_EN             (1<<0)
#define ANAC_ME_CTL_ANAC_IE             (1<<1)
#define ANAC_ME_CTL_ME_ALL_AUTO         (1<<2)
#define ANAC_ME_CTL_TEMP_AUTO           (1<<3)
#define ANAC_ME_CTL_DARKC_AUTO          (1<<4)
#define ANAC_ME_CTL_SMOKE_AUTO          (1<<5)
#define ANAC_ME_CTL_ANA_SET             (1<<6)
#define ANAC_ME_CTL_DARKC_EN            (1<<7)
#define ANAC_ME_CTL_SMOKE_EN            (1<<8)

#define ANAC_SetME_CTL(x)       {ANAC->ME_CTL |= x;}
#define ANAC_ClrME_CTL(x)       {ANAC->ME_CTL &=~(x);}

//anac_flag
#define ANAC_ANAC_FLAG_SMOKE_INTF              (1<<0) //w 0 clr
#define ANAC_ANAC_FLAG_DARKC_INTF              (1<<1) //w 0 clr
#define ANAC_ANAC_FLAG_TEMP_INTF               (1<<2) //w 0 clr
#define ANAC_ANAC_FLAG_ADC_INTF                (1<<3) //w 0 clr
#define ANAC_ANAC_FLAG_ANA_OK                  (1<<4)
#define ANAC_ANAC_FLAG_DCDC_VOK                (1<<5)
#define ANAC_ANAC_FLAG_LVD_INTF                (1<<6) //w 0 clr
#define ANAC_ANAC_FLAG_LVD_OUT                 (1<<7)
#define ANAC_ANAC_FLAG_CMP_OUT                 (1<<8)

#define ANAC_GetANAC_FLAG(x)         {x = ANAC->ANAC_FLAG ;}
#define ANAC_ClrANAC_FLAG(x)         {ANAC->ANAC_FLAG &=~(x);}

//anac_test
#define ANAC_TEST_SEL_OPTION_NVR            (0<<0)
#define ANAC_TEST_SEL_OPTION_ANAC           (1<<0)
#define ANAC_TEST_TESTMODE_EN               0x55
#define ANAC_TEST_TESTMODE_8BIT(x)          (x<<8)

//wprot
#define ANAC_WRPROTECT_CLOSE()   {ANAC->WPROT = 0x5a5a;ANAC->WPROT = 0xa5a5;}



//
#define  ANAC_ADC_RST_mask       0x0fff





#endif
