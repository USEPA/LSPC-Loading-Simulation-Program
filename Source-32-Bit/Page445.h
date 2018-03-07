// Page445.h: interface for the CPage445 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE445_H__BB96B962_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE445_H__BB96B962_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage445 : public CPageAdd  
{
public:
	CPage445();
	CPage445(UINT nID);
	virtual ~CPage445();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
};

#endif // !defined(AFX_PAGE445_H__BB96B962_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
