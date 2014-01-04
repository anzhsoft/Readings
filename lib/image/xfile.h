/*
 * File:	xfile.h
 * Purpose:	General Purpose File Class 
 */
/* === C R E D I T S  &  D I S C L A I M E R S ==============
 * Permission is given by the author to freely redistribute and include
 * this code in any program as long as this credit is given where due.
 *
 * CxFile (c)  11/May/2002 <ing.davide.pizzolato@libero.it>
 * CxFile version 1.00 11/May/2002
 * See the file history.htm for the complete bugfix and news report.
 *
 * COVERED CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY
 * OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES
 * THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE
 * OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED
 * CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT
 * THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY
 * SERVICING, REPAIR OR CORRECTION. THIS DISCLAIMER OF WARRANTY CONSTITUTES AN ESSENTIAL
 * PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER
 * THIS DISCLAIMER.
 *
 * Use at your own risk!
 * ==========================================================
 */
#if !defined(__xfile_h)
#define __xfile_h

#include <windows.h>
#include <stdio.h>

class CxFile
{
public:
	CxFile(long type = 0);
	CxFile(FILE* pFile);
	CxFile(BYTE* pBuffer, DWORD size);
	~CxFile();
	void*	Open(const char *filename, const char *mode);
	long	Close();
	size_t	Read(void *buffer, size_t size, size_t count);
	size_t	Write(const void *buffer, size_t size, size_t count);
	long	Seek(long offset, int origin);
	long	Tell();
	long	Size();
	long	Flush();
	long	Eof();
	long	Error();
	long	PutC(unsigned char c);
	long	GetC();
	long	GetPos(fpos_t *pos);
	void*	GetStream() const {return m_stream;}
	void*	Alloc(DWORD nBytes);
	void	Free();
	void	Transfer(CxFile &from);

protected:
	void	*m_stream;	//file or memory pointer or whatever
	long	m_StorageType; //type (0=file, 1=memory, ...)
	long	m_Position;	//current position
	long	m_Size;		//file size
	long	m_Edge;		//buffer size
	long	m_DontCloseOnExit;
};

#endif //__xfile_h