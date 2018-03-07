#if !defined(AFX_PAGE30_H__BB96B96C_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE30_H__BB96B96C_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page30.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage30 dialog

class CPage30 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage30)

// Construction
public:
	CPage30();
	~CPage30();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );


// Dialog Data
	//{{AFX_DATA(CPage30)
	enum { IDD = IDD_DIALOG30 };
	CString	m_strWeatherLocation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage30)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage30)
	afx_msg void OnWssb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE30_H__BB96B96C_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
