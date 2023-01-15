/**
 * Taken from the official Reference Implementation of CRYSTALS-KYBER.
 * See: https://github.com/pq-crystals/kyber/tree/round3/ref
*/
#ifndef RANDOMBYTES_H
#define RANDOMBYTES_H

#include <stddef.h>
#include <stdint.h>

void randombytes(uint8_t *out, size_t outlen);

#endif
