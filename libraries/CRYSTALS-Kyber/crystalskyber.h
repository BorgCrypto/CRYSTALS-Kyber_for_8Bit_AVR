#ifndef CRYSTALS_KYBER_H
#define CRYSTALS_KYBER_H
#include "params.h"
#define CRYPTO_PUBLICKEYBYTES  KYBER_PUBLICKEYBYTES
#define CRYPTO_CIPHERTEXTBYTES KYBER_CIPHERTEXTBYTES
#define CRYPTO_BYTES           KYBER_SSBYTES
#define CRYPTO_SECRETKEYBYTES  KYBER_SECRETKEYBYTES

class CrystalsKyber
{
private:
    CrystalsKyber(){}
    ~CrystalsKyber(){}

public:

 static int CrystalsKyber::crypto_kem_keypair(unsigned char *pk, unsigned char *sk);

 static int CrystalsKyber::crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);

 static int CrystalsKyber::crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

};

#endif
