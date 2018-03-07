// Page92.cpp: implementation of the CPage92 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page92.h"
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

CPage92::CPage92()
{

}

CPage92::CPage92(UINT nID) : CPageAdd(nID)
{

}

CPage92::~CPage92()
{

}

void CPage92::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(6);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] ={"defid","deluid","iceflag","forest","fzg","fzgl"}; // column name

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
	
	for (int col = 0; col < m_Grid.GetColumnCount(); col++)
	{
		Item.col = col;
		Item.szText = str[col];
		m_Grid.SetItem(&Item);
	}
	m_Grid.AutoSize();
}

BOOL CPage92::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return true;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return true;
	
	if(bSaveAndValidate)
	{
		if(pDoc->lspc.snowfg == 0)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != pDoc->lspc.nlandp*pDoc->lspc.ngroup )
		{
			AfxMessageBox("Card 92: Number of rows should be equal to nlandp*ngroup!");
			Log("Card 92: Number of rows should be equal to nlandp*ngroup!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.pSnowCtl2 != NULL)
			delete [](pDoc->lspc.pSnowCtl2);
		
		if (pDoc->lspc.nlandp*pDoc->lspc.ngroup > 0)
			pDoc->lspc.pSnowCtl2 = new CSnowControl2[pDoc->lspc.nlandp*pDoc->lspc.ngroup];
		
		for(int i = 0; i < pDoc->lspc.nlandp*pDoc->lspc.ngroup; ++i)
		{
			pDoc->lspc.pSnowCtl2[i].nGrpid   = m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.pSnowCtl2[i].luid     = m_Grid.GetItemInt(i+1,  1);
			pDoc->lspc.pSnowCtl2[i].ICEFLAG  = m_Grid.GetItemInt(i+1,  2);
			pDoc->lspc.pSnowCtl2[i].lfForest = m_Grid.GetItemFloat(i+1,3);
			pDoc->lspc.pSnowCtl2[i].fzg      = m_Grid.GetItemFloat(i+1,4);
			pDoc->lspc.pSnowCtl2[i].fzgl     = m_Grid.GetItemFloat(i+1,5);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.pSnowCtl2 == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < pDoc->lspc.nlandp*pDoc->lspc.ngroup; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0,pDoc->lspc.pSnowCtl2[i].nGrpid);
			m_Grid.SetItemInt(nRow,  1,pDoc->lspc.pSnowCtl2[i].luid);
			m_Grid.SetItemInt(nRow,  2,pDoc->lspc.pSnowCtl2[i].ICEFLAG);
			m_Grid.SetItemFloat(nRow,3,pDoc->lspc.pSnowCtl2[i].lfForest);
			m_Grid.SetItemFloat(nRow,4,pDoc->lspc.pSnowCtl2[i].fzg);
			m_Grid.SetItemFloat(nRow,5,pDoc->lspc.pSnowCtl2[i].fzgl);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
