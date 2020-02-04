//
// _startup.c -- Reset initialization
//
//  Copyright (c) 2012-2013 Andrew Payne <andy@payne.org>
//

#include <stdio.h>
#include <string.h>
#include "system.h"

void _reset_init(void)    __attribute__((naked, aligned(8)));
extern int main();

// ----------------------------------------------------------------------------------
// Flash configuration field (loaded into flash memory at 0x400)
//
//      Note:  RESET_PIN_CFG is set to enable external RESET signal
//
__attribute__ ((section (".cfmconfig"))) const uint8_t _cfm[0x10] = {
    0xFF,  /* NV_BACKKEY3: KEY=0xFF */
    0xFF,  /* NV_BACKKEY2: KEY=0xFF */
    0xFF,  /* NV_BACKKEY1: KEY=0xFF */
    0xFF,  /* NV_BACKKEY0: KEY=0xFF */
    0xFF,  /* NV_BACKKEY7: KEY=0xFF */
    0xFF,  /* NV_BACKKEY6: KEY=0xFF */
    0xFF,  /* NV_BACKKEY5: KEY=0xFF */
    0xFF,  /* NV_BACKKEY4: KEY=0xFF */
    0xFF,  /* NV_FPROT3: PROT=0xFF */
    0xFF,  /* NV_FPROT2: PROT=0xFF */
    0xFF,  /* NV_FPROT1: PROT=0xFF */
    0xFF,  /* NV_FPROT0: PROT=0xFF */
    0x7E,  /* NV_FSEC: KEYEN=1,MEEN=3,FSLACC=3,SEC=2 */
    0xFF,  /* NV_FOPT: ??=1,??=1,FAST_INIT=1,LPBOOT1=1,RESET_PIN_CFG=1,
                        NMI_DIS=1,EZPORT_DIS=1,LPBOOT0=1 */
    0xFF,
    0xFF
  };

// ----------------------------------------------------------------------------------
//
// Initialize the system clocks to a 48 Mhz core clock speed
static void init_clocks(void)
{

}


// ----------------------------------------------------------------------------------
//  Default interrupt handler
void __attribute__((interrupt("IRQ"))) Default_Handler()
{
}

// The register frame pushed onto the stack during exceptions
typedef struct {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    void *pc;
    uint32_t psr;
} hw_stackframe_t;

// Handle hard faults:  print debugging information and halt
static void __attribute__((naked)) HardFault_Handler(void)
{
    // Set up arguments and invoke _HardFault_Handler()
    asm("mov    r0, lr\n"                   // Arg 0
        "mrs  r1, psp\n"                    // Arg 1
        "mrs  r2, msp\n"                    // Arg 2
        "b  _HardFault_Handler\n");
}

void __attribute__((naked)) _HardFault_Handler(uint32_t lr, void *psp, void *msp)
{

}


/* Weak definitions of handlers point to Default_Handler if not implemented */
void NMI_Handler()          __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler()          __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler()       __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler()      __attribute__ ((weak, alias("Default_Handler")));
void PMU_IrqHandler()      __attribute__ ((weak, alias("Default_Handler")));
void LPT_IrqHandler()      __attribute__ ((weak, alias("Default_Handler")));
void TIMER1_IrqHandler()      __attribute__ ((weak, alias("Default_Handler")));
void TIMER2_IrqHandler()      __attribute__ ((weak, alias("Default_Handler")));
void TIMER3_IrqHandler()       __attribute__ ((weak, alias("Default_Handler")));
void TIMER4_IrqHandler()      __attribute__ ((weak, alias("Default_Handler")));
void USART1_IrqHandler()       __attribute__ ((weak, alias("Default_Handler")));
void USART2_IrqHandler()       __attribute__ ((weak, alias("Default_Handler")));
void SPI_IrqHandler()      __attribute__ ((weak, alias("Default_Handler")));
void ANAC_IrqHandler()      __attribute__ ((weak, alias("Default_Handler")));
void EFC_IrqHandler()      __attribute__ ((weak, alias("Default_Handler")));
void IOM_IrqHandler()      __attribute__ ((weak, alias("Default_Handler")));
void I2C_IrqHandler()     __attribute__ ((weak, alias("Default_Handler")));
void CRC_IrqHandler()     __attribute__ ((weak, alias("Default_Handler")));

// ----------------------------------------------------------------------------------
// Interrupt vector table (loaded into flash memory at 0x000)
//
void (* const InterruptVector[])() __attribute__ ((section(".isr_vector"))) = {
    (void(*)(void)) __StackTop,                     // Initial stack pointer
    _reset_init,                                    // Reset handler
    NMI_Handler,
    HardFault_Handler,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    SVC_Handler,
    0,
    0,
    PendSV_Handler,
    SysTick_Handler,

    /* Interrupts */
    PMU_IrqHandler, /* PMU Handler */
    LPT_IrqHandler, /* LP Timer Handler */
    TIMER1_IrqHandler, /* Timer1 Handler */
    TIMER2_IrqHandler, /* Timer2 Handler */
    TIMER3_IrqHandler, /* Timer3 Handler */
    TIMER4_IrqHandler, /* Timer4 Handler */
    USART1_IrqHandler, /* UART1 handler */
    USART2_IrqHandler, /* UART2 handler */
    SPI_IrqHandler, /* SPIM handler */
    ANAC_IrqHandler, /* Analog Controller handler */
    EFC_IrqHandler, /* Embeded Flash Controller handler */
    IOM_IrqHandler, /* IOM Handler */
    I2C_IrqHandler, /* I2C handler */
    CRC_IrqHandler, /* CRC Handler */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};


// ----------------------------------------------------------------------------------
//
// _reset_init() -- Reset entry point.
//
//      The CPU reset vector points here.  Initialize the CPU, and jump
//      to the C runtime start, which will eventually invoke main()
//
void _reset_init(void)
{
    // SIM_COPC = 0;                       // Disable the watchdog timer
    // SCB_VTOR = (uint32_t)InterruptVector;
    SCB->VTOR = 0x10100000;

    // Copy values to initialize data segment
    uint32_t *fr = __etext;
    uint32_t *to = __data_start__;
    unsigned int len = __data_end__ - __data_start__;
    while(len--)
        *to++ = *fr++;

    // init_clocks();
    init_clocks();
    main();                           // Goto C lib startup
}
