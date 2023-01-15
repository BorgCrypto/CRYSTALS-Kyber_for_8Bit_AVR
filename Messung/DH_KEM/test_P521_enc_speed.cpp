#include <stdint.h>
#include "P521KEM.h"
#include "print.h"
#include "cpucycles.h"
#include "avr.h"
#include "RNG.h"
#include "DH_KEM_P521_test_values_for_bench.h"

#define NRUNS 15


int main(void)
{   print("P521_KEM enc (with RNG): \n");
    uint8_t pk[132];
    uint8_t ct[133];
    uint8_t key[64];
    RNG.begin("Test");
    for (int j = 0; j < NRUNS; j++)
    {
    unsigned long long begin=0,end=0;
    memcpy_P(pk,(uint8_t*)pgm_read_word(&(indexes[j*3])),132);
    begin= cpucycles();
    P521KEM::encap(key,ct,key,0);
    end= cpucycles();
    printllu(end-begin);
    print("\n");
    }
    avr_end();
    return 0;
}