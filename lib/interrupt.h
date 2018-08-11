#ifndef INTERRUPTCTRL_H_
#define INTERRUPTCTRL_H_

#include <stdint.h>

typedef struct
{
  volatile uint32_t PENDINGS;
  volatile uint32_t MASKS;
} INTERRUPT_CTRL_REG;

void interrupt_ctrl_init(INTERRUPT_CTRL_REG *reg);

#endif /* INTERRUPTCTRL_H_ */
