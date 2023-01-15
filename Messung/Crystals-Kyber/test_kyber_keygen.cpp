#include "crystalskyber.h"
#include <string.h>
#include "randombytes.h"
#include "params.h"
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "HardwareSerial.h"
#include "WString.h"
#include "test_vectors.h"

void check_KeyGen();
void exec_randombytes();
 int main(int argc, char const *argv[])
 { Serial.begin(38400); 
    if (Use_Deterministic_RNG == 0 && SECRET_KEY_VARIANT < 0 && SECRET_KEY_VARIANT > 8 && KYBER_K < 2 && KYBER_K > 4) {
    Serial.print(F("Select valid values. \n"));

  } else {
    Serial.print(F("Test KeyGen:\n"));
    Serial.print(F("K:"));Serial.println(KYBER_K);
    Serial.print(F("SK variant:"));Serial.println(SECRET_KEY_VARIANT);

    check_KeyGen();//Whe testing Variant 1 the output may look different. This may be caused by the high amount of RAM that is used. 

 }  
    Serial.flush();
    Serial.write(4);
    while (1){}
    return 0;
 }


void check_KeyGen() {
  #if(SECRET_KEY_VARIANT!=1)
  for (int j = 0; j < 5; j++)
  {
  #endif   //variables used for the comparison of values
  int i;
  uint8_t checkvalue;
  bool fehler=false;
  //ouputs
  uint8_t pk[CRYPTO_PUBLICKEYBYTES];
  uint8_t sk[CRYPTO_SECRETKEYBYTES];
  CrystalsKyber::crypto_kem_keypair(pk, sk);
  //Serial.print(F("Comparison of generated pk with test values.\n"));
  #if(SECRET_KEY_VARIANT!=1)
  uint8_t *test_values_address=pgm_read_ptr(&(indexes[j*4]));
  #else
  uint8_t *test_values_address=pgm_read_ptr(&(indexes[0]));
  #endif
  for (i = 0; i < CRYPTO_PUBLICKEYBYTES; i++) {
    checkvalue = pgm_read_word(test_values_address + i);
    if (pk[i] != checkvalue) {
      fehler = true;
    }
  }
  if (fehler) {
    Serial.print(F("Error."));
  } else {
    Serial.print(F("PKs match.\n"));
  }
  //Serial.print(F("Comparison of generated sk with test values.\n"));
  fehler = false;
  #if(SECRET_KEY_VARIANT!=1)
  test_values_address=pgm_read_ptr(&(indexes[j*4+1]));
  #else
  test_values_address=pgm_read_ptr(&(indexes[1]));
  #endif
  for (i = 0; i < CRYPTO_SECRETKEYBYTES; i++) {
    checkvalue = pgm_read_word(test_values_address + i);
    if (sk[i] != checkvalue) {
      fehler = true;
    }
  }
  if (fehler) {
   Serial.print(F("ERROR.\n"));
  } else {
    Serial.print(F("SKs match.\n"));
  }
  #if(SECRET_KEY_VARIANT!=1)
  exec_randombytes();
  }
  #endif
}

void exec_randombytes() {
  uint8_t test[32];
  randombytes(test, 32);
}