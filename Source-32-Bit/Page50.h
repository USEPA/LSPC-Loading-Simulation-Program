#if !defined(AFX_PAGE50_H__FECC0DCB_E955_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_PAGE50_H__FECC0DCB_E955_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page50.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage50 dialog

class CPage50 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage50)

// Construction
public:
	CPage50();   // standard constructor
	~CPage50();   // standard destructor
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

// Dialog Data
	//{{AFX_DATA(CPage50)
	enum { IDD = IDD_TempSettings };
	CComboBox	m_oCombo;
	int 		m_nMinutes;															//   (09-03)
	//}}AFX_DATA
	CDateTimeCtrl m_cStart;
	CDateTimeCtrl m_cEnd;

	COleDateTime tStart;
	COleDateTime tEnd;

	CDateTimeCtrl m_oStart;			// (03-2005)
	CDateTimeCtrl m_oEnd;

	COleDateTime toStart;			// (03-2005)
	COleDateTime toEnd;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPage50)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPage50)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusEdit1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE50_H__FECC0DCB_E955_11D3_B2F2_00104B9CFF37__INCLUDED_)
