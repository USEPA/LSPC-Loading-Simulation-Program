// Page457.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page457.h"
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

CPage457::CPage457()
{

}

CPage457::CPage457(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage457::~CPage457()
{

}

void CPage457::InitGrid()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);

	CLSPCDoc *pDoc = pView->GetDocument();

	int nNumSed = pDoc->lspc.NumSed;

	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(4+nNumSed);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString *str = NULL;
	str = new CString[4+nNumSed];
	str[0]= "rchid";
	str[1]= "kber";
	str[2]= "jber";
	str[3]= "qber";
	for (int j=0; j<nNumSed; j++)
		str[j+4].Format("sed_%d", j+1);

	UINT *format = NULL;
	format = new UINT[4+nNumSed];
	format[0]= DT_RIGHT;
	format[1]= DT_RIGHT;
	format[2]= DT_RIGHT;
	format[3]= DT_RIGHT;
	for (j=0; j<nNumSed; j++)
		format[j+4]= DT_RIGHT;

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

BOOL CPage457::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;
	
	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;

	int nNum = pDoc->lspc.nch;

	if(bSaveAndValidate)
	{
		if(pDoc->lspc.sedfg !=1 || pDoc->lspc.sedber != 1)
			return true;
		
		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 457:Number of rows should be equal to nch!");
			Log("Card 457:Number of rows should be equal to nch!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.rsd_berparam != NULL)
			delete [](pDoc->lspc.rsd_berparam);

		if (nNum > 0)
			pDoc->lspc.rsd_berparam = new SD_BER[nNum];
		
		for(int i = 0; i < nNum; ++i)
		{
			pDoc->lspc.rsd_berparam[i].rchid = m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.rsd_berparam[i].kber  = m_Grid.GetItemFloat(i+1,1);
			pDoc->lspc.rsd_berparam[i].jber  = m_Grid.GetItemFloat(i+1,2);
			pDoc->lspc.rsd_berparam[i].qber  = m_Grid.GetItemFloat(i+1,3);

			if (pDoc->lspc.NumSed > 0)
				pDoc->lspc.rsd_berparam[i].sed = new double[pDoc->lspc.NumSed];
			
			for(int j=0; j<pDoc->lspc.NumSed; j++)
				pDoc->lspc.rsd_berparam[i].sed[j] = m_Grid.GetItemFloat(i+1,j+4);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.rsd_berparam == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0,pDoc->lspc.rsd_berparam[i].rchid);
			m_Grid.SetItemFloat(nRow,1,pDoc->lspc.rsd_berparam[i].kber);
			m_Grid.SetItemFloat(nRow,2,pDoc->lspc.rsd_berparam[i].jber);
			m_Grid.SetItemFloat(nRow,3,pDoc->lspc.rsd_berparam[i].qber);
		
			for(int j=0; j<pDoc->lspc.NumSed; j++)
				m_Grid.SetItemFloat(nRow,4+j,pDoc->lspc.rsd_berparam[i].sed[j]);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}

