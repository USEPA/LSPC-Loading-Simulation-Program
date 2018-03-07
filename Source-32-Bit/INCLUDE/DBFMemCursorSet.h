// DBFMemCursorset.h : interface of the CDBFMemCursorset class
//
/////////////////////////////////////////////////////////////////////////////
/*
Автор:              Долгачев А.В.
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_DBFMEMCURSORSET_H__INCLUDED_)
#define ALX_DBFMEMCURSORSET_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBFRecordset.h"

////////////////////////////////////////////////////////////////////////
// CDBFMemCursorset - the result of a query or base table browse

class ALX_EXT_CLASS CDBFMemCursorset : public CDBFRecordset
{
	DECLARE_DYNAMIC(CDBFMemCursorset)

// Constructor
public:
	CDBFMemCursorset(CDBFTableDef* pDBFTable = NULL);

	void Open(LPCTSTR lpszFileName, UINT nOpenFlag = ALX_DBF_USE_SHARED);
	void Close();

// Attributes
public:
	BOOL IsOpen() const;	// открыт ли набор
	BOOL IsBOF() const;		// первая ли запись
	BOOL IsEOF() const;		// последняя ли запись

	BOOL IsDeleted();		// если была помечена на удаление

	long GetRecordCount();	// количество записей

protected:
	DBF_HEAD	m_DBFHead;	// заголовок

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

	virtual void Delete();		// пометить на удаление
	virtual void Undelete();	// снять пометку на удаление
	virtual void Remove();		// удалить запись из набора

	// Recordset operations
	void Requery();				// обновить набор записей
	BOOL Save();				// записать изменения в файл
	// экспорт

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
	virtual ~CDBFMemCursorset();

	// Out-of-date functions kept for backward compatability

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:
	virtual void ThrowDBFException(long nError = NO_ALX_DBF_ERROR, LPCTSTR szDescription = NULL);

	// Функция - фильтр по всем полям записи
	BOOL FAR fnFilter(DBF_CHAR* pRec, CDBFFields* pDBFFields);
private:

	DBF_CHAR* GetRecord(long lRecNumber);						// получаем запись 
	void GetRecord(long lRecNumber, DBF_CHAR* lpBuffer);		// получаем запись
	void SetRecord(long lRecNumber, const DBF_CHAR* lpBuffer);	// изменяем запись

	void qsort (int from, int to);						// алгоритм сортировки записей
	int compare(const void *elem1, const void *elem2 );	// алгоритм сравнения записей

	DBF_UCHAR*	m_pTableData;	// указатель на область памяти где хранится DBF таблица
	CPtrArray	m_RecPtr;		// массив указателей на записи таблицы
	CPtrArray	m_NewRecPtr;	// массив указателей на новые записи добавленные в таблицу

	int m_nSortedField;			// индекс поля по которому отсортирована таблица
	BOOL m_bDesc;				// если нужно сортировать по убыванию TRUE

	BOOL m_bShowDeletedRec;		// определяет выводить ли помеченные на удаление записи

	DBF_CHAR*	m_pEditRec;		// указатель на буфер редактируемой записи
	DBF_LONG	m_lLoadRecord;	// кол-во записей загруженных из DBF таблицы

	// указатель на функцию фильтра
	BOOL (FAR * m_lpfnIsInclude) (CDBFRecordset* pSet);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_DBFMEMCURSORSET_H__INCLUDED_)
