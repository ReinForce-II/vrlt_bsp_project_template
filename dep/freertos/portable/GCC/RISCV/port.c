/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the RISC-V port.
 *----------------------------------------------------------*/

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"
#include "vrlt.h"

/* A variable is used to keep track of the critical section nesting.  This
variable has to be stored as part of the task context and must be initialised to
a non zero value to ensure interrupts don't inadvertently become unmasked before
the scheduler starts.  As it is stored as part of the task context it will
automatically be set to 0 when the first task is started. */
static UBaseType_t uxCriticalNesting = 0xaaaaaaaa;

/*
 * Handler for timer interrupt
 */
void vPortSysTickHandler(void);

/*
 * Setup the timer to generate the tick interrupts.
 */
void vPortSetupTimer(void);

/*
 * Used to catch tasks that attempt to return from their implementing function.
 */
static void prvTaskExitError(void);

/* Sets and enable the timer interrupt */
void vPortSetupTimer(void)
{
    timer_init(TIMER_0, 1, configTICK_CLOCK_HZ / configTICK_RATE_HZ);
    TIMER_PRESCALER->LIMIT = 0;
    TIMER_INTERRUPT->MASKS = 0x1;
    TIMER_INTERRUPT->PENDINGS = 0x1;

    /* Enable timer interupt */
    __asm volatile("csrs mie,%0" ::"r"(0x80));
}
/*-----------------------------------------------------------*/

void prvTaskExitError(void)
{
    /* A function that implements a task must not exit or attempt to return to
	its caller as there is nothing to return to.  If a task wants to exit it
	should instead call vTaskDelete( NULL ).

	Artificially force an assert() to be triggered if configASSERT() is
	defined, then stop here so application writers can catch the error. */
    configASSERT(uxCriticalNesting == ~0UL);
    portDISABLE_INTERRUPTS();
    for (;;)
        ;
}
/*-----------------------------------------------------------*/

/* Clear current interrupt mask and set given mask */
void vPortClearInterruptMask(int mask)
{
    __asm volatile("csrw mie, %0" ::"r"(mask));
}
/*-----------------------------------------------------------*/

/* Set interrupt mask and return current interrupt enable register */
int vPortSetInterruptMask(void)
{
    int ret;
    __asm volatile("csrr %0,mie"
                   : "=r"(ret));
    __asm volatile("csrc mie,%0" ::"i"(7));
    return ret;
}
/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */
StackType_t *pxPortInitialiseStack(StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters)
{
    /* Simulate the stack frame as it would be created by a context switch
	interrupt. */
    register int *tp asm("x3");
    pxTopOfStack--;
    *pxTopOfStack = (portSTACK_TYPE)pxCode; /* Start address */
    pxTopOfStack -= 22;
    *pxTopOfStack = (portSTACK_TYPE)pvParameters; /* Register a0 */
    pxTopOfStack -= 6;
    *pxTopOfStack = (portSTACK_TYPE)tp; /* Register thread pointer */
    pxTopOfStack -= 3;
    *pxTopOfStack = (portSTACK_TYPE)prvTaskExitError; /* Register ra */

    return pxTopOfStack;
}
/*-----------------------------------------------------------*/

void vPortSysTickHandler(void)
{
    /* Increment the RTOS tick. */
    if (xTaskIncrementTick() != pdFALSE)
    {
        vTaskSwitchContext();
    }
}
/*-----------------------------------------------------------*/
