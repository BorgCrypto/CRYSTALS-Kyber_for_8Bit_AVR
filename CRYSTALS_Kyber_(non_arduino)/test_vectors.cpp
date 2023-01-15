/**
 * Taken from the official Reference Implementation and edited.
 * See: https://github.com/pq-crystals/kyber/tree/round3/ref
*/
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "api.h"
#include "randombytes.h"
#include <stdio.h>
#include <string.h>

#define NTESTS 5

int main(void)
{ 
    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t sk[CRYPTO_SECRETKEYBYTES];
    uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
    uint8_t key_a[CRYPTO_BYTES];
    uint8_t key_b[CRYPTO_BYTES];
    int i,j;
    for(i=0;i<NTESTS;i++) {
        // Key-pair generation
        crypto_kem_keypair(pk, sk);
        printf("Public Key: {");
        for(j=0;j<CRYPTO_PUBLICKEYBYTES;j++){
            if(j==CRYPTO_PUBLICKEYBYTES-1){
               printf("0x%02x",pk[j]); 
            }else{
               printf("0x%02x,",pk[j]); 
            }
        }
            
        printf("} \n");
        printf("Secret Key: {");
        for(j=0;j<CRYPTO_SECRETKEYBYTES;j++){
            if(j==CRYPTO_SECRETKEYBYTES-1){
               printf("0x%02x",sk[j]); 
            }else{
               printf("0x%02x,",sk[j]); 
            }
        }
            
        printf("}\n");

        // Encapsulation
        crypto_kem_enc(ct, key_b, pk);
        printf("Ciphertext: {");
        for(j=0;j<CRYPTO_CIPHERTEXTBYTES;j++){
            if(j==CRYPTO_CIPHERTEXTBYTES-1){
               printf("0x%02x",ct[j]); 
            }else{
               printf("0x%02x,",ct[j]); 
            }
            
        }
            
        printf("}\n");
        printf("Shared Secret B: {");
        for(j=0;j<CRYPTO_BYTES;j++){
            if(j==CRYPTO_BYTES-1){
                printf("0x%02x",key_b[j]);
            }else{
                printf("0x%02x,",key_b[j]);
            }
            
        }
        printf("}\n");

        // Decapsulation
        crypto_kem_dec(key_a, ct, sk);
        printf("Shared Secret A: {");
        for(j=0;j<CRYPTO_BYTES;j++){
            if(j==CRYPTO_BYTES-1){
               printf("0x%02x",key_a[j]); 
            }else{
                printf("0x%02x,",key_a[j]); 
            }
        }
        printf("}\n");   
        for(j=0;j<CRYPTO_BYTES;j++) {
        if(key_a[j] != key_b[j]) {
        fprintf(stderr, "ERROR\n");
        return -1;
      }
    }
  }
  return 0;
}
