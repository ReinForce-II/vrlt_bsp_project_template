#ifndef _SPI_MASTER_H_
#define _SPI_MASTER_H_
#include <stdint.h>
#include <stdlib.h>
/*
     * In short, it has one command fifo (write/read/chip-select commands) and one read fifo.
     * data -> 0x00 :
     *   rxData -> R[7:0]
     *   rxOccupancy -> R[30:16] rx fifo occupancy (include the rxData in the amount)
     *   rxValid -> R[31] Inform that the readed rxData is valid
     *   When you read this register it pop an byte of the rx fifo and provide its value (via rxData)
     *   When you write this register, it push a command into the fifo. There is the commands that you can use :
     *     0x000000xx =>  Send byte xx
     *     0x010000xx =>  Send byte xx and also push the read data into the FIFO
     *     0x1100000X =>  Enable the SS line X
     *     0x1000000X =>  Disable the SS line X
     *
     * status -> 0x04
     *   cmdIntEnable -> RW[0] Command fifo empty interrupt enable
     *   rspIntEnable -> RW[1] Read fifo not empty interrupt enable
     *   cmdInt -> RW[8] Command fifo empty interrupt pending
     *   rspInt -> RW[9] Read fifo not empty interrupt pending
     *   txAvailability -> R[31:16] Command fifo space availability (SS commands + send byte commands)
     *
     * config -> 0x08
     *   cpol -> W[0]
     *   cpha -> W[1]
     *   ssActiveHigh -> W[31..4] For each ss, the corresponding bit specify if that's a active high one.
     *
     * clockDivider -> W 0x0C SPI frequency = FCLK / (2 * clockDivider)
     * ssSetup -> W 0x10 time between chip select enable and the next byte
     * ssHold -> W 0x14 time between the last byte transmission and the chip select disable
     * ssHold -> W 0x18 time between chip select disable and chip select enable
     */
// void spi_master_config(volatile unsigned int _addr, unsigned int cpol, unsigned int cpha);

// unsigned int spi_master_activate(volatile unsigned int _addr, unsigned int channel);

// unsigned int spi_master_deactivate(volatile unsigned int _addr, unsigned int channel);

// unsigned int spi_master_rw_byte(volatile unsigned int _addr, unsigned char idata, unsigned char *odata);

void spi_master_config(volatile uint32_t *_addr, uint32_t cpol, uint32_t cpha);

uint32_t spi_master_activate(volatile uint32_t *_addr, uint32_t channel);

uint32_t spi_master_deactivate(volatile uint32_t *_addr, uint32_t channel);

uint32_t spi_master_rw_byte(volatile uint32_t *_addr, uint8_t idata, uint8_t *odata);

void spi_master_rw(volatile uint32_t *addr, uint8_t *idata, uint8_t *odata, size_t len);

void spi_nor_read(volatile uint32_t *addr, uint8_t channel, uint8_t *buffer, uint8_t *start_addr, uint32_t len);

void spi_nor_program(volatile uint32_t *addr, uint8_t channel, uint8_t *buffer, uint8_t *start_addr, uint32_t len);

#endif // _SPI_MASTER_H_
