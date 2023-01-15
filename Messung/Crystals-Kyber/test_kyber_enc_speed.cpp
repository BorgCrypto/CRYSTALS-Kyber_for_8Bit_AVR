#include <stdint.h>
#include "crystalskyber.h"
#include "print.h"
#include "cpucycles.h"
#include "avr.h"
#include "RNG.h"
#include "test_values_for_enc_bench.h"

#define NRUNS 20


int main(void)
{   RNG.begin("Test");
    print("K: ");printllu(KYBER_K);print("\t");
    print("SK-Variant: ");printllu(SECRET_KEY_VARIANT);print("\t");
    print("RNG: ");printllu(Use_Deterministic_RNG);print("\n");
    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
    uint8_t key[CRYPTO_BYTES];
    for (int j = 0; j < NRUNS; j++)
    {
    unsigned long long begin=0,end=0;
    memcpy_P(pk,(uint8_t*)pgm_read_word(&(values[j])),CRYPTO_PUBLICKEYBYTES);
    begin= cpucycles();
    CrystalsKyber::crypto_kem_enc(ct,key,pk);
    end= cpucycles();
    printllu(end-begin);
    print(",");
    }
    print("\n");
    avr_end();
    return 0;
}