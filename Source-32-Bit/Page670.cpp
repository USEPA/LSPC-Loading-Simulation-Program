// Page670.cpp: implementation of the CPage670 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page670.h"
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

CPage670::CPage670()
{

}

CPage670::CPage670(UINT nID) : CPageAdd(nID)
{

}

CPage670::~CPage670()
{

}

void CPage670::InitGrid()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return ;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;

	int nNum = pDoc->lspc.nQuals;
	int Num  = nNum;
	if (pDoc->lspc.sedfg == 1)
		Num += 3;	// sand, silt, and clay

	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(4+Num);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString *str = NULL;
	str = new CString[4+Num];
	str[0] = "Subbasin";
	str[1] = "Landuse ID";
	str[2] = "Landuse Name";					
	str[3] = "reduction_flow";					
	for (int j=0; j<nNum; j++)
		str[j+4].Format("reduction_%s", pDoc->lspc.pQual[j].qname);

	if (pDoc->lspc.sedfg == 1)
	{
		str[4+nNum+0] = "reduction_sand";
		str[4+nNum+1] = "reduction_silt";
		str[4+nNum+2] = "reduction_clay";
	}

	UINT *format = NULL;
	format = new UINT[4+Num];
	format[0]= DT_RIGHT;
	format[1]= DT_RIGHT;
	format[2]= DT_LEFT;
	format[3]= DT_RIGHT;
	for (j=0; j<Num; j++)
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

BOOL CPage670::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;
	
	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;

	int Num = pDoc->lspc.nQuals + 1;	// flow is always there
	if (pDoc->lspc.sedfg == 1)
		Num += 3;	// sand, silt, and clay

	int nNum = pDoc->lspc.nsws*pDoc->lspc.nlandp;

	if(bSaveAndValidate)
	{
		if(pDoc->lspc.ncland == 0)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 670: Number of rows should be equal to nsws*nlandp!");
			Log("Card 670: Number of rows should be equal to nsws*nlandp!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.pTMDLDistrib != NULL)
			delete [](pDoc->lspc.pTMDLDistrib);

		if (nNum*Num > 0)
			pDoc->lspc.pTMDLDistrib = new TMDL_LU_QUAL_CPARAM[nNum*Num];
		
		for(int i=0; i<nNum; ++i)
		{
			pDoc->lspc.pTMDLDistrib[i*Num].subbasin  = m_Grid.GetItemInt(i+1, 0);
			pDoc->lspc.pTMDLDistrib[i*Num].luid      = m_Grid.GetItemInt(i+1, 1);
			pDoc->lspc.pTMDLDistrib[i*Num].strLUName = m_Grid.GetItemText(i+1,2);
			for (int j=0; j<Num; ++j)
			{
				pDoc->lspc.pTMDLDistrib[i*Num+j].subbasin  = pDoc->lspc.pTMDLDistrib[i*Num].subbasin;
				pDoc->lspc.pTMDLDistrib[i*Num+j].luid      = pDoc->lspc.pTMDLDistrib[i*Num].luid;
				pDoc->lspc.pTMDLDistrib[i*Num+j].strLUName = pDoc->lspc.pTMDLDistrib[i*Num].strLUName;
				pDoc->lspc.pTMDLDistrib[i*Num+j].fRatio    = m_Grid.GetItemFloat(i+1,3+j);
			}
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.pTMDLDistrib == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i=0; i<nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0,pDoc->lspc.pTMDLDistrib[i*Num].subbasin);
			m_Grid.SetItemInt(nRow,  1,pDoc->lspc.pTMDLDistrib[i*Num].luid);
			m_Grid.SetItemText(nRow, 2,pDoc->lspc.pTMDLDistrib[i*Num].strLUName);
			for (int j=0; j<Num; ++j)
				m_Grid.SetItemFloat(nRow,3+j,pDoc->lspc.pTMDLDistrib[i*Num+j].fRatio);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
