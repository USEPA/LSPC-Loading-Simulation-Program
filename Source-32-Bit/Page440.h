#if !defined(AFX_PAGE440_H__A155812A_D992_4E59_8407_70144E6CFF22__INCLUDED_)
#define AFX_PAGE440_H__A155812A_D992_4E59_8407_70144E6CFF22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page440.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage440 dialog

class CPage440 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage440)

// Construction
public:
	CPage440();   
	~CPage440();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

// Dialog Data
	//{{AFX_DATA(CPage440)
	enum { IDD = IDD_DIALOG440 };
	int		m_ncrvfg;
	int		m_nvsivfg;
	int		m_nsandfg;
	int		m_nsweepfg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPage440)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPage440)
	afx_msg void OnChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE440_H__A155812A_D992_4E59_8407_70144E6CFF22__INCLUDED_)
