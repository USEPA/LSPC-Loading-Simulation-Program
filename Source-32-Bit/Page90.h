// Page90.h: interface for the CPage90 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE90_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_PAGE90_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage90 : public CPageAdd  
{
public:
	CPage90();
	CPage90(UINT nIDCap);
	virtual ~CPage90();
	virtual void InitGrid();
	virtual BOOL UpdateData(BOOL bSaveAndValidate = TRUE);
};

#endif // !defined(AFX_Page90_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
