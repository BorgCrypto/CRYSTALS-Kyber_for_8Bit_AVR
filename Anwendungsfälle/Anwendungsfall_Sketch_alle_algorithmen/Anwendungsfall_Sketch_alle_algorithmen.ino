#include <crystalskyber.h>
#include <string.h>
#include <RNG.h>
  unsigned char pk[CRYPTO_PUBLICKEYBYTES];
  uint8_t sk[CRYPTO_SECRETKEYBYTES];
  uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
  uint8_t key_a[CRYPTO_BYTES];
  uint8_t key_b[CRYPTO_BYTES];

void setup() {

  RNG.begin("CrystalsKyber");
  Serial.begin(9600);
    Serial.println(F("Executing "));
    CrystalsKyber::crypto_kem_keypair(pk, sk);
    CrystalsKyber::crypto_kem_enc(ct, key_b, pk);
    CrystalsKyber::crypto_kem_dec(key_a, ct, sk);

    if (memcmp(key_a, key_b, CRYPTO_BYTES) != 0) {
      Serial.println(F("Keys do not match."));
    } else {
      Serial.println(F("Keys match."));
    }
}

void loop() {

}