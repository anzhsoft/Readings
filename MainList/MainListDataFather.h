#pragma once

/*****************************
* create time : 2010 04 22
* author : wangcong
* description : 
* CMailListView��ʾ�������ݴ�����ĸ���
* �ھ�����ʾʱ��������Ӧ�����ѽ��о������
******************************/

class CMainListDataFather
{
public:
	CMainListDataFather(void);
	~CMainListDataFather(void);
	//��������
	virtual bool DealMessage(CMainListView* ListView);
	//��ͼ
	virtual bool DrawList(NMHDR* pNMHDR);
};
