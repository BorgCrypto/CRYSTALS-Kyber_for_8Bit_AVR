#include "Curve25519KEM.h"
#include "print.h"
#include <string.h>
#include "RNG.h"
#include "avr.h"
void  random_test_KEM_Curve25519();

int main()
{ RNG.begin("Test");  
  random_test_KEM_Curve25519();
  avr_end();
  return 0;
}

void random_test_KEM_Curve25519(){
    int i;
    uint8_t static_public_key[32];
    uint8_t static_secret_key[32];
    uint8_t static_secret_key_temp[32];
    uint8_t enc[32]; //ephemeral public key
    uint8_t shared_secret_a[32];    
    uint8_t shared_secret_b[32];
    print("#include <avr/pgmspace.h> \n");    
    for (int j = 0; j < 15; j++)
    {
    Curve25519KEM::keyGen(static_public_key,static_secret_key);   
    Curve25519KEM::encap(shared_secret_a,enc,static_public_key,0);
    memcpy(static_secret_key_temp,static_secret_key,32);
    Curve25519KEM::decap(shared_secret_b, enc, static_secret_key_temp);
    print("const PROGMEM uint8_t pk_values_");printllu(j);print("[32]={");
    for (i = 0; i < 32; i++)
    {
      if (i==31)
      {
        printllu(static_public_key[i]);
      }else{
        printllu(static_public_key[i]);print(",");
      }
    }
    print("};\n");
    print("const PROGMEM uint8_t sk_values_");printllu(j);print("[32]={");
    for (i = 0; i < 32; i++)
    {
      if (i==31)
      {
        printllu(static_secret_key[i]);
      }else{
        printllu(static_secret_key[i]);print(",");
      }
    }
    print("};\n");
    print("const PROGMEM uint8_t enc_values_");printllu(j);print("[32]={");
    for (i = 0; i < 32; i++)
    {
      if (i==31)
      {
        printllu(enc[i]);
      }else{
        printllu(enc[i]);print(",");
      }
    }
    print("};\n");
    }
    print("const uint8_t* const indexes[45]PROGMEM={");
    for (i = 0; i < 15; i++)
    {
      if(i==14){
        print("pk_values_");printllu(i);print(",sk_values_");printllu(i);print(",enc_values_");printllu(i);
      }else{
        print("pk_values_");printllu(i);print(",sk_values_");printllu(i);print(",enc_values_");printllu(i);print(",");
      }
      
    }
    print("};");
    
    
}
