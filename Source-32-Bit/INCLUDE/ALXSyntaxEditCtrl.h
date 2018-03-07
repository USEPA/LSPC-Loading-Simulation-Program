#if !defined(ALX_SYNTAXEDITCTRL_H__INCLUDED_)
#define ALX_SYNTAXEDITCTRL_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ALXSyntaxEditCtrl.h : header file
//

#include <afxcmn.h>			// MFC support for Windows Common Controls
#include "ALXParser.h"

#define TXT_CLR_DEFAULT		RGB(0,0,0)
#define TXT_CLR_FUNCTION	RGB(0,0,255)
#define TXT_CLR_STRING		RGB(128,0,128)
#define TXT_CLR_NUM			RGB(0,0,160)
#define TXT_CLR_DATE		RGB(128,0,0)
#define TXT_CLR_BOOL		RGB(128,0,255)
#define TXT_CLR_LOGIC_OPR	RGB(64,128,128)
#define TXT_CLR_ERROR		RGB(255,0,0)

/////////////////////////////////////////////////////////////////////////////
// Helper functions

BOOL alxIsLogicOprExpr(LPCSTR szExpr);

/////////////////////////////////////////////////////////////////////////////
// CALXSyntaxEditCtrl window

class ALX_EXT_CLASS CALXSyntaxEditCtrl : public CRichEditCtrl
{
// Construction
public:
	CALXSyntaxEditCtrl();

// Attributes
public:

	BOOL m_bInForcedChange;

	CHARFORMAT	m_cfDefault;

	int m_nPrevLine;

// list function info
	ALXPRS_DEF_FUNCTION* m_alxFunList;
	int m_alxFuncCount;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CALXSyntaxEditCtrl)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	void Init();
	virtual ~CALXSyntaxEditCtrl();

	// Generated message map functions
protected:
	void SetFormatRange(int nStart, int nEnd, COLORREF clr);
	void Colorize(LPCTSTR lpszBuf, int iOffset = 0);
	//{{AFX_MSG(CALXSyntaxEditCtrl)
	afx_msg void OnChange();
	//}}AFX_MSG
	afx_msg void OnProtected(NMHDR*, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_SYNTAXEDITCTRL_H__INCLUDED_)
