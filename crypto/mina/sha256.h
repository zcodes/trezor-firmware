/*********************************************************************
* Filename:   sha256.h
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Defines the API for the corresponding SHA1 implementation.
*********************************************************************/

#ifndef MINA_SHA256_H_
#define MINA_SHA256_H_

/*************************** HEADER FILES ***************************/
#include "../sha2.h"

/****************************** MACROS ******************************/
#define SHA256_BLOCK_SIZE 32            // SHA256 outputs a 32 byte digest
void sha256_hash(const void *in, const size_t in_len, void *out, size_t out_len);

#endif   // SHA256_H
