// LSPCDoc.h : interface of the CLSPCDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LSPCDOC_H__472C279A_E940_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_LSPCDOC_H__472C279A_E940_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LSPCMODEL.h"

class CLSPCDoc : public CDocument
{
protected: // create from serialization only
	CLSPCDoc();
	DECLARE_DYNCREATE(CLSPCDoc)

// Attributes
public:
	CLSPCMODEL lspc;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLSPCDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLSPCDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CLSPCDoc)
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LSPCDOC_H__472C279A_E940_11D3_B2F2_00104B9CFF37__INCLUDED_)
