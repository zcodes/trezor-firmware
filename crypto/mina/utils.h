#ifndef MINA_UTILS_H_
#define MINA_UTILS_H_
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

void packed_bit_array_set(uint8_t *bits, size_t i, bool b);
bool packed_bit_array_get(uint8_t *bits, size_t i);
#endif
