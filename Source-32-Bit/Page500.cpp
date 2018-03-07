// Page500.cpp: implementation of the CPage500 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page500.h"
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

CPage500::CPage500()
{

}

CPage500::CPage500(UINT nID) : CPageAdd(nID)
{

}


CPage500::~CPage500()
{

}

void CPage500::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(12);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] ={"rgid","dwqid","lutype","bod","nox","tam","snh4","po4","spo4","orn","orp","orc"}; // column name

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

BOOL CPage500::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;
	
	int nNum = pDoc->lspc.nQuals*pDoc->lspc.nrgroup*4;

	if(bSaveAndValidate)
	{
		if(pDoc->lspc.oxfg != 1)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 500: Number of rows should be equal to nQuals*nrgroup*4 in Card 500!");
			Log("Card 500: Number of rows should be equal to nQuals*nrgroup*4 in Card 500!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.land_mapp != NULL)
			delete [](pDoc->lspc.land_mapp);

		if (nNum > 0)
			pDoc->lspc.land_mapp = new LAND_MAPP[nNum];
		for(int i = 0; i < nNum; ++i)
		{
			pDoc->lspc.land_mapp[i].gid   = m_Grid.GetItemInt(i+1, 0);
			pDoc->lspc.land_mapp[i].qualid = m_Grid.GetItemInt(i+1,1);
			pDoc->lspc.land_mapp[i].lutype = m_Grid.GetItemInt(i+1,2);
			pDoc->lspc.land_mapp[i].bod = m_Grid.GetItemFloat(i+1, 3);
			pDoc->lspc.land_mapp[i].nox = m_Grid.GetItemFloat(i+1, 4);
			pDoc->lspc.land_mapp[i].tam = m_Grid.GetItemFloat(i+1, 5);
			pDoc->lspc.land_mapp[i].snh4 = m_Grid.GetItemFloat(i+1,6);
			pDoc->lspc.land_mapp[i].po4 = m_Grid.GetItemFloat(i+1, 7);
			pDoc->lspc.land_mapp[i].spo4 = m_Grid.GetItemFloat(i+1,8);
			pDoc->lspc.land_mapp[i].orn = m_Grid.GetItemFloat(i+1, 9);
			pDoc->lspc.land_mapp[i].orp = m_Grid.GetItemFloat(i+1,10);
			pDoc->lspc.land_mapp[i].orc = m_Grid.GetItemFloat(i+1,11);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.land_mapp == NULL)
			return true;
		
		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow,size.cy);
			m_Grid.SetItemInt(nRow,   0,pDoc->lspc.land_mapp[i].gid);
			m_Grid.SetItemInt(nRow,   1,pDoc->lspc.land_mapp[i].qualid);
			m_Grid.SetItemInt(nRow,   2,pDoc->lspc.land_mapp[i].lutype);
			m_Grid.SetItemFloat(nRow, 3,pDoc->lspc.land_mapp[i].bod);
			m_Grid.SetItemFloat(nRow, 4,pDoc->lspc.land_mapp[i].nox);
			m_Grid.SetItemFloat(nRow, 5,pDoc->lspc.land_mapp[i].tam);
			m_Grid.SetItemFloat(nRow, 6,pDoc->lspc.land_mapp[i].snh4);
			m_Grid.SetItemFloat(nRow, 7,pDoc->lspc.land_mapp[i].po4);
			m_Grid.SetItemFloat(nRow, 8,pDoc->lspc.land_mapp[i].spo4);
			m_Grid.SetItemFloat(nRow, 9,pDoc->lspc.land_mapp[i].orn);
			m_Grid.SetItemFloat(nRow,10,pDoc->lspc.land_mapp[i].orp);
			m_Grid.SetItemFloat(nRow,11,pDoc->lspc.land_mapp[i].orc);

		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
