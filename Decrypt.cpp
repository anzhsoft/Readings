#include "StdAfx.h"
#include "Decrypt.h"
#include "idea.h"

extern char CurrentPath[_MAX_PATH];
extern char UserName[50];

BOOL perDecrypt(LPCSTR WolFilePath, char* CifFilePath, _WOLFDECRYPT* &pWolfAtt, char* g_pWolBuf)
{
	WolFileHeadInfo m_Head = {0};
	memcpy(&m_Head, g_pWolBuf, sizeof(WolFileHeadInfo));
	pWolfAtt = new _WOLFDECRYPT;
	memset(pWolfAtt, 0, sizeof(_WOLFDECRYPT));
	strcpy_s(pWolfAtt->pFileName, _MAX_PATH, WolFilePath);
	pWolfAtt->wDecryptType = GetDecryptMode(m_Head.cDecryptType);
	if(!GetBookName(pWolfAtt->szBookName, g_pWolBuf))
		return FALSE;
	pWolfAtt->dwTxtAdd = sizeof(WolFileHeadInfo)+m_Head.wDocumentInfoLen+m_Head.dwCoverLen;
	
	memcpy(pWolfAtt->CifName, CifFilePath, strlen(CifFilePath));
	if(InitDecryptCode(pWolfAtt))
		return FALSE;
	char szCheckText[20] = {0};
//	fseek(fp, pWolfAtt->dwTxtAdd, 0);
//	fread(szCheckText, 16, 1, fp);
	memcpy(szCheckText, g_pWolBuf+pWolfAtt->dwTxtAdd, 16);
	DecryptDataMain(pWolfAtt, szCheckText, 16);
	
	if(strncmp(szCheckText,"<wolf>\r\n<catalog",16)!=0)
		return FALSE;

	return TRUE;
}

WORD GetDecryptMode(BYTE cDecryptType)
{
	switch(cDecryptType)
	{
	case 0x0:
		return DECRYPT_MODE_NONE;
	case 0x11:
		return DECRYPT_MODE_11;
	case 0x13:
		return DECRYPT_MODE_13;
	case 0x15:
		return DECRYPT_MODE_15;
	case 0x16:
		return DECRYPT_MODE_16;
	case 0x21:
		return DECRYPT_MODE_21;
	case 0x31:
		return DECRYPT_MODE_31;
	case 0x51:
		return DECRYPT_MODE_51;
	case 0x43:
		return DECRYPT_MODE_43;
	case 0xD4:
		return DECRYPT_MODE_D4;
	default:
		return DECRYPT_MODE_NOTSUPPORT;
	}
}

BOOL GetBookName(char *Name, char* g_pWolBuf)
{
	WolFileHeadInfo m_Head = {0};
	memcpy(&m_Head, g_pWolBuf, sizeof(WolFileHeadInfo));
	char *pBuf = new char[m_Head.wDocumentInfoLen+1];
	memset(pBuf, 0, m_Head.wDocumentInfoLen+1);
	memcpy(pBuf, g_pWolBuf+sizeof(WolFileHeadInfo), m_Head.wDocumentInfoLen);
	char *pSera = NULL;
	char *pSerb = NULL;
	pSera = strstr(pBuf, "<title>");
	pSerb = strstr(pSera, "\r\n");
	if(!pSera||!pSerb)
		return FALSE;
	strncpy_s(Name, _MAX_PATH, pSera+7, pSerb-pSera-7);
	return TRUE;
}

WORD DecryptDataMain(_WOLFDECRYPT *pWolReaderInfo,char* szBuffer,DWORD dwBufLen)
{
	DWORD i,j;
	WORD input[DECRYPT_IDEA_GROUPLEN/2],output[DECRYPT_IDEA_GROUPLEN/2],k;
	BYTE tmp[DECRYPT_IDEA_GROUPLEN];
	
	switch(pWolReaderInfo->wDecryptType)
	{
	case DECRYPT_MODE_NONE:
		return WF_EN_SUCCESS;
	case DECRYPT_MODE_11:
	case DECRYPT_MODE_15:
	case DECRYPT_MODE_21:
	case DECRYPT_MODE_31:
	case DECRYPT_MODE_51:
		for(i=0;i<dwBufLen;i++)
		{
			j=i % pWolReaderInfo->wDecryptLen;
			szBuffer[i]=(szBuffer[i])^(pWolReaderInfo->pDecryptCode[j]);
		}
		break;
	case DECRYPT_MODE_13:
	case DECRYPT_MODE_16:
	case DECRYPT_MODE_43:
	case DECRYPT_MODE_D4:
		while(dwBufLen)
		{
			memcpy(input,szBuffer,DECRYPT_IDEA_GROUPLEN);
			IdeaDecrypt(input,output,pWolReaderInfo->IDEA_Key);
			for(k=0;k<4;k++)
			{
				tmp[2*k]=(BYTE)(output[k]>>8);
				tmp[2*k+1]=(BYTE)(output[k]&255);
			}
			memcpy(szBuffer,tmp,DECRYPT_IDEA_GROUPLEN);
			szBuffer+=DECRYPT_IDEA_GROUPLEN;
			dwBufLen-=DECRYPT_IDEA_GROUPLEN;
		}
		break;
	}
	return WF_EN_SUCCESS;
}

WORD InitDecryptCode(_WOLFDECRYPT *pWolReaderInfo)
{
	WORD UserKey[WF_IDEA_KEY_LEN+8]={0},SubKey[WF_IDEA_KEY_NUM+8],wRetNum;
	switch(pWolReaderInfo->wDecryptType)
	{
	case DECRYPT_MODE_16:
	case DECRYPT_MODE_D4:
		wRetNum=LoadCIFFile(UserKey,pWolReaderInfo);
		if(wRetNum)
			return wRetNum;
		SubKeyCreate(SubKey,UserKey);
		SubKeyChanged(SubKey,pWolReaderInfo->IDEA_Key);
		pWolReaderInfo->wDecryptLen=DECRYPT_IDEALENGTH;
		break;

	default:
		return WF_EN_OUTOFDECRYPT;
	}
	return WF_EN_SUCCESS;
}

void Wolf_GetSimCardID(char *pCodeBuffer)
{
	char Path[_MAX_PATH] = {0};
	sprintf_s(Path, _MAX_PATH, "%s/system/sim.ini", CurrentPath);
	char simID[16] = {0};
	GetPrivateProfileString(UserName,"simID","",simID,16,Path);
	memcpy(pCodeBuffer,simID,15);//
	pCodeBuffer[15]=0;
}

WORD LoadCIFFile(WORD *pUserKey,_WOLFDECRYPT *pWolReaderInfo)
{
	char szOpenFilePath[260]={0};
	char CIFLongName[260]={0},CIFShortName[100]={0};
	FILE *fp;

	//获取证书文件的文件名
	memcpy(CIFShortName,pWolReaderInfo->pFileName,strlen(pWolReaderInfo->pFileName)-3);
	strcat_s(CIFShortName, 100, "cif");
	//打开文件
	fp=fopen(pWolReaderInfo->CifName, "rb");
	if(fp)
	{
		if(pWolReaderInfo->wDecryptType==DECRYPT_MODE_D4)
			fread((BYTE *)pUserKey,WF_IDEA_KEY_LEN,1,fp);
		else
		{//16
			BYTE szKey[16];
			WORD szSimKey[WF_IDEA_KEY_LEN],SubKey[WF_IDEA_KEY_NUM],IdeaKey[WF_IDEA_KEY_NUM];
			WORD SimKeyLen=16;

			//validate cif file
			if(!IsCIFFileValidate(fp,pWolReaderInfo,szKey,16))
				return WF_EN_BADCIFFILE;
			Wolf_GetSimCardID((char *)szSimKey);
			SubKeyCreate(SubKey,szSimKey);
			SubKeyChanged(SubKey,IdeaKey);
			GetDecryptCodeFromSimID(szKey,IdeaKey,(BYTE *)pUserKey,SimKeyLen);
		}
		fclose(fp);
	}
	else
	{
		memset(pUserKey,0,WF_IDEA_KEY_LEN);
		return WF_EN_OPENFILEFAIL;
	}
	return WF_EN_SUCCESS;
}

BOOL IsCIFFileValidate(FILE* pFilePtr,_WOLFDECRYPT *pWolReaderInfo,LPBYTE szRsaKey,int nKeyLen)
{
	DWORD dwFileLen;
	BYTE *FileBuf,*CurBuf,cCheckSum;
	int nCheckSum,i,NameLen;
	
	if(!pFilePtr)
		return FALSE;
	fseek(pFilePtr, 0, 2);
	dwFileLen=ftell(pFilePtr);
	fseek(pFilePtr, 0, 0);
	if(dwFileLen>0x2000||!dwFileLen)//>8k or =0
		return FALSE;
	FileBuf=new BYTE[dwFileLen+100];//(LPBYTE)MallocMem(dwFileLen+100);
	if(!FileBuf)
		return FALSE;
	memset(FileBuf, 0, dwFileLen+100);
	if(!fread(FileBuf,dwFileLen,1, pFilePtr))
	{
		delete []FileBuf;
		return FALSE;
	}
	//analyzer file buf
	
	CurBuf=(BYTE *)strstr((char*)FileBuf,"<BookName>");
	if(!CurBuf)
	{
		delete []FileBuf;
		return FALSE;
	}
	CurBuf+=10;
	NameLen=strlen(pWolReaderInfo->szBookName);
	if(strncmp((char*)CurBuf,(char*)pWolReaderInfo->szBookName,NameLen)!=0||CurBuf[NameLen]!='<')
	{//not match
		delete []FileBuf;
		return FALSE;
	}
	CurBuf=(BYTE *)strstr((char*)FileBuf," CheckSum=");
	if(!CurBuf)
	{
		delete []FileBuf;
		return FALSE;
	}
	CurBuf+=10;
	if(*CurBuf=='\"')
		CurBuf++;
	nCheckSum=atoi((const char*)CurBuf);
	
	CurBuf=(BYTE *)strstr((char *)FileBuf,"<ContentKey ");
	if(!CurBuf)
	{
		delete []FileBuf;
		return FALSE;
	}
	while(CurBuf[0]!='>')
	{
		if(CurBuf>FileBuf+dwFileLen)
		{
			delete []FileBuf;
			return FALSE;
		}
		CurBuf++;
	}
	CurBuf++;
	if(strncmp((char*)&(CurBuf[nKeyLen]),"</ContentKey>",13)!=0)
	{
		delete []FileBuf;
		return FALSE;
	}
	
	cCheckSum=*CurBuf;
	for(i=1;i<nKeyLen;i++)
	{
		cCheckSum^=CurBuf[i];
	}
	if(cCheckSum!=(BYTE)nCheckSum)
	{
		delete []FileBuf;
		return FALSE;
	}
	memcpy(szRsaKey,CurBuf,nKeyLen);
	delete []FileBuf;
	return TRUE;
}

//由sim ID得到解密码（16方式使用）
void GetDecryptCodeFromSimID(BYTE *szKey,WORD *IdeaKey,BYTE *szIdeaKey,WORD SimKeyLen)
{
	BYTE tmp[DECRYPT_IDEA_GROUPLEN];
	WORD input[DECRYPT_IDEA_GROUPLEN/2],output[DECRYPT_IDEA_GROUPLEN/2],k;
	while(SimKeyLen)
	{
		memcpy(input,szKey,DECRYPT_IDEA_GROUPLEN);
		IdeaDecrypt(input,output,IdeaKey);
		for(k=0;k<4;k++)
		{
			tmp[2*k]=(BYTE)(output[k]>>8);
			tmp[2*k+1]=(BYTE)(output[k]&255);
		}
		
		memcpy(szIdeaKey,tmp,DECRYPT_IDEA_GROUPLEN);
		szIdeaKey+=DECRYPT_IDEA_GROUPLEN;
		szKey+=DECRYPT_IDEA_GROUPLEN;
		SimKeyLen-=DECRYPT_IDEA_GROUPLEN;
	}
}

