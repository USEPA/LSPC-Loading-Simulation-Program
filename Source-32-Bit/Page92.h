// Page92.h: interface for the CPage92 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_92_H__EE188935_5F15_11D4_B303_00104B9CFF37__INCLUDED_)
#define AFX_92_H__EE188935_5F15_11D4_B303_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage92 : public CPageAdd  
{
public:
	CPage92();
	CPage92(UINT nID);
	virtual ~CPage92();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
};

#endif // !defined(AFX_92_H__EE188935_5F15_11D4_B303_00104B9CFF37__INCLUDED_)
