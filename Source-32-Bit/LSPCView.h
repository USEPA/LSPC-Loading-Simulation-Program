// LSPCView.h : interface of the CLSPCView class
//
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_LSPCVIEW_H__472C279C_E940_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_LSPCVIEW_H__472C279C_E940_11D3_B2F2_00104B9CFF37__INCLUDED_

#include "LSPCDoc.h"
#include "MyPropertySheet1.h"
#include "MyPropertySheet2.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CLSPCView : public CFormView
{
protected: // create from serialization only
	CLSPCView();
	DECLARE_DYNCREATE(CLSPCView)

public:
	//{{AFX_DATA(CLSPCView)
	enum{ IDD = IDD_LSPC_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

public:
	int m_nPropertySheetNum;
	CMyPropertySheet1 *m_pMyPropSheet1;
	CMyPropertySheet2 *m_pMyPropSheet2;

// Attributes
public:
	CLSPCDoc* GetDocument();
	void SetActivePage(int nSheetNum, int nPageIndex);
	int GetGeneralIndex();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLSPCView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLSPCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLSPCView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in LSPCView.cpp
inline CLSPCDoc* CLSPCView::GetDocument()
   { return (CLSPCDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LSPCVIEW_H__472C279C_E940_11D3_B2F2_00104B9CFF37__INCLUDED_)
