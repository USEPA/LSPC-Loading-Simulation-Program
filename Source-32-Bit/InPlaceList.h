#if !defined(AFX_INPLACELIST_H__ECD42822_16DF_11D1_992F_895E185F9C72__INCLUDED_)
#define AFX_INPLACELIST_H__ECD42822_16DF_11D1_992F_895E185F9C72__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// InPlaceList.h : header file
//
//
/////////////////////////////////////////////////////////////////////////////

#define IDC_COMBOEDIT 1001

/////////////////////////////////////////////////////////////////////////////
// CComboEdit window

class CComboEdit : public CEdit
{
// Construction
public:
	CComboEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboEdit)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CComboEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CComboEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CInPlaceList window

class CInPlaceList : public CComboBox
{
    friend class CComboEdit;

// Construction
public:
	CInPlaceList(CWnd* pParent,         // parent
                 CRect& rect,           // dimensions & location
                 DWORD dwStyle,         // window/combobox style
                 UINT nID,              // control ID
                 int nRow, int nColumn, // row and column
				 CStringArray& Items,   // Items in list
                 CString sInitText,     // initial selection
				 UINT nFirstChar);      // first character to pass to control

// Attributes
public:
   CComboEdit m_edit;  // subclassed edit control

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInPlaceList)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInPlaceList();
protected:
    void EndEdit();

// Generated message map functions
protected:
	//{{AFX_MSG(CInPlaceList)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	int		m_nNumLines;
	CString m_sInitText;
	int		m_nRow;
	int		m_nCol;
 	UINT    m_nLastChar; 
	BOOL	m_bExitOnArrows; 
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPLACELIST_H__ECD42822_16DF_11D1_992F_895E185F9C72__INCLUDED_)
