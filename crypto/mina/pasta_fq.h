#ifndef MINA_PASTA_FQ_H_
#define MINA_PASTA_FQ_H_
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

void fiat_pasta_fq_set_one(uint64_t out1[4]);
void fiat_pasta_fq_opp(uint64_t out1[4], const uint64_t arg1[4]);
void fiat_pasta_fq_square(uint64_t out1[4], const uint64_t arg1[4]);
void fiat_pasta_fq_to_montgomery(uint64_t out1[4], const uint64_t arg1[4]);
void fiat_pasta_fq_from_montgomery(uint64_t out1[4], const uint64_t arg1[4]);
#endif
