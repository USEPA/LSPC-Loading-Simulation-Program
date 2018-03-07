// FPTMemoFile.cpp : implementation of the CFPTMemoFile class
//
/////////////////////////////////////////////////////////////////////////////
/*
Автор:              Долгачев А.В.
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "../INCLUDE/FPTMemoFile.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CFPTMemoFile
IMPLEMENT_DYNAMIC(CFPTMemoFile, CMemoFile)

/* Конструктор */
CFPTMemoFile::CFPTMemoFile()
{
	m_nBlockSize = 64;
}

/* Деструктор */
CFPTMemoFile::~CFPTMemoFile()
{
	Close();
}

/* Создаем мемо файл */
void CFPTMemoFile::Create(LPCTSTR lpszName)
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

	DBF_UCHAR* pFirstBlock = new DBF_UCHAR[512];
	memset(pFirstBlock, 0, 512);
	Write(pFirstBlock, 512);
	delete[] pFirstBlock;

	memset(&m_FPTHead, 0, sizeof(FPT_HEAD));
	m_FPTHead.block_size = (DBF_UINT)m_nBlockSize;
	m_FPTHead.next_block = (DBF_ULONG) ceil((double)512/(double)m_FPTHead.block_size);

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
void CFPTMemoFile::Open(LPCTSTR lpszName, UINT nOpenFlag)
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

/* Закрываем таблицу */
void CFPTMemoFile::Close()
{
	if(m_bOpen)
		CMemoFile::Close();
}

UINT CFPTMemoFile::GetMemoFileType()
{
	return MEMO_TYPE_FOXPRO;
}

/* Читаем заголовок memo файла */
BOOL CFPTMemoFile::ReadHeader()
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != NULL);

TRY
{
	Seek(0, CFile::begin);

	// Читаем заголовок мемо файла
	if(Read(&m_FPTHead, sizeof(FPT_HEAD)) != sizeof(FPT_HEAD))
		return FALSE;

	m_nNextBlock = SWAP_DWORD_BYTE(m_FPTHead.next_block);
	m_nBlockSize = SWAP_WORD_BYTE(m_FPTHead.block_size);

	return TRUE;
}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

} END_CATCH

	return FALSE;
}

/* Записываем заголовок таблицы */
BOOL CFPTMemoFile::WriteHeader()
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != NULL);

TRY
{
	Seek(0, CFile::begin);

	// Записываем заголовок мемо файла
	Write(&m_FPTHead, sizeof(FPT_HEAD));

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
void CFPTMemoFile::ReadMemoRecord(DBF_ULONG lOffsetRec, DBF_LONG& nLen, CString& strMemo)
{
	ASSERT_VALID(this);
	ASSERT(lOffsetRec > 0);

	strMemo = "";

	if(!IsOpen())
		return;

TRY
{
	DBF_LONG  nLenMemo = 0;

	Seek(lOffsetRec * m_nBlockSize + sizeof(DBF_LONG),CFile::begin);

	// Получаем длину поля
	DBF_CHECK_ERROR(
		Read(&nLenMemo, sizeof(nLenMemo)) == sizeof(DBF_LONG),
		ALX_DBF_ERROR_MEMO_FIELD_READ);

	nLenMemo = SWAP_DWORD_BYTE(nLenMemo);

	// Проверяем длину поля
	DBF_CHECK_ERROR(nLenMemo > 0,ALX_DBF_ERROR_MEMO_FIELD_READ);

	char* buffer = strMemo.GetBuffer(nLenMemo + 1);

	// Читаем мемо поле
	DBF_CHECK_ERROR(
		Read(buffer, nLenMemo) == (UINT)nLenMemo,
		ALX_DBF_ERROR_MEMO_FIELD_READ);

	buffer[nLenMemo] = '\0';
	strMemo.ReleaseBuffer(nLenMemo + 1);

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
void CFPTMemoFile::ReadMemoRecord(DBF_ULONG lOffsetRec, DBF_LONG& nLen, unsigned char** pMemoData)
{
	ASSERT_VALID(this);
	ASSERT(lOffsetRec > 0);

	*pMemoData = NULL;

	if(!IsOpen())
		return;

TRY
{
	DBF_LONG  nLenMemo = 0;

	Seek(lOffsetRec * m_nBlockSize + sizeof(DBF_LONG),CFile::begin);

	// Получаем длину поля
	DBF_CHECK_ERROR(
		Read(&nLenMemo, sizeof(nLenMemo)) == sizeof(DBF_LONG),
		ALX_DBF_ERROR_MEMO_FIELD_READ);

	nLenMemo = SWAP_DWORD_BYTE(nLenMemo);

	// Проверяем длину поля
	DBF_CHECK_ERROR(nLenMemo > 0,ALX_DBF_ERROR_MEMO_FIELD_READ);

	*pMemoData = new unsigned char[nLenMemo];

	// Читаем мемо поле
	DBF_CHECK_ERROR(
		Read(*pMemoData, nLenMemo) == (UINT)nLenMemo,
		ALX_DBF_ERROR_MEMO_FIELD_READ);

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
void CFPTMemoFile::WriteMemoRecord(DBF_ULONG& lOffsetRec, DBF_LONG& nLen, CString& strMemo)
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
		Seek(lOffsetRec * m_nBlockSize + sizeof(DBF_LONG),CFile::begin);

		// Получаем длину поля
		DBF_CHECK_ERROR(
			Read(&nLenMemo, sizeof(nLenMemo)) == sizeof(DBF_LONG),
			ALX_DBF_ERROR_MEMO_FIELD_READ);

		// получаем текущую длину поля
		nLenMemo = SWAP_DWORD_BYTE(nLenMemo);
		// получаем максимально возможную длину (с учетом выравнивания по границе блока)
		nLenMemo = (DBF_LONG) ceil(((double)(nLenMemo + sizeof(DBF_LONG)))/((double)m_nBlockSize)) * m_nBlockSize - sizeof(DBF_LONG);

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

/* Добавляем memo поле */
DBF_LONG CFPTMemoFile::AddMemoRecord(CString& strMemo)
{
	// номер записи
	DBF_ULONG lOffsetRec = 0;

TRY
{
	DBF_LONG nSign = MF_TEXTREC;
	DBF_LONG nLenMemo = strMemo.GetLength();

	ReadHeader();

	lOffsetRec = m_nNextBlock;
	m_nNextBlock += (DBF_LONG) ceil((double)(nLenMemo + sizeof(nSign)) / (double)m_nBlockSize);
	m_FPTHead.next_block = SWAP_DWORD_BYTE(m_nNextBlock);

	Seek(lOffsetRec * m_nBlockSize,CFile::begin);
		
	// записываем сигнатуру блока
	nSign = SWAP_DWORD_BYTE(nSign); // запись в перевернутом виде
	Write(&nSign, sizeof(nSign));

	// записываем длину записи
	nLenMemo = SWAP_DWORD_BYTE(nLenMemo); // запись в перевернутом виде
	Write(&nLenMemo, sizeof(nLenMemo));
		
	// записываем данные
	Write((LPCSTR)strMemo, strMemo.GetLength());


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
void CFPTMemoFile::UpdateMemoRecord(DBF_ULONG lOffsetRec, CString& strMemo)
{

TRY
{
	DBF_LONG nLenMemo = strMemo.GetLength();

	Seek(lOffsetRec * m_nBlockSize + sizeof(DBF_LONG),CFile::begin);
		
	// записываем длину записи
	nLenMemo = SWAP_DWORD_BYTE(nLenMemo); // запись в перевернутом виде
	Write(&nLenMemo, sizeof(nLenMemo));
		
	// записываем данные
	Write((LPCSTR)strMemo, strMemo.GetLength());

}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

} END_CATCH

}

#ifdef _DEBUG
void CFPTMemoFile::AssertValid() const
{
	CMemoFile::AssertValid();
}

void CFPTMemoFile::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);

	CMemoFile::Dump(dc);

	dc << "m_nNextBlock = " << m_nNextBlock;
	dc << "m_nBlockSize = " << m_nBlockSize;

	dc << "\n";
}
#endif //_DEBUG
