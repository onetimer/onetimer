#ifndef HASHBLOCK_H
#define HASHBLOCK_H

#include "uint256.h"
#include "sph_luffa.h"
#include "sph_cubehash.h"
#include "sph_shavite.h"
#include "sph_simd.h"
#include "sph_echo.h"

#ifndef QT_NO_DEBUG
#include <string>
#endif

#ifdef GLOBALDEFINED
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL sph_luffa512_context		z_luffa;
GLOBAL sph_cubehash512_context	z_cubehash;
GLOBAL sph_shavite512_context	z_shavite;
GLOBAL sph_simd512_context		z_simd;
GLOBAL sph_echo512_context		z_echo;

#define fillz() do { \
    sph_luffa512_init(&z_luffa); \
	sph_cubehash512_init(&z_cubehash); \
    sph_shavite512_init(&z_shavite); \
	sph_simd512_init(&z_simd); \
	sph_echo512_init(&z_echo); \
} while (0) 

template<typename T1>
inline uint256 Hash5(const T1 pbegin, const T1 pend)

{
    sph_luffa512_context	 ctx_luffa;
	sph_cubehash512_context  ctx_cubehash;
	sph_shavite512_context	 ctx_shavite;
	sph_simd512_context		 ctx_simd;
	sph_echo512_context		 ctx_echo;
    static unsigned char pblank[1];

#ifndef QT_NO_DEBUG
    //std::string strhash;
    //strhash = "";
#endif

       
    uint512 hash[5];

    sph_luffa512_init(&ctx_luffa);
    sph_luffa512 (&ctx_luffa, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
    sph_luffa512_close(&ctx_luffa, static_cast<void*>(&hash[0]));
    
    sph_cubehash512_init(&ctx_cubehash);
    sph_cubehash512 (&ctx_cubehash, static_cast<const void*>(&hash[0]), 64);
    sph_cubehash512_close(&ctx_cubehash, static_cast<void*>(&hash[1]));
    
    sph_shavite512_init(&ctx_shavite);
    sph_shavite512(&ctx_shavite, static_cast<const void*>(&hash[1]), 64);
    sph_shavite512_close(&ctx_shavite, static_cast<void*>(&hash[2]));
        
    sph_simd512_init(&ctx_simd);
    sph_simd512 (&ctx_simd, static_cast<const void*>(&hash[2]), 64);
    sph_simd512_close(&ctx_simd, static_cast<void*>(&hash[3]));

    sph_echo512_init(&ctx_echo);
    sph_echo512 (&ctx_echo, static_cast<const void*>(&hash[3]), 64);
    sph_echo512_close(&ctx_echo, static_cast<void*>(&hash[4]));

    return hash[4].trim256();
}






#endif // HASHBLOCK_H
