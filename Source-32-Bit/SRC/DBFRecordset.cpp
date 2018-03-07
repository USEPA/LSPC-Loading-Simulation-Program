// DBFTableDef.cpp : implementation of the CDBFTableDef class
//
/////////////////////////////////////////////////////////////////////////////
/*
Автор:              Долгачев А.В.
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../INCLUDE/DBFRecordset.h"
#include "../INCLUDE/DBFException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* Функция конвертирует текст OEM -> ANSI или ANSI -> OEM */
void FAR alxOemToAnsiConvert(CString& strConvertText, TextConvertDirection enDirection)
{
	if(enDirection == GetText)
		strConvertText.OemToAnsi();
	else
		strConvertText.AnsiToOem();
}

/* Функция конвертирует текст ANSI -> OEM или OEM -> ANSI */
void FAR alxAnsiToOemConvert(CString& strConvertText, TextConvertDirection enDirection)
{
	if(enDirection == GetText)
		strConvertText.AnsiToOem();
	else
		strConvertText.OemToAnsi();
}

/* Функция - фильтр. */
BOOL FAR alxFilter(CDBFRecordset* pSet)
{
	// получаем значения полей используемых в выражении
	alxFetchParserVariable(pSet, pSet->m_pFilterParser);

	// вычисляем результат
	VARIANT* pvarExprResult = pSet->m_pFilterParser->Execute();

	// если не логическое выражение
	if(V_VT(pvarExprResult) != VT_BOOL)
		ALXThrowDBFException(ALX_DBF_ERROR_NOT_LOGIC_EXPR);

	return V_BOOL(pvarExprResult) != FALSE;
} 

// заполнение массива значениями полей
void FAR alxFetchEmptyParserVariable(CDBFFields& DBFFields, CALXParser* pParser)
{
	for(short i = 0; i < DBFFields.GetFieldCount(); i++)
	{
		COleVariant varFldValue;

		switch(DBFFields[i].m_chType)
		{
			case FLD_TYPE_CHARACTER:
			case FLD_TYPE_MEMO:
				varFldValue.ChangeType(VT_BSTR);
				pParser->AddVariable(DBFFields[i].m_strName, varFldValue, i);
				break;
			case FLD_TYPE_DOUBLE: // возможно FLD_TYPE_BINARY
				if(DBFFields[i].m_nLen != sizeof(DBF_DOUBLE))
					break;
			case FLD_TYPE_NUMERIC:
			case FLD_TYPE_FLOAT:
			case FLD_TYPE_CURRENCY:
			case FLD_TYPE_INTEGER:
				varFldValue.ChangeType(VT_R8);
				pParser->AddVariable(DBFFields[i].m_strName, varFldValue, i);
				break;
			case FLD_TYPE_DATE:
			case FLD_TYPE_DATETIME:
				varFldValue.ChangeType(VT_DATE);
				pParser->AddVariable(DBFFields[i].m_strName, varFldValue, i);
				break;
			case FLD_TYPE_LOGICAL:
				varFldValue.ChangeType(VT_BOOL);
				pParser->AddVariable(DBFFields[i].m_strName, varFldValue, i);
				break;
		}
	}
}

// заполнение массива значениями полей
void FAR alxFetchParserVariable(CDBFRecordset* pSet, CALXParser* pParser)
{
	LPCSTR szName = pParser->GetFirstVariableName();
	// перебираем поля используемые в выражении
	while(szName != NULL)
	{
		COleVariant varFldValue;
		// получаем индекс поля
		DWORD dwInd = pParser->GetVariableKey();
		// получаем значение поля
		pSet->GetFieldValue(dwInd, varFldValue);
		// передаем значения полей
		pParser->SetVariableValue(pSet->m_DBFFields[dwInd].m_strName, varFldValue);

		// перехади к следующему полю
		szName = pParser->GetNextVariableName();
	}
}
//////////////////////////////////////////////////////////////////////////
// CDBFRecordset
IMPLEMENT_DYNAMIC(CDBFRecordset, CObject )

/* Конструктор */
CDBFRecordset::CDBFRecordset(CDBFTableDef* pDBFTable)
{
	m_pTableDef = NULL;

	m_lpfnTextConvert = NULL;

	m_pFindParser = NULL;
	m_pFilterParser = NULL;

	m_nCurTableRecN = 0;
	m_bBOF = TRUE;			
	m_bEOF = TRUE;

	m_nLockMode = LockPessimistic;
	m_nLockType = NoLock;

	m_nEditMode = edit;
}

/* Деструктор */
CDBFRecordset::~CDBFRecordset()
{
	if (IsOpen())
		Close();
}

/* Создаем новую таблицу */
BOOL CDBFRecordset::CreateTable(LPCTSTR lpszFileName)
{
	if(IsOpen())
		Close();

	CString strFileName;

	if(lpszFileName == NULL)
		strFileName = GetDefaultDBName();
	else
		strFileName = lpszFileName;

	ASSERT(strFileName.GetLength() > 4);

	m_pTableDef = new CDBFTableDef;

	// таблица в режиме создания
	if(m_pTableDef->CreateTable(strFileName))
		return FALSE;

	// получаем структуру полей
	m_DBFFields = m_pTableDef->m_DBFFields;

	return TRUE;
}

/* Модификация таблицы */
void CDBFRecordset::ModifyTable()
{
	// таблица в режиме модификации
	m_pTableDef->ModifyTable();

	return;
}

/* Добавляем поле в таблицу */
void CDBFRecordset::AddField(FIELD_REC *pFieldRec)
{
	m_pTableDef->AddField(pFieldRec);

	// получаем структуру полей
	m_DBFFields = m_pTableDef->m_DBFFields;
}

/* Вставляем поле в таблицу */
void CDBFRecordset::InsertField(short nIndex, FIELD_REC *pFieldRec)
{
	m_pTableDef->InsertField(nIndex, pFieldRec);

	// получаем структуру полей
	m_DBFFields = m_pTableDef->m_DBFFields;
}

/* Удаляем поле из таблицы */
void CDBFRecordset::DeleteField(short nIndex)
{
	m_pTableDef->DeleteField(nIndex);

	// получаем структуру полей
	m_DBFFields = m_pTableDef->m_DBFFields;
}

/* Изменяем поле таблицы */
void CDBFRecordset::ModifyField(short nIndex, FIELD_REC *pFieldRec)
{
	m_pTableDef->ModifyField(nIndex, pFieldRec);

	// получаем структуру полей
	m_DBFFields = m_pTableDef->m_DBFFields;
}

/* Перемещаем поле таблицы на новую позицию */
void CDBFRecordset::MoveField(short nIndex, short nNewIndex)
{
	m_pTableDef->MoveField(nIndex, nNewIndex);

	// получаем структуру полей
	m_DBFFields = m_pTableDef->m_DBFFields;
}

/* Отказ от изменений сделанных в структуре таблицы */
void CDBFRecordset::CancelUpdateTable()
{
	m_pTableDef->CancelUpdateTable();

	// получаем структуру полей
	m_DBFFields = m_pTableDef->m_DBFFields;
}

/* Сохраняем изменения в таблице */
void CDBFRecordset::UpdateTable()
{
	m_pTableDef->UpdateTable();

	// получаем структуру полей
	m_DBFFields = m_pTableDef->m_DBFFields;

	if(m_pTableDef->GetRecordCount() > 0)
		MoveFirst();
	else
		m_nCurTableRecN = 0;
}

/* Открытие набора записей */
void CDBFRecordset::Open(LPCTSTR lpszFileName, UINT nOpenFlag)
{
	ASSERT_VALID(this);

	CString strFileName;

	if(lpszFileName == NULL)
		strFileName = GetDefaultDBName();
	else
		strFileName = lpszFileName;

	ASSERT(strFileName.GetLength() > 4);

	// Re-Opening is invalid.
	if (IsOpen())
	{
		ASSERT(FALSE);
		return;
	}

	m_pTableDef = new CDBFTableDef;

	// открываем таблицу
	m_pTableDef->Open(strFileName, nOpenFlag);

	if(!IsOpen())
		return;

	// получаем структуру полей
	m_DBFFields = m_pTableDef->m_DBFFields;

	if(m_pTableDef->GetRecordCount() > 0)
	{
		//changed from the first to the last by Jian
		//MoveFirst();
		//MoveLast();
		//m_pTableDef->m_DBFile.Seek(0, CFile::end);

	}
	else
		m_nCurTableRecN = 0;
	m_pTableDef->m_DBFile.Seek(0, CFile::end);
}

/* Закрываем набор записей */
void CDBFRecordset::Close()
{
	ASSERT_VALID(this);

	if (IsOpen())
	{
		//the following is added by Jian Ouyang
		m_pTableDef->WriteHeader();
		m_pTableDef->Close();
	}

	if(m_pFilterParser != NULL)
		delete m_pFilterParser;
	if(m_pFindParser != NULL)
		delete m_pFindParser;

	m_nEditMode = noMode;

	m_nCurTableRecN = -1;
	m_bBOF = TRUE;			
	m_bEOF = TRUE;

	if(m_pTableDef != NULL)
	{
		delete m_pTableDef;
		m_pTableDef = NULL;
		m_DBFFields.m_pFields = NULL;
		m_DBFFields.m_pCurRec = NULL;
	}
}

CString CDBFRecordset::GetDefaultDBName()
{
	ASSERT_VALID(this);

	// Override and add UNC path to .DBF file
	return _T("");
}

/* Функция возвращаеи TRUE если индекс тек. записи
   вышел за границу таблицы */
BOOL CDBFRecordset::IsBOF() const
{
	ASSERT_VALID(this);

	return m_bBOF;
}

/* Функция возвращаеи TRUE если индекс тек. записи
   вышел за границу таблицы */
BOOL CDBFRecordset::IsEOF() const
{
	ASSERT_VALID(this);

	return m_bEOF;
}

/* Функция возвращаеи TRUE если тек. помечена для удаления */
BOOL CDBFRecordset::IsDeleted() const
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());
	return (m_DBFFields.m_pCurRec[0] == REC_FLAG_DELETED);
}

/* Функция возвращает имя таблицы */
CString CDBFRecordset::GetName()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_pTableDef->GetName();
}

/* Функция возвращает полный путь к DBF таблице */
CString CDBFRecordset::GetDBFilePath()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_pTableDef->GetDBFilePath();
}

/* Функция возвращает дату создания таблицы */
CTime CDBFRecordset::GetDateCreated()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_pTableDef->GetDateCreated();
}

/* Функция возвращает дату изменения таблицы */
CTime CDBFRecordset::GetDateLastUpdated()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_pTableDef->GetDateLastUpdated();
}

/* Кол-во записей в таблице */
long CDBFRecordset::GetRecordCount()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_pTableDef->GetRecordCount();
}

/* Кол-во полей в таблице */
short CDBFRecordset::GetFieldCount()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_DBFFields.GetFieldCount();
}

/* Функция возвращает абсолютную позицию тек. записи */
long CDBFRecordset::GetAbsolutePosition()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_nCurTableRecN;
}

/* Функция делает тек. запись абсолютная позиция которой задается пар. lPosition */
void CDBFRecordset::SetAbsolutePosition(long lPosition)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());
	ASSERT(lPosition >= 0 && lPosition < GetRecordCount());

	DBF_LONG lTableRecN = lPosition; 

	// Call Move.
	m_pTableDef->ReadRecord(lTableRecN, m_DBFFields.m_pCurRec);
	m_nCurTableRecN = lTableRecN;
	m_bBOF = m_bEOF = FALSE;

}

/* Переход к следующей записи */
void CDBFRecordset::MoveNext()
{
	ASSERT_VALID(this);
	ASSERT(!IsEOF());

	m_nEditMode = noMode;

	DBF_LONG lTableRecN = m_nCurTableRecN;
	
	lTableRecN++;

	if(lTableRecN < GetRecordCount())
	{
		// Call Move.
		m_pTableDef->ReadRecord(lTableRecN, m_DBFFields.m_pCurRec);
		m_nCurTableRecN = lTableRecN;
		m_bBOF = m_bEOF = FALSE;
	}
	else
	{
		m_bEOF = TRUE;
		m_bBOF = GetRecordCount() > 0; 
	}
	
}

/* Переход к предыдущей записи */
void CDBFRecordset::MovePrev()
{
	ASSERT_VALID(this);
	ASSERT(!IsBOF());

	m_nEditMode = noMode;

	DBF_LONG lTableRecN = m_nCurTableRecN; 

	lTableRecN--;

	if(lTableRecN >= 0)
	{
		// Call Move.
		m_pTableDef->ReadRecord(lTableRecN, m_DBFFields.m_pCurRec);
		m_nCurTableRecN = lTableRecN;
		m_bBOF = m_bEOF = FALSE;
	}
	else
	{
		m_bBOF = TRUE;
		m_bEOF = GetRecordCount() > 0; 
	}
}

/* Переход к первой записи */
void CDBFRecordset::MoveFirst()
{
	ASSERT_VALID(this);
	ASSERT(GetRecordCount() > 0);

	m_nEditMode = noMode;

	DBF_LONG lTableRecN = 0; 

	// Call Move.
	m_pTableDef->ReadRecord(lTableRecN, m_DBFFields.m_pCurRec);
	m_nCurTableRecN = lTableRecN;
	m_bBOF = m_bEOF = FALSE;
	
}

/* Переход к последней записи */
void CDBFRecordset::MoveLast()
{
	ASSERT_VALID(this);
	ASSERT(GetRecordCount() > 0);

	m_nEditMode = noMode;

	DBF_LONG lTableRecN = GetRecordCount() - 1; 

	// Call Move.
	m_pTableDef->ReadRecord(lTableRecN, m_DBFFields.m_pCurRec);
	m_nCurTableRecN = lTableRecN;
	m_bBOF = m_bEOF = FALSE;
}

/* Переход на запись по смещению относительно текущей записи */
void CDBFRecordset::Move(long lOffsetRec)
{
	ASSERT_VALID(this);
	ASSERT(GetRecordCount() > 0);

	m_nEditMode = noMode;

	DBF_LONG lTableRecN = m_nCurTableRecN + lOffsetRec; 

	// переход к предыдущим записям
	if(lOffsetRec < 0)
	{
		if(lTableRecN < 0)
		{
			lTableRecN = 0;
			m_bBOF = TRUE;
		}
		else
			m_bBOF = FALSE;

		m_bEOF = FALSE; 
	}
	// переход к следующим записям
	else if(lOffsetRec > 0)
	{
		if(lTableRecN >= GetRecordCount())
		{
			lTableRecN = GetRecordCount() - 1;
			m_bEOF = TRUE;
		}
		else
			m_bEOF = FALSE;

		m_bBOF = FALSE; 
	}

	// Call Move.
	m_pTableDef->ReadRecord(lTableRecN, m_DBFFields.m_pCurRec);
	m_nCurTableRecN = lTableRecN;
}

/* Поиск первой записи */
BOOL CDBFRecordset::FindFirst(LPCTSTR lpszFilter)
{
	m_nCurTableRecN = -1;
	return FindNext(lpszFilter);
}

/* Поиск следующей записи */
BOOL CDBFRecordset::FindNext(LPCTSTR lpszFilter)
{
	if(m_pFindParser != NULL)	delete m_pFindParser;
	m_pFindParser = new CALXParser;

	alxFetchEmptyParserVariable(m_DBFFields, m_pFindParser);

	try
	{
		m_nCurTableRecN++;

		m_pFindParser->Parse(lpszFilter);
		m_pFindParser->DelUnusedVariables();

		m_nCurTableRecN--;

		return FindNext();
	}
	catch(CALXParserException* e)
	{
		ALXThrowDBFException(e);
	}
	return FALSE;
}

/* Поиск следующей записи */
BOOL CDBFRecordset::FindNext()
{
	ASSERT(m_pFindParser != NULL);

	BOOL bResult = FALSE;
	VARIANT* pvarExprResult = NULL;

	try
	{
		m_nCurTableRecN++;

		if(m_nCurTableRecN >= GetRecordCount())
			return FALSE;

		// перебираем записи
		for(; m_nCurTableRecN < GetRecordCount(); m_nCurTableRecN++)
		{
			m_pTableDef->ReadRecord(m_nCurTableRecN, m_DBFFields.m_pCurRec);
		
			alxFetchParserVariable(this, m_pFindParser);

			// вычисляем результат
			pvarExprResult = m_pFindParser->Execute();

			// если не логическое выражение
			if(V_VT(pvarExprResult) != VT_BOOL)
				break;

			// если нашли
			if(V_BOOL(pvarExprResult) != FALSE)
			{
				bResult = TRUE;
				break;
			}
		}
	
	}
	catch(CALXParserException* e)
	{
		ALXThrowDBFException(e);
	}

	if(m_nCurTableRecN >= GetRecordCount())
	{
		m_nCurTableRecN = GetRecordCount() - 1;
		m_bEOF = TRUE;
	}
	else
		m_bEOF = FALSE;

	m_bBOF = FALSE;

	if(V_VT(pvarExprResult) != VT_BOOL)
		ALXThrowDBFException(ALX_DBF_ERROR_NOT_LOGIC_EXPR);

	return bResult;
}
/* Поиск последней записи */
BOOL CDBFRecordset::FindLast(LPCTSTR lpszFilter)
{
	m_nCurTableRecN = GetRecordCount();
	return FindPrev(lpszFilter);
}

/* Поиск предыдущей записи */
BOOL CDBFRecordset::FindPrev(LPCTSTR lpszFilter)
{
	if(m_pFindParser != NULL)	delete m_pFindParser;
	m_pFindParser = new CALXParser;

	alxFetchEmptyParserVariable(m_DBFFields, m_pFindParser);

	try
	{
		m_nCurTableRecN--;

		m_pFindParser->Parse(lpszFilter);
		m_pFindParser->DelUnusedVariables();

		m_nCurTableRecN++;

		return FindPrev();
	}
	catch(CALXParserException* e)
	{
		ALXThrowDBFException(e);
	}
	return FALSE;
}

/* Поиск предыдущей записи */
BOOL CDBFRecordset::FindPrev()
{
	ASSERT(m_pFindParser != NULL);

	BOOL bResult = FALSE;
	VARIANT* pvarExprResult = NULL;

	try
	{
		m_nCurTableRecN--;

		if(m_nCurTableRecN < 0)
			return FALSE;

		// перебираем записи
		for(; m_nCurTableRecN >= 0; m_nCurTableRecN--)
		{
			m_pTableDef->ReadRecord(m_nCurTableRecN, m_DBFFields.m_pCurRec);
		
			alxFetchParserVariable(this, m_pFindParser);

			// вычисляем результат
			pvarExprResult = m_pFindParser->Execute();

			// если не логическое выражение
			if(V_VT(pvarExprResult) != VT_BOOL)
				break;

			// если нашли
			if(V_BOOL(pvarExprResult) != FALSE)
			{
				bResult = TRUE;
				break;
			}
		}
	
	}
	catch(CALXParserException* e)
	{
		ALXThrowDBFException(e);
	}

	if(m_nCurTableRecN < 0)
	{
		m_nCurTableRecN = 0;
		m_bBOF = TRUE;
	}
	else
		m_bBOF = FALSE;

	m_bEOF = FALSE;

	if(V_VT(pvarExprResult) != VT_BOOL)
		ALXThrowDBFException(ALX_DBF_ERROR_NOT_LOGIC_EXPR);

	return bResult;
}

/* Отказ от сохранения изменений сделанных в записи */
void CDBFRecordset::CancelUpdate()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode != noMode);

	m_pTableDef->ReadRecord(m_nCurTableRecN, m_DBFFields.m_pCurRec);

	m_nEditMode = noMode;
}

/* Блокировка текущей записи */
BOOL CDBFRecordset::LockRecord()
{
	ASSERT_VALID(this);
	ASSERT(!IsBOF());
	ASSERT(!IsEOF());
	ASSERT(m_nEditMode == noMode);

	if(!(m_nOpenType & ALX_DBF_USE_EXCLUSIVE))
	{
		switch(m_nLockType)
		{
		case LockDBase:
			{
				// 0xEFFFFFFFL - rec number
				QWORD offset = 0xEFFFFFFFL - m_nCurTableRecN;
				QWORD range = 1;
				return::LockFile((HANDLE)m_pTableDef->m_DBFile.m_hFile, LODWORD(offset), HIDWORD(offset), LODWORD(range), HIDWORD(range));
			}
		case LockClipper:
			{
				// 0x40000000L + rec number
				QWORD offset = 1000000000 + (m_nCurTableRecN + 1);
				QWORD range = 1;
				return::LockFile((HANDLE)m_pTableDef->m_DBFile.m_hFile, LODWORD(offset), HIDWORD(offset), LODWORD(range), HIDWORD(range));
			}
		case LockFoxpro:
			{
				// 0x40000000L + rec offset
				QWORD offset = 0x40000000L +/* m_pTableDef->m_DBFHead.data_offset +*/ m_pTableDef->m_DBFHead.rec_size * m_nCurTableRecN;
				QWORD range = 1;
				return::LockFile((HANDLE)m_pTableDef->m_DBFile.m_hFile, LODWORD(offset), HIDWORD(offset), LODWORD(range), HIDWORD(range));
			}
		}
	}
	return TRUE;
}

/* Снятие блокировки с текущей записи */
BOOL CDBFRecordset::UnlockRecord()
{
	ASSERT_VALID(this);
	ASSERT(!IsBOF());
	ASSERT(!IsEOF());
	ASSERT(m_nEditMode == noMode);

	if(!(m_nOpenType & ALX_DBF_USE_EXCLUSIVE))
	{
		switch(m_nLockType)
		{
		case LockDBase:
			{
				// 0xEFFFFFFFL - rec number
				QWORD offset = 0xEFFFFFFFL - m_nCurTableRecN;
				QWORD range = 1;
				return::UnlockFile((HANDLE)m_pTableDef->m_DBFile.m_hFile, LODWORD(offset), HIDWORD(offset), LODWORD(range), HIDWORD(range));
			}
		case LockClipper:
			{
				// 0x40000000L + rec number
				QWORD offset = 1000000000 + (m_nCurTableRecN + 1);
				QWORD range = 1;
				return::UnlockFile((HANDLE)m_pTableDef->m_DBFile.m_hFile, LODWORD(offset), HIDWORD(offset), LODWORD(range), HIDWORD(range));
			}
		case LockFoxpro:
			{
				// 0x40000000L + rec offset
				QWORD offset = 0x40000000L +/* m_pTableDef->m_DBFHead.data_offset +*/ m_pTableDef->m_DBFHead.rec_size * m_nCurTableRecN;
				QWORD range = 1;
				return::UnlockFile((HANDLE)m_pTableDef->m_DBFile.m_hFile, LODWORD(offset), HIDWORD(offset), LODWORD(range), HIDWORD(range));
			}
		}
	}
	return TRUE;
}

/* Добавление новой записи */
void CDBFRecordset::AddNew()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	m_nEditMode = addnew;

	memset(m_DBFFields.m_pCurRec, 0, m_pTableDef->m_DBFHead.rec_size);

	COleVariant varEmpty;
	varEmpty.Clear();
	// присваиваем всем полям пустое значение
	for(short i = 0; i < m_DBFFields.GetFieldCount(); i++)
	{
		FIELD_REC* pFieldRec = m_DBFFields.GetFieldRec(i);
		alxSetFieldValue(m_DBFFields.m_pCurRec, pFieldRec, varEmpty, m_lpfnTextConvert);
	}

	m_DBFFields.m_pCurRec[0] = REC_FLAG_NORMAL;
}

/* Изменение текущей записи */
void CDBFRecordset::Edit()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	m_nEditMode = edit;
}

/* Сохранение измененной записи */
void CDBFRecordset::Update()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode != noMode);

	switch(m_nEditMode)
	{
	case edit:
		{
			m_pTableDef->ReadHeader();
			m_pTableDef->WriteRecord(m_nCurTableRecN, m_DBFFields.m_pCurRec);
			m_pTableDef->WriteHeader();
			break;
		}
	case addnew:
		{
			//m_pTableDef->ReadHeader();
			//int ncOunt = GetRecordCount();
			m_pTableDef->m_DBFHead.last_rec++;
			m_nCurTableRecN = max(0,m_pTableDef->m_DBFHead.last_rec-1);
			//m_nCurTableRecN = max(0,GetRecordCount() - 1);
			m_pTableDef->WriteRecord(m_nCurTableRecN, m_DBFFields.m_pCurRec);
			//m_pTableDef->WriteHeader();
			m_bBOF = m_bEOF = FALSE;
			break;
		}
	}

	m_nEditMode = noMode;
}

/* Запись помечается для удаления */
void CDBFRecordset::Delete()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);

	m_DBFFields.m_pCurRec[0] = REC_FLAG_DELETED;

	m_pTableDef->WriteRecord(m_nCurTableRecN, m_DBFFields.m_pCurRec);
}

/* Снимается пометка на удаление */
void CDBFRecordset::Undelete()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);

	m_DBFFields.m_pCurRec[0] = REC_FLAG_NORMAL;

	m_pTableDef->WriteRecord(m_nCurTableRecN, m_DBFFields.m_pCurRec);
}

/* Получаем значение поля */
COleVariant CDBFRecordset::GetFieldValue(LPCTSTR lpszName)
{
	COleVariant varValue;

	GetFieldValue(lpszName, varValue);

	return varValue;
}

/* Получаем значение поля */
COleVariant CDBFRecordset::GetFieldValue(int nIndex)
{
	COleVariant varValue;

	GetFieldValue(nIndex, varValue);

	return varValue;
}

/* Получаем значение поля */
long CDBFRecordset::GetFieldValue(LPCTSTR lpszName, COleVariant& varValue)
{
	for(short nIndex = 0;
		strcmp(m_DBFFields.m_pFields[nIndex].field_name, lpszName) != 0 &&
		nIndex <= m_DBFFields.m_nFieldCount;
		nIndex++)
		{};
	
	return GetFieldValue(nIndex, varValue);
}

/* Получаем значение поля */
long CDBFRecordset::GetFieldValue(int nIndex, COleVariant& varValue)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());
	ASSERT(nIndex < m_DBFFields.m_nFieldCount);

	return m_pTableDef->GetFieldValue(m_DBFFields.m_pCurRec, m_DBFFields.GetFieldRec(nIndex), varValue, m_lpfnTextConvert);
}

/* Сохраняем значение поля */
void CDBFRecordset::SetFieldValue(LPCTSTR lpszName,	const COleVariant& varValue)
{
	for(short nIndex = 0;
		strcmp(m_DBFFields.m_pFields[nIndex].field_name, lpszName) != 0 &&
		nIndex <= m_DBFFields.m_nFieldCount;
		nIndex++)
		{};

	SetFieldValue(nIndex, varValue);
}

/* Сохраняем значение поля */
void CDBFRecordset::SetFieldValue(int nIndex, const COleVariant& varValue)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());
	ASSERT(nIndex < m_DBFFields.m_nFieldCount);

	m_pTableDef->SetFieldValue(m_DBFFields.m_pCurRec, m_DBFFields.GetFieldRec(nIndex), varValue, m_lpfnTextConvert);
}

/* Функция определяет указатель на функцию конвертации текста */
void CDBFRecordset::SetTextConvertFunction(void (FAR * lpfnTextConvert) (CString& strConvertText, TextConvertDirection enDirection))
{
	m_lpfnTextConvert = lpfnTextConvert;
}

/* Функция возвращает TRUE если набор был успешно открыт */
BOOL CDBFRecordset::IsOpen() const
{
	ASSERT_VALID(this);
	return (m_pTableDef == NULL ? FALSE : m_pTableDef->IsOpen());
}

/* Функция выбрасывает исключение */
void CDBFRecordset::ThrowDBFException(long nALXDBFError, LPCTSTR szDescription)
{
	ASSERT_VALID(this);

	ALXThrowDBFException(nALXDBFError, szDescription);
}

#ifdef _DEBUG
void CDBFRecordset::AssertValid() const
{
	CObject::AssertValid();
}

void CDBFRecordset::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);

	CObject::Dump(dc);

	dc << "\nm_nOpenType = " << m_nOpenType;
	dc << "\nm_nEditMode = " << m_nEditMode;
	dc << "\nm_nCurTableRecN = " << m_nCurTableRecN;
	dc << "\nm_bBOF = " << m_bBOF;			
	dc << "\nm_bEOF = " << m_bEOF;			

	dc << "\n";
}
#endif //_DEBUG




BOOL CDBFRecordset::ChangeOpenFlag(UINT nNewFlag)
{
	ASSERT(m_pTableDef != NULL);

	return m_pTableDef->ChangeOpenFlag(nNewFlag);
}
