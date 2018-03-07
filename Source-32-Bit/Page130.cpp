// Page130.cpp: implementation of the CPage130 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page130.h"
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

CPage130::CPage130()
{

}

CPage130::CPage130(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage130::~CPage130()
{

}

void CPage130::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(8);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] = {"defid","deluid","cepsc","uzsn","nsur","intfw","irc","lzetp"}; // column name

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

BOOL CPage130::UpdateData( BOOL bSaveAndValidate)
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
		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 130: Number of rows should be equal to nlanp*ngroup in Card 4!");
			Log("Card 130: Number of rows should be equal to nlanp*ngroup in Card 4!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.ppw_param4 != NULL)
			delete [](pDoc->lspc.ppw_param4);
		
		if (nNum > 0)
			pDoc->lspc.ppw_param4 = new PW_PARA4[nNum];
		
		for(int i = 0; i < nNum; ++i)
		{
			pDoc->lspc.ppw_param4[i].gid     = m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.ppw_param4[i].lid     = m_Grid.GetItemInt(i+1,  1);
			pDoc->lspc.ppw_param4[i].cepsc_p = m_Grid.GetItemFloat(i+1,2);
			pDoc->lspc.ppw_param4[i].uzsn_p  = m_Grid.GetItemFloat(i+1,3);
			pDoc->lspc.ppw_param4[i].nsur_p  = m_Grid.GetItemFloat(i+1,4);
			pDoc->lspc.ppw_param4[i].intfw_p = m_Grid.GetItemFloat(i+1,5);
			pDoc->lspc.ppw_param4[i].irc_p   = m_Grid.GetItemFloat(i+1,6);
			pDoc->lspc.ppw_param4[i].lzetp_p = m_Grid.GetItemFloat(i+1,7);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.ppw_param4 == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i=0; i<nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0,pDoc->lspc.ppw_param4[i].gid);
			m_Grid.SetItemInt(nRow,  1,pDoc->lspc.ppw_param4[i].lid);
			m_Grid.SetItemFloat(nRow,2,pDoc->lspc.ppw_param4[i].cepsc_p);
			m_Grid.SetItemFloat(nRow,3,pDoc->lspc.ppw_param4[i].uzsn_p);
			m_Grid.SetItemFloat(nRow,4,pDoc->lspc.ppw_param4[i].nsur_p);
			m_Grid.SetItemFloat(nRow,5,pDoc->lspc.ppw_param4[i].intfw_p);
			m_Grid.SetItemFloat(nRow,6,pDoc->lspc.ppw_param4[i].irc_p);
			m_Grid.SetItemFloat(nRow,7,pDoc->lspc.ppw_param4[i].lzetp_p);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
