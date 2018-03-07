// Page521.cpp: implementation of the CPage521 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page521.h"
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

CPage521::CPage521()
{

}

CPage521::CPage521(UINT nID) : CPageAdd(nID)
{

}


CPage521::~CPage521()
{

}

void CPage521::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(11);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);
                                                
	CString str[] ={"rgid","cvbo","cvbpc","cvbpn","bpcntc","ktam20","kno220","tcnit","kno320","tcden","denoxt"}; // column name

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

BOOL CPage521::UpdateData( BOOL bSaveAndValidate)
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
		if(pDoc->lspc.nutfg != 1)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 521: Number of rows should be equal to the number of stream groups in Card 521!");
			Log("Card 521: Number of rows should be equal to nrgroup in Card 521!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.nut_parm1 != NULL)
			delete [](pDoc->lspc.nut_parm1);

		if (nNum > 0)
			pDoc->lspc.nut_parm1 = new NUT_PARA1[nNum];
		for(int i = 0; i < nNum; ++i)
		{                        
			pDoc->lspc.nut_parm1[i].gid    = m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.nut_parm1[i].cvbo   = m_Grid.GetItemFloat(i+1,1);
			pDoc->lspc.nut_parm1[i].cvbpc  = m_Grid.GetItemFloat(i+1,2);
			pDoc->lspc.nut_parm1[i].cvbpn  = m_Grid.GetItemFloat(i+1,3);
			pDoc->lspc.nut_parm1[i].bpcntc = m_Grid.GetItemFloat(i+1,4);
			pDoc->lspc.nut_parm1[i].ktam20 = m_Grid.GetItemFloat(i+1,5);
			pDoc->lspc.nut_parm1[i].kno220 = m_Grid.GetItemFloat(i+1,6);
			pDoc->lspc.nut_parm1[i].tcnit  = m_Grid.GetItemFloat(i+1,7);
			pDoc->lspc.nut_parm1[i].kno320 = m_Grid.GetItemFloat(i+1,8);
			pDoc->lspc.nut_parm1[i].tcden  = m_Grid.GetItemFloat(i+1,9);
			pDoc->lspc.nut_parm1[i].denoxt = m_Grid.GetItemFloat(i+1,10);
		}
	}
	else                       
	{
		InitGrid();

		if(pDoc->lspc.nut_parm1 == NULL)
			return true;
		
		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow,size.cy);
			m_Grid.SetItemInt(nRow,  0,pDoc->lspc.nut_parm1[i].gid);
			m_Grid.SetItemFloat(nRow,1,pDoc->lspc.nut_parm1[i].cvbo);
			m_Grid.SetItemFloat(nRow,2,pDoc->lspc.nut_parm1[i].cvbpc);
			m_Grid.SetItemFloat(nRow,3,pDoc->lspc.nut_parm1[i].cvbpn);
			m_Grid.SetItemFloat(nRow,4,pDoc->lspc.nut_parm1[i].bpcntc);
			m_Grid.SetItemFloat(nRow,5,pDoc->lspc.nut_parm1[i].ktam20);
			m_Grid.SetItemFloat(nRow,6,pDoc->lspc.nut_parm1[i].kno220);
			m_Grid.SetItemFloat(nRow,7,pDoc->lspc.nut_parm1[i].tcnit);
			m_Grid.SetItemFloat(nRow,8,pDoc->lspc.nut_parm1[i].kno320);
			m_Grid.SetItemFloat(nRow,9,pDoc->lspc.nut_parm1[i].tcden);
			m_Grid.SetItemFloat(nRow,10,pDoc->lspc.nut_parm1[i].denoxt);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
