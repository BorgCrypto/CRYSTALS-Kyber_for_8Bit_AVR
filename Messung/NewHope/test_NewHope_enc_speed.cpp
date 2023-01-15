#include <stdint.h>
#include "NewHope.h"
#include "print.h"
#include "cpucycles.h"
#include "avr.h"
#include "RNG.h"
#include "KEM_NewHope_test_values_for_enc_bench.h"

#define NRUNS 15


int main(void)
{   print("NewHope_KEM enc (with RNG): \n");
    uint8_t ct[2048];
    uint8_t key[32];
    RNG.begin("Test");
    for (int j = 0; j < NRUNS; j++)
    {
    unsigned long long begin=0,end=0;
    memcpy_P(ct,(uint8_t*)pgm_read_word(&(indexes[j])),1824);
    begin= cpucycles();
    NewHope::sharedb(key,ct,ct);
    end= cpucycles();
    printllu(end-begin);
    print(",");
    }
    print("\n");
    avr_end();
    return 0;
}