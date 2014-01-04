#include "StdAfx.h"
#include "base64.h"

const char* ch64="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-/"; 

const char EndFlag = 1;
const char EndFlag2 = '@';

unsigned char* encode(unsigned char *src,int srclen)
{
	int dstlen,i,j;
//	int pading = 0;
	unsigned char *buf;
	unsigned char *dst; 

	buf = src;
	dstlen = (srclen+2)*4/3;

	dst = (unsigned char*)malloc(dstlen+1);
	memset(dst, 0, dstlen+1);

	for(i=0,j=0; i<srclen; i+=3,j+=4){
		dst[j] = ch64[(buf[i]&0xFC)>>2];
		if(i+1>=srclen){
			dst[j+1] = ch64[(buf[i]&0x03)<<4];
			dst[j+2] = EndFlag;
			dst[j+3] = EndFlag;
			break;
		}
		dst[j+1] = ch64[((buf[i]&0x03)<<4) + ((buf[i+1]&0xF0)>>4)];
		if(i+2>=srclen){
			dst[j+2] = ch64[(buf[i+1]&0x0F)<<2];
			dst[j+3] = EndFlag;
			break;
		}
		dst[j+2] = ch64[((buf[i+1]&0x0F)<<2) + ((buf[i+2]&0xC0)>>6)];
		dst[j+3] = ch64[buf[i+2]&0x3F]; 
	}
	return dst;
}

unsigned char* encode2(unsigned char *src,int srclen)
{
	int dstlen,i,j;
//	int pading = 0;
	unsigned char *buf;
	unsigned char *dst; 

	buf = src;
	dstlen = (srclen+2)*4/3;

	dst = (unsigned char*)malloc(dstlen+1);
	memset(dst, 0, dstlen+1);

	for(i=0,j=0; i<srclen; i+=3,j+=4){
		dst[j] = ch64[(buf[i]&0xFC)>>2];
		if(i+1>=srclen){
			dst[j+1] = ch64[(buf[i]&0x03)<<4];
			dst[j+2] = EndFlag2;
			dst[j+3] = EndFlag2;
			break;
		}
		dst[j+1] = ch64[((buf[i]&0x03)<<4) + ((buf[i+1]&0xF0)>>4)];
		if(i+2>=srclen){
			dst[j+2] = ch64[(buf[i+1]&0x0F)<<2];
			dst[j+3] = EndFlag2;
			break;
		}
		dst[j+2] = ch64[((buf[i+1]&0x0F)<<2) + ((buf[i+2]&0xC0)>>6)];
		dst[j+3] = ch64[buf[i+2]&0x3F]; 
	}
	return dst;
}