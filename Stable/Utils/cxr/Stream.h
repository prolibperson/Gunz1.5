/*********************************************************************

   Copyright (C) 2002 Smaller Animals Software, Inc.

   This software is provided 'as-is', without any express or implied
   warranty.  In no event will the authors be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.

   3. This notice may not be removed or altered from any source distribution.

   http://www.smalleranimals.com
   smallest@smalleranimals.com

**********************************************************************/
/*
	modified by Chris Losinger for Smaller Animals Software, Inc.
*/
// Sapphire.cpp -- modified by Wei Dai from:

/* Sapphire.cpp -- the Saphire II stream cipher class.
   Dedicated to the Public Domain the author and inventor:
   (Michael Paul Johnson).  This code comes with no warranty.
   Use it at your own risk.
   Ported from the Pascal implementation of the Sapphire Stream
   Cipher 9 December 1994.
   Added hash pre- and post-processing 27 December 1994.
   Modified initialization to make index variables key dependent,
   made the output function more resistant to cryptanalysis,
   and renamed to CPASapphire II 2 January 1995
*/

#ifndef CXR_STREAM_H
#define CXR_STREAM_H

#include <algorithm>

#define BYTE unsigned char

class CCXRIntBase
{
protected:
   CCXRIntBase(const BYTE *key, unsigned int ks)
   {
      int i;BYTE rs;unsigned kp;
      for(i=0;i<256;i++)c[i]=i;kp=0;rs=0;for(i=255;i;i--)std::swap(c[i],c[kr(i,key,ks,&rs,&kp)]);r2=c[1];r1=c[3];av=c[5];lp=c[7];lc=c[rs];rs=0;kp=0;
   }
	inline void SC(){BYTE st=c[lc];r1+=c[r2++];c[lc]=c[r1];c[r1]=c[lp];c[lp]=c[r2];c[r2]=st;av+=c[st];}
	BYTE c[256],r2,r1,av,lp,lc;    

   BYTE kr(unsigned int lm, const BYTE *uk, BYTE ks, BYTE *rs, unsigned *kp)
   {
      unsigned rl=0,mk=1,u;
      while(mk<lm)mk=(mk<<1)+1;
      do{*rs=c[*rs]+uk[(*kp)++];
      if(*kp>=ks){*kp=0;*rs+=ks;}u=mk&*rs;
      if(++rl>11)u%=lm;}while(u>lm);
      return u;
   }
};
struct CCXRIntDec:CCXRIntBase
{
	CCXRIntDec(const BYTE *userKey, unsigned int keyLength=16) : CCXRIntBase(userKey, keyLength) {}
	inline BYTE ProcessByte(BYTE b){SC();lp=b^c[(c[r1]+c[r2])&0xFF]^c[c[(c[lp]+c[lc]+c[av])&0xFF]];lc=b;return lp;}
};

struct CCXRIntEnc:CCXRIntBase
{
	CCXRIntEnc(const BYTE *userKey, unsigned int keyLength=16) : CCXRIntBase(userKey, keyLength) {}
	inline BYTE ProcessByte(BYTE b){SC();lc=b^c[(c[r1]+c[r2])&0xFF]^c[c[(c[lp]+c[lc]+c[av])&0xFF]];lp=b;return lc;}
};
#endif
