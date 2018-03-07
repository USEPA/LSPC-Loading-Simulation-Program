// Page446.cpp: implementation of the CPage446 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page446.h"
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

CPage446::CPage446()
{

}

CPage446::CPage446(UINT nID) : CPageAdd(nID)
{

}

CPage446::~CPage446()
{

}

void CPage446::InitGrid()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return ;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;

	int Num = pDoc->lspc.nQuals;

	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(3+Num);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString *str = NULL;
	str = new CString[3+Num];
	str[0] = "defid";
	str[1] = "deluid";
	str[2] = "luname";					
	for (int j=0; j<Num; j++)
		str[j+3].Format("effic_%s", pDoc->lspc.pQual[j].qname);

	UINT *format = NULL;
	format = new UINT[3+Num];
	format[0]= DT_RIGHT;
	format[1]= DT_RIGHT;
	format[2]= DT_LEFT;
	for (j=0; j<Num; j++)
		format[j+3]= DT_RIGHT;

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
	if (str != NULL)
		delete []str;
	if (format != NULL)
		delete []format;
}

BOOL CPage446::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;
	
	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;

	int Num = pDoc->lspc.nQuals;	
	int nNum = pDoc->lspc.nlandimp*pDoc->lspc.ngroup;

	if(bSaveAndValidate)
	{
		if(pDoc->lspc.sedfg != 1)
			return true;
		
		if(pDoc->lspc.pqalfg != 1)
			return true;
		
		if(pDoc->lspc.sweepfg != 2)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 446: Number of rows should be equal to nlandimp*ngroup!");
			Log("Card 446: Number of rows should be equal to nlandimp*ngroup!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.pGQ_sweep != NULL)
			delete [](pDoc->lspc.pGQ_sweep);

		if (nNum > 0)
			pDoc->lspc.pGQ_sweep = new GQ_SWEEP[nNum];
		
		for(int i=0; i<nNum; ++i)
		{
			pDoc->lspc.pGQ_sweep[i].defid = m_Grid.GetItemInt(i+1, 0);
			pDoc->lspc.pGQ_sweep[i].deluid = m_Grid.GetItemInt(i+1, 1);
			pDoc->lspc.pGQ_sweep[i].luname = m_Grid.GetItemText(i+1,2);
			if (Num > 0)
				pDoc->lspc.pGQ_sweep[i].effic_GQual = new double[Num];

			for (int j=0; j<Num; ++j)
				pDoc->lspc.pGQ_sweep[i].effic_GQual[j]  = m_Grid.GetItemFloat(i+1,j+3);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.pGQ_sweep == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i=0; i<nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0,pDoc->lspc.pGQ_sweep[i].defid);
			m_Grid.SetItemInt(nRow,  1,pDoc->lspc.pGQ_sweep[i].deluid);
			m_Grid.SetItemText(nRow, 2,pDoc->lspc.pGQ_sweep[i].luname);
			for (int j=0; j<Num; ++j)
				m_Grid.SetItemFloat(nRow,j+3,pDoc->lspc.pGQ_sweep[i].effic_GQual[j]);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
