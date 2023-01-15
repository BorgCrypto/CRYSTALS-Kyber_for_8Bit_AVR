#include <stdint.h>
#include "Crypto.h"
#include "NewHope.h"
#include "RNG.h"
#include "string.h"
#include "print.h"
#include "avr/pgmspace.h"
#include "avr.h"
#include "KEM_NewHope_testvectors.h"
#include "randombytes.h"
void testNewHopeFixed();
int main(int argc, char const *argv[])
{
    testNewHopeFixed();
    avr_end();
    return 0;
}

void testNewHopeFixed(){
    int i,j;
    NewHopePrivateKey sk;
    uint8_t buffer[2048];
    uint8_t ss_a[32];
    uint8_t ss_b[32];
    bool error=false;
    uint8_t checkvalue;
    uint8_t random_seed[64];
    uint8_t *test_values_address;
    for ( j = 0; j < 5; j++)
    {   
        randombytes(random_seed,64);
        NewHope::keygen(buffer,sk,NewHope::Ref,random_seed);
        test_values_address=pgm_read_ptr(&(indexes[j*4]));
    for (i = 0; i < 32; i++) {
        checkvalue = pgm_read_word(test_values_address+ i); 
        if(checkvalue!=sk.seed[i]);
    }

    test_values_address=pgm_read_ptr(&(indexes[j*4+1]));
    for ( i = 0; i < 1824; i++)
    {
        checkvalue = pgm_read_word(test_values_address + i); 
        if(checkvalue!=buffer[i]){
            error=true;
        }
    }

    randombytes(random_seed,32);
    NewHope::sharedb(ss_a,buffer,buffer,NewHope::Ref,random_seed);
    test_values_address=pgm_read_ptr(&(indexes[j*4+2]));
    for ( i = 0; i < 2048; i++)
    {
        checkvalue=pgm_read_word(test_values_address + i);
        if(checkvalue!=buffer[i]){
            error=true;
        }
    }

    test_values_address=pgm_read_ptr(&(indexes[j*4+3]));
    for ( i = 0; i < 32; i++)
    {
        checkvalue=pgm_read_word(test_values_address+ i);
        if(checkvalue!=ss_a[i]){
            error=true;
        }
    }

    NewHope::shareda(ss_b, sk, buffer);
    test_values_address=pgm_read_ptr(&(indexes[j*4+3]));
    for ( i = 0; i < 32; i++)
    {
        checkvalue=pgm_read_word(test_values_address+ i);
        if(checkvalue!=ss_b[i]){
            error=true;
        }
    }  

    }
        if(error){
        print("Error.\n");
    }else{
        print("All values match the testvalues.\n");
    }
}
