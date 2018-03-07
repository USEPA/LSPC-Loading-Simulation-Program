// DBTMemoFile.cpp : implementation of the CDBTMemoFile class
//
/////////////////////////////////////////////////////////////////////////////
/*
Автор:              Долгачев А.В.
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "../INCLUDE/DBTMemoFile.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CDBTMemoFile
IMPLEMENT_DYNAMIC(CDBTMemoFile, CMemoFile)

/* Конструктор */
CDBTMemoFile::CDBTMemoFile()
{
	m_nBlockSize = 256;
}

/* Деструктор */
CDBTMemoFile::~CDBTMemoFile()
{
	Close();
}

/* Создаем мемо файл */
void CDBTMemoFile::Create(LPCTSTR lpszName)
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

	memset(&m_DBTHead, 0, sizeof(DBT_HEAD));
	strncpy(m_DBTHead.dbf_file_name, GetFileTitle(), 8);
	m_DBTHead.block_size = (DBF_UINT)m_nBlockSize;
	m_DBTHead.next_block = (DBF_ULONG) ceil((double)512/(double)m_DBTHead.block_size);;

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
void CDBTMemoFile::Open(LPCTSTR lpszName, UINT nOpenFlag)
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
	DBF_CHECK_ERROR(m_DBTHead.block_size > 0,ALX_DBF_ERROR_MEMO_BLOCK_SIZE);

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
void CDBTMemoFile::Close()
{
	if(m_bOpen)
		CMemoFile::Close();
}

UINT CDBTMemoFile::GetMemoFileType()
{
	return MEMO_TYPE_DBASEIV;
}

/* Читаем заголовок memo файла */
BOOL CDBTMemoFile::ReadHeader()
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != NULL);

TRY
{
	Seek(0, CFile::begin);

	// Читаем заголовок мемо файла
	if(Read(&m_DBTHead, sizeof(DBT_HEAD)) != sizeof(DBT_HEAD))
		return FALSE;

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
BOOL CDBTMemoFile::WriteHeader()
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != NULL);

TRY
{
	Seek(0, CFile::begin);

	// Записываем заголовок мемо файла
	Write(&m_DBTHead, sizeof(DBT_HEAD));

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
void CDBTMemoFile::ReadMemoRecord(DBF_ULONG lOffsetRec, DBF_LONG& nLen, CString& strMemo)
{
	ASSERT_VALID(this);
	ASSERT(lOffsetRec > 0);

	strMemo = "";

	if(!IsOpen())
		return;

TRY
{
	DBF_LONG  nLenMemo = 0;

	Seek(lOffsetRec * m_DBTHead.block_size + sizeof(DBF_LONG),CFile::begin);
	// Получаем длину поля
	DBF_CHECK_ERROR(
		Read(&nLenMemo, sizeof(nLenMemo)) == sizeof(DBF_LONG),
		ALX_DBF_ERROR_MEMO_FIELD_READ);
	
	// получаем длину данных
	nLenMemo -= sizeof(DBF_LONG) * 2;

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
void CDBTMemoFile::ReadMemoRecord(DBF_ULONG lOffsetRec, DBF_LONG& nLen, unsigned char** pMemoData)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());
	ASSERT(lOffsetRec > 0);

	*pMemoData = NULL;

	if(!IsOpen())
		return;

TRY
{
	DBF_LONG  nLenMemo = 0;

	Seek(lOffsetRec * m_DBTHead.block_size + sizeof(DBF_LONG),CFile::begin);
	// Получаем длину поля
	DBF_CHECK_ERROR(
		Read(&nLenMemo, sizeof(nLenMemo)) == sizeof(DBF_LONG),
		ALX_DBF_ERROR_MEMO_FIELD_READ);
	
	// получаем длину данных
	nLenMemo -= sizeof(DBF_LONG) * 2;

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
void CDBTMemoFile::WriteMemoRecord(DBF_ULONG& lOffsetRec, DBF_LONG& nLen, CString& strMemo)
{
	ASSERT_VALID(this);

	if(!IsOpen())
		return;

TRY
{
	DBF_LONG  nLenMemo = 0;

	// если запись существует
	if(lOffsetRec > 0)
	{
		// необходимое кол-во блоков
		DBF_UINT nNeedBlockCnt = (DBF_UINT) ceil(((double)(strMemo.GetLength() + sizeof(DBF_ULONG) + sizeof(DBF_LONG)))/((double)m_DBTHead.block_size));

		if(strMemo.IsEmpty())
		{
			// удаляем запись
			DeleteMemoRecord(lOffsetRec);
			lOffsetRec = 0;
		}
		else
		{
			Seek(lOffsetRec * m_DBTHead.block_size + sizeof(DBF_LONG),CFile::begin);

			// Получаем длину поля
			DBF_CHECK_ERROR(
			Read(&nLenMemo, sizeof(nLenMemo)) == sizeof(DBF_LONG),
			ALX_DBF_ERROR_MEMO_FIELD_READ);
			
			// тек. кол-во блоков занимаемое мемо полем
			DBF_UINT nBlockCnt = (DBF_UINT) ceil(((double)(nLenMemo))/((double)m_DBTHead.block_size));

			// если запись не подходит по размерам
			if(nBlockCnt != nNeedBlockCnt)
			{
				// удаляем запись
				DeleteMemoRecord(lOffsetRec);
				lOffsetRec = 0;

				// если не удается востановить какую-нибудь запись
				if(!UndeleteMemoRecord(lOffsetRec, strMemo))
					lOffsetRec = AddMemoRecord(strMemo);
			}
			else
				// обновляем запись
				UpdateMemoRecord(lOffsetRec, strMemo);
		}
	}
	else
	{
		// если не удается востановить какую-нибудь запись
		if(!UndeleteMemoRecord(lOffsetRec, strMemo))
			// добавляем новую запись
			lOffsetRec = AddMemoRecord(strMemo);
	}
}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

	ALXThrowDBFException(ALX_DBF_ERROR_MEMO_FIELD_WRITE);

} END_CATCH

}

/* Добавляем memo поле */
DBF_LONG CDBTMemoFile::AddMemoRecord(CString& strMemo)
{
	// номер записи
	DBF_ULONG lOffsetRec = 0;

TRY
{
	// сигнатура
	DBF_LONG nSign = MF_USEDREC;
	DBF_LONG nLenMemo = strMemo.GetLength() + sizeof(DBF_LONG) * 2;

	lOffsetRec = (DBF_ULONG) ceil((double)GetLength() / (double)m_DBTHead.block_size);

	Seek(lOffsetRec * m_DBTHead.block_size,CFile::begin);
		
	// записываем сигнатуру блока
	Write(&nSign, sizeof(nSign));

	// записываем длину записи
	Write(&nLenMemo, sizeof(nLenMemo));
		
	// записываем данные
	Write((LPCSTR)strMemo, strMemo.GetLength());

	ReadHeader();

	if(m_DBTHead.next_block == lOffsetRec)
	{
		m_DBTHead.next_block = (DBF_ULONG) ceil((double)GetLength() / (double)m_DBTHead.block_size);
		WriteHeader();
	}

	// выравниваем по границе блока
	SetLength((DBF_ULONG) ceil((double)GetLength() / (double)m_DBTHead.block_size) * m_DBTHead.block_size);
}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

} END_CATCH

	return lOffsetRec;
}

/* Перезаписываем memo поле */
void CDBTMemoFile::UpdateMemoRecord(DBF_ULONG lOffsetRec, CString& strMemo)
{

TRY
{
	// длина поля
	DBF_LONG nLenMemo = strMemo.GetLength() + sizeof(DBF_LONG) * 2;

	Seek(lOffsetRec * m_DBTHead.block_size + sizeof(DBF_LONG),CFile::begin);
		
	// записываем длину записи
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

/* Удаляем memo поле */
void CDBTMemoFile::DeleteMemoRecord(DBF_ULONG lDelRec)
{

TRY
{
	DBF_LONG  nLenMemo = 0;
	// размер блоков
	DBF_UINT nBlockSize = m_DBTHead.block_size;

	Seek(lDelRec * nBlockSize + sizeof(DBF_ULONG),CFile::begin);
		
	// получаем длину поля
	DBF_CHECK_ERROR(
		Read(&nLenMemo, sizeof(nLenMemo)) == sizeof(DBF_LONG),
		ALX_DBF_ERROR_MEMO_FIELD_READ);

	// получаем кол-во блоков занимаемых мемо полем
	DBF_LONG nDelBlockCnt = (DBF_LONG) ceil(((double)nLenMemo)/((double)nBlockSize));

	ReadHeader();

	// предыдущая запись в списке свободных записей
	DBF_ULONG lPrevFreeRec = 0;
	// текущий запись в списке свободных записей
	DBF_ULONG lCurFreeRec = 0;
	// получаем номер первой записи в списке свободных записей
	DBF_ULONG lNextFreeRec = m_DBTHead.next_block;
	// первая запись за границей файла
	DBF_ULONG lEOFRec = (DBF_ULONG) ceil((double)GetLength() / (double)nBlockSize);

	// перебираем все свободные запирси в списке
	while(lNextFreeRec != lEOFRec && lNextFreeRec != MF_USEDREC)
	{
		DBF_LONG nCurFreeBlockCnt = 1;

		lPrevFreeRec = lCurFreeRec;
		lCurFreeRec = lNextFreeRec;

		Seek(lCurFreeRec * nBlockSize,CFile::begin);

		// получаем указатель на следующую свободную запись
		DBF_CHECK_ERROR(
			Read(&lNextFreeRec, sizeof(lNextFreeRec)) == sizeof(DBF_ULONG),
			ALX_DBF_ERROR_MEMO_FIELD_READ);

		// получаем кол-во своб. смежных блоков
		DBF_CHECK_ERROR(
			Read(&nCurFreeBlockCnt, sizeof(nCurFreeBlockCnt)) == sizeof(DBF_LONG),
			ALX_DBF_ERROR_MEMO_FIELD_READ);

		// если текущий св. запись смежная с удаляемой
		if(lCurFreeRec + nCurFreeBlockCnt == lDelRec)
		{
			Seek(lCurFreeRec * nBlockSize + sizeof(DBF_LONG),CFile::begin);

			// увеличиваем количество смежных блоков
			nCurFreeBlockCnt += nDelBlockCnt;
			Write(&nCurFreeBlockCnt, sizeof(nCurFreeBlockCnt));

			return;
		}
		// если текущий св. запись смежная с удаляемой
		else if(lDelRec + nDelBlockCnt == lCurFreeRec)
		{
			Seek(lDelRec * nBlockSize,CFile::begin);

			// записываем указатель на след. св. запись
			Write(&lNextFreeRec, sizeof(lNextFreeRec));
			// записываем количество смежных блоков
			nDelBlockCnt += nCurFreeBlockCnt;
			Write(&nDelBlockCnt, sizeof(nDelBlockCnt));

			return;
		}
	}

	// если список удаленных записей не пустой
	if(lPrevFreeRec > 0)
	{
		Seek(lPrevFreeRec * nBlockSize,CFile::begin);

		// записываем указатель на удаляемую запись
		Write(&lDelRec, sizeof(lDelRec));

	}
	// пустой список удаленных записей
	else
	{
		// указатель на первый свободный блок
		m_DBTHead.next_block = lDelRec;

		WriteHeader();
	}

	// помечаем блок как удаленный
	Seek(lDelRec * nBlockSize,CFile::begin);

	// записываем указатель на первый блок за границей файла
	Write(&lEOFRec, sizeof(lEOFRec));

	// записываем количество смежных блоков
	Write(&nDelBlockCnt, sizeof(nDelBlockCnt));

}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

} END_CATCH

}

/* Пытаемся вставить новое мемо поле вместо удаленного */
BOOL CDBTMemoFile::UndeleteMemoRecord(DBF_ULONG& lOffsetRec, CString& strMemo)
{

TRY
{
	// длина данных
	DBF_LONG nLenData = strMemo.GetLength();
	// длина мемо поля (с учетом размера служебной информации)
	DBF_LONG nLenMemo = nLenData + sizeof(DBF_LONG) * 2;
	// размер блоков
	DBF_UINT nBlockSize = m_DBTHead.block_size;
	// получаем необходимое кол-во блоков для записи мемо поля
	DBF_LONG nNeedBlockCnt = (DBF_LONG) ceil(((double)(nLenMemo))/((double)nBlockSize));

	ReadHeader();

	// предыдущая запись в списке свободных записей
	DBF_ULONG lPrevFreeRec = 0;
	// текущий запись в списке свободных записей
	DBF_ULONG lCurFreeRec = 0;
	// получаем номер первой записи в списке свободных записей
	DBF_ULONG lNextFreeRec = m_DBTHead.next_block;
	// первая запись за границей файла
	DBF_ULONG lEOFRec = (DBF_ULONG) ceil((double)GetLength() / (double)nBlockSize);

	// перебираем все свободные записи в списке
	while(lNextFreeRec != lEOFRec && lNextFreeRec != MF_USEDREC)
	{
		DBF_LONG nCurFreeBlockCnt = 1;

		lPrevFreeRec = lCurFreeRec;
		lCurFreeRec = lNextFreeRec;

		Seek(lCurFreeRec * nBlockSize,CFile::begin);

		// получаем указатель на следующую свободную запись
		DBF_CHECK_ERROR(
			Read(&lNextFreeRec, sizeof(lNextFreeRec)) == sizeof(DBF_ULONG),
			ALX_DBF_ERROR_MEMO_FIELD_READ);

		// получаем кол-во своб. смежных блоков
		DBF_CHECK_ERROR(
			Read(&nCurFreeBlockCnt, sizeof(nCurFreeBlockCnt)) == sizeof(DBF_LONG),
			ALX_DBF_ERROR_MEMO_FIELD_READ);

		// если текущая св. запись подходит по размеру (больше чем нужно)
		if(nNeedBlockCnt < nCurFreeBlockCnt)
		{
			lOffsetRec = (lCurFreeRec + nCurFreeBlockCnt - nNeedBlockCnt);

			Seek(lOffsetRec * nBlockSize,CFile::begin);

			// сигнатура
			DBF_LONG nSign = MF_USEDREC;

			// записываем сигнатуру блока
			Write(&nSign, sizeof(nSign));

			// записываем длину записи
			Write(&nLenMemo, sizeof(nLenMemo));
		
			// записываем данные
			Write((LPCSTR)strMemo, strMemo.GetLength());

			Seek(lCurFreeRec * nBlockSize + sizeof(DBF_ULONG),CFile::begin);

			// уменьшаем количество смежных блоков
			nCurFreeBlockCnt -= nNeedBlockCnt;
			Write(&nCurFreeBlockCnt, sizeof(nCurFreeBlockCnt));

			return TRUE;
		}
		// если текущий св. запись подходит по размеру
		else if(nNeedBlockCnt == nCurFreeBlockCnt)
		{
			lOffsetRec = lCurFreeRec;

			Seek(lOffsetRec * nBlockSize,CFile::begin);

			// сигнатура
			DBF_LONG nSign = MF_USEDREC;

			// записываем сигнатуру блока
			Write(&nSign, sizeof(nSign));

			// записываем длину записи
			Write(&nLenMemo, sizeof(nLenMemo));
		
			// записываем данные
			Write((LPCSTR)strMemo, strMemo.GetLength());

			Seek(lPrevFreeRec * nBlockSize,CFile::begin);

			// уменьшаем количество св. записей
			Write(&lNextFreeRec, sizeof(lNextFreeRec));

			return TRUE;
		}
	}
}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

} END_CATCH

	return FALSE;
}

#ifdef _DEBUG
void CDBTMemoFile::AssertValid() const
{
	CMemoFile::AssertValid();
}

void CDBTMemoFile::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);

	CMemoFile::Dump(dc);

	dc << "m_DBTHead.next_block = " << m_DBTHead.next_block;
	dc << "m_DBTHead.dbf_file_name = " << m_DBTHead.dbf_file_name;
	dc << "m_DBTHead.block_size = " << m_DBTHead.block_size;

	dc << "\n";
}
#endif //_DEBUG
