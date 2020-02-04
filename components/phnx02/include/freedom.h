//
// freedom.h -- Definitions for Freescale Freedom development board
//
//  Copyright (c) 2012-2013 Andrew Payne <andy@payne.org>
//
#ifndef _FREEDOM_H_
#define _FREEDOM_H_

#include "MKL25Z4.h" // CPU definitions

#define CORE_CLOCK 48000000 // Core clock speed
// #define CORE_CLOCK          24000000    // Core clock speed

extern uint32_t __heap_start[];
extern uint32_t __StackTop[];
extern uint32_t __data_start__[], __data_end__[];
extern uint32_t __bss_start__[], __bss_end__[];
extern uint32_t __etext[]; // End of code/flash

// Interrupt enabling and disabling
static inline void enable_irq(int n)
{
    NVIC_ICPR |= 1 << (n - 16);
    NVIC_ISER |= 1 << (n - 16);
}
// TODO:  IRQ disable

static inline void __enable_irq(void) { asm volatile("cpsie i"); }
static inline void __disable_irq(void) { asm volatile("cpsid i"); }

#endif
