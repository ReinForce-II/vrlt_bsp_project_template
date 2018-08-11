#ifndef PRESCALERCTRL_H_
#define PRESCALERCTRL_H_

#include <stdint.h>

typedef struct
{
  volatile uint32_t LIMIT;
} PRESCALER_REG;

void prescaler_init(PRESCALER_REG *reg);

#endif /* PRESCALERCTRL_H_ */
