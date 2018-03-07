// Page456.cpp: implementation of the CPage456 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page456.h"
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

CPage456::CPage456()
{

}

CPage456::CPage456(UINT nID) : CPageAdd(nID)
{

}

CPage456::~CPage456()
{

}

void CPage456::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(12);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] ={"rgid","sed_id","sedflg","sedo","sedfrac","db50/d","w","rho","ksand/taucd","expsnd/taucs","m","burial"}; // column name

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

BOOL CPage456::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;
	
	int nNum = pDoc->lspc.nrgroup*pDoc->lspc.NumSed;

	if(bSaveAndValidate)
	{
		if(pDoc->lspc.sedfg  != 1)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 456: Number of rows should be equal to number of sediment groups * number of stream groups in Card 456!");
			Log("Card 456: Number of rows should be equal to NumSed*nrgroup in Card 456!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.rsd_param != NULL)
			delete [](pDoc->lspc.rsd_param);

		if (nNum > 0)
			pDoc->lspc.rsd_param = new STREAMSED_PARM[nNum];
		
		for(int i=0; i<nNum; ++i)
		{
			pDoc->lspc.rsd_param[i].gid		= m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.rsd_param[i].sed_id	= m_Grid.GetItemInt(i+1,  1);
			pDoc->lspc.rsd_param[i].sedflg	= m_Grid.GetItemInt(i+1,  2);
			pDoc->lspc.rsd_param[i].sedo	= m_Grid.GetItemFloat(i+1,3);
			pDoc->lspc.rsd_param[i].sedfrac	= m_Grid.GetItemFloat(i+1,4);
			pDoc->lspc.rsd_param[i].d		= m_Grid.GetItemFloat(i+1,5);
			pDoc->lspc.rsd_param[i].w		= m_Grid.GetItemFloat(i+1,6);
			pDoc->lspc.rsd_param[i].rho		= m_Grid.GetItemFloat(i+1,7);
			pDoc->lspc.rsd_param[i].taucd	= m_Grid.GetItemFloat(i+1,8);
			pDoc->lspc.rsd_param[i].taucs	= m_Grid.GetItemFloat(i+1,9);
			pDoc->lspc.rsd_param[i].m		= m_Grid.GetItemFloat(i+1,10);
			pDoc->lspc.rsd_param[i].burial	= m_Grid.GetItemFloat(i+1,11);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.rsd_param == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i=0; i<nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0, pDoc->lspc.rsd_param[i].gid);
			m_Grid.SetItemInt(nRow,  1, pDoc->lspc.rsd_param[i].sed_id);
			m_Grid.SetItemInt(nRow,  2, pDoc->lspc.rsd_param[i].sedflg);
			m_Grid.SetItemFloat(nRow,3, pDoc->lspc.rsd_param[i].sedo); 
			m_Grid.SetItemFloat(nRow,4, pDoc->lspc.rsd_param[i].sedfrac);
			m_Grid.SetItemFloat(nRow,5, pDoc->lspc.rsd_param[i].d); 
			m_Grid.SetItemFloat(nRow,6, pDoc->lspc.rsd_param[i].w); 
			m_Grid.SetItemFloat(nRow,7, pDoc->lspc.rsd_param[i].rho);
			m_Grid.SetItemFloat(nRow,8, pDoc->lspc.rsd_param[i].taucd); 
			m_Grid.SetItemFloat(nRow,9, pDoc->lspc.rsd_param[i].taucs); 
			m_Grid.SetItemFloat(nRow,10,pDoc->lspc.rsd_param[i].m); 
			m_Grid.SetItemFloat(nRow,11,pDoc->lspc.rsd_param[i].burial); 
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
