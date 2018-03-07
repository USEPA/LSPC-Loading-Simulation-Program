// Page532.h: interface for the CPage532 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE532_H__BB96B967_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE532_H__BB96B967_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage532 : public CPageAdd  
{
public:
	CPage532();
	CPage532(UINT nID);
	virtual ~CPage532();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

};

#endif // !defined(AFX_PAGE532_H__BB96B967_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
