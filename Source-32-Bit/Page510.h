#if !defined(AFX_PAGE510_H__BB96B96A_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE510_H__BB96B96A_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page510.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage510 dialog

class CPage510 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage510)

// Construction
public:
	CPage510();
	~CPage510();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

// Dialog Data
	//{{AFX_DATA(CPage510)
	enum { IDD = IDD_DIALOG510 };
	BOOL	m_bBenrfg;
	int		m_nReamfg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage510)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage510)
	afx_msg void OnChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE510_H__BB96B96A_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
