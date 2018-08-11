
#if !defined(_DAC_H)
#define _DAC_H

#include <stdint.h>

typedef struct
{
    volatile uint32_t VALUE;    // 16 bit by default
} DAC_REG;

#endif // _DAC_H
