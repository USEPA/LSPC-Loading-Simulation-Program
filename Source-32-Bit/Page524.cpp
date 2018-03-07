// Page524.cpp: implementation of the CPage524 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page524.h"
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

CPage524::CPage524()
{

}

CPage524::CPage524(UINT nID) : CPageAdd(nID)
{

}

CPage524::~CPage524()
{

}

void CPage524::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(11);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);
                              
	CString str[] ={"rgid","no3","tam","no2","po4","snh4_1","snh4_2","snh4_3","spo4_1","spo4_2","spo4_3"}; // column name

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

BOOL CPage524::UpdateData( BOOL bSaveAndValidate)
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
		if(pDoc->lspc.nutfg  != 1)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 524: Number of rows should be equal to the number of stream groups in Card 524!");
			Log("Card 524: Number of rows should be equal to nrgroup in Card 524!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.nut_init != NULL)
			delete [](pDoc->lspc.nut_init);

		if (nNum > 0)
			pDoc->lspc.nut_init = new NUT_INIT[nNum];
		
		for(int i = 0; i < nNum; ++i)
		{
			pDoc->lspc.nut_init[i].gid		= m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.nut_init[i].no3		= m_Grid.GetItemFloat(i+1,1);
			pDoc->lspc.nut_init[i].tam		= m_Grid.GetItemFloat(i+1,2);
			pDoc->lspc.nut_init[i].no2		= m_Grid.GetItemFloat(i+1,3);
			pDoc->lspc.nut_init[i].po4		= m_Grid.GetItemFloat(i+1,4);
			pDoc->lspc.nut_init[i].snh4_1	= m_Grid.GetItemFloat(i+1,5);
			pDoc->lspc.nut_init[i].snh4_2	= m_Grid.GetItemFloat(i+1,6);
			pDoc->lspc.nut_init[i].snh4_3	= m_Grid.GetItemFloat(i+1,7);
			pDoc->lspc.nut_init[i].spo4_1	= m_Grid.GetItemFloat(i+1,8);
			pDoc->lspc.nut_init[i].spo4_2	= m_Grid.GetItemFloat(i+1,9);
			pDoc->lspc.nut_init[i].spo4_3	= m_Grid.GetItemFloat(i+1,10);
		}
	}
	else                               
	{
		InitGrid();

		if(pDoc->lspc.nut_init == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0, pDoc->lspc.nut_init[i].gid);
			m_Grid.SetItemFloat(nRow,1, pDoc->lspc.nut_init[i].no3);
			m_Grid.SetItemFloat(nRow,2, pDoc->lspc.nut_init[i].tam);
			m_Grid.SetItemFloat(nRow,3, pDoc->lspc.nut_init[i].no2);
			m_Grid.SetItemFloat(nRow,4, pDoc->lspc.nut_init[i].po4);
			m_Grid.SetItemFloat(nRow,5, pDoc->lspc.nut_init[i].snh4_1);
			m_Grid.SetItemFloat(nRow,6, pDoc->lspc.nut_init[i].snh4_2);
			m_Grid.SetItemFloat(nRow,7, pDoc->lspc.nut_init[i].snh4_3);
			m_Grid.SetItemFloat(nRow,8, pDoc->lspc.nut_init[i].spo4_1);
			m_Grid.SetItemFloat(nRow,9, pDoc->lspc.nut_init[i].spo4_2);
			m_Grid.SetItemFloat(nRow,10, pDoc->lspc.nut_init[i].spo4_3);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
