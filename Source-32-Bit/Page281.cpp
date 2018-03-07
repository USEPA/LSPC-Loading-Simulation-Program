// Page281.cpp: implementation of the CPage281 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page281.h"
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

CPage281::CPage281()
{

}

CPage281::CPage281(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage281::~CPage281()
{

}

void CPage281::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(9);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] = {"rgid","dwqid","qsdfg","ini_cond","decay","tcdecay","addc","awdc","potber"}; // column name
	UINT format[] = {DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT}; // column format

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

BOOL CPage281::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;
	
	int nNum = pDoc->lspc.nQuals *pDoc->lspc.nrgroup;

	if(bSaveAndValidate)
	{
		if (pDoc->lspc.pqalfg == 0)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;

		if(nRow != nNum)
		{
			AfxMessageBox("Card 281: Row number should be equal to nrgroup*nquals.");
			Log("Card 281: Row number should be equal to nrgroup*nquals.");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.pQualwq != NULL)
			delete [](pDoc->lspc.pQualwq);

		if (nRow > 0)
			pDoc->lspc.pQualwq = new QUALSWQ[nRow];
		
		for(int i = 0; i < nRow; ++i)
		{
			pDoc->lspc.pQualwq[i].gid    = m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.pQualwq[i].qualid = m_Grid.GetItemInt(i+1,  1);
			pDoc->lspc.pQualwq[i].qsdfg  = m_Grid.GetItemInt(i+1,  2);
			pDoc->lspc.pQualwq[i].inicon = m_Grid.GetItemFloat(i+1,3);
			pDoc->lspc.pQualwq[i].fstdec = m_Grid.GetItemFloat(i+1,4);
			pDoc->lspc.pQualwq[i].thfst  = m_Grid.GetItemFloat(i+1,5);
			pDoc->lspc.pQualwq[i].addc   = m_Grid.GetItemFloat(i+1,6);
			pDoc->lspc.pQualwq[i].awdc   = m_Grid.GetItemFloat(i+1,7);
			pDoc->lspc.pQualwq[i].potber = m_Grid.GetItemFloat(i+1,8);

			if (pDoc->lspc.pQualwq[i].thfst < 1) pDoc->lspc.pQualwq[i].thfst = 1.0;
			if (pDoc->lspc.pQualwq[i].thfst > 2) pDoc->lspc.pQualwq[i].thfst = 2.0;
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.pQualwq == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0,pDoc->lspc.pQualwq[i].gid);
			m_Grid.SetItemInt(nRow,  1,pDoc->lspc.pQualwq[i].qualid);
			m_Grid.SetItemInt(nRow,  2,pDoc->lspc.pQualwq[i].qsdfg);
			m_Grid.SetItemFloat(nRow,3,pDoc->lspc.pQualwq[i].inicon);
			m_Grid.SetItemFloat(nRow,4,pDoc->lspc.pQualwq[i].fstdec);
			m_Grid.SetItemFloat(nRow,5,pDoc->lspc.pQualwq[i].thfst);
			m_Grid.SetItemFloat(nRow,6,pDoc->lspc.pQualwq[i].addc);
			m_Grid.SetItemFloat(nRow,7,pDoc->lspc.pQualwq[i].awdc);
			m_Grid.SetItemFloat(nRow,8,pDoc->lspc.pQualwq[i].potber);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
