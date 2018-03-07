// Page610.cpp: implementation of the CPage610 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page610.h"
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

CPage610::CPage610()
{

}

CPage610::CPage610(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage610::~CPage610()
{

}

void CPage610::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(16);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] ={"trgid","tnum","ttype","tdays","jan","feb","mar","apr","may","jun","jul","aug","sep","oct","nov","dec"}; // column name

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

BOOL CPage610::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;
	
	int nNum = pDoc->lspc.ntrgp*pDoc->lspc.ntnum;
	
	if(bSaveAndValidate)
	{
		if(pDoc->lspc.ntnum == 0)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("card 610: Row number should be equal to ntrgp*ntnum.");
			Log("card 610: Row number should be equal to ntrgp*ntnum.");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.pMTDATA != NULL)
		{
			delete [](pDoc->lspc.pMTDATA);
			pDoc->lspc.pMTDATA = NULL;
		}

		if (nNum > 0)
			pDoc->lspc.pMTDATA = new MON_TDATA[nNum];

		for(int i=0; i<nNum; ++i)
		{
			pDoc->lspc.pMTDATA[i].trgid    = m_Grid.GetItemInt(i+1,0);
			pDoc->lspc.pMTDATA[i].tnum	   = m_Grid.GetItemInt(i+1,1);
			pDoc->lspc.pMTDATA[i].ttype    = m_Grid.GetItemInt(i+1,2);
			pDoc->lspc.pMTDATA[i].tdays    = m_Grid.GetItemInt(i+1,3);
			for(int j= 0; j< 12;++j)
				pDoc->lspc.pMTDATA[i].fnum[j] = m_Grid.GetItemFloat(i+1,j+4);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.pMTDATA == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,0,pDoc->lspc.pMTDATA[i].trgid);
			m_Grid.SetItemInt(nRow,1,pDoc->lspc.pMTDATA[i].tnum);
			m_Grid.SetItemInt(nRow,2,pDoc->lspc.pMTDATA[i].ttype);
			m_Grid.SetItemInt(nRow,3,pDoc->lspc.pMTDATA[i].tdays);
			for(int j= 0; j< 12;++j)
				m_Grid.SetItemFloat(nRow,4+j,pDoc->lspc.pMTDATA[i].fnum[j]);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
