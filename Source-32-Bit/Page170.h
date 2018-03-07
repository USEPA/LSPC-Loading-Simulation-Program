// Page170.h: interface for the CPage170 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Page170_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_Page170_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage170 : public CPageAdd  
{
public:
	CPage170();
	CPage170(UINT nIDCap);
	virtual ~CPage170();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
};

#endif // !defined(AFX_Page170_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
