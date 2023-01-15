#include <stdint.h>
#include <avr/pgmspace.h>
#include "crystalskyber.h"
#include "print.h"
#include "cpucycles.h"
#include "avr.h"
#include "RNG.h"
#include "test_vals_for_dec.h"
#define NRUNS 30

int main(void)
{ 
  #if !(KYBER_K==3 && SECRET_KEY_VARIANT==3)
  print("K: ");
  printllu(KYBER_K);print("\t");
  print(" SK-Variant: ");
  printllu(SECRET_KEY_VARIANT);print("\t");
  print(" RNG: ");
  printllu(Use_Deterministic_RNG);print("\n");
  #endif
  uint8_t key[CRYPTO_BYTES];
  uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
  uint8_t sk[CRYPTO_SECRETKEYBYTES];
  RNG.begin("Test");
  unsigned long long begin;
  #if !(KYBER_K==3 && SECRET_KEY_VARIANT==3)
  for (int i = 0; i < NRUNS; i+=2){ 
    memcpy_P(sk,(uint8_t*)pgm_read_word(&(values[i])),CRYPTO_SECRETKEYBYTES);
    memcpy_P(ct,(uint8_t*)pgm_read_word(&(values[i+1])),CRYPTO_CIPHERTEXTBYTES);
    begin = cpucycles();
    CrystalsKyber::crypto_kem_dec(key,ct,sk);
    begin=cpucycles()-begin;
    printllu(begin);
    print(",");
  }
  #else
  memcpy_P(sk,(uint8_t*)pgm_read_word(&(values[0])),CRYPTO_SECRETKEYBYTES);
    memcpy_P(ct,(uint8_t*)pgm_read_word(&(values[0+1])),CRYPTO_CIPHERTEXTBYTES);
    begin = cpucycles();
    CrystalsKyber::crypto_kem_dec(key,ct,sk);
    begin=cpucycles()-begin;
    printllu(begin);
  #endif
  print("\n");
  avr_end();
  return 0;
}