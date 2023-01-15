#include <stdint.h>
#include "NewHope.h"
#include "print.h"
#include "cpucycles.h"
#include "avr.h"
#include "RNG.h"
#define NRUNS 15

int main(void)
{ 
  print("NewHope_KEM keygen (with RNG): \n");
  NewHopePrivateKey sk;
  uint8_t pk[1824];
  unsigned int i;
  unsigned long long begin,end;
  RNG.begin("Test");
  for(i=0;i<NRUNS;i++)
  {
    begin = cpucycles();
    NewHope::keygen(pk,sk);
    end=cpucycles();
    printllu(end-begin);
    print(",");
  }
  print("\n");
  avr_end();
  return 0;
}