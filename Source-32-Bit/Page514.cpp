// Page514.cpp: implementation of the CPage514 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page514.h"
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

CPage514::CPage514()
{

}

CPage514::CPage514(UINT nID) : CPageAdd(nID)
{

}

CPage514::~CPage514()
{

}

void CPage514::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(3);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);
   
	CString str[] ={"rgid","scrvel","scrmul"}; // column name

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

BOOL CPage514::UpdateData( BOOL bSaveAndValidate)
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
		if(pDoc->lspc.oxfg  != 1)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 514: Number of rows should be equal to the number of stream groups in Card 514!");
			Log("Card 514: Number of rows should be equal to nrgroup in Card 514!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.scour_parms != NULL)
			delete [](pDoc->lspc.scour_parms);

		if (nNum > 0)
			pDoc->lspc.scour_parms = new SCOUR_PARMS[nNum];
		
		for(int i = 0; i < nNum; ++i)
		{
			pDoc->lspc.scour_parms[i].gid     = m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.scour_parms[i].scrvel  = m_Grid.GetItemFloat(i+1,1);
			pDoc->lspc.scour_parms[i].scrmul  = m_Grid.GetItemFloat(i+1,2);
		}
	}
	else    
	{
		InitGrid();

		if(pDoc->lspc.scour_parms == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0, pDoc->lspc.scour_parms[i].gid);
			m_Grid.SetItemFloat(nRow,1, pDoc->lspc.scour_parms[i].scrvel);
			m_Grid.SetItemFloat(nRow,2, pDoc->lspc.scour_parms[i].scrmul);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
