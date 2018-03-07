// Page430.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page430.h"
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

CPage430::CPage430()
{

}

CPage430::CPage430(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage430::~CPage430()
{

}

void CPage430::InitGrid()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return ;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;

	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(4);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] = {"subbasin","permit","pipe","wd_target"}; // column name
    UINT format[] = {DT_RIGHT,DT_LEFT,DT_LEFT,DT_RIGHT}; // column format

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

BOOL CPage430::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;

	if (pDoc->lspc.nPtSource == 0)
		return true;

	int Num = pDoc->lspc.nPtWithDrawal;
	if (Num <= 0)
		return true;

	if(bSaveAndValidate)
	{
		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != Num)
		{
			AfxMessageBox("Card 430: Number of rows should be equal to nPtWithDrawal in Card 430!");
			Log("Card 430: Number of rows should be equal to nPtWithDrawal in Card 430!");
			return false;
		}

		if(pDoc->lspc.pPtWithDrawal != NULL)
			delete []pDoc->lspc.pPtWithDrawal;

		// allocate memory here
		if (nRow > 0)
			pDoc->lspc.pPtWithDrawal = new PtWithDrawal[nRow];
		// read data
		for (int i=0; i<nRow; ++i)
		{
			pDoc->lspc.pPtWithDrawal[i].wdrawlrchid = m_Grid.GetItemInt(i+1,0);	
			pDoc->lspc.pPtWithDrawal[i].strPermit  = m_Grid.GetItemText(i+1,1);
			pDoc->lspc.pPtWithDrawal[i].strPipe    = m_Grid.GetItemText(i+1,2);
			pDoc->lspc.pPtWithDrawal[i].targetrchid = m_Grid.GetItemInt(i+1,3);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.pPtWithDrawal == NULL)
			return true;
		
		CSize size = m_Grid.GetTextExtent(" ");
		for(int i=0; i<Num; i++)
		{
			m_Grid.InsertRow(_T(""), -1);
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow, 0,pDoc->lspc.pPtWithDrawal[i].wdrawlrchid);
			m_Grid.SetItemText(nRow,1,pDoc->lspc.pPtWithDrawal[i].strPermit);
			m_Grid.SetItemText(nRow,2,pDoc->lspc.pPtWithDrawal[i].strPipe);
			m_Grid.SetItemInt(nRow, 3,pDoc->lspc.pPtWithDrawal[i].targetrchid);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
