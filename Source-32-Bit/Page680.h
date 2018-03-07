// Page680.h: interface for the CPage680 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Page680_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_Page680_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage680 : public CPageAdd  
{
public:
	CPage680();
	CPage680(UINT nIDCap);
	virtual ~CPage680();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
};

#endif // !defined(AFX_Page680_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
