//#include "cpucycles.h"
//#include "avr.h"
//#include "print.h"
#include "crystalskyber.h"
#include <string.h>
#include "test_vectors.h"
#include "randombytes.h"
#include "params.h"
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "HardwareSerial.h"
#include "WString.h"

void exec_randombytes();
void check_Enc();


 int main(int argc, char const *argv[])
 { Serial.begin(38400); 
    if (Use_Deterministic_RNG == 0 && SECRET_KEY_VARIANT < 0 && SECRET_KEY_VARIANT > 8 && KYBER_K < 2 && KYBER_K > 4) {
    Serial.print(F("Select valid values. \n"));

  } else {
    Serial.print(F("Testing Enc:\n"));
    Serial.print(F("K:"));Serial.println(KYBER_K);
    Serial.print(F("SK variant:"));Serial.println(SECRET_KEY_VARIANT);
    //simulating the calls of randombytes by Keygen
    check_Enc();
 }  

    Serial.flush();
    Serial.write(4);
    while (1){}
    
    return 0;
 }

 void check_Enc() {
  bool fehler;
  for (int j = 0; j < 5; j++)
  {
    int i;
    uint8_t checkvalue;
    exec_randombytes();
    exec_randombytes();
    //inputs
    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t *test_values_address=pgm_read_ptr(&(indexes[j*4]));
    for (i = 0; i < CRYPTO_PUBLICKEYBYTES; i++) {
      pk[i] = pgm_read_word(test_values_address + i);
    }
    //outpus
    uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
    uint8_t key[CRYPTO_BYTES];
    CrystalsKyber::crypto_kem_enc(ct, key, pk);
    fehler = false;
    //Serial.print(F("Comparison of generated ciphertext with test values.\n"));
    test_values_address=pgm_read_ptr(&(indexes[j*4+2]));
    for (i = 0; i < KYBER_CIPHERTEXTBYTES; i++) {
      checkvalue = pgm_read_word(test_values_address + i);
      if (ct[i] != checkvalue) {
        fehler = true;
      }
    }
    //Serial.print(F("Comparing the generated shared key with test values.\n"));
    test_values_address=pgm_read_ptr(&(indexes[j*4+3]));
    for (i = 0; i < KYBER_SSBYTES; i++) {
      checkvalue = pgm_read_word(test_values_address + i);
      if (key[i] != checkvalue) {
        fehler = true;
      }
    }
    }
    if (fehler) {
      Serial.print(F("ERROR.\n"));
    } else {
      Serial.print(F("Shared keys and Ciphertexts match.\n"));
    }

}

void exec_randombytes() {
  uint8_t test[32];
  randombytes(test, 32);
}