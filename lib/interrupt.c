#include <interrupt.h>
#include <vrlt.h>
void interrupt_ctrl_init(INTERRUPT_CTRL_REG *reg)
{
    reg->PENDINGS = 0xffffffff;
    reg->MASKS = 0x0;
}
