#include <stdint.h>
#include "P521KEM.h"
#include "print.h"
#include "cpucycles.h"
#include "avr.h"
#include "RNG.h"
#define NRUNS 15

int main(void)
{ 
  print("P521_KEM keygen (with RNG): \n");
  uint8_t sk[66];
  uint8_t pk[132];
  unsigned int i;
  unsigned long long begin,end;
  RNG.begin("Test");
  for(i=0;i<NRUNS;i++)
  {
    begin = cpucycles();
    P521KEM::keyGen(pk,sk);
    end=cpucycles();
    print("\n");
    printllu(end-begin);
    print("\n");
  }
  avr_end();
  return 0;
}