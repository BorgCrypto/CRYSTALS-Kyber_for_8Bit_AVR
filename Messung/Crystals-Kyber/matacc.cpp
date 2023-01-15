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
#include "matacc.h"



/*************************************************
* Name:        matacc
*
* Description: Multiplies a row of A or A^T, generated on-the-fly,
*              with a vector of polynomials and accumulates into the result.
*
* Arguments:   - poly *r:                    pointer to output polynomial to accumulate in
*              - polyvec *b:                 pointer to input vector of polynomials to multiply with
*              - unsigned char i:            byte to indicate the index < KYBER_K of the row of A or A^T
*              - const unsigned char *seed:  pointer to the public seed used to generate A
*              - int transposed:             boolean indicatin whether A or A^T is generated
**************************************************/
void matacc(poly* r, const polyvec *b, unsigned char i, const unsigned char *seed, int transposed) {
  //Ergebnis ist nur ein Polynom!
  unsigned char buf[XOF_BLOCKBYTES+2];
  xof_state state;
  int16_t c[4];
  int j = 0;

  if (transposed){
    xof_absorb(&state, seed, i, j);
  }else{
    xof_absorb(&state, seed, j, i); 
  }
  xof_squeezeblocks(buf, 1, &state);
  matacc_asm(r->coeffs, b->vec[j].coeffs, c, buf, zetas, &state);

  for(j=1;j<KYBER_K;j++) {
    if (transposed)
      xof_absorb(&state, seed, i, j);
    else
      xof_absorb(&state, seed, j, i);

    xof_squeezeblocks(buf, 1, &state);
    matacc_asm_acc(r->coeffs, b->vec[j].coeffs, c, buf, zetas, &state);
  } 
}

void matacc_asm(int16_t *coeffs, const int16_t *b, int16_t c[4], unsigned char buf[XOF_BLOCKBYTES+2], const int16_t zeta[128], xof_state *state){
    
    //b ist ein polynom
    //Das Ergebnis (r) soll auch nur ein Polynom sein
    //c wird in jedem durchlauf mit koeffizienten gefüllt, bis 4 vorhanden sind; dann werden diese 4 koeffizenten mit den entsprechenden aus dem polynom des sk multipliziert.
    //dadurch entstehen 4 koeefizienten für das "ergebnispolynom"; Wenn das Polynom 256 Koeffizenten erreicht hat, dann wird es in den Ergebnis vektor eingefügt.
    uint16_t val0, val1,ctr=0, pos=0,buflen=XOF_BLOCKBYTES,k=0;
    while (ctr<(KYBER_N/4)){
      val0 = ((buf[pos+0] >> 0) | ((uint16_t)buf[pos+1] << 8)) & 0xFFF;
      val1 = ((buf[pos+1] >> 4) | ((uint16_t)buf[pos+2] << 4)) & 0xFFF;
      pos+=3;

      if(val0<KYBER_Q){
        c[k]=val0;
        k++;
        if(k==4){
          basemul(&coeffs[ctr*4],&c[0],&b[ctr*4],zeta[64+ctr]);
          basemul(&coeffs[ctr*4+2],&c[2],&b[ctr*4+2],-zeta[64+ctr]);
          ctr+=1;
          k=0;
        }
      }

      if(val1<KYBER_Q && ctr < KYBER_N/4){
        c[k]=val1;
        k++;
        if(k==4){
          basemul(&coeffs[ctr*4],&c[0],&b[ctr*4],zeta[64+ctr]);
          basemul(&coeffs[ctr*4+2],&c[2],&b[ctr*4+2],-zeta[64+ctr]);
          ctr+=1;
          k=0;
        }
      }

      if(pos+3 > buflen && ctr < KYBER_N/4){
          xof_squeezeblocks(buf, 1, state);
          pos=0;
      }
      
    }   
}

void matacc_asm_acc(int16_t *coeffs, const int16_t *b, int16_t c[4], unsigned char buf[XOF_BLOCKBYTES+2], const int16_t zeta[128], xof_state *state){
   uint16_t val0=0, val1=0,ctr=0, pos=0,buflen=XOF_BLOCKBYTES,k=0;
   int16_t temp[2];
    while (ctr<(KYBER_N/4)){
      val0 = ((buf[pos+0] >> 0) | ((uint16_t)buf[pos+1] << 8)) & 0xFFF;
      val1 = ((buf[pos+1] >> 4) | ((uint16_t)buf[pos+2] << 4)) & 0xFFF;
      pos+=3;
      if(val0<KYBER_Q){
        c[k]=val0;
        k++;
        if(k==4){
          basemul(&temp[0],&b[ctr*4],&c[0],zeta[64+ctr]);
          poly_add_pointwise(&coeffs[ctr*4],&coeffs[ctr*4],&temp[0]);
          basemul(&temp[0],&b[ctr*4+2],&c[2],-zeta[64+ctr]);
          poly_add_pointwise(&coeffs[ctr*4+2],&coeffs[ctr*4+2],&temp[0]);
          ctr+=1;
          k=0;
        }
      }

      if(val1<KYBER_Q && ctr < KYBER_N/4){
        c[k]=val1;
        k++;
        if(k==4){
                 
          basemul(&temp[0],&b[ctr*4],&c[0],zeta[64+ctr]);
          poly_add_pointwise(&coeffs[ctr*4],&coeffs[ctr*4],&temp[0]);
          basemul(&temp[0],&b[ctr*4+2],&c[2],-zeta[64+ctr]);
          poly_add_pointwise(&coeffs[ctr*4+2],&coeffs[ctr*4+2],&temp[0]);
          ctr+=1;
          k=0;
        }
      }

      if(pos+3 > buflen && ctr < KYBER_N/4){
          xof_squeezeblocks(buf, 1, state);
          pos=0;
      }
    }
}

#if (SECRET_KEY_VARIANT==1)
void matacc_save_A(poly* r,polyvec *a, const polyvec *b, unsigned char i, const unsigned char *seed, int transposed) {
  //Ergebnis ist nur ein Polynom!
  unsigned char buf[XOF_BLOCKBYTES+2];
  xof_state state;
  int16_t c[4];
  int j = 0;

  if (transposed){
    xof_absorb(&state, seed, i, j);
  }else{
    xof_absorb(&state, seed, j, i); 
  }
  xof_squeezeblocks(buf, 1, &state);
  matacc_asm_save_A(r->coeffs,a->vec[j].coeffs, b->vec[j].coeffs, c, buf, zetas, &state);

  for(j=1;j<KYBER_K;j++) {
    if (transposed)
      xof_absorb(&state, seed, i, j);
    else
      xof_absorb(&state, seed, j, i);

    xof_squeezeblocks(buf, 1, &state);
    matacc_asm_acc_save_A(r->coeffs,a->vec[j].coeffs, b->vec[j].coeffs, c, buf, zetas, &state);
  } 
}

void matacc_asm_save_A(int16_t *coeffs,int16_t *a, const int16_t *b, int16_t c[4],unsigned char buf[XOF_BLOCKBYTES+2], const int16_t zeta[128], xof_state *state){
    
    //b ist ein polynom
    //Das Ergebnis (r) soll auch nur ein Polynom sein
    //c wird in jedem durchlauf mit koeffizienten gefüllt, bis 4 vorhanden sind; dann werden diese 4 koeffizenten mit den entsprechenden aus dem polynom des sk multipliziert.
    //dadurch entstehen 4 koeefizienten für das "ergebnispolynom"; Wenn das Polynom 256 Koeffizenten erreicht hat, dann wird es in den Ergebnis vektor eingefügt.
    uint16_t val0, val1,ctr=0, pos=0,buflen=XOF_BLOCKBYTES,k=0;
    while (ctr<(KYBER_N/4)){
      val0 = ((buf[pos+0] >> 0) | ((uint16_t)buf[pos+1] << 8)) & 0xFFF;
      val1 = ((buf[pos+1] >> 4) | ((uint16_t)buf[pos+2] << 4)) & 0xFFF;
      pos+=3;

      if(val0<KYBER_Q){
        c[k]=val0;
        a[ctr*4+k]=val0;
        k++;
        if(k==4){
          basemul(&coeffs[ctr*4],&c[0],&b[ctr*4],zeta[64+ctr]);
          basemul(&coeffs[ctr*4+2],&c[2],&b[ctr*4+2],-zeta[64+ctr]);
          ctr+=1;
          k=0;
        }
      }

      if(val1<KYBER_Q && ctr < KYBER_N/4){
        c[k]=val1;
        a[ctr*4+k]=val1;
        k++;
        if(k==4){
          basemul(&coeffs[ctr*4],&c[0],&b[ctr*4],zeta[64+ctr]);
          basemul(&coeffs[ctr*4+2],&c[2],&b[ctr*4+2],-zeta[64+ctr]);
          ctr+=1;
          k=0;
        }
      }

      if(pos+3 > buflen && ctr < KYBER_N/4){
          xof_squeezeblocks(buf, 1, state);
          pos=0;
      }
      
    }   
}

void matacc_asm_acc_save_A(int16_t *coeffs,int16_t *a, const int16_t *b, int16_t c[4], unsigned char buf[XOF_BLOCKBYTES+2], const int16_t zeta[128], xof_state *state){
   uint16_t val0=0, val1=0,ctr=0, pos=0,buflen=XOF_BLOCKBYTES,k=0;
   int16_t temp[2];
    while (ctr<(KYBER_N/4)){
      val0 = ((buf[pos+0] >> 0) | ((uint16_t)buf[pos+1] << 8)) & 0xFFF;
      val1 = ((buf[pos+1] >> 4) | ((uint16_t)buf[pos+2] << 4)) & 0xFFF;
      pos+=3;
      if(val0<KYBER_Q){
        c[k]=val0;
        a[ctr*4+k]=val0;
        k++;
        if(k==4){
          basemul(&temp[0],&b[ctr*4],&c[0],zeta[64+ctr]);
          poly_add_pointwise(&coeffs[ctr*4],&coeffs[ctr*4],&temp[0]);
          basemul(&temp[0],&b[ctr*4+2],&c[2],-zeta[64+ctr]);
          poly_add_pointwise(&coeffs[ctr*4+2],&coeffs[ctr*4+2],&temp[0]);
          ctr+=1;
          k=0;
        }
      }

      if(val1<KYBER_Q && ctr < KYBER_N/4){
        c[k]=val1;
        a[ctr*4+k]=val1;
        k++;
        if(k==4){
                 
          basemul(&temp[0],&b[ctr*4],&c[0],zeta[64+ctr]);
          poly_add_pointwise(&coeffs[ctr*4],&coeffs[ctr*4],&temp[0]);
          basemul(&temp[0],&b[ctr*4+2],&c[2],-zeta[64+ctr]);
          poly_add_pointwise(&coeffs[ctr*4+2],&coeffs[ctr*4+2],&temp[0]);
          ctr+=1;
          k=0;
        }
      }

      if(pos+3 > buflen && ctr < KYBER_N/4){
          xof_squeezeblocks(buf, 1, state);
          pos=0;
      }
    }
}
#endif