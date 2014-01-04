
const int BUFFER_SIZE = 1024;
const int MD5LEN = 16;
#define FILE_ITEM_NUM 700 //added by zhanganzhan for xml items number on 20081119
#define MAX_CHATROOM_ID 9999999
//#define SHAREFILEFOLDER		"./share"
/*
enum{ 
	state_login_r = 0, 			//登陆请求
	state_login_a,				//登陆请求确认（握手标志）
	state_comm, 				//for debug	
	state_logout_r,				//用户登出
	state_userinfo_append,		//用户信息追加		
	state_userinfo_delete,		//用户信息删除
	state_userinfo_modify,		//用户信息修改
	state_server_quit,			//服务器程序中断
	state_server_broadcast,		//服务器广播
	state_server_advertisement,	//广告服务
	state_server_tip,			//消息提示
	
	state_peer_file_request,	//文件传输请求
	state_peer_filetran,		//文件传输
	state_peer_file_tran_f,		//文件传输完成
	state_peer_file_deny,		//文件传输拒绝
	state_peer_file_tran_finished,	//文件传输结束
	state_xml_tran_finished,  //xml 文件传输结束 //zhanganzhan added on 20081118

	state_server_share_info,	//本机共享文件信息发布
	state_server_xml_ans,//zhanganzhan xml传输准备好应答。
	state_server_search_info_request,	//搜索请求
	state_server_search_info_result,	//服务器搜索结果	客户端间查询结果
	state_server_search_info_finished,	//搜索结束

	state_peer_close_connect,	//客户端连接关闭
	state_peer_file_not_exist,	//请求的文件并不存在

	state_server_userinfo_query,	//用户查询请求
	state_server_userinfo_query_answer,	//用户查询结果
	
	state_server_user_shareinfo_query,	//用户查询请求
	
	state_userinfo_detail_modify,	//用户详情信息修改
	state_userinfo_detail_query,	//用户详情信息查询
	state_userinfo_detail_answer,	//用户详情信息查询结果

	state_resource_download_times, //资源下载次数更新
	state_heart_beat,    //心跳标示位

	state_peer_one_resource_request, //用户根据sid请求一个资源的结构体
	start_peer_one_resource_response, //返回给用户一个结构体标示
	start_peer_one_resource_response_no, //返回给用户一个结构体失败标示

	state_peer_send_request_info,	//服务器协助请求文件
	state_peer_comm_request_info,	//服务器协助通讯

	state_groupinfo_append,		//小组信息追加	
	state_p2p_request_response,//zhanganzhan 20081204


	state_nop = 0xff,
};
*/
enum{ 
	state_login_r = 0, 			//登陆请求
	state_login_a,				//登陆请求确认（握手标志）
	state_comm, 				//for debug	
	state_logout_r,				//用户登出
	state_userinfo_append,		//用户信息追加		
	state_userinfo_delete,		//用户信息删除
	state_userinfo_modify,		//用户信息修改
	state_server_quit,			//服务器程序中断
	state_server_broadcast,		//服务器广播
	state_server_advertisement,	//广告服务
	state_server_tip,			//消息提示
	
	state_peer_file_request,	//文件传输请求
	state_peer_filetran,		//文件传输
	state_peer_file_tran_f,		//文件传输完成
	state_peer_file_deny,		//文件传输拒绝
	state_peer_file_tran_finished,	//文件传输结束
	state_xml_tran_finished,  //xml 文件传输结束 //zhanganzhan added on 20081118
	state_file_tran_begin,    //文件传输开始标志 //zhanganzhan added on 20081209
    state_file_tran_prepare,
	
	state_server_share_info,	//本机共享文件信息发布
	state_server_xml_ans,       //zhanganzhan xml准备好连接应答
	state_server_search_info_request,	//搜索请求 关键字
	state_server_search_info_result,	//搜索结果
	state_server_search_info_finished,	//搜索结束

	state_peer_close_connect,	//客户端连接关闭
	state_peer_file_not_exist,	//请求的文件并不存在
	
	state_server_userinfo_query,	//用户查询请求
	state_server_userinfo_query_answer,	//用户查询结果

	state_server_user_shareinfo_query,	//针对用户的共享信息查询

	state_userinfo_detail_modify,	//用户详情信息修改
	state_userinfo_detail_query,	//用户详情信息查询
	state_userinfo_detail_answer,	//用户详情信息查询结果
	
	state_resource_download_times,	//资源下载次数更新
	state_heart_beat,    //心跳标示位
	//add by dongzhenhua 20071221
	state_peer_one_resource_request,	//用户根据sid请求一个资源的结构体
	start_peer_one_resource_response,	//返回给用户一个结构体标示
	start_peer_one_resource_response_no,	//返回给用户一个结构体失败标示

	state_peer_send_request_info,	//服务器协助请求文件
	state_peer_comm_request_info,	//服务器协助通讯

	state_groupinfo_append,		//小组信息追加	
	state_p2p_request_response, //zhanganzhan 回应客户端，使其打洞
};


//add by yangxiao	2008.8.21 begin
enum COMMOPER {
	comm_oper_appendUser, //用户信息追加
	comm_oper_delUser,   //用户信息删除
	comm_oper_recvMsg, //接收聊天消息
	comm_oper_sentMSg, //发送聊天消息
	comm_oper_tranHotKey, //传送热词
	comm_oper_leftTree_search, //左树搜索
	comm_oper_search, //搜索
	comm_oper_server_response_stru, //服务器端返回所请求的资源结构体标示
	comm_oper_server_response_stru_no, //服务器端返回所请求的资源结构体标示失败
};
//add by yangxiao	2008.8.21 end



//协助打洞端口
enum{
	state_peer_connect_request,
	state_peer_comm_request,
};
//登录信息
typedef struct {
	char	UserName[30];	//用户名
	char	PassWord[20];	//密码
	char	IP[20];			//IP地址
	int		ctrPort;		//监听控制端口
	int		tranPort;		//文件传输监听
	char	Reserved[60];	//保留
} STRUCTLOGIN;

//用户信息发送头
typedef struct{
		int	num;		//当前发送的用户信息的个数
		int	day;		//用户信息的发送时间
		int	hour;
		int	minute;
		int	second;
		long ttl;		//生存期，单位(ms)
}STRUCTUSERINFOHEAD;

//用户信息
typedef struct{
		char		UserName[30];	//用户名
		char		IP[20];			//IP地址
		char        NetName[50];    ////所在网络名称 zuoss 20081113
		int 		ctrPort;		//控制端口
		int			tranPort;		//文件传输端口	
		char        Field[30];		//表示用户注册时提供的所在地	
		int			shareFileNum;	//用户共享文件数目 zuoss 090310
		char		Reserved[60];	//保留
	
}STRUCTUSERINFO;
//聊天室信息
//added by zhanganzhan on 20090414 for proxy resources
typedef struct  
{
	char chatroomName[30];
	int resCount;
	int chatroomID;
}STRUCTCHATROOMINFO;
//文件请求传输信息
typedef struct STRUCTREQUESTFILEINFO {
	char FilePath [_MAX_PATH+1];	// 文件路径	拥有资源的客户端的资源相对路径
									// 路径以 '\' 开头
	long FileID;					// 文件ID		客户端随机分配 作为标识
	char UserName[30];				// 申请资源的用户名
	char MD5[MD5LEN];					//MD5校验
} REQUESTFILEINFO;

//文件传输头
typedef struct STRUCTFILEBLOCKHEAD {
	long FileID;		//文件ID
	long FileSize;		//文件总长度
	long FileOffset;	//当前文件块的起始位置
	long BlockSize;		//FILEBLOCK的大小
	char Reserved[16];	//保留
} FILEBLOCKHEAD;


//共享文件信息头
typedef struct STRUCTSHAREINFOHEAD {
	int	num;			//共享文件信息个数
	char Reserved[32];	//保留
} SHAREINFOHEAD;

//共享文件信息
typedef struct STRUCTSHAREINFO{
	char		FileName[_MAX_PATH+1];	//路径
	FILETIME	ftLastWriteTime;		//文件的最后修改时间
	long		nFileSize;				//文件大小的高位双字
	int 		FileType;				//文件类型
	char		MD5[MD5LEN];			//MD5校验
} SHAREINFO;

//文件搜索请求
typedef struct STRUCTSEARCHINFO{
	char	KeyWord[100];	//关键字
	int		FileType;		//文件类型	如果为0，搜索所有类型
	char	UserName[30];
	char	Reserved[226];	//保留
}SEARCHINFO;

//搜索结果头
typedef struct STRUCTSEARCHRESHEAD{
	int		num;			//SEARCHINFO数
	char	UserName[30];
	char	Reserved[2];	//保留
} SEARCHRESHEAD;

//搜索结果
typedef struct STRUCTSEARCHRES{
	char		FileName[_MAX_PATH+1];
	FILETIME	ftLastWriteTime; 		//文件的最后修改时间
	long		nFileSize; 				//文件大小的高位双字
	int 		FileType;
	char		MD5[MD5LEN];
	char		UserName[30];			//共享用户
	char		IP[20];
	int			ctrport;				//端口名
	int			tranport;
	long		sid;
} SEARCHRES;

//用户详情
typedef struct STRUCTUSERINFODETAIL{
	char UserName[30];
	char NickName[50];
	char College[30];
	char Field[30];
	char Constellation[10];
	char Career[20];
	char RealName[20];
	char Email[255];
	char Location[50];
	int Sex;
	int Age;
	int BirthMonth;
	int BirthDay;
}USERINFODETAIL;

//心跳包 
typedef struct STRUCTHEARTBEAT{
	char UserName[30];
} HEARTBEAT;
 
//资源下载报告包
typedef struct STRUCTDOWNLOADREPORT{
	long sid; 
} DOWNLOADREPORT;

//用户请求资源包 ：（根据sid服务器端返回结构体STRUCTSEARCHRES）
typedef struct STRUCTREQUESTONERESOURCE{
	long sid;
}REQUESTONERESOURCE;

//用户所属小组信息发送报头
typedef struct STRUCTGROUPINFOHEAD{
		int	num;		//发送该包内的用户所属小组的个数
        char reserve[64];
}GROUPINFOHEAD;

//用户所属小组包
typedef struct STRUCTGROUPINFO{
	long gid;
	char groupname[60];
	long totalnum;
}GROUPINFO;

//文件下载请求
typedef struct STRUCTFILEREQ{
	char		FileName[_MAX_PATH+1];
	FILETIME	ftLastWriteTime; 		//兼容
	long		nFileSize; 				//兼容
	int 		FileType;				//兼容	
	char		MD5[MD5LEN];
	char		UserName[30];			//共享用户
	char		IP[20];					//请求用户私有IP
	int			port;					//私有端口
	char		wanIP[20];				//公有IP
	int			wanPort;				//公有端口
} FILEREQ;
typedef struct STRUCTSHAREINFOREQ
{
	char UserName[55];
	int fileNum;
	long uid;
}SHAREINFOREQ;

