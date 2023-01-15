#include <stdint.h>
#include "Curve25519KEM.h"
#include "print.h"
#include "cpucycles.h"
#include "avr.h"
#include "RNG.h"
#include "DH_KEM_Curve25519_test_values_for_bench.h"
#define NRUNS 15

int main(void)
{ print("Curve25519_KEM dec (with RNG): \n");
    uint8_t sk[32];
    uint8_t ct[32];
    uint8_t key[32];
    RNG.begin("Test");
  unsigned long long begin,end;
  for (int i = 0; i < NRUNS; i++){ 
    memcpy_P(sk,(uint8_t*)pgm_read_word(&(indexes[i*3+1])),32);
    memcpy_P(ct,(uint8_t*)pgm_read_word(&(indexes[i*3+2])),32);
    begin = cpucycles();
    Curve25519KEM::decap(key,ct,sk);
    end=cpucycles();
    printllu(end-begin);
    print(",");
  }
  print("\n");
  avr_end();
  return 0;
}