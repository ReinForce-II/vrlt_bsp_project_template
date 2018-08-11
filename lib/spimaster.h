#ifndef _SPI_MASTER_H_
#define _SPI_MASTER_H_
#include <stdint.h>
#include <stdlib.h>

void spi_master_config(volatile uint32_t *_addr, uint32_t cpol, uint32_t cpha);

uint32_t spi_master_activate(volatile uint32_t *_addr, uint32_t channel);

uint32_t spi_master_deactivate(volatile uint32_t *_addr, uint32_t channel);

uint32_t spi_master_rw_byte(volatile uint32_t *_addr, uint8_t idata, uint8_t *odata);

void spi_master_rw(volatile uint32_t *addr, uint8_t *idata, uint8_t *odata, size_t len);

void spi_nor_read(volatile uint32_t *addr, uint8_t channel, uint8_t *buffer, uint8_t *start_addr, uint32_t len);

void spi_nor_program(volatile uint32_t *addr, uint8_t channel, uint8_t *buffer, uint8_t *start_addr, uint32_t len);

#endif // _SPI_MASTER_H_
