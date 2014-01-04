// Reader.h: interface for the CReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_READER_H__DA09DC9C_FC63_41E0_9DD0_92F583F166B7__INCLUDED_)
#define AFX_READER_H__DA09DC9C_FC63_41E0_9DD0_92F583F166B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CReader  
{
public:
	CReader();
	virtual ~CReader();

	void Open();
	void Close();
	
public:
	int filetype;

protected:

};

#endif // !defined(AFX_READER_H__DA09DC9C_FC63_41E0_9DD0_92F583F166B7__INCLUDED_)
