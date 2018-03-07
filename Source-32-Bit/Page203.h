// Page203.h: interface for the CPage203 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGEIRRIGAPPLN_H__964EF2EE_B0F7_438A_BD1B_9662BA803809__INCLUDED_)
#define AFX_PAGE203_H__964EF2EE_B0F7_438A_BD1B_9662BA803809__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"
//Card 203 sets the grid for inputing irrigation parameters

class CPage203  : public CPageAdd
{
public:
	CPage203();
    CPage203(UINT nIDCap);
    virtual ~CPage203();
    virtual void InitGrid();
    virtual BOOL UpdateData(BOOL bSaveAndValidate = TRUE);
};

#endif // !defined(AFX_PAGE203_H__964EF2EE_B0F7_438A_BD1B_9662BA803809__INCLUDED_)
