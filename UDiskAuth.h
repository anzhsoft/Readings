// UDiskAuth.h: interface for the CUDiskAuth class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UDISKAUTH_H__16348E52_27E6_44C9_9CD9_B381C76AB200__INCLUDED_)
#define AFX_UDISKAUTH_H__16348E52_27E6_44C9_9CD9_B381C76AB200__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <malloc.h>
#include "usb.h"
//#include "devioctl.h"
//#include "usbdi.h"
#include <dbt.h>						
//#include <winioctl.h>					

extern "C" {
//#include "hidsdi.h"
#include <setupapi.h>
}
#include <setupapi.h>
#include <stdio.h>
#include <objbase.h>
class CUDiskAuth  
{
public:
	CUDiskAuth();
	virtual ~CUDiskAuth();

	void CrnReplaceString(LPSTR lpBuf, char c1, char c2);
	bool GetUSBDiskID(char cDiskID);

	char chFirstDriveFromMask(ULONG unitmask);


};

#endif // !defined(AFX_UDISKAUTH_H__16348E52_27E6_44C9_9CD9_B381C76AB200__INCLUDED_)
