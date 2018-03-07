// Page541.cpp: implementation of the CPage541 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page541.h"
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

CPage541::CPage541()
{

}

CPage541::CPage541(UINT nID) : CPageAdd(nID)
{

}


CPage541::~CPage541()
{

}

void CPage541::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(6);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);
                                                                                       
	CString str[] ={"rgid","phcnt","alkcon","cfcinv","brco2_1","brco2_2"}; // column name

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

BOOL CPage541::UpdateData( BOOL bSaveAndValidate)
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
		if(pDoc->lspc.phfg != 1)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 541: Number of rows should be equal to the number of stream groups in Card 541!");
			Log("Card 541: Number of rows should be equal to nrgroup in Card 541!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.ph_para != NULL)
			delete [](pDoc->lspc.ph_para);

		if (nNum > 0)
			pDoc->lspc.ph_para = new PH_PARA[nNum];
		
		for(int i = 0; i < nNum; ++i)
		{
			pDoc->lspc.ph_para[i].gid		= m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.ph_para[i].phcnt		= m_Grid.GetItemInt(i+1,  1);
			pDoc->lspc.ph_para[i].alkcon	= m_Grid.GetItemInt(i+1,  2);
			pDoc->lspc.ph_para[i].cfcinv	= m_Grid.GetItemFloat(i+1,3);
			pDoc->lspc.ph_para[i].brco2_1	= m_Grid.GetItemFloat(i+1,4);
			pDoc->lspc.ph_para[i].brco2_2	= m_Grid.GetItemFloat(i+1,5);
		}
	}                  
	else                                           
	{
		InitGrid();

		if(pDoc->lspc.ph_para == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow,size.cy);
			m_Grid.SetItemInt(nRow,  0, pDoc->lspc.ph_para[i].gid);
			m_Grid.SetItemInt(nRow,  1, pDoc->lspc.ph_para[i].phcnt);
			m_Grid.SetItemInt(nRow,  2, pDoc->lspc.ph_para[i].alkcon);
			m_Grid.SetItemFloat(nRow,3, pDoc->lspc.ph_para[i].cfcinv); 
			m_Grid.SetItemFloat(nRow,4, pDoc->lspc.ph_para[i].brco2_1); 
			m_Grid.SetItemFloat(nRow,5, pDoc->lspc.ph_para[i].brco2_2);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
