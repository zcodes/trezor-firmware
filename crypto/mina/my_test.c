#include <stdio.h>
#include "pasta.h"
#include "bignum.h"
#include "crypto.h"
#include "curve_checks.h"

const uint64_t x[4] = {
    0xbd3c7224c5038313,
    0x9786eb7e009341b7,
    0x343c69d2d1e29e88,
    0x324ebaa7006d73f9
};

const uint64_t y[4] = {
    0xac28e895d9e4ae14,
    0x98444f7bb83580cc,
    0x6e937b98cb4cf9b7,
    0x3d6d348d771d862a
};

/* const uint64_t m[4] = { */
/*     0x4000000000000000, */
/*     0x0000000000000000, */
/*     0x224698fc094cf91b, */
/*     0x992d30ed00000001, */
/* }; */

const uint64_t m[4] = {
    /* 0x0000000000000040, */
    /* 0x0000000000000000, */
    /* 0x1bf94c09fc984622, */
    /* 0x01000000ed302d99 */

    0x992d30ed00000001,
    0x224698fc094cf91b,
    0x0000000000000000,
    0x4000000000000000,
};

const uint8_t m8[32] = {
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x22, 0x46, 0x98, 0xfc, 0x09, 0x4c, 0xf9, 0x1b,
    0x99, 0x2d, 0x30, 0xed, 0x00, 0x00, 0x00, 0x01,
};

const uint64_t m2[4] = {
    0x4000000000000000,
    0x0000000000000000,
    0x224698fc0994a8dd,
    0x8c46eb2100000001,
};

void dump_keypair(Keypair *key)
{
    printf("private key: \n");
    for (int i =0; i < LIMBS_PER_FIELD; i++) {
        printf("0x%016lx ", key->priv[i]);
    }

    printf("\n");
    printf("public key x:\n");
    for (int i = 0; i < LIMBS_PER_FIELD; i++) {
        printf("0x%016lx ", key->pub.x[i]);
    }

    printf("\n");
    printf("public key x:\n");
    for (int i = 0; i < LIMBS_PER_FIELD; i++) {
        printf("0x%016lx ", key->pub.y[i]);
    }
    printf("\n");
}

void dump(const uint64_t out[4])
{
    for (int i = 0; i < 4; i++) {
        printf("0x%016lx ", out[i]);
    }
    printf("\n");
}

void dump8(const uint8_t out[32])
{
    for (int i = 0; i < 32; i++) {
        printf("%02x", out[i]);
    }
    printf("\n");
}

void dump32(const uint32_t out[8])
{
    for (int i = 0; i < 8; i++) {
        printf("0x%08x ", out[i]);
    }
    printf("\n");
}

void my_add(uint64_t out[4], const uint64_t x[4], const uint64_t y[4], bool fq)
{
    uint8_t tmp[32] = {0};
    bignum256 bx, by, bm;
    fiat_pasta_to_bytes(tmp, x);
    bn_read_le(tmp, &bx);
    fiat_pasta_to_bytes(tmp, y);
    bn_read_le(tmp, &by);
    fiat_pasta_to_bytes(tmp, m);
    bn_read_le(tmp, &bm);
    bn_add(&bx, &by);
    bn_mod(&bx, &bm);
    bn_write_le(&bx, tmp);
    fiat_pasta_from_bytes(out, tmp);
}

void my_sub(uint64_t out[4], const uint64_t x[4], const uint64_t y[4], bool fq)
{
    uint8_t tmp[32] = {0};
    bignum256 bx, by, bm, bres;
    fiat_pasta_to_bytes(tmp, x);
    bn_read_le(tmp, &bx);
    fiat_pasta_to_bytes(tmp, y);
    bn_read_le(tmp, &by);
    fiat_pasta_to_bytes(tmp, m);
    bn_read_le(tmp, &bm);
    bn_add(&bx, &bm);
    bn_subtract(&bx, &by, &bres);
    bn_mod(&bres, &bm);
    bn_write_le(&bres, tmp);
    fiat_pasta_from_bytes(out, tmp);
}

int main(int argc, char *argv[])
{
    /* dump(m); */
    /* dump8((uint8_t *)m); */
    /* dump32((uint32_t *)m); */
    uint64_t tmp[4] = {0};
    fiat_pasta_from_bytes(tmp, m8);
    uint8_t tmp2[32] = {0};
    fiat_pasta_to_bytes(tmp2, tmp);
    dump8(tmp2);
    bignum256 bn;
    bn_read_le(m8, &bn);
    bn_write_le(&bn, tmp2);
    dump8(tmp2);
    printf("----\n");
    uint64_t out[4] = {0};
    uint64_t out2[4] = {0};
    dump(out);
    dump(out2);

    fiat_pasta_to_bytes(tmp2, y);
    dump8(tmp2);
    dump(y);
    printf("xxxxxxxxxxxxxxxxxxxxx\n");
    fiat_pasta_add(out, y, y, false);
    dump(out);
    fiat_pasta_to_bytes(tmp2, out);
    dump8(tmp2);
    /* dump8((uint8_t *)out); */

    my_add(out2, y, y, false);
    dump(out2);
    fiat_pasta_to_bytes(tmp2, out2);
    dump8(tmp2);
    /* dump8((uint8_t *)out2); */

    for (int i = 0; i < 5; i++) {
        dump(A[i]->x);
        /* fiat_pasta_from_montgomery(out, A[i]->x, false); */
        /* dump(out); */
        /* printf("++++++++++++++++++++++++++\n"); */
        dump(A[i]->y);
        /* fiat_pasta_from_montgomery(out, A[i]->y, false); */
        /* dump(out); */
        printf("=====================\n");
        fiat_pasta_sub(out, A[i]->x, A[i]->y, false);
        dump(out);

        my_sub(out2, A[i]->x, A[i]->y, false);
        dump(out2);
        printf("\n----------------\n");
    }


    /* Keypair key = {0}; */
    /* generate_keypair(&key, 1); */
    /* dump_keypair(&key); */
    /* char address[MINA_ADDRESS_LEN + 1] = {0}; */
    /* generate_address(address, MINA_ADDRESS_LEN, &(key.pub)); */
    /* printf("%s\n", address); */

    /* Affine pub_key; */
    /* generate_pubkey(&pub_key, key.priv); */

    /* generate_address(address, MINA_ADDRESS_LEN, &pub_key); */
    /* printf("%s\n", address); */

    /* dump_keypair(&key); */

    return 0;
}
