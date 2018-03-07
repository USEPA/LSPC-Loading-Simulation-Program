#if !defined(AFX_PAGENOADD_H__F8E4F305_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_PAGENOADD_H__F8E4F305_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageNoAdd.h : header file
//
#include "GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CPageNoAdd dialog

class CPageNoAdd : public CDialog
{
	DECLARE_DYNCREATE(CPageNoAdd)

// Construction
public:
	CPageNoAdd();
	~CPageNoAdd();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

// Dialog Data
	//{{AFX_DATA(CPageNoAdd)
	enum { IDD = IDD_DIALOG2_2 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA
	CGridCtrl m_Grid;
	int nIndex;
	double *fh, *fsa, *fv, *fq;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageNoAdd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageNoAdd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
public:
	virtual void InitGrid();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGENOADD_H__F8E4F305_EE09_11D3_B2F2_00104B9CFF37__INCLUDED_)
