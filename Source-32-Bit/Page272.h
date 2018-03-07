// Page272.h: interface for the CPage272 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Page272_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_Page272_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage272 : public CPageAdd  
{
public:
	CPage272();
	CPage272(UINT nIDCap);
	virtual ~CPage272();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
};

#endif // !defined(AFX_Page272_H__F8E4F306_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
