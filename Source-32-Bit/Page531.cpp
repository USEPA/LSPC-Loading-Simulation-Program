// Page531.cpp: implementation of the CPage531 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page531.h"
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

CPage531::CPage531()
{

}

CPage531::CPage531(UINT nID) : CPageAdd(nID)
{

}


CPage531::~CPage531()
{

}

void CPage531::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(7);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);
                                                                
	CString str[] ={"rgid","ratclp","nonref","litsed","alnpr","extb","malgr"}; // column name

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

BOOL CPage531::UpdateData( BOOL bSaveAndValidate)
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
			AfxMessageBox("Card 531: Number of rows should be equal to the number of stream groups in Card 531!");
			Log("Card 531: Number of rows should be equal to nrgroup in Card 531!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.plank_parm1 != NULL)
			delete [](pDoc->lspc.plank_parm1);

		if (nNum > 0)
			pDoc->lspc.plank_parm1 = new PLANK_PARA1[nNum];
		for(int i = 0; i < nNum; ++i)
		{                        
			pDoc->lspc.plank_parm1[i].gid		= m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.plank_parm1[i].ratclp	= m_Grid.GetItemFloat(i+1,1);
			pDoc->lspc.plank_parm1[i].nonref	= m_Grid.GetItemFloat(i+1,2);
			pDoc->lspc.plank_parm1[i].litsed	= m_Grid.GetItemFloat(i+1,3);
			pDoc->lspc.plank_parm1[i].alnpr		= m_Grid.GetItemFloat(i+1,4);
			pDoc->lspc.plank_parm1[i].extb		= m_Grid.GetItemFloat(i+1,5);
			pDoc->lspc.plank_parm1[i].malgr		= m_Grid.GetItemFloat(i+1,6);
		}
	}
	else                                       
	{
		InitGrid();

		if(pDoc->lspc.plank_parm1 == NULL)
			return true;
		
		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow,size.cy);
			m_Grid.SetItemInt(nRow,  0,pDoc->lspc.plank_parm1[i].gid);
			m_Grid.SetItemFloat(nRow,1,pDoc->lspc.plank_parm1[i].ratclp);
			m_Grid.SetItemFloat(nRow,2,pDoc->lspc.plank_parm1[i].nonref);
			m_Grid.SetItemFloat(nRow,3,pDoc->lspc.plank_parm1[i].litsed);
			m_Grid.SetItemFloat(nRow,4,pDoc->lspc.plank_parm1[i].alnpr);
			m_Grid.SetItemFloat(nRow,5,pDoc->lspc.plank_parm1[i].extb);
			m_Grid.SetItemFloat(nRow,6,pDoc->lspc.plank_parm1[i].malgr);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
