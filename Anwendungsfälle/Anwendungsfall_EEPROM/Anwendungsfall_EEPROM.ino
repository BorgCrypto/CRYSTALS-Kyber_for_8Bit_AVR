#include <crystalskyber.h>
#include <string.h>
#include <RNG.h>
#include <EEPROM.h>

uint8_t sk[CRYPTO_SECRETKEYBYTES];
uint8_t pk[CRYPTO_PUBLICKEYBYTES];

void keygen() {
  uint8_t pk[CRYPTO_PUBLICKEYBYTES];  //public key that shall be sent to the other communication party
  CrystalsKyber::crypto_kem_keypair(pk, sk);
}

void save_sk() {
  int maxLen = 0;
  if (Use_Deterministic_RNG == 0) {
    maxLen = 4096 - 48;  //4KB of EEPROM and the 48 Byte used by RNG;
  } else {
    maxLen = 4096;
  }
  if (CRYPTO_SECRETKEYBYTES < maxLen) {
    for (int i = 0; i < CRYPTO_SECRETKEYBYTES; i++) {
      EEPROM.write(i, sk[i]);
    }
  } else {
    Serial.println("Secret key is to large for saving in EEPROM.");
  }
}

void read_sk() {
  for (int i = 0; i < CRYPTO_SECRETKEYBYTES; i++) {
    sk[i] = EEPROM.read(i);
  }
}

void clear_SK_variable() {
  for (int i = 0; i < CRYPTO_SECRETKEYBYTES; i++) {
    sk[i] = 0;
  }
}
void clear_SK_EEPROM() {
  for (int i=0; i < CRYPTO_SECRETKEYBYTES; i++) {
    EEPROM.write(i, 0);
  }
}

void print_sk() {
  Serial.print("[");
  for (int i = 0; i < CRYPTO_SECRETKEYBYTES; i++) {
    if (i % 100) {
      Serial.print(sk[i]);
      Serial.print(",");
    } else {
      Serial.print(sk[i]);
      Serial.println(",");
    }
  }
  Serial.println("]");
}

void print_pk() {
  Serial.print("[");
  for (int i = 0; i < CRYPTO_PUBLICKEYBYTES; i++) {
    if (i % 100) {
      Serial.print(sk[i]);
      Serial.print(",");
    } else {
      Serial.print(pk[i]);
      Serial.println(",");
    }
  }
  Serial.println("]");
}


void compare_sk() {
  bool equal = true;
  for (int i=0; i < CRYPTO_SECRETKEYBYTES; i++) {
    if (sk[i] != EEPROM.read(i)) {
      equal = false;
      //Serial.println("Equal wird auf false gesetzt.");
    }
  }
  if(equal){
    Serial.println("SK's are equal.");    
  }else{
    Serial.println("SK's are not equal.");    
    }
  
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if (Use_Deterministic_RNG == 0) {
    Serial.println("RNG is used.");
    RNG.begin("TEST");
  }else{
    Serial.println("Test-RNG is used.");
  }
  
  Serial.println("1. Keygen");
  Serial.println("2. save_sk");
  Serial.println("3. read_sk");
  Serial.println("4. clear_sk_variable");
  Serial.println("5. clear_sk_eeprom");
  Serial.println("6. Print SK to serial monitor");
  Serial.println("7. Compare SK variable with SK in EEPROM.");
}

void loop() {
  Serial.println("Which action shall be executed? ");

  while (Serial.available() == 0) {
  }

  int menuChoice = Serial.parseInt();

  switch (menuChoice) {
    case 1:
      // temp sensor code goes here
      Serial.println("Keygen gets executed.");
      keygen();
      break;

    case 2:
      // humidity sensor code goes here
      Serial.println("SK gets saved in EEPROM.");
      save_sk();
      break;

    case 3:
      // pressure sensor code goes here
      Serial.println("SK gets read from EEPROM.");
      read_sk();
      break;
    case 4:
      // temp sensor code goes here
      Serial.println("SK variable gets cleared.");
      clear_SK_variable();
      break;
    case 5:
      // temp sensor code goes here
      Serial.println("SK in EEPROM gets cleared.");
      clear_SK_EEPROM();
      break;
    case 6:
      // temp sensor code goes here
      Serial.println("Print PK to the Serial Monitor.");
      Serial.println(String(CRYPTO_PUBLICKEYBYTES) + " Bytes");
      print_sk();
      Serial.println("Print SK to the Serial Monitor.");
      Serial.println(String(CRYPTO_SECRETKEYBYTES) + " Bytes");
      print_pk();
      break;
    case 7:
      // temp sensor code goes here
      Serial.println("Compare SK Variable with SK saved in EEPROM.");
      compare_sk();
      break;
    default:
      Serial.println("Please choose a valid selection");
  }
}


