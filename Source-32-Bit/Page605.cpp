// Page605.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page605.h"
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

CPage605::CPage605()
{

}

CPage605::CPage605(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage605::~CPage605()
{

}

void CPage605::InitGrid()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);

	CLSPCDoc *pDoc = pView->GetDocument();

	int maxTQID = 0;
	int nNum = pDoc->lspc.ntnum;

	for(int i=0; i<nNum; i++)
	{
		int count = pDoc->lspc.pThreshold[i].tqnum;
		if (count > maxTQID)
			maxTQID = count;
	}
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(3+maxTQID);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString *str = NULL;
	str = new CString[3+maxTQID];
	str[0]= "threshold number";
	str[1]= "threshold qual";
	str[2]= "number of quals";
	for (int j=0; j<maxTQID; j++)
		str[j+3].Format("qual_%d", j+1);

	UINT *format = NULL;
	format = new UINT[3+maxTQID];
	format[0]= DT_RIGHT;
	format[1]= DT_RIGHT;
	format[2]= DT_RIGHT;
	for (j=0; j<maxTQID; j++)
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

BOOL CPage605::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;

	int nNum = pDoc->lspc.ntnum;

	if(bSaveAndValidate)
	{
		if (nNum <= 0)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 605: Number of rows should be equal to number of TNUM!");
			Log("Card 605: Number of rows should be equal to number of TNUM!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.pThreshold != NULL)
		{
			delete [](pDoc->lspc.pThreshold);
			pDoc->lspc.pThreshold = NULL;
		}
		if (nNum > 0)
			pDoc->lspc.pThreshold = new THR_MAPPING[nNum];
		
		for(int i=0; i<nNum; ++i)
		{
			pDoc->lspc.pThreshold[i].tnum  = m_Grid.GetItemInt(i+1,0);
			pDoc->lspc.pThreshold[i].tqsd  = m_Grid.GetItemInt(i+1,1);
			pDoc->lspc.pThreshold[i].tqnum = m_Grid.GetItemInt(i+1,2);

			//allocate memory here 
			if (pDoc->lspc.pThreshold[i].tqnum > 0)
			{
				pDoc->lspc.pThreshold[i].tqid = new int[pDoc->lspc.pThreshold[i].tqnum];
				for(int j=0; j<pDoc->lspc.pThreshold[i].tqnum; j++)
					pDoc->lspc.pThreshold[i].tqid[j] = m_Grid.GetItemInt(i+1,j+3);
			}
			//check the tqsd value
			if (pDoc->lspc.pThreshold[i].tqsd != 1)
				pDoc->lspc.pThreshold[i].tqsd = 2;
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.pThreshold == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i=0; i<nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,0,pDoc->lspc.pThreshold[i].tnum);
			m_Grid.SetItemInt(nRow,1,pDoc->lspc.pThreshold[i].tqsd);
			m_Grid.SetItemInt(nRow,2,pDoc->lspc.pThreshold[i].tqnum);
		
			for(int j=0; j<pDoc->lspc.pThreshold[i].tqnum; j++)
				m_Grid.SetItemInt(nRow,3+j,pDoc->lspc.pThreshold[i].tqid[j]);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
