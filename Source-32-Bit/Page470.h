// Page470.h: interface for the CPage470 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE470_H__BB96B965_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE470_H__BB96B965_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage470 : public CPageAdd  
{
public:
	CPage470();
	CPage470(UINT nID);
	virtual ~CPage470();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

};

#endif // !defined(AFX_PAGE470_H__BB96B965_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
