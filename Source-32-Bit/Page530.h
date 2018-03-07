#if !defined(AFX_PAGE530_H__BB96B96A_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE530_H__BB96B96A_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page530.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage530 dialog

class CPage530 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage530)

// Construction
public:
	CPage530();
	~CPage530();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

// Dialog Data
	//{{AFX_DATA(CPage530)
	enum { IDD = IDD_DIALOG530 };
	BOOL	m_bphyfg;
	BOOL	m_bzoofg;
	BOOL	m_bbalfg;
	BOOL	m_bsdltfg;
	BOOL	m_bamrfg;
	BOOL	m_bdecfg;
	BOOL	m_bnsfg;
	BOOL	m_borefg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage530)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage530)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE530_H__BB96B96A_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
