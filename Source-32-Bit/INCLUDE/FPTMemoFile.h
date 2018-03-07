// FPTMemoFile.h : interface of the CFPTMemoFile class
//
/////////////////////////////////////////////////////////////////////////////
/*
Автор:              Долгачев А.В.
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_FPTMEMOFILE_H__INCLUDED_)
#define ALX_FPTMEMOFILE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MemoFile.h"

////////////////////////////////////////////////////////////////////////
// CFPTMemoFile - a Memo File

class ALX_EXT_CLASS CFPTMemoFile : public CMemoFile
{

	DECLARE_DYNAMIC(CFPTMemoFile)

// Constructors
public:
	CFPTMemoFile();

// Attributes
public:
	FPT_HEAD	m_FPTHead;	// заголовок мемо поля

// Overridables
public:

// Operations
public:

// Implementation
public:
	~CFPTMemoFile();

	virtual void Create(LPCTSTR lpszName);
	virtual void Open(LPCTSTR lpszName, UINT nOpenFlag);
	virtual void Close();

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

	DBF_ULONG	m_nNextBlock;	// номер свободного блока
	DBF_LONG	m_nBlockSize;	// размер блока
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_FPTMEMOFILE_H__INCLUDED_)

