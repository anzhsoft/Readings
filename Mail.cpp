// Mail.cpp: implementation of the CMail class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Readings.h"
#include "Mail.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMail::CMail()
{

}

CMail::CMail(string title,string body,string id)
{
	MailBody = body;
	MailId = id;
	MailTitle = title;

	int pos1 = body.find("username:" , 0);
	int pos2 = body.find("\r\n" , pos1 + 9);
	RequesterUser = body.substr(pos1 + 9,pos2 - pos1 - 9);

	pos1 = body.find("email:" , pos2);
	pos2 = body.find("\r\n" , pos1 + 6);
	RequesterEmail = body.substr(pos1 + 6,pos2 - pos1 - 6);

	pos1 = body.find("filename:" , pos2);
	pos2 = body.find("\r\n" , pos1 + 9);
	RequestSourceFilename = body.substr(pos1 + 9,pos2 - pos1 - 9);
	files = parseResourceList(body);
	
}

CMail::~CMail()
{

}


CList<CString,CString&>* CMail::parseResourceList(string body)
{
	CList<CString,CString&>* resources=new CList<CString,CString&>();
	string key="resource-list:\r\n";
	int length=key.length();
	if(body.find(key,0)==-1){
		return resources;
	}
	int resource_begin=body.find(key,0)+length;
	char seps[]="\r\n";
	char *token;
	string rl=body.substr(resource_begin);
	char *resource_list=(char *)(rl.c_str());
    token = strtok(resource_list, seps );
    while( token != NULL )
    {	  
	  CString temp(token);
      resources->AddHead(temp);
      token = strtok( NULL, seps );
    }
	return resources;
}
