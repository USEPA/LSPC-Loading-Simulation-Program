#if !defined(AFX_PAGE540_H__71E42F0E_2624_4EDE_905A_6AC4EBEF83CB__INCLUDED_)
#define AFX_PAGE540_H__71E42F0E_2624_4EDE_905A_6AC4EBEF83CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page540.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage540 dialog

class CPage540 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage540)

// Construction
public:
	CPage540();
	~CPage540();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

// Dialog Data
	//{{AFX_DATA(CPage540)
	enum { IDD = IDD_DIALOG540 };
	int		m_nphffg1;
	int		m_nphffg2;
	double	m_lfphfrc1;
	double	m_lfphfrc2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPage540)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPage540)
	afx_msg void OnChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE540_H__71E42F0E_2624_4EDE_905A_6AC4EBEF83CB__INCLUDED_)
