// Transcoding.cpp: implementation of the CTranscoding class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Transcoding.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTranscoding::CTranscoding()
{
	
}

CTranscoding::~CTranscoding()
{
	
}

void CTranscoding::UTF_8ToUnicode(WCHAR* pOut,char *pText)//把UTF-8转换成Unicode   
{
	char* uchar = (char *)pOut;
	
	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);
	
	return;
	
}

void CTranscoding::UnicodeToGB2312(char* pOut,unsigned short uData)//把Unicode转换成GB2312   
{
	WideCharToMultiByte(CP_ACP,NULL,(LPCWSTR)&uData,1,pOut,sizeof(WCHAR),NULL,NULL);
	return;   
	
}

CString CTranscoding::HexToBin(CString string)//将16进制数转换成2进制
{
	if(   string   ==   "0")   return   "0000";   
	if(   string   ==   "1")   return   "0001";   
	if(   string   ==   "2")   return   "0010";   
	if(   string   ==   "3")   return   "0011";   
	if(   string   ==   "4")   return   "0100";   
	if(   string   ==   "5")   return   "0101";   
	if(   string   ==   "6")   return   "0110";   
	if(   string   ==   "7")   return   "0111";   
	if(   string   ==   "8")   return   "1000";   
	if(   string   ==   "9")   return   "1001";   
	if(   string   ==   "a")   return   "1010";   
	if(   string   ==   "b")   return   "1011";   
	if(   string   ==   "c")   return   "1100";   
	if(   string   ==   "d")   return   "1101";   
	if(   string   ==   "e")   return   "1110";   
	if(   string   ==   "f")   return   "1111";   
	return   "";   
	
}

CString CTranscoding::BinToHex(CString BinString)//将2进制数转换成16进制
{
	if(   BinString   ==   "0000")   return   "0";   
	if(   BinString   ==   "0001")   return   "1";   
	if(   BinString   ==   "0010")   return   "2";   
	if(   BinString   ==   "0011")   return   "3";   
	if(   BinString   ==   "0100")   return   "4";   
	if(   BinString   ==   "0101")   return   "5";   
	if(   BinString   ==   "0110")   return   "6";   
	if(   BinString   ==   "0111")   return   "7";   
	if(   BinString   ==   "1000")   return   "8";   
	if(   BinString   ==   "1001")   return   "9";   
	if(   BinString   ==   "1010")   return   "a";   
	if(   BinString   ==   "1011")   return   "b";   
	if(   BinString   ==   "1100")   return   "c";   
	if(   BinString   ==   "1101")   return   "d";   
	if(   BinString   ==   "1110")   return   "e";   
	if(   BinString   ==   "1111")   return   "f";   
	return   "";   
	
}

int CTranscoding::BinToInt(char *pBin)
{
	int   m_Dec   =   0;           
	int   m_Len   =   strlen(pBin);   
    
	for   (int   i   =0   ;i   <   m_Len   ;i   ++)   
	{   
		m_Dec   =   (m_Dec   <<   1)   +   (pBin[i]   -   '0');   
	}   
    
	return   m_Dec;   
	
}

//　UTF-8转换成GB2312先把UTF-8转换成Unicode.然后再把Unicode通过函数WideCharToMultiByte转换成GB2312   
void CTranscoding::UTF_8ToGB2312(CString &pOut, char *pText, int pLen)
{
	char * newBuf = new char[pLen];
         char Ctemp[4];
         memset(Ctemp,0,4);
		 
         int i =0;
         int j = 0;
         
         while(i < pLen)
         {
			 if(pText[i] > 0)
			 {
				 newBuf[j++] = pText[i++];                       
			 }
			 else                 
			 {
				 WCHAR Wtemp;
				 UTF_8ToUnicode(&Wtemp,pText + i);
				 
				 UnicodeToGB2312(Ctemp,Wtemp);
				 
				 newBuf[j] = Ctemp[0];
				 newBuf[j + 1] = Ctemp[1];
				 
				 i += 3;    
				 j += 2;   
			 }
         }
         newBuf[j] = '\0';
         
         pOut = newBuf;
         delete []newBuf;
         
         return; 

}
