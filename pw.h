#if !defined(AFX_STRUCT1_H__4D7D8CA2_03E7_11D5_8FAF_0080C8E86C23__INCLUDED_)
#define AFX_STRUCT1_H__4D7D8CA2_03E7_11D5_8FAF_0080C8E86C23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


//�ļ�ͷ��Ϣ

#define SCREENTYPE_SMALL_16		1	//16����������ʾ��480X640
#define SCREENTYPE_SMALL_24		2	//24����������ʾ��480X640
#define SCREENTYPE_RSMALL_16		3	//16���󣬷�ת��ʾ��480X640
#define SCREENTYPE_RSMALL_24		4	//24���󣬷�ת��ʾ��480X640
#define SCREENTYPE_BIG_24		5	//24����������ʾ��640X880
#define SCREENTYPE_BIG_32		6	//32����������ʾ��640X880
#define SCREENTYPE_RBIG_24		7	//24���󣬷�ת��ʾ��640X880
#define SCREENTYPE_RBIG_32		8	//32���󣬷�ת��ʾ��640X880

//ҳ�����  С
#define HEIGHT	580
#define WIDTH	470

//������ʾ���� С
#define FONT_SIZE_NORMAL	18	//С��	24
#define LINE_MARGIN			8//12	//�м��
#define LEFT_MARGIN			10	//������
#define RIGHT_MARGIN		10	//������
#define HEAD_MARGIN			10	//ҳü

//ҳ�����  ��
#define HEIGHT_LARGE	800
#define WIDTH_LARGE		620

//������ʾ���� ��
#define FONT_SIZE_LARGE	24	//С��	24




typedef struct
{
	char	szFlag[16];				//0	16	��־���汾��Ϣ
	BYTE	cDocumentType;			//16	1	�ĵ�����(no used)
	BYTE	cLanguage;				//17	1	����
	BYTE	cFormat;				//18	1	����
	DWORD	dwFileLen;				//19	4	�ĵ�����(no used)
	WORD	wDocumentInfoLen;		//23	2	�ĵ���Ϣ�γ���
	DWORD	dwCoverLen;				//25	4	���泤��
	BYTE	cCoverState;			//29	1	����״̬(no used)
	DWORD	dwCatalogLen;			//30	4	Ŀ¼����
	WORD	wCatalogNum;			//34	2	Ŀ¼��
	WORD	wQuestionNum;			//36	2	��������(no used)
	DWORD	dwTextLen;				//38	4	���ĳ���
	WORD	wAppendWordLen;			//42	2	�����ֿⳤ��(no used)
	DWORD	dwPageInfoLen;			//44	4	��ҳ��Ϣ�γ���(no used)
	BYTE	cFontType;				//48	1	������Ϣ(no used)
	BYTE	cFontSize;				//49	1	�ֺ���Ϣ(no used)
	WORD	wDispWidth;				//50	2	��ʾ���(no used)
	WORD	wDispHight;				//52	2	��ʾ�߶�(no used)
	BYTE	cDispFormat;			//54	1	�����ʽ(no used)
	BYTE	cDecryptType;			//55	1	���ܷ�ʽ
	DWORD	dwJapaAppendLen;		//56	4	Japa���Ӷγ���(no used)
	DWORD	dwPagetableLen;			//60	4	ҳ����Ϣ�γ���
	BYTE	cSourceFileType;		//64	1	Դ�ļ���ʽ����(no used)
	BYTE	cFileType;				//65	1	�ļ�����
	DWORD	dwDispTotalPage;		//66	4	���������ʾ����ҳ��
	BYTE	cHaveCatalogFlag;		//70	1	�Ƿ���Ŀ¼��־(no used)
	DWORD	dwMode;					//71	4	ҳ����ʽ��Ϣ��(no used)
	DWORD	dwDispTotalPage2;		//75	4	���������ʾ����ҳ��(����)
	BYTE	cAssisKey;				//79	1	����������
	DWORD	dwWaterMarkAdr;			//80	4	ˮӡͼƬ��ַ
	BYTE	cWaterMarkType;			//84	1	ˮӡͼƬ����
	BYTE	cCatalogPic;			//85	1	ͼ�μ���Ŀ¼	00h:��;01h����;02h:˫��
	BYTE	cbook;					//86		�鼮���Ʊ�ʶ	00h:��; 01h��Ѱ�;02h:ע���
	DWORD	dwNoteTableLen;			//87	4	ע�͵�ַ����
	DWORD	dwAdvertisementLen;		//91	4	��泤��
	WORD	wSubCatalogNum;			//95		��Ŀ¼��	���ֽ���ǰ
	DWORD	dwSubCatalogAdr;		//97		��Ŀ¼λ�ã�������ļ�ͷ
	WORD	wKeywordLen;			//101	2	�ؼ��ֳ���
	BYTE	cCataDispStyle;			//103	1	һ������ʾĿ¼���� 0��ʾ8����40��ʾ40��
	char	cReserve[1];			//104	1	����
	DWORD	dwIndexLen;				//105	4	��������
	char	cReserve1[19];			//109	19	����
}WolFileHeadInfo;//��128

typedef struct
{
	CHAR	title[128];				//����
	CHAR	subject[128];				//����,���
	CHAR	author[128];				//����
	CHAR	adpter[128];				//�ı���
	CHAR	translator[128];			//������
	CHAR	time_write[16];				//д������
	CHAR	publisher[128];				//������
	CHAR	time_publish[16];			//��������
	CHAR	rights[128];				//��Ȩ
	CHAR	charset[128];				//����
	CHAR	patent[128];				//ר����
	CHAR	introduction[1024];			//���ݼ��
	CHAR	ISBN[128];				//���ʱ�׼ͼ����
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

#define Warning							"����"
#define OpenFileFail						"���ļ�ʱ���ִ���,\n���ļ�������ȷ��ʾ!"

struct _CATAINFO
{
	char Title[_MAX_PATH];
	WORD  Lev;
	DWORD pos;
};
typedef struct{
	DWORD	CatalogPos;			//Ŀ¼��Ӧ�������������е�λ�ã���������ĵ�λ�ã���<wolf>��ǩ��λ�ã��ĸ��ֽ�
	DWORD	CatalogNamePos;			//Ŀ¼����Ŀ¼������λ�ã������Ŀ¼������ʼ��λ�ã��ĸ��ֽ�
	WORD	CatalogNameLen;			//Ŀ¼���Ƴ��ȣ������ֽ�
	WORD	ChildrenNum;			//��һ����Ŀ¼�ĸ���������Ŀ¼����Ϊ0��������һ����Ŀ¼
	DWORD	FrontCatalogPos;		//ǰһ��ͬ��Ŀ¼��λ��
	DWORD	BackCatalogPos;		//��һ��ͬ��Ŀ¼��λ��
	DWORD	ChildCatalogPos;		//��һ����Ŀ¼��λ�ã���Ϊ0xFFFFFFFF��������һ����Ŀ¼
	DWORD	ParentCatalogPos;		//��һ����Ŀ¼��λ��
	DWORD	CatalogLayNo;			//Ŀ¼�Ĳ�ṹ���ĸ��ֽ�
	CHAR	ShortCatName[40];	//���ļ���������Ŀ¼��ֱ����ʾ
	CHAR	Reserved[8];		//����
}Subcatalog_Info;				//ÿ���ṹ�干80�ֽ�


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

#define DECRYPT_MODE_NONE						0//������
#define DECRYPT_MODE_11							1//sim��id������
#define DECRYPT_MODE_13							2//sim��id idea����
#define DECRYPT_MODE_15							3//sim��id�Ŵ�������
#define DECRYPT_MODE_16							4//sim��id idea֤���ļ�����
#define DECRYPT_MODE_21							5//�ն�id������
#define DECRYPT_MODE_31							6//sm��id������
#define DECRYPT_MODE_51							7//�ն˰���������
#define DECRYPT_MODE_43							8//sim��Ԥ����idea����
#define DECRYPT_MODE_D4							9//rsa idea����
#define DECRYPT_MODE_WATERMARK					10//����ˮӡ
#define DECRYPT_MODE_NOTSUPPORT					0x101//��֧�ֵļ��ܷ�ʽ

#define WF_EN_SUCCESS				0X0
#define WF_EN_NOMEMORY				0X101	//�ڴ治��
#define WF_EN_OPENFILEFAIL			0X102	//�޷����ļ�
#define WF_EN_BADCIFFILE			0X10B	//���ǺϷ���cif�ļ�
#define WF_EN_OUTOFDECRYPT			0X10D	//��֧�ֵļ��ܷ�ʽ

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
	WORD IDEA_Key[WF_IDEA_KEY_NUM];	//IDEA������
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