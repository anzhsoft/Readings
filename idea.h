//---------------------------------------------------------------------------

#include <Windows.h>

#include "struct.h"

#define IDEAKEYSIZE 16
#define IDEABLOCKSIZE 8
#define ROUNDS	8
#define KEYLEN	(6*ROUNDS+4)

typedef unsigned char		BYTE;
//typedef unsigned int		DWORD;
typedef unsigned short int	word16;
typedef unsigned int		word32;

#define IDEA_DECODE_ROUNDS ROUNDS
#define IDEA_SUBKEY_NUM    KEYLEN

#define low16(x) ((x) & 0xffff)

typedef unsigned int uint16;
typedef word16 IDEAkey[KEYLEN];
typedef unsigned short int WORD;

long EnIDEA(BYTE *IdeaKey,BYTE *InDataBuf,BYTE *OutDataBuf, long buflen);
long DEIDEA(BYTE *IdeaKey,BYTE *InDataBuf,BYTE *OutDataBuf,long InDataLength);
//---------------------------------------------------------------------------------
void cipher_idea_encrypt(word16 in[4],word16 out[4],IDEAkey Z);
void Idea_Decode_Data(WORD *in,WORD *out,WORD *Z);
void SubKeyCreate(WORD *SubKey,WORD *szSimKey);
void SubKeyChanged(WORD *Z,WORD *DK);
uint16 mul(uint16 a, uint16 b);
void IdeaDecrypt(WORD *in,WORD *out,WORD *Z);
