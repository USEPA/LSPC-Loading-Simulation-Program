#if !defined(AFX_PAGE40_H__FECC0DC9_E955_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_PAGE40_H__FECC0DC9_E955_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page40.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage40 dialog

class CPage40 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage40)

// Construction
public:
	CPage40();
	~CPage40();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

// Dialog Data
	//{{AFX_DATA(CPage40)
	enum { IDD = IDD_DIALOG3 };
	int		m_nWaterShed;
	int		m_nChan;
	int		m_nRGroup;
	int		m_nGroup;
	int		m_nPLand;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage40)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage40)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocus();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE40_H__FECC0DC9_E955_11D3_B2F2_00104B9CFF37__INCLUDED_)
