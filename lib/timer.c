#include <timer.h>
#include <vrlt.h>
void timer_init(TIMER_REG *reg, uint8_t auto_clear, uint32_t limit_value)
{
    reg->CLEARS_TICKS = (auto_clear << 16) | 0x1;
    reg->LIMIT = limit_value;
    reg->VALUE = 0;
}
