// DBFCursorSet.cpp : implementation of the CDBFCursorSet class
//
/////////////////////////////////////////////////////////////////////////////
/*
Автор:              Долгачев А.В.
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../INCLUDE/DBFCursorset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CDBFCursorset
IMPLEMENT_DYNAMIC(CDBFCursorset, CDBFRecordset)

/* Конструктор */
CDBFCursorset::CDBFCursorset(CDBFTableDef* pDBFTable)
{
	m_bDesc = FALSE;

	m_bShowDeletedRec = TRUE;

	m_nCurCursorRecN = -1;
	m_bBOF = TRUE;			
	m_bEOF = TRUE;

	m_lpfnIsInclude = NULL;
}

/* Деструктор */
CDBFCursorset::~CDBFCursorset()
{
	if (IsOpen())
		Close();
}

/* Открытие таблицы */
void CDBFCursorset::Open(LPCTSTR lpszFileName, UINT nOpenFlag)
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

	// обновляем курсор
	Requery();
}

/* Закрываем набор записей */
void CDBFCursorset::Close()
{
	CDBFRecordset::Close();

	m_bDesc = FALSE;
	m_bShowDeletedRec = TRUE;

	m_nCurCursorRecN = -1;
	m_bBOF = TRUE;			
	m_bEOF = TRUE;

	// освобождаем ресурсы

	m_RecPtr.RemoveAll();
}

/* Функция возвращает TRUE если тек. ячвляется первая запись */
BOOL CDBFCursorset::IsBOF() const
{
	ASSERT_VALID(this);

	return m_bBOF;
}

/* Функция возвращает TRUE если тек. ячвляется последняя запись */
BOOL CDBFCursorset::IsEOF() const
{
	ASSERT_VALID(this);

	return m_bEOF;
}

/* Функция возвращает количество записей в наборе */
long CDBFCursorset::GetRecordCount()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_RecPtr.GetSize();
}

/* Функция возвращает абсолютную позицию записи в наборе */
long CDBFCursorset::GetAbsolutePosition()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_nCurCursorRecN;
}

/* Функция устанавливает абсолютную позицию записи в наборе */
void CDBFCursorset::SetAbsolutePosition(long lPosition)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());
	ASSERT(lPosition >= 0 && lPosition < GetRecordCount());

	DBF_LONG lCursorRecN = lPosition; 

	// Call Move.
	m_pTableDef->ReadRecord(m_RecPtr[lCursorRecN], m_DBFFields.m_pCurRec);
	m_nCurTableRecN = m_RecPtr[lCursorRecN];
	m_nCurCursorRecN = lCursorRecN;
	m_bBOF = m_bEOF = FALSE;

}

/* Переход на следующую запись */
void CDBFCursorset::MoveNext()
{
	ASSERT_VALID(this);
	ASSERT(!IsEOF());

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = m_nCurCursorRecN;
	lCursorRecN++;

	if(lCursorRecN < GetRecordCount())
	{
		// Call Move.
		m_pTableDef->ReadRecord(m_RecPtr[lCursorRecN], m_DBFFields.m_pCurRec);
		m_nCurTableRecN = m_RecPtr[lCursorRecN];
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
void CDBFCursorset::MovePrev()
{
	ASSERT_VALID(this);
	ASSERT(!IsBOF());

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = m_nCurCursorRecN;
	lCursorRecN--;

	if(lCursorRecN < GetRecordCount())
	{
		// Call Move.
		m_pTableDef->ReadRecord(m_RecPtr[lCursorRecN], m_DBFFields.m_pCurRec);
		m_nCurTableRecN = m_RecPtr[lCursorRecN];
		m_nCurCursorRecN = lCursorRecN;
	}
	else
	{
		m_bEOF = TRUE;
		m_bBOF = GetRecordCount() > 0; 
	}

}

/* Переход на первую запись в наборе */
void CDBFCursorset::MoveFirst()
{
	ASSERT_VALID(this);
	ASSERT(GetRecordCount() > 0);

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = 0; 

	// Call Move.
	m_pTableDef->ReadRecord(m_RecPtr[lCursorRecN], m_DBFFields.m_pCurRec);
	m_nCurTableRecN = m_RecPtr[lCursorRecN];
	m_nCurCursorRecN = lCursorRecN;
	m_bBOF = m_bEOF = FALSE;
}

/* Переход на последнюю запись в наборе */
void CDBFCursorset::MoveLast()
{
	ASSERT_VALID(this);
	ASSERT(GetRecordCount() > 0);

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = 0; 

	lCursorRecN = GetRecordCount() - 1;

	// Call Move.
	m_pTableDef->ReadRecord(m_RecPtr[lCursorRecN], m_DBFFields.m_pCurRec);
	m_nCurTableRecN = m_RecPtr[lCursorRecN];
	m_nCurCursorRecN = lCursorRecN;
	m_bBOF = m_bEOF = FALSE;
}

/* Переход на запись по смещению относительно текущей записи */
void CDBFCursorset::Move(long lOffsetRec)
{
	ASSERT_VALID(this);
	ASSERT(GetRecordCount() > 0);

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
	m_pTableDef->ReadRecord(m_RecPtr[lCursorRecN], m_DBFFields.m_pCurRec);
	m_nCurTableRecN = m_RecPtr[lCursorRecN];
	m_nCurCursorRecN = lCursorRecN;
}

/* Поиск первой записи */
BOOL CDBFCursorset::FindFirst(LPCTSTR lpszFilter)
{
	m_nCurCursorRecN = -1;
	return FindNext(lpszFilter);
}

/* Поиск следующей записи */
BOOL CDBFCursorset::FindNext(LPCTSTR lpszFilter)
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
BOOL CDBFCursorset::FindNext()
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
			m_nCurTableRecN = m_RecPtr[m_nCurCursorRecN];
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
BOOL CDBFCursorset::FindLast(LPCTSTR lpszFilter)
{
	m_nCurCursorRecN = m_RecPtr.GetSize();
	return FindPrev(lpszFilter);
}

/* Поиск предыдущей записи */
BOOL CDBFCursorset::FindPrev(LPCTSTR lpszFilter)
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
BOOL CDBFCursorset::FindPrev()
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
			m_nCurTableRecN = m_RecPtr[m_nCurCursorRecN];
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
void CDBFCursorset::CancelUpdate()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode != noMode);

	m_pTableDef->ReadRecord(m_RecPtr[m_nCurCursorRecN], m_DBFFields.m_pCurRec);
	m_nCurTableRecN = m_RecPtr[m_nCurCursorRecN];

	m_nEditMode = noMode;
}

/* Добавление новой записи. Для сохранения изменений необходимо
   вызвать функцию - Update(), для отката - CancelUpdate()      */ 
void CDBFCursorset::AddNew()
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

/* Изменение записи. Для сохранения изменений необходимо
   вызвать функцию - Update(), для отката - CancelUpdate() */ 
void CDBFCursorset::Edit()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);

	m_nEditMode = edit;
}

/* Сохранение измененной записи */
void CDBFCursorset::Update()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode != noMode);

	switch(m_nEditMode)
	{
	case edit:
		{
			m_pTableDef->WriteRecord(m_nCurTableRecN, m_DBFFields.m_pCurRec);
			break;
		}
	case addnew:
		{
			m_pTableDef->ReadHeader();
			m_pTableDef->m_DBFHead.last_rec++;
			m_nCurTableRecN = max(0,m_pTableDef->m_DBFHead.last_rec - 1);
			m_pTableDef->WriteRecord(m_nCurTableRecN, m_DBFFields.m_pCurRec);
			m_pTableDef->WriteHeader();
			m_RecPtr.Add(m_nCurTableRecN);
			m_nCurCursorRecN = GetRecordCount() -1;
			break;
		}
	}

	m_nEditMode = noMode;
}

/* Удаляем указатель на запись. Чтобы действительно запись была удалена
   необходимо вызвать функцию Save(). Запись будет востановлена после Requery() */
void CDBFCursorset::Remove()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);

	m_RecPtr.RemoveAt(m_nCurCursorRecN);

	if(m_nCurCursorRecN >= m_RecPtr.GetSize() && m_nCurCursorRecN > 0)
		m_nCurCursorRecN--;

	if(GetRecordCount() > 0)
	{
		m_pTableDef->ReadRecord(m_RecPtr[m_nCurCursorRecN], m_DBFFields.m_pCurRec);
		m_nCurTableRecN = m_RecPtr[m_nCurCursorRecN];
	}
}

/* Функция возвращает TRUE если набор записей был успешно открыт */
BOOL CDBFCursorset::IsOpen() const
{
	ASSERT_VALID(this);
	return (m_pTableDef == NULL ? FALSE : m_pTableDef->IsOpen());
}

/* Функция выбрасывает исключение */
void CDBFCursorset::ThrowDBFException(long nALXDBFError, LPCTSTR szDescription)
{
	ASSERT_VALID(this);

	ALXThrowDBFException(nALXDBFError, szDescription);
}

#ifdef _DEBUG
void CDBFCursorset::AssertValid() const
{
	CDBFRecordset::AssertValid();
}

void CDBFCursorset::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);

	CDBFRecordset::Dump(dc);

	dc << "\n";
}
#endif //_DEBUG

/* Функция копирует данные из указанного буфера в буфер записи
   Используется для занисения изменений в буфер DBF таблицы    */
void CDBFCursorset::SetRecord(long lRecNumber, const DBF_CHAR* lpBuffer)
{
/*	ASSERT(lRecNumber < m_RecPtr.GetSize());
	memcpy(m_RecPtr[lRecNumber],lpBuffer,m_DBFHead.rec_size);
*/
}

/* Сортировка по указанному полю */
void CDBFCursorset::SortByField(LPCTSTR lpszName, BOOL bDesc /* = FALSE */)
{
	for(short nIndex = 0;
		strcmp(m_DBFFields.m_pFields[nIndex].field_name, lpszName) != 0 &&
		nIndex <= m_DBFFields.m_nFieldCount;
		nIndex++)
	
	ASSERT(nIndex < m_DBFFields.m_nFieldCount);

	SortByField(nIndex, bDesc);
}

/* Сортировка по указанному полю */
void CDBFCursorset::SortByField(int nIndex, BOOL bDesc /* = FALSE */)
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

		qsort( 0, GetRecordCount() - 1 );

		m_pTableDef->ReadRecord(m_RecPtr[m_nCurCursorRecN], m_DBFFields.m_pCurRec);
		m_nCurTableRecN = m_RecPtr[m_nCurCursorRecN];
	}

}

/* Функция сравнения двух записей (индекс поля по которому
   осуществляется сравнение задается m_nSortedField)       */
int CDBFCursorset::compare(const long elem1, const long elem2)
{
	// if elem1 < elem2 return TRUE else return FALSE
	COleVariant var1, var2;
	BOOL bResult;

	// если по убыванию
	if(m_bDesc)
	{
		// получаем значения полей (причем меняем их местами)
		m_pTableDef->ReadRecord(elem1, m_DBFFields.m_pCurRec);
		var2 = GetFieldValue(m_nSortedField);
		m_pTableDef->ReadRecord(elem2, m_DBFFields.m_pCurRec);
		var1 = GetFieldValue(m_nSortedField);
	}
	else
	{
		// получаем значения полей
		m_pTableDef->ReadRecord(elem1, m_DBFFields.m_pCurRec);
		var1 = GetFieldValue(m_nSortedField);
		m_pTableDef->ReadRecord(elem2, m_DBFFields.m_pCurRec);
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
void CDBFCursorset::qsort(int from, int to)
{
    int i, j;
	long x, tmp;

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
void CDBFCursorset::SetDelete(BOOL bHideDeleteRec /* = TRUE */)
{
	m_bShowDeletedRec = !bHideDeleteRec;
}

/* Определяем способ вывода полей (отображаются
   или нет помеченные на удаление записи)        */
BOOL CDBFCursorset::GetDelete()
{
	return !m_bShowDeletedRec;
}

/* Обновляем курсор */
void CDBFCursorset::Requery()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	m_nEditMode = noMode;

	m_RecPtr.SetSize(m_pTableDef->GetRecordCount());

	long nRec = 0;
	// если отображать помеченные на удаление записи
	if(m_bShowDeletedRec)
	{
		// перебираем все записи
		for(long i = 0; i < m_RecPtr.GetSize(); i++)
		{
			m_RecPtr[nRec] =  i;
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
		for(long i = 0; i < m_RecPtr.GetSize(); i++)
		{
			m_pTableDef->ReadRecord(m_RecPtr[nRec], m_DBFFields.m_pCurRec);
			if(REC_FLAG_DELETED != m_DBFFields.m_pCurRec[0])
			{
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
	
	if(GetRecordCount() > 0)
		MoveFirst();
	else
	{
		m_nCurTableRecN = 0;
		m_nCurCursorRecN = 0;
	}
}

/* Передаем указатель на функцию фильтра. Чтобы убрать фильтр нужно передать NULL */
void CDBFCursorset::SetFilter(BOOL (FAR * lpfnIsInclude) (CDBFRecordset* pSet) )
{
	m_lpfnIsInclude = lpfnIsInclude;
}

void CDBFCursorset::SetFilter(LPCTSTR lpszFilter)
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