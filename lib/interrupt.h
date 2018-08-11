#ifndef INTERRUPTCTRL_H_
#define INTERRUPTCTRL_H_

#include <stdint.h>

typedef struct
{
  volatile uint32_t PENDINGS;
  volatile uint32_t MASKS;
} INTERRUPT_CTRL_REG;

#endif /* INTERRUPTCTRL_H_ */
