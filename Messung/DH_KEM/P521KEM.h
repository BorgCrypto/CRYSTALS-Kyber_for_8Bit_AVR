#ifndef CRYPTO_P521KEM__h
#define CRYPTO_P521EM__h

#include <stdint.h>

class P521KEM
{
public:

static void keyGen(uint8_t pk[132],uint8_t sk[66]);
static void encap(uint8_t shared_secret[64], uint8_t enc[133], const uint8_t publicKey[133],uint8_t test_eph_sk[66]);
static void decap(uint8_t shared_secret[64], uint8_t enc_local[133], const uint8_t secretKey[66]);


private:
static void  extractAndExpand(uint8_t *output, uint8_t *dh, uint8_t *kem_context);
 
    // Constructor and destructor are private - cannot instantiate this class.
    P521KEM() {}
    ~P521KEM() {}
};

#endif
