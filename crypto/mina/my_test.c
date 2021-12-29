#include <stdio.h>
#include "crypto.h"

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

int main(int argc, char *argv[])
{
    Keypair key = {0};
    generate_keypair(&key, 1);
    dump_keypair(&key);
    char address[MINA_ADDRESS_LEN + 1] = {0};
    generate_address(address, MINA_ADDRESS_LEN, &(key.pub));
    printf("%s\n", address);

    Affine pub_key;
    generate_pubkey(&pub_key, key.priv);

    generate_address(address, MINA_ADDRESS_LEN, &pub_key);
    printf("%s\n", address);

    dump_keypair(&key);

    return 0;
}
