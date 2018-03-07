// Page512.h: interface for the CPage512 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE512_H__BB96B967_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE512_H__BB96B967_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage512 : public CPageAdd  
{
public:
	CPage512();
	CPage512(UINT nID);
	virtual ~CPage512();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

};

#endif // !defined(AFX_PAGE512_H__BB96B967_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
