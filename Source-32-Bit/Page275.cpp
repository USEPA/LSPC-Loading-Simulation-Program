// Page275.cpp: implementation of the CPage275 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page275.h"
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

CPage275::CPage275()
{

}

CPage275::CPage275(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage275::~CPage275()
{

}

void CPage275::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(15);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] ={"defid","dwqid","deluid","jan","feb","mar","apr","may","jun","jul","aug","sept","oct","nov","dec"}; // column name

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

BOOL CPage275::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;
	
	int nNum = pDoc->lspc.nlandp*pDoc->lspc.ngroup*pDoc->lspc.nQuals;
	
	if(bSaveAndValidate)
	{
		if(pDoc->lspc.maddfglnd != 1)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 275: Row number should be equal to ngroup*nQuals*nlandp.");
			Log("Card 275: Row number should be equal to ngroup*nQuals*nlandp.");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.pIMonDRYDEP != NULL)
			delete [](pDoc->lspc.pIMonDRYDEP);

		if (nNum > 0)
			pDoc->lspc.pIMonDRYDEP = new IMON_DATE[nNum];

		for(int i = 0; i < nNum; ++i)
		{
			pDoc->lspc.pIMonDRYDEP[i].gid    = m_Grid.GetItemInt(i+1,0);
			pDoc->lspc.pIMonDRYDEP[i].qualid = m_Grid.GetItemInt(i+1,1);
			pDoc->lspc.pIMonDRYDEP[i].lid    = m_Grid.GetItemInt(i+1,2);
			for(int j=0; j<12; ++j)
				pDoc->lspc.pIMonDRYDEP[i].fnum[j] = m_Grid.GetItemFloat(i+1,j+3);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.pIMonDRYDEP == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i=0; i<nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow,size.cy);
			m_Grid.SetItemInt(nRow,0,pDoc->lspc.pIMonDRYDEP[i].gid);
			m_Grid.SetItemInt(nRow,1,pDoc->lspc.pIMonDRYDEP[i].qualid);
			m_Grid.SetItemInt(nRow,2,pDoc->lspc.pIMonDRYDEP[i].lid);
			for(int j= 0; j< 12;++j)
				m_Grid.SetItemFloat(nRow,3+j,pDoc->lspc.pIMonDRYDEP[i].fnum[j]);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
