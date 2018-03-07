// Page660.cpp: implementation of the CPage660 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page660.h"
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

CPage660::CPage660()
{

}

CPage660::CPage660(UINT nID) : CPageAdd(nID)
{

}

CPage660::~CPage660()
{

}

void CPage660::InitGrid()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;

	int Num = pDoc->lspc.nPtQuals;

	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(4+Num);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString *str = NULL;
	str = new CString[4+Num];
	str[0]= "rchid";
	str[1]= "permit";
	str[2]= "pipe";					
	str[3]= "reduction_flow";
	for (int j=0; j<Num; j++)
	{
		if (pDoc->lspc.pPtQual[j].qualid > 100)
			str[j+4].Format("reduction_%s", pDoc->lspc.pPtQual[j].qualname);
		else
			str[j+4].Format("reduction_%s_gqual", pDoc->lspc.pPtQual[j].qualname);
	}

	UINT *format = NULL;
	format = new UINT[4+Num];
	format[0]= DT_RIGHT;
	format[1]= DT_LEFT;
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

BOOL CPage660::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;
	
	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;

	int nNum = pDoc->lspc.nPtSource;
	int Num = pDoc->lspc.nPtQuals;

	if(bSaveAndValidate)
	{
		if(pDoc->lspc.nPtSource <= 0)
			return true;
		
		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 660: Number of rows should be equal to nPtSource in Card 660!");
			Log("Card 660: Number of rows should be equal to nPtSource in Card 660!");
			return false;
		}
		// delete the old one
		if(pDoc->lspc.pPSReduction != NULL)
		{
			delete []pDoc->lspc.pPSReduction;
			pDoc->lspc.pPSReduction = NULL;
		}

		if(nRow > 0)
		{
			pDoc->lspc.pPSReduction = new PSReduction[nRow];

			for(int i=0; i<nRow; ++i)
			{
				pDoc->lspc.pPSReduction[i].nPSindex = i+1;
				pDoc->lspc.pPSReduction[i].rchid = m_Grid.GetItemInt(i+1,  0);

				//added (2-11-2014)
				int nIndex = pDoc->lspc.FindIndexOrder(pDoc->lspc.pPSReduction[i].rchid);
				if (nIndex == -1)
				{
					CString strError;
					strError.Format("Reach ID: %d: in card 665 is not valid",pDoc->lspc.pPSReduction[i].rchid);
					AfxMessageBox(strError);
					TRACE(strError);
					Log(strError);
					return false;
				}
				
				pDoc->lspc.pPSReduction[i].strPermitName   = m_Grid.GetItemText(i+1, 1);
				pDoc->lspc.pPSReduction[i].strPipeName     = m_Grid.GetItemText(i+1, 2);
				pDoc->lspc.pPSReduction[i].fReduction_flow = m_Grid.GetItemFloat(i+1,3);
				if (Num > 0)
				{
					pDoc->lspc.pPSReduction[i].fReduction_qual = new double[Num];
					for(int j=0; j<Num; j++)
						pDoc->lspc.pPSReduction[i].fReduction_qual[j] = m_Grid.GetItemFloat(i+1,j+4);
				}

				//point source summary file
				if (pDoc->lspc.nPSsummary > 0)
				{
					PS_SUMMARY pPS_SUMMARY;
					pPS_SUMMARY.strPermit = pDoc->lspc.pPSReduction[i].strPermitName;
					pPS_SUMMARY.strPipe = pDoc->lspc.pPSReduction[i].strPipeName;
					pDoc->lspc.pChannelInfo[nIndex].cListPointSources.AddTail(pPS_SUMMARY);
				}
			}
		}
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.pPSReduction == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow, 0,pDoc->lspc.pPSReduction[i].rchid);
			m_Grid.SetItemText(nRow,1,pDoc->lspc.pPSReduction[i].strPermitName);
			m_Grid.SetItemText(nRow,2,pDoc->lspc.pPSReduction[i].strPipeName);
			m_Grid.SetItemFloat(nRow,3,pDoc->lspc.pPSReduction[i].fReduction_flow);
			for(int j=0; j<Num; j++)
				m_Grid.SetItemFloat(nRow,4+j,pDoc->lspc.pPSReduction[i].fReduction_qual[j]);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
