// Page455.cpp: implementation of the CPage455 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page455.h"
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

CPage455::CPage455()
{

}

CPage455::CPage455(UINT nID) : CPageAdd(nID)
{

}

CPage455::~CPage455()
{

}

void CPage455::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(5);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] ={"rgid","bedwid","beddep","por","burial"}; // column name

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

BOOL CPage455::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;
	
	int nNum = pDoc->lspc.nrgroup;

	if(bSaveAndValidate)
	{
		if(pDoc->lspc.sedfg  != 1)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 455: Number of rows should be equal to number of stream groups in Card 455!");
			Log("Card 455: Number of rows should be equal to nrgroup in Card 455!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.rsd_genparam != NULL)
			delete [](pDoc->lspc.rsd_genparam);

		if (nNum > 0)
			pDoc->lspc.rsd_genparam = new STREAMSED_GENPARM[nNum];
		
		for(int i = 0; i < nNum; ++i)
		{
			pDoc->lspc.rsd_genparam[i].gid	    = m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.rsd_genparam[i].bedwid	= m_Grid.GetItemFloat(i+1,1);
			pDoc->lspc.rsd_genparam[i].beddep	= m_Grid.GetItemFloat(i+1,2);
			pDoc->lspc.rsd_genparam[i].por		= m_Grid.GetItemFloat(i+1,3);
			pDoc->lspc.rsd_genparam[i].burial	= m_Grid.GetItemFloat(i+1,4);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.rsd_genparam == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0, pDoc->lspc.rsd_genparam[i].gid);
			m_Grid.SetItemFloat(nRow,1, pDoc->lspc.rsd_genparam[i].bedwid); 
			m_Grid.SetItemFloat(nRow,2, pDoc->lspc.rsd_genparam[i].beddep);
			m_Grid.SetItemFloat(nRow,3, pDoc->lspc.rsd_genparam[i].por); 
			m_Grid.SetItemFloat(nRow,4, pDoc->lspc.rsd_genparam[i].burial); 
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
