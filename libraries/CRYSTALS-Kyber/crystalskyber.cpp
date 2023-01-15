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
#include "crystalskyber.h"
#include "indcpa.h"
#include "params.h"
#include "symmetric.h"
#include "verify.h"
#include <stdlib.h>
#include "polyvec.h"
#include <string.h>
#include <Crypto.h>
#if (Use_Deterministic_RNG==1)
#include "randombytes.h"
#else
#include <RNG.h>
#endif

#if (SECRET_KEY_VARIANT==0)
/*************************************************
* Name:        crypto_kem_keypair
*
* Description: Generates public and private key
*              for CCA-secure Kyber key encapsulation mechanism
*
* Arguments:   - unsigned char *pk: pointer to output public key (an already allocated array of CRYPTO_PUBLICKEYBYTES bytes)
*              - unsigned char *sk: pointer to output private key (an already allocated array of CRYPTO_SECRETKEYBYTES bytes)
*
* Returns 0 (success)
**************************************************/
 int CrystalsKyber::crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
    size_t i;
    indcpa_keypair(pk, sk);
    for (i = 0; i < KYBER_INDCPA_PUBLICKEYBYTES; i++) {
        sk[i + KYBER_INDCPA_SECRETKEYBYTES] = pk[i];
    }
    hash_h(sk + KYBER_SECRETKEYBYTES - 2 * KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES);
    #if (Use_Deterministic_RNG==1)
    randombytes(sk + KYBER_SECRETKEYBYTES - KYBER_SYMBYTES, KYBER_SYMBYTES);    /* Value z for pseudo-random output on reject */
    #else
    RNG.rand(sk + KYBER_SECRETKEYBYTES - KYBER_SYMBYTES, KYBER_SYMBYTES);
    #endif
    return 0;
}

/*************************************************
* Name:        crypto_kem_dec
*
* Description: Generates shared secret for given
*              cipher text and private key
*
* Arguments:   - unsigned char *ss:       pointer to output shared secret (an already allocated array of CRYPTO_BYTES bytes)
*              - const unsigned char *ct: pointer to input cipher text (an already allocated array of CRYPTO_CIPHERTEXTBYTES bytes)
*              - const unsigned char *sk: pointer to input private key (an already allocated array of CRYPTO_SECRETKEYBYTES bytes)
*
* Returns 0.
*
* On failure, ss will contain a pseudo-random value.
**************************************************/
int CrystalsKyber::crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk) {
    size_t i;
    unsigned char fail;
    unsigned char buf[2 * KYBER_SYMBYTES];
    unsigned char kr[2 * KYBER_SYMBYTES];                                             /* Will contain key, coins */
    const unsigned char *pk = sk + KYBER_INDCPA_SECRETKEYBYTES;

    indcpa_dec(buf, ct, sk);

    for (i = 0; i < KYBER_SYMBYTES; i++) {                                            /* Multitarget countermeasure for coins + contributory KEM */
        buf[KYBER_SYMBYTES + i] = sk[KYBER_SECRETKEYBYTES - 2 * KYBER_SYMBYTES + i];  /* Save hash by storing H(pk) in sk */
    }
    hash_g(kr, buf, 2 * KYBER_SYMBYTES);
    fail =indcpa_enc_cmp(ct, buf, pk, kr + KYBER_SYMBYTES);                          /* coins are in kr+KYBER_SYMBYTES */

    hash_h(kr + KYBER_SYMBYTES, ct, KYBER_CIPHERTEXTBYTES);                           /* overwrite coins in kr with H(c)  */

    cmov(kr, sk + KYBER_SECRETKEYBYTES - KYBER_SYMBYTES, KYBER_SYMBYTES, fail);       /* Overwrite pre-k with z on re-encryption failure */

    kdf(ss, kr, 2 * KYBER_SYMBYTES);                                                  /* hash concatenation of pre-k and H(c) to k */
    clean(&fail,1);
    clean(buf,2 * KYBER_SYMBYTES);
    clean(kr,2 * KYBER_SYMBYTES);
    return 0;
}

#endif

/*************************************************
* Name:        crypto_kem_enc
*
* Description: Generates cipher text and shared
*              secret for given public key
*
* Arguments:   - unsigned char *ct:       pointer to output cipher text (an already allocated array of CRYPTO_CIPHERTEXTBYTES bytes)
*              - unsigned char *ss:       pointer to output shared secret (an already allocated array of CRYPTO_BYTES bytes)
*              - const unsigned char *pk: pointer to input public key (an already allocated array of CRYPTO_PUBLICKEYBYTES bytes)
*
* Returns 0 (success)
**************************************************/
int CrystalsKyber::crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *ppk) {
    unsigned char  kr[2 * KYBER_SYMBYTES];                                   /* Will contain key, coins */
    unsigned char buf[2 * KYBER_SYMBYTES];
    #if (Use_Deterministic_RNG==1)
    randombytes(buf, KYBER_SYMBYTES);  
    #else
    RNG.rand(buf, KYBER_SYMBYTES);
    #endif
    hash_h(buf, buf, KYBER_SYMBYTES);                                        /* Don't release system RNG output */
    hash_h(buf + KYBER_SYMBYTES, ppk, KYBER_PUBLICKEYBYTES);                  /* Multitarget countermeasure for coins + contributory KEM */
    hash_g(kr, buf, 2 * KYBER_SYMBYTES);
    indcpa_enc(ct, buf, ppk, kr + KYBER_SYMBYTES);                            /* coins are in kr+KYBER_SYMBYTES */
    hash_h(kr + KYBER_SYMBYTES, ct, KYBER_CIPHERTEXTBYTES);                  /* overwrite coins in kr with H(c) */
    kdf(ss, kr, 2 * KYBER_SYMBYTES);                                         /* hash concatenation of pre-k and H(c) to k */
    clean(buf,2 * KYBER_SYMBYTES);
    clean(kr,2 * KYBER_SYMBYTES);
    return 0;
}


/********************************Beginning Variants with reduced sk size**************/

//KeyGen Variants

#if (SECRET_KEY_VARIANT==1)
//sk=(sk'|A|pk|z|H(pk))
int CrystalsKyber::crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
    int i;
    indcpa_keypair(pk, sk);
    for (i = 0; i < KYBER_INDCPA_PUBLICKEYBYTES; i++) {
        sk[i+KYBER_POLYVECBYTES+KYBER_K*KYBER_POLYVECBYTES] = pk[i];
    }
    #if (Use_Deterministic_RNG==1)
    randombytes(sk + KYBER_POLYVECBYTES+KYBER_K*KYBER_POLYVECBYTES+KYBER_INDCPA_PUBLICKEYBYTES, KYBER_SYMBYTES);  
    #else
    RNG.rand(sk + KYBER_POLYVECBYTES+KYBER_K*KYBER_POLYVECBYTES+KYBER_INDCPA_PUBLICKEYBYTES, KYBER_SYMBYTES);
    #endif
    hash_h(sk + KYBER_POLYVECBYTES+KYBER_K*KYBER_POLYVECBYTES+KYBER_INDCPA_PUBLICKEYBYTES + KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES);
    return 0;
}

int CrystalsKyber::crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk) {
    unsigned char fail;
    unsigned char buf[2 * KYBER_SYMBYTES];
    unsigned char kr[2 * KYBER_SYMBYTES];                                             /* Will contain key, coins */
    const unsigned char *pk = sk + KYBER_POLYVECBYTES+KYBER_K*KYBER_POLYVECBYTES;

    indcpa_dec(buf, ct, sk);
    memcpy(buf + KYBER_SYMBYTES,sk + KYBER_POLYVECBYTES+KYBER_K*KYBER_POLYVECBYTES+KYBER_INDCPA_PUBLICKEYBYTES + KYBER_SYMBYTES , KYBER_SYMBYTES);

    hash_g(kr, buf, 2 * KYBER_SYMBYTES);
    
    fail =indcpa_enc_cmp(ct, buf, pk, kr + KYBER_SYMBYTES,sk+KYBER_POLYVECBYTES);                          /* coins are in kr+KYBER_SYMBYTES */

    hash_h(kr + KYBER_SYMBYTES, ct, KYBER_CIPHERTEXTBYTES);                           /* overwrite coins in kr with H(c)  */

    cmov(kr, sk + KYBER_POLYVECBYTES+KYBER_K*KYBER_POLYVECBYTES+KYBER_INDCPA_PUBLICKEYBYTES, KYBER_SYMBYTES, fail);       /* Overwrite pre-k with z on re-encryption failure */

    kdf(ss, kr, 2 * KYBER_SYMBYTES);                                                  /* hash concatenation of pre-k and H(c) to k */
    clean(&fail,1);
    clean(buf,2 * KYBER_SYMBYTES);
    clean(kr,2 * KYBER_SYMBYTES);
    return 0;
}
#endif
#if (SECRET_KEY_VARIANT==2)
//leaving out H(pk)
//sk=(sk'|pk|z)
int CrystalsKyber::crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
    int i;
    indcpa_keypair(pk, sk);
    for (i = 0; i < KYBER_INDCPA_PUBLICKEYBYTES; i++) {
        sk[i + KYBER_INDCPA_SECRETKEYBYTES] = pk[i];
    }
    #if (Use_Deterministic_RNG==1)
    randombytes(sk + KYBER_INDCPA_SECRETKEYBYTES+KYBER_INDCPA_PUBLICKEYBYTES, KYBER_SYMBYTES);  
    #else
    RNG.rand(sk + KYBER_INDCPA_SECRETKEYBYTES+KYBER_INDCPA_PUBLICKEYBYTES, KYBER_SYMBYTES);
    #endif
    return 0;
}

//recompute H(pk)
int CrystalsKyber::crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk) {
    unsigned char fail;
    unsigned char buf[2 * KYBER_SYMBYTES];
    unsigned char kr[2 * KYBER_SYMBYTES];                                             /* Will contain key, coins */
    const unsigned char *pk = sk + KYBER_INDCPA_SECRETKEYBYTES;

    indcpa_dec(buf, ct, sk);

    hash_h(buf + KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES);

    hash_g(kr, buf, 2 * KYBER_SYMBYTES);  

    fail = indcpa_enc_cmp(ct, buf, pk, kr + KYBER_SYMBYTES);                          /* coins are in kr+KYBER_SYMBYTES */
    
    hash_h(kr + KYBER_SYMBYTES, ct, KYBER_CIPHERTEXTBYTES);                           /* overwrite coins in kr with H(c)  */
    
    cmov(kr, sk + KYBER_INDCPA_SECRETKEYBYTES  + KYBER_INDCPA_PUBLICKEYBYTES, KYBER_SYMBYTES, fail); 
    
    kdf(ss, kr, 2 * KYBER_SYMBYTES);                                                  /* hash concatenation of pre-k and H(c) to k */
    clean(&fail,1);
    clean(buf,2 * KYBER_SYMBYTES);
    clean(kr,2 * KYBER_SYMBYTES);
    return 0;
}
#endif

#if (SECRET_KEY_VARIANT==3)
//sk=(sk'|noiseseed|publicseed|z)
int CrystalsKyber::crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
    indcpa_keypair(pk, sk);
    #if (Use_Deterministic_RNG==1)
    randombytes(sk + KYBER_INDCPA_SECRETKEYBYTES + 2 * KYBER_SYMBYTES, KYBER_SYMBYTES);  
    #else
    RNG.rand(sk + KYBER_INDCPA_SECRETKEYBYTES + 2 * KYBER_SYMBYTES, KYBER_SYMBYTES);
    #endif
    return 0;
}

//sk=(sk'|publicseed|noiseseed|z)
int CrystalsKyber::crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk) {
    unsigned char fail;
    unsigned char buf[2 * KYBER_SYMBYTES];
    unsigned char kr[2 * KYBER_SYMBYTES];                                             /* Will contain key, coins */
    const unsigned char *publicseed = sk + KYBER_POLYVECBYTES;
    const unsigned char *noiseseed = sk + KYBER_POLYVECBYTES + KYBER_SYMBYTES;
    unsigned char pk[KYBER_INDCPA_PUBLICKEYBYTES];
    indcpa_dec(buf, ct, sk);
    genPkFromSeeds(pk,sk,publicseed,noiseseed);
    hash_h(buf + KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES);

    hash_g(kr, buf, 2 * KYBER_SYMBYTES);  

    fail = indcpa_enc_cmp(ct, buf, pk, kr + KYBER_SYMBYTES);                          /* coins are in kr+KYBER_SYMBYTES */
    
    hash_h(kr + KYBER_SYMBYTES, ct, KYBER_CIPHERTEXTBYTES);                           /* overwrite coins in kr with H(c)  */
    cmov(kr, sk+KYBER_INDCPA_SECRETKEYBYTES+ 2 * KYBER_SYMBYTES, KYBER_SYMBYTES, fail); 
    
    kdf(ss, kr, 2 * KYBER_SYMBYTES);                                                  /* hash concatenation of pre-k and H(c) to k */
    clean(&fail,1);
    clean(buf,2 * KYBER_SYMBYTES);
    clean(kr,2 * KYBER_SYMBYTES);
    return 0;
}
#endif

#if (SECRET_KEY_VARIANT==5)
//sk=(compressed(sk')|pk|z)
int CrystalsKyber::crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
    indcpa_keypair(pk, sk);
    #if (Use_Deterministic_RNG==1)
    randombytes(sk  + (KYBER_K * 96) + 2*KYBER_SYMBYTES , KYBER_SYMBYTES);  
    #else
    RNG.rand(sk +(KYBER_K * 96) + 2*KYBER_SYMBYTES , KYBER_SYMBYTES);
    #endif
    return 0;
}


//enc_cmp is not edited
int CrystalsKyber::crypto_kem_dec(unsigned char *ss, const unsigned char *ct,const unsigned char *sk) {
    unsigned char fail;
    unsigned char buf[2 * KYBER_SYMBYTES];
    unsigned char kr[2 * KYBER_SYMBYTES];                                             /* Will contain key, coins */
    const unsigned char *publicseed = sk + KYBER_K * 96;
    const unsigned char *noiseseed = sk + KYBER_K * 96 + KYBER_SYMBYTES;
    unsigned char pk[KYBER_INDCPA_PUBLICKEYBYTES];

    genPkFromSeeds_and_dec(pk,sk,publicseed,noiseseed,buf,ct);
    hash_h(buf + KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES);
    
    hash_g(kr, buf, 2 * KYBER_SYMBYTES);  

    fail =indcpa_enc_cmp(ct, buf, pk, kr + KYBER_SYMBYTES);                          /* coins are in kr+KYBER_SYMBYTES */
    
    hash_h(kr + KYBER_SYMBYTES, ct, KYBER_CIPHERTEXTBYTES);                           /* overwrite coins in kr with H(c)  */
    cmov(kr,sk + (KYBER_K * 96) + 2*KYBER_SYMBYTES, KYBER_SYMBYTES, fail); 
    
    kdf(ss, kr, 2 * KYBER_SYMBYTES);                                                  /* hash concatenation of pre-k and H(c) to k */
    clean(&fail,1);
    clean(buf,2 * KYBER_SYMBYTES);
    clean(kr,2 * KYBER_SYMBYTES);
    return 0;
}
#endif
#if (SECRET_KEY_VARIANT==4)
//sk=(compressed(sk')|pk|z)
int CrystalsKyber::crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
    indcpa_keypair(pk, sk);
    for (int i = 0; i < KYBER_INDCPA_PUBLICKEYBYTES; i++) {
        sk[i + KYBER_K*96] = pk[i];
    }
    #if (Use_Deterministic_RNG==1)
    randombytes(sk + (KYBER_K * 96) + KYBER_INDCPA_PUBLICKEYBYTES , KYBER_SYMBYTES);  
    #else
    RNG.rand(sk + (KYBER_K * 96) + KYBER_INDCPA_PUBLICKEYBYTES , KYBER_SYMBYTES);
    #endif
    return 0;
}

int CrystalsKyber::crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk) {
    unsigned char fail;
    unsigned char buf[2 * KYBER_SYMBYTES];
    unsigned char kr[2 * KYBER_SYMBYTES];                                             /* Will contain key, coins */
    const unsigned char *pk = sk + KYBER_K*96;

    indcpa_dec(buf, ct, sk);

    hash_h(buf + KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES);
    
    hash_g(kr, buf, 2 * KYBER_SYMBYTES);  

    fail =indcpa_enc_cmp(ct, buf, pk, kr + KYBER_SYMBYTES);                          /* coins are in kr+KYBER_SYMBYTES */
    hash_h(kr + KYBER_SYMBYTES, ct, KYBER_CIPHERTEXTBYTES);                           /* overwrite coins in kr with H(c)  */
    cmov(kr, sk + (KYBER_K * 96) + KYBER_INDCPA_PUBLICKEYBYTES, KYBER_SYMBYTES, fail); 
    
    kdf(ss, kr, 2 * KYBER_SYMBYTES);                                                  /* hash concatenation of pre-k and H(c) to k */
    clean(&fail,1);
    clean(buf,2 * KYBER_SYMBYTES);
    clean(kr,2 * KYBER_SYMBYTES);
    return 0;
}
#endif
#if (SECRET_KEY_VARIANT==6)
//sk=(d)
int CrystalsKyber::crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
    indcpa_keypair(pk, sk);
    return 0;
}


//sk=(d)
int CrystalsKyber::crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk) {
    unsigned char skpk[KYBER_PUBLICKEYBYTES];
    unsigned char fail;
    unsigned char buf[3 * KYBER_SYMBYTES];
    unsigned char kr[2 * KYBER_SYMBYTES]; /* Will contain key, coins */
    hash_g(buf+KYBER_SYMBYTES, sk, KYBER_SYMBYTES); //generate public[1] and noise[2] seed out of d ans save in buf
    genSKFromSeed(skpk,buf+ 2*KYBER_SYMBYTES); //gen sk polynomial vektor out of d
    indcpa_dec(buf, ct, skpk);//decapsulation of ct and storing the secret into buf[0]
    genPkFromSeeds(skpk,skpk,buf+KYBER_SYMBYTES,buf+2*KYBER_SYMBYTES);
    hash_h(buf + KYBER_SYMBYTES, skpk, KYBER_PUBLICKEYBYTES);//save H(pk) in buf[1]
    hash_g(kr, buf, 2 * KYBER_SYMBYTES);//G(m'|H(pk))-->hashg(buf[0],buf[1])
    fail =indcpa_enc_cmp(ct, buf, skpk, kr + KYBER_SYMBYTES);             /* coins are in kr+KYBER_SYMBYTES */
    hash_h(kr + KYBER_SYMBYTES, ct, KYBER_CIPHERTEXTBYTES);               /* overwrite coins in kr with H(c)  */
    hash_h(buf,sk,KYBER_SYMBYTES);
    cmov(kr, buf, KYBER_SYMBYTES, fail);       /* Overwrite pre-k with z on re-encryption failure */
    kdf(ss, kr, 2 * KYBER_SYMBYTES);                /* hash concatenation of pre-k and H(c) to k */
    clean(&fail,1);
    clean(buf,3 * KYBER_SYMBYTES);
    clean(kr,2 * KYBER_SYMBYTES);
    return 0;
}
#endif
#if (SECRET_KEY_VARIANT==7)
//sk=(d|z)
int CrystalsKyber::crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
    indcpa_keypair(pk, sk);
    #if (Use_Deterministic_RNG==1)
    randombytes(sk + 32 , KYBER_SYMBYTES);  
    #else
    RNG.rand(sk + 32 , KYBER_SYMBYTES);
    #endif
    return 0;
}

int CrystalsKyber::crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk) {
    unsigned char skpk[KYBER_PUBLICKEYBYTES];
    unsigned char fail;
    unsigned char buf[3 * KYBER_SYMBYTES];
    unsigned char kr[2 * KYBER_SYMBYTES]; /* Will contain key, coins */

    hash_g(buf+KYBER_SYMBYTES, sk, KYBER_SYMBYTES); //generate public[1] and noise[2] seed out of d ans save in buf

    genSKFromSeed(skpk,buf+ 2*KYBER_SYMBYTES); //gen sk polynomial vektor out of d

    indcpa_dec(buf, ct, skpk);//decapsulation of ct and storing the secret into buf[0]

    genPkFromSeeds(skpk,skpk,buf+KYBER_SYMBYTES,buf+2*KYBER_SYMBYTES);

    hash_h(buf + KYBER_SYMBYTES, skpk, KYBER_PUBLICKEYBYTES);//save H(pk) in buf[1]

    hash_g(kr, buf, 2 * KYBER_SYMBYTES);//G(m'|H(pk))-->hashg(buf[0],buf[1])

    fail =indcpa_enc_cmp(ct, buf, skpk, kr + KYBER_SYMBYTES);             /* coins are in kr+KYBER_SYMBYTES */

    hash_h(kr + KYBER_SYMBYTES, ct, KYBER_CIPHERTEXTBYTES);               /* overwrite coins in kr with H(c)  */

    cmov(kr, sk+32 , KYBER_SYMBYTES, fail);       /* Overwrite pre-k with z on re-encryption failure */

    kdf(ss, kr, 2 * KYBER_SYMBYTES);                /* hash concatenation of pre-k and H(c) to k */
    clean(&fail,1);
    clean(buf,3 * KYBER_SYMBYTES);
    clean(kr,2 * KYBER_SYMBYTES);
    return 0;
}
#endif
#if (SECRET_KEY_VARIANT==8)
//sk=noiseseed
int CrystalsKyber::crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
    indcpa_keypair(pk, sk);
    for (int i = 0; i < KYBER_INDCPA_PUBLICKEYBYTES; i++) {
        sk[i +32] = pk[i];
    }
    #if (Use_Deterministic_RNG==1)
    randombytes(sk + 32+ KYBER_INDCPA_PUBLICKEYBYTES , KYBER_SYMBYTES);  
    #else
    RNG.rand(sk + 32+ KYBER_INDCPA_PUBLICKEYBYTES , KYBER_SYMBYTES);
    #endif
    return 0;
}

// variante 8, also die Lösung
int CrystalsKyber::crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk) {
    unsigned char fail;
    unsigned char buf[2 * KYBER_SYMBYTES];
    unsigned char kr[2 * KYBER_SYMBYTES];                                             /* Will contain key, coins */
    const unsigned char *pk = sk + 32;

    indcpa_dec(buf, ct, sk);

    hash_h(buf + KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES);
    
    hash_g(kr, buf, 2 * KYBER_SYMBYTES);  

    fail =indcpa_enc_cmp(ct, buf, pk, kr + KYBER_SYMBYTES);                          /* coins are in kr+KYBER_SYMBYTES */
    hash_h(kr + KYBER_SYMBYTES, ct, KYBER_CIPHERTEXTBYTES);                           /* overwrite coins in kr with H(c)  */
    cmov(kr, sk + 32+ KYBER_INDCPA_PUBLICKEYBYTES, KYBER_SYMBYTES, fail); 
    kdf(ss, kr, 2 * KYBER_SYMBYTES);                                                  /* hash concatenation of pre-k and H(c) to k */
    clean(&fail,1);
    clean(buf,2 * KYBER_SYMBYTES);
    clean(kr,2 * KYBER_SYMBYTES);
    return 0;
}
#endif












