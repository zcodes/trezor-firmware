#include "pasta.h"

/*
 * The function fiat_pasta_fp_addcarryx_u64 is an addition with carry.
 * Postconditions:
 *   out1 = (arg1 + arg2 + arg3) mod 2^64
 *   out2 = ⌊(arg1 + arg2 + arg3) / 2^64⌋
 *
 * Input Bounds:
 *   arg1: [0x0 ~> 0x1]
 *   arg2: [0x0 ~> 0xffffffffffffffff]
 *   arg3: [0x0 ~> 0xffffffffffffffff]
 * Output Bounds:
 *   out1: [0x0 ~> 0xffffffffffffffff]
 *   out2: [0x0 ~> 0x1]
 */
void fiat_pasta_addcarryx_u64(uint64_t* out1, fiat_pasta_uint1* out2, fiat_pasta_uint1 arg1, uint64_t arg2, uint64_t arg3) {
  uint64_t tmp = arg3 + arg1;
  *out1 = arg2 + tmp;
  *out2 = (arg2 > *out1) | (arg3 > tmp);
}

/*
 * The function fiat_pasta_fp_subborrowx_u64 is a subtraction with borrow.
 * Postconditions:
 *   out1 = (-arg1 + arg2 + -arg3) mod 2^64
 *   out2 = -⌊(-arg1 + arg2 + -arg3) / 2^64⌋
 *
 * Input Bounds:
 *   arg1: [0x0 ~> 0x1]
 *   arg2: [0x0 ~> 0xffffffffffffffff]
 *   arg3: [0x0 ~> 0xffffffffffffffff]
 * Output Bounds:
 *   out1: [0x0 ~> 0xffffffffffffffff]
 *   out2: [0x0 ~> 0x1]
 */
void fiat_pasta_subborrowx_u64(uint64_t* out1, fiat_pasta_uint1* out2, fiat_pasta_uint1 arg1, uint64_t arg2, uint64_t arg3) {
  uint64_t tmp = arg3 + arg1;
  *out1 = arg2 - tmp;
  *out2 = (arg2 < *out1) | (arg3 > tmp);
}

/*
 * The function fiat_pasta_fp_mulx_u64 is a multiplication, returning the full double-width result.
 * Postconditions:
 *   out1 = (arg1 * arg2) mod 2^64
 *   out2 = ⌊arg1 * arg2 / 2^64⌋
 *
 * Input Bounds:
 *   arg1: [0x0 ~> 0xffffffffffffffff]
 *   arg2: [0x0 ~> 0xffffffffffffffff]
 * Output Bounds:
 *   out1: [0x0 ~> 0xffffffffffffffff]
 *   out2: [0x0 ~> 0xffffffffffffffff]
 */
void fiat_pasta_mulx_u64(uint64_t* out1, uint64_t* out2, uint64_t a, uint64_t b) {
  uint64_t    a_lo = (uint32_t)a;
  uint64_t    a_hi = a >> 32;
  uint64_t    b_lo = (uint32_t)b;
  uint64_t    b_hi = b >> 32;

  uint64_t    a_x_b_hi =  a_hi * b_hi;
  uint64_t    a_x_b_mid = a_hi * b_lo;
  uint64_t    b_x_a_mid = b_hi * a_lo;
  uint64_t    a_x_b_lo =  a_lo * b_lo;

  uint64_t    carry_bit = ((uint64_t)(uint32_t)a_x_b_mid +
                          (uint64_t)(uint32_t)b_x_a_mid +
                          (a_x_b_lo >> 32) ) >> 32;

 *out2 = a_x_b_hi +
     (a_x_b_mid >> 32) + (b_x_a_mid >> 32) +
     carry_bit;

  // TODO: This multiplication could be avoided.
  *out1 = a * b;
}

/*
 * The function fiat_pasta_fp_cmovznz_u64 is a single-word conditional move.
 * Postconditions:
 *   out1 = (if arg1 = 0 then arg2 else arg3)
 *
 * Input Bounds:
 *   arg1: [0x0 ~> 0x1]
 *   arg2: [0x0 ~> 0xffffffffffffffff]
 *   arg3: [0x0 ~> 0xffffffffffffffff]
 * Output Bounds:
 *   out1: [0x0 ~> 0xffffffffffffffff]
 */
void fiat_pasta_cmovznz_u64(uint64_t* out1, fiat_pasta_uint1 arg1, uint64_t arg2, uint64_t arg3) {
  fiat_pasta_uint1 x1;
  uint64_t x2;
  x1 = (!(!arg1));
  x2 = ((fiat_pasta_int1)(0x0 - x1) & UINT64_C(0xffffffffffffffff));
  *out1 = ((x2 & arg3) | ((~x2) & arg2));
}

/*
 * The function fiat_pasta_fq_sub subtracts two field elements in the Montgomery domain.
 * Preconditions:
 *   0 ≤ eval arg1 < m
 *   0 ≤ eval arg2 < m
 * Postconditions:
 *   eval (from_montgomery out1) mod m = (eval (from_montgomery arg1) - eval (from_montgomery arg2)) mod m
 *   0 ≤ eval out1 < m
 *
 * Input Bounds:
 *   arg1: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff]]
 *   arg2: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff]]
 * Output Bounds:
 *   out1: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff]]
 */
void fiat_pasta_sub(uint64_t out1[4], const uint64_t arg1[4], const uint64_t arg2[4], bool fq) {
    const uint64_t *xx;

    if (fq) {
        xx = mina_fq;
    } else {
        xx = mina_fp;
    }

  uint64_t x1;
  fiat_pasta_uint1 x2;
  uint64_t x3;
  fiat_pasta_uint1 x4;
  uint64_t x5;
  fiat_pasta_uint1 x6;
  uint64_t x7;
  fiat_pasta_uint1 x8;
  uint64_t x9;
  fiat_pasta_uint1 x11;
  fiat_pasta_uint1 x13;
  fiat_pasta_uint1 x15;
  fiat_pasta_uint1 x17;
  fiat_pasta_subborrowx_u64(&x1, &x2, 0x0, (arg1[0]), (arg2[0]));
  fiat_pasta_subborrowx_u64(&x3, &x4, x2, (arg1[1]), (arg2[1]));
  fiat_pasta_subborrowx_u64(&x5, &x6, x4, (arg1[2]), (arg2[2]));
  fiat_pasta_subborrowx_u64(&x7, &x8, x6, (arg1[3]), (arg2[3]));
  fiat_pasta_cmovznz_u64(&x9, x8, 0x0, UINT64_C(0xffffffffffffffff));
  fiat_pasta_addcarryx_u64(&out1[0], &x11, 0x0, x1, (x9 & xx[3]));
  fiat_pasta_addcarryx_u64(&out1[1], &x13, x11, x3, (x9 & xx[2]));
  fiat_pasta_addcarryx_u64(&out1[2], &x15, x13, x5, 0x0);
  fiat_pasta_addcarryx_u64(&out1[3], &x17, x15, x7, (x9 & xx[1]));
}

/*
 * The function fiat_pasta_fp_nonzero outputs a single non-zero word if the input is non-zero and zero otherwise.
 * Preconditions:
 *   0 ≤ eval arg1 < m
 * Postconditions:
 *   out1 = 0 ↔ eval (from_montgomery arg1) mod m = 0
 *
 * Input Bounds:
 *   arg1: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff]]
 * Output Bounds:
 *   out1: [0x0 ~> 0xffffffffffffffff]
 */
void fiat_pasta_nonzero(uint64_t* out1, const uint64_t arg1[4]) {
  *out1 = ((arg1[0]) | ((arg1[1]) | ((arg1[2]) | (arg1[3]))));
}

bool fiat_pasta_equals(const uint64_t x[4], const uint64_t y[4], bool fq) {
  uint64_t x_minus_y[4];
  fiat_pasta_sub(x_minus_y, x, y, fq);

  uint64_t x_minus_y_nonzero;
  fiat_pasta_nonzero(&x_minus_y_nonzero, x_minus_y);
  if (x_minus_y_nonzero) {
    return false;
  } else {
    return true;
  }
}
