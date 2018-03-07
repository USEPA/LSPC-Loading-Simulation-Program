// RecordsetEx.cpp : implementation file
//

#include "stdafx.h"
#include "RecordsetEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecordsetEx

IMPLEMENT_DYNAMIC(CRecordsetEx, CRecordset)

CRecordsetEx::CRecordsetEx(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRecordsetEx)
	//}}AFX_FIELD_INIT
	
	m_nDefaultType = snapshot;
}

BOOL CRecordsetEx::ExecDirect(const CString& sSQL)
{
	return Open(CRecordset::forwardOnly, sSQL, CRecordset::executeDirect);
}

BOOL CRecordsetEx::Open(UINT nOpenType, LPCTSTR lpszSQL, DWORD dwOptions)
{
	ASSERT(!IsOpen());
	ASSERT_VALID(this);
	ASSERT(lpszSQL == NULL || AfxIsValidString(lpszSQL));
	ASSERT(nOpenType == AFX_DB_USE_DEFAULT_TYPE ||
		nOpenType == dynaset || nOpenType == snapshot ||
		nOpenType == forwardOnly || nOpenType == dynamic);
	ASSERT(!(dwOptions & readOnly && dwOptions & appendOnly));

	// Can only use optimizeBulkAdd with appendOnly recordsets
	ASSERT((dwOptions & optimizeBulkAdd && dwOptions & appendOnly) ||
		!(dwOptions & optimizeBulkAdd));

	// forwardOnly recordsets have limited functionality
	ASSERT(!(nOpenType == forwardOnly && dwOptions & skipDeletedRecords));

	// Cache state info and allocate hstmt
	SetState(nOpenType, lpszSQL, dwOptions);
	if(!AllocHstmt())
		return FALSE;

	// Check if bookmarks upported (CanBookmark depends on open DB)
	ASSERT(dwOptions & useBookmarks ? CanBookmark() : TRUE);

	TRY
	{
		OnSetOptions(m_hstmt);

		// Allocate the field/param status arrays, if necessary
		BOOL bUnbound = FALSE;
		if (m_nFields > 0 || m_nParams > 0)
			AllocStatusArrays();
		else
			bUnbound = TRUE;

		// Build SQL and prep/execute or just execute direct
		m_strSQL = lpszSQL;
		PrepareAndExecute();
		
		// Cache some field info and prepare the rowset
		AllocAndCacheFieldInfo();
		AllocRowset();

		// If late binding, still need to allocate status arrays
		if (bUnbound && (m_nFields > 0 || m_nParams > 0))
			AllocStatusArrays();

		// Give derived classes a call before binding
		PreBindFields();

		if(m_nResultCols)
			MoveNext();

		// If EOF, then result set empty, so set BOF as well
		m_bBOF = m_bEOF;
	}
	CATCH_ALL(e)
	{
		Close();
		THROW_LAST();
	}
	END_CATCH_ALL

	return TRUE;
}

void CRecordsetEx::AllocAndCacheFieldInfo()
{
	m_nResultCols = -1;
	ASSERT(GetODBCFieldCount() < 0);
	
	if(m_rgODBCFieldInfos != NULL)
	{
		delete [] m_rgODBCFieldInfos;
		m_rgODBCFieldInfos = NULL;
	}
	
	RETCODE nRetCode;
	SWORD nActualLen;	   // Cache the number of result columns.
	AFX_ODBC_CALL(::SQLNumResultCols(m_hstmt, &m_nResultCols));
	if(!Check(nRetCode))
	{
		TRACE(_T("Error: Can't get field info.\n"));
		AfxThrowDBException(nRetCode, m_pDatabase,m_hstmt);
	}
	
	// If there are no fields, quit now
	if(m_nResultCols <= 0)
		 return;
	  
	// Allocate buffer and get the ODBC meta data.
	m_rgODBCFieldInfos = new CODBCFieldInfo[m_nResultCols];
	
	LPSTR lpszFieldName;

#ifdef _UNICODE
	  // Need proxy to temporarily store non-UNICODE name.
	  lpszFieldName = new char[MAX_FNAME_LEN + 1];
#endif

      // Get the field info for each field.
   for(WORD n = 1; n <= GetODBCFieldCount(); n++)
   {
#ifndef _UNICODE 	// Reset the buffer to point to next element.
		lpszFieldName = m_rgODBCFieldInfos[n - 1].m_strName.GetBuffer(MAX_FNAME_LEN + 1);
#endif

		AFX_ODBC_CALL(::SQLDescribeCol(m_hstmt, n,
			 (UCHAR*)lpszFieldName, MAX_FNAME_LEN, &nActualLen,
			 &m_rgODBCFieldInfos[n - 1].m_nSQLType,
			 &m_rgODBCFieldInfos[n - 1].m_nPrecision,
			 &m_rgODBCFieldInfos[n - 1].m_nScale,
			 &m_rgODBCFieldInfos[n - 1].m_nNullability));
   
#ifndef _UNICODE
		m_rgODBCFieldInfos[n - 1].m_strName.ReleaseBuffer(nActualLen);
#else		 // Copy the proxy data to correct element.
		m_rgODBCFieldInfos[n - 1].m_strName = lpszFieldName;
#endif
		
		if(!Check(nRetCode))
		{
			TRACE1("Error: ODBC failure getting field #%d info.\n", n);
				AfxThrowDBException(nRetCode, m_pDatabase, m_hstmt);
		}
  }

#ifdef _UNICODE
	delete[] lpszFieldName;
#endif
}

void CRecordsetEx::GetFieldValue(short nIndex, CString& strValue)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	// No data or no column info fetched yet
	if (GetODBCFieldCount() <= 0)
	{
		ASSERT(FALSE);
		return;
	}

	// Convert index to 1-based and check range
	nIndex++;
	if (nIndex < 1 || nIndex > GetODBCFieldCount())
	{
		ThrowDBException(AFX_SQL_ERROR_FIELD_NOT_FOUND);
	}

	int nLen = GetTextLen(m_rgODBCFieldInfos[nIndex - 1].m_nSQLType,
			m_rgODBCFieldInfos[nIndex - 1].m_nPrecision);
	if(nLen > 255)
		nLen = 255;
#ifndef _UNICODE
		CString& strData = strValue;
#else
		CString strProxy;
		CString& strData = strProxy;
#endif
	void* pvData = strData.GetBufferSetLength(nLen);

	// Now can actually get the data
	long nActualSize = GetData(m_pDatabase, m_hstmt, nIndex,
		SQL_C_CHAR, pvData, nLen,
		m_rgODBCFieldInfos[nIndex - 1].m_nSQLType);

	// Handle NULL data separately
	if (nActualSize == SQL_NULL_DATA)
	{
		// Clear value
		strValue.Empty();
	}
	else
	{
		// May need to cleanup and call SQLGetData again if necessary
		GetLongCharDataAndCleanup(m_pDatabase, m_hstmt, nIndex,
			nActualSize, &pvData, nLen, strData,
			m_rgODBCFieldInfos[nIndex - 1].m_nSQLType);

#ifdef _UNICODE
	// Now must convert string to UNICODE
	strValue = (LPCSTR)strData.GetBuffer(0);
#endif // _UNIOCDE
	}

	if(m_rgODBCFieldInfos[nIndex - 1].m_nSQLType == SQL_BINARY ||
		m_rgODBCFieldInfos[nIndex - 1].m_nSQLType == SQL_LONGVARBINARY ||
		m_rgODBCFieldInfos[nIndex - 1].m_nSQLType == SQL_VARBINARY)
		if(!strValue.IsEmpty())
			strValue = _T("0x") + strValue;
}

int PASCAL CRecordsetEx::GetTextLen(short nSQLType, UDWORD nPrecision)
{
	int nLen;

	if (nSQLType == SQL_LONGVARCHAR || nSQLType == SQL_LONGVARBINARY ||
		nSQLType == SQL_WLONGVARCHAR || nSQLType == SQL_UNICODE_LONGVARCHAR)
	{
		// Use a dummy length of 1 (will just get NULL terminator)
		nLen = 1;
	}
	else
	{
		// better know the length
		ASSERT(nPrecision >= 0);

		nLen = nPrecision + 1;

		// If converting Numeric or Decimal to text need
		// room for decimal point and sign in string
		if (nSQLType == SQL_NUMERIC || nSQLType == SQL_DECIMAL)
			nLen += 2;
	}

	return nLen;
}

void PASCAL CRecordsetEx::GetLongCharDataAndCleanup(CDatabase* pdb,
	HSTMT hstmt, short nFieldIndex, long nActualSize, LPVOID* ppvData,
	int nLen, CString& strValue, short nSQLType)
{
	RETCODE nRetCode;

	// Release the buffer now that data has been fetched
	strValue.ReleaseBuffer(nActualSize < nLen ? nActualSize : nLen);

	// If long data, may need to call SQLGetData again
	if (nLen <= nActualSize && (nSQLType == SQL_LONGVARCHAR ||
		nSQLType == SQL_LONGVARBINARY || nSQLType == SQL_WLONGVARCHAR ||
		nSQLType == SQL_UNICODE_LONGVARCHAR))
	{
		// Reallocate the size (this will copy the data)
		*ppvData = strValue.GetBufferSetLength(nActualSize + 1);

		// Get pointer, skipping over original data, but not the NULL
		int nOldLen = nLen - 1;
		*ppvData = (BYTE*)*ppvData + nOldLen;
		nLen = nActualSize + 1 - nOldLen;

		// Retrieve the column in question
		AFX_ODBC_CALL(::SQLGetData(hstmt, nFieldIndex,
			SQL_C_CHAR, *ppvData, nLen, &nActualSize));
		if (nRetCode == SQL_SUCCESS_WITH_INFO)
		{
#ifdef _DEBUG
			if (afxTraceFlags & traceDatabase)
			{
				TRACE1("Warning: ODBC Success With Info on field %d.\n",
					nFieldIndex - 1);
				CDBException e(nRetCode);
				e.BuildErrorString(pdb, hstmt);
			}
#endif // _DEBUG
		}
		else if (nRetCode != SQL_SUCCESS)
		{
			TRACE1("Error: GetFieldValue operation failed on field %d.\n",
				nFieldIndex - 1);
			AfxThrowDBException(nRetCode, pdb, hstmt);
		}

		// Release the buffer now that data has been fetched
		strValue.ReleaseBuffer(nActualSize + nOldLen);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRecordsetEx diagnostics

#ifdef _DEBUG
void CRecordsetEx::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRecordsetEx::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG