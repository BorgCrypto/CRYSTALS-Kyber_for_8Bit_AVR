#include <stdint.h>
#include "P521KEM.h"
#include "print.h"
#include "cpucycles.h"
#include "avr.h"
#include "RNG.h"
#include "DH_KEM_P521_test_values_for_bench.h"
#define NRUNS 15

int main(void)
{ print("P521_KEM dec (with RNG): \n");
    uint8_t sk[66];
    uint8_t ct[133];
    uint8_t key[64];
    RNG.begin("Test");
  unsigned long long begin,end;
  for (int i = 0; i < NRUNS; i++){ 
    memcpy_P(sk,(uint8_t*)pgm_read_word(&(indexes[i*3+1])),66);
    printllu(sk[0]);print("\n");
    memcpy_P(ct,(uint8_t*)pgm_read_word(&(indexes[i*3+2])),133);
    printllu(ct[0]);print("\n");
    begin = cpucycles();
    P521KEM::decap(key,ct,sk);
    end=cpucycles();
    printllu(end-begin);
    print(",");
  }
  print("\n");
  avr_end();
  return 0;
}