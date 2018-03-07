// Page533.h: interface for the CPage533 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE533_H__BB96B968_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE533_H__BB96B968_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage533 : public CPageAdd  
{
public:
	CPage533();
	CPage533(UINT nID);
	virtual ~CPage533();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

};

#endif // !defined(AFX_PAGE533_H__BB96B968_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
