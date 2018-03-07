#if !defined(AFX_PAGE502_H__BB96B96A_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE502_H__BB96B96A_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page502.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage502 dialog

class CPage502 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage502)

// Construction
public:
	CPage502();
	~CPage502();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

// Dialog Data
	//{{AFX_DATA(CPage502)
	enum { IDD = IDD_DIALOG502 };
	BOOL	m_midofg;
	BOOL	m_mico2fg;
	BOOL	m_madofg;
	BOOL	m_maco2fg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage502)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage502)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE502_H__BB96B96A_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
