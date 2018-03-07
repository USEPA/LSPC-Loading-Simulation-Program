// Page60.cpp: implementation of the CPage60 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lspc.h"
#include "Page60.h"

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

CPage60::CPage60()
{

}

CPage60::CPage60(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage60::~CPage60()
{

}

void CPage60::InitGrid()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	CLSPCDoc *pDoc = pView->GetDocument();
	
	int maxWStations = 0;
	int nNum = pDoc->lspc.nsws;

	for(int i=0; i<nNum; i++)
	{
		int count = pDoc->lspc.pswsinfo[i].nwst;
		if (count>maxWStations)
			maxWStations = count;
	}
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(3+maxWStations*2);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString *str = NULL;
	str = new CString[3+maxWStations*2];
	str[0]= "subbasin";
	str[1]= "defid";
	str[2]= "nwst";
	for (int j=0; j<maxWStations; j++)
	{
		str[j*2+3].Format("wst%d", j+1);
		str[j*2+4].Format("wt%d", j+1);
	}

	UINT *format = NULL;
	format = new UINT[3+maxWStations*2];
	format[0]= DT_RIGHT;
	format[1]= DT_RIGHT;
	format[2]= DT_RIGHT;
	for (j=3; j<3+maxWStations*2; j++)
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

BOOL CPage60::UpdateData( BOOL bSaveAndValidate)
{
	// set the active page
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
		if(nRow != pDoc->lspc.nsws)
		{
			AfxMessageBox("Card 60: Number of rows should be equal to nsws in Card 40!");
			Log("Card 60: Number of rows should be equal to nsws in Card 40!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.pswsinfo != NULL)
			delete [](pDoc->lspc.pswsinfo);
		
		int *nGroupArray = NULL;
		int *nGroupId = NULL;

		if(pDoc->lspc.nsws > 0 && pDoc->lspc.ngroup > 0)
		{
			nGroupArray = new int[pDoc->lspc.nsws];
			nGroupId = new int[pDoc->lspc.ngroup];
			pDoc->lspc.pswsinfo = new swsinfo[pDoc->lspc.nsws];
		}
		
		for(int i=0; i<pDoc->lspc.nsws; i++)
		{
			pDoc->lspc.pswsinfo[i].subbasin = m_Grid.GetItemInt(i+1,0);
			pDoc->lspc.pswsinfo[i].gid		= m_Grid.GetItemInt(i+1,1);
			pDoc->lspc.pswsinfo[i].nwst     = m_Grid.GetItemInt(i+1,2);
			if (pDoc->lspc.pswsinfo[i].nwst > 0)
			{
				if (pDoc->lspc.pswsinfo[i].wstindex != NULL)
					delete[]pDoc->lspc.pswsinfo[i].wstindex;
				pDoc->lspc.pswsinfo[i].wstindex = new int[pDoc->lspc.pswsinfo[i].nwst];
				if (pDoc->lspc.pswsinfo[i].wti != NULL)
					delete[]pDoc->lspc.pswsinfo[i].wti;
				pDoc->lspc.pswsinfo[i].wti = new double[pDoc->lspc.pswsinfo[i].nwst];
			}
			for (int j=0; j<pDoc->lspc.pswsinfo[i].nwst; j++)
			{
				int nID = m_Grid.GetItemInt(i+1,3+j*2);
				pDoc->lspc.pswsinfo[i].wstindex[j] = pDoc->lspc.cWD.FindStationIndex(nID);
				pDoc->lspc.pswsinfo[i].wti[j] = m_Grid.GetItemFloat(i+1,4+j*2);
			}
			//array of land group id
			nGroupArray[i] = pDoc->lspc.pswsinfo[i].gid;
		}

		//sort the array
		qsort (nGroupArray, pDoc->lspc.nsws, sizeof(int), compare);

		int gid = -1;
		CList<int,int> nGroupList;
		for (i=0; i<pDoc->lspc.nsws; i++)
		{
			if (nGroupArray[i] != gid)
			{
				gid = nGroupArray[i];
				nGroupList.AddTail(nGroupArray[i]);
			}

		}

		if (pDoc->lspc.ngroup != nGroupList.GetCount())
			AfxMessageBox("Number of Land Groups in Card 40 should be equal to the number of Land Groups in card 60");

		int nIndex = 0;
		POSITION pos = nGroupList.GetHeadPosition();
		while (pos)
			nGroupId[nIndex++] = nGroupList.GetNext(pos);

		for (i=0; i<pDoc->lspc.nsws; i++)
		{
			for (int j=0; j<pDoc->lspc.ngroup; j++)
			{
				if (pDoc->lspc.pswsinfo[i].gid == nGroupId[j])
				{
					pDoc->lspc.pswsinfo[i].gindex = j;
					break;
				}
			}
		}

		// Remove all of the elements in the list.
		nGroupList.RemoveAll();

		if (nGroupArray != NULL)	delete []nGroupArray;
		if (nGroupId != NULL)		delete []nGroupId;
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.pswsinfo == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i=0; i<pDoc->lspc.nsws; i++)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0, pDoc->lspc.pswsinfo[i].subbasin);
			m_Grid.SetItemInt(nRow,  1, pDoc->lspc.pswsinfo[i].gid);
			m_Grid.SetItemInt(nRow,  2, pDoc->lspc.pswsinfo[i].nwst);
			for (int j=0; j<pDoc->lspc.pswsinfo[i].nwst; j++)
			{
				int nIndex = pDoc->lspc.pswsinfo[i].wstindex[j];
				m_Grid.SetItemInt(nRow,  3+j*2, pDoc->lspc.cWD.pWSTATIONINFO[nIndex].nWStationID);
				m_Grid.SetItemFloat(nRow,4+j*2, pDoc->lspc.pswsinfo[i].wti[j]);
			}
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}
