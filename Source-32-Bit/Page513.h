// Page513.h: interface for the CPage513 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE513_H__BB96B968_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE513_H__BB96B968_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage513 : public CPageAdd  
{
public:
	CPage513();
	CPage513(UINT nID);
	virtual ~CPage513();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

};

#endif // !defined(AFX_PAGE513_H__BB96B968_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
