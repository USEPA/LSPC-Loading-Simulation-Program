// SMTMemoFile.cpp : implementation of the CSMTMemoFile class
//
/////////////////////////////////////////////////////////////////////////////
/*
Автор:              Долгачев А.В.
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "../INCLUDE/SMTMemoFile.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CSMTMemoFile
IMPLEMENT_DYNAMIC(CSMTMemoFile, CMemoFile)

/* Конструктор */
CSMTMemoFile::CSMTMemoFile()
{
	m_nBlockSize = 128;
}

/* Деструктор */
CSMTMemoFile::~CSMTMemoFile()
{
	Close();
}

/* Создаем мемо файл */
void CSMTMemoFile::Create(LPCTSTR lpszName)
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

	memset(&m_SMTHead, 0, sizeof(SMT_HEAD));
	strncpy(m_SMTHead.description, "SIxMemo", 8);
	m_SMTHead.block_size = (DBF_LONG)m_nBlockSize;
	m_SMTHead.next_block = (DBF_ULONG) ceil((double)512/(double)m_SMTHead.block_size);

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
void CSMTMemoFile::Open(LPCTSTR lpszName, UINT nOpenFlag)
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
	DBF_CHECK_ERROR(m_SMTHead.block_size > 0,ALX_DBF_ERROR_MEMO_BLOCK_SIZE);

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
void CSMTMemoFile::Close()
{
	if(m_bOpen)
		CMemoFile::Close();
}

UINT CSMTMemoFile::GetMemoFileType()
{
	return MEMO_TYPE_SIX;
}

/* Читаем заголовок memo файла */
BOOL CSMTMemoFile::ReadHeader()
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != NULL);

TRY
{
	Seek(0, CFile::begin);

	// Читаем заголовок мемо файла
	if(Read(&m_SMTHead, sizeof(SMT_HEAD)) != sizeof(SMT_HEAD))
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
BOOL CSMTMemoFile::WriteHeader()
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != NULL);

TRY
{
	Seek(0, CFile::begin);

	// Записываем заголовок мемо файла
	Write(&m_SMTHead, sizeof(SMT_HEAD));

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
void CSMTMemoFile::ReadMemoRecord(DBF_ULONG lOffsetRec, DBF_LONG& nLen, CString& strMemo)
{
	ASSERT_VALID(this);
	ASSERT(lOffsetRec > 0);

	strMemo = "";

	if(!IsOpen())
		return;

TRY
{
	Seek(lOffsetRec * m_SMTHead.block_size,CFile::begin);
			
	// Проверяем длину поля
	DBF_CHECK_ERROR(nLen > 0,ALX_DBF_ERROR_MEMO_FIELD_READ);

	char* buffer = strMemo.GetBuffer(nLen + 1);

	// Читаем мемо поле
	DBF_CHECK_ERROR(
		Read(buffer, nLen) == (UINT)nLen,
		ALX_DBF_ERROR_MEMO_FIELD_READ);

	buffer[nLen] = '\0';
	strMemo.ReleaseBuffer(nLen + 1);

}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

	ALXThrowDBFException(ALX_DBF_ERROR_MEMO_FIELD_READ);

} END_CATCH

}

/* Читаем мемо поле */
void CSMTMemoFile::ReadMemoRecord(DBF_ULONG lOffsetRec, DBF_LONG& nLen, unsigned char** pMemoData)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());
	ASSERT(lOffsetRec > 0);

	*pMemoData = NULL;

	if(!IsOpen())
		return;

TRY
{
	Seek(lOffsetRec * m_SMTHead.block_size,CFile::begin);
			
	// Проверяем длину поля
	DBF_CHECK_ERROR(nLen > 0,ALX_DBF_ERROR_MEMO_FIELD_READ);

	*pMemoData = new unsigned char[nLen];

	// Читаем мемо поле
	DBF_CHECK_ERROR(
		Read(*pMemoData, nLen) == (UINT)nLen,
		ALX_DBF_ERROR_MEMO_FIELD_READ);

}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

	ALXThrowDBFException(ALX_DBF_ERROR_MEMO_FIELD_READ);

} END_CATCH

}

/* Записываем в мемо поле */
void CSMTMemoFile::WriteMemoRecord(DBF_ULONG& lOffsetRec, DBF_LONG& nLenMemo, CString& strMemo)
{
	ASSERT_VALID(this);

	if(!IsOpen())
		return;

TRY
{
	// если запись существует
	if(lOffsetRec > 0)
	{
		// тек. кол-во блоков занимаемое мемо полем
		DBF_UINT nBlockCnt = (DBF_UINT) ceil(((double)(nLenMemo))/((double)m_SMTHead.block_size));

		if(strMemo.IsEmpty())
		{
			// удаляем запись
			DeleteMemoRecord(lOffsetRec, nBlockCnt);
			lOffsetRec = 0;
		}
		else
		{
			// необходимое кол-во блоков
			DBF_UINT nNeedBlockCnt = (DBF_UINT) ceil(((double)(strMemo.GetLength()))/((double)m_SMTHead.block_size));

			// если запись не подходит по размерам
			if(nBlockCnt != nNeedBlockCnt)
			{
				// удаляем запись
				DeleteMemoRecord(lOffsetRec, nBlockCnt);
				lOffsetRec = 0;

				// если не удается востановить какую-нибудь запись
				if(!UndeleteMemoRecord(lOffsetRec, strMemo))
					// добавляем новую запись
					lOffsetRec = AddMemoRecord(strMemo);
			}
			else
				// обновляем запись
				UpdateMemoRecord(lOffsetRec, strMemo);
		}
	}
	else
	{
		if(!UndeleteMemoRecord(lOffsetRec, strMemo))
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

#ifdef _DEBUG
void CSMTMemoFile::AssertValid() const
{
	CMemoFile::AssertValid();
}

void CSMTMemoFile::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);

	CMemoFile::Dump(dc);

	dc << "m_SMTHead.next_block = " << m_SMTHead.next_block;
	dc << "m_SMTHead.block_size = " << m_SMTHead.block_size;
	dc << "m_SMTHead.description = " << m_SMTHead.description;

	dc << "\n";
}
#endif //_DEBUG

/* Добавляем memo поле */
DBF_LONG CSMTMemoFile::AddMemoRecord(CString& strMemo)
{
	// номер записи
	DBF_ULONG lOffsetRec = 0;

TRY
{
	DBF_LONG nLenMemo = strMemo.GetLength();

	lOffsetRec = (DBF_ULONG) ceil((double)GetLength() / (double)m_SMTHead.block_size);

	Seek(lOffsetRec * m_SMTHead.block_size,CFile::begin);
		
	// записываем данные
	Write((LPCSTR)strMemo, strMemo.GetLength());

	ReadHeader();

	m_SMTHead.next_block = (DBF_ULONG) ceil((double)GetLength() / (double)m_SMTHead.block_size);

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
void CSMTMemoFile::UpdateMemoRecord(DBF_ULONG lOffsetRec, CString& strMemo)
{

TRY
{

	Seek(lOffsetRec * m_SMTHead.block_size,CFile::begin);
		
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
void CSMTMemoFile::DeleteMemoRecord(DBF_ULONG lDelRec, DBF_UINT nDelBlockCnt)
{

TRY
{
	ReadHeader();

	// 2 байта на кол-во блоков в записи и 4 номер записи
	int rec_size = sizeof(DBF_UINT) + sizeof(DBF_ULONG);
	// максимальное кол-во записей в списке
	int max_rec_cnt = sizeof(m_SMTHead.free_rec_ptr)/rec_size;

	for(int i = 0; i < m_SMTHead.free_rec_count; i++)
	{
		DBF_UINT nCurFreeBlockCnt = 0;
		DBF_ULONG lCurFreeRec = 0;

		// получаем кол-во блоков в свободной записи
		memcpy(&nCurFreeBlockCnt, m_SMTHead.free_rec_ptr + i * rec_size, sizeof(nCurFreeBlockCnt));
		// получаем номер свободной записи
		memcpy(&lCurFreeRec, m_SMTHead.free_rec_ptr + i * rec_size + sizeof(nCurFreeBlockCnt), sizeof(lCurFreeRec));

		// если записи смежные
		if(lDelRec + nDelBlockCnt == lCurFreeRec)
		{
			lCurFreeRec = lDelRec;
			nCurFreeBlockCnt += nDelBlockCnt;

			// меняем кол-во блоков в свободной записи
			memcpy(m_SMTHead.free_rec_ptr + i * rec_size, &nCurFreeBlockCnt,  sizeof(nCurFreeBlockCnt));
			// меняем номер свободной записи
			memcpy(m_SMTHead.free_rec_ptr + i * rec_size + sizeof(nCurFreeBlockCnt), &lCurFreeRec, sizeof(lCurFreeRec));

			WriteHeader();

			return;
		}
		// записи должны быть упорядочены
		else if(lDelRec < lCurFreeRec)
		{
			// вставляем запись в список

			if(i < max_rec_cnt)
			{
				DBF_CHAR* buf = new DBF_CHAR[(m_SMTHead.free_rec_count - i) * rec_size];

				// смещаем записи в право

				memcpy(buf, m_SMTHead.free_rec_ptr + i * rec_size,  (m_SMTHead.free_rec_count - i) * rec_size);
				memcpy(m_SMTHead.free_rec_ptr + (i + 1) * rec_size, buf, (m_SMTHead.free_rec_count - i) * rec_size);

				m_SMTHead.free_rec_count++;

				delete[] buf;
			}

			// вставляем запись в список

			// кол-во блоков в свободной записи
			memcpy(m_SMTHead.free_rec_ptr + i * rec_size, &nDelBlockCnt,  sizeof(nDelBlockCnt));
			// номер свободной записи
			memcpy(m_SMTHead.free_rec_ptr + i * rec_size + sizeof(nDelBlockCnt), &lDelRec, sizeof(lDelRec));

			WriteHeader();

			return;
		}
	}

	// если можно вставить еще запись
	if(m_SMTHead.free_rec_count < max_rec_cnt)
	{
		// добавляем свободную запись в список

		int i = m_SMTHead.free_rec_count;

		// кол-во блоков в свободной записи
		memcpy(m_SMTHead.free_rec_ptr + i * rec_size, &nDelBlockCnt,  sizeof(nDelBlockCnt));
		// номер свободной записи
		memcpy(m_SMTHead.free_rec_ptr + i * rec_size + sizeof(nDelBlockCnt), &lDelRec, sizeof(lDelRec));

		m_SMTHead.free_rec_count++;

		WriteHeader();
	}
}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

} END_CATCH

}

/* Пытаемся вставить новое мемо поле вместо удаленного */
BOOL CSMTMemoFile::UndeleteMemoRecord(DBF_ULONG& lOffsetRec, CString& strMemo)
{

TRY
{
	ReadHeader();

	DBF_LONG lLenMemo = strMemo.GetLength();
	// получаем кол-во блоков занимаемых мемо полем
	DBF_UINT nNeedBlockCnt = (DBF_UINT) ceil((double)lLenMemo / (double)m_SMTHead.block_size);
	// 2 байта на кол-во блоков в записи и 4 номер записи
	int rec_size = sizeof(DBF_UINT) + sizeof(DBF_ULONG);
	// максимальное кол-во записей в списке
	int max_rec_cnt = sizeof(m_SMTHead.free_rec_ptr)/rec_size;

	for(int i = 0; i < m_SMTHead.free_rec_count; i++)
	{
		DBF_UINT nCurFreeBlockCnt = 0;
		DBF_ULONG lCurFreeRec = 0;

		// получаем кол-во блоков в свободной записи
		memcpy(&nCurFreeBlockCnt, m_SMTHead.free_rec_ptr + i * rec_size, sizeof(nCurFreeBlockCnt));
		// получаем номер свободной записи
		memcpy(&lCurFreeRec, m_SMTHead.free_rec_ptr + i * rec_size + sizeof(nCurFreeBlockCnt), sizeof(lCurFreeRec));

		// если запись подходит по размеру (больше чем нужно)
		if(nCurFreeBlockCnt > nNeedBlockCnt)
		{
			lOffsetRec = (lCurFreeRec + nCurFreeBlockCnt - nNeedBlockCnt);

			Seek(lOffsetRec * m_SMTHead.block_size,CFile::begin);

			// записываем данные
			Write((LPCSTR)strMemo, strMemo.GetLength());

			nCurFreeBlockCnt -= nNeedBlockCnt;

			// меняем кол-во блоков в свободной записи
			memcpy(m_SMTHead.free_rec_ptr + i * rec_size, &nCurFreeBlockCnt,  sizeof(nCurFreeBlockCnt));

			WriteHeader();

			return TRUE;
		}
		// если запись подходит по размеру
		else if(nCurFreeBlockCnt == nNeedBlockCnt)
		{
			lOffsetRec = lCurFreeRec;

			Seek(lOffsetRec * m_SMTHead.block_size,CFile::begin);

			// записываем данные
			Write((LPCSTR)strMemo, strMemo.GetLength());

			// Удаляем запись из списка
			// если последняя запись
			if(i == m_SMTHead.free_rec_count - 1)
			{
				lCurFreeRec = 0;
				nCurFreeBlockCnt = 0;

				// кол-во блоков в свободной записи
				memcpy(m_SMTHead.free_rec_ptr + i * rec_size, &nCurFreeBlockCnt,  sizeof(nCurFreeBlockCnt));
				// номер свободной записи
				memcpy(m_SMTHead.free_rec_ptr + i * rec_size + sizeof(nCurFreeBlockCnt), &lCurFreeRec, sizeof(lCurFreeRec));
			}
			else
			{
				DBF_CHAR* buf = new DBF_CHAR[(m_SMTHead.free_rec_count - i - 1) * rec_size];

				// смещаем записи в лево
				memcpy(buf, m_SMTHead.free_rec_ptr + (i + 1) * rec_size,  (m_SMTHead.free_rec_count - i - 1) * rec_size);
				memcpy(m_SMTHead.free_rec_ptr + i * rec_size, buf, (m_SMTHead.free_rec_count - i - 1) * rec_size);

				delete[] buf;
			}

			m_SMTHead.free_rec_count--;

			WriteHeader();

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
