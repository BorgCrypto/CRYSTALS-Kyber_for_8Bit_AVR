/**
 * This Code is based on and contains parts of:
 * 
 * The Reference Implementation of CRYSTALS-KYBER
 * URL: https://github.com/pq-crystals/kyber/tree/round3
 * License of used code: Public Domain (https://creativecommons.org/share-your-work/public-domain/cc0/)
 * 
 * Post-quantum crypto library for the ARM Cortex-M4
 * By Matthias J. Kannwischer and Richard Petri and Joost Rijneveld and Peter Schwabe and Ko Stoffelen
 * URL: https://github.com/mupq/pqm4
 * License of used code: CC0 1.0 Universal (CC0 1.0) Public Domain Dedication (https://creativecommons.org/publicdomain/zero/1.0/)
 * 
 * and own code (adjustments and extensions) under MIT-License.
*/
#include "indcpa.h"
#include "ntt.h"
#include "poly.h"
#include "symmetric.h"
#include "matacc.h"
#include <string.h>
#include <Crypto.h>
#if (Use_Deterministic_RNG==1)
#include "randombytes.h"
#else
#include <RNG.h>
#endif

#if (SECRET_KEY_VARIANT==4) || (SECRET_KEY_VARIANT==5)
/*************************************************
* Name:        complexPackAndShift
*
* Description: Calculating 3-Bit Form of the input value.
*              Outputting this value (in splitted form) into two given 8-Bit Variables 
*              and Left_shift them.
* Arguments:   - uint16_t *inputValue: pointer to input value
*              - uint8_t *firstOutput: pointer to first output value
*              - uint8_t *secondOutput: pointer to second output value
*              - int freeBitsInFirstOutput: number of free bits in first ouputvalue 
* 
**************************************************/
void linebreakPackAndShift(const int16_t inputValue,uint8_t *firstOutput, uint8_t *secondOutput, int freeBitsInFirstOutput){
    int16_t tempInput=inputValue;
    *firstOutput=*firstOutput<<freeBitsInFirstOutput;
    if(tempInput<0){
        if (freeBitsInFirstOutput==1)
        {
            *firstOutput=*firstOutput|1;
        }else{//freeBitsInFirstOutput==2
            *firstOutput=*firstOutput|2;
        }
        tempInput*=(-1);        
    }
    if (freeBitsInFirstOutput==1){
        *secondOutput=*secondOutput|tempInput;
    }else{//freeBitsInFirstOutput==2
        if (tempInput>=2){
            *firstOutput=*firstOutput|1;
        }
        if (tempInput%2)
        {   
            *secondOutput=*secondOutput|1;
        }
        
    }
}

void linebreakUnpackAndShift(int16_t *output,uint8_t *firstInput, uint8_t *secondInput, int indikatorFirstOutput){
    if (indikatorFirstOutput==1){   
        //*secondInput=*secondInput >> 6;
        *output=(*secondInput & 3);
        if (*firstInput & 1)
        {
            *output*=-1;
        }
        
    }else{
        //*secondInput=*secondInput >> 7;
        *output=((*firstInput & 1)*2)+(*secondInput & 1);
        if (*firstInput & 2)
        {
            *output*=-1;
        }
        
    }
    *firstInput=*firstInput >> indikatorFirstOutput;
    //*secondInput=*secondInput >> indikatorFirstOutput;
}

/*************************************************
* Name:        simplePackAndShift
*
* Description: Calculating 3-Bit Form of the input value.
*              Outputting this value Into given 8-Bit Variable 
*              and Left_shift.
* Arguments:   - uint16_t inputValue: input value
*              - uint8_t *outputValue: pointer to output value
*              - int shift: number of shifts that are executed after the calculation
* 
**************************************************/
void simplePackAndShift(const int16_t inputValue,uint8_t *outputValue, int shift){
    int16_t tempInput=inputValue;
    *outputValue=*outputValue<<shift;
    if(tempInput<0){
        *outputValue=*outputValue|4;
        tempInput*=-1;     
    }
    *outputValue|=tempInput;
}

void simpleUnpackAndShift(uint8_t *inputValue, int16_t *ouputvalue, int shift){
    *ouputvalue=*inputValue & 3;
    if (*inputValue & 4)
    {
        *ouputvalue*=-1;
    }
    *inputValue=*inputValue>>shift;
}

void sk_serialization(const polyvec skpv, unsigned char *sk){
    for (int i = 0; i < KYBER_K; i++){
        int counter=0;
        for (int j = 0; j < KYBER_N; j+=8){ 
            //preset all bytes with zero
            sk[i*96+counter]=0;
            sk[i*96+counter+1]=0;
            sk[i*96+counter+2]=0;
            simplePackAndShift(skpv.vec[i].coeffs[j],sk+i*96+counter,0);
            simplePackAndShift(skpv.vec[i].coeffs[j+1],sk+i*96+counter,3);
            linebreakPackAndShift(skpv.vec[i].coeffs[j+2],sk+i*96+counter,sk+i*96+counter+1,2);
            simplePackAndShift(skpv.vec[i].coeffs[j+3],sk+i*96+counter+1,3);
            simplePackAndShift(skpv.vec[i].coeffs[j+4],sk+i*96+counter+1,3);
            linebreakPackAndShift(skpv.vec[i].coeffs[j+5],sk+i*96+counter+1,sk+i*96+counter+2,1);
            simplePackAndShift(skpv.vec[i].coeffs[j+6],sk+i*96+counter+2,3);
            simplePackAndShift(skpv.vec[i].coeffs[j+7],sk+i*96+counter+2,3);
            counter+=3;
        }
        
    }
    
}

void sk_deserialization(polyvec *skpv, const unsigned char *sk){
    for (int i = 0; i < KYBER_K; i++)
    {
        int counter=0;
        for (int j = 0; j < KYBER_N; j+=8)
        {   uint8_t temp[2];
            temp[0]=sk[i*96+counter+2];
            simpleUnpackAndShift(&temp[0],&skpv->vec[i].coeffs[j+7],3);
            simpleUnpackAndShift(&temp[0],&skpv->vec[i].coeffs[j+6],3);
            temp[1]=sk[i*96+counter+1];
            linebreakUnpackAndShift(&skpv->vec[i].coeffs[j+5],&temp[1],&temp[0],1);
            simpleUnpackAndShift(&temp[1],&skpv->vec[i].coeffs[j+4],3);
            simpleUnpackAndShift(&temp[1],&skpv->vec[i].coeffs[j+3],3);
            temp[0]=sk[i*96+counter];
            linebreakUnpackAndShift(&skpv->vec[i].coeffs[j+2],&temp[0],&temp[1],2);
            simpleUnpackAndShift(&temp[0],&skpv->vec[i].coeffs[j+1],3);
            simpleUnpackAndShift(&temp[0],&skpv->vec[i].coeffs[j],0);
            counter+=3;
        }
        
    }
    
}
#endif

#if (SECRET_KEY_VARIANT==0) || (SECRET_KEY_VARIANT==2)
/*************************************************
* Name:        indcpa_keypair
*
* Description: Generates public and private key for the CPA-secure
*              public-key encryption scheme underlying Kyber
*
* Arguments:   - unsigned char *pk: pointer to output public key (of length KYBER_INDCPA_PUBLICKEYBYTES bytes)
*              - unsigned char *sk: pointer to output private key (of length KYBER_INDCPA_SECRETKEYBYTES bytes)
**************************************************/
void indcpa_keypair(unsigned char *pk, unsigned char *sk) {
    polyvec skpv;//vector of polynomials; k koeffizeinten;
    //skpv= secret key polynomial vector
    poly pkp; //polynom; Array mit 256 koeffizienten (signed) 
    //pkp=public key polynomial
    unsigned char buf[2 * KYBER_SYMBYTES];
    unsigned char *publicseed = buf;
    unsigned char *noiseseed = buf + KYBER_SYMBYTES;
    int i;
    unsigned char nonce = 0;   
    #if(Use_Deterministic_RNG==1) 
    randombytes(buf, KYBER_SYMBYTES);
    #else
    RNG.rand(buf, KYBER_SYMBYTES);
    #endif
    hash_g(buf, buf, KYBER_SYMBYTES); //IN,OUT,INPUTLEN

    for (i = 0; i < KYBER_K; i++){
        poly_getnoise_eta1(skpv.vec + i, noiseseed, nonce++);
    }

    polyvec_ntt(&skpv);//transfrom s  into ntt domain
    for (i = 0; i < KYBER_K; i++) {
        //generate matrix a on the fly and multiply with secret key; accumulate result; row by row;
        matacc(&pkp, &skpv, i, publicseed, 0);// generate a row of a in NTT domain and multiply with s (in NTT domain)
        poly_invntt(&pkp);//inverse ntt 
        poly_addnoise_eta1(&pkp, noiseseed, nonce++);//generate e polynomial and add noise to the calculated row
        poly_ntt(&pkp);//ntt
        poly_tobytes(pk+i*KYBER_POLYBYTES, &pkp);//barret reduce of polynomial and adding row of public matrix to public key       
    }
    polyvec_tobytes(sk, &skpv);//encode secret key
    memcpy(pk + KYBER_POLYVECBYTES, publicseed, KYBER_SYMBYTES); // Pack the public seed in the public key 
    clean(buf,2 * KYBER_SYMBYTES);
    clean(&skpv,sizeof(skpv));
    clean(&pkp,sizeof(pkp));
    
}
#endif

#if (SECRET_KEY_VARIANT==1)
//sk=(sk|pk|A)
void indcpa_keypair(unsigned char *pk, unsigned char *sk) {
    polyvec skpv;//vector of polynomials; k koeffizeinten;
    //skpv= secret key polynomial vector
    poly pkp; //polynom; Array mit 256 koeffizienten (signed) 
    //pkp=public key polynomial
    polyvec a;
    unsigned char buf[2 * KYBER_SYMBYTES];
    unsigned char *publicseed = buf;
    unsigned char *noiseseed = buf + KYBER_SYMBYTES;
    int i;
    unsigned char nonce = 0; 
    #if (Use_Deterministic_RNG==1)   
    randombytes(buf, KYBER_SYMBYTES);
    #else
    RNG.rand(buf, KYBER_SYMBYTES);
    #endif
    hash_g(buf, buf, KYBER_SYMBYTES); //IN,OUT,INPUTLEN
    for (i = 0; i < KYBER_K; i++){
        poly_getnoise_eta1(skpv.vec + i, noiseseed, nonce++);
    }
    polyvec_ntt(&skpv);//transfrom s  into ntt domain
    for (i = 0; i < KYBER_K; i++) {
        //generate matrix a on the fly and multiply with secret key; accumulate result; row by row;
        matacc_save_A(&pkp,&a, &skpv, i, publicseed, 0);// generate a row of a in NTT domain and multiply with s (in NTT domain)
        poly_invntt(&pkp);//inverse ntt 
        poly_addnoise_eta1(&pkp, noiseseed, nonce++);//generate e polynomial and add noise to the calculated row
        poly_ntt(&pkp);//ntt
        poly_tobytes(pk+i*KYBER_POLYBYTES, &pkp);//barret reduce of polynomial and adding row of public matrix to public key
        polyvec_tobytes(sk+KYBER_POLYVECBYTES+i*KYBER_POLYVECBYTES,&a);       
    }
    polyvec_tobytes(sk, &skpv);//encode secret key 
    memcpy(pk + KYBER_POLYVECBYTES, publicseed, KYBER_SYMBYTES); // Pack the public seed in the public key 
    clean(buf,2 * KYBER_SYMBYTES);
    clean(&skpv,sizeof(skpv));
    clean(&pkp,sizeof(pkp));
    clean(&a,sizeof(a));
}
#endif

#if (SECRET_KEY_VARIANT==3)
//sk=(sk|publicseed|noiseseed)
void indcpa_keypair(unsigned char *pk, unsigned char *sk) {
    polyvec skpv;//vector of polynomials; k koeffizeinten;
    //skpv= secret key polynomial vector
    poly pkp; //polynom; Array mit 256 koeffizienten (signed) 
    //pkp=public key polynomial
    unsigned char buf[2 * KYBER_SYMBYTES];
    unsigned char *publicseed = buf;
    unsigned char *noiseseed = buf + KYBER_SYMBYTES;
    uint16_t i;
    unsigned char nonce = 0;  
    #if(Use_Deterministic_RNG==1)  
    randombytes(buf, KYBER_SYMBYTES);
    #else
    RNG.rand(buf, KYBER_SYMBYTES);
    #endif
    hash_g(buf, buf, KYBER_SYMBYTES); //IN,OUT,INPUTLEN
    for (i = 0; i < KYBER_K; i++){
        poly_getnoise_eta1(skpv.vec + i, noiseseed, nonce++);
    }

    polyvec_ntt(&skpv);//transfrom s  into ntt domain
    for (i = 0; i < KYBER_K; i++) {
        //generate matrix a on the fly and multiply with secret key; accumulate result; row by row;
        matacc(&pkp, &skpv, i, publicseed, 0);// generate a row of a in NTT domain and multiply with s (in NTT domain)
        poly_invntt(&pkp);//inverse ntt 
        poly_addnoise_eta1(&pkp, noiseseed, nonce++);//generate e polynomial and add noise to the calculated row
        poly_ntt(&pkp);//ntt
        poly_tobytes(pk+i*KYBER_POLYBYTES, &pkp);//barret reduce of polynomial and adding row of public matrix to public key       
    }
    polyvec_tobytes(sk, &skpv);//encode secret key
    memcpy(sk + KYBER_POLYVECBYTES, publicseed, KYBER_SYMBYTES); // Pack the public seed in the secret key 
    memcpy(sk + KYBER_POLYVECBYTES + KYBER_SYMBYTES, noiseseed, KYBER_SYMBYTES); // Pack the noise seed in the secret key 
    memcpy(pk + KYBER_POLYVECBYTES, publicseed, KYBER_SYMBYTES); // Pack the public seed in the public key 
    clean(buf,2 * KYBER_SYMBYTES);
    clean(&skpv,sizeof(skpv));
    clean(&pkp,sizeof(pkp));
}
#endif

#if(SECRET_KEY_VARIANT==4)
//sk=(compressed(sk')|pk)
void indcpa_keypair(unsigned char *pk, unsigned char *sk) {
    polyvec skpv;//vector of polynomials; k koeffizeinten;
    //skpv= secret key polynomial vector
    poly pkp; //polynom; Array mit 256 koeffizienten (signed) 
    //pkp=public key polynomial
    unsigned char buf[2 * KYBER_SYMBYTES];
    unsigned char *publicseed = buf;
    unsigned char *noiseseed = buf + KYBER_SYMBYTES;
    uint16_t i;
    unsigned char nonce = 0;  
    #if(Use_Deterministic_RNG==1)  
    randombytes(buf, KYBER_SYMBYTES);
    #else
    RNG.rand(buf, KYBER_SYMBYTES);
    #endif
    hash_g(buf, buf, KYBER_SYMBYTES); //IN,OUT,INPUTLEN
    for (i = 0; i < KYBER_K; i++){
        poly_getnoise_eta1(skpv.vec + i, noiseseed, nonce++);
    }

    sk_serialization(skpv,sk);
    polyvec_ntt(&skpv);//transfrom s  into ntt domain
    for (i = 0; i < KYBER_K; i++) {
        //generate matrix a on the fly and multiply with secret key; accumulate result; row by row;
        matacc(&pkp, &skpv, i, publicseed, 0);// generate a row of a in NTT domain and multiply with s (in NTT domain)
        poly_invntt(&pkp);//inverse ntt 
        poly_addnoise_eta1(&pkp, noiseseed, nonce++);//generate e polynomial and add noise to the calculated row
        poly_ntt(&pkp);//ntt
        poly_tobytes(pk+i*KYBER_POLYBYTES, &pkp);//barret reduce of polynomial and adding row of public matrix to public key       
    }
    memcpy(pk + KYBER_POLYVECBYTES, publicseed, KYBER_SYMBYTES); // Pack the public seed in the public key 
    clean(buf,2 * KYBER_SYMBYTES);
    clean(&skpv,sizeof(skpv));
    clean(&pkp,sizeof(pkp));
}
#endif

#if (SECRET_KEY_VARIANT==5)
//sk=(compressed(sk)|publicseed|noiseseed)
void indcpa_keypair(unsigned char *pk, unsigned char *sk) {
    polyvec skpv;//vector of polynomials; k koeffizeinten;
    //skpv= secret key polynomial vector
    poly pkp; //polynom; Array mit 256 koeffizienten (signed) 
    //pkp=public key polynomial
    unsigned char buf[2 * KYBER_SYMBYTES];
    unsigned char *publicseed = buf;
    unsigned char *noiseseed = buf + KYBER_SYMBYTES;
    uint16_t i;
    unsigned char nonce = 0;   
    #if(Use_Deterministic_RNG==1) 
    randombytes(buf, KYBER_SYMBYTES);
    #else
    RNG.rand(buf, KYBER_SYMBYTES);
    #endif
    hash_g(buf, buf, KYBER_SYMBYTES); //IN,OUT,INPUTLEN
    
    for (i = 0; i < KYBER_K; i++){
        poly_getnoise_eta1(skpv.vec + i, noiseseed, nonce++);
    }

    sk_serialization(skpv,sk);
    polyvec_ntt(&skpv);//transfrom s  into ntt domain
    for (i = 0; i < KYBER_K; i++) {
        //generate matrix a on the fly and multiply with secret key; accumulate result; row by row;
        matacc(&pkp, &skpv, i, publicseed, 0);// generate a row of a in NTT domain and multiply with s (in NTT domain)
        poly_invntt(&pkp);//inverse ntt 
        poly_addnoise_eta1(&pkp, noiseseed, nonce++);//generate e polynomial and add noise to the calculated row
        poly_ntt(&pkp);//ntt
        poly_tobytes(pk+i*KYBER_POLYBYTES, &pkp);//barret reduce of polynomial and adding row of public matrix to public key       
    }
    memcpy(sk+KYBER_K*96,publicseed,KYBER_SYMBYTES);
    memcpy(sk+KYBER_K*96+KYBER_SYMBYTES,noiseseed,KYBER_SYMBYTES);
    memcpy(pk + KYBER_POLYVECBYTES, publicseed, KYBER_SYMBYTES); // Pack the public seed in the public key 
    clean(buf,2 * KYBER_SYMBYTES);
    clean(&skpv,sizeof(skpv));
    clean(&pkp,sizeof(pkp));
}
#endif

#if (SECRET_KEY_VARIANT==6) || (SECRET_KEY_VARIANT==7)
//sk=(d)
void indcpa_keypair(unsigned char *pk, unsigned char *sk) {
    polyvec skpv;//vector of polynomials; k koeffizeinten;
    //skpv= secret key polynomial vector
    poly pkp; //polynom; Array mit 256 koeffizienten (signed) 
    //pkp=public key polynomial
    unsigned char buf[2 * KYBER_SYMBYTES];
    unsigned char *publicseed = buf;
    unsigned char *noiseseed = buf + KYBER_SYMBYTES;
    int i;
    unsigned char nonce = 0;   
    #if(Use_Deterministic_RNG==1) 
    randombytes(buf, KYBER_SYMBYTES);
    #else
    RNG.rand(buf, KYBER_SYMBYTES);
    #endif
    memcpy(sk,buf,KYBER_SYMBYTES);
    hash_g(buf, buf, KYBER_SYMBYTES); //IN,OUT,INPUTLEN
    for (i = 0; i < KYBER_K; i++){
        poly_getnoise_eta1(skpv.vec + i, noiseseed, nonce++);
    }
    polyvec_ntt(&skpv);//transfrom s  into ntt domain
    for (i = 0; i < KYBER_K; i++) {
        //generate matrix a on the fly and multiply with secret key; accumulate result; row by row;
        matacc(&pkp, &skpv, i, publicseed, 0);// generate a row of a in NTT domain and multiply with s (in NTT domain)
        poly_invntt(&pkp);//inverse ntt 
        poly_addnoise_eta1(&pkp, noiseseed, nonce++);//generate e polynomial and add noise to the calculated row
        poly_ntt(&pkp);//ntt
        poly_tobytes(pk+i*KYBER_POLYBYTES, &pkp);//barret reduce of polynomial and adding row of public matrix to public key       
    }  
    memcpy(pk + KYBER_POLYVECBYTES, publicseed, KYBER_SYMBYTES); // Pack the public seed in the public key 
    clean(buf,2 * KYBER_SYMBYTES);
    clean(&skpv,sizeof(skpv));
    clean(&pkp,sizeof(pkp));
}
#endif

#if (SECRET_KEY_VARIANT==8)
//sk=(noiseseed|pk)
void indcpa_keypair(unsigned char *pk, unsigned char *sk) {
    polyvec skpv;//vector of polynomials; k koeffizeinten;
    //skpv= secret key polynomial vector
    poly pkp; //polynom; Array mit 256 koeffizienten (signed) 
    //pkp=public key polynomial
    unsigned char buf[2 * KYBER_SYMBYTES];
    unsigned char *publicseed = buf;
    unsigned char *noiseseed = buf + KYBER_SYMBYTES;
    uint16_t i;
    unsigned char nonce = 0;    
    #if(Use_Deterministic_RNG==1)
    randombytes(buf, KYBER_SYMBYTES);
    #else
    RNG.rand(buf, KYBER_SYMBYTES);
    #endif
    hash_g(buf, buf, KYBER_SYMBYTES); //IN,OUT,INPUTLEN
    for (i = 0; i < KYBER_K; i++){
        poly_getnoise_eta1(skpv.vec + i, noiseseed, nonce++);
    }
    polyvec_ntt(&skpv);//transfrom s  into ntt domain
    for (i = 0; i < KYBER_K; i++) {
        //generate matrix a on the fly and multiply with secret key; accumulate result; row by row;
        matacc(&pkp, &skpv, i, publicseed, 0);// generate a row of a in NTT domain and multiply with s (in NTT domain)
        poly_invntt(&pkp);//inverse ntt 
        poly_addnoise_eta1(&pkp, noiseseed, nonce++);//generate e polynomial and add noise to the calculated row
        poly_ntt(&pkp);//ntt
        poly_tobytes(pk+i*KYBER_POLYBYTES, &pkp);//barret reduce of polynomial and adding row of public matrix to public key       
    }
    memcpy(sk, noiseseed, KYBER_SYMBYTES);
    memcpy(pk + KYBER_POLYVECBYTES, publicseed, KYBER_SYMBYTES); // Pack the public seed in the public key
    clean(buf,2 * KYBER_SYMBYTES);
    clean(&skpv,sizeof(skpv));
    clean(&pkp,sizeof(pkp)); 
}
#endif

#if (SECRET_KEY_VARIANT==3) ||(SECRET_KEY_VARIANT==6) || (SECRET_KEY_VARIANT==7)
void genPkFromSeeds(unsigned char *out,const unsigned char *sk, const unsigned char *publicseed, const unsigned char *noiseseed){
    polyvec pkpv;
    polyvec skpv;
    uint8_t i;
    unsigned char nonce=KYBER_K;
    polyvec_frombytes(&skpv,sk);
    for (i = 0; i < KYBER_K; i++) {
        //generate matrix a on the fly and multiply with secret key; accumulate result; row by row;
        matacc(&pkpv.vec[i], &skpv, i, publicseed, 0);// generate a row of a in NTT domain and multiply with s (in NTT domain)
        poly_invntt(&pkpv.vec[i]);//inverse ntt 
        poly_addnoise_eta1(&pkpv.vec[i], noiseseed, nonce++);//generate e polynomial and add noise to the calculated row
        poly_ntt(&pkpv.vec[i]);//ntt
    }
    polyvec_tobytes(out,&pkpv);
    memcpy(out + KYBER_POLYVECBYTES, publicseed, KYBER_SYMBYTES);
}
#endif

#if SECRET_KEY_VARIANT==5
void genPkFromSeeds_and_dec(unsigned char *out,const unsigned char *sk, const unsigned char *publicseed, const unsigned char *noiseseed, unsigned char *m, const unsigned char *c){
    polyvec pkpv;
    polyvec skpv;
    sk_deserialization(&skpv,sk);
    polyvec_ntt(&skpv);
    indcpa_dec(m,c,&skpv);
    uint8_t i;
    unsigned char nonce=KYBER_K;
    //polyvec_frombytes(&skpv,sk_tmp);
    for (i = 0; i < KYBER_K; i++) {
        //generate matrix a on the fly and multiply with secret key; accumulate result; row by row;
        matacc(&pkpv.vec[i], &skpv, i, publicseed, 0);// generate a row of a in NTT domain and multiply with s (in NTT domain)
        poly_invntt(&pkpv.vec[i]);//inverse ntt 
        poly_addnoise_eta1(&pkpv.vec[i], noiseseed, nonce++);//generate e polynomial and add noise to the calculated row
        poly_ntt(&pkpv.vec[i]);//ntt
    }
    polyvec_tobytes(out,&pkpv);
    memcpy(out + KYBER_POLYVECBYTES, publicseed, KYBER_SYMBYTES);
}
#endif

#if (SECRET_KEY_VARIANT==6) || (SECRET_KEY_VARIANT==7)
void genSKFromSeed(unsigned char *sk, const unsigned char *noiseseed){
    int i;
    polyvec skpv;
    int nonce=0;
    for (i = 0; i < KYBER_K; i++){
        poly_getnoise_eta1(skpv.vec + i, noiseseed, nonce++);
    }
    polyvec_ntt(&skpv);//transfrom s  into ntt domain
    polyvec_tobytes(sk, &skpv);//encode secret key 
}
#endif

/*************************************************
* Name:        indcpa_enc
*
* Description: Encryption function of the CPA-secure
*              public-key encryption scheme underlying Kyber.
*
* Arguments:   - unsigned char *c:          pointer to output ciphertext (of length KYBER_INDCPA_BYTES bytes)
*              - const unsigned char *m:    pointer to input message (of length KYBER_INDCPA_MSGBYTES bytes)
*              - const unsigned char *pk:   pointer to input public key (of length KYBER_INDCPA_PUBLICKEYBYTES bytes)
*              - const unsigned char *coin: pointer to input random coins used as seed (of length KYBER_SYMBYTES bytes)
*                                           to deterministically generate all randomness
**************************************************/
void indcpa_enc(unsigned char *c, //output ciphertext
               const unsigned char *m,//message
               const unsigned char *pk,//public key (includes t(=A*s+e) and Rho as seed for A)
               const unsigned char *coins) {//random coins
    polyvec sp;//r; random polynomial to multiply with A
    poly bp;
    poly *pkp = &bp;
    poly *k = &bp;
    poly *v = &sp.vec[0];
    const unsigned char *seed = pk+KYBER_POLYVECBYTES;//Rho
    int i; //laufvariable für alle For-Schleifen
    unsigned char nonce = 0;//incremented nonce
    for (i = 0; i < KYBER_K; i++)
        poly_getnoise_eta1(sp.vec + i, coins, nonce++);//Generate r

    polyvec_ntt(&sp);//Transform r into NTT-Domain

    for (i = 0; i < KYBER_K; i++) {//u := A^T *r + e 1
        matacc(&bp, &sp, i, seed, 1);//use seed to gen A and mulitply it on the fly with r
        poly_invntt(&bp);//inverse NTT
        poly_addnoise_eta2(&bp, coins, nonce++);//add e1 => A*r+e1
        poly_reduce(&bp);//
        poly_packcompress(c, &bp, i);//compression of a polyvec, one poly at a time
    }

    
    poly_frombytes(pkp, pk);//unpack public key t (first polynomial;polybytes) and write to pkp (recycling poly storage)  
    poly_basemul(v, pkp, &sp.vec[0]); //t[0] * r[0]

    for (i = 1; i < KYBER_K; i++) {
        poly_frombytes(pkp, pk + i*KYBER_POLYBYTES);//unpack public key t poly per poly
        poly_basemul_acc(v, pkp, &sp.vec[i]);//v=t[i]+r[i]
    }

    poly_invntt(v);//inverse ntt v
    poly_addnoise_eta2(v, coins, nonce++);//v+e2
    poly_frommsg(k, m);//convert byte-message to poly
    poly_add(v, v, k);//v+m
    poly_reduce(v);
    poly_compress(c + KYBER_POLYVECCOMPRESSEDBYTES, v);//serialize and compress the rest of the cipher text; maybe change to positive standard representatives
    clean(&sp,sizeof(sp));
    clean(&bp,sizeof(bp));
}

/*************************************************
* Name:        indcpa_enc_cmp
*
* Description: Re-encryption function.
*              Compares the re-encypted ciphertext with the original ciphertext byte per byte.
*              The comparison is performed in a constant time manner.
*
*
* Arguments:   - unsigned char *ct:         pointer to input ciphertext to compare the new ciphertext with (of length KYBER_INDCPA_BYTES bytes)
*              - const unsigned char *m:    pointer to input message (of length KYBER_INDCPA_MSGBYTES bytes)
*              - const unsigned char *pk:   pointer to input public key (of length KYBER_INDCPA_PUBLICKEYBYTES bytes)
*              - const unsigned char *coin: pointer to input random coins used as seed (of length KYBER_SYMBYTES bytes)
*                                           to deterministically generate all randomness
* Returns:     - boolean byte indicating that re-encrypted ciphertext is NOT equal to the original ciphertext
**************************************************/

#if (SECRET_KEY_VARIANT !=1)
unsigned char indcpa_enc_cmp(const unsigned char *c,
                             const unsigned char *m,
                             const unsigned char *pk,
                             const unsigned char *coins) {
    uint64_t rc = 0;
    polyvec sp;
    poly bp;
    poly *pkp = &bp;
    poly *k = &bp;
    poly *v = &sp.vec[0];
    const unsigned char *seed = pk+KYBER_POLYVECBYTES;
    int i;
    unsigned char nonce = 0;

    for (i = 0; i < KYBER_K; i++)
        poly_getnoise_eta1(sp.vec + i, coins, nonce++);

    polyvec_ntt(&sp);

    for (i = 0; i < KYBER_K; i++) {
        matacc(&bp, &sp, i, seed, 1);
        poly_invntt(&bp);
        poly_addnoise_eta2(&bp, coins, nonce++);
        poly_reduce(&bp);

        rc |= cmp_poly_packcompress(c, &bp, i);
    }

    poly_frombytes(pkp, pk);
    poly_basemul(v, pkp, &sp.vec[0]);

    for (i = 1; i < KYBER_K; i++) {
        poly_frombytes(pkp, pk + i*KYBER_POLYBYTES);
        poly_basemul_acc(v, pkp, &sp.vec[i]);
    }

    poly_invntt(v);
    poly_addnoise_eta2(v, coins, nonce++);
    poly_frommsg(k, m);
    poly_add(v, v, k);
    poly_reduce(v);

    rc |= cmp_poly_compress(c + KYBER_POLYVECCOMPRESSEDBYTES, v);

    rc = ~rc + 1;
    rc >>= 63;
    return (unsigned char)rc;
}
#else
unsigned char indcpa_enc_cmp(const unsigned char *c,
                             const unsigned char *m,
                             const unsigned char *pk,
                             const unsigned char *coins,
                             const unsigned char *a
                             ) { 
    uint64_t rc = 0;
    polyvec sp;
    poly bp;
    poly *pkp = &bp;
    poly *k = &bp;
    poly *v = &sp.vec[0];
    //const unsigned char *seed = pk+KYBER_POLYVECBYTES;
    int i;
    unsigned char nonce = 0;

    for (i = 0; i < KYBER_K; i++)
        poly_getnoise_eta1(sp.vec + i, coins, nonce++);

    polyvec_ntt(&sp);
    int counter=0;
    for (i = 0; i < KYBER_K; i++) {
        for (int j = 0; j < KYBER_K; j++)
        {   
            if(j==0){
                poly_frombytes_mul(&bp,&sp.vec[j],a+counter*KYBER_POLYBYTES);
            }else{
                poly_frombytes_mul_acc(&bp,&sp.vec[j],a+counter*KYBER_POLYBYTES);
            }
            counter+=KYBER_K;
        }
        counter=i+1;
        poly_invntt(&bp);
        poly_addnoise_eta2(&bp, coins, nonce++);
        poly_reduce(&bp);

        rc |= cmp_poly_packcompress(c, &bp, i);
    }

    poly_frombytes(pkp, pk);
    poly_basemul(v, pkp, &sp.vec[0]);

    for (i = 1; i < KYBER_K; i++) {
        poly_frombytes(pkp, pk + i*KYBER_POLYBYTES);
        poly_basemul_acc(v, pkp, &sp.vec[i]);
    }

    poly_invntt(v);
    poly_addnoise_eta2(v, coins, nonce++);
    poly_frommsg(k, m);
    poly_add(v, v, k);
    poly_reduce(v);
    rc=0;
    rc |= cmp_poly_compress(c + KYBER_POLYVECCOMPRESSEDBYTES, v);
    rc = ~rc + 1;
    rc >>= 63;
    return (unsigned char)rc;
}
#endif
#if (SECRET_KEY_VARIANT != 5) && (SECRET_KEY_VARIANT != 8) && (SECRET_KEY_VARIANT!=4)
/*************************************************
* Name:        indcpa_dec
*
* Description: Decryption function of the CPA-secure
*              public-key encryption scheme underlying Kyber.
*
* Arguments:   - unsigned char *m:        pointer to output decrypted message (of length KYBER_INDCPA_MSGBYTES)
*              - const unsigned char *c:  pointer to input ciphertext (of length KYBER_INDCPA_BYTES)
*              - const unsigned char *sk: pointer to input secret key (of length KYBER_INDCPA_SECRETKEYBYTES)
**************************************************/
void  indcpa_dec(unsigned char *m,const unsigned char *c,const unsigned char *sk) {
    poly mp, bp;
    poly *v = &bp;
    int i;

    poly_unpackdecompress(&mp, c, 0);//korrekt
    poly_ntt(&mp);  
    poly_frombytes_mul(&mp, &mp, sk);
    for(i = 1; i < KYBER_K; i++) {
        poly_unpackdecompress(&bp, c, i);//korrekt
        poly_ntt(&bp);
        poly_frombytes_mul_acc(&mp, &bp, sk + i*KYBER_POLYBYTES);
    }    
    poly_invntt(&mp);    
    poly_decompress(v, c+KYBER_POLYVECCOMPRESSEDBYTES);
    poly_sub(&mp, v, &mp);
    poly_reduce(&mp);
    poly_tomsg(m, &mp);
    clean(&mp,sizeof(mp));
    clean(&bp,sizeof(bp));
}
#endif

#if (SECRET_KEY_VARIANT == 5) 
//sk' in skpv form
void  indcpa_dec(unsigned char *m,const unsigned char *c,const polyvec *sk) {
    poly mp, bp;
    poly *v = &bp;
    int i;

    poly_unpackdecompress(&mp, c, 0);//korrekt
    poly_ntt(&mp);  
    poly_basemul(&mp,sk->vec,&mp);
    for(i = 1; i < KYBER_K; i++) {
        poly_unpackdecompress(&bp, c, i);//korrekt
        poly_ntt(&bp);
        poly_basemul_acc(&mp,&bp,sk->vec+i);
    }    
    poly_invntt(&mp);    
    poly_decompress(v, c+KYBER_POLYVECCOMPRESSEDBYTES);
    poly_sub(&mp, v, &mp);
    poly_reduce(&mp);
    poly_tomsg(m, &mp);
    clean(&mp,sizeof(mp));
    clean(&bp,sizeof(bp));
}
#endif

#if (SECRET_KEY_VARIANT==4)
//sk' in serialized/compressed form
void  indcpa_dec(unsigned char *m,const unsigned char *c,const unsigned char *sk) {
    poly mp, bp;
    poly *v = &bp;
    int i;
    polyvec skpv;

    sk_deserialization(&skpv,sk);//korrekt
    poly_unpackdecompress(&mp, c, 0);//korrekt
    poly_ntt(&mp);  
    polyvec_ntt(&skpv);//korrekt
    poly_basemul(&mp,&skpv.vec[0],&mp);
    for(i = 1; i < KYBER_K; i++) {
        poly_unpackdecompress(&bp, c, i);//korrekt
        poly_ntt(&bp);
        poly_basemul_acc(&mp,&bp,&skpv.vec[i]);
    }    
    poly_invntt(&mp);    
    poly_decompress(v, c+KYBER_POLYVECCOMPRESSEDBYTES);
    poly_sub(&mp, v, &mp);
    poly_reduce(&mp);
    poly_tomsg(m, &mp);
    clean(&mp,sizeof(mp));
    clean(&bp,sizeof(bp));
}
#endif

#if (SECRET_KEY_VARIANT == 8)
//noiseseed as sk
void  indcpa_dec(unsigned char *m,const unsigned char *c,const unsigned char *sk) {
    polyvec skpv;
    poly mp, bp;
    poly *v = &bp;
    int i;
    int nonce=0;

    for (i = 0; i < KYBER_K; i++){
        poly_getnoise_eta1(skpv.vec + i, sk, nonce++);
    }
    polyvec_ntt(&skpv);//transfrom s  into ntt domain
    poly_unpackdecompress(&mp, c, 0);//korrekt
    poly_ntt(&mp);  
    poly_basemul(&mp,&skpv.vec[0],&mp);
    for(i = 1; i < KYBER_K; i++) {
        poly_unpackdecompress(&bp, c, i);//korrekt
        poly_ntt(&bp);
        poly_basemul_acc(&mp,&bp,&skpv.vec[i]);
    }    
    poly_invntt(&mp);    
    poly_decompress(v, c+KYBER_POLYVECCOMPRESSEDBYTES);
    poly_sub(&mp, v, &mp);
    poly_reduce(&mp);
    poly_tomsg(m, &mp);
    clean(&mp,sizeof(mp));
    clean(&bp,sizeof(bp));
}
#endif


