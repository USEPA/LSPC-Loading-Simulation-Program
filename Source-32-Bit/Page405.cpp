// Page405.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page405.h"
#include "MainFrm.h"
#include "LSPCView.h"
#include "LSPCDoc.h"
#include "LSPCMODEL.h"
//#include "RecordsetEx.h"	 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPage405::CPage405()
{

}

CPage405::CPage405(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage405::~CPage405()
{

}

void CPage405::InitGrid()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);

	CLSPCDoc *pDoc = pView->GetDocument();

	int maxDSchan = 0;
	int nNum = pDoc->lspc.nch;

	for(int i = 0; i < nNum; i++)
	{
		int count = pDoc->lspc.pChannelInfo[i].nDSchan;
		if (count>maxDSchan)
			maxDSchan = count;
	}
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(3+maxDSchan);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString *str = NULL;
	str = new CString[3+maxDSchan];
	str[0]= "rchid";
	str[1]= "output control";
	str[2]= "number of outlets";
	for (int j=0; j<maxDSchan; j++)
		str[j+3].Format("ds_%d", j+1);

	UINT *format = NULL;
	format = new UINT[3+maxDSchan];
	format[0]= DT_RIGHT;
	format[1]= DT_RIGHT;
	format[2]= DT_RIGHT;
	for (j=3; j<3+maxDSchan; j++)
	format[j]= DT_RIGHT;

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

BOOL CPage405::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;

	int nNum = pDoc->lspc.nch;

	if(bSaveAndValidate)
	{
		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 405: Number of rows should be equal to nch!");
			Log("Card 405: Number of rows should be equal to nch!");
			return false;
		}

		// delete the old one
		if(pDoc->lspc.pChannelInfo != NULL)
		{
			delete [](pDoc->lspc.pChannelInfo);
			pDoc->lspc.pChannelInfo = NULL;
		}

		if(nNum > 0)
			pDoc->lspc.pChannelInfo = new ChannelInfo[nNum];
		
		for(int i=0; i<nNum; i++)
		{
			pDoc->lspc.pChannelInfo[i].rchid   = m_Grid.GetItemInt(i+1,0);
			pDoc->lspc.pChannelInfo[i].comp    = (m_Grid.GetItemInt(i+1,1) == 0)?0:1;
			pDoc->lspc.pChannelInfo[i].nDSchan = m_Grid.GetItemInt(i+1,2);

			//allocate memory here   
			if (pDoc->lspc.pChannelInfo[i].nDSchan > 0)
				pDoc->lspc.pChannelInfo[i].down_ch = new int[pDoc->lspc.pChannelInfo[i].nDSchan];

			// initialize
			for(int j=0; j<pDoc->lspc.pChannelInfo[i].nDSchan; j++)
				pDoc->lspc.pChannelInfo[i].down_ch[j] = m_Grid.GetItemInt(i+1,j+3);

			pDoc->lspc.pChannelInfo[i].nSelfOrder = -1;
//			pDoc->lspc.pChannelInfo[i].nIndexSWS  = -1;
//			pDoc->lspc.pChannelInfo[i].nIndexUL   = -1;
//			pDoc->lspc.pChannelInfo[i].nIndexUR   = -1;
//			pDoc->lspc.pChannelInfo[i].nIndexD    = -1;

			// delete the old one (c435)
			pDoc->lspc.pChannelInfo[i].cListUpStream.RemoveAll();
			pDoc->lspc.pChannelInfo[i].cListDownStream.RemoveAll();
		}

		// save back to database
/*		CLSPCApp *pApp = (CLSPCApp *) AfxGetApp();
		if(pApp == NULL)
			return false;

		if (pApp->strDBFileName.Right(3) == "mdb")
		{
			CString strSQL;
			CDatabase db;
			BOOL bCanOpen = FALSE;
			try
			{
				strSQL = CLSPCApp::GetMDBDriver();
				if(strSQL.IsEmpty())
				{
					AfxMessageBox("Cannot find the Microsoft Access ODBC driver.");
					return false;
				}
				bCanOpen = db.OpenEx(strSQL,CDatabase::noOdbcDialog);
			}
			catch(CException e)
			{
				AfxMessageBox("ODBC Error ---Cannot open database for reading.");
				return false;
			}
			if(!bCanOpen)
			{
				AfxMessageBox("ODBC Error ---Cannot open database for reading.");
				return false;
			}
			CRecordsetEx rs(&db);
			
			//card 405		Table "reachmodel"
			for(i=0; i<nNum; i++)
			{
				strSQL.Format("UPDATE reachmodel set CONTROL=%d WHERE SUBBASIN=%d",
					pDoc->lspc.pChannelInfo[i].comp, pDoc->lspc.pChannelInfo[i].rchid);
				db.ExecuteSQL(LPCSTR(strSQL));
			}

			rs.ExecDirect(strSQL);
			rs.Close();
			db.Close();
		}
*/
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.pChannelInfo == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i =0; i < nNum; ++i)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0,pDoc->lspc.pChannelInfo[i].rchid);
			m_Grid.SetItemInt(nRow,	 1,pDoc->lspc.pChannelInfo[i].comp);
			m_Grid.SetItemInt(nRow,	 2,pDoc->lspc.pChannelInfo[i].nDSchan);
		
			//   (01-2005)
			for(int j = 0; j< pDoc->lspc.pChannelInfo[i].nDSchan; j++)
				m_Grid.SetItemInt(nRow,	 3+j,pDoc->lspc.pChannelInfo[i].down_ch[j]);
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}

