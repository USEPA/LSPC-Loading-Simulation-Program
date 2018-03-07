// MemoFile.cpp : implementation of the CMemoFile class
//
/////////////////////////////////////////////////////////////////////////////
/*
Автор:              Долгачев А.В.
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "../INCLUDE/MemoFile.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CMemoFile
IMPLEMENT_DYNAMIC(CMemoFile, CObject)

/* Конструктор */
CMemoFile::CMemoFile()
{
	m_bOpen = FALSE;

	m_pDBTHead = NULL;

	m_nBlockSize = 512;
}

/* Деструктор */
CMemoFile::~CMemoFile()
{
	Close();
}

/* Создаем мемо файл */
void CMemoFile::Create(LPCTSTR lpszName)
{
	ASSERT_VALID(this);
	ASSERT(lpszName != NULL);

	// Re-open is invalid
	if (IsOpen())
	{
		ASSERT(FALSE);
		return;
	}

TRY
{
	// Открываем мемо файл
	DBF_CHECK_ERROR(
		CFile::Open(lpszName, CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive), ALX_DBF_ERROR_MEMO_CREATE);

	m_pDBTHead = new DBT_HEAD;

	DBF_UCHAR* pFirstBlock = new DBF_UCHAR[512];
	memset(pFirstBlock, 0, 512);
	Write(pFirstBlock, 512);
	delete[] pFirstBlock;

	memset(m_pDBTHead, 0, sizeof(DBT_HEAD));
	m_pDBTHead->next_block = 1;

	m_nBlockSize = 512;

	WriteHeader();

	m_bOpen = TRUE;
}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();
	Close();
}
END_CATCH

}

/* Открываем мемо файл */
void CMemoFile::Open(LPCTSTR lpszName, UINT nOpenFlag)
{
	ASSERT_VALID(this);
	ASSERT(lpszName != NULL);

	// открываем как бинарный файл
	nOpenFlag = nOpenFlag | CFile::typeBinary;

	// Re-open is invalid
	if (IsOpen())
	{
		ASSERT(FALSE);
		return;
	}


TRY
{

	// Открываем мемо файл
	DBF_CHECK_ERROR(
		CFile::Open(lpszName, nOpenFlag), ALX_DBF_ERROR_MEMO_OPEN);

	// Читаем заголовок
	DBF_CHECK_ERROR(
		ReadHeader(),ALX_DBF_ERROR_MEMO_HEAD_READ);

	// Проверяем размер блока
	DBF_CHECK_ERROR(m_nBlockSize > 0,ALX_DBF_ERROR_MEMO_BLOCK_SIZE);

	m_bOpen = TRUE;
}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();
	Close();
}
END_CATCH

}

/* Открыт ли файл */
BOOL CMemoFile::IsOpen() const
{
	ASSERT_VALID(this);
	return m_bOpen;
}

/* Закрываем таблицу */
void CMemoFile::Close()
{
	if(m_bOpen)
		CFile::Close();

	m_bOpen = FALSE;

	if(m_pDBTHead != NULL)
		delete m_pDBTHead;
	m_pDBTHead = NULL;
}

UINT CMemoFile::GetMemoFileType()
{
	return MEMO_TYPE_DBASE;
}

/* Читаем заголовок memo файла */
BOOL CMemoFile::ReadHeader()
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != NULL);

TRY
{
	Seek(0, CFile::begin);

	if(m_pDBTHead == NULL)
		m_pDBTHead = new DBT_HEAD;

	// Читаем заголовок мемо файла
	if(Read(m_pDBTHead, sizeof(DBT_HEAD)) != sizeof(DBT_HEAD))
		return FALSE;

	m_nBlockSize = m_pDBTHead->block_size;

	if(m_nBlockSize <= 0)
			m_nBlockSize = 512;

	return TRUE;
}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

} END_CATCH

	return FALSE;
}

/* Записываем заголовок мемо файла */
BOOL CMemoFile::WriteHeader()
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != NULL);

TRY
{
	Seek(0, CFile::begin);

	// Записываем заголовок мемо файла
	Write(m_pDBTHead, sizeof(DBT_HEAD));

	return TRUE;
}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

} END_CATCH

	return FALSE;
}

/* Читаем мемо поле */
void CMemoFile::ReadMemoRecord(DBF_ULONG lOffsetRec, DBF_LONG& nLen, CString& strMemo)
{
	ASSERT_VALID(this);
	ASSERT(lOffsetRec > 0);

	strMemo = "";

	if(!IsOpen())
		return;

TRY
{
	DBF_UCHAR*	pBuffer;
	DBF_CHAR	chCurChar	= 0x0;
	DBF_UINT	nBlockCount	= 1;
	DBF_LONG	nLenMemo = 0;
	DBF_LONG	lMaxLen;

	Seek(lOffsetRec * m_nBlockSize,CFile::begin);
	
	// Отведем буфер для одной записи
	pBuffer = new DBF_UCHAR[m_nBlockSize];
	lMaxLen = (DBF_LONG)(GetLength() - GetPosition());

	while(chCurChar != REC_EOF && nLenMemo < lMaxLen)
	{
		// Читаем
		if(Read(&chCurChar, sizeof(chCurChar)) != sizeof(DBF_UCHAR))
		{
			ASSERT(FALSE);
			delete[] pBuffer;
			ALXThrowDBFException(ALX_DBF_ERROR_MEMO_FIELD_READ);
		}
		else
		{
			if(chCurChar == REC_EOF)
				pBuffer[nLenMemo] = '\0';
			else
				pBuffer[nLenMemo] = chCurChar;
			nLenMemo++;
			if(nLenMemo >= nBlockCount * m_nBlockSize)
			{
				DBF_UCHAR* pBuff;
				nBlockCount++;
				pBuff = new DBF_UCHAR[nBlockCount * m_nBlockSize];
				memcpy(pBuff,pBuffer,(nBlockCount - 1) * m_nBlockSize);
				delete[] pBuffer;
				pBuffer = pBuff;
			}
		}
	}

	memcpy(strMemo.GetBuffer(nLenMemo),pBuffer,nLenMemo);
	strMemo.ReleaseBuffer(nLenMemo);

	delete[] pBuffer;

	nLen = nLenMemo;

}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

	ALXThrowDBFException(ALX_DBF_ERROR_MEMO_FIELD_READ);

} END_CATCH

}

/* Читаем мемо поле */
void CMemoFile::ReadMemoRecord(DBF_ULONG lOffsetRec, DBF_LONG& nLen, unsigned char** pMemoData)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());
	ASSERT(lOffsetRec > 0);

	*pMemoData = NULL;

	if(!IsOpen())
		return;

TRY
{
	DBF_CHAR	chCurChar	= 0x0;
	DBF_UINT	nBlockCount	= 1;
	DBF_LONG	nLenMemo = 0;
	DBF_LONG	lMaxLen;

	Seek(lOffsetRec * m_nBlockSize,CFile::begin);
	
	// Отведем буфер для одной записи
	*pMemoData = new DBF_UCHAR[m_nBlockSize];
	lMaxLen = (DBF_LONG)(GetLength() - GetPosition());

	while(chCurChar != REC_EOF && nLenMemo < lMaxLen)
	{
		// Читаем
		if(Read(&chCurChar, sizeof(chCurChar)) != sizeof(DBF_UCHAR))
		{
			ASSERT(FALSE);
			delete[] *pMemoData;
			*pMemoData = NULL;
			ALXThrowDBFException(ALX_DBF_ERROR_MEMO_FIELD_READ);
		}
		else
		{
			if(chCurChar == REC_EOF)
				(*pMemoData)[nLenMemo] = '\0';
			else
				(*pMemoData)[nLenMemo] = chCurChar;
			nLenMemo++;
			if(nLenMemo >= nBlockCount * m_nBlockSize)
			{
				DBF_UCHAR* pBuff;
				nBlockCount++;
				pBuff = new DBF_UCHAR[nBlockCount * m_nBlockSize];
				memcpy(pBuff,*pMemoData,(nBlockCount - 1) * m_nBlockSize);
				delete[] *pMemoData;
				*pMemoData = pBuff;
			}
		}
	}
	nLen = nLenMemo;

}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

	ALXThrowDBFException(ALX_DBF_ERROR_MEMO_FIELD_READ);

} END_CATCH

}

/* Записываем в мемо поле */
void CMemoFile::WriteMemoRecord(DBF_ULONG& lOffsetRec, DBF_LONG& nLen, CString& strMemo)
{
	ASSERT_VALID(this);

	if(!IsOpen())
		return;

TRY
{
	DBF_LONG  nLenMemo = 0;

	// проверяем длину поля
	if(strMemo.IsEmpty())
		lOffsetRec = 0;
	// если запись существует
	else if(lOffsetRec > 0)
	{
		CString strCurMemoData;

		// получаем текущую длину поля
		ReadMemoRecord(lOffsetRec, nLenMemo, strCurMemoData);
		Seek(lOffsetRec * m_nBlockSize + sizeof(DBF_LONG),CFile::begin);

		// получаем максимально возможную длину (с учетом выравнивания по границе блока)
		nLenMemo = (DBF_LONG) ceil(((double)(nLenMemo + sizeof(DBF_CHAR)))/((double)m_nBlockSize)) * m_nBlockSize - sizeof(DBF_CHAR);

		// если запись не подходит по размеру
		if(nLenMemo < strMemo.GetLength())
			// добавляем запись
			lOffsetRec = AddMemoRecord(strMemo);
		else
			// обновляем запись
			UpdateMemoRecord(lOffsetRec, strMemo);
	}
	else
		// добавляем запись
		lOffsetRec = AddMemoRecord(strMemo);
}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

	ALXThrowDBFException(ALX_DBF_ERROR_MEMO_FIELD_WRITE);

} END_CATCH

}

#ifdef _DEBUG
void CMemoFile::AssertValid() const
{
	CObject::AssertValid();
}

void CMemoFile::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);

	CObject::Dump(dc);

	dc << "m_bOpen = " << m_bOpen;

	dc << "\n";
}
#endif //_DEBUG

/* Добавляем memo поле */
DBF_LONG CMemoFile::AddMemoRecord(CString& strMemo)
{
	// номер записи
	DBF_ULONG lOffsetRec = 0;

TRY
{
	DBF_LONG nLenMemo = strMemo.GetLength();

	ReadHeader();

	lOffsetRec = m_pDBTHead->next_block;
	m_pDBTHead->next_block += (DBF_LONG) ceil((double)(nLenMemo + sizeof(DBF_CHAR)) / (double)m_nBlockSize);

	Seek(lOffsetRec * m_nBlockSize,CFile::begin);
		
	// записываем данные
	Write((LPCSTR)strMemo, strMemo.GetLength());

	DBF_CHAR eof = REC_EOF;
	// записываем признак конца записи
	Write(&eof, sizeof(eof));


	WriteHeader();
}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

} END_CATCH

	return lOffsetRec;
}

/* Перезаписываем memo поле */
void CMemoFile::UpdateMemoRecord(DBF_ULONG lOffsetRec, CString& strMemo)
{

TRY
{
	DBF_LONG nLenMemo = strMemo.GetLength();

	Seek(lOffsetRec * m_nBlockSize,CFile::begin);
		
	// записываем данные
	Write((LPCSTR)strMemo, strMemo.GetLength());

	DBF_CHAR eof = REC_EOF;
	// записываем признак конца записи
	Write(&eof, sizeof(eof));
}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

} END_CATCH

}

