#include "StdAfx.h"
#include "WaterMaker.h"
#include <math.h>

//构造结点
PBTNODE CreateNode(PBTNODE pfNode, BT_DIRE dire)
{
	PBTNODE node = (PBTNODE)calloc(1,sizeof(BTNODE));
	if(pfNode!=NULL){
		node->farther = pfNode;
		if(dire==BT_LEFT)
			pfNode->left = node;
		else
			pfNode->right = node;
	}
	return node;
}

//后继节点
PBTNODE GotoNext(PBTNODE pNode)
{
	PBTNODE resNode;
	if(pNode->right!=NULL)
		return GotoNext(pNode->right);
	resNode = pNode->farther ;
    while((resNode!=NULL)&&(pNode==resNode->right) ){
		pNode=resNode;
		resNode=resNode->farther;
	}
	return resNode;
}

//前序编号
void InitPreOrderWalk(PBTNODE pNode, DWORD& num)
{
	if(pNode->left!=NULL)
		InitPreOrderWalk(pNode->left,num);
	pNode->dwPre = num++;
	if(pNode->right!=NULL)
		InitPreOrderWalk(pNode->right,num);
}

//层序编号
void InitLevelOrderWalk(CList<PBTNODE,PBTNODE&>* pNL, DWORD& num)
{
	POSITION pos = pNL->GetHeadPosition();
	CList<PBTNODE,PBTNODE&> SubNL;
	while(pos){
		PBTNODE pNode = pNL->GetNext(pos);
		pNode->dwLevel = num++;
		if(pNode->left)
			SubNL.AddTail(pNode->left);
		if(pNode->right)
			SubNL.AddTail(pNode->right);
	}
	if(SubNL.GetCount()>0)
		InitLevelOrderWalk(&SubNL, num);
}

PBTNODE BuildBinaryTree()
{
	PBTNODE root = CreateNode(NULL,BT_NONE);
	PBTNODE n2 = CreateNode(root, BT_LEFT);
	PBTNODE n3 = CreateNode(root, BT_RIGHT);
	CreateNode(n2, BT_RIGHT);//n5
	PBTNODE n4 = CreateNode(n2,BT_LEFT);
	CreateNode(
		CreateNode(
			CreateNode(
				CreateNode(n4,BT_LEFT)
				,BT_RIGHT)
			,BT_RIGHT)
		,BT_RIGHT);
	CreateNode(CreateNode(CreateNode(n4,BT_RIGHT),BT_RIGHT),BT_RIGHT);
	CreateNode(CreateNode(n3, BT_LEFT),BT_RIGHT);
	CreateNode(CreateNode(n3,BT_RIGHT),BT_RIGHT);
	return root;
}

__int64 Extract(PBTNODE pNode)
{
	__int64 res = 0;
	CList<PBTNODE,PBTNODE&> pNL;
	GetLeafList(pNode, pNL);

	__int64 ir = pNL.GetCount();	//底数
	POSITION pos = pNL.GetHeadPosition();
	while(pos){
		PBTNODE c = pNL.GetNext(pos);
		PBTNODE n = GotoNext(c);
		DWORD lv = n==NULL?0:n->dwLevel;
		res += pow(ir,lv)*c->dwPre;
	}
	res += 62;
	return res;
}

void GetLeafList(PBTNODE pNode, CList<PBTNODE,PBTNODE&>& pNL)
{
	if(pNode->left==NULL && pNode->right==NULL){
		pNL.AddTail(pNode);
		return;
	}
	if(pNode->left!=NULL)
		GetLeafList(pNode->left,pNL);
	if(pNode->right!=NULL)
		GetLeafList(pNode->right,pNL);
}
