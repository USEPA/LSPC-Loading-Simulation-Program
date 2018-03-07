// DBFFields.cpp : implementation of the CDBFFields class
//
/////////////////////////////////////////////////////////////////////////////
/*
Автор:              Долгачев А.В.
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../INCLUDE/DBFFields.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////
// CDBFFields

/* Конструктор */
CDBFFields::CDBFFields()
{
	m_pCurRec = NULL;
	m_pFields = NULL;
	m_nFieldCount = 0;

	m_CurField.m_chType = '\0';
	m_CurField.m_nLen = 0;
	m_CurField.m_nDec = 0;
}

/* Деструктор */
CDBFFields::~CDBFFields()
{
}

/* Переопределяем операцию индекса */
Field& CDBFFields::operator[](int nIndexFld)
{
	ASSERT(nIndexFld < m_nFieldCount);

	// приводим структуру FIELD_REC к Field
	m_CurField.m_strName = m_pFields[nIndexFld].field_name;
	m_CurField.m_chType = m_pFields[nIndexFld].field_type;

	switch(m_CurField.m_chType)
	{
	case FLD_TYPE_CHARACTER:
	case FLD_TYPE_MEMO:
	case FLD_TYPE_GENERAL:
	case FLD_TYPE_PICTURE:
		{
			m_CurField.m_nLen = m_pFields[nIndexFld].len_info.char_len;
			m_CurField.m_nDec = 0;
			break;
		}
	case FLD_TYPE_CURRENCY:
		{
			m_CurField.m_nLen = m_pFields[nIndexFld].len_info.num_size.len;
			m_CurField.m_nDec = m_pFields[nIndexFld].len_info.num_size.dec;
			break;
		}
	case FLD_TYPE_NUMERIC:
	case FLD_TYPE_FLOAT:
		{
			m_CurField.m_nLen = m_pFields[nIndexFld].len_info.num_size.len;
			m_CurField.m_nDec = m_pFields[nIndexFld].len_info.num_size.dec;
			break;
		}
	case FLD_TYPE_DATE:
	case FLD_TYPE_DATETIME:
		{
			m_CurField.m_nLen = m_pFields[nIndexFld].len_info.char_len;
			m_CurField.m_nDec = 0;
			break;
		}
	case FLD_TYPE_DOUBLE: // или FLD_TYPE_BINARY
		{
			m_CurField.m_nLen = m_pFields[nIndexFld].len_info.num_size.len;
			m_CurField.m_nDec = m_pFields[nIndexFld].len_info.num_size.dec;
			break;
		}
	case FLD_TYPE_INTEGER:
		{
			m_CurField.m_nLen = m_pFields[nIndexFld].len_info.char_len;
			m_CurField.m_nDec = 0;
			break;
		}
	case FLD_TYPE_LOGICAL:
		{
			m_CurField.m_nLen = m_pFields[nIndexFld].len_info.char_len;
			m_CurField.m_nDec = 0;
			break;
		}
	default:
		{
			m_CurField.m_nLen = m_pFields[nIndexFld].len_info.char_len;
			m_CurField.m_nDec = 0;
		}
	}

	return m_CurField;
}

/* Переопределяем операцию индекса */
Field& CDBFFields::operator[](CString strNameFld)
{
	ASSERT(m_nFieldCount > 0);

	short i = 0;
	CString strCurFldName;

	do
	{
		strCurFldName = m_pFields[i].field_name;
	}
	while(strCurFldName != strNameFld && i <= m_nFieldCount);
	
	ASSERT(i < m_nFieldCount);

	return (*this)[i];
}

/* Функция возвращает количество полей */
short CDBFFields::GetFieldCount()
{
	return m_nFieldCount;
}

/* Функция возвращает указатель на структуру поля */
FIELD_REC* CDBFFields::GetFieldRec(int nIndexFld) const
{
	return &m_pFields[nIndexFld];
}
