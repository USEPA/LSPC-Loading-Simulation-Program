// Page70.cpp: implementation of the CPage70 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page70.h"
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

CPage70::CPage70()
{

}

CPage70::CPage70(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage70::~CPage70()
{

}

void CPage70::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(4);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] = {"deluid", "deluname", "pre_mult", "pet_mult"}; // column name
	UINT format[] = {DT_RIGHT,DT_LEFT,DT_RIGHT,DT_RIGHT}; // column format

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;

	for(int col=0; col<m_Grid.GetColumnCount(); col++)
	{
		Item.col = col;
		Item.nFormat = format[col]|DT_VCENTER|DT_SINGLELINE;
		Item.szText = str[col];
		m_Grid.SetItem(&Item);
	}
	m_Grid.AutoSize();
}

BOOL CPage70::UpdateData(BOOL bSaveAndValidate)
{
	// set the active page
	CMainFrame *pMain = (CMainFrame *) AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *) pMain->m_wndSplitter.GetPane(0, 1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;

	if(bSaveAndValidate)
	{
		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != pDoc->lspc.nlandp)
		{
			AfxMessageBox("Card 70: Number of rows should be equal to nlandp in Card 40");
			Log("Card 70: Number of rows should be equal to nlandp in Card 40");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.ppluinfo != NULL)
			delete []pDoc->lspc.ppluinfo;

		if (pDoc->lspc.nlandp > 0)
			pDoc->lspc.ppluinfo = new LandUsePInfo[pDoc->lspc.nlandp];

		for(int i=0; i<nRow; i++)
		{
			pDoc->lspc.ppluinfo[i].luidp    = m_Grid.GetItemInt(i+1, 0);
			pDoc->lspc.ppluinfo[i].plusname = m_Grid.GetItemText(i+1,1);
			pDoc->lspc.ppluinfo[i].luPREmult = m_Grid.GetItemFloat(i+1,2);
			pDoc->lspc.ppluinfo[i].luPETmult = m_Grid.GetItemFloat(i+1,3);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.ppluinfo == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i=0; i<pDoc->lspc.nlandp; i++)
		{
			m_Grid.InsertRow(_T(""), -1);
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow, 0,pDoc->lspc.ppluinfo[i].luidp);
			m_Grid.SetItemText(nRow,1,pDoc->lspc.ppluinfo[i].plusname);
			m_Grid.SetItemFloat(nRow,2,pDoc->lspc.ppluinfo[i].luPREmult);
			m_Grid.SetItemFloat(nRow,3,pDoc->lspc.ppluinfo[i].luPETmult);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
