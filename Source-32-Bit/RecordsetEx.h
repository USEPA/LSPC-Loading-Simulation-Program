#if !defined(AFX_RECORDSETEX_H__43D82455_75A6_11D2_BD7C_204C4F4F5020__INCLUDED_)
#define AFX_RECORDSETEX_H__43D82455_75A6_11D2_BD7C_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecordsetEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRecordsetEx recordset

class CRecordsetEx : public CRecordset
{
public:
	CRecordsetEx(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRecordsetEx)

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordsetEx)
	public:
	virtual BOOL Open(UINT nOpenType = snapshot, LPCTSTR lpszSql = NULL, DWORD dwOptions = none);
	//}}AFX_VIRTUAL

	void AllocAndCacheFieldInfo();
	BOOL ExecDirect(const CString& sSQL);
	void GetFieldValue(short nIndex, CString& strValue);
	
// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
private:
	static int PASCAL GetTextLen(short nSQLType, UDWORD nPrecision);
	static void PASCAL GetLongCharDataAndCleanup(CDatabase* pdb, HSTMT hstmt,
		short nFieldIndex, long nActualSize, LPVOID* ppvData, int nLen,
		CString& strValue, short nSQLType);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDSETEX_H__43D82455_75A6_11D2_BD7C_204C4F4F5020__INCLUDED_)