// DBFTableDef.h : interface of the CDBFTableDef class
//
/////////////////////////////////////////////////////////////////////////////
/*
Автор:              Долгачев А.В.
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_DBFTABLEDEF_H__INCLUDED_)
#define ALX_DBFTABLEDEF_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBFDef.h"
#include "DBFFields.h"
#include "DBTMemoFile.h"
#include "FPTMemoFile.h"
#include "SMTMemoFile.h"
#include "DBFException.h"

// режим конвертирования текста
enum TextConvertDirection
{
	GetText,
	SetText
};

// функция меняет расширение в имени файла
CString alxChangeFileExt(CString& strFileName, CString strNewFileExt);

// функция создает и возвращает указатель на класс мемо файла
CMemoFile* alxNewMemoFile(UINT nMemoType);

// функция определяет расширение для имени мемо файла
LPCTSTR alxMemoFileExt(UINT nMemoType);

// функция записывает значение поля в буфер записи
void alxSetFieldValue(DBF_CHAR* pRec, FIELD_REC* pFieldRec, const COleVariant& varValue,
					  void (FAR * lpfnTextConvert) (CString& strConvertText, TextConvertDirection enDirection));
// функция возвращает значение поля из буфера записи
long alxGetFieldValue(DBF_CHAR* pRec, FIELD_REC* pFieldRec, COleVariant& varValue,
					  void (FAR * lpfnTextConvert) (CString& strConvertText, TextConvertDirection enDirection));

////////////////////////////////////////////////////////////////////////
// CDBFTableDef - a DBF TableDef

class ALX_EXT_CLASS CDBFTableDef : public CObject
{

friend class CDBFRecordset;
friend class CDBFCursorset;
friend class CDBFMemCursorset;

	DECLARE_DYNAMIC(CDBFTableDef)

// Constructors
public:
	CDBFTableDef();

	void Open(LPCTSTR lpszName, UINT nOpenFlag = ALX_DBF_USE_EXCLUSIVE);
	void Close();

// Attributes
public:
	DBF_HEAD	m_DBFHead;	// заголовок таблицы

	BOOL IsOpen() const;	// открыт ли набор записей

	CString GetName();		// возвращает имя набора
	CString GetDBFilePath();// возвращает полный путь к DBF таблице
	CString GetMemoName();	// возвращает имя Мемо файла
	CString GetMemoFilePath();// возвращает полный путь к Мемо файлу

	CTime GetDateCreated();		// дата создания таблицы
	CTime GetDateLastUpdated();	// дата последней модификации

	long GetRecordCount();	// кол-во записей в таблице

// Overridables
public:

// Operations
public:
	BOOL CreateTable(LPCTSTR lpszName);	// создаем таблицу
	void ModifyTable();					// модифицируем
	void UpdateTable();					// сохраняем изменения
	void CancelUpdateTable();			// откат

	void AddField(FIELD_REC* pFieldRec);					// добавляем поле
	void InsertField(short nIndex, FIELD_REC* pFieldRec);	// вставляем поле
	void MoveField(short nIndex, short nNewIndex);			// перемещаем поле на позицию...
	void ModifyField(short nIndex, FIELD_REC* pFieldRec);	// изменяем поле
	void DeleteField(short nIndex);							// удаляем поле

	short GetFieldCount();	// кол-во полей
	void WriteHeader(); // записываем заголовок
// Implementation
public:
	~CDBFTableDef();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CDBFFields m_DBFFields; // структура полей

protected:
	BOOL ChangeOpenFlag(UINT nNewFlag);
	BOOL m_bOpen;				// открыта ли таблица
	BOOL m_bNew;				// создана ли таблица
	UINT m_nOpenFlag;			// способ открытия файла

	CFile m_DBFile;				// DBF таблица
	CMemoFile* m_pMemoFile;		// Файл с мемо полями

	enum EditMode
	{
		noMode,
		modify,
		create
	};
	UINT m_nEditMode;

	BOOL ReadHeader();	// читаем заголовок
	//void WriteHeader(); // записываем заголовок

	void ReadRecord(long lRecNumber, void* lpBuffer);	// читаем запись
	void WriteRecord(long lRecNumber, void* lpBuffer);	// записываем запись

	// читаем мемо поле
	void ReadMemoRecord(DBF_ULONG lOffsetRec, DBF_LONG& nLen, CString& strMemo); 
	void ReadMemoRecord(DBF_ULONG lOffsetRec, DBF_LONG& nLen, unsigned char** pMemoData);
	// записываем мемо поле
	void WriteMemoRecord(DBF_ULONG& lOffsetRec, DBF_LONG& nLen, CString& strMemo);

	// возвращает значение поля
	long GetFieldValue(DBF_CHAR* pRec, FIELD_REC* pFieldRec, COleVariant& varValue,
					  void (FAR * lpfnTextConvert) (CString& strConvertText, TextConvertDirection enDirection));
	// функция сохраняет значение поля
	void SetFieldValue(DBF_CHAR* pRec, FIELD_REC* pFieldRec, const COleVariant& varValue,
					  void (FAR * lpfnTextConvert) (CString& strConvertText, TextConvertDirection enDirection));
private:
	// указатели используемые при модификации таблицы
	DBF_HEAD*	m_pOldDBFHead;		// указатель на старую структуру таблицы
	CDBFFields* m_pOldDBFFields;	// указатель структуру полей
	DBF_INT*	m_pSwapFld;			// массив описывающий соответствие старых и новых полей

	DBF_UCHAR*	m_pData;	// буфер для хранения двоичных данных (поля Picture, Binary, General)
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_DBFTABLEDEF_H__INCLUDED_)

