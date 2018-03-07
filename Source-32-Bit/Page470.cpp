// Page470.cpp: implementation of the CPage470 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page470.h"
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

CPage470::CPage470()
{

}

CPage470::CPage470(UINT nID)
:CPageAdd(nID)
{
}

CPage470::~CPage470()
{

}

void CPage470::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(5);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] = {"subbasin","elev","eldat","relev","reldat"}; // column name
	UINT format[] = {DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT};

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;

	for (int col = 0; col < m_Grid.GetColumnCount(); col++)
	{ 
		Item.col = col;
		Item.nFormat = format[col]|DT_VCENTER|DT_SINGLELINE;
		Item.szText = str[col];
		m_Grid.SetItem(&Item);
	}
	m_Grid.AutoSize();
}

BOOL CPage470::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;

	if(bSaveAndValidate)
	{
		if(pDoc->lspc.tempfg != 1)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != pDoc->lspc.nsws)
		{
			AfxMessageBox("Card 470: Number of rows should be equal to nsws in Card 470!");
			Log("Card 470: Number of rows should be equal to nsws in Card 470!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.ptmp_param1 != NULL)
			delete [](pDoc->lspc.ptmp_param1);

		if (pDoc->lspc.nsws > 0)
			pDoc->lspc.ptmp_param1 = new TMP_PARA1[pDoc->lspc.nsws];
		for(int i = 0; i < pDoc->lspc.nsws; ++i)
		{
			pDoc->lspc.ptmp_param1[i].subbasin = m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.ptmp_param1[i].elev     = m_Grid.GetItemFloat(i+1,1);
			pDoc->lspc.ptmp_param1[i].eldat    = m_Grid.GetItemFloat(i+1,2);
			pDoc->lspc.ptmp_param1[i].relev    = m_Grid.GetItemFloat(i+1,3);
			pDoc->lspc.ptmp_param1[i].reldat   = m_Grid.GetItemFloat(i+1,4);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.ptmp_param1 == NULL)
			return true;
		
		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < pDoc->lspc.nsws; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0,pDoc->lspc.ptmp_param1[i].subbasin);
			m_Grid.SetItemFloat(nRow,1,pDoc->lspc.ptmp_param1[i].elev);
			m_Grid.SetItemFloat(nRow,2,pDoc->lspc.ptmp_param1[i].eldat);
			m_Grid.SetItemFloat(nRow,3,pDoc->lspc.ptmp_param1[i].relev);
			m_Grid.SetItemFloat(nRow,4,pDoc->lspc.ptmp_param1[i].reldat);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
