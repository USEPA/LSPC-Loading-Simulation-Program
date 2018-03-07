// Page401.cpp: implementation of the CPage401 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page401.h"
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

CPage401::CPage401()
{

}

CPage401::CPage401(UINT nID) : CPageAdd(nID)
{

}


CPage401::~CPage401()
{

}

void CPage401::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(13);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] ={"rgid","jan","feb","mar","apr","may","jun","jul","aug","sep","oct","nov","dec"}; // column name

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

BOOL CPage401::UpdateData( BOOL bSaveAndValidate)
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
		if(pDoc->lspc.vconfg != 1)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 401: Number of rows should be equal to the number of stream groups!");
			Log("Card 401: Number of rows should be equal to nrgroup");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.pMONTH_COV != NULL)
			delete [](pDoc->lspc.pMONTH_COV);

		if (nNum > 0)
			pDoc->lspc.pMONTH_COV = new MONTH_PH[nNum];
		
		for(int i = 0; i < nNum; ++i)
		{
			pDoc->lspc.pMONTH_COV[i].gid      = m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.pMONTH_COV[i].fnum[0]  = m_Grid.GetItemFloat(i+1,1);
			pDoc->lspc.pMONTH_COV[i].fnum[1]  = m_Grid.GetItemFloat(i+1,2);
			pDoc->lspc.pMONTH_COV[i].fnum[2]  = m_Grid.GetItemFloat(i+1,3);
			pDoc->lspc.pMONTH_COV[i].fnum[3]  = m_Grid.GetItemFloat(i+1,4);
			pDoc->lspc.pMONTH_COV[i].fnum[4]  = m_Grid.GetItemFloat(i+1,5);
			pDoc->lspc.pMONTH_COV[i].fnum[5]  = m_Grid.GetItemFloat(i+1,6);
			pDoc->lspc.pMONTH_COV[i].fnum[6]  = m_Grid.GetItemFloat(i+1,7);
			pDoc->lspc.pMONTH_COV[i].fnum[7]  = m_Grid.GetItemFloat(i+1,8);
			pDoc->lspc.pMONTH_COV[i].fnum[8]  = m_Grid.GetItemFloat(i+1,9);
			pDoc->lspc.pMONTH_COV[i].fnum[9]  = m_Grid.GetItemFloat(i+1,10);
			pDoc->lspc.pMONTH_COV[i].fnum[10] = m_Grid.GetItemFloat(i+1,11);
			pDoc->lspc.pMONTH_COV[i].fnum[11] = m_Grid.GetItemFloat(i+1,12);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.pMONTH_COV == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow,size.cy);
			m_Grid.SetItemInt(nRow,  0, pDoc->lspc.pMONTH_COV[i].gid);
			m_Grid.SetItemFloat(nRow,1, pDoc->lspc.pMONTH_COV[i].fnum[0]);
			m_Grid.SetItemFloat(nRow,2, pDoc->lspc.pMONTH_COV[i].fnum[1]);
			m_Grid.SetItemFloat(nRow,3, pDoc->lspc.pMONTH_COV[i].fnum[2]);
			m_Grid.SetItemFloat(nRow,4, pDoc->lspc.pMONTH_COV[i].fnum[3]);
			m_Grid.SetItemFloat(nRow,5, pDoc->lspc.pMONTH_COV[i].fnum[4]);
			m_Grid.SetItemFloat(nRow,6, pDoc->lspc.pMONTH_COV[i].fnum[5]);
			m_Grid.SetItemFloat(nRow,7, pDoc->lspc.pMONTH_COV[i].fnum[6]);
			m_Grid.SetItemFloat(nRow,8, pDoc->lspc.pMONTH_COV[i].fnum[7]);
			m_Grid.SetItemFloat(nRow,9, pDoc->lspc.pMONTH_COV[i].fnum[8]);
			m_Grid.SetItemFloat(nRow,10,pDoc->lspc.pMONTH_COV[i].fnum[9]);
			m_Grid.SetItemFloat(nRow,11,pDoc->lspc.pMONTH_COV[i].fnum[10]);
			m_Grid.SetItemFloat(nRow,12,pDoc->lspc.pMONTH_COV[i].fnum[11]);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
