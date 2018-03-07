// Page282.cpp: implementation of the CPage282 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page282.h"
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

CPage282::CPage282()
{

}

CPage282::CPage282(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage282::~CPage282()
{

}

void CPage282::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(14);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] ={"rgid","dwqid","jan","feb","mar","apr","may","jun","jul","aug","sept","oct","nov","dec"}; // column name

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

BOOL CPage282::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;
	
	int nNum = pDoc->lspc.nrgroup*pDoc->lspc.nQuals;
	
	if(bSaveAndValidate)
	{
		if(pDoc->lspc.maddfgrch != 1)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 282: Row number should be equal to nrgroup*nQuals.");
			Log("Card 282: Row number should be equal to nrgroup*nQuals.");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.pMDDrch != NULL)
			delete [](pDoc->lspc.pMDDrch);

		if (nNum > 0)
			pDoc->lspc.pMDDrch = new MON_DATA[nNum];

		for(int i=0; i<nNum; ++i)
		{
			pDoc->lspc.pMDDrch[i].rgid    = m_Grid.GetItemInt(i+1,0);
			pDoc->lspc.pMDDrch[i].qualid  = m_Grid.GetItemInt(i+1,1);
			for(int j=0; j<12; ++j)
				pDoc->lspc.pMDDrch[i].fnum[j] = m_Grid.GetItemFloat(i+1,j+2);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.pMDDrch == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i=0; i<nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow,size.cy);
			m_Grid.SetItemInt(nRow,0,pDoc->lspc.pMDDrch[i].rgid);
			m_Grid.SetItemInt(nRow,1,pDoc->lspc.pMDDrch[i].qualid);
			for(int j=0; j<12; ++j)
				m_Grid.SetItemFloat(nRow,2+j,pDoc->lspc.pMDDrch[i].fnum[j]);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
