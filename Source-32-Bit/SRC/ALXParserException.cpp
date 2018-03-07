/////////////////////////////////////////////////////////////////////////////
//
// ALXParserException.cpp : implementation of the ALXParserExeption.h file
//
/////////////////////////////////////////////////////////////////////////////
/*
 Copyright© 2002, Alexey Dolgachov
 mail:	alxsoft@gazinter.net	
 web:	http://www.alxsoft.narod.ru
*/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../INCLUDE/ALXParserException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// Info structure diagnostics

#ifdef _DEBUG
void CALXParserErrorInfo::Dump(CDumpContext& dc) const
{
	dc << "a CALXParserErrorInfo at " << (void*)this;
	dc << "\nm_nErrorCode = " << m_nErrorCode;
	dc << "\nm_strDescription = " << m_strDescription;
	dc << "\n";
}
#endif // _DEBUG

//////////////////////////////////////////////////////////////////////////
// CALXParserException
IMPLEMENT_DYNAMIC(CALXParserException, CException)

CALXParserException::CALXParserException(CALXParserErrorInfoItem* pErrorInfoItem)
{
	m_nErrorCount = 0;

	if(pErrorInfoItem != NULL)
	{
		m_pErrorInfoItem = pErrorInfoItem;
		m_nErrorCount++;
		// find first error
		while(m_pErrorInfoItem->m_pstPrevErrorItem != NULL)
		{
			m_pErrorInfoItem = m_pErrorInfoItem->m_pstPrevErrorItem;
			m_nErrorCount++;
		}

		m_pErrorInfo = &m_pErrorInfoItem->m_stErrorInfo;
	}
	else
		m_pErrorInfo = NULL;

	m_pErrorInfoItem = pErrorInfoItem;
}

CALXParserException::~CALXParserException()
{
	while(m_pErrorInfoItem != NULL)
	{
		CALXParserErrorInfoItem* pDelErrorInfoItem = m_pErrorInfoItem;
		m_pErrorInfoItem = m_pErrorInfoItem->m_pstPrevErrorItem;
		delete pDelErrorInfoItem;
	}

	m_nErrorCount = 0;
	m_pErrorInfo = NULL;
}

// Operations
int CALXParserException::GetErrorCount()
{
	return m_nErrorCount;
}

void CALXParserException::GetErrorInfo(int nIndex /* = 0*/)
{
	FillErrorInfo(nIndex);
}

// Implementation
void CALXParserException::FillErrorInfo(int nIndex)
{
	ASSERT(nIndex >= 0 && nIndex < m_nErrorCount);

	int nCurIndex = m_nErrorCount - 1;
	CALXParserErrorInfoItem* pErrorInfoItem = m_pErrorInfoItem;

	while(nCurIndex != nIndex &&  pErrorInfoItem != NULL)
	{
		pErrorInfoItem = pErrorInfoItem->m_pstPrevErrorItem;
		nCurIndex--;
	}

	if(pErrorInfoItem != NULL)
		m_pErrorInfo = &pErrorInfoItem->m_stErrorInfo;
}

BOOL CALXParserException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError,
	PUINT pnHelpContext)
{
	ASSERT(lpszError != NULL && AfxIsValidString(lpszError, nMaxError));

	if (pnHelpContext != NULL)
		*pnHelpContext = 0;

	if(m_pErrorInfo == NULL)
		return FALSE;

	lstrcpyn(lpszError, m_pErrorInfo->m_strDescription, nMaxError-1);
	lpszError[nMaxError-1] = '\0';

	return TRUE;
}

void AFXAPI ALXThrowFunOperException(UINT nIDS, ...)
{
	CString strError;
	if(strError.LoadString(nIDS))
	{
		va_list argList;
		va_start(argList, nIDS);
		strError.FormatV(CString(strError), argList);
		va_end(argList);
	}
	else
	{
		strError.Format("00000000%X",nIDS);
		strError = "Error number 0x" + strError.Right(8) + ".";
	}

	TRACE(strError+"\n");

	CALXParserErrorInfoItem* pErrorItem = new CALXParserErrorInfoItem; 

	pErrorItem->m_stErrorInfo.m_nErrorCode = nIDS;
	pErrorItem->m_stErrorInfo.m_strDescription = strError;
	pErrorItem->m_pstPrevErrorItem = NULL;

	CALXParserException* pException = new CALXParserException(pErrorItem);

	THROW(pException);
}
