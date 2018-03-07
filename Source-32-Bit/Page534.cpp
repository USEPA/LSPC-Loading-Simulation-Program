// Page534.cpp: implementation of the CPage534 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page534.h"
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

CPage534::CPage534()
{

}

CPage534::CPage534(UINT nID) : CPageAdd(nID)
{

}

CPage534::~CPage534()
{

}

void CPage534::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(12);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);
                                                  
	CString str[] ={"rgid","phycon","seed","mxstay","oref","claldh","physet","refset","cfsaex","mbal","cfbalr","cfbalg"}; // column name

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

BOOL CPage534::UpdateData( BOOL bSaveAndValidate)
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
		if(pDoc->lspc.plkfg != 1)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 534: Number of rows should be equal to the number of stream groups in Card 534!");
			Log("Card 534: Number of rows should be equal to nrgroup in Card 534!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.plank_parm4 != NULL)
			delete [](pDoc->lspc.plank_parm4);

		if (nNum > 0)
			pDoc->lspc.plank_parm4 = new PLANK_PARA4[nNum];
		
		for(int i = 0; i < nNum; ++i)
		{
			pDoc->lspc.plank_parm4[i].gid     = m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.plank_parm4[i].phycon  = m_Grid.GetItemFloat(i+1,1);
			pDoc->lspc.plank_parm4[i].seed    = m_Grid.GetItemFloat(i+1,2);
			pDoc->lspc.plank_parm4[i].mxstay  = m_Grid.GetItemFloat(i+1,3);
			pDoc->lspc.plank_parm4[i].oref    = m_Grid.GetItemFloat(i+1,4);
			pDoc->lspc.plank_parm4[i].claldh  = m_Grid.GetItemFloat(i+1,5);
			pDoc->lspc.plank_parm4[i].physet  = m_Grid.GetItemFloat(i+1,6);
			pDoc->lspc.plank_parm4[i].refset  = m_Grid.GetItemFloat(i+1,7);
			pDoc->lspc.plank_parm4[i].cfsaex  = m_Grid.GetItemFloat(i+1,8);
			pDoc->lspc.plank_parm4[i].mbal    = m_Grid.GetItemFloat(i+1,9);
			pDoc->lspc.plank_parm4[i].cfbalr  = m_Grid.GetItemFloat(i+1,10);
			pDoc->lspc.plank_parm4[i].cfbalg  = m_Grid.GetItemFloat(i+1,11);
		}
	}         
	else                                           
	{
		InitGrid();

		if(pDoc->lspc.plank_parm4 == NULL)
			return true;
		
		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0, pDoc->lspc.plank_parm4[i].gid);
			m_Grid.SetItemFloat(nRow,1, pDoc->lspc.plank_parm4[i].phycon);
			m_Grid.SetItemFloat(nRow,2, pDoc->lspc.plank_parm4[i].seed);
			m_Grid.SetItemFloat(nRow,3, pDoc->lspc.plank_parm4[i].mxstay);
			m_Grid.SetItemFloat(nRow,4, pDoc->lspc.plank_parm4[i].oref); 
			m_Grid.SetItemFloat(nRow,5, pDoc->lspc.plank_parm4[i].claldh);
			m_Grid.SetItemFloat(nRow,6, pDoc->lspc.plank_parm4[i].physet);
			m_Grid.SetItemFloat(nRow,7, pDoc->lspc.plank_parm4[i].refset); 
			m_Grid.SetItemFloat(nRow,8, pDoc->lspc.plank_parm4[i].cfsaex); 
			m_Grid.SetItemFloat(nRow,9, pDoc->lspc.plank_parm4[i].mbal);
			m_Grid.SetItemFloat(nRow,10, pDoc->lspc.plank_parm4[i].cfbalr);
			m_Grid.SetItemFloat(nRow,11, pDoc->lspc.plank_parm4[i].cfbalg); 
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
