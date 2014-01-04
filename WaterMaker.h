
typedef struct STRUCTBTNODE{
	STRUCTBTNODE* left;
	STRUCTBTNODE* right;
	STRUCTBTNODE* farther;
	DWORD dwPre;
	DWORD dwLevel;
}BTNODE,*PBTNODE;

enum BT_DIRE{
	BT_LEFT,
	BT_RIGHT,
	BT_NONE,
};

enum INPUT_STATE{
	INPUT_STATE1,
	INPUT_STATE2,
	INPUT_STATE3,
	INPUT_STATE_END,
};

PBTNODE CreateNode(PBTNODE pfNode, BT_DIRE dire);
//step 1
PBTNODE BuildBinaryTree();
//step 2
void InitPreOrderWalk(PBTNODE pNode, DWORD& num);
//step 3
void InitLevelOrderWalk(CList<PBTNODE,PBTNODE&>* pNL, DWORD& num);
//extract
__int64 Extract(PBTNODE pNode);
//tools
PBTNODE GotoNext(PBTNODE pNode);
void GetLeafList(PBTNODE pNode, CList<PBTNODE,PBTNODE&>& pNL);