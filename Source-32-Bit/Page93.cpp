// Page93.cpp: implementation of the CPage93 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page93.h"
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

CPage93::CPage93()
{

}

CPage93::CPage93(UINT nID) : CPageAdd(nID)
{

}

CPage93::~CPage93()
{

}

void CPage93::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(7);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] ={"defid","deluid","lat","melev","shade","snowcf","covind"}; // column name
	
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

BOOL CPage93::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;

	if(bSaveAndValidate)
	{
		if(pDoc->lspc.snowfg == 0)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != pDoc->lspc.nlandp*pDoc->lspc.ngroup )
		{
			AfxMessageBox("Card 93: Number of rows should be equal to nlandp*ngroup!");
			Log("Card 93: Number of rows should be equal to nlandp*ngroup!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.pSnowParam1 != NULL)
			delete [](pDoc->lspc.pSnowParam1);

		if (pDoc->lspc.nlandp*pDoc->lspc.ngroup > 0)
			pDoc->lspc.pSnowParam1 = new CSnowParam1[pDoc->lspc.nlandp*pDoc->lspc.ngroup];
		
		for(int i = 0; i < pDoc->lspc.nlandp*pDoc->lspc.ngroup; ++i)
		{
			pDoc->lspc.pSnowParam1[i].nGrpid = m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.pSnowParam1[i].luid   = m_Grid.GetItemInt(i+1,  1);
			pDoc->lspc.pSnowParam1[i].LAT    = m_Grid.GetItemFloat(i+1,2);
			pDoc->lspc.pSnowParam1[i].MELEV  = m_Grid.GetItemFloat(i+1,3);
			pDoc->lspc.pSnowParam1[i].SHADE  = m_Grid.GetItemFloat(i+1,4);
			pDoc->lspc.pSnowParam1[i].SNOWCF = m_Grid.GetItemFloat(i+1,5);
			pDoc->lspc.pSnowParam1[i].COVIND = m_Grid.GetItemFloat(i+1,6);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.pSnowParam1 == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < pDoc->lspc.nlandp*pDoc->lspc.ngroup; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0, pDoc->lspc.pSnowParam1[i].nGrpid);
			m_Grid.SetItemInt(nRow,  1, pDoc->lspc.pSnowParam1[i].luid);
			m_Grid.SetItemFloat(nRow,2, pDoc->lspc.pSnowParam1[i].LAT);
			m_Grid.SetItemFloat(nRow,3, pDoc->lspc.pSnowParam1[i].MELEV);
			m_Grid.SetItemFloat(nRow,4, pDoc->lspc.pSnowParam1[i].SHADE);
			m_Grid.SetItemFloat(nRow,5, pDoc->lspc.pSnowParam1[i].SNOWCF);
			m_Grid.SetItemFloat(nRow,6, pDoc->lspc.pSnowParam1[i].COVIND);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
