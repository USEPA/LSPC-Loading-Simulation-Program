#if !defined(AFX_PAGE400_H__092E39A4_03F8_11D4_B2F6_00104B9CFF37__INCLUDED_)
#define AFX_PAGE400_H__092E39A4_03F8_11D4_B2F6_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page400.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage400 dialog

class CPage400 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage400)

// Construction
public:
	CPage400();
	~CPage400();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

// Dialog Data
	//{{AFX_DATA(CPage400)
	enum { IDD = IDD_DIALOG400 };
	int		m_nadmod;
	double	m_fkc;
	int		m_nsedber;
	int     m_nvconfg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage400)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage400)
	afx_msg void OnChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE400_H__092E39A4_03F8_11D4_B2F6_00104B9CFF37__INCLUDED_)
