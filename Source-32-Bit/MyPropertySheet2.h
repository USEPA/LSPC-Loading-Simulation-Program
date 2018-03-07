/////////////////////////////////////////////////////////////////////////////
// MyPropertySheet2.h

#include "Page500.h"
#include "Page501.h"
#include "Page502.h"
#include "Page503.h"
#include "Page504.h"
#include "Page505.h"
#include "Page506.h"
#include "Page507.h"
#include "Page510.h"
#include "Page511.h"
#include "Page512.h"
#include "Page513.h"
#include "Page514.h"
#include "Page520.h"
#include "Page521.h"
#include "Page522.h"
#include "Page523.h"
#include "Page524.h"
#include "Page530.h"
#include "Page531.h"
#include "Page532.h"
#include "Page533.h"
#include "Page534.h"
#include "Page535.h"
#include "Page540.h"
#include "Page541.h"
#include "Page542.h"
#include "Page543.h"
#include "Page544.h"
#include "Page600.h"
#include "Page605.h"
#include "Page610.h"
#include "Page660.h"
#include "Page670.h"
#include "Page680.h"
#include "Page685.h"
#include "Page690.h"

#if !defined(AFX_MYPROPERTYSHEET2_H__77DFDBE0_E941_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_MYPROPERTYSHEET2_H__77DFDBE0_E941_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_RESIZEPAGE (WM_USER + 111)

/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet2

class CMyPropertySheet2 : public CPropertySheet
{
	DECLARE_DYNAMIC(CMyPropertySheet2)

// Construction
public:
	CMyPropertySheet2(CWnd* pParentWnd);
	//CMyPropertySheet2(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	CPage500 *page500;
	CPage501 *page501;
	CPage502  page502;
	CPage503 *page503;
	CPage504 *page504;
	CPage505 *page505;
	CPage506 *page506;
	CPage507 *page507;
	CPage510  page510;
	CPage511 *page511;
	CPage512 *page512;
	CPage513 *page513;
	CPage514 *page514;
	CPage520  page520;
	CPage521 *page521;
	CPage522 *page522;
	CPage523 *page523;
	CPage524 *page524;
	CPage530  page530;
	CPage531 *page531;
	CPage532 *page532;
	CPage533 *page533;
	CPage534 *page534;
	CPage535 *page535;
	CPage540  page540;
	CPage541 *page541;
	CPage542 *page542;
	CPage543 *page543;
	CPage544 *page544;
	CPage600  page600;
	CPage605 *page605;
	CPage610 *page610;
	CPage660 *page660;
	CPage670 *page670;
	CPage680 *page680;
	CPage685 *page685;
	CPage690 *page690;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyPropertySheet2)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void EnablePageButtons(bool bEnable);
	virtual void Resize();
	virtual ~CMyPropertySheet2();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );


	// Generated message map functions
protected:
	//{{AFX_MSG(CMyPropertySheet2)
	//}}AFX_MSG
	virtual afx_msg LRESULT OnResizePage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPROPERTYSHEET2_H__77DFDBE0_E941_11D3_B2F2_00104B9CFF37__INCLUDED_)
