#if !defined(AFX_PAGE100_H__FECC0DD8_E955_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_PAGE100_H__FECC0DD8_E955_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page100.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage100 dialog

class CPage100 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage100)

// Construction
public:
	CPage100();
	~CPage100();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

// Dialog Data
	//{{AFX_DATA(CPage100)
	enum { IDD = IDD_DIALOG9 };
	BOOL	m_bvcsfg;
	BOOL	m_bvuzfg;
	BOOL	m_bvnnfg;
	BOOL	m_bvifwfg;
	BOOL	m_bvircfg;
	BOOL	m_bvlefg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage100)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage100)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE100_H__FECC0DD8_E955_11D3_B2F2_00104B9CFF37__INCLUDED_)
