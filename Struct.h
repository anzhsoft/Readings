#if !defined(AFX_STRUCT_H__4D7D8CA2_03E7_11D5_8FAF_0080C8E86C23__INCLUDED_)
#define AFX_STRUCT_H__4D7D8CA2_03E7_11D5_8FAF_0080C8E86C23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

struct MESSAGE{
	char buffer[BUFFER_SIZE];
};

struct DownLoadMissionInfo{
	char title[_MAX_PATH];
	char URL[1024];
	char DstPath[_MAX_PATH];
	char IP[20];
	char MD5[MD5LEN];
	int fileType;
	int sourceType;
	int port;
	int from;
	long fileLen;
	long finishedSize;
	long speed;
	long passTime;
	long restTime;
	char UserName[30];
	char bookID[50];
	char requestID[50];
	double process;
};

struct FindIconListInfo{
	int src;
	char cover[1024];
};

struct FileHead
{
	int BlockSize;//���С
	int LoadingBlocks[5];//�����߳��������صĿ�
	int CurBlock;//��һ�������صĿ�
	int Ranges[5][2];
	char Reserved[956];
};//��1024�ֽ�

struct MissionInfo
{
	int Identifier;//������
	int RetryTimes;//���Դ���
	char Reserved[120];
};//��128�ֽڣ������������һЩ��Ϣ

//*************************************************************************
//Channel�ļ��洢һ��ChannelFileHead��AllInfoNumber��ChannelInfo
//*************************************************************************
typedef struct
{
	DWORD UpdateFileTime;	//�����ļ�ʱ�䣬�汾�ظ���ʱ��
	DWORD UpdateChannelTime;//Ƶ������ʱ�䣬�����ϵ�Ƶ����Ϣ�ĸ���ʱ��
	DWORD AllInfoNumber;	//Ƶ�����鼮���͵�������
	char  Reserve[20];		//�����ֶ�
}ChannelFileHead;

typedef struct
{
	BYTE Type;//��Ϣ���ͣ�1��ʾƵ����2��ʾ�鼮����, 3��ʾ���
	char ID[20];//Ψһȷ������Ŀ
	char Name[60];//����
	WORD Pages;//����Ƶ�����Ƿ�չ�����������ͣ�ҳ��
	DWORD Numbers;//����Ƶ����ͼ����ʽ���������ͣ��鼮����
	char Reserve[39];//�����ֶ�
}ChannelInfo;

typedef struct
{
	int clevel;
	int id;
	char name[200];
}OrderChannelInfo;//�������ṹ

typedef struct  
{
	char id[50];
	int type;		//�ļ�����
	char Last[50];	//����������
	char Cycle[50];	//��������
	char name[200];	//������
	bool checked;	//�Ƿ��� 
}OrderDetailInfo;

//*************************************************************************
//�鼮�������鼮��Ϣ�洢�ṹ���ļ����Ǹ����͵�ID��
//*************************************************************************
typedef struct
{
	DWORD UpdateFileTime;//�����ļ�ʱ�䣬�汾�ظ���ʱ��
	DWORD UpdateBookListTime;//Ƶ������ʱ�䣬�����ϵ�Ƶ����Ϣ�ĸ���ʱ��
	char Reserve[24];
}BookInfoFileHead;

typedef struct
{
	char BookName[200];
	char Author[50];
	char CreateDate[50];
	char FileType[50];
	char Size[50];
	char Path[_MAX_PATH];
	SYNFLAG syn;
}DiskBookInfo;


typedef struct
{
	char BookName[88];
	char RefreshData[12];
	char HanlinCode[20];
	char Author[40];
	char VisitNumber[10];
	char Publisher[50];
	char Size[20];//�ֽ�
	char Price[20];//��
	char Quality[16];//�����ȼ�
	char Path[10];//����·��
	char CertificateFlag[1];	 //add wutao
	char BookId[10];			 //add LiYoumeng
	char Abstract[210];			 //add LiYoumeng
	char flag;					 //0: normal, 1:goto page  add LiYoumeng
	char operation;				 //9�������أ�8����������>5����
	char Reserve[3];
}BookInfo;

typedef struct
{
	char Date[12];
	char BookName[88];
	char HanlinCode[12];
	char User[50];
	char VisitNumber[10];
	char DownNumber[10];
	char FileName[100];
	char Size[20];
}AttentionInfo;

typedef struct
{
	char BookName[88];
	char HanlinCode[20];
	char Quality[16];
	char VisitNumber[10];
	char DownNumber[10];
	char Recommand[12];
	char Author[40];
	char Publisher[50];
	char Size[15];
	char Type[10];
	char CertificateFlag[1];	 //add wutao
	char BookId[10];			//add LiYoumeng
	char Reserve[8];
}FriendBookInfo;

//*************************************************************************
//*************************************************************************
typedef struct
{
	char ID[20];//����ID
	DWORD UpdateTime;//����ʱ��
}BookTypeUpdateTime;

typedef struct
{
	BYTE Type;
	char IndexID[10];
	WORD TotlePage;
	WORD CurrentPage;
	WORD CurrentDoanLoad;
	DWORD BookNumber;
	char * KeyWords;
	char * url;
}TreeItemInfo;
// �洢��������
struct msgNode//added by zhanganzhan on 20090326 for chatrooms
{
	char name[100];
	char content[BUFFER_SIZE];
	char time[100];
	char chatroomID[100];
};
typedef struct
{
	char HanlinCode[50];
	char BookName[100];
	char SaveCifPath[250];
}CifDownInfo;
//*************************************************************************
//�����û���Ϣ���ļ��ṹ
//*************************************************************************
typedef struct
{
	BOOL RememberPw;//�Ƿ��ס����
	char UserName[32];//�û�����
	WORD Puzzle[4];//�������
	BYTE Pasword[20];//�û�����
	char Reserve[16];//�����ֶ�
}UserInfo;
//*************************************************************************
//�����鼮����Ϣ���ļ��ṹ
//*************************************************************************
typedef struct
{
	char BookName[150];
	char HanlinCode[18];
	char UserCardNo[16];
	char BookId[50];
	BYTE DownLoadState;//0���ȴ����أ�1�����ض��ʧ�ܣ�2����������ʵ�壻
						//3����������֤�飻4����ͣ���أ�
	BYTE LoadingPort;//�������صı�ʶ
	BYTE TryTimes;//���Դ���
	bool SucceedLoaded;//��ǰ�����Ƿ�ɹ����ɹ�ɾ���������������أ�
	char TypeStr[10];
	char Reserve;
	bool Certificate;//�Ƿ�����֤��
	char WebPath[300];
	char SaveFileName[_MAX_PATH];
}DownLoadFileInfo;
//*************************************************************************
//*************************************************************************
//*************************************************************************
//�洢ϵͳ��Ϣ
//*************************************************************************
typedef struct
{
	char ServerName[64];
	char NewVersion[10];
	BYTE ShowMouseMsg;
	char Reserve[255];
}SYSTEMINFO;
//*************************************************************************
//*************************************************************************
//*************************************************************************
//�洢��������Ϣ
//*************************************************************************
typedef struct
{
	char ServerIP[64];
	BYTE DefState;//0=Ĭ��
	BYTE SelServer;//1=ѡ��
	char Description[64];
	char Reserve[126];
}DOWNLOADSERVERS;
//*************************************************************************
//*************************************************************************
//*************************************************************************
//�����ȼ���Ӧ�б�
//*************************************************************************
typedef struct
{
	char	TEXT[8];
	char	ShowChr[8];
	char	SerialNo[4];
	WORD	ShowNumber;
	DWORD   ShowColor;
	HBITMAP hBitmap;
	char	Reserve[6];
}TEXTTOSHOW;
//*************************************************************************
//*************************************************************************
//�ļ�ͷ��Ϣ

typedef struct
{
	int Reserve;
	int FileType;
	char Title[256];
	char Path[1024];
}READINGS_SYS;

typedef struct
{
	int FileType;
	char Title[256];
	char Type[20];
	char FilePath[1024];
}LOCALLIST;

typedef struct{
	char title[200];
	char link[1024];
	char cover[1024];
	char abst1[1000];
	char abst2[1000];
	char abst3[1000];
	char score[100];
	int src;
	int icon;
	BOOL isAlive;
}FINDINFO;

struct ThreadInfo{
	char WebPath[1024];
	int id;
	CWnd* pWnd;
	HANDLE handle;
	int flag;	//��ʾ��group����level
	int gid;		//groupID , LevelID
	int doflg;    //20080711  0�����̣߳� 1���ǿգ�ִ�в���
	BOOL isAlive;
};

struct PREVIEWMAGHEAD{
	char	szFlag[16];
	char	szFounder[16];
	char	version[8];
	DWORD 	length;
	DWORD 	segNum;
	DWORD	covOffset;
	char	reserve[12];
};

struct SEGHEAD{
	char	relativePath[256];
	DWORD	fileLength;
	int		fileType;
};

struct ShareInfo
{
	long sid;
	char title[200];
	int filetype;
	char type[100];
	char UserName[50];
	BOOL isAlive;
	char LWT[50];
	long DownTimes;
	char intro[1024];
	char group[100];
	long CommentTimes;
	char url[1024];
	int  downloadType;//��־��http ���ػ���tcp����  zhanganzhan on 090415
	int  filefrom; //��־�ļ���Ⱥ�������û�����0-Ⱥ���� ��1-�û�����
	long filesize;
	char hash[1024];
};

enum{
	file_copy,
	file_move,
};

enum{ 
	show_normal, 
	show_large, 
};

//Ĵָͼ�ߴ� icon
#define THUMBNAIL_WIDTH		106		//88		+18
#define THUMBNAIL_HEIGHT	145		//131		+14

//�б�Ĵָͼ�ߴ�
#define THUMBNAIL_WIDTH_LIST	80
#define THUMBNAIL_HEIGHT_LIST	100

//����
#define WIDTH_ORDER			200
#define HEIGHT_ORDER		118

typedef struct{
	char key[1024];
	char date[100];
	char res[100];
} HotKeyInfo;

#define MAXFINDLISTLOCAL	20	//����������  ԭ��Ϊ10020080710


typedef CList<SOCKET, SOCKET&> CSocketList;
typedef CList<HANDLE, HANDLE&> CThreadList;
typedef CList<HTREEITEM, HTREEITEM&> CTreeItemList;
typedef CList<CWinThread*, CWinThread*> CWinThreadList;
typedef CList<LOCALLIST, LOCALLIST&> CLocalList;
typedef CList<MESSAGE,MESSAGE&> CMsgList;
typedef CList<HWND,HWND&> CWndList;

#endif
