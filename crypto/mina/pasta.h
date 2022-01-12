#ifndef MINA_PASTA_H_
#define MINA_PASTA_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef unsigned char fiat_pasta_uint1;
typedef signed char fiat_pasta_int1;
typedef unsigned char fiat_pasta_uint1;
typedef signed char fiat_pasta_int1;

// x^{(p - 1) / 2}
static const bool P_MINUS_1_OVER_2[] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static const size_t P_MINUS_1_OVER_2_LEN = 254;

static const uint64_t aa = 0x992d30ecffffffff;
static const uint64_t bb = 0x4000000000000000;
static const uint64_t cc = 0x224698fc094cf91b;
static const uint64_t dd = 0x992d30ed00000001;

static const uint64_t mina_fp[4] = {
    0x992d30ecffffffff,
    0x4000000000000000,
    0x224698fc094cf91b,
    0x992d30ed00000001
};

static const uint64_t mina_fq[4] = {
    0x8c46eb20ffffffff,
    0x4000000000000000,
    0x224698fc0994a8dd,
    0x8c46eb2100000001,
};

void fiat_pasta_addcarryx_u64(uint64_t* out1, fiat_pasta_uint1* out2, fiat_pasta_uint1 arg1, uint64_t arg2, uint64_t arg3);
void fiat_pasta_subborrowx_u64(uint64_t* out1, fiat_pasta_uint1* out2, fiat_pasta_uint1 arg1, uint64_t arg2, uint64_t arg3);
void fiat_pasta_mulx_u64(uint64_t* out1, uint64_t* out2, uint64_t a, uint64_t b);
void fiat_pasta_cmovznz_u64(uint64_t* out1, fiat_pasta_uint1 arg1, uint64_t arg2, uint64_t arg3);
void fiat_pasta_sub(uint64_t out1[4], const uint64_t arg1[4], const uint64_t arg2[4], bool fq);
void fiat_pasta_nonzero(uint64_t* out1, const uint64_t arg1[4]);
bool fiat_pasta_equals(const uint64_t x[4], const uint64_t y[4], bool fq);
bool fiat_pasta_equals_zero(const uint64_t x[4]);
void fiat_pasta_selectznz(uint64_t out1[4], fiat_pasta_uint1 arg1, const uint64_t arg2[4], const uint64_t arg3[4]);
void fiat_pasta_to_bytes(uint8_t out1[32], const uint64_t arg1[4]);
void fiat_pasta_from_bytes(uint64_t out1[4], const uint8_t arg1[32]);
void fiat_pasta_add(uint64_t out1[4], const uint64_t arg1[4], const uint64_t arg2[4], bool fq);
void fiat_pasta_mul(uint64_t out1[4], const uint64_t arg1[4], const uint64_t arg2[4], bool fq);
void fiat_pasta_square(uint64_t out1[4], const uint64_t arg1[4], bool fq);
void fiat_pasta_from_montgomery(uint64_t out1[4], const uint64_t arg1[4], bool fq);

#endif
