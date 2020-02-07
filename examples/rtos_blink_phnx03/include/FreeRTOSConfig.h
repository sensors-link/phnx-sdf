/*
    FreeRTOS V8.2.3 - Copyright (C) 2015 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "gd32vf103.h"

/*
 * For some reason the standard demo timer demo/test tasks fail when executing
 * in QEMU, although they pass on other RISC-V platforms.  This requires
 * further investigation, but for now, defining _WINDOWS_ has the effect of
 * using the wider timer test thresholds that are normally only used when the
 * tests are used with the FreeRTOS Windows port (which is not deterministic
 * and therefore requires wider margins).
 */
#define _WINDOWS_

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

#define configCLINT_BASE_ADDRESS ECLIC_ADDR_BASE
#define configUSE_PREEMPTION 1
#define configUSE_IDLE_HOOK 0
#define configUSE_TICK_HOOK 0         // 1
#define configCPU_CLOCK_HZ (10000000) /*QEMU*/
#define configTICK_RATE_HZ ((TickType_t)100)
#define configMAX_PRIORITIES (7)
#define configMINIMAL_STACK_SIZE ((unsigned short)70)
#define configTOTAL_HEAP_SIZE ((size_t)2048)
#define configMAX_TASK_NAME_LEN (16)
#define configUSE_TRACE_FACILITY 0
#define configUSE_16_BIT_TICKS 0
#define configIDLE_SHOULD_YIELD 0
#define configUSE_MUTEXES 0
#define configQUEUE_REGISTRY_SIZE 8
#define configCHECK_FOR_STACK_OVERFLOW 0 // 2
#define configUSE_RECURSIVE_MUTEXES 0
#define configUSE_MALLOC_FAILED_HOOK 0 // 1
#define configUSE_APPLICATION_TASK_TAG 0
#define configUSE_COUNTING_SEMAPHORES 0 // 1
#define configGENERATE_RUN_TIME_STATS 0
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 0
#define configMAX_CO_ROUTINE_PRIORITIES (2)

/* Software timer definitions. */
#define configUSE_TIMERS 0 // 1
#define configTIMER_TASK_PRIORITY (configMAX_PRIORITIES - 1)
#define configTIMER_QUEUE_LENGTH 4
#define configTIMER_TASK_STACK_DEPTH (100)

/* Task priorities.  Allow these to be overridden. */
#ifndef uartPRIMARY_PRIORITY
#define uartPRIMARY_PRIORITY (configMAX_PRIORITIES - 3)
#endif

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet 0      // 1
#define INCLUDE_uxTaskPriorityGet 0     // 1
#define INCLUDE_vTaskDelete 0           // 1
#define INCLUDE_vTaskCleanUpResources 0 // 1
#define INCLUDE_vTaskSuspend 0          // 1
#define INCLUDE_vTaskDelayUntil 1       // 1
#define INCLUDE_vTaskDelay 1
#define INCLUDE_eTaskGetState 0          // 1
#define INCLUDE_xTimerPendFunctionCall 0 // 1

/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
void vAssertCalled(void);
#define configASSERT(x)                                                        \
  if ((x) == 0)                                                                \
  vAssertCalled()

/* Overwrite some of the stack sizes allocated to various test and demo tasks.
Like all task stack sizes, the value is the number of words, not bytes. */
#define bktBLOCK_TIME_TASK_STACK_SIZE 100
#define notifyNOTIFIED_TASK_STACK_SIZE 120
#define priSUSPENDED_RX_TASK_STACK_SIZE 90
#define tmrTIMER_TEST_TASK_STACK_SIZE 100
#define ebRENDESVOUS_TEST_TASK_STACK_SIZE 100
#define ebEVENT_GROUP_SET_BITS_TEST_TASK_STACK_SIZE 115
#define genqMUTEX_TEST_TASK_STACK_SIZE 90
#define genqGENERIC_QUEUE_TEST_TASK_STACK_SIZE 100
#define recmuRECURSIVE_MUTEX_TEST_TASK_STACK_SIZE 90

#endif /* FREERTOS_CONFIG_H */
