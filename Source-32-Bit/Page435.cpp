// Page435.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page435.h"
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

CPage435::CPage435()
{

}

CPage435::CPage435(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage435::~CPage435()
{

}

void CPage435::InitGrid()
{
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(3);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] = {"rchid","outlet","file path"}; // column name
    UINT format[] = {DT_RIGHT,DT_RIGHT,DT_LEFT}; // column format

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

BOOL CPage435::UpdateData( BOOL bSaveAndValidate)
{
	POSITION pos;
	CString *strArray = NULL;
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;

	int nNum = pDoc->lspc.nLinkFiles;

	if(bSaveAndValidate)
	{
		int nRow = m_Grid.GetRowCount() - 1;
		if(nRow != nNum)
		{
			AfxMessageBox("Card 435: Check number of records!");
			Log("Card 435: Check number of records!");
			return false;
		}

		if (nNum > 0)
		{
			if (strArray != NULL)	
				delete []strArray;
			strArray = new CString[nNum];
		}

		for(int i=0; i<nNum; i++)
		{
			int rchid = m_Grid.GetItemInt(i+1,0);
			int nOutlet = m_Grid.GetItemInt(i+1,1);
			CString strFilePath = m_Grid.GetItemText(i+1,2);
			strFilePath.TrimLeft();
			strFilePath.TrimRight();
			strArray[i] = strFilePath;

			if (nOutlet < 0)
			{
				CString strError;
				strError.Format("Outlet index:%d for reach:%d is not correct in card 435",nOutlet, rchid);
				AfxMessageBox(strError);
				TRACE(strError);
				Log(strError);
				if (strArray != NULL)	
					delete []strArray;
				return false;
			}
			// verify the input file path
			if (nOutlet == 0)
			{
				if (!FolderExists(strFilePath))
				{
					CString strError;
					strError.Format("Card 435: Invalid Linkage Input File Path: %s for Reach ID: %d",strFilePath,rchid);
					AfxMessageBox(strError);
					TRACE(strError);
					Log(strError);
					if (strArray != NULL)	
						delete []strArray;
					return false;
				}
			}
			else // verify the output folder path
			{
				//find the folder
				int nPathLength = strFilePath.GetLength();
				int nFileLength = nPathLength - strFilePath.ReverseFind('\\') - 1;
				CString strFolderPath = strFilePath.Left(nPathLength - nFileLength);

				if (!FolderExists(strFolderPath))
				{
					CString strError;
					strError.Format("Card 435: Invalid Linkage Output Folder Path: %s for Reach ID: %d",strFolderPath,rchid);
					AfxMessageBox(strError);
					TRACE(strError);
					Log(strError);
					if (strArray != NULL)	
						delete []strArray;
					return false;
				}
			}

			//find the reach index
			int nIndex = pDoc->lspc.FindIndexOrder(rchid);
			if (nIndex == -1)
			{
				CString strError;
				strError.Format("Reach ID: %d: in card 435 not found",rchid);
				AfxMessageBox(strError);
				TRACE(strError);
				Log(strError);
				if (strArray != NULL)	
					delete []strArray;
				return false;
			}

			Link435 plink435;
			plink435.nrchid = rchid;
			plink435.nOutlet = nOutlet;
			plink435.strFilePath = strFilePath;
			plink435.m_linkfile = NULL;
			plink435.pLinkData = NULL;

			if (nOutlet == 0)
				pDoc->lspc.pChannelInfo[nIndex].cListUpStream.AddTail(plink435);
			else
				pDoc->lspc.pChannelInfo[nIndex].cListDownStream.AddTail(plink435);
		}

		// check for duplicate files
		if (DuplicateExists(nNum,strArray))
		{
			CString strError;
			strError.Format("Duplicate File Paths found in card 435");
			AfxMessageBox(strError);
			TRACE(strError);
			Log(strError);
			if (strArray != NULL)	
				delete []strArray;
			return false;
		}
		
		//release the memory
		if (strArray != NULL)	
			delete []strArray;
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.nLinkFiles == 0)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");

		for(int i=0; i<pDoc->lspc.nch; ++i)
		{
			//write the input timeseries connections
			if(!pDoc->lspc.pChannelInfo[i].cListUpStream.IsEmpty())
			{
				pos = pDoc->lspc.pChannelInfo[i].cListUpStream.GetHeadPosition();
				while (pos != NULL)
				{
					Link435& plink435 = (Link435&) pDoc->lspc.pChannelInfo[i].cListUpStream.GetNext(pos);

					m_Grid.InsertRow(_T(""), -1);	
					int nRow = m_Grid.GetRowCount() - 1;
					m_Grid.SetRowHeight(nRow, size.cy);
					m_Grid.SetItemInt(nRow,  0,plink435.nrchid);
					m_Grid.SetItemInt(nRow,	 1,plink435.nOutlet);
					m_Grid.SetItemText(nRow, 2,plink435.strFilePath);
				}
			}

			//write the output timeseries connections
			if(!pDoc->lspc.pChannelInfo[i].cListDownStream.IsEmpty())
			{
				pos = pDoc->lspc.pChannelInfo[i].cListDownStream.GetHeadPosition();
				while (pos != NULL)
				{
					Link435& plink435 = (Link435&) pDoc->lspc.pChannelInfo[i].cListDownStream.GetNext(pos);

					m_Grid.InsertRow(_T(""), -1);	
					int nRow = m_Grid.GetRowCount() - 1;
					m_Grid.SetRowHeight(nRow, size.cy);
					m_Grid.SetItemInt(nRow,  0,plink435.nrchid);
					m_Grid.SetItemInt(nRow,	 1,plink435.nOutlet);
					m_Grid.SetItemText(nRow, 2,plink435.strFilePath);
				}
			}
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}

