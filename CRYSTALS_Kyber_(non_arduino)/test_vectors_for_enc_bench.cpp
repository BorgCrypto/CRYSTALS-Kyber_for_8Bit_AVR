/* Deterministic randombytes by Daniel J. Bernstein */
/* taken from SUPERCOP (https://bench.cr.yp.to)     */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "api.h"
#include <stdio.h>
#include <string.h>

#define NTESTS 20

int main(void)
{ 
    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t sk[CRYPTO_SECRETKEYBYTES];
    int i,j;

    printf("#if KYBER_K==%d \n",KYBER_K);
    printf("#include <crystalskyber.h> \n #include <avr/pgmspace.h> \n");

    for(i=0;i<NTESTS;i++) {
        // Key-pair generation

        crypto_kem_keypair(pk, sk); 
    printf("const PROGMEM uint8_t pk_values_%d[%d]={",i,CRYPTO_PUBLICKEYBYTES);       
        for(j=0;j<CRYPTO_PUBLICKEYBYTES;j++){
            if(j==(CRYPTO_PUBLICKEYBYTES-1)){
               printf("0x%02x",pk[j]); 
            }else{
               printf("0x%02x,",pk[j]); 
            }
        } 
    printf("};\n");
  }
  printf("const uint8_t* const values[%d]PROGMEM={",NTESTS);
  for ( i = 0; i < NTESTS; i++)
  {
   if(i==NTESTS-1){
         printf("pk_values_%d",i);
      }else{
         printf("pk_values_%d,",i);
      }
  }
    printf("};\n");
        printf("#endif\n"); 
  return 0;
}
