#if !defined(AFX_DLGWQ_H__F27A8DB9_6728_11D4_B304_00104B9CFF37__INCLUDED_)
#define AFX_DLGWQ_H__F27A8DB9_6728_11D4_B304_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgWQ.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgWQ dialog

class CDlgWQ : public CDialog
{
// Construction
public:
	CDlgWQ(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgWQ)
	enum { IDD = IDD_DLGWQ };
	BOOL	m_b8;
	BOOL	m_b12;
	BOOL	m_b19;
	BOOL	m_b20;
	BOOL	m_b21;
	BOOL	m_b22;
	BOOL	m_b23;
	BOOL	m_b24;
	BOOL	m_b25;
	BOOL	m_b1;
	BOOL	m_b2;
	BOOL	m_b3;
	BOOL	m_b4;
	BOOL	m_b5;
	BOOL	m_b6;
	BOOL	m_b7;
	BOOL	m_b10;
	BOOL	m_b9;
	BOOL	m_b11;
	BOOL	m_b13;
	BOOL	m_b15;
	BOOL	m_b14;
	BOOL	m_b16;
	BOOL	m_b17;
	BOOL	m_b18;
	BOOL	m_b26;
	BOOL	m_b27;
	BOOL	m_b28;
	BOOL	m_b29;
	BOOL	m_b36;
	BOOL	m_b37;
	BOOL	m_b38;
	BOOL	m_b39;
	BOOL	m_b40;
	BOOL	m_b41;
	BOOL	m_b42;
	BOOL	m_b43;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgWQ)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgWQ)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGWQ_H__F27A8DB9_6728_11D4_B304_00104B9CFF37__INCLUDED_)
