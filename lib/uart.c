#include <uart.h>
enum UART_PARITY
{
    NONE = 0,
    EVEN = 1,
    ODD = 2
};

enum UART_STOP
{
    ONE = 0,
    TWO = 1
};

typedef struct
{
    uint32_t data_length;
    enum UART_PARITY parity;
    enum UART_STOP stop;
    uint32_t clock_divider;
} UART_CONFIG;

uint32_t uart_write_availability(UART_REG *reg)
{
    return (reg->STATUS >> 16) & 0xFF;
}
uint32_t uart_read_occupancy(UART_REG *reg)
{
    return reg->STATUS >> 24;
}

void uart_write(uint32_t data, UART_REG *reg)
{
    while (uart_write_availability(reg) == 0)
        ;
    reg->DATA = data;
}
int32_t uart_write_try(uint32_t data, UART_REG *reg, uint32_t timeout)
{
    while (timeout)
    {
        if (uart_write_availability(reg))
        {
            break;
        }
        timeout--;
        // uint64_t _tm = time();
        // while (time() < (_tm + (uint64_t)timeout * CORE_HZ / 1000))
        //     ;
    }
    if (timeout)
    {
        reg->DATA = data;
        return 0;
    }
    else
    {
        return -1;
    }
}

char uart_read(UART_REG *reg)
{
    while (uart_read_occupancy(reg) == 0)
        ;
    return (char)(reg->DATA);
}

char uart_read_try(UART_REG *reg, uint32_t timeout)
{
    while (timeout)
    {
        if (uart_read_occupancy(reg))
        {
            break;
        }
        timeout--;
        // uint64_t _tm = time();
        // while (time() < (_tm + (uint64_t)timeout * CORE_HZ / 1000))
        //     ;
    }
    if (timeout)
    {
        return (char)(reg->DATA);
    }
    else
    {
        return -1;
    }
}

void uart_apply_config(UART_CONFIG *config, UART_REG *reg)
{
    reg->CLOCK_DIVIDER = config->clock_divider;
    reg->FRAME_CONFIG = ((config->data_length - 1) << 0) | (config->parity << 8) | (config->stop << 16);
}

void uart_init(uint32_t baud_rate, UART_REG *uart)
{
    UART_CONFIG uartConfig;
    uartConfig.data_length = 8;
    uartConfig.parity = NONE;
    uartConfig.stop = ONE;
    uartConfig.clock_divider = CORE_HZ / 8 / baud_rate - 1;
    uart_apply_config(&uartConfig, uart);
}
