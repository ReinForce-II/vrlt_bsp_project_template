#ifndef _UART_H_
#define _UART_H_
#include <stdint.h>
#include <vrlt.h>
typedef struct
{
	volatile uint32_t DATA;
	volatile uint32_t STATUS;
	volatile uint32_t CLOCK_DIVIDER;
	volatile uint32_t FRAME_CONFIG;
} UART_REG;

uint32_t uart_read_occupancy(UART_REG *reg);
void uart_write(uint32_t data, UART_REG *reg);
char uart_read(UART_REG *reg);
void uart_init(uint32_t baud_rate, UART_REG *uart);
int32_t uart_write_try(uint32_t data, UART_REG *reg, uint32_t timeout);
char uart_read_try(UART_REG *reg, uint32_t timeout);

#endif /* _UART_H_ */
