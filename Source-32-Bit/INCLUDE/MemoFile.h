// MemoFile.h : interface of the CMemoFile class
//
/////////////////////////////////////////////////////////////////////////////
/*
Автор:              Долгачев А.В.
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_MEMOFILE_H__INCLUDED_)
#define ALX_MEMOFILE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBFDef.h"
#include "DBFException.h"


////////////////////////////////////////////////////////////////////////
// CMemoFile - a Memo File

class ALX_EXT_CLASS CMemoFile : public CFile
{

friend class CFPTMemoFile;
friend class CDBTMemoFile;
friend class CSMTMemoFile;

	DECLARE_DYNAMIC(CMemoFile)

// Constructors
public:
	CMemoFile();

// Attributes
public:
	DBT_HEAD*	m_pDBTHead;	// заголовок мемо поля

// Overridables
public:

// Operations
public:

// Implementation
public:
	~CMemoFile();

	virtual void Create(LPCTSTR lpszName);
	virtual void Open(LPCTSTR lpszName, UINT nOpenFlag);
	virtual void Close();
	// открыт ли файл
	virtual BOOL IsOpen() const;
	
	// тип файла
	virtual UINT GetMemoFileType();

	// читаем мемо поле
	virtual void ReadMemoRecord(DBF_ULONG lOffsetRec, DBF_LONG& nLen, CString& strMemo); 
	virtual void ReadMemoRecord(DBF_ULONG lOffsetRec, DBF_LONG& nLen, unsigned char** pMemoData);
	// записываем мемо поле
	virtual void WriteMemoRecord(DBF_ULONG& lOffsetRec, DBF_LONG& nLen, CString& strMemo);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:
	BOOL ReadHeader();	// читаем заголовок
	BOOL WriteHeader();	// записываем заголовок

private:
	// добавляем мемо поле
	DBF_LONG AddMemoRecord(CString& strMemo);
	// обновляем мемо поле
	void UpdateMemoRecord(DBF_ULONG lOffsetRec, CString& strMemo);

	BOOL m_bOpen;				// открыт ли файл

	DBF_ULONG	m_nNextBlock;	// номер свободного блока
	DBF_LONG	m_nBlockSize;	// размер блока
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_MEMOFILE_H__INCLUDED_)

