#if !defined(_CRC32_H_)
#define _CRC32_H_

#include <stdint.h>
#include <stdlib.h>

void crc32_tiny(const void *data, size_t n_bytes, uint32_t *crc);

#endif // _CRC32_H_
