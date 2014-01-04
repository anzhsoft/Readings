
//用于PDF
const int WS = 200;

//用于DOC
const int HS = 0x20;
static BYTE head[HS] = 
	{
		//0xFE, 0xFF, 0, 0, 0x5, 0, 0x2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0x1, 0, 0, 0, 0xE0, 0x85, 0x9F, 0xF2,
		0xF9, 0x4F, 0x68, 0x10, 0xAB, 0x91, 0x08, 0, 0x2B, 0x27, 0xB3, 0xD9, 0x30, 0, 0, 0
	};
static BYTE SegFlag[8] = { 2,0,0,0,0xA8,0x03,0,0};
static BYTE WaterMarkHead[7] = {0x0d,0x0a,1,1,0x0d,0x0a,0};



//Golbal
BOOL FormatWaterMark(CString FilePath, CString Un);
CString ExtractPDF(CString FileName);
CString ExtractCHM(CString FileName);
CString ExtractDOC(CString FileName);