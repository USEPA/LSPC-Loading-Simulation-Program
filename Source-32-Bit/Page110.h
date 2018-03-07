// Page110.h: interface for the CPage110 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE110_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_PAGE110_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage110 : public CPageAdd  
{
public:
	CPage110();
	CPage110(UINT nIDCap);
	virtual ~CPage110();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
};

#endif // !defined(AFX_Page110_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
