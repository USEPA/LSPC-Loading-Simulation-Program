// Page287.h: interface for the CPage287 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Page287_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_Page287_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage287 : public CPageAdd  
{
public:
	CPage287();
	CPage287(UINT nIDCap);
	virtual ~CPage287();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
};

#endif // !defined(AFX_Page287_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
