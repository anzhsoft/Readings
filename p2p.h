
const int BUFFER_SIZE = 1024;
const int MD5LEN = 16;
#define FILE_ITEM_NUM 700 //added by zhanganzhan for xml items number on 20081119
#define MAX_CHATROOM_ID 9999999
//#define SHAREFILEFOLDER		"./share"
/*
enum{ 
	state_login_r = 0, 			//��½����
	state_login_a,				//��½����ȷ�ϣ����ֱ�־��
	state_comm, 				//for debug	
	state_logout_r,				//�û��ǳ�
	state_userinfo_append,		//�û���Ϣ׷��		
	state_userinfo_delete,		//�û���Ϣɾ��
	state_userinfo_modify,		//�û���Ϣ�޸�
	state_server_quit,			//�����������ж�
	state_server_broadcast,		//�������㲥
	state_server_advertisement,	//������
	state_server_tip,			//��Ϣ��ʾ
	
	state_peer_file_request,	//�ļ���������
	state_peer_filetran,		//�ļ�����
	state_peer_file_tran_f,		//�ļ��������
	state_peer_file_deny,		//�ļ�����ܾ�
	state_peer_file_tran_finished,	//�ļ��������
	state_xml_tran_finished,  //xml �ļ�������� //zhanganzhan added on 20081118

	state_server_share_info,	//���������ļ���Ϣ����
	state_server_xml_ans,//zhanganzhan xml����׼����Ӧ��
	state_server_search_info_request,	//��������
	state_server_search_info_result,	//�������������	�ͻ��˼��ѯ���
	state_server_search_info_finished,	//��������

	state_peer_close_connect,	//�ͻ������ӹر�
	state_peer_file_not_exist,	//������ļ���������

	state_server_userinfo_query,	//�û���ѯ����
	state_server_userinfo_query_answer,	//�û���ѯ���
	
	state_server_user_shareinfo_query,	//�û���ѯ����
	
	state_userinfo_detail_modify,	//�û�������Ϣ�޸�
	state_userinfo_detail_query,	//�û�������Ϣ��ѯ
	state_userinfo_detail_answer,	//�û�������Ϣ��ѯ���

	state_resource_download_times, //��Դ���ش�������
	state_heart_beat,    //������ʾλ

	state_peer_one_resource_request, //�û�����sid����һ����Դ�Ľṹ��
	start_peer_one_resource_response, //���ظ��û�һ���ṹ���ʾ
	start_peer_one_resource_response_no, //���ظ��û�һ���ṹ��ʧ�ܱ�ʾ

	state_peer_send_request_info,	//������Э�������ļ�
	state_peer_comm_request_info,	//������Э��ͨѶ

	state_groupinfo_append,		//С����Ϣ׷��	
	state_p2p_request_response,//zhanganzhan 20081204


	state_nop = 0xff,
};
*/
enum{ 
	state_login_r = 0, 			//��½����
	state_login_a,				//��½����ȷ�ϣ����ֱ�־��
	state_comm, 				//for debug	
	state_logout_r,				//�û��ǳ�
	state_userinfo_append,		//�û���Ϣ׷��		
	state_userinfo_delete,		//�û���Ϣɾ��
	state_userinfo_modify,		//�û���Ϣ�޸�
	state_server_quit,			//�����������ж�
	state_server_broadcast,		//�������㲥
	state_server_advertisement,	//������
	state_server_tip,			//��Ϣ��ʾ
	
	state_peer_file_request,	//�ļ���������
	state_peer_filetran,		//�ļ�����
	state_peer_file_tran_f,		//�ļ��������
	state_peer_file_deny,		//�ļ�����ܾ�
	state_peer_file_tran_finished,	//�ļ��������
	state_xml_tran_finished,  //xml �ļ�������� //zhanganzhan added on 20081118
	state_file_tran_begin,    //�ļ����俪ʼ��־ //zhanganzhan added on 20081209
    state_file_tran_prepare,
	
	state_server_share_info,	//���������ļ���Ϣ����
	state_server_xml_ans,       //zhanganzhan xml׼��������Ӧ��
	state_server_search_info_request,	//�������� �ؼ���
	state_server_search_info_result,	//�������
	state_server_search_info_finished,	//��������

	state_peer_close_connect,	//�ͻ������ӹر�
	state_peer_file_not_exist,	//������ļ���������
	
	state_server_userinfo_query,	//�û���ѯ����
	state_server_userinfo_query_answer,	//�û���ѯ���

	state_server_user_shareinfo_query,	//����û��Ĺ�����Ϣ��ѯ

	state_userinfo_detail_modify,	//�û�������Ϣ�޸�
	state_userinfo_detail_query,	//�û�������Ϣ��ѯ
	state_userinfo_detail_answer,	//�û�������Ϣ��ѯ���
	
	state_resource_download_times,	//��Դ���ش�������
	state_heart_beat,    //������ʾλ
	//add by dongzhenhua 20071221
	state_peer_one_resource_request,	//�û�����sid����һ����Դ�Ľṹ��
	start_peer_one_resource_response,	//���ظ��û�һ���ṹ���ʾ
	start_peer_one_resource_response_no,	//���ظ��û�һ���ṹ��ʧ�ܱ�ʾ

	state_peer_send_request_info,	//������Э�������ļ�
	state_peer_comm_request_info,	//������Э��ͨѶ

	state_groupinfo_append,		//С����Ϣ׷��	
	state_p2p_request_response, //zhanganzhan ��Ӧ�ͻ��ˣ�ʹ���
};


//add by yangxiao	2008.8.21 begin
enum COMMOPER {
	comm_oper_appendUser, //�û���Ϣ׷��
	comm_oper_delUser,   //�û���Ϣɾ��
	comm_oper_recvMsg, //����������Ϣ
	comm_oper_sentMSg, //����������Ϣ
	comm_oper_tranHotKey, //�����ȴ�
	comm_oper_leftTree_search, //��������
	comm_oper_search, //����
	comm_oper_server_response_stru, //�������˷������������Դ�ṹ���ʾ
	comm_oper_server_response_stru_no, //�������˷������������Դ�ṹ���ʾʧ��
};
//add by yangxiao	2008.8.21 end



//Э���򶴶˿�
enum{
	state_peer_connect_request,
	state_peer_comm_request,
};
//��¼��Ϣ
typedef struct {
	char	UserName[30];	//�û���
	char	PassWord[20];	//����
	char	IP[20];			//IP��ַ
	int		ctrPort;		//�������ƶ˿�
	int		tranPort;		//�ļ��������
	char	Reserved[60];	//����
} STRUCTLOGIN;

//�û���Ϣ����ͷ
typedef struct{
		int	num;		//��ǰ���͵��û���Ϣ�ĸ���
		int	day;		//�û���Ϣ�ķ���ʱ��
		int	hour;
		int	minute;
		int	second;
		long ttl;		//�����ڣ���λ(ms)
}STRUCTUSERINFOHEAD;

//�û���Ϣ
typedef struct{
		char		UserName[30];	//�û���
		char		IP[20];			//IP��ַ
		char        NetName[50];    ////������������ zuoss 20081113
		int 		ctrPort;		//���ƶ˿�
		int			tranPort;		//�ļ�����˿�	
		char        Field[30];		//��ʾ�û�ע��ʱ�ṩ�����ڵ�	
		int			shareFileNum;	//�û������ļ���Ŀ zuoss 090310
		char		Reserved[60];	//����
	
}STRUCTUSERINFO;
//��������Ϣ
//added by zhanganzhan on 20090414 for proxy resources
typedef struct  
{
	char chatroomName[30];
	int resCount;
	int chatroomID;
}STRUCTCHATROOMINFO;
//�ļ���������Ϣ
typedef struct STRUCTREQUESTFILEINFO {
	char FilePath [_MAX_PATH+1];	// �ļ�·��	ӵ����Դ�Ŀͻ��˵���Դ���·��
									// ·���� '\' ��ͷ
	long FileID;					// �ļ�ID		�ͻ���������� ��Ϊ��ʶ
	char UserName[30];				// ������Դ���û���
	char MD5[MD5LEN];					//MD5У��
} REQUESTFILEINFO;

//�ļ�����ͷ
typedef struct STRUCTFILEBLOCKHEAD {
	long FileID;		//�ļ�ID
	long FileSize;		//�ļ��ܳ���
	long FileOffset;	//��ǰ�ļ������ʼλ��
	long BlockSize;		//FILEBLOCK�Ĵ�С
	char Reserved[16];	//����
} FILEBLOCKHEAD;


//�����ļ���Ϣͷ
typedef struct STRUCTSHAREINFOHEAD {
	int	num;			//�����ļ���Ϣ����
	char Reserved[32];	//����
} SHAREINFOHEAD;

//�����ļ���Ϣ
typedef struct STRUCTSHAREINFO{
	char		FileName[_MAX_PATH+1];	//·��
	FILETIME	ftLastWriteTime;		//�ļ�������޸�ʱ��
	long		nFileSize;				//�ļ���С�ĸ�λ˫��
	int 		FileType;				//�ļ�����
	char		MD5[MD5LEN];			//MD5У��
} SHAREINFO;

//�ļ���������
typedef struct STRUCTSEARCHINFO{
	char	KeyWord[100];	//�ؼ���
	int		FileType;		//�ļ�����	���Ϊ0��������������
	char	UserName[30];
	char	Reserved[226];	//����
}SEARCHINFO;

//�������ͷ
typedef struct STRUCTSEARCHRESHEAD{
	int		num;			//SEARCHINFO��
	char	UserName[30];
	char	Reserved[2];	//����
} SEARCHRESHEAD;

//�������
typedef struct STRUCTSEARCHRES{
	char		FileName[_MAX_PATH+1];
	FILETIME	ftLastWriteTime; 		//�ļ�������޸�ʱ��
	long		nFileSize; 				//�ļ���С�ĸ�λ˫��
	int 		FileType;
	char		MD5[MD5LEN];
	char		UserName[30];			//�����û�
	char		IP[20];
	int			ctrport;				//�˿���
	int			tranport;
	long		sid;
} SEARCHRES;

//�û�����
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

//������ 
typedef struct STRUCTHEARTBEAT{
	char UserName[30];
} HEARTBEAT;
 
//��Դ���ر����
typedef struct STRUCTDOWNLOADREPORT{
	long sid; 
} DOWNLOADREPORT;

//�û�������Դ�� ��������sid�������˷��ؽṹ��STRUCTSEARCHRES��
typedef struct STRUCTREQUESTONERESOURCE{
	long sid;
}REQUESTONERESOURCE;

//�û�����С����Ϣ���ͱ�ͷ
typedef struct STRUCTGROUPINFOHEAD{
		int	num;		//���͸ð��ڵ��û�����С��ĸ���
        char reserve[64];
}GROUPINFOHEAD;

//�û�����С���
typedef struct STRUCTGROUPINFO{
	long gid;
	char groupname[60];
	long totalnum;
}GROUPINFO;

//�ļ���������
typedef struct STRUCTFILEREQ{
	char		FileName[_MAX_PATH+1];
	FILETIME	ftLastWriteTime; 		//����
	long		nFileSize; 				//����
	int 		FileType;				//����	
	char		MD5[MD5LEN];
	char		UserName[30];			//�����û�
	char		IP[20];					//�����û�˽��IP
	int			port;					//˽�ж˿�
	char		wanIP[20];				//����IP
	int			wanPort;				//���ж˿�
} FILEREQ;
typedef struct STRUCTSHAREINFOREQ
{
	char UserName[55];
	int fileNum;
	long uid;
}SHAREINFOREQ;

