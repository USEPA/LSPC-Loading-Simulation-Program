/////////////////////////////////////////////////////////////////////////////
/*
јвтор:              ƒолгачев ј.¬.
 оординаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_DBFFIELDS_H__INCLUDED_)
#define ALX_DBFFIELDS_H__INCLUDED_

#include "DBFDef.h"

////////////////////////////////////////////////////////////////////////
// CDBFFieldInfo

typedef struct DBFField
{
	CString			m_strName;	// им€ пол€
	TCHAR			m_chType;	// тип пол€
	unsigned short	m_nLen;		// ширина пол€
	unsigned char	m_nDec;		// дробна€ часть
} Field;

////////////////////////////////////////////////////////////////////////
// CDBFFields

class ALX_EXT_CLASS CDBFFields
{

friend class CDBFTableDef;
friend class CDBFRecordset;
friend class CDBFCursorset;
friend class CDBFMemCursorset;

public:
	CDBFFields();
	~CDBFFields();

	short GetFieldCount();							// кол-во полей в структуре
	FIELD_REC* GetFieldRec(int nIndexFld)  const;	// возвращает указатель на структуру пол€

	Field& operator[](int nIndexFld);
	Field& operator[](CString strNameFld);

private:

	FIELD_REC*	m_pFields;		// указатель на структуру полей
	Field		m_CurField;		// текущее поле
	short		m_nFieldCount;	// кол-во полей
	DBF_CHAR*	m_pCurRec;		// указатель на текущую запись
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_DBFFIELDS_H__INCLUDED_)

