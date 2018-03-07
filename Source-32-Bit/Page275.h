// Page275.h: interface for the CPage275 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Page275_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_Page275_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage275 : public CPageAdd  
{
public:
	CPage275();
	CPage275(UINT nIDCap);
	virtual ~CPage275();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
};

#endif // !defined(AFX_Page275_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
