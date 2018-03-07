#if !defined(AFX_PAGE0_H__77DFDBE5_E941_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_PAGE0_H__77DFDBE5_E941_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page0.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage0 dialog

class CPage0 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage0)

// Construction
public:
	CPage0();
	~CPage0();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

// Dialog Data
	//{{AFX_DATA(CPage0)
	enum { IDD = IDD_DIALOG0 };
	BOOL	m_bPWater;
	BOOL	m_bSediment;
	BOOL	m_bQual;
	BOOL	m_bTemp;
	BOOL	m_bPH;
	BOOL	m_bSnow;
	BOOL	m_bOX;
	BOOL	m_bNUT;
	BOOL	m_bPLK;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage0)
	public:
	virtual LRESULT OnWizardNext();
	virtual void OnCancel();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage0)
	afx_msg void OnCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE0_H__77DFDBE5_E941_11D3_B2F2_00104B9CFF37__INCLUDED_)
