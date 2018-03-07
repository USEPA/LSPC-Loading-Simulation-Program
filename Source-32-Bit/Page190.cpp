// Page190.cpp: implementation of the CPage190 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page190.h"
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

CPage190::CPage190()
{

}

CPage190::CPage190(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage190::~CPage190()
{

}

void CPage190::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(14);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

 	CString str[] ={"defid","deluid","jan","feb","mar","apr","may","jun","jul","aug","sept","oct","nov","dec"}; // column name

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

BOOL CPage190::UpdateData( BOOL bSaveAndValidate)
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
		if(pDoc->lspc.vircfg==0)
			return true;

		if(pDoc->lspc.vircfg !=1)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 190: Number of rows should be equal to nlanp*ngroup in Card 190!");
			Log("Card 190: Number of rows should be equal to nlanp*ngroup in Card 190!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.ircm_p != NULL)
			delete [](pDoc->lspc.ircm_p);
		
		if (nNum > 0)
			pDoc->lspc.ircm_p = new MON_DATE[nNum];
		
		for(int i = 0; i < nNum; ++i)
		{
			pDoc->lspc.ircm_p[i].gid = m_Grid.GetItemInt(i+1,0);
			pDoc->lspc.ircm_p[i].lid = m_Grid.GetItemInt(i+1,1);
			for(int j=0; j<12; ++j)
				pDoc->lspc.ircm_p[i].fnum[j] = m_Grid.GetItemFloat(i+1,j+2);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.ircm_p == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,0,pDoc->lspc.ircm_p[i].gid);
			m_Grid.SetItemInt(nRow,1,pDoc->lspc.ircm_p[i].lid);
			for(int j=0; j<12;++j)
				m_Grid.SetItemFloat(nRow,2+j,pDoc->lspc.ircm_p[i].fnum[j]);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
