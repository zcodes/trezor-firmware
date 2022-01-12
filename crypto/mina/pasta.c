#include "pasta.h"

/*
 * The function fiat_pasta_addcarryx_u64 is an addition with carry.
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
 * The function fiat_pasta_subborrowx_u64 is a subtraction with borrow.
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
 * The function fiat_pasta_mulx_u64 is a multiplication, returning the full double-width result.
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
 * The function fiat_pasta_cmovznz_u64 is a single-word conditional move.
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
 * The function fiat_pasta_sub subtracts two field elements in the Montgomery domain.
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
 * The function fiat_pasta_nonzero outputs a single non-zero word if the input is non-zero and zero otherwise.
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

bool fiat_pasta_equals_zero(const uint64_t x[4]) {
  uint64_t x_nonzero;
  fiat_pasta_nonzero(&x_nonzero, x);
  if (x_nonzero) {
    return false;
  } else {
    return true;
  }
}

/*
 * The function fiat_pasta_selectznz is a multi-limb conditional select.
 * Postconditions:
 *   eval out1 = (if arg1 = 0 then eval arg2 else eval arg3)
 *
 * Input Bounds:
 *   arg1: [0x0 ~> 0x1]
 *   arg2: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff]]
 *   arg3: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff]]
 * Output Bounds:
 *   out1: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff]]
 */
void fiat_pasta_selectznz(uint64_t out1[4], fiat_pasta_uint1 arg1, const uint64_t arg2[4], const uint64_t arg3[4]) {
  fiat_pasta_cmovznz_u64(&out1[0], arg1, (arg2[0]), (arg3[0]));
  fiat_pasta_cmovznz_u64(&out1[1], arg1, (arg2[1]), (arg3[1]));
  fiat_pasta_cmovznz_u64(&out1[2], arg1, (arg2[2]), (arg3[2]));
  fiat_pasta_cmovznz_u64(&out1[3], arg1, (arg2[3]), (arg3[3]));
}

/*
 * The function fiat_pasta_to_bytes serializes a field element NOT in the Montgomery domain to bytes in little-endian order.
 * Preconditions:
 *   0 ≤ eval arg1 < m
 * Postconditions:
 *   out1 = map (λ x, ⌊((eval arg1 mod m) mod 2^(8 * (x + 1))) / 2^(8 * x)⌋) [0..31]
 *
 * Input Bounds:
 *   arg1: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0x7fffffffffffffff]]
 * Output Bounds:
 *   out1: [[0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0x7f]]
 */
void fiat_pasta_to_bytes(uint8_t out1[32], const uint64_t arg1[4]) {
  uint64_t x1;
  uint64_t x2;
  uint64_t x3;
  uint64_t x4;
  uint8_t x5;
  uint64_t x6;
  uint8_t x7;
  uint64_t x8;
  uint8_t x9;
  uint64_t x10;
  uint8_t x11;
  uint64_t x12;
  uint8_t x13;
  uint64_t x14;
  uint8_t x15;
  uint64_t x16;
  uint8_t x17;
  uint8_t x18;
  uint8_t x19;
  uint64_t x20;
  uint8_t x21;
  uint64_t x22;
  uint8_t x23;
  uint64_t x24;
  uint8_t x25;
  uint64_t x26;
  uint8_t x27;
  uint64_t x28;
  uint8_t x29;
  uint64_t x30;
  uint8_t x31;
  uint8_t x32;
  uint8_t x33;
  uint64_t x34;
  uint8_t x35;
  uint64_t x36;
  uint8_t x37;
  uint64_t x38;
  uint8_t x39;
  uint64_t x40;
  uint8_t x41;
  uint64_t x42;
  uint8_t x43;
  uint64_t x44;
  uint8_t x45;
  uint8_t x46;
  uint8_t x47;
  uint64_t x48;
  uint8_t x49;
  uint64_t x50;
  uint8_t x51;
  uint64_t x52;
  uint8_t x53;
  uint64_t x54;
  uint8_t x55;
  uint64_t x56;
  uint8_t x57;
  uint64_t x58;
  uint8_t x59;
  uint8_t x60;
  x1 = (arg1[3]);
  x2 = (arg1[2]);
  x3 = (arg1[1]);
  x4 = (arg1[0]);
  x5 = (uint8_t)(x4 & UINT8_C(0xff));
  x6 = (x4 >> 8);
  x7 = (uint8_t)(x6 & UINT8_C(0xff));
  x8 = (x6 >> 8);
  x9 = (uint8_t)(x8 & UINT8_C(0xff));
  x10 = (x8 >> 8);
  x11 = (uint8_t)(x10 & UINT8_C(0xff));
  x12 = (x10 >> 8);
  x13 = (uint8_t)(x12 & UINT8_C(0xff));
  x14 = (x12 >> 8);
  x15 = (uint8_t)(x14 & UINT8_C(0xff));
  x16 = (x14 >> 8);
  x17 = (uint8_t)(x16 & UINT8_C(0xff));
  x18 = (uint8_t)(x16 >> 8);
  x19 = (uint8_t)(x3 & UINT8_C(0xff));
  x20 = (x3 >> 8);
  x21 = (uint8_t)(x20 & UINT8_C(0xff));
  x22 = (x20 >> 8);
  x23 = (uint8_t)(x22 & UINT8_C(0xff));
  x24 = (x22 >> 8);
  x25 = (uint8_t)(x24 & UINT8_C(0xff));
  x26 = (x24 >> 8);
  x27 = (uint8_t)(x26 & UINT8_C(0xff));
  x28 = (x26 >> 8);
  x29 = (uint8_t)(x28 & UINT8_C(0xff));
  x30 = (x28 >> 8);
  x31 = (uint8_t)(x30 & UINT8_C(0xff));
  x32 = (uint8_t)(x30 >> 8);
  x33 = (uint8_t)(x2 & UINT8_C(0xff));
  x34 = (x2 >> 8);
  x35 = (uint8_t)(x34 & UINT8_C(0xff));
  x36 = (x34 >> 8);
  x37 = (uint8_t)(x36 & UINT8_C(0xff));
  x38 = (x36 >> 8);
  x39 = (uint8_t)(x38 & UINT8_C(0xff));
  x40 = (x38 >> 8);
  x41 = (uint8_t)(x40 & UINT8_C(0xff));
  x42 = (x40 >> 8);
  x43 = (uint8_t)(x42 & UINT8_C(0xff));
  x44 = (x42 >> 8);
  x45 = (uint8_t)(x44 & UINT8_C(0xff));
  x46 = (uint8_t)(x44 >> 8);
  x47 = (uint8_t)(x1 & UINT8_C(0xff));
  x48 = (x1 >> 8);
  x49 = (uint8_t)(x48 & UINT8_C(0xff));
  x50 = (x48 >> 8);
  x51 = (uint8_t)(x50 & UINT8_C(0xff));
  x52 = (x50 >> 8);
  x53 = (uint8_t)(x52 & UINT8_C(0xff));
  x54 = (x52 >> 8);
  x55 = (uint8_t)(x54 & UINT8_C(0xff));
  x56 = (x54 >> 8);
  x57 = (uint8_t)(x56 & UINT8_C(0xff));
  x58 = (x56 >> 8);
  x59 = (uint8_t)(x58 & UINT8_C(0xff));
  x60 = (uint8_t)(x58 >> 8);
  out1[0] = x5;
  out1[1] = x7;
  out1[2] = x9;
  out1[3] = x11;
  out1[4] = x13;
  out1[5] = x15;
  out1[6] = x17;
  out1[7] = x18;
  out1[8] = x19;
  out1[9] = x21;
  out1[10] = x23;
  out1[11] = x25;
  out1[12] = x27;
  out1[13] = x29;
  out1[14] = x31;
  out1[15] = x32;
  out1[16] = x33;
  out1[17] = x35;
  out1[18] = x37;
  out1[19] = x39;
  out1[20] = x41;
  out1[21] = x43;
  out1[22] = x45;
  out1[23] = x46;
  out1[24] = x47;
  out1[25] = x49;
  out1[26] = x51;
  out1[27] = x53;
  out1[28] = x55;
  out1[29] = x57;
  out1[30] = x59;
  out1[31] = x60;
}

/*
 * The function fiat_pasta_from_bytes deserializes a field element NOT in the Montgomery domain from bytes in little-endian order.
 * Preconditions:
 *   0 ≤ bytes_eval arg1 < m
 * Postconditions:
 *   eval out1 mod m = bytes_eval arg1 mod m
 *   0 ≤ eval out1 < m
 *
 * Input Bounds:
 *   arg1: [[0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0xff], [0x0 ~> 0x7f]]
 * Output Bounds:
 *   out1: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0x7fffffffffffffff]]
 */
void fiat_pasta_from_bytes(uint64_t out1[4], const uint8_t arg1[32]) {
  uint64_t x1;
  uint64_t x2;
  uint64_t x3;
  uint64_t x4;
  uint64_t x5;
  uint64_t x6;
  uint64_t x7;
  uint8_t x8;
  uint64_t x9;
  uint64_t x10;
  uint64_t x11;
  uint64_t x12;
  uint64_t x13;
  uint64_t x14;
  uint64_t x15;
  uint8_t x16;
  uint64_t x17;
  uint64_t x18;
  uint64_t x19;
  uint64_t x20;
  uint64_t x21;
  uint64_t x22;
  uint64_t x23;
  uint8_t x24;
  uint64_t x25;
  uint64_t x26;
  uint64_t x27;
  uint64_t x28;
  uint64_t x29;
  uint64_t x30;
  uint64_t x31;
  uint8_t x32;
  uint64_t x33;
  uint64_t x34;
  uint64_t x35;
  uint64_t x36;
  uint64_t x37;
  uint64_t x38;
  uint64_t x39;
  uint64_t x40;
  uint64_t x41;
  uint64_t x42;
  uint64_t x43;
  uint64_t x44;
  uint64_t x45;
  uint64_t x46;
  uint64_t x47;
  uint64_t x48;
  uint64_t x49;
  uint64_t x50;
  uint64_t x51;
  uint64_t x52;
  uint64_t x53;
  uint64_t x54;
  uint64_t x55;
  uint64_t x56;
  uint64_t x57;
  uint64_t x58;
  uint64_t x59;
  uint64_t x60;
  x1 = ((uint64_t)(arg1[31]) << 56);
  x2 = ((uint64_t)(arg1[30]) << 48);
  x3 = ((uint64_t)(arg1[29]) << 40);
  x4 = ((uint64_t)(arg1[28]) << 32);
  x5 = ((uint64_t)(arg1[27]) << 24);
  x6 = ((uint64_t)(arg1[26]) << 16);
  x7 = ((uint64_t)(arg1[25]) << 8);
  x8 = (arg1[24]);
  x9 = ((uint64_t)(arg1[23]) << 56);
  x10 = ((uint64_t)(arg1[22]) << 48);
  x11 = ((uint64_t)(arg1[21]) << 40);
  x12 = ((uint64_t)(arg1[20]) << 32);
  x13 = ((uint64_t)(arg1[19]) << 24);
  x14 = ((uint64_t)(arg1[18]) << 16);
  x15 = ((uint64_t)(arg1[17]) << 8);
  x16 = (arg1[16]);
  x17 = ((uint64_t)(arg1[15]) << 56);
  x18 = ((uint64_t)(arg1[14]) << 48);
  x19 = ((uint64_t)(arg1[13]) << 40);
  x20 = ((uint64_t)(arg1[12]) << 32);
  x21 = ((uint64_t)(arg1[11]) << 24);
  x22 = ((uint64_t)(arg1[10]) << 16);
  x23 = ((uint64_t)(arg1[9]) << 8);
  x24 = (arg1[8]);
  x25 = ((uint64_t)(arg1[7]) << 56);
  x26 = ((uint64_t)(arg1[6]) << 48);
  x27 = ((uint64_t)(arg1[5]) << 40);
  x28 = ((uint64_t)(arg1[4]) << 32);
  x29 = ((uint64_t)(arg1[3]) << 24);
  x30 = ((uint64_t)(arg1[2]) << 16);
  x31 = ((uint64_t)(arg1[1]) << 8);
  x32 = (arg1[0]);
  x33 = (x31 + (uint64_t)x32);
  x34 = (x30 + x33);
  x35 = (x29 + x34);
  x36 = (x28 + x35);
  x37 = (x27 + x36);
  x38 = (x26 + x37);
  x39 = (x25 + x38);
  x40 = (x23 + (uint64_t)x24);
  x41 = (x22 + x40);
  x42 = (x21 + x41);
  x43 = (x20 + x42);
  x44 = (x19 + x43);
  x45 = (x18 + x44);
  x46 = (x17 + x45);
  x47 = (x15 + (uint64_t)x16);
  x48 = (x14 + x47);
  x49 = (x13 + x48);
  x50 = (x12 + x49);
  x51 = (x11 + x50);
  x52 = (x10 + x51);
  x53 = (x9 + x52);
  x54 = (x7 + (uint64_t)x8);
  x55 = (x6 + x54);
  x56 = (x5 + x55);
  x57 = (x4 + x56);
  x58 = (x3 + x57);
  x59 = (x2 + x58);
  x60 = (x1 + x59);
  out1[0] = x39;
  out1[1] = x46;
  out1[2] = x53;
  out1[3] = x60;
}

/*
 * The function fiat_pasta_add adds two field elements in the Montgomery domain.
 * Preconditions:
 *   0 ≤ eval arg1 < m
 *   0 ≤ eval arg2 < m
 * Postconditions:
 *   eval (from_montgomery out1) mod m = (eval (from_montgomery arg1) + eval (from_montgomery arg2)) mod m
 *   0 ≤ eval out1 < m
 *
 * Input Bounds:
 *   arg1: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff]]
 *   arg2: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff]]
 * Output Bounds:
 *   out1: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff]]
 */
void fiat_pasta_add(uint64_t out1[4], const uint64_t arg1[4], const uint64_t arg2[4], bool fq) {
  const uint64_t *prime;
  if (fq) {
    prime = mina_fq;
  } else {
    prime = mina_fp;
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
  fiat_pasta_uint1 x10;
  uint64_t x11;
  fiat_pasta_uint1 x12;
  uint64_t x13;
  fiat_pasta_uint1 x14;
  uint64_t x15;
  fiat_pasta_uint1 x16;
  uint64_t x17;
  fiat_pasta_uint1 x18;
  fiat_pasta_addcarryx_u64(&x1, &x2, 0x0, (arg1[0]), (arg2[0]));
  fiat_pasta_addcarryx_u64(&x3, &x4, x2, (arg1[1]), (arg2[1]));
  fiat_pasta_addcarryx_u64(&x5, &x6, x4, (arg1[2]), (arg2[2]));
  fiat_pasta_addcarryx_u64(&x7, &x8, x6, (arg1[3]), (arg2[3]));
  fiat_pasta_subborrowx_u64(&x9, &x10, 0x0, x1, prime[3]);
  fiat_pasta_subborrowx_u64(&x11, &x12, x10, x3, prime[2]);
  fiat_pasta_subborrowx_u64(&x13, &x14, x12, x5, 0x0);
  fiat_pasta_subborrowx_u64(&x15, &x16, x14, x7, prime[1]);
  fiat_pasta_subborrowx_u64(&x17, &x18, x16, x8, 0x0);
  fiat_pasta_cmovznz_u64(&out1[0], x18, x9, x1);
  fiat_pasta_cmovznz_u64(&out1[1], x18, x11, x3);
  fiat_pasta_cmovznz_u64(&out1[2], x18, x13, x5);
  fiat_pasta_cmovznz_u64(&out1[3], x18, x15, x7);
}

/*
 * The function fiat_pasta_mul multiplies two field elements in the Montgomery domain.
 * Preconditions:
 *   0 ≤ eval arg1 < m
 *   0 ≤ eval arg2 < m
 * Postconditions:
 *   eval (from_montgomery out1) mod m = (eval (from_montgomery arg1) * eval (from_montgomery arg2)) mod m
 *   0 ≤ eval out1 < m
 *
 * Input Bounds:
 *   arg1: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff]]
 *   arg2: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff]]
 * Output Bounds:
 *   out1: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff]]
 */
void fiat_pasta_mul(uint64_t out1[4], const uint64_t arg1[4], const uint64_t arg2[4], bool fq) {
  const uint64_t *prime;
  if (fq) {
    prime = mina_fq;
  } else {
    prime = mina_fp;
  }

  uint64_t x1;
  uint64_t x2;
  uint64_t x3;
  uint64_t x4;
  uint64_t x5;
  uint64_t x6;
  uint64_t x7;
  uint64_t x8;
  uint64_t x9;
  uint64_t x10;
  uint64_t x11;
  uint64_t x12;
  uint64_t x13;
  fiat_pasta_uint1 x14;
  uint64_t x15;
  fiat_pasta_uint1 x16;
  uint64_t x17;
  fiat_pasta_uint1 x18;
  uint64_t x19;
  uint64_t x20;
  uint64_t x21;
  uint64_t x22;
  uint64_t x23;
  uint64_t x24;
  uint64_t x25;
  uint64_t x26;
  uint64_t x27;
  uint64_t x28;
  fiat_pasta_uint1 x29;
  uint64_t x30;
  uint64_t x31;
  fiat_pasta_uint1 x32;
  uint64_t x33;
  fiat_pasta_uint1 x34;
  uint64_t x35;
  fiat_pasta_uint1 x36;
  uint64_t x37;
  fiat_pasta_uint1 x38;
  uint64_t x39;
  fiat_pasta_uint1 x40;
  uint64_t x41;
  uint64_t x42;
  uint64_t x43;
  uint64_t x44;
  uint64_t x45;
  uint64_t x46;
  uint64_t x47;
  uint64_t x48;
  uint64_t x49;
  fiat_pasta_uint1 x50;
  uint64_t x51;
  fiat_pasta_uint1 x52;
  uint64_t x53;
  fiat_pasta_uint1 x54;
  uint64_t x55;
  uint64_t x56;
  fiat_pasta_uint1 x57;
  uint64_t x58;
  fiat_pasta_uint1 x59;
  uint64_t x60;
  fiat_pasta_uint1 x61;
  uint64_t x62;
  fiat_pasta_uint1 x63;
  uint64_t x64;
  fiat_pasta_uint1 x65;
  uint64_t x66;
  uint64_t x67;
  uint64_t x68;
  uint64_t x69;
  uint64_t x70;
  uint64_t x71;
  uint64_t x72;
  uint64_t x73;
  uint64_t x74;
  fiat_pasta_uint1 x75;
  uint64_t x76;
  uint64_t x77;
  fiat_pasta_uint1 x78;
  uint64_t x79;
  fiat_pasta_uint1 x80;
  uint64_t x81;
  fiat_pasta_uint1 x82;
  uint64_t x83;
  fiat_pasta_uint1 x84;
  uint64_t x85;
  fiat_pasta_uint1 x86;
  uint64_t x87;
  uint64_t x88;
  uint64_t x89;
  uint64_t x90;
  uint64_t x91;
  uint64_t x92;
  uint64_t x93;
  uint64_t x94;
  uint64_t x95;
  uint64_t x96;
  fiat_pasta_uint1 x97;
  uint64_t x98;
  fiat_pasta_uint1 x99;
  uint64_t x100;
  fiat_pasta_uint1 x101;
  uint64_t x102;
  uint64_t x103;
  fiat_pasta_uint1 x104;
  uint64_t x105;
  fiat_pasta_uint1 x106;
  uint64_t x107;
  fiat_pasta_uint1 x108;
  uint64_t x109;
  fiat_pasta_uint1 x110;
  uint64_t x111;
  fiat_pasta_uint1 x112;
  uint64_t x113;
  uint64_t x114;
  uint64_t x115;
  uint64_t x116;
  uint64_t x117;
  uint64_t x118;
  uint64_t x119;
  uint64_t x120;
  uint64_t x121;
  fiat_pasta_uint1 x122;
  uint64_t x123;
  uint64_t x124;
  fiat_pasta_uint1 x125;
  uint64_t x126;
  fiat_pasta_uint1 x127;
  uint64_t x128;
  fiat_pasta_uint1 x129;
  uint64_t x130;
  fiat_pasta_uint1 x131;
  uint64_t x132;
  fiat_pasta_uint1 x133;
  uint64_t x134;
  uint64_t x135;
  uint64_t x136;
  uint64_t x137;
  uint64_t x138;
  uint64_t x139;
  uint64_t x140;
  uint64_t x141;
  uint64_t x142;
  uint64_t x143;
  fiat_pasta_uint1 x144;
  uint64_t x145;
  fiat_pasta_uint1 x146;
  uint64_t x147;
  fiat_pasta_uint1 x148;
  uint64_t x149;
  uint64_t x150;
  fiat_pasta_uint1 x151;
  uint64_t x152;
  fiat_pasta_uint1 x153;
  uint64_t x154;
  fiat_pasta_uint1 x155;
  uint64_t x156;
  fiat_pasta_uint1 x157;
  uint64_t x158;
  fiat_pasta_uint1 x159;
  uint64_t x160;
  uint64_t x161;
  uint64_t x162;
  uint64_t x163;
  uint64_t x164;
  uint64_t x165;
  uint64_t x166;
  uint64_t x167;
  uint64_t x168;
  fiat_pasta_uint1 x169;
  uint64_t x170;
  uint64_t x171;
  fiat_pasta_uint1 x172;
  uint64_t x173;
  fiat_pasta_uint1 x174;
  uint64_t x175;
  fiat_pasta_uint1 x176;
  uint64_t x177;
  fiat_pasta_uint1 x178;
  uint64_t x179;
  fiat_pasta_uint1 x180;
  uint64_t x181;
  uint64_t x182;
  fiat_pasta_uint1 x183;
  uint64_t x184;
  fiat_pasta_uint1 x185;
  uint64_t x186;
  fiat_pasta_uint1 x187;
  uint64_t x188;
  fiat_pasta_uint1 x189;
  uint64_t x190;
  fiat_pasta_uint1 x191;
  x1 = (arg1[1]);
  x2 = (arg1[2]);
  x3 = (arg1[3]);
  x4 = (arg1[0]);
  fiat_pasta_mulx_u64(&x5, &x6, x4, (arg2[3]));
  fiat_pasta_mulx_u64(&x7, &x8, x4, (arg2[2]));
  fiat_pasta_mulx_u64(&x9, &x10, x4, (arg2[1]));
  fiat_pasta_mulx_u64(&x11, &x12, x4, (arg2[0]));
  fiat_pasta_addcarryx_u64(&x13, &x14, 0x0, x12, x9);
  fiat_pasta_addcarryx_u64(&x15, &x16, x14, x10, x7);
  fiat_pasta_addcarryx_u64(&x17, &x18, x16, x8, x5);
  x19 = (x18 + x6);
  fiat_pasta_mulx_u64(&x20, &x21, x11, prime[0]);
  fiat_pasta_mulx_u64(&x22, &x23, x20, prime[1]);
  fiat_pasta_mulx_u64(&x24, &x25, x20, prime[2]);
  fiat_pasta_mulx_u64(&x26, &x27, x20, prime[3]);
  fiat_pasta_addcarryx_u64(&x28, &x29, 0x0, x27, x24);
  x30 = (x29 + x25);
  fiat_pasta_addcarryx_u64(&x31, &x32, 0x0, x11, x26);
  fiat_pasta_addcarryx_u64(&x33, &x34, x32, x13, x28);
  fiat_pasta_addcarryx_u64(&x35, &x36, x34, x15, x30);
  fiat_pasta_addcarryx_u64(&x37, &x38, x36, x17, x22);
  fiat_pasta_addcarryx_u64(&x39, &x40, x38, x19, x23);
  fiat_pasta_mulx_u64(&x41, &x42, x1, (arg2[3]));
  fiat_pasta_mulx_u64(&x43, &x44, x1, (arg2[2]));
  fiat_pasta_mulx_u64(&x45, &x46, x1, (arg2[1]));
  fiat_pasta_mulx_u64(&x47, &x48, x1, (arg2[0]));
  fiat_pasta_addcarryx_u64(&x49, &x50, 0x0, x48, x45);
  fiat_pasta_addcarryx_u64(&x51, &x52, x50, x46, x43);
  fiat_pasta_addcarryx_u64(&x53, &x54, x52, x44, x41);
  x55 = (x54 + x42);
  fiat_pasta_addcarryx_u64(&x56, &x57, 0x0, x33, x47);
  fiat_pasta_addcarryx_u64(&x58, &x59, x57, x35, x49);
  fiat_pasta_addcarryx_u64(&x60, &x61, x59, x37, x51);
  fiat_pasta_addcarryx_u64(&x62, &x63, x61, x39, x53);
  fiat_pasta_addcarryx_u64(&x64, &x65, x63, x40, x55);
  fiat_pasta_mulx_u64(&x66, &x67, x56, prime[0]);
  fiat_pasta_mulx_u64(&x68, &x69, x66, prime[1]);
  fiat_pasta_mulx_u64(&x70, &x71, x66, prime[2]);
  fiat_pasta_mulx_u64(&x72, &x73, x66, prime[3]);
  fiat_pasta_addcarryx_u64(&x74, &x75, 0x0, x73, x70);
  x76 = (x75 + x71);
  fiat_pasta_addcarryx_u64(&x77, &x78, 0x0, x56, x72);
  fiat_pasta_addcarryx_u64(&x79, &x80, x78, x58, x74);
  fiat_pasta_addcarryx_u64(&x81, &x82, x80, x60, x76);
  fiat_pasta_addcarryx_u64(&x83, &x84, x82, x62, x68);
  fiat_pasta_addcarryx_u64(&x85, &x86, x84, x64, x69);
  x87 = ((uint64_t)x86 + x65);
  fiat_pasta_mulx_u64(&x88, &x89, x2, (arg2[3]));
  fiat_pasta_mulx_u64(&x90, &x91, x2, (arg2[2]));
  fiat_pasta_mulx_u64(&x92, &x93, x2, (arg2[1]));
  fiat_pasta_mulx_u64(&x94, &x95, x2, (arg2[0]));
  fiat_pasta_addcarryx_u64(&x96, &x97, 0x0, x95, x92);
  fiat_pasta_addcarryx_u64(&x98, &x99, x97, x93, x90);
  fiat_pasta_addcarryx_u64(&x100, &x101, x99, x91, x88);
  x102 = (x101 + x89);
  fiat_pasta_addcarryx_u64(&x103, &x104, 0x0, x79, x94);
  fiat_pasta_addcarryx_u64(&x105, &x106, x104, x81, x96);
  fiat_pasta_addcarryx_u64(&x107, &x108, x106, x83, x98);
  fiat_pasta_addcarryx_u64(&x109, &x110, x108, x85, x100);
  fiat_pasta_addcarryx_u64(&x111, &x112, x110, x87, x102);
  fiat_pasta_mulx_u64(&x113, &x114, x103, prime[0]);
  fiat_pasta_mulx_u64(&x115, &x116, x113, prime[1]);
  fiat_pasta_mulx_u64(&x117, &x118, x113, prime[2]);
  fiat_pasta_mulx_u64(&x119, &x120, x113, prime[3]);
  fiat_pasta_addcarryx_u64(&x121, &x122, 0x0, x120, x117);
  x123 = (x122 + x118);
  fiat_pasta_addcarryx_u64(&x124, &x125, 0x0, x103, x119);
  fiat_pasta_addcarryx_u64(&x126, &x127, x125, x105, x121);
  fiat_pasta_addcarryx_u64(&x128, &x129, x127, x107, x123);
  fiat_pasta_addcarryx_u64(&x130, &x131, x129, x109, x115);
  fiat_pasta_addcarryx_u64(&x132, &x133, x131, x111, x116);
  x134 = ((uint64_t)x133 + x112);
  fiat_pasta_mulx_u64(&x135, &x136, x3, (arg2[3]));
  fiat_pasta_mulx_u64(&x137, &x138, x3, (arg2[2]));
  fiat_pasta_mulx_u64(&x139, &x140, x3, (arg2[1]));
  fiat_pasta_mulx_u64(&x141, &x142, x3, (arg2[0]));
  fiat_pasta_addcarryx_u64(&x143, &x144, 0x0, x142, x139);
  fiat_pasta_addcarryx_u64(&x145, &x146, x144, x140, x137);
  fiat_pasta_addcarryx_u64(&x147, &x148, x146, x138, x135);
  x149 = (x148 + x136);
  fiat_pasta_addcarryx_u64(&x150, &x151, 0x0, x126, x141);
  fiat_pasta_addcarryx_u64(&x152, &x153, x151, x128, x143);
  fiat_pasta_addcarryx_u64(&x154, &x155, x153, x130, x145);
  fiat_pasta_addcarryx_u64(&x156, &x157, x155, x132, x147);
  fiat_pasta_addcarryx_u64(&x158, &x159, x157, x134, x149);
  fiat_pasta_mulx_u64(&x160, &x161, x150, prime[0]);
  fiat_pasta_mulx_u64(&x162, &x163, x160, prime[1]);
  fiat_pasta_mulx_u64(&x164, &x165, x160, prime[2]);
  fiat_pasta_mulx_u64(&x166, &x167, x160, prime[3]);
  fiat_pasta_addcarryx_u64(&x168, &x169, 0x0, x167, x164);
  x170 = (x169 + x165);
  fiat_pasta_addcarryx_u64(&x171, &x172, 0x0, x150, x166);
  fiat_pasta_addcarryx_u64(&x173, &x174, x172, x152, x168);
  fiat_pasta_addcarryx_u64(&x175, &x176, x174, x154, x170);
  fiat_pasta_addcarryx_u64(&x177, &x178, x176, x156, x162);
  fiat_pasta_addcarryx_u64(&x179, &x180, x178, x158, x163);
  x181 = ((uint64_t)x180 + x159);
  fiat_pasta_subborrowx_u64(&x182, &x183, 0x0, x173, prime[3]);
  fiat_pasta_subborrowx_u64(&x184, &x185, x183, x175, prime[2]);
  fiat_pasta_subborrowx_u64(&x186, &x187, x185, x177, 0x0);
  fiat_pasta_subborrowx_u64(&x188, &x189, x187, x179, prime[1]);
  fiat_pasta_subborrowx_u64(&x190, &x191, x189, x181, 0x0);
  fiat_pasta_cmovznz_u64(&out1[0], x191, x182, x173);
  fiat_pasta_cmovznz_u64(&out1[1], x191, x184, x175);
  fiat_pasta_cmovznz_u64(&out1[2], x191, x186, x177);
  fiat_pasta_cmovznz_u64(&out1[3], x191, x188, x179);
}

/*
 * The function fiat_pasta_square squares a field element in the Montgomery domain.
 * Preconditions:
 *   0 ≤ eval arg1 < m
 * Postconditions:
 *   eval (from_montgomery out1) mod m = (eval (from_montgomery arg1) * eval (from_montgomery arg1)) mod m
 *   0 ≤ eval out1 < m
 *
 * Input Bounds:
 *   arg1: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff]]
 * Output Bounds:
 *   out1: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff]]
 */
void fiat_pasta_square(uint64_t out1[4], const uint64_t arg1[4], bool fq) {
  const uint64_t *prime;
  if (fq) {
    prime = mina_fq;
  } else {
    prime = mina_fp;
  }

  uint64_t x1;
  uint64_t x2;
  uint64_t x3;
  uint64_t x4;
  uint64_t x5;
  uint64_t x6;
  uint64_t x7;
  uint64_t x8;
  uint64_t x9;
  uint64_t x10;
  uint64_t x11;
  uint64_t x12;
  uint64_t x13;
  fiat_pasta_uint1 x14;
  uint64_t x15;
  fiat_pasta_uint1 x16;
  uint64_t x17;
  fiat_pasta_uint1 x18;
  uint64_t x19;
  uint64_t x20;
  uint64_t x21;
  uint64_t x22;
  uint64_t x23;
  uint64_t x24;
  uint64_t x25;
  uint64_t x26;
  uint64_t x27;
  uint64_t x28;
  fiat_pasta_uint1 x29;
  uint64_t x30;
  uint64_t x31;
  fiat_pasta_uint1 x32;
  uint64_t x33;
  fiat_pasta_uint1 x34;
  uint64_t x35;
  fiat_pasta_uint1 x36;
  uint64_t x37;
  fiat_pasta_uint1 x38;
  uint64_t x39;
  fiat_pasta_uint1 x40;
  uint64_t x41;
  uint64_t x42;
  uint64_t x43;
  uint64_t x44;
  uint64_t x45;
  uint64_t x46;
  uint64_t x47;
  uint64_t x48;
  uint64_t x49;
  fiat_pasta_uint1 x50;
  uint64_t x51;
  fiat_pasta_uint1 x52;
  uint64_t x53;
  fiat_pasta_uint1 x54;
  uint64_t x55;
  uint64_t x56;
  fiat_pasta_uint1 x57;
  uint64_t x58;
  fiat_pasta_uint1 x59;
  uint64_t x60;
  fiat_pasta_uint1 x61;
  uint64_t x62;
  fiat_pasta_uint1 x63;
  uint64_t x64;
  fiat_pasta_uint1 x65;
  uint64_t x66;
  uint64_t x67;
  uint64_t x68;
  uint64_t x69;
  uint64_t x70;
  uint64_t x71;
  uint64_t x72;
  uint64_t x73;
  uint64_t x74;
  fiat_pasta_uint1 x75;
  uint64_t x76;
  uint64_t x77;
  fiat_pasta_uint1 x78;
  uint64_t x79;
  fiat_pasta_uint1 x80;
  uint64_t x81;
  fiat_pasta_uint1 x82;
  uint64_t x83;
  fiat_pasta_uint1 x84;
  uint64_t x85;
  fiat_pasta_uint1 x86;
  uint64_t x87;
  uint64_t x88;
  uint64_t x89;
  uint64_t x90;
  uint64_t x91;
  uint64_t x92;
  uint64_t x93;
  uint64_t x94;
  uint64_t x95;
  uint64_t x96;
  fiat_pasta_uint1 x97;
  uint64_t x98;
  fiat_pasta_uint1 x99;
  uint64_t x100;
  fiat_pasta_uint1 x101;
  uint64_t x102;
  uint64_t x103;
  fiat_pasta_uint1 x104;
  uint64_t x105;
  fiat_pasta_uint1 x106;
  uint64_t x107;
  fiat_pasta_uint1 x108;
  uint64_t x109;
  fiat_pasta_uint1 x110;
  uint64_t x111;
  fiat_pasta_uint1 x112;
  uint64_t x113;
  uint64_t x114;
  uint64_t x115;
  uint64_t x116;
  uint64_t x117;
  uint64_t x118;
  uint64_t x119;
  uint64_t x120;
  uint64_t x121;
  fiat_pasta_uint1 x122;
  uint64_t x123;
  uint64_t x124;
  fiat_pasta_uint1 x125;
  uint64_t x126;
  fiat_pasta_uint1 x127;
  uint64_t x128;
  fiat_pasta_uint1 x129;
  uint64_t x130;
  fiat_pasta_uint1 x131;
  uint64_t x132;
  fiat_pasta_uint1 x133;
  uint64_t x134;
  uint64_t x135;
  uint64_t x136;
  uint64_t x137;
  uint64_t x138;
  uint64_t x139;
  uint64_t x140;
  uint64_t x141;
  uint64_t x142;
  uint64_t x143;
  fiat_pasta_uint1 x144;
  uint64_t x145;
  fiat_pasta_uint1 x146;
  uint64_t x147;
  fiat_pasta_uint1 x148;
  uint64_t x149;
  uint64_t x150;
  fiat_pasta_uint1 x151;
  uint64_t x152;
  fiat_pasta_uint1 x153;
  uint64_t x154;
  fiat_pasta_uint1 x155;
  uint64_t x156;
  fiat_pasta_uint1 x157;
  uint64_t x158;
  fiat_pasta_uint1 x159;
  uint64_t x160;
  uint64_t x161;
  uint64_t x162;
  uint64_t x163;
  uint64_t x164;
  uint64_t x165;
  uint64_t x166;
  uint64_t x167;
  uint64_t x168;
  fiat_pasta_uint1 x169;
  uint64_t x170;
  uint64_t x171;
  fiat_pasta_uint1 x172;
  uint64_t x173;
  fiat_pasta_uint1 x174;
  uint64_t x175;
  fiat_pasta_uint1 x176;
  uint64_t x177;
  fiat_pasta_uint1 x178;
  uint64_t x179;
  fiat_pasta_uint1 x180;
  uint64_t x181;
  uint64_t x182;
  fiat_pasta_uint1 x183;
  uint64_t x184;
  fiat_pasta_uint1 x185;
  uint64_t x186;
  fiat_pasta_uint1 x187;
  uint64_t x188;
  fiat_pasta_uint1 x189;
  uint64_t x190;
  fiat_pasta_uint1 x191;
  x1 = (arg1[1]);
  x2 = (arg1[2]);
  x3 = (arg1[3]);
  x4 = (arg1[0]);
  fiat_pasta_mulx_u64(&x5, &x6, x4, (arg1[3]));
  fiat_pasta_mulx_u64(&x7, &x8, x4, (arg1[2]));
  fiat_pasta_mulx_u64(&x9, &x10, x4, (arg1[1]));
  fiat_pasta_mulx_u64(&x11, &x12, x4, (arg1[0]));
  fiat_pasta_addcarryx_u64(&x13, &x14, 0x0, x12, x9);
  fiat_pasta_addcarryx_u64(&x15, &x16, x14, x10, x7);
  fiat_pasta_addcarryx_u64(&x17, &x18, x16, x8, x5);
  x19 = (x18 + x6);
  fiat_pasta_mulx_u64(&x20, &x21, x11, prime[0]);
  fiat_pasta_mulx_u64(&x22, &x23, x20, prime[1]);
  fiat_pasta_mulx_u64(&x24, &x25, x20, prime[2]);
  fiat_pasta_mulx_u64(&x26, &x27, x20, prime[3]);
  fiat_pasta_addcarryx_u64(&x28, &x29, 0x0, x27, x24);
  x30 = (x29 + x25);
  fiat_pasta_addcarryx_u64(&x31, &x32, 0x0, x11, x26);
  fiat_pasta_addcarryx_u64(&x33, &x34, x32, x13, x28);
  fiat_pasta_addcarryx_u64(&x35, &x36, x34, x15, x30);
  fiat_pasta_addcarryx_u64(&x37, &x38, x36, x17, x22);
  fiat_pasta_addcarryx_u64(&x39, &x40, x38, x19, x23);
  fiat_pasta_mulx_u64(&x41, &x42, x1, (arg1[3]));
  fiat_pasta_mulx_u64(&x43, &x44, x1, (arg1[2]));
  fiat_pasta_mulx_u64(&x45, &x46, x1, (arg1[1]));
  fiat_pasta_mulx_u64(&x47, &x48, x1, (arg1[0]));
  fiat_pasta_addcarryx_u64(&x49, &x50, 0x0, x48, x45);
  fiat_pasta_addcarryx_u64(&x51, &x52, x50, x46, x43);
  fiat_pasta_addcarryx_u64(&x53, &x54, x52, x44, x41);
  x55 = (x54 + x42);
  fiat_pasta_addcarryx_u64(&x56, &x57, 0x0, x33, x47);
  fiat_pasta_addcarryx_u64(&x58, &x59, x57, x35, x49);
  fiat_pasta_addcarryx_u64(&x60, &x61, x59, x37, x51);
  fiat_pasta_addcarryx_u64(&x62, &x63, x61, x39, x53);
  fiat_pasta_addcarryx_u64(&x64, &x65, x63, x40, x55);
  fiat_pasta_mulx_u64(&x66, &x67, x56, prime[0]);
  fiat_pasta_mulx_u64(&x68, &x69, x66, prime[1]);
  fiat_pasta_mulx_u64(&x70, &x71, x66, prime[2]);
  fiat_pasta_mulx_u64(&x72, &x73, x66, prime[3]);
  fiat_pasta_addcarryx_u64(&x74, &x75, 0x0, x73, x70);
  x76 = (x75 + x71);
  fiat_pasta_addcarryx_u64(&x77, &x78, 0x0, x56, x72);
  fiat_pasta_addcarryx_u64(&x79, &x80, x78, x58, x74);
  fiat_pasta_addcarryx_u64(&x81, &x82, x80, x60, x76);
  fiat_pasta_addcarryx_u64(&x83, &x84, x82, x62, x68);
  fiat_pasta_addcarryx_u64(&x85, &x86, x84, x64, x69);
  x87 = ((uint64_t)x86 + x65);
  fiat_pasta_mulx_u64(&x88, &x89, x2, (arg1[3]));
  fiat_pasta_mulx_u64(&x90, &x91, x2, (arg1[2]));
  fiat_pasta_mulx_u64(&x92, &x93, x2, (arg1[1]));
  fiat_pasta_mulx_u64(&x94, &x95, x2, (arg1[0]));
  fiat_pasta_addcarryx_u64(&x96, &x97, 0x0, x95, x92);
  fiat_pasta_addcarryx_u64(&x98, &x99, x97, x93, x90);
  fiat_pasta_addcarryx_u64(&x100, &x101, x99, x91, x88);
  x102 = (x101 + x89);
  fiat_pasta_addcarryx_u64(&x103, &x104, 0x0, x79, x94);
  fiat_pasta_addcarryx_u64(&x105, &x106, x104, x81, x96);
  fiat_pasta_addcarryx_u64(&x107, &x108, x106, x83, x98);
  fiat_pasta_addcarryx_u64(&x109, &x110, x108, x85, x100);
  fiat_pasta_addcarryx_u64(&x111, &x112, x110, x87, x102);
  fiat_pasta_mulx_u64(&x113, &x114, x103, prime[0]);
  fiat_pasta_mulx_u64(&x115, &x116, x113, prime[1]);
  fiat_pasta_mulx_u64(&x117, &x118, x113, prime[2]);
  fiat_pasta_mulx_u64(&x119, &x120, x113, prime[3]);
  fiat_pasta_addcarryx_u64(&x121, &x122, 0x0, x120, x117);
  x123 = (x122 + x118);
  fiat_pasta_addcarryx_u64(&x124, &x125, 0x0, x103, x119);
  fiat_pasta_addcarryx_u64(&x126, &x127, x125, x105, x121);
  fiat_pasta_addcarryx_u64(&x128, &x129, x127, x107, x123);
  fiat_pasta_addcarryx_u64(&x130, &x131, x129, x109, x115);
  fiat_pasta_addcarryx_u64(&x132, &x133, x131, x111, x116);
  x134 = ((uint64_t)x133 + x112);
  fiat_pasta_mulx_u64(&x135, &x136, x3, (arg1[3]));
  fiat_pasta_mulx_u64(&x137, &x138, x3, (arg1[2]));
  fiat_pasta_mulx_u64(&x139, &x140, x3, (arg1[1]));
  fiat_pasta_mulx_u64(&x141, &x142, x3, (arg1[0]));
  fiat_pasta_addcarryx_u64(&x143, &x144, 0x0, x142, x139);
  fiat_pasta_addcarryx_u64(&x145, &x146, x144, x140, x137);
  fiat_pasta_addcarryx_u64(&x147, &x148, x146, x138, x135);
  x149 = (x148 + x136);
  fiat_pasta_addcarryx_u64(&x150, &x151, 0x0, x126, x141);
  fiat_pasta_addcarryx_u64(&x152, &x153, x151, x128, x143);
  fiat_pasta_addcarryx_u64(&x154, &x155, x153, x130, x145);
  fiat_pasta_addcarryx_u64(&x156, &x157, x155, x132, x147);
  fiat_pasta_addcarryx_u64(&x158, &x159, x157, x134, x149);
  fiat_pasta_mulx_u64(&x160, &x161, x150, prime[0]);
  fiat_pasta_mulx_u64(&x162, &x163, x160, prime[1]);
  fiat_pasta_mulx_u64(&x164, &x165, x160, prime[2]);
  fiat_pasta_mulx_u64(&x166, &x167, x160, prime[3]);
  fiat_pasta_addcarryx_u64(&x168, &x169, 0x0, x167, x164);
  x170 = (x169 + x165);
  fiat_pasta_addcarryx_u64(&x171, &x172, 0x0, x150, x166);
  fiat_pasta_addcarryx_u64(&x173, &x174, x172, x152, x168);
  fiat_pasta_addcarryx_u64(&x175, &x176, x174, x154, x170);
  fiat_pasta_addcarryx_u64(&x177, &x178, x176, x156, x162);
  fiat_pasta_addcarryx_u64(&x179, &x180, x178, x158, x163);
  x181 = ((uint64_t)x180 + x159);
  fiat_pasta_subborrowx_u64(&x182, &x183, 0x0, x173, prime[3]);
  fiat_pasta_subborrowx_u64(&x184, &x185, x183, x175, prime[2]);
  fiat_pasta_subborrowx_u64(&x186, &x187, x185, x177, 0x0);
  fiat_pasta_subborrowx_u64(&x188, &x189, x187, x179, prime[1]);
  fiat_pasta_subborrowx_u64(&x190, &x191, x189, x181, 0x0);
  fiat_pasta_cmovznz_u64(&out1[0], x191, x182, x173);
  fiat_pasta_cmovznz_u64(&out1[1], x191, x184, x175);
  fiat_pasta_cmovznz_u64(&out1[2], x191, x186, x177);
  fiat_pasta_cmovznz_u64(&out1[3], x191, x188, x179);
}
