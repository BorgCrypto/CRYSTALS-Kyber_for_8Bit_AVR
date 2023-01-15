#include <stdint.h>
#include "crystalskyber.h"
#include "print.h"
#include "cpucycles.h"
#include "avr.h"
#include "RNG.h"
#define NRUNS 15

int main(void)
{ 
  print("K: ");printllu(KYBER_K);print("\t");
  print("SK-Variant: ");printllu(SECRET_KEY_VARIANT);print("\t");
  print("RNG: ");printllu(Use_Deterministic_RNG);print("\n");
  uint8_t sk[CRYPTO_SECRETKEYBYTES];
  uint8_t pk[CRYPTO_PUBLICKEYBYTES];
  unsigned int i;
  unsigned long long begin,end;
  RNG.begin("Test");
  for(i=0;i<NRUNS;i++)
  {
    begin = cpucycles();
    CrystalsKyber::crypto_kem_keypair(pk,sk);
    end=cpucycles();
    printllu(end-begin);
    print(",");
  }
  print("\n");
  avr_end();
  return 0;
}