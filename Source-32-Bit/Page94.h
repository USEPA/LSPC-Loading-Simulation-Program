// Page94.h: interface for the CPage94 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE94_H__EE188935_5F15_11D4_B303_00104B9CFF37__INCLUDED_)
#define AFX_PAGE94_H__EE188935_5F15_11D4_B303_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage94 : public CPageAdd  
{
public:
	CPage94();
	CPage94(UINT nID);
	virtual ~CPage94();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
};

#endif // !defined(AFX_PAGE94_H__EE188935_5F15_11D4_B303_00104B9CFF37__INCLUDED_)
