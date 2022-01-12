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
