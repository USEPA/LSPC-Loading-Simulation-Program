// Page180.h: interface for the CPage180 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Page180_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_Page180_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage180 : public CPageAdd  
{
public:
	CPage180();
	CPage180(UINT nIDCap);
	virtual ~CPage180();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
};

#endif // !defined(AFX_Page180_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
