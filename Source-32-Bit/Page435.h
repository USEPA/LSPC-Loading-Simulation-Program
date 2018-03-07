#if !defined(AFX_PAGE435_H__092E39A5_03F8_11D4_B2F6_00104B9CFF37__INCLUDED_)
#define AFX_PAGE435_H__092E39A5_03F8_11D4_B2F6_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page435.h : header file
//

#include "PageAdd.h"

class CPage435 : public CPageAdd  
{
public:
	CPage435();
	CPage435(UINT nIDCap);
	virtual ~CPage435();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE435_H__092E39A5_03F8_11D4_B2F6_00104B9CFF37__INCLUDED_)
