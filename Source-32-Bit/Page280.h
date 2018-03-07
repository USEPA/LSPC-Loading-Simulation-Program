#if !defined(AFX_PAGE280_H__DE4ABCF7_56DE_4F92_B707_5E430A6EEA0C__INCLUDED_)
#define AFX_PAGE280_H__DE4ABCF7_56DE_4F92_B707_5E430A6EEA0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page280.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage280 dialog

class CPage280 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage280)
// Construction
public:
	CPage280();
	~CPage280();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

// Dialog Data
	//{{AFX_DATA(CPage280)
	enum { IDD = IDD_DIALOG280 };
	BOOL	m_badfg;
	BOOL	m_baddfg;
	BOOL	m_bawdfg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPage280)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPage280)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE280_H__DE4ABCF7_56DE_4F92_B707_5E430A6EEA0C__INCLUDED_)
