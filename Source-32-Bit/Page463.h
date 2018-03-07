// Page463.h: interface for the CPage463 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE463_H__BB96B967_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE463_H__BB96B967_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage463 : public CPageAdd  
{
public:
	CPage463();
	CPage463(UINT nID);
	virtual ~CPage463();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

};

#endif // !defined(AFX_PAGE463_H__BB96B967_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
