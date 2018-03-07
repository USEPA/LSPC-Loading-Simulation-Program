// Page80.h: interface for the CPage80 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE80_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_PAGE80_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage80 : public CPageAdd  
{
public:
	CPage80();
	CPage80(UINT nIDCap);
	virtual ~CPage80();
	virtual void InitGrid();
	virtual BOOL UpdateData(BOOL bSaveAndValidate = TRUE);
};

#endif // !defined(AFX_Page80_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
