// Page500.h: interface for the CPage500 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE500_H__BB96B966_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE500_H__BB96B966_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage500 : public CPageAdd  
{
public:
	CPage500();
	CPage500(UINT nID);
	virtual ~CPage500();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

};

#endif // !defined(AFX_PAGE500_H__BB96B966_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
