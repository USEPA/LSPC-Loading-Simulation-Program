// Page670.h: interface for the CPage670 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE670_H__EE188934_5F15_11D4_B303_00104B9CFF37__INCLUDED_)
#define AFX_PAGE670_H__EE188934_5F15_11D4_B303_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage670 : public CPageAdd  
{
public:
	CPage670();
	CPage670(UINT nID);
	virtual ~CPage670();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
};

#endif // !defined(AFX_PAGE670_H__EE188934_5F15_11D4_B303_00104B9CFF37__INCLUDED_)
