// DBFException.cpp : implementation of the DBFExeptio.h file
//
/////////////////////////////////////////////////////////////////////////////
/*
Автор:              Долгачев А.В.
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../INCLUDE/DBFDef.h"
#include "../INCLUDE/DBFException.h"
#include "../INCLUDE/ALXParserException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// Logging helpers
#ifdef _DEBUG
BOOL ALXAssertFailedLine(LPCSTR lpszDBFCall, LPCSTR lpszFile, int nLine)
{
	if (afxTraceFlags & traceDatabase)
	{
		TRACE0("\nDBF Call Failed.");
		TRACE1("\n\t%hs", lpszDBFCall);
		TRACE2("\n\tIn file %hs on line %d", lpszFile, nLine);
	}
	return AfxAssertFailedLine(lpszFile, nLine);
}
#endif // _DEBUG
//////////////////////////////////////////////////////////////////////////
// Info structure diagnostics

#ifdef _DEBUG
void CDBFErrorInfo::Dump(CDumpContext& dc) const
{
	dc << "a CDaoErrorInfo at " << (void*)this;

	dc << "\nm_lErrorCode = " << m_lErrorCode;
//	dc << "\nm_strSource = " << m_strSource;
	dc << "\nm_strDescription = " << m_strDescription;
//	dc << "\nm_strHelpFile = " << m_strHelpFile;
//	dc << "\nm_lHelpContext = " << m_lHelpContext;

	dc << "\n";
}
#endif // _DEBUG

//////////////////////////////////////////////////////////////////////////
// CDBFException
IMPLEMENT_DYNAMIC(CDBFException, CException)

CDBFException::CDBFException()
{
	m_pErrorInfo = NULL;
}

CDBFException::~CDBFException()
{
	delete m_pErrorInfo;
	m_pErrorInfo = NULL;
}

// Operations
void CDBFException::GetErrorInfo()
{
	FillErrorInfo();
}

// Implementation
void CDBFException::FillErrorInfo()
{
	// Allocate the error info structure if necessary
	if (m_pErrorInfo == NULL)
		m_pErrorInfo = new CDBFErrorInfo;

	m_pErrorInfo->m_lErrorCode = m_nALXDBFError;
}

BOOL CDBFException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError,
	PUINT pnHelpContext)
{
	ASSERT(lpszError != NULL && AfxIsValidString(lpszError, nMaxError));

	if (pnHelpContext != NULL)
		*pnHelpContext = 0;

	GetErrorInfo();

	CString strError;
	if(m_pErrorInfo->m_strDescription.IsEmpty())
		if(!m_pErrorInfo->m_strDescription.LoadString(m_nALXDBFError))
		{
			strError.Format("00000000%X",m_nALXDBFError);
			m_pErrorInfo->m_strDescription = "Error number 0x" + strError.Right(8) + ".";
		}

	lstrcpyn(lpszError, (LPCSTR)m_pErrorInfo->m_strDescription, nMaxError-1);
	lpszError[nMaxError-1] = '\0';
	return TRUE;
}

void AFXAPI ALXThrowDBFException(long nALXDBFError, LPCTSTR szDescription)
{
	CDBFException* pException;
	pException = new CDBFException;

	pException->m_nALXDBFError = nALXDBFError;

	if(szDescription != NULL)
	{
		pException->m_pErrorInfo = new CDBFErrorInfo;
		pException->m_pErrorInfo->m_strDescription = szDescription;
		pException->m_pErrorInfo->m_lErrorCode = nALXDBFError;
	}


	THROW(pException);
}

void AFXAPI ALXThrowDBFException(CALXParserException* e)
{
	// получаем информацию об ошибке
	e->GetErrorInfo();
	UINT nErrorCode = e->m_pErrorInfo->m_nErrorCode;
	CString strError = e->m_pErrorInfo->m_strDescription;
	e->Delete();
	
	ALXThrowDBFException(nErrorCode, strError);
}
