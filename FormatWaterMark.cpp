#include "StdAfx.h"
#include "FormarWaterMark.h"
#include "Readings.h"

//Method Private
BOOL AllZero(char* buf, int size);
//Embedd WaterMark Function
BOOL WMPdf(CString FilePath, CString Un);
BOOL WMChm(CString FilePath, CString Un);
BOOL WMDoc(CString FilePath, CString Un);

BOOL WMPdf(CString FilePath, CString Un)
{
	CFile cf;
	if(!cf.Open(FilePath, CFile::modeReadWrite)){
		Tips("File Open Error!",td_alert);
		return FALSE;
	}
	
	if(cf.GetLength()<WS){
		Tips("Too Short!",td_alert);
		cf.Close();
		return FALSE;
	}
	char buf[WS+1] = {0};
	LONG pos = cf.Seek(-WS,CFile::end);
	cf.Read(buf, WS);
	
	char* temp = strstr(buf, "trailer");

	if(!temp){
		Tips("Unsupported PDF Format",td_alert);
		cf.Close();
		return FALSE;
	}

	LONG offset = (temp-buf)-WS;
	cf.Seek(offset,CFile::end);
	cf.Write("WM/", 3);
	cf.Write((LPCSTR)Un, Un.GetLength());
	cf.Write("\r\n",strlen("\r\n"));
	cf.Write(temp,-offset);
	cf.Close();

	Tips("WaterMark Embeed Successfully!",td_alert);
	return TRUE;
}

BOOL WMChm(CString FilePath, CString Un)
{
	CFile cf;
	if(!cf.Open(FilePath, CFile::modeReadWrite)){
		Tips("File Open Error!",td_alert);
		return FALSE;
	}

	DWORD size = cf.GetLength();

	if(size<0x38){
		Tips("Too Short!",td_alert);
		return FALSE;
	}

	cf.Seek(0x38,CFile::begin);

	DWORD s2Off;
	DWORD s3Off;
	//s1
	cf.Seek(4*2,CFile::current);
	cf.Seek(4*2,CFile::current);
	//s2
	cf.Read(&s2Off,sizeof(DWORD));
	cf.Seek(4,CFile::current);
	cf.Seek(4*2,CFile::current);
	//s3
	cf.Read(&s3Off,sizeof(DWORD));

	char win[20] = {0};

	int offset = 20;
	BOOL flag = TRUE;
	while(TRUE){
		if(s3Off-offset<=s2Off){
			flag = FALSE;
			break;
		}
		cf.Seek(s3Off-offset,CFile::begin);
		cf.Read(win, 20);
		if(AllZero(win,20))
			break;
		offset += 20;
	}

	if(!flag){
		Tips("Not Enough Space!",td_alert);
		cf.Close();
		return FALSE;
	}
	cf.Write("WM/",strlen("WM/"));
	cf.Write((LPCSTR)Un, Un.GetLength());
	cf.Close();
	Tips("WaterMark Embeed Successfully!",td_alert);
	return TRUE;
}

BOOL AllZero(char* buf, int size)
{
	for(int n=0; n<size; n++)
		if(buf[n]!=0)
			return FALSE;
	return TRUE;
}

BOOL WMDoc(CString FilePath, CString Un)
{
	CFile cf;
	if(!cf.Open(FilePath, CFile::modeReadWrite)){
		Tips("File Open Error!",td_alert);
		return FALSE;
	}

	DWORD size = cf.GetLength();

	//从三分之一处开始寻找
	DWORD offset = 0;
	offset -= offset%0x200;
	offset = 0;

	BOOL flag = TRUE;
	char win[HS] = {0};
	while(TRUE){
		if(offset>=size){
			flag = FALSE;
			break;
		}
		cf.Seek(offset+0x10, CFile::begin);
		cf.Read(win, HS);
		if(memcmp(win, head, HS)==0)
			break;
		offset += 0x200;
	}

	if(flag){
		char buffer[0x200] = {0};
		cf.Seek(offset, CFile::begin);
		cf.Read(buffer, 0x200);

		if(buffer[0x60]!=6){	//没有备注段
			char seg[8];
			memcpy(seg,buffer+0x60,8);
			seg[0] = 6;
			flag = FALSE;
			LONG addr;
			for(addr=0x60;addr<0x200;addr+=8){
				if(buffer[addr]==2){
					flag =TRUE;
					break;
				}
			}
			if(flag){
				cf.Seek(offset+0x60, CFile::begin);
				cf.Write(seg,8);
				cf.Write(buffer+0x60,addr-0x60);
				DWORD temp = *(DWORD*)(seg+4);
				cf.Write(buffer+addr, temp+0x30-addr);
				char content[24] = {0};
				content[0] = 0x1E;
				content[4] = 9;
				strcpy_s(content+8, 16, "Readings");
				cf.Write(content,24);
				LONG rest =  cf.GetPosition();
				cf.Write(buffer+temp+0x30, offset+0x200-rest);
				cf.Seek(offset,CFile::begin);
				cf.Read(buffer,0x200);

				for(int n=0x38; n<0x200; n+=8){
					if(buffer[n]==2 && memcmp(buffer+n,SegFlag,8)==0)
						break;
					if(buffer[n]<=6)
						*(DWORD*)(buffer+n+4)+=8;
					else
						*(DWORD*)(buffer+n+4)+=8+24;
				}
				cf.Seek(offset,CFile::begin);
				cf.Write(buffer, 0x200);
				
				cf.Seek(offset, CFile::begin);
				cf.Read(buffer, 0x200);
			}
			else{
				cf.Close();
				Tips("Format Error!",td_alert);
				return FALSE;
			}
		}
		DWORD loc = *(DWORD*)(buffer+0x64);

		DWORD pos = 0x30+loc;
		if(buffer[pos]==0x1E){
			//offset+pos为注释段头
			//segLen包括串尾符'\0'
			DWORD oldsegLen = *(DWORD*)(buffer+pos+4);

			DWORD segLen = strlen(buffer+pos+8);
			DWORD appendLen = segLen-oldsegLen+1;
			char* end = buffer+segLen;

			char tmp[0x200] = {0};
			memcpy(tmp,buffer+pos+8,segLen);
			cf.Seek(offset+pos+8+segLen,CFile::begin);
			if(strstr(tmp,(LPCSTR)WaterMarkHead)==NULL){//没有水印
				cf.Write((LPCSTR)WaterMarkHead,strlen((LPCSTR)WaterMarkHead));
				appendLen+=strlen((LPCSTR)WaterMarkHead);
			}
			cf.Write((LPCSTR)Un, Un.GetLength());
			cf.Write("\r\n",strlen("\r\n"));
			char ef = 0;
			cf.Write(&ef,1);
			appendLen+=strlen("\r\n")+Un.GetLength()+1;
			segLen+=appendLen;
			cf.Write(end,buffer+0x200-end);
				//修改段长度
			cf.Seek(offset+pos+4,CFile::begin);
			cf.Write(&segLen,4);
			//修改偏移量
			int addr = 0x6c;
 			for(int n=0; n<11; n++){
				DWORD l = *(DWORD*)(buffer+addr);
				l+=appendLen;
				cf.Seek(offset+addr,CFile::begin);
				cf.Write(&l, 4);
				addr+=8;
			}
			Tips("WaterMark Embeed Successfully!",td_alert);
		}
		else
			Tips("Format Error!",td_alert);
	}
	else{
		Tips("Not Found!",td_alert);
	}
	cf.Close();
	return TRUE;
}

BOOL FormatWaterMark(CString FilePath, CString Un)
{
	CString ext = GetExtNameFromFull(FilePath);
	if(ext.IsEmpty())
		return FALSE;

	if(ext.CompareNoCase("pdf")==0){
		return WMPdf(FilePath,Un);
	}
	else if(ext.CompareNoCase("doc")==0){
		return WMDoc(FilePath,Un);
	}
	else if(ext.CompareNoCase("chm")==0){
		return WMChm(FilePath,Un);
	} 
	else
		return FALSE;
}

CString ExtractPDF(CString FileName)
{
	CString res;

	CFile cf;
	if(!cf.Open(FileName, CFile::modeRead)){
		Tips("File Open Error!",td_alert);
		return "";
	}
	
	if(cf.GetLength()<WS){
		Tips("Too Short!",td_alert);
		cf.Close();
		return "";
	}
	char buf[WS+1] = {0};
	LONG pos = cf.Seek(-WS,CFile::end);
	cf.Read(buf, WS);
	
	char* start = strstr(buf, "WM/");
	if(start){
		char temp[WS] = {0};
		char* end = strstr(buf, "trailer");
		memcpy(temp, start, end - start);
		res = temp;
		res.Replace("WM/","");
	}
	cf.Close();
	return res;
}

CString ExtractCHM(CString FileName)
{
	CString res;
	
	CFile cf;
	if(!cf.Open(FileName, CFile::modeRead)){
		Tips("File Open Error!",td_alert);
		return "";
	}

	DWORD size = cf.GetLength();

	if(size<0x38){
		Tips("Too Short!",td_alert);
		return "";
	}

	cf.Seek(0x38,CFile::begin);

	DWORD s2Off;
	DWORD s3Off;
	//s1
	cf.Seek(4*2,CFile::current);
	cf.Seek(4*2,CFile::current);
	//s2
	cf.Read(&s2Off,sizeof(DWORD));
	cf.Seek(4,CFile::current);
	cf.Seek(4*2,CFile::current);
	//s3
	cf.Read(&s3Off,sizeof(DWORD));

	char win[20] = {0};

	int offset = 20;
	BOOL flag = TRUE;
	while(TRUE){
		if(s3Off-offset<=s2Off){
			flag = FALSE;
			break;
		}
		cf.Seek(s3Off-offset,CFile::begin);
		cf.Read(win, 20);
		if(_strnicmp(win, "WM/",3)==0){
			res +=win+3;
			res += "\r\n";
		}
		else if(AllZero(win,20))
			break;
		offset += 20;
	}
	cf.Close();
	return res;
}

CString ExtractDOC(CString FileName)
{
	CString res;
	CFile cf;
	if(!cf.Open(FileName, CFile::modeRead)){
		Tips("File Open Error!",td_alert);
		return res;
	}

	DWORD size = cf.GetLength();

	//从三分之一处开始寻找
	DWORD offset = 0;
	offset -= offset%0x200;
	offset = 0;

	BOOL flag = TRUE;
	char win[HS] = {0};
	while(TRUE){
		if(offset>=size){
			flag = FALSE;
			break;
		}
		cf.Seek(offset+0x10, CFile::begin);
		cf.Read(win, HS);
		if(memcmp(win, head, HS)==0)
			break;
		offset += 0x200;
	}

	if(flag){
		char buffer[0x200] = {0};
		cf.Seek(offset, CFile::begin);
		cf.Read(buffer, 0x200);

		if(buffer[0x60]!=6){
			cf.Close();
			Tips("Format Error!",td_alert);
			return res;
		}
		DWORD loc = *(DWORD*)(buffer+0x64);

		DWORD pos = 0x30+loc;
		if(buffer[pos]==0x1E){
			//offset+pos为注释段头
			//segLen包括串尾符'\0'
			DWORD oldsegLen = *(DWORD*)(buffer+pos+4);

			DWORD segLen = strlen(buffer+pos+8);
			DWORD appendLen = segLen-oldsegLen+1;
			char* end = buffer+segLen;

			char tmp[0x200] = {0};
			memcpy(tmp,buffer+pos+8,segLen);
			char* start = strstr(tmp,(LPCSTR)WaterMarkHead);
			if(start!=NULL){//没有水印
				res += start+(strlen((LPCSTR)WaterMarkHead));
			}
		}
	}
	cf.Close();
	return res;
}
