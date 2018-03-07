// Page461.h: interface for the CPage461 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE461_H__BB96B964_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
#define AFX_PAGE461_H__BB96B964_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage461 : public CPageAdd  
{
public:
	CPage461();
	CPage461(UINT nID);
	virtual ~CPage461();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );

};

#endif // !defined(AFX_PAGE461_H__BB96B964_4D33_11D4_B2FF_00104B9CFF37__INCLUDED_)
