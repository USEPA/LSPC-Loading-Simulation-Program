// Page446.h: interface for the CPage446 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE446_H__EE188934_5F15_11D4_B303_00104B9CFF37__INCLUDED_)
#define AFX_PAGE446_H__EE188934_5F15_11D4_B303_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage446 : public CPageAdd  
{
public:
	CPage446();
	CPage446(UINT nID);
	virtual ~CPage446();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
};

#endif // !defined(AFX_PAGE446_H__EE188934_5F15_11D4_B303_00104B9CFF37__INCLUDED_)
