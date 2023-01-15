/*
 * Copyright (C) 2016 Southern Storm Software, Pty Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef CRYPTO_NEWHOPE_h	//include guard
#define CRYPTO_NEWHOPE_h

#include <inttypes.h>
// constant algorithm parameters
#define NEWHOPE_SENDABYTES  1824
#define NEWHOPE_SENDBBYTES  2048
#define NEWHOPE_SHAREDBYTES 32

#if defined(__AVR__)
#define NEWHOPE_SMALL_FOOTPRINT 1
#else
#define NEWHOPE_SMALL_FOOTPRINT 0
#endif

typedef struct // Private key
{
    /** @cond */
#if NEWHOPE_SMALL_FOOTPRINT
    uint8_t seed[32]; // privkey gets regenerated from seed -> needs less memory
#else
    uint16_t coeffs[1024];// all private key coefficients
#endif
    /** @endcond */

} NewHopePrivateKey;

class NewHope
{
private: // constructor and destructor are private to prevent acces to all intern functions. Only the static public functions shall be accessible.
    NewHope() {} //constructor	
    ~NewHope() {} //destructor

public:
    enum Variant //NewHope Varants Enumeration
    {
        Ref,
        Torref
    };
	//Declaration of fucntions; only these are accessible from the outside
    static void keygen(uint8_t send[NEWHOPE_SENDABYTES], NewHopePrivateKey &sk,
                       Variant variant = Ref, const uint8_t *random_seed = 0);
    static void sharedb(uint8_t shared_key[NEWHOPE_SHAREDBYTES],
                        uint8_t send[NEWHOPE_SENDBBYTES],
                        uint8_t received[NEWHOPE_SENDABYTES],
                        Variant variant = Ref, const uint8_t *random_seed = 0);
    static void shareda(uint8_t shared_key[NEWHOPE_SHAREDBYTES],
                        const NewHopePrivateKey &sk,
                        uint8_t received[NEWHOPE_SENDBBYTES]);
};
#endif
