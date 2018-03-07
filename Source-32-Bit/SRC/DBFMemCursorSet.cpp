// DBFMemCursorset.cpp : implementation of the CDBFMemCursorset class
//
/////////////////////////////////////////////////////////////////////////////
/*
Автор:              Долгачев А.В.
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../INCLUDE/DBFMemCursorset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CDBFMemCursorset
IMPLEMENT_DYNAMIC(CDBFMemCursorset, CDBFRecordset )

/* Конструктор */
CDBFMemCursorset::CDBFMemCursorset(CDBFTableDef* pDBFTable)
{
	m_bDesc = FALSE;

	m_bShowDeletedRec = TRUE;

	m_pTableData = NULL;

	m_pEditRec = NULL;

	m_lLoadRecord = 0;

	m_lpfnIsInclude = NULL;

	m_nCurCursorRecN = -1;

	m_bBOF = TRUE;			
	m_bEOF = TRUE;
}

/* Деструктор */
CDBFMemCursorset::~CDBFMemCursorset()
{
	if (IsOpen())
		Close();
}

/* Открытие таблицы */
void CDBFMemCursorset::Open(LPCTSTR lpszFileName, UINT nOpenFlag)
{
	ASSERT_VALID(this);

	CString strFileName;

	// получаем имя файла
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

	// заполняем структуру заголовка и полей
	m_DBFFields = m_pTableDef->m_DBFFields;
	m_DBFHead = m_pTableDef->m_DBFHead;

	// распределяем память для загрузки таблицы
	UINT lFileSize = m_pTableDef->m_DBFile.GetLength();
	m_pTableData = new DBF_UCHAR[lFileSize];

	// Загружаем файл в память
	m_pTableDef->m_DBFile.Seek(0, CFile::begin);

#if _MSC_VER > 1200
	DBF_CHECK_ERROR(
		m_pTableDef->m_DBFile.Read(m_pTableData, lFileSize) == lFileSize, ALX_DBF_ERROR_DBFILE_MEMORYLOAD);
#else
	DBF_CHECK_ERROR(
		m_pTableDef->m_DBFile.ReadHuge(m_pTableData, lFileSize) == lFileSize, ALX_DBF_ERROR_DBFILE_MEMORYLOAD);
#endif // _MSC_VER > 1200

	// получаем количество загруженных записей
	m_lLoadRecord = m_pTableDef->GetRecordCount();

	// обновляем курсор
	Requery();
}

/* Закрываем набор записей */
void CDBFMemCursorset::Close()
{
	CDBFRecordset::Close();

	m_bDesc = FALSE;
	m_bShowDeletedRec = TRUE;

	m_nCurCursorRecN = -1;

	m_bBOF = TRUE;			
	m_bEOF = TRUE;

	// освобождаем ресурсы

	if(m_pTableData != NULL) { delete[] m_pTableData; m_pTableData = NULL; }

	m_lLoadRecord = 0;

	if(m_pEditRec != NULL) { delete[] m_pEditRec; m_pEditRec = NULL; }


	m_RecPtr.RemoveAll();
	for(int i = 0; i < 	m_NewRecPtr.GetSize(); i++)
	{
		DBF_UCHAR* pNewRec = (DBF_UCHAR*)m_NewRecPtr[i];
		delete[] pNewRec;
	}
	m_NewRecPtr.RemoveAll();
}

/* Функция возвращает TRUE если тек. ячвляется первая запись */
BOOL CDBFMemCursorset::IsBOF() const
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return (m_nCurCursorRecN == 0);
}

/* Функция возвращает TRUE если тек. ячвляется последняя запись */
BOOL CDBFMemCursorset::IsEOF() const
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return (m_nCurCursorRecN >= m_RecPtr.GetSize() - 1);
}

/* Функция возвращает количество записей в наборе */
long CDBFMemCursorset::GetRecordCount()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_RecPtr.GetSize();
}

/* Функция возвращает абсолютную позицию записи в наборе */
long CDBFMemCursorset::GetAbsolutePosition()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_nCurCursorRecN;
}

/* Функция устанавливает абсолютную позицию записи в наборе */
void CDBFMemCursorset::SetAbsolutePosition(long lPosition)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());
	ASSERT(lPosition >= 0 && lPosition < GetRecordCount());

	DBF_LONG lCursorRecN = lPosition; 

	// Call Move.
	m_DBFFields.m_pCurRec = GetRecord(lCursorRecN);
	m_nCurCursorRecN = lCursorRecN;
	m_bBOF = m_bEOF = FALSE;

}

/* Переход на следующую запись */
void CDBFMemCursorset::MoveNext()
{
	ASSERT_VALID(this);
	ASSERT(!IsEOF());

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = m_nCurCursorRecN;
	
	lCursorRecN++;

	if(lCursorRecN < GetRecordCount())
	{
		// Call Move.
		m_DBFFields.m_pCurRec = GetRecord(lCursorRecN);
		m_nCurCursorRecN = lCursorRecN;
		m_bBOF = m_bEOF = FALSE;
	}
	else
	{
		m_bEOF = TRUE;
		m_bBOF = GetRecordCount() > 0; 
	}
}

/* Переход на предыдущую запись */
void CDBFMemCursorset::MovePrev()
{
	ASSERT_VALID(this);
	ASSERT(!IsBOF());

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = m_nCurCursorRecN; 

	lCursorRecN--;

	if(lCursorRecN >= 0)
	{
		// Call Move.
		m_DBFFields.m_pCurRec = GetRecord(lCursorRecN);
		m_nCurCursorRecN = lCursorRecN;
		m_bBOF = m_bEOF = FALSE;
	}
	else
	{
		m_bBOF = TRUE;
		m_bEOF = GetRecordCount() > 0; 
	}
}

/* Переход на первую запись в наборе */
void CDBFMemCursorset::MoveFirst()
{
	ASSERT_VALID(this);
	ASSERT(GetRecordCount() > 0);

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = 0; 

	// Call Move.
	m_DBFFields.m_pCurRec = GetRecord(lCursorRecN);
	m_nCurCursorRecN = lCursorRecN;
	m_bBOF = m_bEOF = FALSE;

}

/* Переход на последнюю запись в наборе */
void CDBFMemCursorset::MoveLast()
{
	ASSERT_VALID(this);
	ASSERT(GetRecordCount() > 0);

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = GetRecordCount() - 1; 

	// Call Move.
	m_DBFFields.m_pCurRec = GetRecord(lCursorRecN);
	m_nCurCursorRecN = lCursorRecN;
	m_bBOF = m_bEOF = FALSE;

}

/* Переход на запись по смещению относительно текущей записи */
void CDBFMemCursorset::Move(long lOffsetRec)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = m_nCurCursorRecN + lOffsetRec;
	
	// переход к предыдущим записям
	if(lOffsetRec < 0)
	{
		if(lCursorRecN < 0)
		{
			lCursorRecN = 0;
			m_bBOF = TRUE;
		}
		else
			m_bBOF = FALSE;

		m_bEOF = FALSE; 
	}
	// переход к следующим записям
	else if(lOffsetRec > 0)
	{
		if(lCursorRecN >= GetRecordCount())
		{
			lCursorRecN = GetRecordCount() - 1;
			m_bEOF = TRUE;
		}
		else
			m_bEOF = FALSE;

		m_bBOF = FALSE; 
	}

	// Call Move.
	m_DBFFields.m_pCurRec = GetRecord(lCursorRecN);
	m_nCurCursorRecN = lCursorRecN;
}

/* Поиск первой записи */
BOOL CDBFMemCursorset::FindFirst(LPCTSTR lpszFilter)
{
	m_nCurCursorRecN = -1;
	return FindNext(lpszFilter);
}

/* Поиск следующей записи */
BOOL CDBFMemCursorset::FindNext(LPCTSTR lpszFilter)
{
	if(m_pFindParser != NULL)	delete m_pFindParser;
	m_pFindParser = new CALXParser;

	alxFetchEmptyParserVariable(m_DBFFields, m_pFindParser);

	try
	{
		m_nCurCursorRecN++;

		m_pFindParser->Parse(lpszFilter);
		m_pFindParser->DelUnusedVariables();

		m_nCurCursorRecN--;

		return FindNext();
	}
	catch(CALXParserException* e)
	{
		ALXThrowDBFException(e);
	}
	return FALSE;
}

/* Поиск следующей записи */
BOOL CDBFMemCursorset::FindNext()
{
	ASSERT(m_pFindParser != NULL);

	BOOL bResult = FALSE;
	VARIANT* pvarExprResult = NULL;

	try
	{
		m_nCurCursorRecN++;

		if(m_nCurCursorRecN >= GetRecordCount())
			return FALSE;

		// перебираем записи
		for(; m_nCurCursorRecN < GetRecordCount(); m_nCurCursorRecN++)
		{
			m_DBFFields.m_pCurRec = GetRecord(m_nCurCursorRecN);
		
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

	if(m_nCurCursorRecN >= GetRecordCount())
	{
		m_nCurCursorRecN = GetRecordCount() - 1;
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
BOOL CDBFMemCursorset::FindLast(LPCTSTR lpszFilter)
{
	m_nCurCursorRecN = m_RecPtr.GetSize();
	return FindPrev(lpszFilter);
}

/* Поиск предыдущей записи */
BOOL CDBFMemCursorset::FindPrev(LPCTSTR lpszFilter)
{
	if(m_pFindParser != NULL)	delete m_pFindParser;
	m_pFindParser = new CALXParser;

	alxFetchEmptyParserVariable(m_DBFFields, m_pFindParser);

	try
	{
		m_nCurCursorRecN--;

		m_pFindParser->Parse(lpszFilter);
		m_pFindParser->DelUnusedVariables();

		m_nCurCursorRecN++;

		return FindPrev();
	}
	catch(CALXParserException* e)
	{
		ALXThrowDBFException(e);
	}
	return FALSE;
}

/* Поиск предыдущей записи */
BOOL CDBFMemCursorset::FindPrev()
{
	ASSERT(m_pFindParser != NULL);

	BOOL bResult = FALSE;
	VARIANT* pvarExprResult = NULL;

	try
	{
		m_nCurCursorRecN--;

		if(m_nCurCursorRecN < 0)
			return FALSE;

		// перебираем записи
		for(; m_nCurCursorRecN >= 0; m_nCurCursorRecN--)
		{
			m_DBFFields.m_pCurRec = GetRecord(m_nCurCursorRecN);
		
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

	if(m_nCurCursorRecN < 0)
	{
		m_nCurCursorRecN = 0;
		m_bBOF = TRUE;
	}
	else
		m_bBOF = FALSE;

	m_bEOF = FALSE;

	if(V_VT(pvarExprResult) != VT_BOOL)
		ALXThrowDBFException(ALX_DBF_ERROR_NOT_LOGIC_EXPR);

	return bResult;
}

/* Отказ от сохранения изменений в записи */
void CDBFMemCursorset::CancelUpdate()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode != noMode);
	ASSERT(m_pEditRec != NULL);

	m_DBFFields.m_pCurRec = GetRecord(m_nCurCursorRecN);

	if(m_pEditRec != NULL)
		delete[] m_pEditRec;

	m_nEditMode = noMode;
}

/* Добавление новой записи. Для сохранения изменений необходимо
   вызвать функцию - Update(), для отката - CancelUpdate()      */ 
void CDBFMemCursorset::AddNew()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);
	ASSERT(m_pEditRec == NULL);

	m_nEditMode = addnew;

	if(m_pEditRec != NULL)
		delete[] m_pEditRec;

	// выделяем память для новой записи
	m_pEditRec = new DBF_CHAR[m_DBFHead.rec_size];

	memset(m_pEditRec, 0, m_DBFHead.rec_size);

	COleVariant varEmpty;
	varEmpty.Clear();
	// присваиваем всем полям пустое значение
	for(short i = 0; i < m_DBFFields.GetFieldCount(); i++)
	{
		FIELD_REC* pFieldRec = m_DBFFields.GetFieldRec(i);
		alxSetFieldValue(m_pEditRec, pFieldRec, varEmpty, m_lpfnTextConvert);
	}

	m_pEditRec[0] = REC_FLAG_NORMAL;

	m_DBFFields.m_pCurRec = m_pEditRec;
}

/* Изменение записи. Для сохранения изменений необходимо
   вызвать функцию - Update(), для отката - CancelUpdate() */ 
void CDBFMemCursorset::Edit()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);
	ASSERT(m_pEditRec == NULL);

	m_nEditMode = edit;

	if(m_pEditRec != NULL)
		delete[] m_pEditRec;

	// выделяем память для копии записи
	m_pEditRec = new DBF_CHAR[m_DBFHead.rec_size];

	// копируем запись
	memcpy(m_pEditRec, m_DBFFields.m_pCurRec, m_DBFHead.rec_size);

	m_DBFFields.m_pCurRec = m_pEditRec;
}

/* Сохранение измененной записи */
void CDBFMemCursorset::Update()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode != noMode);
	ASSERT(m_pEditRec != NULL);

	switch(m_nEditMode)
	{
	case edit:
		{
			SetRecord(m_nCurCursorRecN, m_pEditRec);
			delete [] m_pEditRec;
			break;
		}
	case addnew:
		{
			m_DBFHead.last_rec++;
			m_NewRecPtr.Add(m_pEditRec);
			m_RecPtr.Add(m_pEditRec);
			m_nCurCursorRecN = max(0,GetRecordCount() - 1);
			SetRecord(m_nCurCursorRecN, m_pEditRec);
			m_bBOF = m_bEOF = FALSE;
			break;
		}
	}
	
	m_pEditRec = NULL;

	m_DBFFields.m_pCurRec = GetRecord(m_nCurCursorRecN);

	m_nEditMode = noMode;
}

/* Запись помечается на удаление */
void CDBFMemCursorset::Delete()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);

	m_DBFFields.m_pCurRec[0] = REC_FLAG_DELETED;

	SetRecord(m_nCurCursorRecN, m_DBFFields.m_pCurRec);
}

/* Функция возвращает TRUE если запись была помечена для удаления */
BOOL CDBFMemCursorset::IsDeleted()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return (m_DBFFields.m_pCurRec[0] == REC_FLAG_DELETED);
}

/* Снимаем пометку на удаление */
void CDBFMemCursorset::Undelete()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);

	m_DBFFields.m_pCurRec[0] = REC_FLAG_NORMAL;

	SetRecord(m_nCurCursorRecN, m_DBFFields.m_pCurRec);
}

/* Удаляем указатель на запись. Чтобы действительно запись была удалена
   необходимо вызвать функцию Save(). Запись будет востановлена после Requery() */
void CDBFMemCursorset::Remove()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);

	m_DBFHead.last_rec--;
	m_RecPtr.RemoveAt(m_nCurCursorRecN);

	if(m_nCurCursorRecN >= GetRecordCount() && m_nCurCursorRecN > 0)
		m_nCurCursorRecN--;

	m_DBFFields.m_pCurRec = GetRecord(m_nCurCursorRecN);
}

/* Функция возвращает TRUE если набор записей был успешно открыт */
BOOL CDBFMemCursorset::IsOpen() const
{
	ASSERT_VALID(this);
	return (m_pTableDef == NULL ? FALSE : m_pTableDef->IsOpen());
}

/* Функция выбрасывает исключение */
void CDBFMemCursorset::ThrowDBFException(long nALXDBFError, LPCTSTR szDescription)
{
	ASSERT_VALID(this);

	ALXThrowDBFException(nALXDBFError, szDescription);
}

#ifdef _DEBUG
void CDBFMemCursorset::AssertValid() const
{
	CDBFRecordset::AssertValid();
}

void CDBFMemCursorset::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);

	CDBFRecordset::Dump(dc);

	dc << "\n";
}
#endif //_DEBUG

/* Функция возвращает указатель на запись */
DBF_CHAR* CDBFMemCursorset::GetRecord(long lRecNumber)
{
	ASSERT(lRecNumber < m_RecPtr.GetSize());
	return (DBF_CHAR*)m_RecPtr[lRecNumber];
}

/* Функция возвращает указатель на запись */
void CDBFMemCursorset::GetRecord(long lRecNumber, DBF_CHAR* lpBuffer)
{
	ASSERT(lRecNumber < m_RecPtr.GetSize());
}

/* Функция копирует данные из указанного буфера в буфер записи
   Используется для занисения изменений в буфер DBF таблицы    */
void CDBFMemCursorset::SetRecord(long lRecNumber, const DBF_CHAR* lpBuffer)
{
	ASSERT(lRecNumber < m_RecPtr.GetSize());
	memcpy(m_RecPtr[lRecNumber],lpBuffer,m_DBFHead.rec_size);
}

/* Сортировка по указанному полю */
void CDBFMemCursorset::SortByField(LPCTSTR lpszName, BOOL bDesc /* = FALSE */)
{
	for(short nIndex = 0;
		strcmp(m_DBFFields.m_pFields[nIndex].field_name, lpszName) != 0 &&
		nIndex <= m_DBFFields.m_nFieldCount;
		nIndex++)
	
	ASSERT(nIndex < m_DBFFields.m_nFieldCount);

	SortByField(nIndex, bDesc);
}

/* Сортировка по указанному полю */
void CDBFMemCursorset::SortByField(int nIndex, BOOL bDesc /* = FALSE */)
{
	m_nSortedField = nIndex;

	m_bDesc = bDesc;

	if(GetRecordCount() > 1)
	{
		FIELD_REC* pFieldRec = m_DBFFields.GetFieldRec(nIndex);

		// по двоичным полям не сортируем
		switch(pFieldRec->field_type)
		{
		case FLD_TYPE_DOUBLE: // возможно FLD_TYPE_BINARY
			// если FLD_TYPE_DOUBLE
			if(pFieldRec->len_info.num_size.len == sizeof(DBF_DOUBLE))
				break;
		case FLD_TYPE_GENERAL:
		case FLD_TYPE_PICTURE:
			return;
		}

		qsort( 0, m_RecPtr.GetSize() - 1 );

		m_DBFFields.m_pCurRec = GetRecord(m_nCurCursorRecN);
	}
}

/* Функция сравнения двух записей (индекс поля по которому
   осуществляется сравнение задается m_nSortedField)       */
int CDBFMemCursorset::compare(const void *elem1, const void *elem2)
{
	// if elem1 < elem2 return TRUE else return FALSE
	COleVariant var1, var2;
	BOOL bResult;

	// если по убыванию
	if(m_bDesc)
	{
		// получаем значения полей (причем меняем их местами)
		m_DBFFields.m_pCurRec = (DBF_CHAR*)elem1;
		var2 = GetFieldValue(m_nSortedField);
		m_DBFFields.m_pCurRec = (DBF_CHAR*)elem2;
		var1 = GetFieldValue(m_nSortedField);
	}
	else
	{
		// получаем значения полей
		m_DBFFields.m_pCurRec = (DBF_CHAR*)elem1;
		var1 = GetFieldValue(m_nSortedField);
		m_DBFFields.m_pCurRec = (DBF_CHAR*)elem2;
		var2 = GetFieldValue(m_nSortedField);
	}
	
	// сравниваем значения полей
	if(var1 == var2)
		bResult = FALSE;
	else
	{
		if(var1.vt == VT_EMPTY && var2.vt != VT_EMPTY)
			bResult = TRUE;
		else if (var2.vt == VT_EMPTY && var1.vt != VT_EMPTY)
			bResult = FALSE;
		else
		{
			switch(var1.vt)
			{
			case VT_BSTR:
				{
					CString str1 = var1.bstrVal;
					CString str2 = var2.bstrVal;
					bResult = (str1 < str2);
					break;
				}
			case VT_R8:
				{
					bResult = (var1.dblVal < var2.dblVal);
					break;
				}
			case VT_I4:
				{
					bResult = (var1.lVal < var2.lVal);
					break;
				}
			case VT_CY:
				{
					COleCurrency cur1, cur2;
					cur1.m_cur = var1.cyVal;
					cur2.m_cur = var2.cyVal;
					bResult = (cur1 < cur2);
					break;
				}
			case VT_DATE:
				{
					bResult = (var1.date < var2.date);
					break;
				}
			case VT_BOOL:
				{
					bResult = (var1.boolVal < var2.boolVal);
					break;
				}
			}
		}
	}

	return bResult;
}

/* Алгоритм сортировки полей */
void CDBFMemCursorset::qsort(int from, int to)
{
    int i, j;
	void *x, *tmp;

//	int from	= 0;					/* левый начальный индекс */
//	int to		= m_RecPtr.GetSize();	/* правый конечный индекс */

	if( from >= to ) return;
	/* число элементов <= 1 */

	i = from; j = to;
	x = m_RecPtr[ (i+j) / 2 ];     /* значение из середины */

	do
	{
		/* сужение вправо */
		while( compare(m_RecPtr[i], x) ) i++ ;	// m_RecPtr[i] < x

		/* сужение влево */
		while( compare(x, m_RecPtr[j]) ) j--;	// x < m_RecPtr[j] 

		if( i <= j )
		{   /* обменять */
			tmp = m_RecPtr[i]; m_RecPtr[i] = m_RecPtr[j] ; m_RecPtr[j] = tmp;
			i++;  j--;
		}
	} while( i <= j );

            /* Теперь обе части сошлись в одной точке.
             * Длина левой части = j - from + 1
             *       правой      = to - i   + 1
             * Все числа в левой части меньше всех чисел в правой.
             * Теперь надо просто отсортировать каждую часть в отдельности.
             * Сначала сортируем более короткую (для экономии памяти
             * в стеке ). Рекурсия:
             */
            if( (j - from)  <  (to - i) ){
                    qsort( from, j );
                    qsort( i, to   );
            } else {
                    qsort( i,   to );
                    qsort( from, j );
            }
}

/* Устанавливаем способ вывода полей (отображать
   или нет помеченные на удаление записи)        */
void CDBFMemCursorset::SetDelete(BOOL bHideDeleteRec /* = TRUE */)
{
	m_bShowDeletedRec = !bHideDeleteRec;
}

/* Определяем способ вывода полей (отображаются
   или нет помеченные на удаление записи)        */
BOOL CDBFMemCursorset::GetDelete()
{
	return !m_bShowDeletedRec;
}

/* Обновляем курсор */
void CDBFMemCursorset::Requery()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	m_nEditMode = noMode;

	m_RecPtr.SetSize(m_lLoadRecord + m_NewRecPtr.GetSize());

	long nRec = 0;
	// если отображать помеченные на удаление записи
	if(m_bShowDeletedRec)
	{
		// перебираем все загруженные записи
		for(long i = 0; i < m_lLoadRecord; i++)
		{
			m_RecPtr[nRec] =  m_pTableData + m_DBFHead.data_offset + m_DBFHead.rec_size * i;
			// если указана функция фильтра
			if(m_lpfnIsInclude != NULL)
			{
				// если запись удовлетворяет условию фильтра
				if((*m_lpfnIsInclude)(this))
					nRec++;
			}
			else
				nRec++;
		}
		// перебираем добавленные после загрузки записи
		for(long j = 0; j < m_NewRecPtr.GetSize(); j++)
		{
			m_RecPtr[nRec] = m_NewRecPtr[j];
			// если указана функция фильтра
			if(m_lpfnIsInclude != NULL) 
			{
				// если запись удовлетворяет условию фильтра
				if((*m_lpfnIsInclude)(this))
					nRec++;
			}
			else
				nRec++;
		}
	}
	else
	{
		// перебираем все загруженные записи
		for(long i = 0; i < m_lLoadRecord; i++)
		{
			if(REC_FLAG_DELETED != *(DBF_CHAR*)(m_pTableData + m_DBFHead.data_offset + m_DBFHead.rec_size * i))
			{
				m_RecPtr[nRec] =  m_pTableData + m_DBFHead.data_offset + m_DBFHead.rec_size * i;
				// если указана функция фильтра
				if(m_lpfnIsInclude != NULL) 
				{
					// если запись удовлетворяет условию фильтра
					if((*m_lpfnIsInclude)(this))
						nRec++;
				}
				else
					nRec++;
			}
		}
		// перебираем добавленные после загрузки записи
		for(i = 0; i < m_NewRecPtr.GetSize(); i++)
		{
			if(REC_FLAG_DELETED != *(DBF_CHAR*)(m_NewRecPtr[i]))
			{
				m_RecPtr[nRec] = m_NewRecPtr[i];
				// если указана функция фильтра
				if(m_lpfnIsInclude != NULL) 
				{
					// если запись удовлетворяет условию фильтра
					if((*m_lpfnIsInclude)(this))
						nRec++;
				}
				else
					nRec++;
			}
		}
	}

	m_RecPtr.SetSize(nRec);
	
	m_DBFHead.last_rec = m_RecPtr.GetSize();

	if(GetRecordCount() > 0)
		MoveFirst();
	else
		m_nCurCursorRecN = 0;
}

// Запись изменений в файл */
BOOL CDBFMemCursorset::Save()
{
	CFile* pDBFile = &m_pTableDef->m_DBFile;
/*	if (!SaveFile.Open(lpszPathName, CFile::modeCreate |
		CFile::modeReadWrite | CFile::shareDenyNone))
	{
//		ReportSaveLoadException(lpszPathName, &fe,
//			TRUE, AFX_IDP_INVALID_FILENAME);
		return FALSE;
	}
*/
	DBF_CHAR* pRec;
	pDBFile->Seek(m_DBFHead.data_offset, CFile::begin);
	// сохраняем записи из таблицы
	for(long i = 0; i < m_lLoadRecord; i++)
	{
		pRec = (DBF_CHAR*)(m_pTableData + m_DBFHead.data_offset + m_DBFHead.rec_size * i);
		pDBFile->Write(pRec, m_DBFHead.rec_size);
	}
	// добавляем новые записи
	for(long j = 0; j < m_NewRecPtr.GetSize(); j++)
	{
		pRec = (DBF_CHAR*) m_NewRecPtr[j];
		pDBFile->Write(pRec, m_DBFHead.rec_size);
	}

	char eof = REC_EOF;
	// записываем признак конца DBF файла
	pDBFile->Write(&eof, 1);

	// Изменяем структуру заголовка
	COleDateTime curDate = COleDateTime::GetCurrentTime();
	if(m_DBFHead.dbf_id == 0x30) // Visual FoxPro
		m_DBFHead.last_update[0] = atoi(curDate.Format("%y"));
	else
		m_DBFHead.last_update[0] = curDate.GetYear() - ALX_DBF_CENTURY;

	m_DBFHead.last_update[1] = curDate.GetMonth();
	m_DBFHead.last_update[2] = curDate.GetDay();
	m_DBFHead.last_rec = i + j;

	pDBFile->Seek(0, CFile::begin);
	// записываем заголовок
	pDBFile->Write(&m_DBFHead, sizeof(DBF_HEAD));
	// записываем список полей
	for(i = 0; i < m_DBFFields.GetFieldCount(); i++)
		pDBFile->Write(m_DBFFields.GetFieldRec(i), sizeof(FIELD_REC));
	char end = 0x0D;
	pDBFile->Write(&end, 1);
	// измением размер файла
	pDBFile->SetLength(m_DBFHead.data_offset + m_DBFHead.last_rec * m_DBFHead.rec_size + 1);

	m_pTableDef->m_DBFHead.last_rec = m_DBFHead.last_rec;

	return TRUE;
}

/* Передаем указатель на функцию фильтра. Чтобы убрать фильтр нужно передать NULL */
void CDBFMemCursorset::SetFilter(BOOL (FAR * lpfnIsInclude) (CDBFRecordset* pSet) )
{
	m_lpfnIsInclude = lpfnIsInclude;
}

void CDBFMemCursorset::SetFilter(LPCTSTR lpszFilter)
{
	if(lpszFilter == NULL)
	{
		m_lpfnIsInclude = NULL;
		return;
	}

	if(m_pFilterParser != NULL)	delete m_pFilterParser;

	m_pFilterParser = new CALXParser;

	alxFetchEmptyParserVariable(m_DBFFields, m_pFilterParser);

	try
	{
		m_pFilterParser->Parse(lpszFilter);
		m_pFilterParser->DelUnusedVariables();

		m_lpfnIsInclude = alxFilter;
	}
	catch(CALXParserException* e)
	{
		ALXThrowDBFException(e);
	}
}