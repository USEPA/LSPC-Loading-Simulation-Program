// Page250.h: interface for the CPage250 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Page250_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_Page250_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage250 : public CPageAdd  
{
public:
	CPage250();
	CPage250(UINT nIDCap);
	virtual ~CPage250();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
};

#endif // !defined(AFX_Page250_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
