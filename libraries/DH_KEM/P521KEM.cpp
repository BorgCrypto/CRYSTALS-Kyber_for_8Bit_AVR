#include <P521KEM.h>
#include <P521.h>
#include <SHA512.h>
#include <HKDF.h>
#include <string.h>


 void P521KEM::keyGen(uint8_t pk[32],uint8_t sk[32]){
    P521::dh1(pk,sk);
} 

 void P521KEM::encap(uint8_t shared_secret[64], uint8_t enc[133], const uint8_t publicKey[132],uint8_t test_eph_sk[66]){
    uint8_t kem_context[266];
  uint8_t *eph_public_key = kem_context + 1;
  uint8_t eph_secret_key[66];
  if(test_eph_sk!=0){
    memcpy(eph_secret_key,test_eph_sk,66);
    P521::eval(eph_public_key, eph_secret_key,0);
  }else{
    P521::dh1(eph_public_key, eph_secret_key);
  }
  //enc = SerializePublicKey(pkE)
  memcpy(enc + 1, eph_public_key, 132);
  enc[0] = 0x04;
  //pkRm = SerializePublicKey(pkR)
  memcpy(kem_context, enc, 132);
  kem_context[133] = 0x04;
  memcpy(kem_context + 134, publicKey, 132);
  P521::dh2(publicKey, eph_secret_key);
  P521KEM::extractAndExpand(shared_secret, eph_secret_key, kem_context);
 }

 void P521KEM::decap(uint8_t shared_secret[64], uint8_t enc_local[133], const uint8_t secretKey[66]){
  uint8_t *eph_public_key = enc_local + 1;
  uint8_t kem_context[266];
  P521::eval(kem_context + 134, secretKey, 0);  //derive static public key from static secret key
  kem_context[133] = 0x04;
  P521::dh2(eph_public_key, secretKey);
  memcpy(kem_context, enc_local, 133);
  P521KEM::extractAndExpand(shared_secret, secretKey, kem_context);
} 


 void  P521KEM::extractAndExpand(uint8_t *output, uint8_t *dh, uint8_t *kem_context){
  unsigned char labeled_ikm[85] = { 'H', 'P', 'K', 'E', '-', 'v', '1', 'K', 'E', 'M', 0x00, 0x12, 'e', 'a', 'e', '_', 'p', 'r', 'k' };
  memcpy(labeled_ikm + 19, dh, 66);
  unsigned char labeled_info[293] = { 0x00, 0x40, 'H', 'P', 'K', 'E', '-', 'v', '1', 'K', 'E', 'M', 0x00, 0x12, 's', 'h', 'a', 'r', 'e', 'd', '_', 's', 'e', 'c', 'r', 'e', 't' };
  memcpy(labeled_info + 27, kem_context, 266);
  hkdf<SHA512>(output, 64, labeled_ikm, sizeof(labeled_ikm), "", 0, labeled_info, sizeof(labeled_info));
}