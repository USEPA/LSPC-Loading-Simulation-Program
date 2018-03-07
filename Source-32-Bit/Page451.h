// Page451.h: interface for the CPage451 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE451_H__BB96B963_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE451_H__BB96B963_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage451 : public CPageAdd  
{
public:
	CPage451();
	CPage451(UINT nID);
	virtual ~CPage451();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

};

#endif // !defined(AFX_PAGE451_H__BB96B963_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
