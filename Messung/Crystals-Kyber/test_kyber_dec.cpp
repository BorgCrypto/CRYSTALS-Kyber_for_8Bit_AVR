/**
 * This Code is based on and contains parts of:
 * 
 * The Reference Implementation of CRYSTALS-KYBER
 * URL: https://github.com/pq-crystals/kyber/tree/round3
 * License of used code: Public Domain (https://creativecommons.org/share-your-work/public-domain/cc0/)
 * 
 * and own code (adjustments and extensions) under MIT-License.
*/
#include "crystalskyber.h"
#include <string.h>
#include "test_vectors.h"
#include "randombytes.h"
#include "params.h"
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "HardwareSerial.h"
#include "WString.h"


void test_keys();
void test_invalid_sk();
void test_invalid_ciphertext();
uint8_t j;
 int main(int argc, char const *argv[])
 { Serial.begin(38400); 
    if (Use_Deterministic_RNG == 0 && SECRET_KEY_VARIANT < 0 && SECRET_KEY_VARIANT > 8 && KYBER_K < 2 && KYBER_K > 4) {
    Serial.print(F("Select valid values. \n"));

  } else {
      Serial.println(F("Test Decapsulation:"));
    Serial.print(F("K:"));Serial.println(KYBER_K);
    Serial.print(F("SK variant:"));Serial.println(SECRET_KEY_VARIANT);
 
    for ( j = 0; j < 5; j++)
    {
           test_keys();
  #if(KYBER_K!=4 && (KYBER_K!=3 && SECRET_KEY_VARIANT!=3)) || (K==4 && SECRET_KEY_VARIANT==8) 
    test_invalid_sk();
    test_invalid_ciphertext();
  #endif
    }
    
  
 }  

    Serial.flush();
    Serial.write(4);
    while (1){}
    
    return 0;
 }
void test_invalid_sk(){
    uint8_t sk[CRYPTO_SECRETKEYBYTES];
    uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
    uint8_t key[CRYPTO_BYTES];
    int i;
    bool fehler;
    uint8_t checkvalue;

    randombytes(sk,CRYPTO_SECRETKEYBYTES);
    #if (SECRET_KEY_VARIANT==6 || SECRET_KEY_VARIANT==7)
    randombytes(sk,CRYPTO_SECRETKEYBYTES);
    #endif

  uint8_t *test_values_address=pgm_read_ptr(&(indexes[j*4+2]));
  for (i = 0; i < CRYPTO_CIPHERTEXTBYTES; i++) {
    ct[i] = (uint8_t)pgm_read_word_near(test_values_address + i);
  }
    
    CrystalsKyber::crypto_kem_dec(key, ct, sk);
    fehler = false;
      test_values_address=pgm_read_ptr(&(indexes[j*4+3]));

  for (i = 0; i < CRYPTO_BYTES; i++) {
    checkvalue = pgm_read_word_near(test_values_address + i);
    if (key[i] != checkvalue) {
      fehler = true;
    }
  }
  if(!fehler){
    Serial.println("Invalid SK.");
  }
}

void test_invalid_ciphertext(){
    uint8_t sk[CRYPTO_SECRETKEYBYTES];
    uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
    uint8_t key[CRYPTO_BYTES];
    uint8_t b;
    size_t pos;
    int i;
    bool fehler=false;
    uint8_t checkvalue;

  do {
    randombytes(&b, sizeof(uint8_t));
  } while(!b);
  randombytes((uint8_t *)&pos, sizeof(size_t));

  uint8_t *test_values_address=pgm_read_ptr(&(indexes[j*4+1]));
  for (i = 0; i < CRYPTO_SECRETKEYBYTES; i++) {
    sk[i] = (uint8_t)pgm_read_word_near(test_values_address + i);
  }

  test_values_address=pgm_read_ptr(&(indexes[j*4+2]));
  for (i = 0; i < CRYPTO_CIPHERTEXTBYTES; i++) {
    ct[i] = (uint8_t)pgm_read_word_near(test_values_address + i);
  }

  ct[pos % CRYPTO_CIPHERTEXTBYTES] ^= b;

  CrystalsKyber::crypto_kem_dec(key, ct, sk);
  test_values_address=pgm_read_ptr(&(indexes[j*4+3]));
  for (i = 0; i < CRYPTO_BYTES; i++) {
    checkvalue = pgm_read_word_near(test_values_address + i);
    if (key[i] != checkvalue) {
      fehler = true;
    }
  }
  if(!fehler){
    Serial.println("Invalid CT.");
  }
}

 void test_keys() { 
  uint8_t checkvalue;
  bool fehler;
  int i;
  //inputs
  uint8_t sk[CRYPTO_SECRETKEYBYTES];
  uint8_t *test_values_address=pgm_read_ptr(&(indexes[j*4+1]));
  for (int k = 0; k < CRYPTO_SECRETKEYBYTES; k++) {
    sk[k] = (uint8_t)pgm_read_word_near(test_values_address + k);
  }
  uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
  test_values_address=pgm_read_ptr(&(indexes[j*4+2]));
  for (i = 0; i < CRYPTO_CIPHERTEXTBYTES; i++) {
    ct[i] = (uint8_t)pgm_read_word_near(test_values_address + i);
  }
  //ouputs
  uint8_t key[CRYPTO_BYTES];
  CrystalsKyber::crypto_kem_dec(key, ct, sk);
  test_values_address=pgm_read_ptr(&(indexes[j*4+3]));
  fehler = false;
  for (i = 0; i < CRYPTO_BYTES; i++) {
    checkvalue = pgm_read_word_near(test_values_address + i);
    if (key[i] != checkvalue) {
      fehler = true;
    }
  }
  if (fehler) {
    Serial.print(F("ERROR."));
  } else {
    Serial.print(F("Shared key values match.\n"));
  }
}