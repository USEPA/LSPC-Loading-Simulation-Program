#if !defined(AFX_PAGE600_H__EE188936_5F15_11D4_B303_00104B9CFF37__INCLUDED_)
#define AFX_PAGE600_H__EE188936_5F15_11D4_B303_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page600.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage600 dialog

class CPage600 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage600)

// Construction
public:
	CPage600();
	~CPage600();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );


// Dialog Data
	//{{AFX_DATA(CPage600)
	enum { IDD = IDD_DIALOG600 };
	int		m_ncpt;
	int		m_ncland;
	int		m_ncrch;
	int		m_ntrgp;
	int		m_ntnum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage600)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage600)
	afx_msg void OnChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE600_H__EE188936_5F15_11D4_B303_00104B9CFF37__INCLUDED_)
