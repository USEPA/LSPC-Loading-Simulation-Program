// Page457.h : header file
//
#if !defined(AFX_PAGE457_H__092E39A5_03F8_11D4_B2F6_00104B9CFF37__INCLUDED_)
#define AFX_PAGE457_H__092E39A5_03F8_11D4_B2F6_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage457 : public CPageAdd  
{
public:
	CPage457();
	CPage457(UINT nIDCap);
	virtual ~CPage457();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE457_H__092E39A5_03F8_11D4_B2F6_00104B9CFF37__INCLUDED_)
