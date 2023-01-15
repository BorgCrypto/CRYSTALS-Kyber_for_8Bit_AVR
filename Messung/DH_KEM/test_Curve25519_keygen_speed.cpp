#include <stdint.h>
#include "Curve25519KEM.h"
#include "print.h"
#include "cpucycles.h"
#include "avr.h"
#include "RNG.h"
#define NRUNS 15

int main(void)
{ 
  print("Curve25519_KEM keygen (with RNG): \n");
  uint8_t sk[32];
  uint8_t pk[32];
  unsigned int i;
  unsigned long long begin,end;
  RNG.begin("Test");
  for(i=0;i<NRUNS;i++)
  {
    begin = cpucycles();
    Curve25519KEM::keyGen(pk,sk);
    end=cpucycles();
    printllu(end-begin);
    print(",");
  }
  print("\n");
  avr_end();
  return 0;
}