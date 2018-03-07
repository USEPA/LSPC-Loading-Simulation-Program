// Page503.cpp: implementation of the CPage503 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page503.h"
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

CPage503::CPage503()
{

}

CPage503::CPage503(UINT nID) : CPageAdd(nID)
{

}


CPage503::~CPage503()
{

}

void CPage503::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(8);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] ={"defid","deluid","sdoxp","sco2p","idoxp","ico2p","adoxp","aco2p"}; // column name

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

BOOL CPage503::UpdateData( BOOL bSaveAndValidate)
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
		if(pDoc->lspc.oxfg != 1)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 503: Number of rows should be equal to nlanp*ngroup in Card 503!");
			Log("Card 503: Number of rows should be equal to nlanp*ngroup in Card 503!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.doco_parm1 != NULL)
			delete [](pDoc->lspc.doco_parm1);

		if (nNum > 0)
			pDoc->lspc.doco_parm1 = new DOCO_PARA1[nNum];
		for(int i = 0; i < nNum; ++i)
		{
			pDoc->lspc.doco_parm1[i].gid   = m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.doco_parm1[i].lid   = m_Grid.GetItemInt(i+1,  1);
			pDoc->lspc.doco_parm1[i].sdoxp = m_Grid.GetItemFloat(i+1,2);
			pDoc->lspc.doco_parm1[i].sco2p = m_Grid.GetItemFloat(i+1,3);
			pDoc->lspc.doco_parm1[i].idoxp = m_Grid.GetItemFloat(i+1,4);
			pDoc->lspc.doco_parm1[i].ico2p = m_Grid.GetItemFloat(i+1,5);
			pDoc->lspc.doco_parm1[i].adoxp = m_Grid.GetItemFloat(i+1,6);
			pDoc->lspc.doco_parm1[i].aco2p = m_Grid.GetItemFloat(i+1,7);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.doco_parm1 == NULL)
			return true;
		
		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow,size.cy);
			m_Grid.SetItemInt(nRow,  0,pDoc->lspc.doco_parm1[i].gid);
			m_Grid.SetItemInt(nRow,  1,pDoc->lspc.doco_parm1[i].lid);
			m_Grid.SetItemFloat(nRow,2,pDoc->lspc.doco_parm1[i].sdoxp);
			m_Grid.SetItemFloat(nRow,3,pDoc->lspc.doco_parm1[i].sco2p);
			m_Grid.SetItemFloat(nRow,4,pDoc->lspc.doco_parm1[i].idoxp);
			m_Grid.SetItemFloat(nRow,5,pDoc->lspc.doco_parm1[i].ico2p);
			m_Grid.SetItemFloat(nRow,6,pDoc->lspc.doco_parm1[i].adoxp);
			m_Grid.SetItemFloat(nRow,7,pDoc->lspc.doco_parm1[i].aco2p);

		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
