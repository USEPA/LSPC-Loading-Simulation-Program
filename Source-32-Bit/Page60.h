// Page60.h: interface for the CPage60 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGEGROUP_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_PAGE60_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage60 : public CPageAdd  
{
public:
	CPage60();
	CPage60(UINT nIDCap);
	virtual ~CPage60();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
};

#endif // !defined(AFX_PAGE60_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
