#if !defined(AFX_PAGE420_H__0EC280DD_C58A_4257_801B_70E8E30EE701__INCLUDED_)
#define AFX_PAGE420_H__0EC280DD_C58A_4257_801B_70E8E30EE701__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page420.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage420 property page

class CPage420 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage420)
// Construction
public:
	CPage420();
	~CPage420();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

// Dialog Data
	//{{AFX_DATA(CPage420)
	enum { IDD = IDD_DIALOG420 };
	int		m_nPtSource;
	int		m_nPtQuals;
	CString	m_strPSLocation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPage420)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPage420)
	afx_msg void OnChange();
	afx_msg void OnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE420_H__0EC280DD_C58A_4257_801B_70E8E30EE701__INCLUDED_)
