// RefuseMail.cpp: implementation of the CRefuseMail class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "readings.h"
#include "RefuseMail.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRefuseMail::CRefuseMail()
{
	
}

CRefuseMail::CRefuseMail(jmail::IMessagePtr message)
{
	pMessage = message;
	MailID = _com_util::ConvertBSTRToString(pMessage->GetHeaders()->GetHeader("Message-ID"));
	body = _com_util::ConvertBSTRToString(pMessage->Body);
	int pos5 = body.find("username:" , 0);
	int pos6 = body.find("\r\n" , pos5 + 9);
	Replyer = body.substr(pos5 + 9,pos6 - pos5 - 9);
	pos5 = body.find("filename:" , pos6);
	pos6 = body.find("\r\n" , pos5 + 9);
	filename = body.substr(pos5 + 9,pos6 - pos5 - 9);
	pos5 = body.find("reason:" , pos6);
	pos6 = body.find("\r\n" , pos5 + 7);
	Reason = body.substr(pos5 + 7,pos6 - pos5 - 7);
}

CRefuseMail::~CRefuseMail()
{

}
