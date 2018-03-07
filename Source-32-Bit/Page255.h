#if !defined(AFX_PAGE255_H__FECC0DDA_E955_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_PAGE255_H__FECC0DDA_E955_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page255.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage255 dialog

class CPage255 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage255)

// Construction
public:
	CPage255();
	~CPage255();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

// Dialog Data
	//{{AFX_DATA(CPage255)
	enum { IDD = IDD_DIALOG25 };
	BOOL	m_bvqofg;
	BOOL	m_bqifwfg;
	BOOL	m_bviqcfg;
	BOOL	m_bqagwfg;
	BOOL	m_bvaqcfg;
	BOOL	m_badfxfg;
	BOOL	m_bqsowfg;
	BOOL	m_bvsqcfg;
	BOOL	m_bmaddfg;
	BOOL	m_bmawdfg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage255)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage255)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE255_H__FECC0DDA_E955_11D3_B2F2_00104B9CFF37__INCLUDED_)
