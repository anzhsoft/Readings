#pragma once

/*****************************
* create time : 2010 04 22
* author : wangcong
* description : 
* CMailListView显示所用数据处理类的父类
* 在具体显示时将创建相应子类已进行具体操作
******************************/

class CMainListDataFather
{
public:
	CMainListDataFather(void);
	~CMainListDataFather(void);
	//处理数据
	virtual bool DealMessage(CMainListView* ListView);
	//绘图
	virtual bool DrawList(NMHDR* pNMHDR);
};
