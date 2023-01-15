#include <Curve25519KEM.h>
#include <Curve25519.h>
#include <SHA256.h>
#include <HKDF.h>
#include <string.h>

void Curve25519KEM::keyGen(uint8_t pk[32],uint8_t sk[32]){
    Curve25519::dh1(pk, sk);
}

void Curve25519KEM::encap(uint8_t shared_secret[32], uint8_t enc[32], const uint8_t publicKey[32],uint8_t test_eph_sk[32]){
//Initialize with publicKey; dh2() will write its output to the dh array.
  uint8_t dh[32];
  memcpy(dh, publicKey, 32);//copy the public key, because Curve25519::dh2() overwrites the public key parameter with the shared secret
  //ephemeral secret key
  uint8_t ephemeral_secret_key[32];
  //ephemeral public key + static public key
  uint8_t kem_context[64];
  //generate ephemeral keys
  if(test_eph_sk!=0){
    memcpy(ephemeral_secret_key,test_eph_sk,32);
    Curve25519::eval(kem_context,ephemeral_secret_key,0);
  }else{
    Curve25519::dh1(kem_context, ephemeral_secret_key);
  }
  //enc is equal to the ephemeral public key, because no extra encoding is needed
  memcpy(enc, kem_context, 32);
  //there is no extra encoding for the public key in case of Curve25519
  memcpy(kem_context + 32, publicKey, 32);
  Curve25519::dh2(dh, ephemeral_secret_key);//calculate dh value
  Curve25519KEM::extractAndExpand(shared_secret, dh, kem_context);//Calculate the shared secret
}

 void Curve25519KEM::decap(uint8_t shared_secret[32], uint8_t enc_local[32], const uint8_t secretKey[32]){
  //public_key_eph=DeserializePublicKey(enc), but for Curve25519 this is the identity function.
  uint8_t dh[32];             //placeholder for calculated DH-Value
  memcpy(dh, enc_local, 32);  //save ephemeral public key value in dh
  uint8_t kem_context[64];
  Curve25519::eval(kem_context + 32, secretKey, 0);  //derive static public key from static secret key
  Curve25519::dh2(dh, secretKey); //calculate dh and save the result into the dh value.
  memcpy(kem_context, enc_local, 32);//form the kem_context
  Curve25519KEM::extractAndExpand(shared_secret, dh, kem_context);//compute the shared secret
}

void Curve25519KEM::extractAndExpand(uint8_t *output, uint8_t *dh, uint8_t *kem_context) {
  unsigned char labeled_ikm[51] = { 'H', 'P', 'K', 'E', '-', 'v', '1', 'K', 'E', 'M', 0x00, 0x20, 'e', 'a', 'e', '_', 'p', 'r', 'k' };
  memcpy(labeled_ikm + 19, dh, 32);
  unsigned char labeled_info[91] = { 0x00, 0x20, 'H', 'P', 'K', 'E', '-', 'v', '1', 'K', 'E', 'M', 0x00, 0x20, 's', 'h', 'a', 'r', 'e', 'd', '_', 's', 'e', 'c', 'r', 'e', 't' };
  memcpy(labeled_info + 27, kem_context, 64);
  hkdf<SHA256>(output, 32, labeled_ikm, sizeof(labeled_ikm), "", 0, labeled_info, sizeof(labeled_info));
}

