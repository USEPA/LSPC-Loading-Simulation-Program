// Page203.cpp: implementation of the CPage203 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page203.h"
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

CPage203::CPage203()
{

}

CPage203::CPage203(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage203::~CPage203()
{

}


void CPage203::InitGrid()
{
    m_Grid.SetRowCount(1);
    m_Grid.SetColumnCount(11);
    m_Grid.SetFixedRowCount(1);
    m_Grid.SetFixedColumnCount(0);

    CString str[] = {"defid","deluid","start month", "end month","fraction 1", "fraction 2", "fraction 3", "fraction 4", "fraction 5","constant et coefficient","et days"}; // column name
    UINT format[] = {DT_RIGHT, DT_RIGHT,DT_RIGHT,DT_RIGHT, DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT, DT_RIGHT}; // column format

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

BOOL CPage203::UpdateData(BOOL bSaveAndValidate)
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

		if(nRow != pDoc->lspc.ngroup*pDoc->lspc.nlandp)
		{
			AfxMessageBox("Card 203: Number of rows should be equal to ngroup*nlandp in Card 203!");
			Log("Card 203: Number of rows should be equal to ngroup*nlandp in Card 203!");
			return false;
		}

        // delete the old one
        if(pDoc->lspc.pirriginfop != NULL)
            delete []pDoc->lspc.pirriginfop;

		if (pDoc->lspc.ngroup*pDoc->lspc.nlandp > 0)
			pDoc->lspc.pirriginfop = new IrrigInfo[pDoc->lspc.ngroup*pDoc->lspc.nlandp];

        for(int i=0; i<nRow; i++)
        {
            pDoc->lspc.pirriginfop[i].gid       = m_Grid.GetItemInt(i+1,  0);
            pDoc->lspc.pirriginfop[i].luid      = m_Grid.GetItemInt(i+1,  1);
			pDoc->lspc.pirriginfop[i].startmonth= m_Grid.GetItemInt(i+1,  2);
			pDoc->lspc.pirriginfop[i].endmonth  = m_Grid.GetItemInt(i+1,  3);
            pDoc->lspc.pirriginfop[i].fraction1 = m_Grid.GetItemFloat(i+1,4);
            pDoc->lspc.pirriginfop[i].fraction2 = m_Grid.GetItemFloat(i+1,5);
            pDoc->lspc.pirriginfop[i].fraction3 = m_Grid.GetItemFloat(i+1,6);
            pDoc->lspc.pirriginfop[i].fraction4 = m_Grid.GetItemFloat(i+1,7);
            pDoc->lspc.pirriginfop[i].fraction5 = m_Grid.GetItemFloat(i+1,8);
            pDoc->lspc.pirriginfop[i].etcoeff   = m_Grid.GetItemFloat(i+1,9);
			pDoc->lspc.pirriginfop[i].etdays    = m_Grid.GetItemInt(i+1, 10);
        }
    }
    else // fill the blanks with data
    {
        InitGrid();

        if(pDoc->lspc.pirriginfop == NULL)
            return true;

        CSize size = m_Grid.GetTextExtent(" ");
        for(int i=0; i<pDoc->lspc.ngroup*pDoc->lspc.nlandp; i++)
        {
            m_Grid.InsertRow(_T(""), -1);
            int nRow = m_Grid.GetRowCount() - 1;
            m_Grid.SetRowHeight(nRow, size.cy);
            m_Grid.SetItemInt(nRow,  0,pDoc->lspc.pirriginfop[i].gid);
            m_Grid.SetItemInt(nRow,  1,pDoc->lspc.pirriginfop[i].luid);
            m_Grid.SetItemInt(nRow,  2,pDoc->lspc.pirriginfop[i].startmonth);
            m_Grid.SetItemInt(nRow,  3,pDoc->lspc.pirriginfop[i].endmonth);
            m_Grid.SetItemFloat(nRow,4,pDoc->lspc.pirriginfop[i].fraction1);
            m_Grid.SetItemFloat(nRow,5,pDoc->lspc.pirriginfop[i].fraction2);
            m_Grid.SetItemFloat(nRow,6,pDoc->lspc.pirriginfop[i].fraction3);
            m_Grid.SetItemFloat(nRow,7,pDoc->lspc.pirriginfop[i].fraction4);
            m_Grid.SetItemFloat(nRow,8,pDoc->lspc.pirriginfop[i].fraction5);
            m_Grid.SetItemFloat(nRow,9,pDoc->lspc.pirriginfop[i].etcoeff);
			m_Grid.SetItemInt(nRow, 10,pDoc->lspc.pirriginfop[i].etdays);
        }

        m_Grid.Invalidate();
        m_Grid.AutoSize();
    }
    return TRUE;
}
