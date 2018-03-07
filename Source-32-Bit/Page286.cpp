// Page286.cpp: implementation of the CPage286 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page286.h"
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

CPage286::CPage286()
{

}

CPage286::CPage286(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage286::~CPage286()
{

}

void CPage286::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(8);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] = {"rgid","dwqid","adpm1","adpm2","adpm3","adpm4","adpm5","adpm6"}; // column name
	
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

BOOL CPage286::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;
	
	int nNum = pDoc->lspc.nrgroup*pDoc->lspc.nQuals;
	
	if(bSaveAndValidate)
	{
		if(pDoc->lspc.pqalfg != 1 || pDoc->lspc.sedfg != 1)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 286: Row number should be equal to nrgroup*nquals.");
			Log("Card 286: Row number should be equal to nrgroup*nquals.");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.pGQ_KD != NULL)
			delete [](pDoc->lspc.pGQ_KD);

		if (nNum > 0)
			pDoc->lspc.pGQ_KD = new GQ_KD[nNum];

		for(int i = 0; i < nNum; ++i)
		{
			pDoc->lspc.pGQ_KD[i].gid = m_Grid.GetItemInt(i+1,   0);
			pDoc->lspc.pGQ_KD[i].qualid = m_Grid.GetItemInt(i+1,1);
			for (int j=0; j<6; j++)
				pDoc->lspc.pGQ_KD[i].adpm[j] = m_Grid.GetItemFloat(i+1,j+2);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.pGQ_KD == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0, pDoc->lspc.pGQ_KD[i].gid);
			m_Grid.SetItemInt(nRow,  1, pDoc->lspc.pGQ_KD[i].qualid);
			for (int j=0; j<6; j++)
				m_Grid.SetItemFloat(nRow,j+2, pDoc->lspc.pGQ_KD[i].adpm[j]);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
