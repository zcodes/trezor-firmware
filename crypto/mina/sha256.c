/*********************************************************************
* Filename:   sha256.c
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Implementation of the SHA-256 hashing algorithm.
              SHA-256 is one of the three algorithms in the SHA2
              specification. The others, SHA-384 and SHA-512, are not
              offered in this implementation.
              Algorithm specification can be found here:
               * http://csrc.nist.gov/publications/fips/fips180-2/fips180-2withchangenotice.pdf
              This implementation uses little endian byte order.
*********************************************************************/

/*************************** HEADER FILES ***************************/
#include "sha256.h"

void sha256_hash(const void *in, const size_t in_len, void *out, size_t out_len)
{
	if (out_len != 32) {
		return;
	}

	SHA256_CTX sha256_ctx;
	sha256_Init(&sha256_ctx);
	sha256_Update(&sha256_ctx, (const uint8_t *)in, in_len);
	sha256_Final(&sha256_ctx, (uint8_t *)out);
}
