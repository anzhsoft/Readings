

#define SWITCH_ORDER
/*
#define SWITCH_WATERMARKER
#define SWITCH_NETRESOURCE
*/
const int SEARCHSOURCES = 12;


#define VERSION			3.020100324
#define SOFTWARE_NAME	"Readings-Wiki 分阅·同步·开放 [高校]用户桌面版"
#define SOFTWARE_LIFE	90

#define DOWN_MAXNUMBER	100
#define DefaultHost		"61.181.14.184:8088"//:8080"jinke-servers:8080"//"jinke-server:8080"//

#define SERVERLISTURL	"http://www.jinke.com.cn/v8servers.asp"//"http://www.jinke.com.cn/235servers.asp""http://www.jinke.com.cn/v8servers.asp"

#define UPGRADEURL		"http://61.181.14.184/readman/newestexe/version.asp"

#define FTPSET			"http://www.jinke.com.cn/istationftpset.asp"

#define DOWNTAXISNUM	100
#define LOCALSEARCHMAXIUM	60

enum{
	Is_Default,				//根节点
	Is_RecentlyRead,		//最近阅读
	Is_Readman,				//Readman根节点
	Is_Readman_News,		//Readman{新闻}
	Is_Readman_Notice,		//Readman{通告}
	Is_Readman_Known,		//Readman{知道}
	Is_Readman_Guide,		//Readman{导读}
	Is_Readman_Friend,		//Readman{友读}
	Is_Readman_Music,		//Readman{音乐}
	Is_Readman_Advertise,	//Readman{广告}
	Is_Readman_Method,		//Readman{读法}
	Is_Readman_Help,		//Readman{帮助}
	Is_Readman_Fonts,		//Readman{字库}
	Is_Readman_Tool,		//Readman{工具}
	Is_Readman_Install,		//Readman{安装}

	Is_PushFolder,			//推送箱
	Is_MyBookCase,			//本地读物
	Is_MyFavourate,			//收藏夹
	Is_MyFavourate_Folder,	//收藏夹子文件夹
	Is_Collect_Read,		//格式采集
	Is_Collect_Pic,
	Is_Collect_Media,
	Is_Collect_FileType,	//格式采集——文件类型
	Is_Collect_FileType_Year,//格式采集——文件类型——年份
	Is_OrderCenter,			//订阅中心
	Is_OrderList,			//订阅列表
	Is_OrderPaper,			//数字报纸
	Is_OrderPaper_Search,	//数字报纸搜索
	Is_OrderPaper_Node,
	Is_OrderMagzine,		//电子杂志
	Is_OrderMagzine_Search,	//电子杂志搜索
	Is_OrderMagzine_Node,

	Is_NetResource,			//网络读物
	Is_PubInfo,				//公告栏
	Is_NetLibrary,			//免费数图
	Is_Channel,				//频道
	Is_BookType,			//书籍类型
	Is_SearchHistory,		//搜索历史
	Is_ResourceShare,		//读物共享
	Is_ResourceShare_Hot,	
	Is_ResourceShare_Read,		//格式采集
	Is_ResourceShare_Pic,
	Is_ResourceShare_Media,
	Is_ResourceShare_FileType,	//网络共享资源
	Is_Download,			//正在下载
	Is_NetFriendList,		//在线好友
	Is_Chatroom,            //多个聊天室 added by zhanganzhan on 20090325
	Is_MyFriend,            //我单独的好友
	Is_NetFirend,
	Is_MyNetGroupList,		//我的部落
	Is_AllNetGroupList,		//读物部落
	Is_NetGroup_Level,		//读物部落中间节点
	Is_NetGroup,
	Is_MyNetGroup,			//我的读物部落
	
	Is_SDCard,				//移动存储卡
	Is_SDCard_Folder,		//SD卡文件夹
	
	Is_RickerGroup_Node,	//锐客部落中间节点
	Is_RickerGroup_Leaf,	//锐客部落叶节点
	Is_AllRickerGroupList,	//锐客部落

	Is_StudyHall,			//自习室根节点
	Is_MyStudyHall,			//我的自习室
	Is_School,				//学校
	Is_StudyBuilding,		//教学楼
	Is_Hall,				//自习室

	Is_Bookstore,          //读书频道
	Is_Zgsztug,             //中国数字图书馆
	Is_Tianya,				//天涯在线书库
	Is_Rdcxzx,              //瑞德超星在线图书馆
	Is_Qjzw,                 //全景中文图书
    Is_Essk,                  //E书时空
    Is_Dtzw,                 //大唐中文书库
    Is_Sxmd,                 //书香门第
    Is_Rmsc,                 //人民书城
    Is_Kyec,                 //快意恩仇书社
    Is_Sb,                   //书吧
    Is_Fhsc,                 //凤凰书城
    Is_Sjsc,                 //世纪书城
    Is_Zhsk,                 //中华书库
    Is_Sh,                   //书海
	Is_Dnsc,                 //电脑书城
    Is_Dssk,                 //读书时空

	Is_NetBook,              //网络原创读物
	Is_NetBookLeaf,			 //网络原创读物叶节点
	Is_MyResource,			 //我的读物部落
	Is_iStore,			    //istation的书店节点
	Is_iChannel,		    //istation 图书频道节点
	Is_iType,				//istation 的图书分类节点

	Is_HotKey,				//热词根节点
	Is_HotKeyleaf,			//热词叶节点

	Is_FanShuHotBook,				//番薯频道根节点
	Is_FanShuHotBookleaf,			//番薯频道叶节点
	Is_FanShu,  //番薯网
	Is_FanShuLeaf, //番薯网叶节点
	Is_RStore,//
};

enum{
	Is_FindList,			//搜索列表
	Is_BookList,			//书籍列表
	Is_OrdList,				//订阅列表
	Is_FileList,			//文件列表
	Is_PushList,
	Is_ShareList,
	Is_CollectList,
	Is_RecentlyList,
	Is_SDDiskList,
	Is_NetBookList,        //added by zuoss 090526 网络读物列表
};

enum{
	src_movepage_bottom = -2,
	src_movepage_top = -1,
	src_msg = 0,
	src_local,
	src_istation,
	src_baiduwenku,
	src_sinaishare,
	src_opencourse,
	src_baidu,
	src_baiduentity,//zuoss 081029
	src_duxiu,//zuoss 081104
	src_verycd,
	src_readman,
	src_order,
	src_zcom,
	src_sinabook,
	src_share,
	src_huntmine,
	src_netbook,//anzhsoft 090512
	src_mailbook,//anzhsoft 090615
	src_dangdang,
	src_fanshu,
};

enum{
	absmagFileType=-2,
	magFileType=-1,
	unknownFileType=0,    /*0*/
    pdfFileType,
    docFileType,
    chmFileType,
    zipFileType,
    rarFileType,            /*5*/
    picFileType,
    wolFileType,
    htmFileType,
    emlFileType,
    wocFileType,            /*10*/
    updFileType,
    txtFileType,
    cebFileType,
	                                                                                                   
    recentlyReadFileType=256,
    emailBoxFileType,
    newsFileType,
    bookFileType,
    journalFileType,	/*260*/
    blogFileType,
                                                                                                   
    jpgFileType,		/*262*/
    gifFileType,
    bmpFileType,
    mp3FileType,
	pptFileType,
	swfFileType,

	wavFileType=300,  
	wmvFileType,
	rmFileType,
	aviFileType,
	asfFileType,
	_3gpFileType,
	pngFileType,

	ttfFileType,
};

enum{
	td_msg,
	td_alert,
	td_tips,
	td_qst,
};

enum{
	dl_http,
	dl_p2p,
	dl_mail,
	dl_bt,
};

enum{
	from_Normal,
	from_SMS,
	from_rstore,
};
enum {
	downloadcertern, 
	manbooks, 
	readonline, 
	ordercertern
};
enum{
	type_socket,
	type_udt,
};

enum{
	PC2SD,
	SD2PC,
};

enum{
	Is_MyRead = 1,		//我读
	Is_MyListen = 2,	//我听
	Is_MyLook = 3,		//我看
	Is_MyOthers = 4,	//其他
};

enum{
	style_list = 0,
	style_report,
};

enum{
	thing_tree,
	body_tree,
};

enum SYNFLAG{
	syn_moveup = -4,
	syn_movedown = -3,
	syn_no = 0,
	syn_yes = 1,
};

enum ORDERSTYLE{
	order_by_isactive = 0,
	order_by_title = 1,
	order_by_downtimes = 2,
	order_by_createdate = 3,
	order_by_shareuser = 4,
	order_by_filetype = 5,
	order_by_filesize = 6,
};
// added by zhanganzhan on 20090911 for usb.
struct USB_DETAIL_INFO 
{
	CHAR DevName[100];
	CHAR PID[100];
	CHAR VID[100];
	CHAR ID[100];
	CHAR UserName[55];
	BOOL flag;
	CHAR diskFlag;
};
#define HANLINNAME "linux"
#define HANLINHEAD "usbstor"
#define HANLINVID  "file-stor_gadget"
#define HANLINPID_V3 "0312"
#define HANLINPID_V30 "0319"