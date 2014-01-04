#if !defined(AFX_STRUCT1_H__4D7D8CA2_03E7_11D5_8FAF_0080C8E86C23__INCLUDED_)
#define AFX_STRUCT1_H__4D7D8CA2_03E7_11D5_8FAF_0080C8E86C23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


//文件头信息

#define SCREENTYPE_SMALL_16		1	//16点阵，正常显示，480X640
#define SCREENTYPE_SMALL_24		2	//24点阵，正常显示，480X640
#define SCREENTYPE_RSMALL_16		3	//16点阵，翻转显示，480X640
#define SCREENTYPE_RSMALL_24		4	//24点阵，翻转显示，480X640
#define SCREENTYPE_BIG_24		5	//24点阵，正常显示，640X880
#define SCREENTYPE_BIG_32		6	//32点阵，正常显示，640X880
#define SCREENTYPE_RBIG_24		7	//24点阵，翻转显示，640X880
#define SCREENTYPE_RBIG_32		8	//32点阵，翻转显示，640X880

//页面参数  小
#define HEIGHT	580
#define WIDTH	470

//正文显示字体 小
#define FONT_SIZE_NORMAL	18	//小字	24
#define LINE_MARGIN			8//12	//行间距
#define LEFT_MARGIN			10	//左留白
#define RIGHT_MARGIN		10	//右留白
#define HEAD_MARGIN			10	//页眉

//页面参数  大
#define HEIGHT_LARGE	800
#define WIDTH_LARGE		620

//正文显示字体 大
#define FONT_SIZE_LARGE	24	//小字	24




typedef struct
{
	char	szFlag[16];				//0	16	标志及版本信息
	BYTE	cDocumentType;			//16	1	文档类型(no used)
	BYTE	cLanguage;				//17	1	语言
	BYTE	cFormat;				//18	1	版面
	DWORD	dwFileLen;				//19	4	文档长度(no used)
	WORD	wDocumentInfoLen;		//23	2	文档信息段长度
	DWORD	dwCoverLen;				//25	4	封面长度
	BYTE	cCoverState;			//29	1	封面状态(no used)
	DWORD	dwCatalogLen;			//30	4	目录长度
	WORD	wCatalogNum;			//34	2	目录数
	WORD	wQuestionNum;			//36	2	问题数量(no used)
	DWORD	dwTextLen;				//38	4	正文长度
	WORD	wAppendWordLen;			//42	2	附加字库长度(no used)
	DWORD	dwPageInfoLen;			//44	4	分页信息段长度(no used)
	BYTE	cFontType;				//48	1	字体信息(no used)
	BYTE	cFontSize;				//49	1	字号信息(no used)
	WORD	wDispWidth;				//50	2	显示宽度(no used)
	WORD	wDispHight;				//52	2	显示高度(no used)
	BYTE	cDispFormat;			//54	1	版面格式(no used)
	BYTE	cDecryptType;			//55	1	加密方式
	DWORD	dwJapaAppendLen;		//56	4	Japa附加段长度(no used)
	DWORD	dwPagetableLen;			//60	4	页表信息段长度
	BYTE	cSourceFileType;		//64	1	源文件格式类型(no used)
	BYTE	cFileType;				//65	1	文件类型
	DWORD	dwDispTotalPage;		//66	4	在书架上显示的总页数
	BYTE	cHaveCatalogFlag;		//70	1	是否有目录标志(no used)
	DWORD	dwMode;					//71	4	页表样式信息段(no used)
	DWORD	dwDispTotalPage2;		//75	4	在书架上显示的总页数(大屏)
	BYTE	cAssisKey;				//79	1	辅助加密字
	DWORD	dwWaterMarkAdr;			//80	4	水印图片地址
	BYTE	cWaterMarkType;			//84	1	水印图片类型
	BYTE	cCatalogPic;			//85	1	图形检索目录	00h:无;01h单列;02h:双列
	BYTE	cbook;					//86		书籍限制标识	00h:无; 01h免费版;02h:注册版
	DWORD	dwNoteTableLen;			//87	4	注释地址表长度
	DWORD	dwAdvertisementLen;		//91	4	广告长度
	WORD	wSubCatalogNum;			//95		子目录数	低字节在前
	DWORD	dwSubCatalogAdr;		//97		子目录位置，相对于文件头
	WORD	wKeywordLen;			//101	2	关键字长度
	BYTE	cCataDispStyle;			//103	1	一屏内显示目录个数 0表示8个，40表示40个
	char	cReserve[1];			//104	1	保留
	DWORD	dwIndexLen;				//105	4	索引表长度
	char	cReserve1[19];			//109	19	保留
}WolFileHeadInfo;//共128

typedef struct
{
	CHAR	title[128];				//标题
	CHAR	subject[128];				//主题,类别
	CHAR	author[128];				//作者
	CHAR	adpter[128];				//改编者
	CHAR	translator[128];			//翻译者
	CHAR	time_write[16];				//写作日期
	CHAR	publisher[128];				//出版商
	CHAR	time_publish[16];			//出版日期
	CHAR	rights[128];				//版权
	CHAR	charset[128];				//编码
	CHAR	patent[128];				//专利号
	CHAR	introduction[1024];			//内容简介
	CHAR	ISBN[128];				//国际标准图书编号
}BookBaseInfo;

#define List_TITLE						"<title>"
#define List_SUBJECT					"<subject>"
#define List_AUTHOR						"<author>"
#define List_ADPTER						"<adpter>"
#define List_TRANSLATOR					"<translator>"
#define List_TIMEWRITE					"<time_write>"
#define List_PUBLISHER					"<publisher>"
#define List_TIMEPUBLISH				"<time_publish>"
#define List_RIGHTS						"<rights>"
#define List_CHARSET					"<charset>"
#define List_PATENT						"<patent>"
#define List_INTRODUCTION				"<introduction>"
#define List_ISBN						"<ISBN>"
#define List_PUBLISHINGCORPORATION		"<publishing_corporation>"

#define Warning							"错误"
#define OpenFileFail						"打开文件时出现错误,\n此文件不能正确显示!"

struct _CATAINFO
{
	char Title[_MAX_PATH];
	WORD  Lev;
	DWORD pos;
};
typedef struct{
	DWORD	CatalogPos;			//目录对应的内容在文章中的位置，相对于正文的位置，即<wolf>标签的位置，四个字节
	DWORD	CatalogNamePos;			//目录名在目录名集的位置，相对于目录名集开始的位置，四个字节
	WORD	CatalogNameLen;			//目录名称长度，两个字节
	WORD	ChildrenNum;			//下一级子目录的个数，若子目录个数为0，则无下一级子目录
	DWORD	FrontCatalogPos;		//前一个同级目录的位置
	DWORD	BackCatalogPos;		//后一个同级目录的位置
	DWORD	ChildCatalogPos;		//下一级子目录的位置，若为0xFFFFFFFF，则无下一级子目录
	DWORD	ParentCatalogPos;		//上一级父目录的位置
	DWORD	CatalogLayNo;			//目录的层结构，四个字节
	CHAR	ShortCatName[40];	//短文件名，用于目录的直接显示
	CHAR	Reserved[8];		//保留
}Subcatalog_Info;				//每个结构体共80字节


typedef struct
{
	WORD nFlag;
	WORD nWidth;
	WORD nBitcount;
	WORD nByteWidth;
	WORD nHeight;
//	DWORD dwCompressLen;
//	char Reserve[18];
}WOLF_COVER_HEADINFO;

#define DECRYPT_BINDLENGTH						64
#define DECRYPT_SIMIDLENGTH						16
#define DECRYPT_TERMINALIDLENGTH				32
#define DECRYPT_SMCARDIDLENGTH					32
#define DECRYPT_IDEALENGTH						8

#define DECRYPT_IDEA_GROUPLEN					8

#define DECRYPT_MODE_NONE						0//不加密
#define DECRYPT_MODE_11							1//sim卡id异或加密
#define DECRYPT_MODE_13							2//sim卡id idea加密
#define DECRYPT_MODE_15							3//sim卡id放大异或加密
#define DECRYPT_MODE_16							4//sim卡id idea证书文件加密
#define DECRYPT_MODE_21							5//终端id异或加密
#define DECRYPT_MODE_31							6//sm卡id异或加密
#define DECRYPT_MODE_51							7//终端绑定字异或加密
#define DECRYPT_MODE_43							8//sim卡预设字idea加密
#define DECRYPT_MODE_D4							9//rsa idea加密
#define DECRYPT_MODE_WATERMARK					10//数字水印
#define DECRYPT_MODE_NOTSUPPORT					0x101//不支持的加密方式

#define WF_EN_SUCCESS				0X0
#define WF_EN_NOMEMORY				0X101	//内存不足
#define WF_EN_OPENFILEFAIL			0X102	//无法打开文件
#define WF_EN_BADCIFFILE			0X10B	//不是合法的cif文件
#define WF_EN_OUTOFDECRYPT			0X10D	//不支持的加密方式

#define WF_IDEA_ROUNDS				8
#define WF_IDEA_KEY_LEN				16
#define WF_IDEA_KEY_NUM				(6*WF_IDEA_ROUNDS+4)

/*
#define IDEA_DECODE_ROUNDS				WF_IDEA_ROUNDS
#define IDEA_DECODEKEY_LEN				WF_IDEA_KEY_LEN
#define IDEA_SUBKEY_NUM					WF_IDEA_KEY_NUM
*/

typedef struct
{
	WORD wDecryptType;
	WORD wDecryptLen;
	char* pDecryptCode;
	WORD IDEA_Key[WF_IDEA_KEY_NUM];	//IDEA解密字
	char pFileName[_MAX_PATH];
	char szBookName[_MAX_PATH];
	DWORD dwTxtAdd;
	char CifName[_MAX_PATH];
}_WOLFDECRYPT;

#define low16(x) ((x) & 0xffff)

//decypt funtion
BOOL perDecrypt(LPCSTR WolFilePath, char* CifFilePath,_WOLFDECRYPT* &pWolfAtt, char* g_pWolBuf);
WORD GetDecryptMode(BYTE cDecryptType);
BOOL GetBookName(char *Name, char* g_pWolBuf);
WORD DecryptDataMain(_WOLFDECRYPT *pWolReaderInfo,char* szBuffer,DWORD dwBufLen);
WORD InitDecryptCode(_WOLFDECRYPT *pWolReaderInfo);
void Wolf_GetSimCardID(char *pCodeBuffer);
WORD LoadCIFFile(WORD *pUserKey,_WOLFDECRYPT *pWolReaderInfo);
BOOL IsCIFFileValidate(FILE* pFilePtr,_WOLFDECRYPT *pWolReaderInfo,LPBYTE szRsaKey,int nKeyLen);
void GetDecryptCodeFromSimID(BYTE *szKey,WORD *IdeaKey,BYTE *szIdeaKey,WORD SimKeyLen);
void SubKeyCreate(WORD *SubKey,WORD *szSimKey);
void SubKeyChanged(WORD *Z,WORD *DK);
void IdeaDecrypt(WORD *in,WORD *out,WORD *Z);
//

typedef BOOL (CALLBACK* lpSetFuncFilePath)(LPCTSTR FilePath);
typedef BOOL (CALLBACK* lpSetWolfFileName)(LPCTSTR WolfFileName);
typedef BOOL (CALLBACK* lpSetWolfBuffer)(BYTE *Buf, DWORD BufSize);
typedef BOOL (CALLBACK* lpSetWolfInfo)(BYTE DisType,long ForeColor,long BackColor);
typedef BOOL (CALLBACK* lpDisplayOnePage)(CDC *pDC, POINT pos, DWORD PagePos,DWORD PageEnd);
typedef BOOL (CALLBACK* lpAddPageList)(LPCTSTR FileName);
typedef BOOL (CALLBACK* lpEndThread)();
typedef INT (CALLBACK* lpGetWorkPos)();
typedef VOID (CALLBACK* lpInitEnvironment)();
typedef DWORD (CALLBACK* lpGetTotalPage)();


#endif