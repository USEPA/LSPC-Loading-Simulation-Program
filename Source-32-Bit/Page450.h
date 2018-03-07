// Page450.h: interface for the CPage450 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE450_H__BB96B962_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE450_H__BB96B962_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage450 : public CPageAdd  
{
public:
	CPage450();
	CPage450(UINT nID);
	virtual ~CPage450();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
};

#endif // !defined(AFX_PAGE450_H__BB96B962_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
