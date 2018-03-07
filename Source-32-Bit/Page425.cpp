// Page425.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page425.h"
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

CPage425::CPage425()
{

}

CPage425::CPage425(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage425::~CPage425()
{

}

void CPage425::InitGrid()
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

	CString str[] = {"qualindex","qualname","dwqid","sqalfr"}; // column name
    UINT format[] = {DT_RIGHT,DT_LEFT,DT_RIGHT,DT_RIGHT}; // column format

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

BOOL CPage425::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;

	int Num = pDoc->lspc.nPtQuals;

	if(bSaveAndValidate)
	{
		if (pDoc->lspc.nPtQuals == 0)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != Num)
		{
			AfxMessageBox("Card 425: Number of rows should be equal to nPtQuals in Card 425!");
			Log("Card 425: Number of rows should be equal to nPtQuals in Card 425!");
			return false;
		}

		if(pDoc->lspc.pPtQual != NULL)
			delete []pDoc->lspc.pPtQual;

		// allocate memory here
		if (nRow > 0)
			pDoc->lspc.pPtQual = new PtQual[nRow];
		// read data
		for (int i=0; i<nRow; ++i)
		{
//			pDoc->lspc.pPtQual[i].qualindex = m_Grid.GetItemInt(i+1,0);	
			pDoc->lspc.pPtQual[i].qualindex = i+1;	
			pDoc->lspc.pPtQual[i].qualname = m_Grid.GetItemText(i+1,1);
			pDoc->lspc.pPtQual[i].qualid = m_Grid.GetItemInt(i+1,2);
			pDoc->lspc.pPtQual[i].sqalfr = m_Grid.GetItemFloat(i+1,3);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.pPtQual == NULL)
			return true;
		
		CSize size = m_Grid.GetTextExtent(" ");
		for(int i=0; i<Num; i++)
		{
			m_Grid.InsertRow(_T(""), -1);
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow, 0,pDoc->lspc.pPtQual[i].qualindex);
			m_Grid.SetItemText(nRow,1,pDoc->lspc.pPtQual[i].qualname);
			m_Grid.SetItemInt(nRow, 2,pDoc->lspc.pPtQual[i].qualid);
			m_Grid.SetItemFloat(nRow, 3,pDoc->lspc.pPtQual[i].sqalfr);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
