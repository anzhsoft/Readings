// AgreeMail.cpp: implementation of the CAgreeMail class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "readings.h"
#include "AgreeMail.h"
#include "RequestMail.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAgreeMail::CAgreeMail()
{
	
}

CAgreeMail::CAgreeMail(jmail::IMessagePtr message)
{
	pMessage = message;
	MailID = _com_util::ConvertBSTRToString(pMessage->GetHeaders()->GetHeader("Message-ID"));
	body = _com_util::ConvertBSTRToString(pMessage->Body);
	FCResource = CRequestMail::parseResourceList(body);
	attname = pMessage->GetAttachments()->GetItem(0)->GetName();
	int pos3 = body.find("username:" , 0);
	int pos4 = body.find("\r\n" , pos3 + 9);
	AgreeReplyer = body.substr(pos3 + 9,pos4 - pos3 - 9);
	pos3 = body.find("email:" , pos4);
	pos4 = body.find("\r\n" , pos3 + 6);
	desEMail = body.substr(pos3 + 6 , pos4 - pos3 - 6);
}

CAgreeMail::~CAgreeMail()
{

}
