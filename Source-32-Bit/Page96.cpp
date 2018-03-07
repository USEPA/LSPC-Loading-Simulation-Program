// Page96.cpp: implementation of the CPage96 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page96.h"
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

CPage96::CPage96()
{

}

CPage96::CPage96(UINT nID) : CPageAdd(nID)
{

}

CPage96::~CPage96()
{

}

void CPage96::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(11);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] ={"defid","deluid","pack-snow","pack-ice","pack-watr","rdenpf","dull","paktmp","covinx","xlnmlt","skyclr"}; // column name

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

BOOL CPage96::UpdateData( BOOL bSaveAndValidate)
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
		if(pDoc->lspc.snowfg == 0)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != pDoc->lspc.nlandp*pDoc->lspc.ngroup )
		{
			AfxMessageBox("Card96: Number of rows should be equal to nlandp*ngroup!");
			Log("Card96: Number of rows should be equal to nlandp*ngroup!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.pSnowInit1 != NULL)
			delete [](pDoc->lspc.pSnowInit1);

		if (pDoc->lspc.nlandp*pDoc->lspc.ngroup > 0)
			pDoc->lspc.pSnowInit1 = new CSnowInit1[pDoc->lspc.nlandp*pDoc->lspc.ngroup];
		
		for(int i = 0; i < pDoc->lspc.nlandp*pDoc->lspc.ngroup; ++i)
		{
			pDoc->lspc.pSnowInit1[i].nGrpid   = m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.pSnowInit1[i].luid     = m_Grid.GetItemInt(i+1,  1);
			pDoc->lspc.pSnowInit1[i].packsnow = m_Grid.GetItemFloat(i+1,2);
			pDoc->lspc.pSnowInit1[i].packice  = m_Grid.GetItemFloat(i+1,3);
			pDoc->lspc.pSnowInit1[i].packwatr = m_Grid.GetItemFloat(i+1,4);
			pDoc->lspc.pSnowInit1[i].RDENPF   = m_Grid.GetItemFloat(i+1,5);
			pDoc->lspc.pSnowInit1[i].DULL     = m_Grid.GetItemFloat(i+1,6);
			pDoc->lspc.pSnowInit1[i].PAKTMP   = m_Grid.GetItemFloat(i+1,7);
			pDoc->lspc.pSnowInit1[i].COVINX   = m_Grid.GetItemFloat(i+1,8);
			pDoc->lspc.pSnowInit1[i].XLNMLT   = m_Grid.GetItemFloat(i+1,9);
			pDoc->lspc.pSnowInit1[i].SKYCLR   = m_Grid.GetItemFloat(i+1,10);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.pSnowInit1 == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < pDoc->lspc.nlandp *pDoc->lspc.ngroup; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0, pDoc->lspc.pSnowInit1[i].nGrpid);
			m_Grid.SetItemInt(nRow,  1, pDoc->lspc.pSnowInit1[i].luid);
			m_Grid.SetItemFloat(nRow,2, pDoc->lspc.pSnowInit1[i].packsnow);
			m_Grid.SetItemFloat(nRow,3, pDoc->lspc.pSnowInit1[i].packice);
			m_Grid.SetItemFloat(nRow,4, pDoc->lspc.pSnowInit1[i].packwatr);
			m_Grid.SetItemFloat(nRow,5, pDoc->lspc.pSnowInit1[i].RDENPF);
			m_Grid.SetItemFloat(nRow,6, pDoc->lspc.pSnowInit1[i].DULL);
			m_Grid.SetItemFloat(nRow,7, pDoc->lspc.pSnowInit1[i].PAKTMP);
			m_Grid.SetItemFloat(nRow,8, pDoc->lspc.pSnowInit1[i].COVINX);
			m_Grid.SetItemFloat(nRow,9, pDoc->lspc.pSnowInit1[i].XLNMLT);
			m_Grid.SetItemFloat(nRow,10,pDoc->lspc.pSnowInit1[i].SKYCLR);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
