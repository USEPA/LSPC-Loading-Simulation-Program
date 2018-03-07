// Page451.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page451.h"
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

CPage451::CPage451()
{

}

CPage451::CPage451(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage451::~CPage451()
{

}

void CPage451::InitGrid()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);

	CLSPCDoc *pDoc = pView->GetDocument();

	int nNumSed = pDoc->lspc.NumSed;

	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(5+nNumSed);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString *str = NULL;
	str = new CString[5+nNumSed];
	str[0]= "defid";
	str[1]= "deluid";
	str[2]= "sed_suro";
	str[3]= "sed_ifwo";
	str[4]= "sed_agwo";
	for (int j=0; j<nNumSed; j++)
		str[j+5].Format("sed_%d", j+1);

	UINT *format = NULL;
	format = new UINT[5+nNumSed];
	format[0]= DT_RIGHT;
	format[1]= DT_RIGHT;
	format[2]= DT_RIGHT;
	format[3]= DT_RIGHT;
	format[4]= DT_RIGHT;
	for (j=0; j<nNumSed; j++)
		format[j+5]= DT_RIGHT;

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;

	for (int col=0; col<m_Grid.GetColumnCount(); col++)
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

BOOL CPage451::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;

	int nNum = pDoc->lspc.nlandp*pDoc->lspc.ngroup;

	if(bSaveAndValidate)
	{
		if(pDoc->lspc.sedfg !=1)
			return true;
		
		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 451: Number of rows should be equal to nlanp*ngroup in Card 451!");
			Log("Card 451: Number of rows should be equal to nlanp*ngroup in Card 451!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.psd_param2 != NULL)
			delete [](pDoc->lspc.psd_param2);

		if (nNum > 0)
			pDoc->lspc.psd_param2 = new SD_PARA2[nNum];
		
		for(int i = 0; i < nNum; ++i)
		{
			pDoc->lspc.psd_param2[i].gid        = m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.psd_param2[i].lid		= m_Grid.GetItemInt(i+1,  1);
			pDoc->lspc.psd_param2[i].sed_suro   = m_Grid.GetItemFloat(i+1,2);
			pDoc->lspc.psd_param2[i].sed_ifwo   = m_Grid.GetItemFloat(i+1,3);
			pDoc->lspc.psd_param2[i].sed_agwo   = m_Grid.GetItemFloat(i+1,4);

			if (pDoc->lspc.NumSed > 0)
				pDoc->lspc.psd_param2[i].sed = new double[pDoc->lspc.NumSed];
			
			for(int j=0; j<pDoc->lspc.NumSed; j++)
				pDoc->lspc.psd_param2[i].sed[j] = m_Grid.GetItemFloat(i+1,j+5);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.psd_param2 == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0,pDoc->lspc.psd_param2[i].gid);
			m_Grid.SetItemInt(nRow,  1,pDoc->lspc.psd_param2[i].lid);
			m_Grid.SetItemFloat(nRow,2,pDoc->lspc.psd_param2[i].sed_suro);
			m_Grid.SetItemFloat(nRow,3,pDoc->lspc.psd_param2[i].sed_ifwo);
			m_Grid.SetItemFloat(nRow,4,pDoc->lspc.psd_param2[i].sed_agwo);
		
			for(int j=0; j<pDoc->lspc.NumSed; j++)
				m_Grid.SetItemFloat(nRow,5+j,pDoc->lspc.psd_param2[i].sed[j]);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
