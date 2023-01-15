#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "api.h"
#include "randombytes.h"

struct TestVectors{
  uint8_t pk[CRYPTO_PUBLICKEYBYTES];
  uint8_t sk[CRYPTO_SECRETKEYBYTES];
  uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
  uint8_t key[CRYPTO_BYTES];
};

int main(void)
{   
    printf("Test: K=%d \n",KYBER_K);
    struct TestVectors listOfTestValues[100];
    FILE *fp;
    #if KYBER_K ==2
    fp=fopen("testvectors512.bin","rb");
    #elif KYBER_K ==3
    fp=fopen("testvectors768.bin","rb");
    #elif KYBER_K ==4
    fp=fopen("testvectors1024.bin","rb");
    #endif  
    
    size_t status=fread(listOfTestValues,sizeof(struct TestVectors),100,fp);
    fclose(fp);
    if(status==0){
      printf("ERROR with reading data from file. \n");
      return 1;
    }else{
      printf("Test values have been read from file. \n");
    }
    
  uint8_t pk[CRYPTO_PUBLICKEYBYTES];
  uint8_t sk[CRYPTO_SECRETKEYBYTES];
  uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
  uint8_t key_a[CRYPTO_BYTES];
  uint8_t key_b[CRYPTO_BYTES];
  int error=0;
  for(int i=0;i<100;i++) {
    crypto_kem_keypair(pk,sk);
    crypto_kem_enc(ct,key_b,pk);
    crypto_kem_dec(key_a,ct,sk);

    if (memcmp(listOfTestValues[i].pk,pk,CRYPTO_PUBLICKEYBYTES)!=0 || memcmp(listOfTestValues[i].sk,sk,CRYPTO_SECRETKEYBYTES)!=0 || memcmp(listOfTestValues[i].ct,ct,CRYPTO_CIPHERTEXTBYTES)!=0
    || memcmp(listOfTestValues[i].key,key_a,32)!=0 || memcmp(listOfTestValues[i].key,key_b,32)!=0)
    {
      error=1;
    }
  }
  if(error==0){
    printf("All values are matching. \n");
  }else{
    printf("ERROR. Generated and test values are not matching. \n");
  }

  return 0;
}

