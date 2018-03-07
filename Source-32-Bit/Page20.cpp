// Page20.cpp: implementation of the CPage20 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page20.h"
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

CPage20::CPage20()
{

}

CPage20::CPage20(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage20::~CPage20()
{

}

void CPage20::InitGrid()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);

	CLSPCDoc *pDoc = pView->GetDocument();

	int nNumParam = pDoc->lspc.cWD.nNumWeatherParam;

	m_Grid.SetRowCount(1);
//	m_Grid.SetColumnCount(3);
	m_Grid.SetColumnCount(1+nNumParam);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

//	CString str[] = {"Station ID","Data File Name","Station Name"}; // column name
//	UINT format[] = {DT_RIGHT,DT_LEFT,DT_LEFT}; // column format

	CString *str = NULL;
	str = new CString[1+nNumParam];
	str[0]= "station id";
	for (int j=0; j<nNumParam; j++)
		str[j+1].Format("multiplier_%d", j+1);

	UINT *format = NULL;
	format = new UINT[1+nNumParam];
	for (j=0; j<1+nNumParam; j++)
		format[j]= DT_RIGHT;
	
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

BOOL CPage20::UpdateData( BOOL bSaveAndValidate)
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
		int nRow = m_Grid.GetRowCount() - 1;
		if(pDoc->lspc.cWD.pWSTATIONINFO == NULL)
			return true;

		for(int i=0; i<pDoc->lspc.cWD.nNumWeatherStations; i++)
		{
			int nID = m_Grid.GetItemInt(i+1,0);
			//find the station index
			int nIndex = pDoc->lspc.cWD.FindStationIndex(nID);

			for (int j=0; j<pDoc->lspc.cWD.nNumWeatherParam; j++)
				pDoc->lspc.cWD.pWSTATIONINFO[nIndex].m_lfMultiplier[j] = m_Grid.GetItemFloat(i+1, j+1);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.cWD.pWSTATIONINFO == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i=0; i<pDoc->lspc.cWD.nNumWeatherStations; i++)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,0,pDoc->lspc.cWD.pWSTATIONINFO[i].nWStationID);
			for (int j=0; j<pDoc->lspc.cWD.nNumWeatherParam; j++)
				m_Grid.SetItemFloat(nRow,1+j,pDoc->lspc.cWD.pWSTATIONINFO[i].m_lfMultiplier[j]);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
