#ifndef GPIO_H_
#define GPIO_H_
#include <stdint.h>

typedef struct
{
  volatile uint32_t INPUT;
  volatile uint32_t OUTPUT;
  volatile uint32_t OUTPUT_ENABLE;
} GPIO_REG;

#endif /* GPIO_H_ */
