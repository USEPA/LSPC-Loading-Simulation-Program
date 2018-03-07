#if !defined(AFX_PAGE201_H__A437D826_2522_4C10_B00A_B98852EA7A9B__INCLUDED_)
#define AFX_PAGE201_H__A437D826_2522_4C10_B00A_B98852EA7A9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page201.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage201 dialog
//Card 201 sets the irrigation option
//if the m_bIrrig = true then consider the irrigation option
//during water balance calculations
class CPage201 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage201)

// Construction
public:
	CPage201();
	~CPage201();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
// Dialog Data
	//{{AFX_DATA(CPage201)
	enum { IDD = IDD_DIALOGIRRIG };
	BOOL  m_bIrrig ;
	BOOL  m_bMonVary ;
	BOOL  m_bPET;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage201)
    public:
    virtual LRESULT OnWizardNext();
    virtual void OnCancel();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage201)
		// NOTE: the ClassWizard will add member functions here
	afx_msg void OnCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE201_H__A437D826_2522_4C10_B00A_B98852EA7A9B__INCLUDED_)
