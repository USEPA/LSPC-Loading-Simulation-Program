// Page205.cpp: implementation of the CPage203 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page205.h"
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

CPage205::CPage205()
{

}

CPage205::CPage205(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage205::~CPage205()
{

}
void CPage205::InitGrid()
{
    m_Grid.SetRowCount(1);
    m_Grid.SetColumnCount(3);
    m_Grid.SetFixedRowCount(1);
    m_Grid.SetFixedColumnCount(0);

    CString str[] = {"subbasin", "rchid", "irrigdep"}; // column name
    UINT format[] = {DT_RIGHT, DT_RIGHT, DT_RIGHT}; // column format

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

BOOL CPage205::UpdateData(BOOL bSaveAndValidate)
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
        if(pDoc->lspc.irrigfg != 1)
			return true;
		
		int nRow = m_Grid.GetRowCount() - 1;
        if(nRow != pDoc->lspc.nsws ) //*pDoc->lspc.nsws)
        {
            AfxMessageBox("Card 205: Number of rows should be equal to nsws in Card 205!");
            Log("Card 205: Number of rows should be equal to nsws in Card 205!");
            return false;
        }

        // delete the old one
        if(pDoc->lspc.pirrigwdinfo != NULL)
            delete []pDoc->lspc.pirrigwdinfo;

		if (pDoc->lspc.nsws > 0)
			pDoc->lspc.pirrigwdinfo = new IrrigWDInfo[pDoc->lspc.nsws];

        for(int i=0; i<nRow; i++)
        {
            pDoc->lspc.pirrigwdinfo[i].wsid      = m_Grid.GetItemInt(i+1,  0);
            pDoc->lspc.pirrigwdinfo[i].streamid  = m_Grid.GetItemInt(i+1,  1);
            pDoc->lspc.pirrigwdinfo[i].irrigdep  = m_Grid.GetItemFloat(i+1,2);
//			pDoc->lspc.pirrigwdinfo[i].extsrc = true;
        }
    }
    else // fill the blanks with data
    {
        InitGrid();

        if(pDoc->lspc.pirrigwdinfo == NULL)
            return true;

        CSize size = m_Grid.GetTextExtent(" ");
        for(int i=0; i<pDoc->lspc.nsws; i++)
        {
            m_Grid.InsertRow(_T(""), -1);
            int nRow = m_Grid.GetRowCount() - 1;
            m_Grid.SetRowHeight(nRow, size.cy);
            m_Grid.SetItemInt(nRow, 0,pDoc->lspc.pirrigwdinfo[i].wsid);
            m_Grid.SetItemInt(nRow, 1,pDoc->lspc.pirrigwdinfo[i].streamid);
            m_Grid.SetItemFloat(nRow, 2,pDoc->lspc.pirrigwdinfo[i].irrigdep);
        }

        m_Grid.Invalidate();
        m_Grid.AutoSize();
    }
    return TRUE;
}
