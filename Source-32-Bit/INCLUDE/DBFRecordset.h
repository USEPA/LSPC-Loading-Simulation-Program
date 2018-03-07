// DBFRecordset.h : interface of the CDBFRecordset class
//
/////////////////////////////////////////////////////////////////////////////
/*
Автор:              Долгачев А.В.
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_DBFRECORDSET_H__INCLUDED_)
#define ALX_DBFRECORDSET_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBFTableDef.h"
#include "ALXParser.h"

// режим блокировки
typedef enum 
{
	LockPessimistic	= 1,
	LockOptimistic	= 2
}LockModeEnumDBF;
// тип (алгоритм) блокировки
typedef enum 
{
	NoLock		= 0,
	LockDBase	= 1,
	LockClipper	= 2,
	LockFoxpro	= 3
}LockTypeEnumDBF;
// режим редактирования
typedef enum 
{
	noMode,
	edit,
	addnew
}EditModeEnumDBF;

// Функция конвертирует текст OEM -> ANSI или ANSI -> OEM
void FAR alxOemToAnsiConvert(CString& strConvertText, TextConvertDirection enDirection);
// Функция конвертирует текст ANSI -> OEM или OEM -> ANSI
void FAR alxAnsiToOemConvert(CString& strConvertText, TextConvertDirection enDirection);

// Функция - фильтр
BOOL FAR alxFilter(CDBFRecordset* pSet);

// заполнение массива значениями полей
void FAR alxFetchEmptyParserVariable(CDBFFields& DBFFields, CALXParser* pParser);
void FAR alxFetchParserVariable(CDBFRecordset* pSet, CALXParser* pParser);

////////////////////////////////////////////////////////////////////////
// CDBFRecordset - the result of a query or base table browse

class ALX_EXT_CLASS CDBFRecordset : public CObject
{
	DECLARE_DYNAMIC(CDBFRecordset)

friend FAR alxFilter(CDBFRecordset* pSet);

// Constructor
public:
	CDBFRecordset(CDBFTableDef* pDBFTable = NULL);

	void Open(LPCTSTR lpszFileName, UINT nOpenFlag = ALX_DBF_USE_SHARED);
	void Close();

// Attributes
public:

	BOOL IsOpen() const;	// открыт ли набор записей
	BOOL IsBOF() const;		// если тек. является запись перед первой
	BOOL IsEOF() const;		// если тек. является запись после последней
	BOOL IsDeleted() const;	// если была помечена на удаление

	CString GetName();			// возвращает имя файла
	CString GetDBFilePath();	// возвращает полный путь к DBF таблице

	CTime GetDateCreated();		// возвращает дату создания таблицы
	CTime GetDateLastUpdated();	// возвращает дату последней модификации

	long GetRecordCount();		// кол-во записей в таблице

// Operations
public:
	// create/modify table operations
	BOOL CreateTable(LPCTSTR lpszFileName);	// создаем таблицу
	void ModifyTable();						// модифицируем
	void UpdateTable();						// сохраняем изменения
	void CancelUpdateTable();				// откат

	// create/modify field operations
	void AddField(FIELD_REC* pFieldRec);					// добавляем поле
	void InsertField(short nIndex, FIELD_REC* pFieldRec);	// вставляем поле
	void MoveField(short nIndex, short nNewIndex);			// перемещаем поле на позицию...
	void ModifyField(short nIndex, FIELD_REC* pFieldRec);	// изменяем поле
	void DeleteField(short nIndex);							// удаляем поле

	// recordset operations
	void MoveNext();			// переход на следующую запись
	void MovePrev();			// переход на преддыдущую запись
	void MoveFirst();			// переход на первую запись
	void MoveLast();			// переход на последнюю запись
	void Move(long lRecNumber);	// переход на запись по смещению

	// Поиск
	BOOL FindFirst(LPCTSTR lpszFilter);	// поиск первой записи
	BOOL FindNext(LPCTSTR lpszFilter);	// поиск следующей записи
	BOOL FindNext();					// поиск следующей записи
	BOOL FindLast(LPCTSTR lpszFilter);	// поиск последней записи
	BOOL FindPrev(LPCTSTR lpszFilter);	// поиск предыдущей записи
	BOOL FindPrev();					// поиск предыдущей записи

	long GetAbsolutePosition();					// получаем абсолютную позицию тек. записи
	void SetAbsolutePosition(long lOffsetRec);	// делаем тек. записью запись абс. позиция которой задается пар. функции

	// edit buffer operations
	virtual void AddNew();		// добавляем новую запись
	virtual void Edit();		// изменяем текущую запись
	virtual void Update();		// сохраняем изменения
	virtual void CancelUpdate();// откат
	virtual void Delete();		// помечаем запись на удаление
	virtual void Undelete();	// снимаем пометку

	// locking
	BOOL LockRecord();		// блокируем текущую запись
	BOOL UnlockRecord();	// разблокируем текущую запись

	// field operations
	virtual COleVariant GetFieldValue(LPCTSTR lpszName);
	virtual COleVariant GetFieldValue(int nIndex);

	virtual long GetFieldValue(LPCTSTR lpszName, COleVariant& varValue);
	virtual long GetFieldValue(int nIndex, COleVariant& varValue);

	virtual void SetFieldValue(LPCTSTR lpszName, const COleVariant& varValue);
	virtual void SetFieldValue(int nIndex, const COleVariant& varValue);

	// set text convert function
	void SetTextConvertFunction(void (FAR * lpfnTextConvert) (CString& strConvertText, TextConvertDirection enDirection));

	short GetFieldCount();	// кол-во полей

// Overridables
public:
	virtual CString GetDefaultDBName();

	// for recordset field exchange
//	virtual void DoFieldExchange(CDBFFieldExchange* pFX);

// Implementation
public:
	BOOL ChangeOpenFlag(UINT nNewFlag);
	virtual ~CDBFRecordset();


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CDBFTableDef*	m_pTableDef;	// указатель на таблицу
	CDBFFields		m_DBFFields;	// структура полей


protected:
	int m_nOpenType;		// способ открытия таблицы

	BOOL m_bBOF;			
	BOOL m_bEOF;

	long m_nCurTableRecN;	// индекс текущей записи в таблице

	UINT m_nLockMode;		// режим блокировки
	UINT m_nLockType;		// тип (алгоритм) блокировки

	UINT m_nEditMode;		// режим редактирования

	// указатель на функцию конвертации текста
	void (FAR * m_lpfnTextConvert) (CString& strConvertText, TextConvertDirection enDirection);

	CALXParser* m_pFindParser;		// парсер выражение поиска
	CALXParser* m_pFilterParser;	// парсер выражения фильтра
	
	virtual void ThrowDBFException(long nError = NO_ALX_DBF_ERROR, LPCTSTR szDescription = NULL);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_DBFRECORDSET_H__INCLUDED_)
