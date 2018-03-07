// Page514.h: interface for the CPage514 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE514_H__BB96B969_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE514_H__BB96B969_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage514 : public CPageAdd  
{
public:
	CPage514();
	CPage514(UINT nID);
	virtual ~CPage514();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

};

#endif // !defined(AFX_PAGE514_H__BB96B969_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
