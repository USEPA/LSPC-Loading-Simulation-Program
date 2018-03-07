#if !defined(AFX_PAGE520_H__BB96B96A_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE520_H__BB96B96A_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page520.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage520 dialog

class CPage520 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage520)

// Construction
public:
	CPage520();
	~CPage520();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

// Dialog Data
	//{{AFX_DATA(CPage520)
	enum { IDD = IDD_DIALOG520 };
	BOOL	m_btamfg;
	BOOL	m_bno2fg;
	BOOL	m_bpo4fg;
	BOOL	m_bamvfg;
	BOOL	m_bdenfg;
	BOOL	m_badnhfg;
	BOOL	m_badpofg;
	BOOL	m_bmphfg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage520)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage520)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE520_H__BB96B96A_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
