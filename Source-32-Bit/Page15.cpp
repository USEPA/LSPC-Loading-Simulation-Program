// Page15.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page15.h"
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

CPage15::CPage15()
{

}

CPage15::CPage15(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage15::~CPage15()
{

}

void CPage15::InitGrid()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);

	CLSPCDoc *pDoc = pView->GetDocument();

	int maxWFiles = 0;
	int nNum = pDoc->lspc.cWD.nNumWeatherStations;

	for(int i=0; i<nNum; i++)
	{
		int count = pDoc->lspc.cWD.pWSTATIONINFO[i].nWFile;
		if (count>maxWFiles)
			maxWFiles = count;
	}
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(2+maxWFiles);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString *str = NULL;
	str = new CString[2+maxWFiles];
	str[0]= "station id";
	str[1]= "number of files";
	for (int j=0; j<maxWFiles; j++)
		str[j+2].Format("file%d", j+1);

	UINT *format = NULL;
	format = new UINT[2+maxWFiles];
	format[0]= DT_RIGHT;
	format[1]= DT_RIGHT;
	for (j=2; j<2+maxWFiles; j++)
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

BOOL CPage15::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;

	int nNum = pDoc->lspc.cWD.nNumWeatherStations;

	if(bSaveAndValidate)
	{
		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 15: Number of rows should be equal to number of weather stations!");
			Log("Card 15: Number of rows should be equal to weather stations!");
			return false;
		}

		// delete the old one
//		if(pDoc->lspc.cWD.pWSTATIONINFO != NULL)
//			delete [](pDoc->lspc.cWD.pWSTATIONINFO);
//		pDoc->lspc.cWD.pWSTATIONINFO = new WSTATIONINFO[nNum];

		if(pDoc->lspc.cWD.pWSTATIONINFO == NULL)
			return true;
		
		for(int i=0; i<nNum; i++)
		{
			pDoc->lspc.cWD.pWSTATIONINFO[i].nWStationID = m_Grid.GetItemInt(i+1,0);
			pDoc->lspc.cWD.pWSTATIONINFO[i].nWFile = m_Grid.GetItemInt(i+1,1);

			//allocate memory here   
//			if (pDoc->lspc.cWD.pWSTATIONINFO[i].nWFile > 0)
//				pDoc->lspc.cWD.pWSTATIONINFO[i].m_nWFileIndex = new int[pDoc->lspc.cWD.pWSTATIONINFO[i].nWFile];

			// initialize
			for(int j=0; j<pDoc->lspc.cWD.pWSTATIONINFO[i].nWFile; j++)
			{
				int nID = m_Grid.GetItemInt(i+1,j+2);
				pDoc->lspc.cWD.pWSTATIONINFO[i].m_nWFileIndex[j] = pDoc->lspc.cWD.FindFileIndex(nID);
			}
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.cWD.pWSTATIONINFO == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i=0; i<nNum; i++)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0,pDoc->lspc.cWD.pWSTATIONINFO[i].nWStationID);
			m_Grid.SetItemInt(nRow,	 1,pDoc->lspc.cWD.pWSTATIONINFO[i].nWFile);
		
			for(int j=0; j<pDoc->lspc.cWD.pWSTATIONINFO[i].nWFile; j++)
			{
				int nIndex = pDoc->lspc.cWD.pWSTATIONINFO[i].m_nWFileIndex[j];
				m_Grid.SetItemInt(nRow,2+j,pDoc->lspc.cWD.pWFILEINFO[nIndex].nWFileID);
			}
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}

