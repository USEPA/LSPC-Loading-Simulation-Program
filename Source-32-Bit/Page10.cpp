// Page10.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page10.h"
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

CPage10::CPage10()
{

}

CPage10::CPage10(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage10::~CPage10()
{

}

void CPage10::InitGrid()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);

	CLSPCDoc *pDoc = pView->GetDocument();

//	if (pDoc->lspc.cWD.pWFILEINFO == NULL)
//		return;

	int maxWParam = 0;
	int nNum = pDoc->lspc.cWD.nNumWeatherFiles;

	for(int i=0; i<nNum; i++)
	{
		int count = pDoc->lspc.cWD.pWFILEINFO[i].nWParam;
		if (count>maxWParam)
			maxWParam = count;
	}
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(3+maxWParam);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString *str = NULL;
	str = new CString[3+maxWParam];
	str[0]= "file id";
	str[1]= "file name";
	str[2]= "number of params";
	for (int j=0; j<maxWParam; j++)
		str[j+3].Format("param_%d", j+1);

	UINT *format = NULL;
	format = new UINT[3+maxWParam];
	format[0]= DT_RIGHT;
	format[1]= DT_LEFT;
	format[2]= DT_RIGHT;
	for (j=3; j<3+maxWParam; j++)
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

BOOL CPage10::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;

	int nNum = pDoc->lspc.cWD.nNumWeatherFiles;

	if(bSaveAndValidate)
	{
		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 10: Number of rows should be equal to number of weather files!");
			Log("Card 10: Number of rows should be equal to weather files!");
			return false;
		}

		// delete the old one
//		if(pDoc->lspc.cWD.pWFILEINFO != NULL)
//			delete [](pDoc->lspc.cWD.pWFILEINFO);
//		pDoc->lspc.cWD.pWFILEINFO = new WFILEINFO[nNum];

		if(pDoc->lspc.cWD.pWFILEINFO == NULL)
			return true;
		
		for(int i=0; i<nNum; i++)
		{
			pDoc->lspc.cWD.pWFILEINFO[i].nWFileID = m_Grid.GetItemInt(i+1,0);
			pDoc->lspc.cWD.pWFILEINFO[i].strWFile = m_Grid.GetItemText(i+1,1);
			pDoc->lspc.cWD.pWFILEINFO[i].nWParam = m_Grid.GetItemInt(i+1,2);

			//allocate memory here   
//			if (pDoc->lspc.cWD.pWFILEINFO[i].nWParam > 0)
//				pDoc->lspc.cWD.pWFILEINFO[i].m_nParamID = new int[pDoc->lspc.cWD.pWFILEINFO[i].nWParam];

			// initialize
			for(int j=0; j<pDoc->lspc.cWD.pWFILEINFO[i].nWParam; j++)
				pDoc->lspc.cWD.pWFILEINFO[i].m_nParamID[j] = m_Grid.GetItemInt(i+1,j+3);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.cWD.pWFILEINFO == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i=0; i<nNum; i++)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0,pDoc->lspc.cWD.pWFILEINFO[i].nWFileID);
			m_Grid.SetItemText(nRow, 1,pDoc->lspc.cWD.pWFILEINFO[i].strWFile);
			m_Grid.SetItemInt(nRow,	 2,pDoc->lspc.cWD.pWFILEINFO[i].nWParam);
		
			for(int j=0; j<pDoc->lspc.cWD.pWFILEINFO[i].nWParam; j++)
				m_Grid.SetItemInt(nRow,3+j,pDoc->lspc.cWD.pWFILEINFO[i].m_nParamID[j]);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}

