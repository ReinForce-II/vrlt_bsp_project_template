#ifndef TIMERCTRL_H_
#define TIMERCTRL_H_

#include <stdint.h>

typedef struct
{
  volatile uint32_t CLEARS_TICKS;
  volatile uint32_t LIMIT;
  volatile uint32_t VALUE;
} TIMER_REG;

void timer_init(TIMER_REG *reg, uint8_t auto_clear, uint32_t limit_value);

#endif /* TIMERCTRL_H_ */
