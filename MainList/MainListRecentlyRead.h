#pragma once
#include "MainListDataFather.h"

/*****************************
* create time : 2010 04 22
* author : wangcong
* description : 
* ����CMailListView��ʾ����Ķ�
* �̳���CMainListDataFather��
* ���������ⷽ��DealMessage��DrawList���о������
******************************/

class CMainListRecentlyRead
	: CMainListDataFather
{
public:
	CMainListRecentlyRead(void);
	~CMainListRecentlyRead(void);
	//��������
	virtual bool DealMessage(CMainListView* ListView);
	//��ͼ
	virtual bool DrawList(NMHDR* pNMHDR);
};
