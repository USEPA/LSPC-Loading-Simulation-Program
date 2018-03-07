#if !defined(AFX_Page15_H__092E39A5_03F8_11D4_B2F6_00104B9CFF37__INCLUDED_)
#define AFX_Page15_H__092E39A5_03F8_11D4_B2F6_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page15.h : header file
//

#include "PageAdd.h"

class CPage15 : public CPageAdd  
{
public:
	CPage15();
	CPage15(UINT nIDCap);
	virtual ~CPage15();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Page15_H__092E39A5_03F8_11D4_B2F6_00104B9CFF37__INCLUDED_)
