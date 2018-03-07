// PageAdd.h : header file
#if !defined(AFX_PAGEADD_H__FECC0DD7_E955_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_PAGEADD_H__FECC0DD7_E955_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//
#include "GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CPageAdd dialog

class CPageAdd : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageAdd)

// Construction
public:
	CPageAdd();
	CPageAdd(UINT nIDCaption);
	~CPageAdd();
	virtual BOOL UpdateData(BOOL bSaveAndValidate = TRUE);

// Dialog Data
	//{{AFX_DATA(CPageAdd)
	enum { IDD = IDD_DIALOG2 };
	CStatic	m_cInfo;
	CButton	m_cAdd;
	CButton	m_cDelete;
	CButton	m_cEdit;
	CButton	m_cExport;
	//}}AFX_DATA
	CGridCtrl m_Grid;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageAdd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageAdd)
	afx_msg void OnAdd();
	afx_msg void OnDelete();
	afx_msg void OnEdit();
	afx_msg void OnExport();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
public:
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	virtual void InitGrid();
	virtual void EditRow();
	virtual void ExportFtable();
	void Log(LPCTSTR lpszFormat, ...);
	void EnableButtons(bool bEnable);
	void ShowEditButton(bool bShow);
	void ShowExportButton(bool bShow);
	void ShowInfoText(bool bShow);
	void ArrangeCtrls();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEADD_H__FECC0DD7_E955_11D3_B2F2_00104B9CFF37__INCLUDED_)
