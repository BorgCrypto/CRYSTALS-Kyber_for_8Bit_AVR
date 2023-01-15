#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "api.h"
#include "randombytes.h"
#include <stdio.h>
#include <string.h>

#define NTESTS 15

int main(void)
{ 
    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t sk[CRYPTO_SECRETKEYBYTES];
    uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
    uint8_t key_b[CRYPTO_BYTES];
    int i,j;
    printf("#if (KYBER_K==%d && SECRET_KEY_VARIANT==%d) \n",KYBER_K,SECRET_KEY_VARIANT);
    printf("#include <crystalskyber.h> \n #include <avr/pgmspace.h> \n");
    for(i=0;i<NTESTS;i++) {
        crypto_kem_keypair(pk, sk);
        printf("const PROGMEM uint8_t sk_values_%i[CRYPTO_SECRETKEYBYTES]={",i);
        for(j=0;j<CRYPTO_SECRETKEYBYTES;j++){
            if(j==CRYPTO_SECRETKEYBYTES-1){
               printf("0x%02x",sk[j]); 
            }else{
               printf("0x%02x,",sk[j]); 
            }
        }
            
        printf("};\n");

        // Encapsulation
        crypto_kem_enc(ct, key_b, pk);
        printf("const PROGMEM uint8_t ct_values_%i[CRYPTO_CIPHERTEXTBYTES]={",i);
        for(j=0;j<CRYPTO_CIPHERTEXTBYTES;j++){
            if(j==CRYPTO_CIPHERTEXTBYTES-1){
               printf("0x%02x",ct[j]); 
            }else{
               printf("0x%02x,",ct[j]); 
            }
            
        }
            
        printf("};\n");
           
  }
  printf("const uint8_t* const values[%d]PROGMEM={",2*NTESTS);
  for ( i = 0; i < NTESTS; i++)
  {
   if(i==NTESTS-1){
         printf("sk_values_%d,",i);printf("ct_values_%d",i); 
      }else{
         printf("sk_values_%d,",i);printf("ct_values_%d,",i);
      }
  }
  printf("};\n");
  printf("#endif\n");
  return 0;
}
