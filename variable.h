

#define SWITCH_ORDER
/*
#define SWITCH_WATERMARKER
#define SWITCH_NETRESOURCE
*/
const int SEARCHSOURCES = 12;


#define VERSION			3.020100324
#define SOFTWARE_NAME	"Readings-Wiki ���ġ�ͬ�������� [��У]�û������"
#define SOFTWARE_LIFE	90

#define DOWN_MAXNUMBER	100
#define DefaultHost		"61.181.14.184:8088"//:8080"jinke-servers:8080"//"jinke-server:8080"//

#define SERVERLISTURL	"http://www.jinke.com.cn/v8servers.asp"//"http://www.jinke.com.cn/235servers.asp""http://www.jinke.com.cn/v8servers.asp"

#define UPGRADEURL		"http://61.181.14.184/readman/newestexe/version.asp"

#define FTPSET			"http://www.jinke.com.cn/istationftpset.asp"

#define DOWNTAXISNUM	100
#define LOCALSEARCHMAXIUM	60

enum{
	Is_Default,				//���ڵ�
	Is_RecentlyRead,		//����Ķ�
	Is_Readman,				//Readman���ڵ�
	Is_Readman_News,		//Readman{����}
	Is_Readman_Notice,		//Readman{ͨ��}
	Is_Readman_Known,		//Readman{֪��}
	Is_Readman_Guide,		//Readman{����}
	Is_Readman_Friend,		//Readman{�Ѷ�}
	Is_Readman_Music,		//Readman{����}
	Is_Readman_Advertise,	//Readman{���}
	Is_Readman_Method,		//Readman{����}
	Is_Readman_Help,		//Readman{����}
	Is_Readman_Fonts,		//Readman{�ֿ�}
	Is_Readman_Tool,		//Readman{����}
	Is_Readman_Install,		//Readman{��װ}

	Is_PushFolder,			//������
	Is_MyBookCase,			//���ض���
	Is_MyFavourate,			//�ղؼ�
	Is_MyFavourate_Folder,	//�ղؼ����ļ���
	Is_Collect_Read,		//��ʽ�ɼ�
	Is_Collect_Pic,
	Is_Collect_Media,
	Is_Collect_FileType,	//��ʽ�ɼ������ļ�����
	Is_Collect_FileType_Year,//��ʽ�ɼ������ļ����͡������
	Is_OrderCenter,			//��������
	Is_OrderList,			//�����б�
	Is_OrderPaper,			//���ֱ�ֽ
	Is_OrderPaper_Search,	//���ֱ�ֽ����
	Is_OrderPaper_Node,
	Is_OrderMagzine,		//������־
	Is_OrderMagzine_Search,	//������־����
	Is_OrderMagzine_Node,

	Is_NetResource,			//�������
	Is_PubInfo,				//������
	Is_NetLibrary,			//�����ͼ
	Is_Channel,				//Ƶ��
	Is_BookType,			//�鼮����
	Is_SearchHistory,		//������ʷ
	Is_ResourceShare,		//���ﹲ��
	Is_ResourceShare_Hot,	
	Is_ResourceShare_Read,		//��ʽ�ɼ�
	Is_ResourceShare_Pic,
	Is_ResourceShare_Media,
	Is_ResourceShare_FileType,	//���繲����Դ
	Is_Download,			//��������
	Is_NetFriendList,		//���ߺ���
	Is_Chatroom,            //��������� added by zhanganzhan on 20090325
	Is_MyFriend,            //�ҵ����ĺ���
	Is_NetFirend,
	Is_MyNetGroupList,		//�ҵĲ���
	Is_AllNetGroupList,		//���ﲿ��
	Is_NetGroup_Level,		//���ﲿ���м�ڵ�
	Is_NetGroup,
	Is_MyNetGroup,			//�ҵĶ��ﲿ��
	
	Is_SDCard,				//�ƶ��洢��
	Is_SDCard_Folder,		//SD���ļ���
	
	Is_RickerGroup_Node,	//��Ͳ����м�ڵ�
	Is_RickerGroup_Leaf,	//��Ͳ���Ҷ�ڵ�
	Is_AllRickerGroupList,	//��Ͳ���

	Is_StudyHall,			//��ϰ�Ҹ��ڵ�
	Is_MyStudyHall,			//�ҵ���ϰ��
	Is_School,				//ѧУ
	Is_StudyBuilding,		//��ѧ¥
	Is_Hall,				//��ϰ��

	Is_Bookstore,          //����Ƶ��
	Is_Zgsztug,             //�й�����ͼ���
	Is_Tianya,				//�����������
	Is_Rdcxzx,              //��³�������ͼ���
	Is_Qjzw,                 //ȫ������ͼ��
    Is_Essk,                  //E��ʱ��
    Is_Dtzw,                 //�����������
    Is_Sxmd,                 //�����ŵ�
    Is_Rmsc,                 //�������
    Is_Kyec,                 //�����������
    Is_Sb,                   //���
    Is_Fhsc,                 //������
    Is_Sjsc,                 //�������
    Is_Zhsk,                 //�л����
    Is_Sh,                   //�麣
	Is_Dnsc,                 //�������
    Is_Dssk,                 //����ʱ��

	Is_NetBook,              //����ԭ������
	Is_NetBookLeaf,			 //����ԭ������Ҷ�ڵ�
	Is_MyResource,			 //�ҵĶ��ﲿ��
	Is_iStore,			    //istation�����ڵ�
	Is_iChannel,		    //istation ͼ��Ƶ���ڵ�
	Is_iType,				//istation ��ͼ�����ڵ�

	Is_HotKey,				//�ȴʸ��ڵ�
	Is_HotKeyleaf,			//�ȴ�Ҷ�ڵ�

	Is_FanShuHotBook,				//����Ƶ�����ڵ�
	Is_FanShuHotBookleaf,			//����Ƶ��Ҷ�ڵ�
	Is_FanShu,  //������
	Is_FanShuLeaf, //������Ҷ�ڵ�
	Is_RStore,//
};

enum{
	Is_FindList,			//�����б�
	Is_BookList,			//�鼮�б�
	Is_OrdList,				//�����б�
	Is_FileList,			//�ļ��б�
	Is_PushList,
	Is_ShareList,
	Is_CollectList,
	Is_RecentlyList,
	Is_SDDiskList,
	Is_NetBookList,        //added by zuoss 090526 ��������б�
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
	Is_MyRead = 1,		//�Ҷ�
	Is_MyListen = 2,	//����
	Is_MyLook = 3,		//�ҿ�
	Is_MyOthers = 4,	//����
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