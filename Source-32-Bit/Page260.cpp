// Page260.cpp: implementation of the CPage260 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page260.h"
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

CPage260::CPage260()
{

}

CPage260::CPage260(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage260::~CPage260()
{

}

void CPage260::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(15);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] = {"defid","dwqid","deluid","sqo","potfw","potfs","potfc","acqop","sqolim","wsqop","soqc","ioqc","aoqc","addc","awdc"}; // column name
	
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
}

BOOL CPage260::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;
	
	int nNum = pDoc->lspc.ngroup*pDoc->lspc.nQuals*pDoc->lspc.nlandp;
	
	if(bSaveAndValidate)
	{
		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 260: Row number should be equal to ngroup*nQuals*nlandp.");
			Log("Card 260: Row number should be equal to ngroup*nQuals*nlandp.");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.pQualInput != NULL)
			delete [](pDoc->lspc.pQualInput);

		if (nNum > 0)
			pDoc->lspc.pQualInput = new QUAL_INPUT[nNum];

		for(int i = 0; i < nNum; ++i)
		{
			pDoc->lspc.pQualInput[i].gid    = m_Grid.GetItemInt(i+1,  0);
			pDoc->lspc.pQualInput[i].qualid = m_Grid.GetItemInt(i+1,  1);
			pDoc->lspc.pQualInput[i].lid    = m_Grid.GetItemInt(i+1,  2);
			pDoc->lspc.pQualInput[i].sqo    = m_Grid.GetItemFloat(i+1,3);
			pDoc->lspc.pQualInput[i].potfw  = m_Grid.GetItemFloat(i+1,4);
			pDoc->lspc.pQualInput[i].potfs  = m_Grid.GetItemFloat(i+1,5);
			pDoc->lspc.pQualInput[i].potfc  = m_Grid.GetItemFloat(i+1,6);
			pDoc->lspc.pQualInput[i].acqop  = m_Grid.GetItemFloat(i+1,7);
			pDoc->lspc.pQualInput[i].sqolim = m_Grid.GetItemFloat(i+1,8);
			pDoc->lspc.pQualInput[i].wsqop  = m_Grid.GetItemFloat(i+1,9);
			pDoc->lspc.pQualInput[i].soqc   = m_Grid.GetItemFloat(i+1,10);
			pDoc->lspc.pQualInput[i].ioqc   = m_Grid.GetItemFloat(i+1,11);
			pDoc->lspc.pQualInput[i].aoqc   = m_Grid.GetItemFloat(i+1,12);
			pDoc->lspc.pQualInput[i].addc   = m_Grid.GetItemFloat(i+1,13);
			pDoc->lspc.pQualInput[i].awdc   = m_Grid.GetItemFloat(i+1,14);
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.pQualInput == NULL)
			return true;
		
		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0, pDoc->lspc.pQualInput[i].gid);
			m_Grid.SetItemInt(nRow,  1, pDoc->lspc.pQualInput[i].qualid);
			m_Grid.SetItemInt(nRow,  2, pDoc->lspc.pQualInput[i].lid);
			m_Grid.SetItemFloat(nRow,3, pDoc->lspc.pQualInput[i].sqo);
			m_Grid.SetItemFloat(nRow,4, pDoc->lspc.pQualInput[i].potfw);
			m_Grid.SetItemFloat(nRow,5, pDoc->lspc.pQualInput[i].potfs);
			m_Grid.SetItemFloat(nRow,6, pDoc->lspc.pQualInput[i].potfc);
			m_Grid.SetItemFloat(nRow,7, pDoc->lspc.pQualInput[i].acqop);
			m_Grid.SetItemFloat(nRow,8, pDoc->lspc.pQualInput[i].sqolim);
			m_Grid.SetItemFloat(nRow,9, pDoc->lspc.pQualInput[i].wsqop);
			m_Grid.SetItemFloat(nRow,10, pDoc->lspc.pQualInput[i].soqc);
			m_Grid.SetItemFloat(nRow,11, pDoc->lspc.pQualInput[i].ioqc);
			m_Grid.SetItemFloat(nRow,12,pDoc->lspc.pQualInput[i].aoqc);
			m_Grid.SetItemFloat(nRow,13,pDoc->lspc.pQualInput[i].addc);
			m_Grid.SetItemFloat(nRow,14,pDoc->lspc.pQualInput[i].awdc);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
