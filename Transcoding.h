// Transcoding.h: interface for the CTranscoding class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSCODING_H__B1361370_9494_43CD_B26C_465C61A411C9__INCLUDED_)
#define AFX_TRANSCODING_H__B1361370_9494_43CD_B26C_465C61A411C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTranscoding  
{
public:
	static void UTF_8ToGB2312(CString &pOut, char *pText, int pLen);
	static int BinToInt(char   *pBin);
	static CString BinToHex(CString   BinString);
	static CString HexToBin(CString   string);
	static void UnicodeToGB2312(char* pOut,unsigned short uData);
	static void UTF_8ToUnicode(WCHAR* pOut,char *pText);
	CTranscoding();
	virtual ~CTranscoding();

};

#endif // !defined(AFX_TRANSCODING_H__B1361370_9494_43CD_B26C_465C61A411C9__INCLUDED_)
