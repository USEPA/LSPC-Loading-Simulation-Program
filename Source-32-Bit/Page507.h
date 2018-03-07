// Page507.h: interface for the CPage507 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE507_H__BB96B969_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE507_H__BB96B969_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage507 : public CPageAdd  
{
public:
	CPage507();
	CPage507(UINT nID);
	virtual ~CPage507();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

};

#endif // !defined(AFX_PAGE507_H__BB96B969_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
