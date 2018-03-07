// Page450.cpp: implementation of the CPage450 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page450.h"
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

CPage450::CPage450()
{

}

CPage450::CPage450(UINT nID)
:CPageAdd(nID)
{
}


CPage450::~CPage450()
{

}

void CPage450::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(14);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] = {"defid","deluid","smpf","krer","jrer","affix","cover","nvsi","kser","jser","kger","jger","accsdp","remsdp"}; // column name

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

BOOL CPage450::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;
	
	int nNum = pDoc->lspc.nlandp*pDoc->lspc.ngroup;
	if(bSaveAndValidate)
	{
		if(pDoc->lspc.sedfg != 1)
			return true;
		
		int nRow = m_Grid.GetRowCount() - 1;
		
		if(nRow != nNum)
		{
			AfxMessageBox("Card 450: Number of rows should be equal to nlandp*ngroup in Card 450!");
			Log("Card 450: Number of rows should be equal to nlandp*ngroup in Card 450!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.psd_param1 != NULL)
			delete [](pDoc->lspc.psd_param1);

		if (nNum > 0)
			pDoc->lspc.psd_param1 = new SD_PARA1[nNum];
		for(int i = 0; i < nNum; ++i)
		{
			pDoc->lspc.psd_param1[i].gid      = m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.psd_param1[i].lid      = m_Grid.GetItemInt(i+1,  1);
			pDoc->lspc.psd_param1[i].smpf_p   = m_Grid.GetItemFloat(i+1,2);
			pDoc->lspc.psd_param1[i].krer_p   = m_Grid.GetItemFloat(i+1,3);
			pDoc->lspc.psd_param1[i].jrer_p   = m_Grid.GetItemFloat(i+1,4);
			pDoc->lspc.psd_param1[i].affix_p  = m_Grid.GetItemFloat(i+1,5);
			pDoc->lspc.psd_param1[i].cover_p  = m_Grid.GetItemFloat(i+1,6);
			pDoc->lspc.psd_param1[i].nvsi_p   = m_Grid.GetItemFloat(i+1,7);
			pDoc->lspc.psd_param1[i].kser_p   = m_Grid.GetItemFloat(i+1,8);
			pDoc->lspc.psd_param1[i].jser_p   = m_Grid.GetItemFloat(i+1,9);
			pDoc->lspc.psd_param1[i].kger_p   = m_Grid.GetItemFloat(i+1,10);
			pDoc->lspc.psd_param1[i].jger_p   = m_Grid.GetItemFloat(i+1,11);
			pDoc->lspc.psd_param1[i].accsdp_p = m_Grid.GetItemFloat(i+1,12);
			pDoc->lspc.psd_param1[i].remsdp_p = m_Grid.GetItemFloat(i+1,13);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.psd_param1 == NULL)
			return true;
		
		CSize size = m_Grid.GetTextExtent(" ");
		for(int i=0; i < nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow,size.cy);
			m_Grid.SetItemInt(nRow,  0, pDoc->lspc.psd_param1[i].gid);
			m_Grid.SetItemInt(nRow,  1, pDoc->lspc.psd_param1[i].lid);
			m_Grid.SetItemFloat(nRow,2, pDoc->lspc.psd_param1[i].smpf_p);
			m_Grid.SetItemFloat(nRow,3, pDoc->lspc.psd_param1[i].krer_p);
			m_Grid.SetItemFloat(nRow,4, pDoc->lspc.psd_param1[i].jrer_p);
			m_Grid.SetItemFloat(nRow,5, pDoc->lspc.psd_param1[i].affix_p); 
			m_Grid.SetItemFloat(nRow,6, pDoc->lspc.psd_param1[i].cover_p); 
			m_Grid.SetItemFloat(nRow,7, pDoc->lspc.psd_param1[i].nvsi_p);
			m_Grid.SetItemFloat(nRow,8, pDoc->lspc.psd_param1[i].kser_p);
			m_Grid.SetItemFloat(nRow,9, pDoc->lspc.psd_param1[i].jser_p);
			m_Grid.SetItemFloat(nRow,10, pDoc->lspc.psd_param1[i].kger_p);
			m_Grid.SetItemFloat(nRow,11,pDoc->lspc.psd_param1[i].jger_p);
			m_Grid.SetItemFloat(nRow,12,pDoc->lspc.psd_param1[i].accsdp_p);
			m_Grid.SetItemFloat(nRow,13,pDoc->lspc.psd_param1[i].remsdp_p);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
