#include <stdint.h>
#include "Curve25519KEM.h"
#include "print.h"
#include "cpucycles.h"
#include "avr.h"
#include "RNG.h"
#include "DH_KEM_Curve25519_test_values_for_bench.h"

#define NRUNS 15

int main(void)
{   print("Curve25519_KEM enc (with RNG): \n");
    uint8_t pk[32];
    uint8_t ct[32];
    uint8_t key[32];
    RNG.begin("Test");
    for (int j = 0; j < NRUNS; j++)
    {
    unsigned long long begin=0,end=0;
    memcpy_P(pk,(uint8_t*)pgm_read_word(&(indexes[j*3])),32);
    begin= cpucycles();
    Curve25519KEM::encap(key,ct,key,0);
    end= cpucycles();
    printllu(end-begin);
    print(",");
    }
    print("\n");
    avr_end();
    return 0;
}