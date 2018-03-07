#if !defined(AFX_PAGE460_H__84F6DEF9_D9DA_4732_B613_F92AA7A41C53__INCLUDED_)
#define AFX_PAGE460_H__84F6DEF9_D9DA_4732_B613_F92AA7A41C53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page460.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage460 dialog

class CPage460 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage460)

// Construction
public:
	CPage460();
	~CPage460();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

// Dialog Data
	//{{AFX_DATA(CPage460)
	enum { IDD = IDD_DIALOG460 };
	BOOL	m_msltfg;
	BOOL	m_miftfg;
	BOOL	m_mgwtfg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPage460)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPage460)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE460_H__84F6DEF9_D9DA_4732_B613_F92AA7A41C53__INCLUDED_)
