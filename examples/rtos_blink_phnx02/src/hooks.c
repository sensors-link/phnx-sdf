/*
 * Filename:	hooks.c
 * Description:	Demo of FreeRTOS on KL25Z board
 *              Blinks the three LEDs at different rates
 *              TPM initialization and much of the rest of the code
 *              can be attributed to FreeRTOS and/or Rowley and/or
 *              Freescale example code.
 */
#include "freedom.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "led.h"

// void vApplicationIdleHook(void)
// {
//     /* The co-routines are executed in the idle task using the idle task hook. */
//     // vCoRoutineSchedule();	/* Comment this out if not using Co-routines. */
// }
/*-----------------------------------------------------------*/

// void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
// {
//     (void)pcTaskName;
//     (void)pxTask;

//     /* Run time stack overflow checking is performed if
// 	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
// 	function is called if a stack overflow is detected.  This function is
// 	provided as an example only as stack overflow checking does not function
// 	when running the FreeRTOS Windows port. */
//     configASSERT(pdFALSE);
// }
/*-----------------------------------------------------------*/

// void vApplicationTickHook(void)
// {
//     /* This function will be called by each tick interrupt if
// 	configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
// 	added here, but the tick hook is called from an interrupt context, so
// 	code must not attempt to block, and only the interrupt safe FreeRTOS API
// 	functions can be used (those that end in FromISR()). */
// }
/*-----------------------------------------------------------*/

// void vApplicationDaemonTaskStartupHook(void)
// {
//     /* This function will be called once only, when the daemon task starts to
// 	execute	(sometimes called the timer task).  This is useful if the
// 	application includes initialisation code that would benefit from executing
// 	after the scheduler has been started. */
// }

/*-----------------------------------------------------------*/

// void vApplicationMallocFailedHook()
// {
//     configASSERT(pdFALSE);
// }

/*-----------------------------------------------------------*/

// void vMainConfigureTimerForRunTimeStats(void)
// {
// }

/*-----------------------------------------------------------*/

void vAssertCalled(unsigned long ulLine, const char *const pcFileName)
{
    /* Called if an assertion passed to configASSERT() fails.  See
	http://www.freertos.org/a00110.html#configASSERT for more information. */

    /* Parameters are not used. */
    (void)ulLine;
    (void)pcFileName;

    taskENTER_CRITICAL();
    {
        /* Stop the trace recording. */
        fault(FAULT_MEDIUM_BLINK);
    }
    taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
// void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
// {
//     /* If the buffers to be provided to the Idle task are declared inside this
// function then they must be declared static - otherwise they will be allocated on
// the stack and so not exists after this function exits. */
//     static StaticTask_t xIdleTaskTCB;
//     static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

//     /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
// 	state will be stored. */
//     *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

//     /* Pass out the array that will be used as the Idle task's stack. */
//     *ppxIdleTaskStackBuffer = uxIdleTaskStack;

//     /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
// 	Note that, as the array is necessarily of type StackType_t,
// 	configMINIMAL_STACK_SIZE is specified in words, not bytes. */
//     *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
// }
// /*-----------------------------------------------------------*/
// StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
// void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
// {
//     /* If the buffers to be provided to the Timer task are declared inside this
// function then they must be declared static - otherwise they will be allocated on
// the stack and so not exists after this function exits. */
//     static StaticTask_t xTimerTaskTCB;

//     /* Pass out a pointer to the StaticTask_t structure in which the Timer
// 	task's state will be stored. */
//     *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

//     /* Pass out the array that will be used as the Timer task's stack. */
//     *ppxTimerTaskStackBuffer = uxTimerTaskStack;

//     /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
// 	Note that, as the array is necessarily of type StackType_t,
// 	configMINIMAL_STACK_SIZE is specified in words, not bytes. */
//     *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
// }
