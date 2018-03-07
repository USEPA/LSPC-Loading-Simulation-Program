// Page250.cpp: implementation of the CPage250 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page250.h"
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

CPage250::CPage250()
{

}

CPage250::CPage250(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage250::~CPage250()
{

}

void CPage250::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(8);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] = {"defid","dwqid","qname","unit","qsdfg","gqsdfg","qsofg","potfcfg"}; // column name
	UINT format[] = {DT_RIGHT,DT_RIGHT,DT_LEFT,DT_LEFT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT}; // column format

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

BOOL CPage250::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;
	
	int nNum = pDoc->lspc.nQuals *pDoc->lspc.ngroup;

	if(bSaveAndValidate)
	{
		if(pDoc->lspc.pqalfg == 0)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		pDoc->lspc.gqsdfg = 0;

		if(nRow != nNum)
		{
			AfxMessageBox("Card 250: Row number should be equal to ngroup*nquals.");
			Log("Card 250: Row number should be equal to ngroup*nquals.");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.pQual != NULL)
			delete [](pDoc->lspc.pQual);

		if (nRow > 0)
			pDoc->lspc.pQual = new QUALS[nRow];
		
		for(int i = 0; i < nRow; ++i)
		{
			pDoc->lspc.pQual[i].gid   = m_Grid.GetItemInt(i+1,   0);
			pDoc->lspc.pQual[i].qualid = m_Grid.GetItemInt(i+1,  1);
			pDoc->lspc.pQual[i].qname  = m_Grid.GetItemText(i+1, 2);
			pDoc->lspc.pQual[i].qunit  = m_Grid.GetItemText(i+1, 3);
			pDoc->lspc.pQual[i].qsdfg  = m_Grid.GetItemInt(i+1,  4);
			pDoc->lspc.pQual[i].gqsdfg = m_Grid.GetItemInt(i+1,  5);
			pDoc->lspc.pQual[i].qsofg  = m_Grid.GetItemInt(i+1,  6);
			pDoc->lspc.pQual[i].potfcfg = m_Grid.GetItemInt(i+1, 7);
			if (pDoc->lspc.pQual[i].qsofg != 1)	pDoc->lspc.pQual[i].qsofg = 2;
			if (pDoc->lspc.pQual[i].gqsdfg == 1) pDoc->lspc.gqsdfg = 1;
			if (pDoc->lspc.pQual[i].potfcfg != 1) pDoc->lspc.pQual[i].potfcfg = 2;
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.pQual == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0,pDoc->lspc.pQual[i].gid);
			m_Grid.SetItemInt(nRow,  1,pDoc->lspc.pQual[i].qualid);
			m_Grid.SetItemText(nRow, 2,pDoc->lspc.pQual[i].qname);
			m_Grid.SetItemText(nRow, 3,pDoc->lspc.pQual[i].qunit);
			m_Grid.SetItemInt(nRow,  4,pDoc->lspc.pQual[i].qsdfg);
			m_Grid.SetItemInt(nRow,  5,pDoc->lspc.pQual[i].gqsdfg);
			m_Grid.SetItemInt(nRow,  6,pDoc->lspc.pQual[i].qsofg);
			m_Grid.SetItemInt(nRow,  7,pDoc->lspc.pQual[i].potfcfg);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
