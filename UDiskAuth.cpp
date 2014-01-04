// UDiskAuth.cpp: implementation of the CUDiskAuth class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "readings.h"
#include "UDiskAuth.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern USB_DETAIL_INFO uDiskAuthInfo;
extern CHAR UserName[50];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUDiskAuth::CUDiskAuth()
{

}

CUDiskAuth::~CUDiskAuth()
{

}



CHAR buf[512];
TCHAR   szMoveDiskName[33];
TCHAR	szDrvName[33];

//---------------------------------------------------------------------------
void CUDiskAuth::CrnReplaceString(LPSTR lpBuf, char c1, char c2)
{
    int nLen = strlen(lpBuf);
    LPSTR lpTemp = new char[nLen];
    memset(lpTemp, 0, nLen);
    int j=0;
    for(int i=0; i < nLen; i++)
    {
        if(lpBuf[i] == c1)
        {
            if(c2 != 0x00)
            {
                lpTemp[j] = c2;
                j++;
            }
        }
        else
        {
            lpTemp[j] = lpBuf[i];
            j++;
        }
    }
    //
    memset(lpBuf, 0, nLen);
	strncpy_s(lpBuf, nLen, lpTemp, nLen);
    delete []lpTemp;
}
//---------------------------------------------------------------------------
// ��ȡָ���̷������к�(���USB������Ч)
// char cDiskID: ָ���̷����� 'L'
// LPSTR lpPID : ���кŴ�ŵĻ��壬��char szPID[32]
// by ccrun(����) info@ccrun.com
//---------------------------------------------------------------------------
bool CUDiskAuth::GetUSBDiskID(char cDiskID)
{
    char szDrv[4];
    sprintf_s(szDrv, 4, "%c:\\", cDiskID);
    if(GetDriveType(szDrv) != DRIVE_REMOVABLE)
    {
          return false;
    }
	
    char lpRegPath[512] = { 0 };
    char lpRegValue[256] = { 0 };
    sprintf_s(lpRegPath, 512, "SYSTEM\\MountedDevices");
    sprintf_s(lpRegValue, 256, "\\DosDevices\\%c:", cDiskID);
    //
    DWORD dwDataSize(0);
    DWORD dwRegType(REG_BINARY);
    LPBYTE lpRegBinData(NULL);
    LPSTR lpUSBKeyData(NULL);
	
    // ��ѯע�����ӳ�����������豸��Ϣ
    HKEY hKey;
    long lRet = ::RegOpenKeyEx(
		HKEY_LOCAL_MACHINE, // root key
		lpRegPath, // Ҫ���ʵļ���λ��
		0,         //
		KEY_READ,  // �Բ�ѯ�ķ�ʽ����ע���
		&hKey);    // hKEY����˺������򿪵ļ��ľ��
    if(lRet != ERROR_SUCCESS)
        return false;
    else
    {
        lRet = ::RegQueryValueEx(hKey, // ���򿪵ļ��ľ��
			lpRegValue,    // Ҫ��ѯ�ļ�ֵ��
			NULL,
			&dwRegType,    // ��ѯ���ݵ�����
			lpRegBinData,  // ��������ѯ������
			&dwDataSize);  // Ԥ���õ����ݳ���
        if(lRet != ERROR_SUCCESS)
        {
            ::RegCloseKey(hKey);
            return false;
        }
        else
        {
            lpRegBinData = new BYTE[dwDataSize];
            lpUSBKeyData = new char[dwDataSize];
            memset(lpUSBKeyData, 0, dwDataSize);
            lRet = ::RegQueryValueEx(hKey,
				lpRegValue,
				NULL,
				&dwRegType,
				lpRegBinData,
				&dwDataSize);
            if(lRet != ERROR_SUCCESS)
            {
                delete []lpRegBinData;
                delete []lpUSBKeyData;
                ::RegCloseKey(hKey);
                return false;
            }
        }
    }
    ::RegCloseKey(hKey);
    // ���˶����ƴ��е�������Ϣ(��0x0�ַ�ȥ��)
    int j = 0;
    for(DWORD i=0; i<dwDataSize; i++)
    {
        if(lpRegBinData[i] != 0x0)
        {
            lpUSBKeyData[j] = lpRegBinData[i];
            j++;
        }
    }
    delete []lpRegBinData;
	
    // ��ȡlpUSBKeyData�е�������Ϣ, ��: 7&100a16f&0
    // \??\STORAGE#RemovableMedia#7&100a16f&0&RM#{53f5630d-b6bf-11d0-94f2-00a0c91efb8b}
    // 63 63 72 75 6E 2E 63 6F 6D
    LPSTR lpPos1 = strstr(lpUSBKeyData, "#RemovableMedia#") + 16;
    LPSTR lpPos2 = strstr(lpUSBKeyData, "RM");
    strncpy_s(lpUSBKeyData, dwDataSize, lpPos1, int(lpPos2) - int(lpPos1));
    lpUSBKeyData[int(lpPos2) - int(lpPos1) - 1] = 0x0;
    strcpy_s(lpUSBKeyData, dwDataSize, _strupr(lpUSBKeyData));
	
    // Disk Device(�����豸)��GUID
    GUID guidUSB;
    CLSIDFromString(L"{53f56307-b6bf-11d0-94f2-00a0c91efb8b}", &guidUSB);
	HDEVINFO hUSB;
    //
  //  HDEVINFO hUSB = SetupDiGetClassDevs(
	//	&guidUSB, NULL, 0, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if(hUSB == INVALID_HANDLE_VALUE)
    {
        delete []lpUSBKeyData;
        return false;
    }
    //
    int nDevIndex = 0;
    bool bSuccess;
    SP_DEVINFO_DATA DevData;
    SP_DEVICE_INTERFACE_DATA DevIntData;
    PSP_DEVICE_INTERFACE_DETAIL_DATA lpDevIntDetailData;
    DWORD dwBytesReturned;
	BOOL validUDisk = FALSE;
	
    // ���������豸
    do
    {
        DevIntData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
       // bSuccess = SetupDiEnumDeviceInterfaces(hUSB, NULL, &guidUSB,
		//	nDevIndex, &DevIntData) > 0;
		// ����ת�� C++Builder�о� - http://www.ccrun.com/article.asp?i=1021&d=fgp83l
        if(bSuccess)
        {
            // ��ȡ�ӿ���ϸ��Ϣ
            DevData.cbSize = sizeof(SP_DEVINFO_DATA);
            dwBytesReturned = 0;
			//anzhsoft
         //   SetupDiGetDeviceInterfaceDetailA(hUSB, &DevIntData,
		//		NULL, 0, &dwBytesReturned, &DevData);
            if(dwBytesReturned != 0 && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
                lpDevIntDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)
					GlobalAlloc(GMEM_FIXED, dwBytesReturned);
                lpDevIntDetailData->cbSize =
					sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
                if(SetupDiGetDeviceInterfaceDetailA(hUSB, &DevIntData,
					lpDevIntDetailData, dwBytesReturned, &dwBytesReturned, &DevData))
                {
                    // ȡ���豸�ӿ���ϸ��Ϣ������ת�����·����ע����в�ѯ
                    LPSTR lpPathTemp = new char[strlen(lpDevIntDetailData->DevicePath) + 256];
                    strcpy_s(lpRegPath, 512, "SYSTEM\\CurrentControlSet\\Enum\\");
                    strcpy_s(lpPathTemp, strlen(lpDevIntDetailData->DevicePath) + 256, lpDevIntDetailData->DevicePath);
                    lpPos1 = LPSTR(int(lpPathTemp) + 4);
                    lpPos2 = LPSTR(int(strstr(lpPathTemp, "{")) - 1);
                    strncpy_s(lpPathTemp, strlen(lpDevIntDetailData->DevicePath) + 256, lpPos1, int(lpPos2) - int(lpPos1));
					char buf[100]={0};
					strncpy_s(buf, 100, lpPathTemp,7);
					buf[strlen(buf)]='\0';
				//	cout<<buf<<endl;;
				//	AfxMessageBox(buf);
					if (strcmp(buf, strlwr(HANLINHEAD))==0)
					{
						//usbstor#disk&ven_linux&prod_file-stor_gadget&rev_0319#3238204e6f
						char productName[6]={0},PID[10]={0},VID[17]={0},ID[14]={0};
						strncpy_s(productName, 6, lpPathTemp+17,5);
						strncpy_s(VID, 17, lpPathTemp+28,16);
						strncpy_s(PID, 10, lpPathTemp+49,4);
						strncpy_s(ID, 14, lpPathTemp+54,12);
						strcpy_s(ID, 14,_strupr(ID));
//						cout<<ID<<endl;
						if (strcmp(productName,(HANLINNAME))==0
						  &&strcmp(VID,HANLINVID)==0
						  &&(strcmp(PID,HANLINPID_V3)==0 ||strcmp(PID,HANLINPID_V30)==0)
						  &&ID[0]
						  &&(strcmp(UserName,"anzhsoft")==0 ||strcmp(UserName,"faintfaint")==0
						  ||strcmp(UserName,"marklee") ==0
						  ||strcmp(UserName,"start_dzh") ==0)
							)
						{
						//	cout<<"here\n";
							validUDisk = TRUE;
							strcpy(uDiskAuthInfo.DevName,productName);
							strcpy(uDiskAuthInfo.PID,PID);
							strcpy(uDiskAuthInfo.VID,VID);
							uDiskAuthInfo.diskFlag = cDiskID;
							uDiskAuthInfo.flag = TRUE;
						//	AfxMessageBox(VID);
						}
						
					//	cout<<"&*)(&^*(*(*("<<lpPathTemp<<endl<<endl;
					}
				//	AfxMessageBox(lpPathTemp);
                    lpPathTemp[int(lpPos2) - int(lpPos1)] = 0x0;
                    CrnReplaceString(lpPathTemp, '#', '\\');
				
					
                    strcat(lpRegPath, lpPathTemp);
                    //
                    if(RegOpenKeyEx(
						HKEY_LOCAL_MACHINE,
						lpRegPath,
						0,
						KEY_READ,
						&hKey) == ERROR_SUCCESS)
                    {
                        dwRegType = REG_SZ;
                        LPSTR lpRegSzData = NULL;
                        dwDataSize = 0;
                        lRet = ::RegQueryValueEx(hKey,
							"ParentIdPrefix",
							NULL,
							&dwRegType,
							(LPBYTE)lpRegSzData,
							&dwDataSize);
                        if(lRet == ERROR_SUCCESS)
                        {
                            lpRegSzData = new char[dwDataSize];
                            lRet = ::RegQueryValueEx(hKey,
								"ParentIdPrefix",
								NULL,
								&dwRegType,
								(LPBYTE)lpRegSzData,
								&dwDataSize);
                            if(lRet == ERROR_SUCCESS)
                            {
                                strcpy(lpRegSzData, strupr(lpRegSzData));
                                if(!strcmp(lpUSBKeyData, lpRegSzData))
                                {
                                    // ���ȶԣ��ҵ�Ҫ��ѯ�Ĵ����豸
                                    strcpy(lpPathTemp, LPSTR(int(strstr(
                                        lpDevIntDetailData->DevicePath, "#")) + 1));
                                    lpPos1 = LPSTR(int(strstr(lpPathTemp, "#")) + 1);
                                    lpPos2 = LPSTR(int(strstr(lpPathTemp, "#{")));
                                    strncpy(lpPathTemp, lpPos1, int(lpPos2) - int(lpPos1));
                                    lpPathTemp[int(lpPos2) - int(lpPos1)] = 0x0;
									
                                    // ��ȡ���յĴ������к�
                                    CrnReplaceString(lpPathTemp, '&', 0x00);
									char buf[100];
                                    strncpy(buf, strupr(lpPathTemp), 32);
									if (validUDisk)
									{
										strcpy(uDiskAuthInfo.ID,buf);
									}
                                    //
                                    delete []lpRegSzData;
                                    delete []lpPathTemp;
                                    GlobalFree(lpDevIntDetailData);
                                    ::RegCloseKey(hKey);
                                    break;
                                }
                            }
                            delete []lpRegSzData;
                        }
                        ::RegCloseKey(hKey);
                    }
                    delete []lpPathTemp;
                    nDevIndex++;
                }
                GlobalFree(lpDevIntDetailData);
            }
        }
    }while(bSuccess);
	//anzhsoft for paltform
    //SetupDiDestroyDeviceInfoList(hUSB);
	
    delete []lpUSBKeyData;
	
    return validUDisk;
}



