#include <spimaster.h>
#include <vrlt.h>
#include <string.h>

void spi_master_config(volatile uint32_t *addr, uint32_t cpol, uint32_t cpha)
{
    addr[2] = ((cpol << 1) | cpha);
    addr[3] = 0;
    addr[4] = 1;
    addr[5] = 1;
    addr[6] = 1;
}

void spi_short_delay()
{
    for (size_t i = 0; i < 64; i++)
    {
        __asm volatile("nop");
    }
}

uint32_t spi_master_activate(volatile uint32_t *addr, uint32_t channel)
{
    addr[0] = (0x11000000 | channel);
    spi_short_delay();
    return 0;
}

uint32_t spi_master_deactivate(volatile uint32_t *addr, uint32_t channel)
{
    spi_short_delay();
    addr[0] = (0x10000000 | channel);
    spi_short_delay();
    return 0;
}

uint32_t spi_master_rw_byte(volatile uint32_t *addr, uint8_t idata, uint8_t *odata)
{
    addr[0] = (0x01000000 | idata);
    while (1)
    {
        uint32_t _t = addr[0];
        *odata = _t & 0x000000ff;
        if ((_t & 0x80000000) != 0 && (_t & 0x7fff0000) != 0)
        {
            break;
        }
    }
    return 0;
}

void spi_master_rw(volatile uint32_t *addr, uint8_t *idata, uint8_t *odata, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        spi_master_rw_byte(addr, idata[i], &odata[i]);
    }
}

void spi_nor_read(volatile uint32_t *addr, uint8_t channel, uint8_t *buffer, uint8_t *start_addr, uint32_t len)
{
    memset(buffer, 0x00, len);
    spi_master_activate(addr, channel);
    uint8_t cmd[] = {0x0b, ((uint32_t)start_addr >> 16) & 0xff, ((uint32_t)start_addr >> 8) & 0xff, ((uint32_t)start_addr >> 0) & 0xff, 0x00};
    spi_master_rw(addr, cmd, cmd, sizeof(cmd));
    spi_master_rw(addr, buffer, buffer, len);
    spi_master_deactivate(addr, channel);
}

void spi_nor_we(volatile uint32_t *addr, uint8_t channel)
{
    uint8_t cmd[] = {0x06};
    spi_master_activate(addr, channel);
    spi_master_rw(addr, cmd, cmd, sizeof(cmd));
    spi_master_deactivate(addr, channel);
}

uint8_t spi_nor_read_sr_1(volatile uint32_t *addr, uint8_t channel)
{
    uint8_t cmd[] = {0x05, 0x00};
    spi_master_activate(addr, channel);
    spi_master_rw(addr, cmd, cmd, sizeof(cmd));
    spi_master_deactivate(addr, channel);
    return cmd[1];
}

void spi_nor_sector_erase(volatile uint32_t *addr, uint8_t channel, uint8_t *start_addr)
{
    uint8_t cmd[] = {0x20, ((uint32_t)start_addr >> 16) & 0xff, ((uint32_t)start_addr >> 8) & 0xff, ((uint32_t)start_addr >> 0) & 0xff};
    spi_master_activate(addr, channel);
    spi_master_rw(addr, cmd, cmd, sizeof(cmd));
    spi_master_deactivate(addr, channel);
}

void spi_nor_block_erase_64k(volatile uint32_t *addr, uint8_t channel, uint8_t *start_addr)
{
    uint8_t cmd[] = {0xd8, ((uint32_t)start_addr >> 16) & 0xff, ((uint32_t)start_addr >> 8) & 0xff, ((uint32_t)start_addr >> 0) & 0xff};
    spi_master_activate(addr, channel);
    spi_master_rw(addr, cmd, cmd, sizeof(cmd));
    spi_master_deactivate(addr, channel);
}

void spi_nor_page_program(volatile uint32_t *addr, uint8_t channel, uint8_t *start_addr, uint8_t *idata)
{
    uint8_t cmd[] = {0x02, ((uint32_t)start_addr >> 16) & 0xff, ((uint32_t)start_addr >> 8) & 0xff, ((uint32_t)start_addr >> 0) & 0xff};
    spi_master_activate(addr, channel);
    spi_master_rw(addr, cmd, cmd, sizeof(cmd));
    spi_master_rw(addr, idata, idata, 256);
    spi_master_deactivate(addr, channel);
}

void spi_nor_program(volatile uint32_t *addr, uint8_t channel, uint8_t *buffer, uint8_t *start_addr, uint32_t len)
{
    uint32_t _len = 0;
    uint8_t _buffer[4096];
    while (_len < len)
    {
        if (_len <= len - 4096)
        {
            memcpy(_buffer, buffer + _len, 4096);
        }
        else
        {
            spi_nor_read(addr, channel, _buffer, start_addr + _len, 4096);
            memcpy(_buffer, buffer + _len, len - _len);
        }
        spi_nor_we(addr, channel);
        spi_nor_sector_erase(addr, channel, start_addr + _len);
        while ((spi_nor_read_sr_1(addr, channel) & 0x01) == 1)
        {
            spi_short_delay();
        }
        for (uint32_t i = 0; i < 4096 / 256; i++)
        {
            spi_nor_we(addr, channel);
            spi_nor_page_program(addr, channel, start_addr + _len + 256 * i, _buffer + 256 * i);
            while ((spi_nor_read_sr_1(addr, channel) & 0x01) == 1)
            {
                spi_short_delay();
            }
        }
        _len += 4096;
    }
}
