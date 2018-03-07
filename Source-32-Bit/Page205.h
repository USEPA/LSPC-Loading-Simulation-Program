// Page205.h: interface for the CPage203 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGE205_H__044124AD_1095_410C_B216_77DF3066CBA3__INCLUDED_)
#define AFX_PAGE205_H__044124AD_1095_410C_B216_77DF3066CBA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageAdd.h"

//Card 205 sets the grid for setting the source of water withdrawal for each watershed

class CPage205  : public CPageAdd
{
public:
	CPage205();
	CPage205(UINT nIDCap);
	virtual ~CPage205();
    virtual void InitGrid();
    virtual BOOL UpdateData(BOOL bSaveAndValidate = TRUE);
};


#endif // !defined(AFX_PAGE205_H__044124AD_1095_410C_B216_77DF3066CBA3__INCLUDED_)
