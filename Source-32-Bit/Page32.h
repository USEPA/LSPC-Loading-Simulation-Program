#if !defined(AFX_Page32_H__8F7CE4C0_5683_4780_B8FD_91B5D1CE9CF2__INCLUDED_)
#define AFX_Page32_H__8F7CE4C0_5683_4780_B8FD_91B5D1CE9CF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page32.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage32 dialog

class CPage32 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage32)
// Construction
public:
	CPage32();
	~CPage32();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

// Dialog Data
	//{{AFX_DATA(CPage32)
	enum { IDD = IDD_DIALOG91 };
	int		m_nLUchangeFlag;
	CString	m_strLUchangeFilePath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPage32)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPage32)
		// NOTE: the ClassWizard will add member functions here
	afx_msg void OnChange();
	afx_msg void OnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Page32_H__8F7CE4C0_5683_4780_B8FD_91B5D1CE9CF2__INCLUDED_)
