// Page452.cpp: implementation of the CPage452 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page452.h"
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

CPage452::CPage452()
{

}

CPage452::CPage452(UINT nID) : CPageAdd(nID)
{

}


CPage452::~CPage452()
{

}

void CPage452::InitGrid()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);

	CLSPCDoc *pDoc = pView->GetDocument();

	int nNumSed = pDoc->lspc.NumSed;

	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(3+nNumSed);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString *str = NULL;
	str = new CString[3+nNumSed];
	str[0]= "defid";
	str[1]= "dwqid";
	str[2]= "lutype";
	for (int j=0; j<nNumSed; j++)
		str[3+j].Format("sed_%d", j+1);

	UINT *format = NULL;
	format = new UINT[3+nNumSed];
	format[0]= DT_RIGHT;
	format[1]= DT_RIGHT;
	format[2]= DT_RIGHT;
	for (j=0; j<nNumSed; j++)
		format[3+j]= DT_RIGHT;

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

BOOL CPage452::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;
	
	int nNum = pDoc->lspc.ngroup*4;

	if(bSaveAndValidate)
	{
		if(pDoc->lspc.sedfg == 0 || pDoc->lspc.pqalfg == 0 || pDoc->lspc.gqsdfg == 0)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 452: Number of rows should be equal to ngroup*4 in Card 452!");
			Log("Card 452: Number of rows should be equal to ngroup*4 in Card 452!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.sed_mapp != NULL)
			delete [](pDoc->lspc.sed_mapp);

		if (nNum > 0)
			pDoc->lspc.sed_mapp = new SEDGQUAL_MAPP[nNum];
		for(int i = 0; i < nNum; ++i)
		{
			pDoc->lspc.sed_mapp[i].gid   = m_Grid.GetItemInt(i+1, 0);
			pDoc->lspc.sed_mapp[i].qualid = m_Grid.GetItemInt(i+1,1);
			pDoc->lspc.sed_mapp[i].lutype = m_Grid.GetItemInt(i+1,2);
			
			if (pDoc->lspc.NumSed > 0)
				pDoc->lspc.sed_mapp[i].sed = new double[pDoc->lspc.NumSed];

			for(int j=0; j<pDoc->lspc.NumSed; j++)
				pDoc->lspc.sed_mapp[i].sed[j] = m_Grid.GetItemFloat(i+1,j+3);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.sed_mapp == NULL)
			return true;
		
		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow,size.cy);
			m_Grid.SetItemInt(nRow,   0,pDoc->lspc.sed_mapp[i].gid);
			m_Grid.SetItemInt(nRow,   1,pDoc->lspc.sed_mapp[i].qualid);
			m_Grid.SetItemInt(nRow,   2,pDoc->lspc.sed_mapp[i].lutype);

			for(int j=0; j<pDoc->lspc.NumSed; j++)
				m_Grid.SetItemFloat(nRow,3+j,pDoc->lspc.sed_mapp[i].sed[j]);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
