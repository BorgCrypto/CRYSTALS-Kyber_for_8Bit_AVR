#include <stdint.h>
#include "NewHope.h"
#include "print.h"
#include "cpucycles.h"
#include "avr.h"
#include "RNG.h"
#include "KEM_NewHope_test_values_for_dec_bench.h"
#define NRUNS 30

int main(void)
{ print("NewHope_KEM dec (with RNG): \n");
    NewHopePrivateKey sk;
    uint8_t ct[2048];
    uint8_t key[32];
    RNG.begin("Test");
  unsigned long long begin,end;
  for (int i = 0; i < NRUNS; i+=2){ 
    memcpy_P(sk.seed,(uint8_t*)pgm_read_word(&(indexes[i])),32);
    memcpy_P(ct,(uint8_t*)pgm_read_word(&(indexes[i+1])),2048);
    begin = cpucycles();
    NewHope::shareda(key,sk,ct);
    end=cpucycles();
    printllu(end-begin);
    print(",");
  }
  print("\n");
  avr_end();
  return 0;
}