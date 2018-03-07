// Page204.cpp: implementation of the CPage204 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page204.h"
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

CPage204::CPage204()
{

}

CPage204::CPage204(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage204::~CPage204()
{

}

void CPage204::InitGrid()
{
    m_Grid.SetRowCount(1);
    m_Grid.SetColumnCount(14);
    m_Grid.SetFixedRowCount(1);
    m_Grid.SetFixedColumnCount(0);

 	CString str[] ={"defid","deluid","jan","feb","mar","apr","may","jun","jul","aug","sept","oct","nov","dec"}; // column name
    UINT format[] = {DT_RIGHT, DT_RIGHT,DT_RIGHT,DT_RIGHT, DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT, DT_RIGHT,DT_RIGHT,DT_RIGHT, DT_RIGHT}; // column format

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

BOOL CPage204::UpdateData(BOOL bSaveAndValidate)
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
		if(pDoc->lspc.irrigfg == 0 || pDoc->lspc.monVaryIrrig == 0 )
			return true;

        int nRow = m_Grid.GetRowCount() - 1;

		if(nRow != pDoc->lspc.ngroup*pDoc->lspc.nlandp)
		{
			AfxMessageBox("Card 204: Number of rows should be equal to ngroup*nlandp in Card 204!");
			Log("Card 204: Number of rows should be equal to ngroup*nlandp in Card 204!");
			return false;
		}



        // delete the old one
        if(pDoc->lspc.pmonetcinfop != NULL)
            delete []pDoc->lspc.pmonetcinfop;

		if (pDoc->lspc.ngroup*pDoc->lspc.nlandp > 0)
			pDoc->lspc.pmonetcinfop = new MonETCInfo[pDoc->lspc.ngroup*pDoc->lspc.nlandp];

        for(int i=0; i<nRow; i++)
        {
            pDoc->lspc.pmonetcinfop[i].gid		= m_Grid.GetItemInt(i+1,   0);
            pDoc->lspc.pmonetcinfop[i].luid      = m_Grid.GetItemInt(i+1,  1);
			for(int mon=0; mon < 12; mon++)
				pDoc->lspc.pmonetcinfop[i].monETCs[mon] = m_Grid.GetItemFloat(i+1, 2+mon);
        }
    }
    else // fill the blanks with data
    {
        InitGrid();

        if(pDoc->lspc.pmonetcinfop == NULL)
            return true;

        CSize size = m_Grid.GetTextExtent(" ");
        for(int i=0; i<pDoc->lspc.ngroup*pDoc->lspc.nlandp; i++)
        {
            m_Grid.InsertRow(_T(""), -1);
            int nRow = m_Grid.GetRowCount() - 1;
            m_Grid.SetRowHeight(nRow, size.cy);
            m_Grid.SetItemInt(nRow,  0,pDoc->lspc.pirriginfop[i].gid);
            m_Grid.SetItemInt(nRow, 1,pDoc->lspc.pirriginfop[i].luid);
			for(int mon=0; mon < 12 ; mon++)
				m_Grid.SetItemFloat(nRow,2+mon,pDoc->lspc.pmonetcinfop[i].monETCs[mon]);
        }

        m_Grid.Invalidate();
        m_Grid.AutoSize();
    }
    return TRUE;
}