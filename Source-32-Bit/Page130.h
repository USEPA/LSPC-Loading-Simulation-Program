// Page130.h: interface for the CPage130 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE130_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_PAGE130_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage130 : public CPageAdd  
{
public:
	CPage130();
	CPage130(UINT nIDCap);
	virtual ~CPage130();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
};

#endif // !defined(AFX_Page130_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
