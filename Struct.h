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
	int BlockSize;//块大小
	int LoadingBlocks[5];//各个线程正在下载的块
	int CurBlock;//下一个待下载的块
	int Ranges[5][2];
	char Reserved[956];
};//共1024字节

struct MissionInfo
{
	int Identifier;//任务编号
	int RetryTimes;//重试次数
	char Reserved[120];
};//共128字节，由主程序传入的一些信息

//*************************************************************************
//Channel文件存储一个ChannelFileHead和AllInfoNumber个ChannelInfo
//*************************************************************************
typedef struct
{
	DWORD UpdateFileTime;	//更新文件时间，存本地更新时间
	DWORD UpdateChannelTime;//频道更新时间，存网上的频道信息的更新时间
	DWORD AllInfoNumber;	//频道和书籍类型的总数量
	char  Reserve[20];		//保留字段
}ChannelFileHead;

typedef struct
{
	BYTE Type;//信息类型，1表示频道，2表示书籍类型, 3表示书店
	char ID[20];//唯一确定该项目
	char Name[60];//名称
	WORD Pages;//对于频道，是否展开；对于类型，页数
	DWORD Numbers;//对于频道，图标样式；对于类型，书籍个数
	char Reserve[39];//保留字段
}ChannelInfo;

typedef struct
{
	int clevel;
	int id;
	char name[200];
}OrderChannelInfo;//订阅树结构

typedef struct  
{
	char id[50];
	int type;		//文件类型
	char Last[50];	//最后更新日期
	char Cycle[50];	//更新周期
	char name[200];	//刊物名
	bool checked;	//是否订阅 
}OrderDetailInfo;

//*************************************************************************
//书籍类型下书籍信息存储结构，文件名是该类型的ID，
//*************************************************************************
typedef struct
{
	DWORD UpdateFileTime;//更新文件时间，存本地更新时间
	DWORD UpdateBookListTime;//频道更新时间，存网上的频道信息的更新时间
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
	char Size[20];//字节
	char Price[20];//分
	char Quality[16];//质量等级
	char Path[10];//存入路径
	char CertificateFlag[1];	 //add wutao
	char BookId[10];			 //add LiYoumeng
	char Abstract[210];			 //add LiYoumeng
	char flag;					 //0: normal, 1:goto page  add LiYoumeng
	char operation;				 //9不能下载，8可以制作，>5其他
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
	char ID[20];//类型ID
	DWORD UpdateTime;//更新时间
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
// 存储聊天内容
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
//存入用户信息的文件结构
//*************************************************************************
typedef struct
{
	BOOL RememberPw;//是否记住密码
	char UserName[32];//用户名称
	WORD Puzzle[4];//随机数字
	BYTE Pasword[20];//用户密码
	char Reserve[16];//保留字段
}UserInfo;
//*************************************************************************
//下载书籍的信息的文件结构
//*************************************************************************
typedef struct
{
	char BookName[150];
	char HanlinCode[18];
	char UserCardNo[16];
	char BookId[50];
	BYTE DownLoadState;//0＝等待下载；1＝下载多次失败；2＝正在下载实体；
						//3＝正在下载证书；4＝暂停下载；
	BYTE LoadingPort;//用来下载的标识
	BYTE TryTimes;//重试次数
	bool SucceedLoaded;//当前下载是否成功（成功删除，否则重新下载）
	char TypeStr[10];
	char Reserve;
	bool Certificate;//是否下载证书
	char WebPath[300];
	char SaveFileName[_MAX_PATH];
}DownLoadFileInfo;
//*************************************************************************
//*************************************************************************
//*************************************************************************
//存储系统信息
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
//存储服务器信息
//*************************************************************************
typedef struct
{
	char ServerIP[64];
	BYTE DefState;//0=默认
	BYTE SelServer;//1=选中
	char Description[64];
	char Reserve[126];
}DOWNLOADSERVERS;
//*************************************************************************
//*************************************************************************
//*************************************************************************
//质量等级对应列表
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
//文件头信息

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
	int flag;	//标示是group还是level
	int gid;		//groupID , LevelID
	int doflg;    //20080711  0：空线程； 1：非空，执行操作
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
	int  downloadType;//标志是http 下载还是tcp下载  zhanganzhan on 090415
	int  filefrom; //标志文件是群共享还是用户共享。0-群共享 ，1-用户共享
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

//拇指图尺寸 icon
#define THUMBNAIL_WIDTH		106		//88		+18
#define THUMBNAIL_HEIGHT	145		//131		+14

//列表拇指图尺寸
#define THUMBNAIL_WIDTH_LIST	80
#define THUMBNAIL_HEIGHT_LIST	100

//订阅
#define WIDTH_ORDER			200
#define HEIGHT_ORDER		118

typedef struct{
	char key[1024];
	char date[100];
	char res[100];
} HotKeyInfo;

#define MAXFINDLISTLOCAL	20	//本地搜索用  原来为10020080710


typedef CList<SOCKET, SOCKET&> CSocketList;
typedef CList<HANDLE, HANDLE&> CThreadList;
typedef CList<HTREEITEM, HTREEITEM&> CTreeItemList;
typedef CList<CWinThread*, CWinThread*> CWinThreadList;
typedef CList<LOCALLIST, LOCALLIST&> CLocalList;
typedef CList<MESSAGE,MESSAGE&> CMsgList;
typedef CList<HWND,HWND&> CWndList;

#endif
