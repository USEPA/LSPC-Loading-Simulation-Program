// Page260.h: interface for the CPage260 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Page260_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_Page260_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage260 : public CPageAdd  
{
public:
	CPage260();
	CPage260(UINT nIDCap);
	virtual ~CPage260();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
};

#endif // !defined(AFX_Page260_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
