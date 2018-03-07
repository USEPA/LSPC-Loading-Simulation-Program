// Page202.h: interface for the CPage202 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE202_H__964EF2EE_B0F7_438A_BD1B_9662BA803809__INCLUDED_)
#define AFX_PAGE202_H__964EF2EE_B0F7_438A_BD1B_9662BA803809__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"
//Card 202 sets the grid for inputing irrigation parameters

class CPage202  : public CPageAdd
{
public:
	CPage202();
    CPage202(UINT nIDCap);
    virtual ~CPage202();
    virtual void InitGrid();
    virtual BOOL UpdateData(BOOL bSaveAndValidate = TRUE);
};

#endif // !defined(AFX_PAGE202_H__964EF2EE_B0F7_438A_BD1B_9662BA803809__INCLUDED_)
