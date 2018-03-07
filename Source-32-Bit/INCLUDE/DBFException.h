/////////////////////////////////////////////////////////////////////////////
/*
Автор:              Долгачев А.В.
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_DBFEXEPTION_H__INCLUDED_)
#define ALX_DBFEXEPTION_H__INCLUDED_


////////////////////////////////////////////////////////////////////////
// Logging helpers

#ifdef _DEBUG
BOOL ALXAssertFailedLine(LPCSTR lpszDBFCall, LPCSTR lpszFile, int nLine);
#define DBF_CHECK_ERROR(f, err) \
	do \
	{ \
	if(!(f)) \
	 if(ALXAssertFailedLine(#f,THIS_FILE, __LINE__)) \
		{ AfxDebugBreak(); }\
	 else ALXThrowDBFException(err);\
	} \
	while (0)

#else
#define DBF_CHECK_ERROR(f, err) \
	if(!(f)) \
		ALXThrowDBFException(err);

#endif // _DEBUG

class  CALXParserException;

////////////////////////////////////////////////////////////////////////
// Info structures

struct CDBFErrorInfo
{
// Attributes
	long	m_lErrorCode;		// код ошибки
	CString	m_strDescription;	// сообщение

#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
};

////////////////////////////////////////////////////////////////////////
// CDBFException - DBF error trapping mechanism
class ALX_EXT_CLASS CDBFException : public CException
{
	DECLARE_DYNAMIC(CDBFException)

// Constructors
public:
	CDBFException();

// Attributes
public:
	CDBFErrorInfo* m_pErrorInfo;

	long m_nALXDBFError;     // DBF class extended error code

// Operations
public:
	void GetErrorInfo(/*int nIndex*/);

// Implementation
public:
	virtual ~CDBFException();
	virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError,
		PUINT pnHelpContext = NULL);

protected:
	void FillErrorInfo();
};

void AFXAPI ALXThrowDBFException(long nALXDBFError = NO_ALX_DBF_ERROR, LPCTSTR szDescription = NULL);
void AFXAPI ALXThrowDBFException(CALXParserException* e);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_DBFEXEPTION_H__INCLUDED_)

