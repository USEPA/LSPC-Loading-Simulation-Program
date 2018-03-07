// Page93.h: interface for the CPage93 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_93_H__EE188935_5F15_11D4_B303_00104B9CFF37__INCLUDED_)
#define AFX_93_H__EE188935_5F15_11D4_B303_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

class CPage93 : public CPageAdd  
{
public:
	CPage93();
	CPage93(UINT nID);
	virtual ~CPage93();
	virtual void InitGrid();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );
};

#endif // !defined(AFX_93_H__EE188935_5F15_11D4_B303_00104B9CFF37__INCLUDED_)
