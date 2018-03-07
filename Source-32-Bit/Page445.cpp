// Page445.cpp: implementation of the CPage445 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page445.h"
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

CPage445::CPage445()
{

}

CPage445::CPage445(UINT nID)
:CPageAdd(nID)
{
}


CPage445::~CPage445()
{

}

void CPage445::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(10);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] = {"defid","deluid","luname","start_month","end_month","frequency","percent_area","effic_sand","effic_silt","effic_clay"}; // column name

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

BOOL CPage445::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;
	
	int nNum = pDoc->lspc.nlandimp*pDoc->lspc.ngroup;
	
	if(bSaveAndValidate)
	{
		if(pDoc->lspc.sedfg != 1)
			return true;
		
		if(pDoc->lspc.sweepfg == 0)
			return true;
		
		int nRow = m_Grid.GetRowCount() - 1;
		
		if(nRow != nNum)
		{
			AfxMessageBox("Card 445: Number of rows should be equal to nlandimp*ngroup!");
			Log("Card 445: Number of rows should be equal to nlandimp*ngroup!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.psd_sweep != NULL)
			delete [](pDoc->lspc.psd_sweep);

		if (nNum > 0)
			pDoc->lspc.psd_sweep = new SD_SWEEP[nNum];
		for(int i=0; i<nNum; ++i)
		{
			pDoc->lspc.psd_sweep[i].defid        = m_Grid.GetItemInt(i+1,0);
			pDoc->lspc.psd_sweep[i].deluid       = m_Grid.GetItemInt(i+1,1);
			pDoc->lspc.psd_sweep[i].luname       = m_Grid.GetItemText(i+1,2);
			pDoc->lspc.psd_sweep[i].start_month  = m_Grid.GetItemInt(i+1,3);
			pDoc->lspc.psd_sweep[i].end_month    = m_Grid.GetItemInt(i+1,4);
			pDoc->lspc.psd_sweep[i].frequency    = m_Grid.GetItemFloat(i+1,5);
			pDoc->lspc.psd_sweep[i].percent_area = m_Grid.GetItemFloat(i+1,6);
			pDoc->lspc.psd_sweep[i].effic_sand   = m_Grid.GetItemFloat(i+1,7);
			pDoc->lspc.psd_sweep[i].effic_silt   = m_Grid.GetItemFloat(i+1,8);
			pDoc->lspc.psd_sweep[i].effic_clay   = m_Grid.GetItemFloat(i+1,9);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.psd_sweep == NULL)
			return true;
		
		CSize size = m_Grid.GetTextExtent(" ");
		for(int i=0; i<nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow,size.cy);
			m_Grid.SetItemInt(nRow,0, pDoc->lspc.psd_sweep[i].defid);
			m_Grid.SetItemInt(nRow,1, pDoc->lspc.psd_sweep[i].deluid);
			m_Grid.SetItemText(nRow,2, pDoc->lspc.psd_sweep[i].luname);
			m_Grid.SetItemInt(nRow,3, pDoc->lspc.psd_sweep[i].start_month);
			m_Grid.SetItemInt(nRow,4, pDoc->lspc.psd_sweep[i].end_month);
			m_Grid.SetItemFloat(nRow,5, pDoc->lspc.psd_sweep[i].frequency); 
			m_Grid.SetItemFloat(nRow,6, pDoc->lspc.psd_sweep[i].percent_area); 
			m_Grid.SetItemFloat(nRow,7, pDoc->lspc.psd_sweep[i].effic_sand);
			m_Grid.SetItemFloat(nRow,8, pDoc->lspc.psd_sweep[i].effic_silt);
			m_Grid.SetItemFloat(nRow,9, pDoc->lspc.psd_sweep[i].effic_clay);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
