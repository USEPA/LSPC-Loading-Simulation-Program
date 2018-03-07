#if !defined(AFX_PAGEOUTPUT_H__F27A8DB7_6728_11D4_B304_00104B9CFF37__INCLUDED_)
#define AFX_PAGEOUTPUT_H__F27A8DB7_6728_11D4_B304_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageOutput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageOutput dialog

class CPageOutput : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageOutput)

// Construction
public:
	CPageOutput();
	~CPageOutput();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

// Dialog Data
	//{{AFX_DATA(CPageOutput)
	enum { IDD = IDD_DLGOUTPUT };
	CComboBox	m_clanduse;
	CListBox	m_cPList;
	CString	m_strLocation;
	BOOL	m_bCustom;
	BOOL	m_bHydro;
	BOOL	m_bLanduse;
	BOOL	m_bSediment;
	BOOL	m_bWQ;
	BOOL	m_bDefault;
	BOOL	m_bSnow;
	BOOL	m_bWQR;
	BOOL	m_bStream;
	BOOL	m_bPointSource;
	BOOL	m_bThreshold;
	BOOL	m_bSUSTAIN;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageOutput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageOutput)
	afx_msg void OnBout();
	afx_msg void OnSeperate6();
	afx_msg void OnCustom();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCustomlist();
	afx_msg void OnDefaultchk();
	afx_msg void OnLanduse();
	afx_msg void OnStream();
	afx_msg void OnThreshold();
	afx_msg void OnSUSTAIN();
	afx_msg void OnPointSource();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEOUTPUT_H__F27A8DB7_6728_11D4_B304_00104B9CFF37__INCLUDED_)
