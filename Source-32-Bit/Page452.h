// Page452.h: interface for the CPage452 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE452_H__BB96B966_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE452_H__BB96B966_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage452 : public CPageAdd  
{
public:
	CPage452();
	CPage452(UINT nID);
	virtual ~CPage452();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

};

#endif // !defined(AFX_PAGE452_H__BB96B966_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
