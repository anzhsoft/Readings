// ParseTxtFile.cpp: implementation of the CParseTxtFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ParseTxtFile.h"
#include "ReadDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParseTxtFile::CParseTxtFile()
{
	g_pTxtBuf=NULL;
	m_bInitialing = FALSE;

	show_style = show_normal;
	sysWidth = WIDTH;
	sysHeight = HEIGHT;
	fontsize = FONT_SIZE_NORMAL;
}

CParseTxtFile::~CParseTxtFile()
{

}

int CParseTxtFile::BeginParseFile(LPCSTR FileName)
{
	CString fullname = FileName;
	fullname.Replace('/','\\');
	int index = fullname.ReverseFind('\\');
	if(index<0)
		m_Title = fullname;
	else
		m_Title = fullname.Right(fullname.GetLength()-index-1);

	FILE* fp;
	fp = fopen(FileName, "rb");
	if(!fp)
		return -1;
	fseek(fp, 0, 2);
	len = ftell(fp);
	fseek(fp, 0, 0);
	g_pTxtBuf = (char*)malloc(len+1);
	memset(g_pTxtBuf, 0, len+1);
	fread(g_pTxtBuf, len, 1, fp);
	fclose(fp);
	Total = 1;
	DWORD offset = 0;
	map.SetAt(Total,offset);
	close = FALSE;
	m_pThread = AfxBeginThread(_Paged, this, THREAD_PRIORITY_NORMAL, CREATE_SUSPENDED);
	m_pThread->m_bAutoDelete = TRUE;
	m_pThread->ResumeThread();
	return 0;
}

void CParseTxtFile::InitialPage()
{
try{
	char* cur = g_pTxtBuf;
	int line=0;
	int x = LEFT_MARGIN;
	DWORD loc=0;
	for(; loc<strlen(g_pTxtBuf) && !close; loc++){
		if(cur[loc]>0){
			if(x+fontsize/2>sysWidth-LEFT_MARGIN){
				x = LEFT_MARGIN;
				line++;
			}
		}
		else{
			if(x+fontsize>sysWidth-LEFT_MARGIN){
				x = LEFT_MARGIN;
				line++;
			}
		}
		if(line>=20 && !close){
			line=0;
			Total++;
			map.SetAt(Total,loc);
			CReadDlg* pDlg = (CReadDlg*)p;
			pDlg->PostMessage(WM_PAGENUM_CHANGE, (WPARAM)m_bInitialing, (LPARAM)Total);
		}
		if(cur[loc]==0x0d)
			continue;
		if(cur[loc]==0x0a){
			x = LEFT_MARGIN;
			line++;
			continue;
		}
		if(cur[loc]<0){		
			loc++;
			x+=fontsize;
		}
		else
			x+=fontsize/2;
	}
}
catch(...){
#ifdef _DEBUG
	AfxMessageBox("void CParseTxtFile::InitialPage()");
#endif
}
}

void CParseTxtFile::ShowFile(CDC* pDC, DWORD pageNum)
{
	if(pageNum==0)
		ShowCover(pDC);
	else if(pageNum<=Total)
		ShowPage(pDC, pageNum);
}

void CParseTxtFile::ShowCover(CDC* pDC)
{
	CRect rect(LEFT_MARGIN,sysHeight*0.3,sysWidth-LEFT_MARGIN*2, sysHeight-HEAD_MARGIN*2);
	pDC->DrawText((LPCSTR)m_Title, m_Title.GetLength(), rect, DT_WORDBREAK|DT_CENTER);
}

void CParseTxtFile::ShowPage(CDC* pDC, DWORD pageNum)
{
	DWORD loc = 0;
	if(!map.Lookup(pageNum,loc))
		return;
	int x = LEFT_MARGIN;
	int y = HEAD_MARGIN;
	int line = 0;
	for(; loc<strlen(g_pTxtBuf); loc++){
		if(g_pTxtBuf[loc]>0){
			if(x+fontsize/2>sysWidth-LEFT_MARGIN){
				x = LEFT_MARGIN;
				y += LINE_MARGIN+fontsize;
				line++;
			}
		}
		else{
			if(x+fontsize>sysWidth-LEFT_MARGIN){
				x = LEFT_MARGIN;
				y += LINE_MARGIN+fontsize;
				line++;
			}
		}
		if(line>=20)
			break;
		if(g_pTxtBuf[loc]==0x0d)
			continue;
		if(g_pTxtBuf[loc]==0x0a){
			x = LEFT_MARGIN;
			y += LINE_MARGIN+fontsize;
			line++;
			continue;
		}
		if(g_pTxtBuf[loc]<0){
			CString str;
			str.Format("%c%c",g_pTxtBuf[loc],g_pTxtBuf[loc+1]);
			pDC->TextOut(x,y,str);
			loc++;
			x+=fontsize;
		}
		else if(g_pTxtBuf[loc]!=0x20){
			CString str;
			str.Format("%c",g_pTxtBuf[loc]);
			pDC->TextOut(x,y,str);
			x+=fontsize/2;
		}
		
	}
}

void CParseTxtFile::Release()
{
	close = TRUE;
	while(m_bInitialing){
		MSG msg;
		while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)){ 
			if (!AfxGetApp()->PumpMessage()) 
				break; 
		} 
	}
	map.RemoveAll();
	if(g_pTxtBuf)
		free(g_pTxtBuf);
}

UINT CParseTxtFile::_Paged(LPVOID pParam)
{
	CParseTxtFile* p = (CParseTxtFile*)pParam;
	p->m_bInitialing = TRUE;
	p->InitialPage();
	p->m_bInitialing = FALSE;
	return 0;
}

void CParseTxtFile::SetShowStyle(int style)
{
	if(style == show_large){
		show_style = show_large;
		sysWidth = WIDTH_LARGE;
		sysHeight = HEIGHT_LARGE;
		fontsize = FONT_SIZE_LARGE;
	}
	else if(style == show_normal){
		show_style = show_normal;
		sysWidth = WIDTH;
		sysHeight = HEIGHT;
		fontsize = FONT_SIZE_NORMAL;
	}
	else{
		show_style = show_normal;
		sysWidth = WIDTH;
		sysHeight = HEIGHT;
		fontsize = FONT_SIZE_NORMAL;
	}
}