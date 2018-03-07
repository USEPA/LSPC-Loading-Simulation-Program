// Page80.cpp: implementation of the CPage80 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page80.h"
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

CPage80::CPage80()
{

}

CPage80::CPage80(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage80::~CPage80()
{

}

void CPage80::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(5);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] = {"defid", "deluid", "route_suro", "route_ifwo", "route_agwo"}; // column name
	UINT format[] = {DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT}; // column format

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;

	for(int col=0; col<m_Grid.GetColumnCount(); col++)
	{
		Item.col = col;
		Item.nFormat = format[col]|DT_VCENTER|DT_SINGLELINE;
		Item.szText = str[col];
		m_Grid.SetItem(&Item);
	}
	m_Grid.AutoSize();
}

BOOL CPage80::UpdateData(BOOL bSaveAndValidate)
{
	// set the active page
	CMainFrame *pMain = (CMainFrame *) AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *) pMain->m_wndSplitter.GetPane(0, 1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;

	if(bSaveAndValidate)
	{
		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != pDoc->lspc.ngroup*pDoc->lspc.nlandp)
		{
			AfxMessageBox("Card 80: Number of rows should be equal to ngroup*nlandp in Card 80!");
			Log("Card 80: Number of rows should be equal to ngroup*nlandp in Card 80!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.pLURoute != NULL)
			delete []pDoc->lspc.pLURoute;

		if (pDoc->lspc.nlandp*pDoc->lspc.ngroup > 0)
			pDoc->lspc.pLURoute = new LURoute[pDoc->lspc.ngroup*pDoc->lspc.nlandp];

		for(int i=0; i<nRow; i++)
		{
			pDoc->lspc.pLURoute[i].gid = m_Grid.GetItemInt(i+1,0);
			pDoc->lspc.pLURoute[i].luid = m_Grid.GetItemInt(i+1,1);
			pDoc->lspc.pLURoute[i].input_suro = m_Grid.GetItemFloat(i+1,2);
			pDoc->lspc.pLURoute[i].input_ifwo = m_Grid.GetItemFloat(i+1,3);
			pDoc->lspc.pLURoute[i].input_agwo = m_Grid.GetItemFloat(i+1,4);

			//validate the route_suro
			if (pDoc->lspc.pLURoute[i].input_suro >= 1)
			{
				pDoc->lspc.pLURoute[i].route_suro = 1;
				pDoc->lspc.pLURoute[i].bypass_suro = 0;
			}
			else if (pDoc->lspc.pLURoute[i].input_suro <= -1)
			{
				pDoc->lspc.pLURoute[i].route_suro = 0;
				pDoc->lspc.pLURoute[i].bypass_suro = 0;
			}
			else if(pDoc->lspc.pLURoute[i].input_suro >= 0)
			{
				pDoc->lspc.pLURoute[i].route_suro = pDoc->lspc.pLURoute[i].input_suro;
				pDoc->lspc.pLURoute[i].bypass_suro = 1-pDoc->lspc.pLURoute[i].route_suro;
			}
			else // it is negative (by pass water is lost from the system)
			{
				pDoc->lspc.pLURoute[i].route_suro = 1+pDoc->lspc.pLURoute[i].input_suro;
				pDoc->lspc.pLURoute[i].bypass_suro = 0;
			}

			//validate the route_ifwo
			if (pDoc->lspc.pLURoute[i].input_ifwo >= 1)
			{
				pDoc->lspc.pLURoute[i].route_ifwo = 1;
				pDoc->lspc.pLURoute[i].bypass_ifwo = 0;
			}
			else if (pDoc->lspc.pLURoute[i].input_ifwo <= -1)
			{
				pDoc->lspc.pLURoute[i].route_ifwo = 0;
				pDoc->lspc.pLURoute[i].bypass_ifwo = 0;
			}
			else if(pDoc->lspc.pLURoute[i].input_ifwo >= 0)
			{
				pDoc->lspc.pLURoute[i].route_ifwo = pDoc->lspc.pLURoute[i].input_ifwo;
				pDoc->lspc.pLURoute[i].bypass_ifwo = 1-pDoc->lspc.pLURoute[i].route_ifwo;
			}
			else // it is negative (by pass water is lost from the system)
			{
				pDoc->lspc.pLURoute[i].route_ifwo = 1+pDoc->lspc.pLURoute[i].input_ifwo;
				pDoc->lspc.pLURoute[i].bypass_ifwo = 0;
			}

			//validate the route_agwo
			if (pDoc->lspc.pLURoute[i].input_agwo >= 1)
			{
				pDoc->lspc.pLURoute[i].route_agwo = 1;
				pDoc->lspc.pLURoute[i].bypass_agwo = 0;
			}
			else if (pDoc->lspc.pLURoute[i].input_agwo <= -1)
			{
				pDoc->lspc.pLURoute[i].route_agwo = 0;
				pDoc->lspc.pLURoute[i].bypass_agwo = 0;
			}
			else if(pDoc->lspc.pLURoute[i].input_agwo >= 0)
			{
				pDoc->lspc.pLURoute[i].route_agwo = pDoc->lspc.pLURoute[i].input_agwo;
				pDoc->lspc.pLURoute[i].bypass_agwo = 1-pDoc->lspc.pLURoute[i].route_agwo;
			}
			else // it is negative (by pass water is lost from the system)
			{
				pDoc->lspc.pLURoute[i].route_agwo = 1+pDoc->lspc.pLURoute[i].input_agwo;
				pDoc->lspc.pLURoute[i].bypass_agwo = 0;
			}
		}
	}
	else // fill the blanks with data
	{
		InitGrid();

		if(pDoc->lspc.pLURoute == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i=0; i<pDoc->lspc.ngroup*pDoc->lspc.nlandp; i++)
		{
			m_Grid.InsertRow(_T(""), -1);
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0,pDoc->lspc.pLURoute[i].gid);
			m_Grid.SetItemInt(nRow,  1,pDoc->lspc.pLURoute[i].luid);
			m_Grid.SetItemFloat(nRow,2,pDoc->lspc.pLURoute[i].input_suro);
			m_Grid.SetItemFloat(nRow,3,pDoc->lspc.pLURoute[i].input_ifwo);
			m_Grid.SetItemFloat(nRow,4,pDoc->lspc.pLURoute[i].input_agwo);
		}

		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
