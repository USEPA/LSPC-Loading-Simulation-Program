// Page475.cpp: implementation of the CPage475 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page475.h"
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

CPage475::CPage475()
{

}

CPage475::CPage475(UINT nID)
:CPageAdd(nID)
{
}

CPage475::~CPage475()
{

}

void CPage475::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(5);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] = {"rgid","cfsaex","katrad","kcond","kevap"}; // column name
	UINT format[] = {DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT};

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;

	for (int col = 0; col < m_Grid.GetColumnCount(); col++)
	{ 
		Item.col = col;
		Item.nFormat = format[col]|DT_VCENTER|DT_SINGLELINE;
		Item.szText = str[col];
		m_Grid.SetItem(&Item);
	}
	m_Grid.AutoSize();
}

BOOL CPage475::UpdateData( BOOL bSaveAndValidate)
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
		if(pDoc->lspc.tempfg != 1)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != pDoc->lspc.nrgroup)
		{
			AfxMessageBox("Card 475: Number of rows should be equal to nrgroup in Card 475!");
			Log("Card 475: Number of rows should be equal to nrgroup in Card 475!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.ptmps_param1 != NULL)
			delete [](pDoc->lspc.ptmps_param1);

		if (pDoc->lspc.nrgroup > 0)
			pDoc->lspc.ptmps_param1 = new TMPS_PARA1[pDoc->lspc.nrgroup];
		for(int i = 0; i < pDoc->lspc.nrgroup; ++i)
		{
			pDoc->lspc.ptmps_param1[i].gid      = m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.ptmps_param1[i].cfsaex_p = m_Grid.GetItemFloat(i+1,1);
			pDoc->lspc.ptmps_param1[i].katrad_p = m_Grid.GetItemFloat(i+1,2);
			pDoc->lspc.ptmps_param1[i].kcond_p  = m_Grid.GetItemFloat(i+1,3);
			pDoc->lspc.ptmps_param1[i].kevap_p  = m_Grid.GetItemFloat(i+1,4);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.ptmps_param1 == NULL)
			return true;
		
		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < pDoc->lspc.nrgroup; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0,pDoc->lspc.ptmps_param1[i].gid);
			m_Grid.SetItemFloat(nRow,1,pDoc->lspc.ptmps_param1[i].cfsaex_p);
			m_Grid.SetItemFloat(nRow,2,pDoc->lspc.ptmps_param1[i].katrad_p);
			m_Grid.SetItemFloat(nRow,3,pDoc->lspc.ptmps_param1[i].kcond_p);
			m_Grid.SetItemFloat(nRow,4,pDoc->lspc.ptmps_param1[i].kevap_p);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
