/*
 * VRLT.h
 *
 *  Created on: Aug 24, 2016
 *      Author: clp
 *		Modified: Reinforce-II
 */

#ifndef VRLT_H_
#define VRLT_H_

#include "dac.h"
#include "timer.h"
#include "prescaler.h"
#include "interrupt.h"
#include "uart.h"
#include "gpio.h"
#include "spimaster.h"
#include "crc32.h"
#include "crc16.h"

#define CORE_HZ 48000000

#define GPIO_A_BASE ((GPIO_REG *)0xF0000000)
#define GPIO_B_BASE ((GPIO_REG *)0xF0001000)
#define UART_BASE ((UART_REG *)0xF0010000)

#define TIMER_PRESCALER ((PRESCALER_REG *)0xF0020000)
#define TIMER_INTERRUPT ((INTERRUPT_CTRL_REG *)0xF0020010)
#define TIMER_0 ((TIMER_REG *)0xF0020020)
#define TIMER_1 ((TIMER_REG *)0xF0020030)
#define TIMER_2 ((TIMER_REG *)0xF0020040)
#define TIMER_3 ((TIMER_REG *)0xF0020050)
#define TIMER_4 ((TIMER_REG *)0xF0020060)
#define TIMER_5 ((TIMER_REG *)0xF0020070)
#define TIMER_6 ((TIMER_REG *)0xF0020080)
#define TIMER_7 ((TIMER_REG *)0xF0020090)

#define DAC_PRESCALER ((PRESCALER_REG *)0xf0050000)
#define DAC_BASE ((DAC_REG *)0xf0050010)

#define UART_SAMPLE_PER_BAUD 8

#define SPI_MASTER_BASE ((uint32_t *)0xF0040000)
#define TF_MASTER_BASE ((uint32_t *)0xF0041000)

#define SPI_FLASH_PROGRAM_BASE ((uint8_t *)0x000B0000)
#define SPI_FLASH_PROGRAM_LEN 0x50000UL

#define SDRAM_BASE ((uint8_t *)0x40000000)

#endif /* VRLT_H_ */
