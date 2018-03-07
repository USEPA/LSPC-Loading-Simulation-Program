// Page680.cpp: implementation of the CPage680 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page680.h"
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

CPage680::CPage680()
{

}

CPage680::CPage680(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage680::~CPage680()
{

}

void CPage680::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(14);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] ={"rchid","outlet","jan","feb","mar","apr","may","jun","jul","aug","sep","oct","nov","dec"}; // column name

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

BOOL CPage680::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;
	
	int nNum = pDoc->lspc.ncrchOutlets;
	int Num = 12;
	
	if(bSaveAndValidate)
	{
		if(pDoc->lspc.ncrchOutlets <= 0)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 680: Row number should be equal to number of controlled outlets");
			Log("Card 680: Row number should be equal to number of controlled outlets");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.pMON_SWITCH != NULL)
			delete [](pDoc->lspc.pMON_SWITCH);

		if (nRow > 0)
			pDoc->lspc.pMON_SWITCH = new MON_SWITCH[nRow];

		for(int i=0; i<nRow; ++i)
		{
			pDoc->lspc.pMON_SWITCH[i].rchid       = m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.pMON_SWITCH[i].nOutletId   = m_Grid.GetItemInt(i+1,  1);
			for(int j=0; j<Num; j++)
				pDoc->lspc.pMON_SWITCH[i].nSwitch[j] = m_Grid.GetItemInt(i+1,j+2);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.pMON_SWITCH == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i=0; i<nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0,pDoc->lspc.pMON_SWITCH[i].rchid);
			m_Grid.SetItemInt(nRow,  1,pDoc->lspc.pMON_SWITCH[i].nOutletId);
			for(int j=0; j<Num; j++)
				m_Grid.SetItemInt(nRow,2+j,pDoc->lspc.pMON_SWITCH[i].nSwitch[j]);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
