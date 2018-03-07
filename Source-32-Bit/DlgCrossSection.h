#if !defined(AFX_DLGCROSSSECTION_H__F8943095_3B13_44AE_B7ED_51ADFCA7293C__INCLUDED_)
#define AFX_DLGCROSSSECTION_H__F8943095_3B13_44AE_B7ED_51ADFCA7293C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCrossSection.h : header file
//
#include "poly.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCrossSection dialog

class CDlgCrossSection : public CDialog
{
// Construction
public:
	CDlgCrossSection(CWnd* pParent = NULL);   // standard constructor

	enum EDITMODE
	{ 
	  ADDMODE,
	  EDITMODE,
	  NOMODE
	};

	int nIndex;
	CPolygon *poly;

	//the following two memembers are added for editing
	int nMode;
	int nSel;
	bool bDrawBasic;

// Dialog Data
	//{{AFX_DATA(CDlgCrossSection)
	enum { IDD = IDD_DLGCROSS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCrossSection)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgCrossSection)
	afx_msg void OnClear();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnFtable();
	afx_msg void OnEdit();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCROSSSECTION_H__F8943095_3B13_44AE_B7ED_51ADFCA7293C__INCLUDED_)
