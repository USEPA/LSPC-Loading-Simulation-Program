// Page531.h: interface for the CPage531 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE531_H__BB96B966_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE531_H__BB96B966_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage531 : public CPageAdd  
{
public:
	CPage531();
	CPage531(UINT nID);
	virtual ~CPage531();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

};

#endif // !defined(AFX_PAGE531_H__BB96B966_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
