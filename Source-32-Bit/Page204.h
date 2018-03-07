// Page204.h: interface for the CPage204 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE204_H__04763FFB_138F_4E16_A1B3_F80D321ADB5C__INCLUDED_)
#define AFX_PAGE204_H__04763FFB_138F_4E16_A1B3_F80D321ADB5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"
//Card 204 sets the grid for monthly ET coefficients

class CPage204 : public CPageAdd  
{
public:
	CPage204();
	CPage204(UINT nIDCap);
	virtual ~CPage204();
    virtual void InitGrid();
    virtual BOOL UpdateData(BOOL bSaveAndValidate = TRUE);
};

#endif // !defined(AFX_PAGE204_H__04763FFB_138F_4E16_A1B3_F80D321ADB5C__INCLUDED_)
