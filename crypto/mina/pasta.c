#include "pasta.h"
#include <string.h>

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

  *out2 = a_x_b_hi + (a_x_b_mid >> 32) + (b_x_a_mid >> 32) + carry_bit;
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
  x2 = ((fiat_pasta_int1)(0x0 - x1) & mina_ff);
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
  fiat_pasta_cmovznz_u64(&x9, x8, 0x0, mina_ff);
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
  uint64_t x_minus_y_nonzero;

  fiat_pasta_sub(x_minus_y, x, y, fq);
  fiat_pasta_nonzero(&x_minus_y_nonzero, x_minus_y);
  return !x_minus_y_nonzero;
}

bool fiat_pasta_equals_zero(const uint64_t x[4]) {
  uint64_t x_nonzero;
  fiat_pasta_nonzero(&x_nonzero, x);
  return !x_nonzero;
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
 * The function fiat_pasta_from_montgomery translates a field element out of the Montgomery domain.
 * Preconditions:
 *   0 ≤ eval arg1 < m
 * Postconditions:
 *   eval out1 mod m = (eval arg1 * ((2^64)⁻¹ mod m)^4) mod m
 *   0 ≤ eval out1 < m
 *
 * Input Bounds:
 *   arg1: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff]]
 * Output Bounds:
 *   out1: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff]]
 */
void fiat_pasta_from_montgomery(uint64_t out1[4], const uint64_t arg1[4], bool fq) {
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
  fiat_pasta_uint1 x11;
  uint64_t x12;
  fiat_pasta_uint1 x13;
  uint64_t x14;
  fiat_pasta_uint1 x15;
  uint64_t x16;
  fiat_pasta_uint1 x17;
  uint64_t x18;
  uint64_t x19;
  uint64_t x20;
  uint64_t x21;
  uint64_t x22;
  uint64_t x23;
  uint64_t x24;
  uint64_t x25;
  uint64_t x26;
  fiat_pasta_uint1 x27;
  uint64_t x28;
  fiat_pasta_uint1 x29;
  uint64_t x30;
  fiat_pasta_uint1 x31;
  uint64_t x32;
  fiat_pasta_uint1 x33;
  uint64_t x34;
  fiat_pasta_uint1 x35;
  uint64_t x36;
  fiat_pasta_uint1 x37;
  uint64_t x38;
  fiat_pasta_uint1 x39;
  uint64_t x40;
  fiat_pasta_uint1 x41;
  uint64_t x42;
  uint64_t x43;
  uint64_t x44;
  uint64_t x45;
  uint64_t x46;
  uint64_t x47;
  uint64_t x48;
  uint64_t x49;
  uint64_t x50;
  fiat_pasta_uint1 x51;
  uint64_t x52;
  fiat_pasta_uint1 x53;
  uint64_t x54;
  fiat_pasta_uint1 x55;
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
  fiat_pasta_uint1 x77;
  uint64_t x78;
  fiat_pasta_uint1 x79;
  uint64_t x80;
  fiat_pasta_uint1 x81;
  uint64_t x82;
  fiat_pasta_uint1 x83;
  uint64_t x84;
  uint64_t x85;
  fiat_pasta_uint1 x86;
  uint64_t x87;
  fiat_pasta_uint1 x88;
  uint64_t x89;
  fiat_pasta_uint1 x90;
  uint64_t x91;
  fiat_pasta_uint1 x92;
  uint64_t x93;
  fiat_pasta_uint1 x94;
  x1 = (arg1[0]);
  fiat_pasta_mulx_u64(&x2, &x3, x1, prime[0]);
  fiat_pasta_mulx_u64(&x4, &x5, x2, prime[1]);
  fiat_pasta_mulx_u64(&x6, &x7, x2, prime[2]);
  fiat_pasta_mulx_u64(&x8, &x9, x2, prime[3]);
  fiat_pasta_addcarryx_u64(&x10, &x11, 0x0, x9, x6);
  fiat_pasta_addcarryx_u64(&x12, &x13, 0x0, x1, x8);
  fiat_pasta_addcarryx_u64(&x14, &x15, x13, 0x0, x10);
  fiat_pasta_addcarryx_u64(&x16, &x17, 0x0, x14, (arg1[1]));
  fiat_pasta_mulx_u64(&x18, &x19, x16, prime[0]);
  fiat_pasta_mulx_u64(&x20, &x21, x18, prime[1]);
  fiat_pasta_mulx_u64(&x22, &x23, x18, prime[2]);
  fiat_pasta_mulx_u64(&x24, &x25, x18, prime[3]);
  fiat_pasta_addcarryx_u64(&x26, &x27, 0x0, x25, x22);
  fiat_pasta_addcarryx_u64(&x28, &x29, 0x0, x16, x24);
  fiat_pasta_addcarryx_u64(&x30, &x31, x29, (x17 + (x15 + (x11 + x7))), x26);
  fiat_pasta_addcarryx_u64(&x32, &x33, x31, x4, (x27 + x23));
  fiat_pasta_addcarryx_u64(&x34, &x35, x33, x5, x20);
  fiat_pasta_addcarryx_u64(&x36, &x37, 0x0, x30, (arg1[2]));
  fiat_pasta_addcarryx_u64(&x38, &x39, x37, x32, 0x0);
  fiat_pasta_addcarryx_u64(&x40, &x41, x39, x34, 0x0);
  fiat_pasta_mulx_u64(&x42, &x43, x36, prime[0]);
  fiat_pasta_mulx_u64(&x44, &x45, x42, prime[1]);
  fiat_pasta_mulx_u64(&x46, &x47, x42, prime[2]);
  fiat_pasta_mulx_u64(&x48, &x49, x42, prime[3]);
  fiat_pasta_addcarryx_u64(&x50, &x51, 0x0, x49, x46);
  fiat_pasta_addcarryx_u64(&x52, &x53, 0x0, x36, x48);
  fiat_pasta_addcarryx_u64(&x54, &x55, x53, x38, x50);
  fiat_pasta_addcarryx_u64(&x56, &x57, x55, x40, (x51 + x47));
  fiat_pasta_addcarryx_u64(&x58, &x59, x57, (x41 + (x35 + x21)), x44);
  fiat_pasta_addcarryx_u64(&x60, &x61, 0x0, x54, (arg1[3]));
  fiat_pasta_addcarryx_u64(&x62, &x63, x61, x56, 0x0);
  fiat_pasta_addcarryx_u64(&x64, &x65, x63, x58, 0x0);
  fiat_pasta_mulx_u64(&x66, &x67, x60, prime[0]);
  fiat_pasta_mulx_u64(&x68, &x69, x66, prime[1]);
  fiat_pasta_mulx_u64(&x70, &x71, x66, prime[2]);
  fiat_pasta_mulx_u64(&x72, &x73, x66, prime[3]);
  fiat_pasta_addcarryx_u64(&x74, &x75, 0x0, x73, x70);
  fiat_pasta_addcarryx_u64(&x76, &x77, 0x0, x60, x72);
  fiat_pasta_addcarryx_u64(&x78, &x79, x77, x62, x74);
  fiat_pasta_addcarryx_u64(&x80, &x81, x79, x64, (x75 + x71));
  fiat_pasta_addcarryx_u64(&x82, &x83, x81, (x65 + (x59 + x45)), x68);
  x84 = (x83 + x69);
  fiat_pasta_subborrowx_u64(&x85, &x86, 0x0, x78, prime[3]);
  fiat_pasta_subborrowx_u64(&x87, &x88, x86, x80, prime[2]);
  fiat_pasta_subborrowx_u64(&x89, &x90, x88, x82, 0x0);
  fiat_pasta_subborrowx_u64(&x91, &x92, x90, x84, prime[1]);
  fiat_pasta_subborrowx_u64(&x93, &x94, x92, 0x0, 0x0);
  fiat_pasta_cmovznz_u64(&out1[0], x94, x85, x78);
  fiat_pasta_cmovznz_u64(&out1[1], x94, x87, x80);
  fiat_pasta_cmovznz_u64(&out1[2], x94, x89, x82);
  fiat_pasta_cmovznz_u64(&out1[3], x94, x91, x84);
}

/*
 * The function fiat_pasta_to_montgomery translates a field element into the Montgomery domain.
 * Preconditions:
 *   0 ≤ eval arg1 < m
 * Postconditions:
 *   eval (from_montgomery out1) mod m = eval arg1 mod m
 *   0 ≤ eval out1 < m
 *
 * Input Bounds:
 *   arg1: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff]]
 * Output Bounds:
 *   out1: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff]]
 */
void fiat_pasta_to_montgomery(uint64_t out1[4], const uint64_t arg1[4], bool fq) {
  const uint64_t *prime, *prime2;
  if (fq) {
    prime = mina_fq;
    prime2 = mina_fq2;
  } else {
    prime = mina_fp;
    prime2 = mina_fp2;
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
  fiat_pasta_uint1 x28;
  uint64_t x29;
  fiat_pasta_uint1 x30;
  uint64_t x31;
  fiat_pasta_uint1 x32;
  uint64_t x33;
  fiat_pasta_uint1 x34;
  uint64_t x35;
  fiat_pasta_uint1 x36;
  uint64_t x37;
  uint64_t x38;
  uint64_t x39;
  uint64_t x40;
  uint64_t x41;
  uint64_t x42;
  uint64_t x43;
  uint64_t x44;
  uint64_t x45;
  fiat_pasta_uint1 x46;
  uint64_t x47;
  fiat_pasta_uint1 x48;
  uint64_t x49;
  fiat_pasta_uint1 x50;
  uint64_t x51;
  fiat_pasta_uint1 x52;
  uint64_t x53;
  fiat_pasta_uint1 x54;
  uint64_t x55;
  fiat_pasta_uint1 x56;
  uint64_t x57;
  fiat_pasta_uint1 x58;
  uint64_t x59;
  uint64_t x60;
  uint64_t x61;
  uint64_t x62;
  uint64_t x63;
  uint64_t x64;
  uint64_t x65;
  uint64_t x66;
  uint64_t x67;
  fiat_pasta_uint1 x68;
  uint64_t x69;
  fiat_pasta_uint1 x70;
  uint64_t x71;
  fiat_pasta_uint1 x72;
  uint64_t x73;
  fiat_pasta_uint1 x74;
  uint64_t x75;
  fiat_pasta_uint1 x76;
  uint64_t x77;
  uint64_t x78;
  uint64_t x79;
  uint64_t x80;
  uint64_t x81;
  uint64_t x82;
  uint64_t x83;
  uint64_t x84;
  uint64_t x85;
  fiat_pasta_uint1 x86;
  uint64_t x87;
  fiat_pasta_uint1 x88;
  uint64_t x89;
  fiat_pasta_uint1 x90;
  uint64_t x91;
  fiat_pasta_uint1 x92;
  uint64_t x93;
  fiat_pasta_uint1 x94;
  uint64_t x95;
  fiat_pasta_uint1 x96;
  uint64_t x97;
  fiat_pasta_uint1 x98;
  uint64_t x99;
  uint64_t x100;
  uint64_t x101;
  uint64_t x102;
  uint64_t x103;
  uint64_t x104;
  uint64_t x105;
  uint64_t x106;
  uint64_t x107;
  fiat_pasta_uint1 x108;
  uint64_t x109;
  fiat_pasta_uint1 x110;
  uint64_t x111;
  fiat_pasta_uint1 x112;
  uint64_t x113;
  fiat_pasta_uint1 x114;
  uint64_t x115;
  fiat_pasta_uint1 x116;
  uint64_t x117;
  uint64_t x118;
  uint64_t x119;
  uint64_t x120;
  uint64_t x121;
  uint64_t x122;
  uint64_t x123;
  uint64_t x124;
  uint64_t x125;
  fiat_pasta_uint1 x126;
  uint64_t x127;
  fiat_pasta_uint1 x128;
  uint64_t x129;
  fiat_pasta_uint1 x130;
  uint64_t x131;
  fiat_pasta_uint1 x132;
  uint64_t x133;
  fiat_pasta_uint1 x134;
  uint64_t x135;
  fiat_pasta_uint1 x136;
  uint64_t x137;
  fiat_pasta_uint1 x138;
  uint64_t x139;
  uint64_t x140;
  uint64_t x141;
  uint64_t x142;
  uint64_t x143;
  uint64_t x144;
  uint64_t x145;
  uint64_t x146;
  uint64_t x147;
  fiat_pasta_uint1 x148;
  uint64_t x149;
  fiat_pasta_uint1 x150;
  uint64_t x151;
  fiat_pasta_uint1 x152;
  uint64_t x153;
  fiat_pasta_uint1 x154;
  uint64_t x155;
  fiat_pasta_uint1 x156;
  uint64_t x157;
  uint64_t x158;
  fiat_pasta_uint1 x159;
  uint64_t x160;
  fiat_pasta_uint1 x161;
  uint64_t x162;
  fiat_pasta_uint1 x163;
  uint64_t x164;
  fiat_pasta_uint1 x165;
  uint64_t x166;
  fiat_pasta_uint1 x167;
  x1 = (arg1[1]);
  x2 = (arg1[2]);
  x3 = (arg1[3]);
  x4 = (arg1[0]);
  fiat_pasta_mulx_u64(&x5, &x6, x4, prime2[0]);
  fiat_pasta_mulx_u64(&x7, &x8, x4, prime2[1]);
  fiat_pasta_mulx_u64(&x9, &x10, x4, prime2[2]);
  fiat_pasta_mulx_u64(&x11, &x12, x4, prime2[3]);
  fiat_pasta_addcarryx_u64(&x13, &x14, 0x0, x12, x9);
  fiat_pasta_addcarryx_u64(&x15, &x16, x14, x10, x7);
  fiat_pasta_addcarryx_u64(&x17, &x18, x16, x8, x5);
  fiat_pasta_mulx_u64(&x19, &x20, x11, prime[0]);
  fiat_pasta_mulx_u64(&x21, &x22, x19, prime[1]);
  fiat_pasta_mulx_u64(&x23, &x24, x19, prime[2]);
  fiat_pasta_mulx_u64(&x25, &x26, x19, prime[3]);
  fiat_pasta_addcarryx_u64(&x27, &x28, 0x0, x26, x23);
  fiat_pasta_addcarryx_u64(&x29, &x30, 0x0, x11, x25);
  fiat_pasta_addcarryx_u64(&x31, &x32, x30, x13, x27);
  fiat_pasta_addcarryx_u64(&x33, &x34, x32, x15, (x28 + x24));
  fiat_pasta_addcarryx_u64(&x35, &x36, x34, x17, x21);
  fiat_pasta_mulx_u64(&x37, &x38, x1, prime2[0]);
  fiat_pasta_mulx_u64(&x39, &x40, x1, prime2[1]);
  fiat_pasta_mulx_u64(&x41, &x42, x1, prime2[2]);
  fiat_pasta_mulx_u64(&x43, &x44, x1, prime2[3]);
  fiat_pasta_addcarryx_u64(&x45, &x46, 0x0, x44, x41);
  fiat_pasta_addcarryx_u64(&x47, &x48, x46, x42, x39);
  fiat_pasta_addcarryx_u64(&x49, &x50, x48, x40, x37);
  fiat_pasta_addcarryx_u64(&x51, &x52, 0x0, x31, x43);
  fiat_pasta_addcarryx_u64(&x53, &x54, x52, x33, x45);
  fiat_pasta_addcarryx_u64(&x55, &x56, x54, x35, x47);
  fiat_pasta_addcarryx_u64(&x57, &x58, x56, ((x36 + (x18 + x6)) + x22), x49);
  fiat_pasta_mulx_u64(&x59, &x60, x51, prime[0]);
  fiat_pasta_mulx_u64(&x61, &x62, x59, prime[1]);
  fiat_pasta_mulx_u64(&x63, &x64, x59, prime[2]);
  fiat_pasta_mulx_u64(&x65, &x66, x59, prime[3]);
  fiat_pasta_addcarryx_u64(&x67, &x68, 0x0, x66, x63);
  fiat_pasta_addcarryx_u64(&x69, &x70, 0x0, x51, x65);
  fiat_pasta_addcarryx_u64(&x71, &x72, x70, x53, x67);
  fiat_pasta_addcarryx_u64(&x73, &x74, x72, x55, (x68 + x64));
  fiat_pasta_addcarryx_u64(&x75, &x76, x74, x57, x61);
  fiat_pasta_mulx_u64(&x77, &x78, x2, prime2[0]);
  fiat_pasta_mulx_u64(&x79, &x80, x2, prime2[1]);
  fiat_pasta_mulx_u64(&x81, &x82, x2, prime2[2]);
  fiat_pasta_mulx_u64(&x83, &x84, x2, prime2[3]);
  fiat_pasta_addcarryx_u64(&x85, &x86, 0x0, x84, x81);
  fiat_pasta_addcarryx_u64(&x87, &x88, x86, x82, x79);
  fiat_pasta_addcarryx_u64(&x89, &x90, x88, x80, x77);
  fiat_pasta_addcarryx_u64(&x91, &x92, 0x0, x71, x83);
  fiat_pasta_addcarryx_u64(&x93, &x94, x92, x73, x85);
  fiat_pasta_addcarryx_u64(&x95, &x96, x94, x75, x87);
  fiat_pasta_addcarryx_u64(&x97, &x98, x96, ((x76 + (x58 + (x50 + x38))) + x62), x89);
  fiat_pasta_mulx_u64(&x99, &x100, x91, prime[0]);
  fiat_pasta_mulx_u64(&x101, &x102, x99, prime[1]);
  fiat_pasta_mulx_u64(&x103, &x104, x99, prime[2]);
  fiat_pasta_mulx_u64(&x105, &x106, x99, prime[3]);
  fiat_pasta_addcarryx_u64(&x107, &x108, 0x0, x106, x103);
  fiat_pasta_addcarryx_u64(&x109, &x110, 0x0, x91, x105);
  fiat_pasta_addcarryx_u64(&x111, &x112, x110, x93, x107);
  fiat_pasta_addcarryx_u64(&x113, &x114, x112, x95, (x108 + x104));
  fiat_pasta_addcarryx_u64(&x115, &x116, x114, x97, x101);
  fiat_pasta_mulx_u64(&x117, &x118, x3, prime2[0]);
  fiat_pasta_mulx_u64(&x119, &x120, x3, prime2[1]);
  fiat_pasta_mulx_u64(&x121, &x122, x3, prime2[2]);
  fiat_pasta_mulx_u64(&x123, &x124, x3, prime2[3]);
  fiat_pasta_addcarryx_u64(&x125, &x126, 0x0, x124, x121);
  fiat_pasta_addcarryx_u64(&x127, &x128, x126, x122, x119);
  fiat_pasta_addcarryx_u64(&x129, &x130, x128, x120, x117);
  fiat_pasta_addcarryx_u64(&x131, &x132, 0x0, x111, x123);
  fiat_pasta_addcarryx_u64(&x133, &x134, x132, x113, x125);
  fiat_pasta_addcarryx_u64(&x135, &x136, x134, x115, x127);
  fiat_pasta_addcarryx_u64(&x137, &x138, x136, ((x116 + (x98 + (x90 + x78))) + x102), x129);
  fiat_pasta_mulx_u64(&x139, &x140, x131, prime[0]);
  fiat_pasta_mulx_u64(&x141, &x142, x139, prime[1]);
  fiat_pasta_mulx_u64(&x143, &x144, x139, prime[2]);
  fiat_pasta_mulx_u64(&x145, &x146, x139, prime[3]);
  fiat_pasta_addcarryx_u64(&x147, &x148, 0x0, x146, x143);
  fiat_pasta_addcarryx_u64(&x149, &x150, 0x0, x131, x145);
  fiat_pasta_addcarryx_u64(&x151, &x152, x150, x133, x147);
  fiat_pasta_addcarryx_u64(&x153, &x154, x152, x135, (x148 + x144));
  fiat_pasta_addcarryx_u64(&x155, &x156, x154, x137, x141);
  x157 = ((x156 + (x138 + (x130 + x118))) + x142);
  fiat_pasta_subborrowx_u64(&x158, &x159, 0x0, x151, prime[3]);
  fiat_pasta_subborrowx_u64(&x160, &x161, x159, x153, prime[2]);
  fiat_pasta_subborrowx_u64(&x162, &x163, x161, x155, 0x0);
  fiat_pasta_subborrowx_u64(&x164, &x165, x163, x157, prime[1]);
  fiat_pasta_subborrowx_u64(&x166, &x167, x165, 0x0, 0x0);
  fiat_pasta_cmovznz_u64(&out1[0], x167, x158, x151);
  fiat_pasta_cmovznz_u64(&out1[1], x167, x160, x153);
  fiat_pasta_cmovznz_u64(&out1[2], x167, x162, x155);
  fiat_pasta_cmovznz_u64(&out1[3], x167, x164, x157);
}

/*
 * The function fiat_pasta_opp negates a field element in the Montgomery domain.
 * Preconditions:
 *   0 ≤ eval arg1 < m
 * Postconditions:
 *   eval (from_montgomery out1) mod m = -eval (from_montgomery arg1) mod m
 *   0 ≤ eval out1 < m
 *
 * Input Bounds:
 *   arg1: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff]]
 * Output Bounds:
 *   out1: [[0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff], [0x0 ~> 0xffffffffffffffff]]
 */
void fiat_pasta_opp(uint64_t out1[4], const uint64_t arg1[4], bool fq) {
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
  fiat_pasta_uint1 x11;
  fiat_pasta_uint1 x13;
  fiat_pasta_uint1 x15;
  fiat_pasta_uint1 x17;
  fiat_pasta_subborrowx_u64(&x1, &x2, 0x0, 0x0, (arg1[0]));
  fiat_pasta_subborrowx_u64(&x3, &x4, x2, 0x0, (arg1[1]));
  fiat_pasta_subborrowx_u64(&x5, &x6, x4, 0x0, (arg1[2]));
  fiat_pasta_subborrowx_u64(&x7, &x8, x6, 0x0, (arg1[3]));
  fiat_pasta_cmovznz_u64(&x9, x8, 0x0, mina_ff);
  fiat_pasta_addcarryx_u64(&out1[0], &x11, 0x0, x1, (x9 & prime[3]));
  fiat_pasta_addcarryx_u64(&out1[1], &x13, x11, x3, (x9 & prime[2]));
  fiat_pasta_addcarryx_u64(&out1[2], &x15, x13, x5, 0x0);
  fiat_pasta_addcarryx_u64(&out1[3], &x17, x15, x7, (x9 & prime[1]));
}

