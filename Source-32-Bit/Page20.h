// Page20.h: interface for the CPage20 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE20_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_PAGE20_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage20 : public CPageAdd  
{
public:
	CPage20();
	CPage20(UINT nIDCap);
	virtual ~CPage20();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
};

#endif // !defined(AFX_PAGE20_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
