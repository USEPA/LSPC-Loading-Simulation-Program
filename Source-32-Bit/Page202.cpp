// Page202.cpp: implementation of the CPage202 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page202.h"
#include "MainFrm.h"
#include "LSPCView.h"
#include "LSPCDoc.h"
#include "LSPCMODEL.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPage202::CPage202()
{

}

CPage202::CPage202(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage202::~CPage202()
{

}


void CPage202::InitGrid()
{
    m_Grid.SetRowCount(1);
    m_Grid.SetColumnCount(2);
    m_Grid.SetFixedRowCount(1);
    m_Grid.SetFixedColumnCount(0);

    CString str[] = {"defid","pet value"}; // column name
    UINT format[] = {DT_RIGHT,DT_RIGHT}; // column format

    GV_ITEM Item;
    Item.mask = GVIF_TEXT|GVIF_FORMAT;
    Item.row = 0;

    for(int col=0; col<m_Grid.GetColumnCount(); col++)
    {
        Item.col = col;
        Item.nFormat = format[col]|DT_VCENTER|DT_SINGLELINE;
        Item.szText = str[col];
        m_Grid.SetItem(&Item);
    }
    m_Grid.AutoSize();
}

BOOL CPage202::UpdateData(BOOL bSaveAndValidate)
{
    // set the active page
    CMainFrame *pMain = (CMainFrame *) AfxGetMainWnd();
    CLSPCView *pView = (CLSPCView *) pMain->m_wndSplitter.GetPane(0, 1);
    if(pView == NULL)
        return false;

    CLSPCDoc *pDoc = pView->GetDocument();
    if(pDoc == NULL)
        return false;

    if(bSaveAndValidate)
    {
		if(pDoc->lspc.irrigfg != 1 || pDoc->lspc.petfg != 1)
			return true;

        int nRow = m_Grid.GetRowCount() - 1;

		if(nRow != pDoc->lspc.ngroup)
		{
			AfxMessageBox("Card 202: Number of rows should be equal to ngroup in Card 202!");
			Log("Card 202: Number of rows should be equal to ngroup in Card 202!");
			return false;
		}

        // delete the old one
        if(pDoc->lspc.pirrigpet != NULL)
            delete []pDoc->lspc.pirrigpet;

		if (pDoc->lspc.ngroup > 0)
			pDoc->lspc.pirrigpet = new IrrigPET[pDoc->lspc.ngroup];

        for(int i=0; i<nRow; i++)
        {
            pDoc->lspc.pirrigpet[i].gid    = m_Grid.GetItemInt(i+1,  0);
            pDoc->lspc.pirrigpet[i].petval = m_Grid.GetItemFloat(i+1,1);
        }
    }
    else // fill the blanks with data
    {
        InitGrid();

        if(pDoc->lspc.pirrigpet == NULL)
            return true;

        CSize size = m_Grid.GetTextExtent(" ");
        for(int i=0; i<pDoc->lspc.ngroup; i++)
        {
            m_Grid.InsertRow(_T(""), -1);
            int nRow = m_Grid.GetRowCount() - 1;
            m_Grid.SetRowHeight(nRow, size.cy);
            m_Grid.SetItemInt(nRow,  0,pDoc->lspc.pirrigpet[i].gid);
            m_Grid.SetItemFloat(nRow,1,pDoc->lspc.pirrigpet[i].petval);
        }
        m_Grid.Invalidate();
        m_Grid.AutoSize();
    }
    return TRUE;
}
