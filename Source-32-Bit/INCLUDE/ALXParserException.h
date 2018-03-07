/////////////////////////////////////////////////////////////////////////////
/*
 Copyright© 2002, Alexey Dolgachov
 mail:	alxsoft@gazinter.net	
 web:	http://www.alxsoft.narod.ru
*/
/////////////////////////////////////////////////////////////////////////////
#if !defined(ALX_PARSEREXEPTION_H__INCLUDED_)
#define ALX_PARSEREXEPTION_H__INCLUDED_

#include "ALXParserDef.h"

////////////////////////////////////////////////////////////////////////
// Info structures

//The information on a error
struct CALXParserErrorInfo
{
// Attributes
	UINT	m_nErrorCode;		// error code
	CString	m_strDescription;	// description

#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
};

// List errors item
struct CALXParserErrorInfoItem
{
	CALXParserErrorInfo	m_stErrorInfo;				// structure describing a variable
	CALXParserErrorInfoItem* m_pstPrevErrorItem;	// the pointer on the previous item of the list
};

////////////////////////////////////////////////////////////////////////
// CALXParserException - Parser error trapping mechanism
class  ALX_EXT_CLASS CALXParserException : public CException
{
	DECLARE_DYNAMIC(CALXParserException)

// Constructors
public:
	CALXParserException(CALXParserErrorInfoItem* pErrorInfoItem);

// Attributes
public:
	CALXParserErrorInfo*	m_pErrorInfo;

private:	
	CALXParserErrorInfoItem* m_pErrorInfoItem;
	int	m_nErrorCount;

// Operations
public:
	int GetErrorCount();
	void GetErrorInfo(int nIndex = 0);

// Implementation
public:
	virtual ~CALXParserException();
	virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError,
		PUINT pnHelpContext = NULL);

protected:
	void FillErrorInfo(int nIndex);
};

void AFXAPI ALXThrowFunOperException(UINT nIDS, ...);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_PARSEREXEPTION_H__INCLUDED_)

