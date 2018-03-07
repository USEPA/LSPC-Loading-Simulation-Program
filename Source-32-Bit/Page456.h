// Page456.h: interface for the CPage456 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE456_H__BB96B969_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE456_H__BB96B969_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage456 : public CPageAdd  
{
public:
	CPage456();
	CPage456(UINT nID);
	virtual ~CPage456();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

};

#endif // !defined(AFX_PAGE456_H__BB96B969_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
