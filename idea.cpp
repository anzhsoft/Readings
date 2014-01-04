#include "stdafx.h"
#include "idea.h"
#include <memory.h>
#define MUL(x,y) (x=mul(low16(x),y))

uint16 mul(uint16 a, uint16 b)
{
	word32 p;
	if (a){
		if (b){
			p=(word32)a * b;
			b=(uint16)(low16(p));
			a=(uint16)(p>>16);
			return b - a + (b < a);
		}
		return 1 - a;
	}
	return 1 - b;
}


static uint16 inv(uint16 x)
{
	uint16 t0,t1;
	uint16 q,y;
	if (x <= 1)
		return x;
	t1 = (uint16)(0x10001l/x);
	y = (uint16)(0x10001l%x);
	if (y == 1)
		return low16(1 - t1);
	t0 = 1;
	do{
		q = x / y;
		x = x % y;
		t0 += q * t1;
		if (x == 1)
			return t0;
		q = y / x;
		y = y % x;
		t1 += q * t0;
	} while (y != 1);

	return low16(1 - t1);
}

//----------------------
void cipher_idea_encrypt(word16 in[4],word16 out[4],register IDEAkey Z)
{
	register uint16 x1,x2,x3,x4,t1,t2;
	int r = ROUNDS;
	x1 = *in++; x2 = *in++;
	x3 = *in++; x4 = *in;
	do{
		MUL(x1, *Z++);
		x2 += *Z++;
		x3 += *Z++;
		MUL(x4, *Z++);
		t2 = x1^x3;
		MUL(t2, *Z++);
		t1 = t2 + (x2^x4);
		MUL(t1, *Z++);
		t2 = t1 + t2;
		x1 ^= t1;
		x4 ^= t2;
		t2 ^= x2;
		x2 = x3^t1;
		x3 = t2;
	} while(--r);
	
	MUL(x1, *Z++);
	*out++ = x1;
	*out++ = (x3 + *Z++);
	*out++ = (x2 + *Z++);
	MUL(x4, *Z);
	*out = x4;
}

void Idea_Decode_Data(WORD *in,WORD *out,WORD *Z)
{
	DWORD x1,x2,x3,x4,t1,t2;
	WORD OutBuf[4],k;
	WORD r = IDEA_DECODE_ROUNDS;
	x1 = *in++; x2 = *in++;
	x3 = *in++; x4 = *in;
	do{	
		MUL(x1, *Z++);
		x2 += *Z++;
		x3 += *Z++;
		MUL(x4, *Z++);
		t2 = x1^x3;
		MUL(t2, *Z++);
		t1 = t2 + (x2^x4);
		MUL(t1, *Z++);
		t2 = t1 + t2;
		x1 ^= t1;
		x4 ^= t2;
		t2 ^= x2;
		x2 = x3^t1;
		x3 = t2;
	}while (--r);
	
	MUL(x1, *Z++);
	*out++ = (WORD)x1;
	*out++ = (WORD)(x3 + *Z++);
	*out++ = (WORD)(x2 + *Z++);
	MUL(x4, *Z);
	*out = (WORD)x4;

	for(k = 0; k < 4; k++){
        OutBuf[2 * k] = (BYTE)(out[k] >> 8);
        OutBuf[2 * k + 1] = (BYTE)(out[k] & 255);
	}	
}

void SubKeyCreate(WORD *SubKey,WORD *szSimKey)
{
	WORD *tmp, i, j;
	tmp = SubKey;
	
	for (j = 0; j < 8; j++)		
		tmp[j]=szSimKey[j];
	
	for (i = 0; j < IDEA_SUBKEY_NUM; j++){
		i++;
		tmp[i + 7] = ((tmp[i & 7] << 9) | (tmp[i + 1 & 7] >> 7));
		tmp += i & 8;
		i &= 7;		
	}	
}

//由SubKeyCreate()产生的52字节中间工程密码,得到用于解密的数据的密钥[52]字节子密钥
void SubKeyChanged(WORD *Z,WORD *DK)
{
	WORD j, T[IDEA_SUBKEY_NUM];
	DWORD t1, t2, t3;
	//IDEAkey T;
	WORD *p = T + IDEA_SUBKEY_NUM;
	t1 = inv(*Z++);
	t2 = 65536 - *Z++;//65536
	t3 = 65536 - *Z++;//65536
	*--p = (WORD)inv(*Z++);
	*--p = (WORD)t3;
	*--p = (WORD)t2;
	*--p = (WORD)t1;
	for(j = 1; j < IDEA_DECODE_ROUNDS; j++){
		t1 = *Z++;
		*--p = *Z++;
		*--p = (WORD)t1;
		t1 = inv(*Z++);
		t2 = 65536 - *Z++;//65536
		t3 = 65536 - *Z++;//65536
		*--p = (WORD)inv(*Z++);
		*--p = (WORD)t2;
		*--p = (WORD)t3;
		*--p = (WORD)t1;
	}
	t1 = *Z++;
	*--p = *Z++;
	*--p = (WORD)t1;
	t1 = inv(*Z++);
	t2 = 65536 - *Z++;//65536
	t3 = 65536 - *Z++;//65536
	*--p = (WORD)inv(*Z++);
	*--p = (WORD)t3;
	*--p = (WORD)t2;
	*--p = (WORD)t1;
	
	//copy and destroy temp copy
	for(j = 0, p = T; j < IDEA_SUBKEY_NUM; j++){
		*DK++ = *p;
		*p++ = 0;
	}
}

long EnIDEA(BYTE *IdeaKey,BYTE *InDataBuf,BYTE *OutDataBuf, long buflen)
{
    WORD InBuf[10], OutBuf[10];
	long Counts = buflen;
    IDEAkey EN_SubKey;
    long OutDataLength;
	
    Counts = (Counts + IDEA_DECODE_ROUNDS - 1) / IDEA_DECODE_ROUNDS;
    OutDataLength = Counts * IDEA_DECODE_ROUNDS;
    SubKeyCreate((WORD *)&EN_SubKey, (WORD *)IdeaKey);
    while(Counts>1){
        memcpy((BYTE*)InBuf, InDataBuf, 8);
        cipher_idea_encrypt((WORD *)InBuf, (WORD *)OutBuf, EN_SubKey);
        memcpy(OutDataBuf, OutBuf, 8);
        OutDataBuf += 8;
        InDataBuf += 8;
        Counts--;
    }
    if(Counts == 1){
		long len = buflen % 8;
		if(len == 0)
			len = 8;

        memset((BYTE*)InBuf, 0, 8);
        memcpy((BYTE*)InBuf, InDataBuf, len);
        cipher_idea_encrypt((WORD *)InBuf, (WORD *)OutBuf, EN_SubKey);
        memcpy(OutDataBuf, OutBuf, 8);
    }
    return OutDataLength;
}

long DEIDEA(BYTE *IdeaKey,BYTE *InDataBuf,BYTE *OutDataBuf,long InDataLength)
{
    WORD InBuf[4], OutBuf[4];
    long Counts = InDataLength/8;
    IDEAkey EN_SubKey, De_SubKey;
	
    SubKeyCreate((WORD *)&EN_SubKey, (WORD *)IdeaKey);
    SubKeyChanged((WORD *)&EN_SubKey, (WORD *)&De_SubKey);
    while(Counts){
        memcpy(InBuf, InDataBuf, 8);
        Idea_Decode_Data((WORD *)InBuf, (WORD *)OutBuf, (WORD *)&De_SubKey);
        memcpy(OutDataBuf, OutBuf, 8);
        OutDataBuf += 8;
        InDataBuf += 8;
        Counts--;
    }
    return InDataLength;
}

void IdeaDecrypt(WORD *in,WORD *out,WORD *Z)
{
	DWORD x1,x2,x3,x4,t1,t2;
	WORD r=IDEA_DECODE_ROUNDS;
	x1=*in++; x2=*in++;
	x3=*in++; x4=*in;
	do
	{
		MUL(x1,*Z++);
		x2+=*Z++;
		x3+=*Z++;
		MUL(x4,*Z++);
		t2=x1^x3;
		MUL(t2,*Z++);
		t1=t2+(x2^x4);
		MUL(t1,*Z++);
		t2=t1+t2;
		x1^=t1;
		x4^=t2;
		t2^=x2;
		x2=x3^t1;
		x3=t2;
	} while (--r);
	MUL(x1,*Z++);
	*out++=(WORD)x1;
	*out++=(WORD)(x3+*Z++);
	*out++=(WORD)(x2+*Z++);
	MUL(x4,*Z);
	*out=(WORD)x4;
}