// ChargeMail.cpp: implementation of the CChargeMail class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "readings.h"
#include "ChargeMail.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CChargeMail::CChargeMail()
{
	
}

CChargeMail::CChargeMail(jmail::IMessagePtr message)
{
	pMessage = message;
	MailID = _com_util::ConvertBSTRToString(pMessage->GetHeaders()->GetHeader("Message-ID"));
	body = _com_util::ConvertBSTRToString(pMessage->Body);
	filenotfind = new CList<CString,CString&>();
	string key="filenotfind:\r\n";
	int length=key.length();
	int resource_begin=body.find(key,0)+length;
	char seps[]="\r\n";
	char *token;
	string rl=body.substr(resource_begin);
	char *resource_list=(char *)(rl.c_str());
	token = strtok(resource_list, seps );
	while( token != NULL )
	{	  
		CString temp(token);
		token = strtok( NULL, seps );
		filenotfind->AddTail(temp);
	}
	int pos3 = body.find("username:" , 0);
	int pos4 = body.find("\r\n" , pos3 + 9);
	Replyer = body.substr(pos3 + 9,pos4 - pos3 - 9);
	attlist = new CList<CString,CString&>();
	jmail::IAttachmentsPtr atts= pMessage->GetAttachments();
	int i = 0;
	if (atts != NULL)
	{
		for(;i<atts->Count;i++){
			string temp = _com_util::ConvertBSTRToString(pMessage->GetAttachments()->GetItem(i)->GetName());
			attlist->AddTail(CString(temp.c_str()));
		}
	}
}

CChargeMail::~CChargeMail()
{

}
