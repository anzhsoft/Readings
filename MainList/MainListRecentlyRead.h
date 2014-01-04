#pragma once
#include "MainListDataFather.h"

/*****************************
* create time : 2010 04 22
* author : wangcong
* description : 
* 用于CMailListView显示最近阅读
* 继承自CMainListDataFather，
* 重载其虚拟方法DealMessage，DrawList进行具体操作
******************************/

class CMainListRecentlyRead
	: CMainListDataFather
{
public:
	CMainListRecentlyRead(void);
	~CMainListRecentlyRead(void);
	//处理数据
	virtual bool DealMessage(CMainListView* ListView);
	//绘图
	virtual bool DrawList(NMHDR* pNMHDR);
};
