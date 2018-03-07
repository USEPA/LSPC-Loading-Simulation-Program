// DBFCursorset.h : interface of the CDBFCursorset class
//
/////////////////////////////////////////////////////////////////////////////
/*
Автор:              Долгачев А.В.
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_DBFCURSORSET_H__INCLUDED_)
#define ALX_DBFCURSORSET_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DBFRecordset.h"

////////////////////////////////////////////////////////////////////////
// CDBFCursorset - the result of a query or base table browse

class ALX_EXT_CLASS CDBFCursorset : public CDBFRecordset
{
	DECLARE_DYNAMIC(CDBFCursorset)

// Constructor
public:
	CDBFCursorset(CDBFTableDef* pDBFTable = NULL);

	void Open(LPCTSTR lpszFileName, UINT nOpenFlag = ALX_DBF_USE_SHARED);
	void Close();

// Attributes
public:
	BOOL IsOpen() const;	// открыт ли набор
	BOOL IsBOF() const;		// первая ли запись
	BOOL IsEOF() const;		// последняя ли запись

	long GetRecordCount();	// количество записей

protected:
	long m_nCurCursorRecN;	// индекс текущей записи

	BOOL m_bBOF;			
	BOOL m_bEOF;

// Operations
public:
	// Cursor operations
	void MoveNext();		// переход на следующую запись
	void MovePrev();		// переход на предыдущую запись
	void MoveFirst();		// переход на первую запись
	void MoveLast();		// переход на последнюю запись
	void Move(long lOffsetRec); // переход на запись по смещению

	// Поиск
	BOOL FindFirst(LPCTSTR lpszFilter);	// поиск первой записи
	BOOL FindNext(LPCTSTR lpszFilter);	// поиск следующей записи
	BOOL FindNext();					// поиск следующей записи
	BOOL FindLast(LPCTSTR lpszFilter);	// поиск последней записи
	BOOL FindPrev(LPCTSTR lpszFilter);	// поиск предыдущей записи
	BOOL FindPrev();					// поиск предыдущей записи

	long GetAbsolutePosition();					// абсолютная позиция тек. записи
	void SetAbsolutePosition(long lPosition);	// переход на обсолютную позицию в наборе

	// edit buffer operations
	virtual void AddNew();		// добавить запись
	virtual void Edit();		// изменить запись

	virtual void Update();		// сохранить изменения
	virtual void CancelUpdate();// откат

	virtual void Remove();		// удалить запись из набора

	// Recordset operations
	void Requery();				// обновить набор записей

	// определяем показывать или нет помеченные на удаление записи
	void SetDelete(BOOL bHideDeleteRec = TRUE);
	BOOL GetDelete();
	// передаем указатель на функцию фильтра
	void SetFilter(BOOL (FAR * lpfnIsInclude) (CDBFRecordset* pSet) );
	void SetFilter(LPCTSTR lpszFilter);
	// сортировка
	void SortByField(int nIndex, BOOL bDesc = FALSE);
	void SortByField(LPCTSTR lpszName, BOOL bDesc = FALSE);

// Overridables
public:


// Implementation
public:
	virtual ~CDBFCursorset();

	// Out-of-date functions kept for backward compatability

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:
	virtual void ThrowDBFException(long nError = NO_ALX_DBF_ERROR, LPCTSTR szDescription = NULL);

private:
	void SetRecord(long lRecNumber, const DBF_CHAR* lpBuffer);	// изменяем запись

	void qsort (int from, int to);						// алгоритм сортировки записей
	int compare(const long elem1, const long elem2 );	// алгоритм сравнения записей

	CArray<long, long> m_RecPtr;// массив указателей на записи таблицы

	int m_nSortedField;			// индекс поля по которому отсортирована таблица
	BOOL m_bDesc;				// если нужно сортировать по убыванию TRUE

	BOOL m_bShowDeletedRec;		// определяет выводить ли помеченные на удаление записи

	// указатель на функцию фильтра
	BOOL (FAR * m_lpfnIsInclude) (CDBFRecordset* pSet);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_DBFCURSORSET_H__INCLUDED_)
