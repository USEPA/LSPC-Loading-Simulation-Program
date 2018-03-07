// Page480.cpp: implementation of the CPage480 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page480.h"
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

CPage480::CPage480()
{

}

CPage480::CPage480(UINT nID)
:CPageAdd(nID)
{
}

CPage480::~CPage480()
{

}

void CPage480::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(8);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] = {"rgid","preflg","bedflg","tgflg","muddep","tgrnd","kmud","kgrnd"}; // column name
	UINT format[] = {DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT};

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

BOOL CPage480::UpdateData( BOOL bSaveAndValidate)
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
		if(nRow != pDoc->lspc.nrgroup)
		{
			AfxMessageBox("Card 480: Number of rows should be equal to nrgroup in Card 480!");
			Log("Card 480: Number of rows should be equal to nrgroup in Card 480!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.ptmps_param2 != NULL)
			delete [](pDoc->lspc.ptmps_param2);

		if (pDoc->lspc.nrgroup > 0)
			pDoc->lspc.ptmps_param2 = new TMPS_PARA2[pDoc->lspc.nrgroup];
		for(int i=0; i<pDoc->lspc.nrgroup; ++i)
		{
			pDoc->lspc.ptmps_param2[i].rgid   = m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.ptmps_param2[i].preflg = m_Grid.GetItemInt(i+1,  1);
			pDoc->lspc.ptmps_param2[i].bedflg = m_Grid.GetItemInt(i+1,  2);
			pDoc->lspc.ptmps_param2[i].tgflg  = m_Grid.GetItemInt(i+1,  3);
			pDoc->lspc.ptmps_param2[i].muddep = m_Grid.GetItemFloat(i+1,4);
			pDoc->lspc.ptmps_param2[i].tgrnd  = m_Grid.GetItemFloat(i+1,5);
			pDoc->lspc.ptmps_param2[i].kmud   = m_Grid.GetItemFloat(i+1,6);
			pDoc->lspc.ptmps_param2[i].kgrnd  = m_Grid.GetItemFloat(i+1,7);

			if (pDoc->lspc.ptmps_param2[i].bedflg > 0)
				pDoc->lspc.bedflg = true;
			if (pDoc->lspc.ptmps_param2[i].tgflg == 3)
				pDoc->lspc.mtgflg = true;
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.ptmps_param2 == NULL)
			return true;
		
		CSize size = m_Grid.GetTextExtent(" ");
		for(int i=0; i<pDoc->lspc.nrgroup; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0,pDoc->lspc.ptmps_param2[i].rgid);
			m_Grid.SetItemInt(nRow,  1,pDoc->lspc.ptmps_param2[i].preflg);
			m_Grid.SetItemInt(nRow,  2,pDoc->lspc.ptmps_param2[i].bedflg);
			m_Grid.SetItemInt(nRow,  3,pDoc->lspc.ptmps_param2[i].tgflg);
			m_Grid.SetItemFloat(nRow,4,pDoc->lspc.ptmps_param2[i].muddep);
			m_Grid.SetItemFloat(nRow,5,pDoc->lspc.ptmps_param2[i].tgrnd);
			m_Grid.SetItemFloat(nRow,6,pDoc->lspc.ptmps_param2[i].kmud);
			m_Grid.SetItemFloat(nRow,7,pDoc->lspc.ptmps_param2[i].kgrnd);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
