// Page455.h: interface for the CPage455 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE455_H__BB96B969_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE455_H__BB96B969_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage455 : public CPageAdd  
{
public:
	CPage455();
	CPage455(UINT nID);
	virtual ~CPage455();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

};

#endif // !defined(AFX_PAGE455_H__BB96B969_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
