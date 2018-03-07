// Page685.cpp: implementation of the CPage685 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page685.h"
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

CPage685::CPage685()
{

}

CPage685::CPage685(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage685::~CPage685()
{

}

void CPage685::InitGrid()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;

	int j;
	int nNum = pDoc->lspc.nQuals;
	int Num = nNum;

	if (pDoc->lspc.sedfg == 1 || pDoc->lspc.oxfg == 1)
		Num += 12;

	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(3+Num);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString *strQual = NULL;
	if (Num > 0)
		strQual = new CString[Num];

	for (j=0; j<nNum; j++)
		strQual[j] = pDoc->lspc.pQual[j].qname;

	if (pDoc->lspc.sedfg == 1 || pDoc->lspc.oxfg == 1)
	{
		strQual[nNum+0]  = "sand";
		strQual[nNum+1]  = "silt";
		strQual[nNum+2]  = "clay";
		strQual[nNum+3]  = "temp";
		strQual[nNum+4]  = "do";
		strQual[nNum+5]  = "bod";
		strQual[nNum+6]  = "nox";
		strQual[nNum+7]  = "tam";
		strQual[nNum+8]  = "po4";
		strQual[nNum+9]  = "orn";
		strQual[nNum+10] = "orp";
		strQual[nNum+11] = "orc";
	}

	CString *str = NULL;
	str = new CString[3+Num];
	str[0]= "rchid";
	str[1]= "outlet";
	str[2]= "limit_flow";
	for (j=0; j<Num; j++)
		str[j+3].Format("limit_%s", strQual[j]); 

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;

	for (int col = 0; col < m_Grid.GetColumnCount(); col++)
	{
		Item.col = col;
		Item.szText = str[col];
		m_Grid.SetItem(&Item);
	}
	m_Grid.AutoSize();
	if (strQual != NULL)
		delete []strQual;
	if (str != NULL)
		delete []str;
}

BOOL CPage685::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;
	
	int nNum = pDoc->lspc.ncrchOutlets;
	int Num = pDoc->lspc.nQuals;
	if (pDoc->lspc.sedfg == 1 || pDoc->lspc.oxfg == 1)
		Num += 12;
	
	if(bSaveAndValidate)
	{
		if(pDoc->lspc.ncrchOutlets <= 0)
			return true;

		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 685: Row number should be equal to number of controlled outlets");
			Log("Card 685: Row number should be equal to number of controlled outlets");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.pBMPLimit != NULL)
			delete [](pDoc->lspc.pBMPLimit);

		if (nRow > 0)
			pDoc->lspc.pBMPLimit = new BMPLimit[nRow];

		for(int i=0; i<nRow; ++i)
		{
			pDoc->lspc.pBMPLimit[i].rchid       = m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.pBMPLimit[i].nOutletId   = m_Grid.GetItemInt(i+1,  1);
			pDoc->lspc.pBMPLimit[i].fLimit_flow = m_Grid.GetItemFloat(i+1,2);
			if (Num > 0)
			{
				pDoc->lspc.pBMPLimit[i].fLimit_qual = new double[Num];
				for(int j=0; j<Num; j++)
					pDoc->lspc.pBMPLimit[i].fLimit_qual[j] = m_Grid.GetItemFloat(i+1,j+3);
			}
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.pBMPLimit == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i=0; i<nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0,pDoc->lspc.pBMPLimit[i].rchid);
			m_Grid.SetItemInt(nRow,  1,pDoc->lspc.pBMPLimit[i].nOutletId);
			m_Grid.SetItemFloat(nRow,2,pDoc->lspc.pBMPLimit[i].fLimit_flow);
			for(int j=0; j<Num; j++)
				m_Grid.SetItemFloat(nRow,3+j,pDoc->lspc.pBMPLimit[i].fLimit_qual[j]);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
