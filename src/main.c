#include "vrlt.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdlib.h"
#include "stdio.h"

void idle_task(void *param)
{
    while (1)
    {
        vTaskDelay(1000);
    }
}

int main()
{
    // setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    uart_init(115200, UART_BASE);
    xTaskCreate(idle_task, "idle_task", 128, 0, 0, 0);
    vTaskStartScheduler();
    while (1)
    {
    }
}

void vPortSysTickHandler();
void trap_handler(uint32_t mcause, uint32_t mepc)
{
    if ((mcause & 0x80000000) != 0 && (mcause & 0x7fffffff) == 7)
    {
        if ((TIMER_INTERRUPT->PENDINGS & 0x1) != 0)
        {
            TIMER_INTERRUPT->PENDINGS = 0x1;
            vPortSysTickHandler();
        }
    }
    else
    {
        while (1)
            ;
    }
}
