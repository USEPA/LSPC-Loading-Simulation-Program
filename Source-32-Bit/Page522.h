// Page522.h: interface for the CPage522 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE522_H__BB96B967_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE522_H__BB96B967_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage522 : public CPageAdd  
{
public:
	CPage522();
	CPage522(UINT nID);
	virtual ~CPage522();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

};

#endif // !defined(AFX_PAGE522_H__BB96B967_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
