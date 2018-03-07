// Page90.cpp: implementation of the CPage90 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page90.h"
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

CPage90::CPage90()
{

}

CPage90::CPage90(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage90::~CPage90()
{

}

void CPage90::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(7);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] = {"subbasin", "deluid", "deluname", "lutype", "area_ac", "slsur", "lsur"}; // column name
	UINT format[] = {DT_RIGHT,DT_RIGHT,DT_LEFT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT}; // column format

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

BOOL CPage90::UpdateData(BOOL bSaveAndValidate)
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
		if(nRow != pDoc->lspc.nlandp*pDoc->lspc.nsws)
		{
			AfxMessageBox("Card 90: Number of rows should be equal to nlandp*nsws in Card 4!");
			Log("Card 90: Number of rows should be equal to nlandp*nsws in Card 4!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.pluinfop != NULL)
			delete []pDoc->lspc.pluinfop;

		if (pDoc->lspc.nlandp*pDoc->lspc.nsws > 0)
			pDoc->lspc.pluinfop = new LUInfo[pDoc->lspc.nlandp*pDoc->lspc.nsws];

		int nIndex = -1;
		pDoc->lspc.nlandimp = 0;

		for(int i=0; i<nRow; i++)
		{
			pDoc->lspc.pluinfop[i].subbasin = m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.pluinfop[i].luid    = m_Grid.GetItemInt(i+1,   1);
			pDoc->lspc.pluinfop[i].luname  = m_Grid.GetItemText(i+1,  2);
			pDoc->lspc.pluinfop[i].piid    = m_Grid.GetItemInt(i+1,   3);
			pDoc->lspc.pluinfop[i].area    = m_Grid.GetItemFloat(i+1, 4);
			pDoc->lspc.pluinfop[i].slsur   = m_Grid.GetItemFloat(i+1, 5);
			pDoc->lspc.pluinfop[i].lsur    = m_Grid.GetItemFloat(i+1, 6);

			//flag no landuse change as default
			pDoc->lspc.pluinfop[i].bLUchange = false;
			pDoc->lspc.pluinfop[i].sindex = -1;
			pDoc->lspc.pluinfop[i].records = 0;
			pDoc->lspc.pluinfop[i].m_fileOut = NULL;
			
			//count the number of impervious land uses
			if (nIndex == -1)
				nIndex = pDoc->lspc.pluinfop[i].subbasin;
			if (pDoc->lspc.pluinfop[i].piid == 1 && pDoc->lspc.pluinfop[i].subbasin == nIndex)
				pDoc->lspc.nlandimp++;
		}
	}
	else // fill the blanks with data
	{
		InitGrid();

		if(pDoc->lspc.pluinfop == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i=0; i<pDoc->lspc.nlandp*pDoc->lspc.nsws; i++)
		{
			m_Grid.InsertRow(_T(""), -1);
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0,pDoc->lspc.pluinfop[i].subbasin);
			m_Grid.SetItemInt(nRow,  1,pDoc->lspc.pluinfop[i].luid);
			m_Grid.SetItemText(nRow, 2,pDoc->lspc.pluinfop[i].luname);
			m_Grid.SetItemInt(nRow,  3,pDoc->lspc.pluinfop[i].piid);
			m_Grid.SetItemFloat(nRow,4,pDoc->lspc.pluinfop[i].area);
			m_Grid.SetItemFloat(nRow,5,pDoc->lspc.pluinfop[i].slsur);
			m_Grid.SetItemFloat(nRow,6,pDoc->lspc.pluinfop[i].lsur);
		}

		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
