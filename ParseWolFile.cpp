// ParseWolFile.cpp : implementation file
//
#include "stdafx.h"
#include "ReadDlg.h"
#include "ParseWolFile.h"
#include "MainFrm.h"
//#include "Decrypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CParseWolFile dialog

extern CMainFrame* pMainWnd;
extern char CurrentPath[_MAX_PATH];
extern char UserName[50];

/////////图形解压缩
#define RINGBUFLEN       	4096   	// size of ring buffer 
#define MATCHLENLIMIT		18		// upper limit for match_length 
#define THRESHOLD			2   	// encode string into position and length if match_length is greater than this 
const BYTE PixelBit[]={0x80,0x40,0x20,0x10,0x8,0x4,0x2,0x1};
WORD DecodeGraphicsCode(LPBYTE DecodeBuf,LPBYTE CompressBuf, DWORD CompressLength);

WORD DecodeGraphicsCode(LPBYTE DecodeBuf,LPBYTE CompressBuf, DWORD CompressLength)
{
	WORD  i, j, k, r;
	WORD flags, input_char;
	DWORD ip;
	BYTE decode_buf[RINGBUFLEN];
 
	memset((LPBYTE)decode_buf,0x20, RINGBUFLEN-MATCHLENLIMIT);
	r = RINGBUFLEN - MATCHLENLIMIT;
	flags = 0;
	ip=0;
	while(1)
    {
		if (((flags >>= 1) & 256) == 0)
        {	
           	input_char = CompressBuf[ip];
           	ip++;
			if (ip==CompressLength) break;
			flags = input_char | 0xff00;
        }
		if (flags & 1)
        {
           	input_char = CompressBuf[ip];
           	ip++;
           	if (ip==CompressLength) break;
			*DecodeBuf=(BYTE)input_char;
			DecodeBuf++;
			decode_buf[r++] =(BYTE)input_char;
			r &= (RINGBUFLEN - 1);
        }
        else
        {
           	i = CompressBuf[ip];
           	ip++;
           	if (ip==CompressLength) break;
           	j = CompressBuf[ip];
           	ip++;
           	if (ip==CompressLength) break;			
			i |= ((j & 0xf0) << 4);
			j = (j & 0x0f) + THRESHOLD;
			for (k = 0; k <= j; k++)
            {
				input_char = decode_buf[(i + k) & (RINGBUFLEN - 1)];
				*DecodeBuf=(BYTE)input_char;
				DecodeBuf++;
				decode_buf[r++] =(BYTE)input_char;
				r &= (RINGBUFLEN - 1);
            }
		}//else
	}//for
	return 0;
}
///////----end


CParseWolFile::CParseWolFile(CWnd* pParent /*=NULL*/)	
{
	//{{AFX_DATA_INIT(CParseWolFile)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	memset(&m_head, 0, sizeof(WolFileHeadInfo));
	m_bFinish = false;
	m_bFileExist = FALSE;
	pWolf = NULL;
	g_pWolBuf = NULL;

	show_style = show_normal;
	sysWidth = WIDTH;
	sysHeight = HEIGHT;
	fontsize = FONT_SIZE_NORMAL;
	pageCenterFilePath=0;
	strcpy_s(extension, 4, ".pg");
}

int CParseWolFile::BeginParseFile(LPCSTR FileName)
{
	m_bFinish = false;

	DWORD Len = 0;
	if(!Legal(FileName, Len))	//检验合法性
		return -2;		//文件不可读
	
	if(m_head.cDecryptType != 0){	//加密
		char simID[16] = {0};
		char Path[_MAX_PATH] = {0};
		sprintf_s(Path, _MAX_PATH, "%s/system/sim.ini", CurrentPath);
		if(PathFileExists(Path))
			GetPrivateProfileString(UserName,"simID","",simID,15,Path);
		if(strlen(simID)<15)
			return -3;
		char CifFileName[_MAX_PATH] = {0};
		memcpy(CifFileName,FileName, strlen(FileName)-3);
		strcat_s(CifFileName, _MAX_PATH, _T("cif"));
		if(!perDecrypt(FileName,CifFileName,pWolf, g_pWolBuf))
			return -3;
	}
	
	m_bFileExist = TRUE;
	Total = m_head.dwDispTotalPage2;	//总页数
	if(!GetInfo()){
		memset(m_BookName, 0, _MAX_PATH);
		strncpy_s(m_BookName, _MAX_PATH, FileName, strlen(FileName-4));
	}
	return 0;
}

void CParseWolFile::Release()
{
	memset(FileName, 0, _MAX_PATH);
	if(pWolf)
		free(pWolf);
	pWolf = NULL;
	if(g_pWolBuf)
		free(g_pWolBuf);
	g_pWolBuf = NULL;
	//释放目录
	m_bFileExist = FALSE;
}


BOOL CParseWolFile::Legal(LPCSTR FileName, DWORD &Len)
{
	FILE *fp = fopen(FileName, "rb");
	if(!fp)
		return FALSE;

	fseek(fp, 0, 2);
	Len = ftell(fp);
	if(Len<sizeof(WolFileHeadInfo))
		return FALSE;
	fseek(fp, 0, 0);
	
	g_pWolBuf = new char[Len+10];
	memset(g_pWolBuf, 0, Len+10);
	fread(g_pWolBuf, Len, 1, fp);

	fclose(fp);

	char *p = g_pWolBuf;
	memcpy(&m_head, p, sizeof(WolFileHeadInfo));

	if(sizeof(WolFileHeadInfo)+m_head.wDocumentInfoLen+m_head.dwCoverLen+m_head.dwTextLen+m_head.dwCatalogLen+m_head.dwPagetableLen != Len)
		return FALSE;

	if(!m_head.wCatalogNum)
		return FALSE;

	p += Len-4;
	DWORD aa;
	memcpy(&aa, p, 4);
	if(aa!=0xffffffff)
		return FALSE;
	return TRUE;
}

////////////------------------------Cover
void CParseWolFile::DrawCover(CDC *pDC, CPoint point, CRect rect)
{
	char *pTemp = g_pWolBuf;
	char *CoverBuf = new char[m_head.dwCoverLen +1];
	memset(CoverBuf, 0x00, m_head.dwCoverLen+1);
	memcpy(CoverBuf, pTemp+sizeof(WolFileHeadInfo)+m_head.wDocumentInfoLen, m_head.dwCoverLen);
	
	WOLF_COVER_HEADINFO		CoverHead;
	memset(&CoverHead, 0x00, sizeof(WOLF_COVER_HEADINFO));
	memcpy(&CoverHead, CoverBuf, sizeof(WOLF_COVER_HEADINFO));
	
	DWORD i,cx,cy,picw,pich,picbytewidth;
	WORD Count = 1;
	
	picw = CoverHead.nWidth;
	pich = CoverHead.nHeight;
	Count = CoverHead.nBitcount;
	picbytewidth= CoverHead.nByteWidth;
	
	BYTE *respic = (BYTE *)malloc(picw * pich);
	
	if((picbytewidth * pich + 10) > m_head.dwCoverLen)
		DecodeGraphicsCode(respic, (BYTE*)CoverBuf + 10, m_head.dwCoverLen - 10);
	else
		memcpy(respic, (BYTE *)CoverBuf + 10, picbytewidth * pich);
	
	BYTE *pic = respic;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//开始Draw
	CBitmap bmp;
	CDC memDC;
	bmp.CreateCompatibleBitmap(pDC, picw, pich);
	memDC.CreateCompatibleDC(NULL);
	memDC.SelectObject(&bmp);
	memDC.PatBlt(0, 0, picw, pich, WHITENESS);
	rect.SetRect(0, 0, picw, pich);

	BYTE c;
	cx = cy = 0;
	int x = 0, y = 0;
	
	while(cy<pich)
	{
		x = 0;
		cx = 0;
		i = 0;
		int bitshow = 0;
		while(cx<picw)
		{
			if(Count == 1){
				if((cx&7) == 0){
					c=*pic++;
					i = 0;
				}
				if(c&PixelBit[i])
					memDC.SetPixel( x, y, 0 );
				else
					memDC.SetPixel( x, y, RGB(255,255,255));
				
			}
			if(Count == 2){
				if((cx&3) == 0){
					c=*pic++;
					i = 0;
				}
				int pc = (c >> (6 - i * 2)) & 0x03;
				if(pc == 0){	
					if(y>rect.top&&y<rect.bottom)
						memDC.SetPixel( x, y, RGB(0, 0, 0));					
				}
				else if(pc == 1){
					if(y>rect.top&&y<rect.bottom)
						memDC.SetPixel( x, y, RGB(85, 85, 85));				
				}
				else if(pc == 2){
					if(y>rect.top&&y<rect.bottom)
						memDC.SetPixel( x, y, RGB(170, 170, 170));					
				}
				else if(pc == 3){
					if(y>rect.top&&y<rect.bottom)
						memDC.SetPixel( x, y, RGB(255, 255, 255));				
				}
			}
			cx++;		
			x++;
			i++;
		}
		cy++;
		y++;
	}
	if(respic)
		free(respic);
	delete[] CoverBuf;
	int dstw=0,dsth=0;
 	int offsetx=0,offsety=0;
	if((float)picw/(float)pich<(float)(sysWidth-20)/(float)(sysHeight-HEAD_MARGIN*2)){
		dsth = (sysHeight-HEAD_MARGIN*2);
		dstw = (int)((float)picw*(float)(sysHeight-HEAD_MARGIN*2)/(float)pich);
		offsetx = (sysWidth-20-dstw)/2;
	}
	else{
		dstw = sysWidth-20;
		dsth = (int)((float)pich*(float)(sysWidth-20)/(float)picw);
		offsety = ((sysHeight-HEAD_MARGIN*2)-dsth)/2;
	}

	pDC->StretchBlt(point.x+LEFT_MARGIN+offsetx,point.y+HEAD_MARGIN+offsety,dstw,dsth,&memDC,0,0,picw,pich,SRCCOPY);
	memDC.DeleteDC();
	bmp.DeleteObject();
	
}
/////////////////------------end
void CParseWolFile::ShowFile(CPoint point, CRect RangeRect, DWORD ShowPage, CDC *pDC)
{
	if(ShowPage == 0)
		DrawCover(pDC, point, RangeRect);
	else{
		Draw(point, RangeRect, ShowPage, pDC);
	}
}



DWORD CParseWolFile::GetPagePos(int Page)
{
	if(!Page || Page>Total)
		return 0xffffffff;
	DWORD loc = sizeof(WolFileHeadInfo) + m_head.wDocumentInfoLen + m_head.dwCoverLen + 
					m_head.dwTextLen+m_head.dwCatalogLen;
	char* pt = strchr(g_pWolBuf+loc, '>');
	if(!pt)
		return 0xffffffff;	//页表格式错误
	pt += 1 + sizeof(DWORD)*6;	//页表位置
	DWORD pl_offset = *(DWORD*)(pt);
	DWORD pos = *(DWORD*)(g_pWolBuf+pl_offset + sizeof(DWORD)*(Page-1));
	return pos;
}

void CParseWolFile::Draw(CPoint point, CRect RangeRect, DWORD ShowPage, CDC *pDC)
{
	DWORD start = GetPagePos(ShowPage);
	if(start==0xffffffff)
		return;
	DWORD end = GetPagePos(ShowPage+1);
	if(end==0xffffffff || ShowPage==Total)
		end = sizeof(WolFileHeadInfo) + m_head.wDocumentInfoLen + m_head.dwCoverLen + m_head.dwTextLen;

	CPoint p(LEFT_MARGIN,HEAD_MARGIN);	//相对坐标
	BOOL EnTag = FALSE;

	char* content = NULL;
	int loc = 0;
	DWORD adds = 0;
	if(m_head.cDecryptType){
		DWORD offset = start-pWolf->dwTxtAdd;
		adds = offset%8;
		if(adds)
			offset -= adds;
		DWORD len = end-pWolf->dwTxtAdd-offset;
		DWORD adde;
		if(adde=len%8)
			len += 8-adde;
		content = (char*)malloc(len);
		memset(content, 0, len);
		memcpy(content,g_pWolBuf+pWolf->dwTxtAdd+offset, len);
		DecryptDataMain(pWolf, content, len);
		loc = adds;
	}
	else
		content = g_pWolBuf+start;

	for(; loc<end-start+adds; loc++){
		if(content[loc]>0){		//ACSII
			if(EnTag){	//标签处理
				char* temp = NULL;
				try{
					if(_strnicmp(content+loc, _T("img"),3)==0){
						int len;
						if(!DrawImage(pDC, content+loc, p, len))
							throw 1;
						temp = strchr(content+loc, '>');
						if(!temp)
							throw 1;
						loc = temp+1+len-content;
					}
					temp = strchr(content+loc, '>');
					if(!temp)
						throw 2;
				}
				catch(int err){
					char msg[100] = {0};
					switch(err){
					case 1:	//图像格式错误
						strcpy_s(msg, 100, "Image Format Error");
						break;
					case 2:
						strcpy_s(msg, 100, "Tag Format Error");
						break;
					}
					Tips(msg);
				}
				loc = temp - content;
				EnTag = FALSE;								
			}
			else{
				if(content[loc]=='<')
					EnTag = TRUE;
				else if(content[loc]=='\r'){	//硬回车
					loc++;
					p.x = LEFT_MARGIN;
					p.y += fontsize+LINE_MARGIN;
				}
				else if(content[loc]=='&'){
					if(loc+strlen("&nbsp;")<end-start+adds){
						if(_strnicmp(content+loc, "&nbsp;", strlen("&nbsp;"))==0){
							p.x+=fontsize/2;	//&nbsp; 处理
							loc += strlen("&nbsp;")-1;
							continue;
						}
						else if(_strnicmp(content+loc, "&lt;", strlen("&lt;"))==0){
							DrawText(p, pDC,"<", 1);
							loc += strlen("&lt;")-1;
							continue;
						}
						else if(_strnicmp(content+loc, "&gt;", strlen("&gt;"))==0){
							DrawText(p, pDC,">", 1);
							loc += strlen("&gt;")-1;
							continue;
						}
					}
					DrawText(p, pDC,content+loc, 1);
				}
				else{
					if(content[loc]==0x20)//半角空格
						p.x+=fontsize/2;
					else
						DrawText(p, pDC,content+loc, 1);
				}
			}
		}
		else{//中文字符
			if (content[loc]==-95 && content[loc+1]==-95)//全角空格
				p.x+=fontsize;
			else
				DrawText(p, pDC, content+loc, 2);
			loc++;
		}
	}
	
	if(m_head.cDecryptType)
		free(content);
}

void CParseWolFile::DrawText(CPoint &p, CDC* pDC, char *buf, int len)
{
	if(len==1){
		if(p.x+fontsize/2>sysWidth-RIGHT_MARGIN){
			p.x = LEFT_MARGIN;
			p.y += fontsize+LINE_MARGIN;
		}
		if(p.y>sysHeight)
			return;
	}
	else{//len==2
		if(p.x+fontsize>sysWidth-RIGHT_MARGIN){
			p.x = LEFT_MARGIN;
			p.y += fontsize+LINE_MARGIN;
		}
		if(p.y>sysHeight)
			return;
	}

	char t[3]={0};
	memcpy(t, buf, len);
	pDC->TextOut(p.x, p.y, t, len);
	if(len==1)
		p.x += fontsize/2;
	else
		p.x += fontsize;
}

BOOL CParseWolFile::DrawImage(CDC *pDC, char *buf, CPoint& p, int &len)
{
	int w=0,h=0,b=0,compat=0;
	if((w = GetTagVal(buf, _T("width="))) == -1)
		return FALSE;
	if((h = GetTagVal(buf, _T("height="))) == -1)
		return FALSE;
	if((len = GetTagVal(buf, _T("length="))) == -1)
		return FALSE;
	if((b = GetTagVal(buf, _T("bitcount="))) == -1)
		return FALSE;
	if((compat = GetTagVal(buf, _T("compact="))) == -1)
		return FALSE;
	char* start = strchr(buf, '>');
	if(!start)
		return FALSE;
	start++;
	BYTE *decodepic = (BYTE *)malloc(w*h);
	if(compat==1)
		DecodeGraphicsCode(decodepic, (BYTE*)start, len);
	else
		memcpy(decodepic, (BYTE *)start, len);

	if(p.x!=LEFT_MARGIN){
		p.x=LEFT_MARGIN;
		p.y+=LINE_MARGIN+fontsize;
	}

	CBitmap bmp;
	CDC memDC;
	bmp.CreateCompatibleBitmap(pDC, w, h);
	memDC.CreateCompatibleDC(NULL);
	memDC.SelectObject(&bmp);
	memDC.PatBlt(0, 0, w, h, WHITENESS);

	int x = 0, y = 0;
	int cx=0,cy=0;
	DWORD i;
	BYTE* pic=decodepic;
	BYTE c;

	while(cy<h)
	{
		x = 0;
		cx = 0;
		i = 0;
		int bitshow = 0;
		while(cx<w)
		{
			if(b == 1){	//二值
				if((cx&7) == 0){
					c=*pic++;
					i = 0;
				}
				if(c&PixelBit[i])
					memDC.SetPixel(x, y, RGB(0,0,0));
				else
					memDC.SetPixel(x, y, RGB(0xff,0xff,0xff));
			}
			if(b == 2){	//四级灰度
				if((cx&3) == 0){
					c=*pic++;
					i = 0;
				}
				int pc = (c >> (6 - i * 2)) & 0x03;
				if(pc == 0){	
					if(y<h&&y>=0)
						memDC.SetPixel(x, y, RGB(0,0,0));					
				}
				else if(pc == 1){
					if(y<h&&y>=0)
						memDC.SetPixel(x, y, RGB(0x55,0x55,0x55));				
				}
				else if(pc == 2){
					if(y<h&&y>=0)
						memDC.SetPixel(x, y, RGB(0xaa,0xaa,0xaa));
				}
				else if(pc == 3){
					if(y<h&&y>=0)
						memDC.SetPixel(x, y, RGB(0xff,0xff,0xff));				
				}
			}
			cx++;		
			x++;
			i++;
		}
		cy++;
		y++;
	}
	
	int nw,nh;
	nh = (int)((float)h*((float)(sysHeight-HEAD_MARGIN))/(float)800);
	nw = (int)((float)w*((float)(sysHeight-HEAD_MARGIN))/(float)800);
	
	pDC->StretchBlt(p.x,p.y,nw,nh,&memDC,0,0,w,h,SRCCOPY);
	memDC.DeleteDC();
	bmp.DeleteObject();

	p.x = LEFT_MARGIN;
	p.y += nh+LINE_MARGIN;
	if(decodepic)
		free(decodepic);
	return TRUE;
}

int CParseWolFile::GetTagVal(char *buf, char *tag)
{
	char* temp;
	char val[100] = {0};
	temp = strstr(buf, tag);
	if(!temp)
		return -1;
	temp+=strlen(tag);
	int count=0;
	while (temp[count]>='0' && temp[count]<='9'){
		val[count]=temp[count];
		count++;
	}
	return atoi(val);
}

BOOL CParseWolFile::GetInfo()
{
	if(!m_head.wDocumentInfoLen) return FALSE;
	char* DocumentInfo = new char[m_head.wDocumentInfoLen+1];
	memset(DocumentInfo, 0, m_head.wDocumentInfoLen+1);
	memcpy(DocumentInfo, g_pWolBuf+sizeof(WolFileHeadInfo), m_head.wDocumentInfoLen);
	char* temp = strstr(DocumentInfo, "<title>");
	if(!temp) return FALSE;
	char* temp2 = strstr(temp, "\x0d\x0a");
	if(!temp2) return FALSE;
	memset(m_BookName, 0, _MAX_PATH);
	memcpy(m_BookName,temp+7, temp2-temp-7);
	delete[] DocumentInfo;
	return TRUE;
}

void CParseWolFile::SetShowStyle(int style)
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

void CParseWolFile::CreateCataTree(CTreeCtrl* pTree)
{
	char *pTemp = g_pWolBuf;

	pTree->DeleteAllItems();
	HTREEITEM hRoot = pTree->InsertItem(m_BookName);
	pTree->SetItemData(hRoot,0);
	
	if(m_head.dwSubCatalogAdr){	//二级目录
		if(!GetCataTreeWithSub(pTree, hRoot))
			return;
	}
	else{	//一级目录
		pTemp += 9+sizeof(WolFileHeadInfo)+m_head.wDocumentInfoLen+m_head.dwCoverLen+m_head.dwTextLen;
		_CATAINFO Cata;
		char *pHead, *pTail;
		DWORD start = 1;
		for(int i=0; i<m_head.wCatalogNum; i++){
			memset(&Cata, 0, sizeof(_CATAINFO));
			pHead = NULL;
			pTail = NULL;
			pHead = strstr(pTemp, "<item>");
			pTail = strstr(pTemp, "</item>");
			if(!pHead||!pTail)
				return;
			strncpy_s(Cata.Title, _MAX_PATH, pHead+6, pTail-pHead-6);
			HTREEITEM hItem = pTree->InsertItem(Cata.Title,hRoot);
			start = GetPageNum(*(DWORD*)(pTail+7),start);
			pTree->SetItemData(hItem, start);
			pTemp += pTail-pTemp+11;
		}
	}
	pTree->Expand(hRoot, TVE_EXPAND);
	pTree->SelectItem(hRoot);
	pTree->Invalidate(FALSE);
}

BOOL CParseWolFile::GetCataTreeWithSub(CTreeCtrl* pTree, HTREEITEM hItem)
{
	_CATAINFO cata = {0};
	char *pTemp;
	//移动指针到<subcatalog>后
	DWORD MovOne = m_head.dwSubCatalogAdr+12;
	DWORD MovTwo = 0, MovThr = 0;

	DWORD CataNum = 0;

	int cnt=0;
	DWORD start=1;
	do{
		pTemp = g_pWolBuf;
		pTemp += MovOne;
		Subcatalog_Info subCata = {0};
		memcpy(&subCata, pTemp, sizeof(Subcatalog_Info));
		MovOne = subCata.BackCatalogPos;
		HTREEITEM hItem1  = pTree->InsertItem(subCata.ShortCatName,hItem);
		start = GetPageNum(subCata.CatalogPos,start);
		pTree->SetItemData(hItem1,start);
		if(subCata.ChildrenNum&&subCata.ChildCatalogPos){
			MovTwo = subCata.ChildCatalogPos;
			do{
				pTemp = g_pWolBuf;
				pTemp += MovTwo;
				Subcatalog_Info subCata2 = {0};
				memcpy(&subCata2, pTemp, sizeof(Subcatalog_Info));
				MovTwo = subCata2.BackCatalogPos;
				HTREEITEM hItem2  = pTree->InsertItem(subCata2.ShortCatName,hItem1);
				start = GetPageNum(subCata2.CatalogPos,start);
				pTree->SetItemData(hItem2,start);
				if(subCata2.ChildrenNum&&subCata2.ChildCatalogPos){
					MovThr = subCata.ChildCatalogPos;
					do{
						pTemp = g_pWolBuf;
						pTemp += MovThr;
						Subcatalog_Info subCata3 = {0};
						memcpy(&subCata3, pTemp, sizeof(Subcatalog_Info));
						HTREEITEM hItem3 = pTree->InsertItem(subCata3.ShortCatName,hItem2);
						start = GetPageNum(subCata3.CatalogPos,start);
						pTree->SetItemData(hItem3,start);
						MovThr = subCata3.BackCatalogPos;
						if(!MovThr)
							break;
					}while(1);
				}
				if(!MovTwo)
					break;
			}while(1);
		}
		if(!MovOne)
			break;
	}while(1);
	return TRUE;
}


DWORD CParseWolFile::GetPageNum(DWORD offset, DWORD start)
{
	DWORD lp=start;
	DWORD cur=0, next=0;
	cur = GetPagePos(start);
	offset+=sizeof(WolFileHeadInfo)+m_head.wDocumentInfoLen+m_head.dwCoverLen;
	for (;lp<Total;lp++){
		next = GetPagePos(lp+1);
		if(offset>=cur && offset<next)
			break;
		cur = next;
	}
	return lp;
}


int CParseWolFile::getStartPage(LPCSTR FileName)
{
	CString recordFileName = getPageCenterFilePath(FileName);
	PageCenter *pagecenter =(PageCenter*)malloc(sizeof(PageCenter));
	ZeroMemory(pagecenter, sizeof(PageCenter));

	CFile cf;
	if(!cf.Open(recordFileName,CFile::modeRead)){
	//	cf.Close();
	//	AfxMessageBox(recordFileName);
		return 0;
	}
	cf.Read(pagecenter,sizeof(PageCenter));
	cf.Close();
	return pagecenter->pageNo+1;

	return 0;
}

CString CParseWolFile::getPageCenterFilePath(LPCSTR FileName)
{
   
	CString recordFileName = FileName;
//	AfxMessageBox(FileName);
	recordFileName.Replace('\\','_');
//	AfxMessageBox(recordFileName);
	recordFileName.Replace(":_",":\\sys\\V2_Restore_PC\\_root_abook_");
	recordFileName += ".pg";
//	AfxMessageBox(recordFileName);
	return recordFileName;
}
