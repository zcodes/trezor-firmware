#ifndef MINA_PASTA_FP_H_
#define MINA_PASTA_FP_H_
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

bool fiat_pasta_fp_sqrt(uint64_t x[4], const uint64_t value[4]);
void fiat_pasta_fp_set_one(uint64_t out1[4]);
void fiat_pasta_fp_inv(uint64_t out1[4], const uint64_t arg1[4]);
void fiat_pasta_fp_opp(uint64_t out1[4], const uint64_t arg1[4]);
void fiat_pasta_fp_pow(uint64_t out1[4], const uint64_t arg1[4], const bool* msb_bits, const size_t bits_len);
void fiat_pasta_fp_to_montgomery(uint64_t out1[4], const uint64_t arg1[4]);
#endif
