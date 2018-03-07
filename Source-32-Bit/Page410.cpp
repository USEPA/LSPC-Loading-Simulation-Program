// Page410.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page410.h"
#include "MainFrm.h"
#include "LSPCView.h"
#include "LSPCDoc.h"
#include "LSPCMODEL.h"
#include "DlgCrossSection.h"
#include <direct.h>
#include "DirDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPage410::CPage410()
{

}

CPage410::CPage410(UINT nIDCap) : CPageAdd(nIDCap)
{

}

CPage410::~CPage410()
{

}

void CPage410::InitGrid()
{
	ShowEditButton(true);
	ShowExportButton(true);
	ShowInfoText(TRUE);

	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(14);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString str[] = {"rchid","rgid","lkfg","idepth","length","depth","width","slope","manning","r1","r2","w1","crrat","ks"}; 
	UINT format[] = {DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT,DT_RIGHT}; 

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
}

BOOL CPage410::UpdateData( BOOL bSaveAndValidate)
{
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
		if(nRow != pDoc->lspc.nch)
		{
			AfxMessageBox("Card 410:Number of rows should be equal to nch!");
			Log("Card 410:Number of rows should be equal to nch!");
			return false;
		}

		int *nGroupArray = NULL;
		int *nGroupId = NULL;
		ReachInfo *pReachInfo = NULL;

		if (pDoc->lspc.nch > 0 && pDoc->lspc.nrgroup > 0)
		{
			nGroupArray = new int[pDoc->lspc.nch];
			nGroupId = new int[pDoc->lspc.nrgroup];
			pReachInfo = new ReachInfo[pDoc->lspc.nch];
		}

		for(int i=0; i<pDoc->lspc.nch; i++)
		{
			pReachInfo[i].rchid    = m_Grid.GetItemInt(i+1,  0);
			pReachInfo[i].gid      = m_Grid.GetItemInt(i+1,  1);
			pReachInfo[i].lkfg     = m_Grid.GetItemInt(i+1,  2);
			pReachInfo[i].idepth   = m_Grid.GetItemFloat(i+1,3);
			pReachInfo[i].length   = m_Grid.GetItemFloat(i+1,4);
			pReachInfo[i].depth    = m_Grid.GetItemFloat(i+1,5);
			pReachInfo[i].width    = m_Grid.GetItemFloat(i+1,6);
			pReachInfo[i].slope    = m_Grid.GetItemFloat(i+1,7);
			pReachInfo[i].mann     = m_Grid.GetItemFloat(i+1,8);
			pReachInfo[i].r1       = m_Grid.GetItemFloat(i+1,9);
			pReachInfo[i].r2       = m_Grid.GetItemFloat(i+1,10);
			pReachInfo[i].w1       = m_Grid.GetItemFloat(i+1,11);
			pReachInfo[i].crrat    = m_Grid.GetItemFloat(i+1,12);
			pReachInfo[i].ks       = m_Grid.GetItemFloat(i+1,13);

			if(pReachInfo[i].slope <1.0e-4 && pReachInfo[i].lkfg == 0)
				pReachInfo[i].slope = 1.0e-4;
			if(pReachInfo[i].crrat < 1)
				pReachInfo[i].crrat = 1;

			// check for no routing
			if (pReachInfo[i].length <= 0 || pReachInfo[i].width <= 0 || pReachInfo[i].depth <= 0)
				pReachInfo[i].routefg = 0;

			//populate array of land group id
			nGroupArray[i] = pReachInfo[i].gid;
		}

		if(pDoc->lspc.pReachInfo != NULL)
		{
			for(int i=0; i<pDoc->lspc.nch; i++)
				pReachInfo[i].poly = pDoc->lspc.pReachInfo[i].poly;

			delete []pDoc->lspc.pReachInfo;
		}

		pDoc->lspc.pReachInfo = pReachInfo;
		
		//sort the array
		qsort (nGroupArray, pDoc->lspc.nch, sizeof(int), compare);

		int gid = -1;
		CList<int,int> nGroupList;

		for (i=0; i<pDoc->lspc.nch; i++)
		{
			if (nGroupArray[i] != gid)
			{
				gid = nGroupArray[i];
				nGroupList.AddTail(gid);
			}
		}

		if (pDoc->lspc.nrgroup != nGroupList.GetCount())
			AfxMessageBox("Number of Reach Groups in Card 40 should be equal to the number of Reach Groups in card 410");

		int nIndex = 0;
		POSITION pos = nGroupList.GetHeadPosition();
		while (pos)
			nGroupId[nIndex++] = nGroupList.GetNext(pos);

		for (i=0; i<pDoc->lspc.nch; i++)
		{
			for (int j=0; j<pDoc->lspc.nrgroup; j++)
			{
				if (pDoc->lspc.pReachInfo[i].gid == nGroupId[j])
				{
					pDoc->lspc.pReachInfo[i].rgid = j+1;
					break;
				}
			}
		}

		// Remove all of the elements in the list.
		nGroupList.RemoveAll();

		if (nGroupArray != NULL)		delete []nGroupArray;
		if (nGroupId != NULL)			delete []nGroupId;
	}
	else
	{
		InitGrid();

		if(pDoc->lspc.pReachInfo == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		for(int i=0; i<pDoc->lspc.nch; i++)
		{
			m_Grid.InsertRow(_T(""), -1);	
			int nRow = m_Grid.GetRowCount() - 1;
			m_Grid.SetRowHeight(nRow, size.cy);
			m_Grid.SetItemInt(nRow,  0,pDoc->lspc.pReachInfo[i].rchid);
			m_Grid.SetItemInt(nRow,  1,pDoc->lspc.pReachInfo[i].gid);
			m_Grid.SetItemInt(nRow,  2,pDoc->lspc.pReachInfo[i].lkfg);
			m_Grid.SetItemFloat(nRow,3,pDoc->lspc.pReachInfo[i].idepth);
			m_Grid.SetItemFloat(nRow,4,pDoc->lspc.pReachInfo[i].length);
			m_Grid.SetItemFloat(nRow,5,pDoc->lspc.pReachInfo[i].depth);
			m_Grid.SetItemFloat(nRow,6,pDoc->lspc.pReachInfo[i].width);
			m_Grid.SetItemFloat(nRow,7,pDoc->lspc.pReachInfo[i].slope);
			m_Grid.SetItemFloat(nRow,8,pDoc->lspc.pReachInfo[i].mann);
			m_Grid.SetItemFloat(nRow,9,pDoc->lspc.pReachInfo[i].r1);
			m_Grid.SetItemFloat(nRow,10,pDoc->lspc.pReachInfo[i].r2);
			m_Grid.SetItemFloat(nRow,11,pDoc->lspc.pReachInfo[i].w1);
			m_Grid.SetItemFloat(nRow,12,pDoc->lspc.pReachInfo[i].crrat);
			m_Grid.SetItemFloat(nRow,13,pDoc->lspc.pReachInfo[i].ks);

			if (pDoc->lspc.pReachInfo[i].poly.bCustomized)
				for (int j=0; j<14; j++)
					m_Grid.SetItemBkColour(nRow, j, RGB(215, 215, 215));
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}

void CPage410::EditRow()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;

	if(pDoc->lspc.nch == 0 || pDoc->lspc.pReachInfo == NULL)
		return;

	int nRow = m_Grid.GetFocusCell().row;
    if (nRow >= 0 && nRow <= pDoc->lspc.nch)
    {
		if(pDoc->lspc.pReachInfo[nRow-1].lkfg != 0)
		{
			AfxMessageBox("Cross-section design is just for rivers, not for lakes.");
			return;
		}

		UpdateData(true);

		CDlgCrossSection dlg;
		dlg.poly = &(pDoc->lspc.pReachInfo[nRow-1].poly);
		if(dlg.poly->pointList.GetCount() < 3)
		{
			dlg.poly->CPoly_Free();
			dlg.poly->bCustomized = false;
		}

		dlg.nIndex = nRow-1;
		dlg.DoModal();

		for (int i=0; i<14; i++)
			m_Grid.SetItemBkColour(nRow, i, dlg.poly->bCustomized?RGB(215, 215, 215):RGB(255, 255, 255));
		m_Grid.Invalidate();
	}
	else
	{
		AfxMessageBox("Select a reach by selectig any cell in the row",MB_ICONINFORMATION);
	}
}

void CPage410::ExportFtable()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;

	if(pDoc->lspc.nch == 0 || pDoc->lspc.pReachInfo == NULL)
		return;

	// CREATE the output directory if it is necessary
	if (!FolderExists(pDoc->lspc.m_strModelResultLocation))
	{
		TRY
		{
			_mkdir(LPCSTR(pDoc->lspc.m_strModelResultLocation));
		}
		CATCH(CException, e)
		{
			//open dialog box to select the folder
			CDirDialog dlg;
			dlg.m_strSelDir = pDoc->lspc.m_strModelResultLocation;
			dlg.m_strTitle = "Model Results Directory:";
			if(dlg.DoBrowse() == 1)
				pDoc->lspc.m_strModelResultLocation = dlg.m_strPath;
		}
		END_CATCH
	}

	double *fh = new double[60];  
	double *fv = new double[60]; 
	double *fq = new double[60];
	double *qk = new double[60];
	double *fa = new double[60];
	double *fp = new double[60];
	double *fr = new double[60];				
	double *fsa = new double[60];				
	CString strLine, strTemp;
	CString strFileName = pDoc->lspc.m_strModelResultLocation + "ftable.csv";
	FILE *fp1 = NULL;
	fp1 = fopen(LPCSTR(strFileName), "w+t");
	if(fp1 == NULL)
	{
		CString strMsg;
		strMsg = "Cannot open file "+strFileName+" for writing";
		AfxMessageBox(strMsg);
		Log(strMsg);
		return;
	}
	
	if (pDoc->lspc.FtableRecord > 0)
	{
		int maxDSchan = 0;
		int nNum = pDoc->lspc.nch;

		for(int i=0; i<nNum; i++)
		{
			int count = pDoc->lspc.pChannelInfo[i].nDSchan;
			if (count>maxDSchan)
				maxDSchan = count;
		}
		strLine.Format("ReachID,Depth (ft),Surface Area (acre),Volume (acre-ft)");
		for(i=0; i<maxDSchan; i++)
		{
			strTemp.Format(",Outflow_%d (cfs)",i+1);
			strLine += strTemp;
		}
		strLine += "\n";
		fputs(strLine, fp1);
	}
	else
	{
		fputs("ReachID,Depth (ft),Surface Area (acre),Volume (acre-ft),Outflow (cfs)\n", fp1);
	}

	UpdateData(true);
	
	for (int i=0; i<pDoc->lspc.nch; i++)
    {
		if(pDoc->lspc.pReachInfo[i].lkfg != 0)
		{
			//AfxMessageBox("Cross-section design is just for rivers, not for lakes.");
			continue;
		}

		if(pDoc->lspc.pReachInfo[i].poly.pointList.GetCount() > 3)
		{
			pDoc->lspc.pReachInfo[i].ftable(fh,fv,fq,qk,fa,fp,fr,fsa);			
		}
		else
		{
			//add flood plane to this version
			//create 4 points and add them to the poly 
			Point *pt = new Point[4];
			pt[0].x = 0;
			pt[0].y = 0;
			pt[1].x = (1-pDoc->lspc.pReachInfo[i].r1)*pDoc->lspc.pReachInfo[i].width/2;
			pt[1].y = pDoc->lspc.pReachInfo[i].depth;
			pt[2].x = pDoc->lspc.pReachInfo[i].width*pDoc->lspc.pReachInfo[i].r1+pt[1].x;
			pt[2].y = pDoc->lspc.pReachInfo[i].depth;
			pt[3].x = pDoc->lspc.pReachInfo[i].width;
			pt[3].y = 0;
			pDoc->lspc.pReachInfo[i].poly.pointList.AddTail((CObject*)&pt[0]);
			pDoc->lspc.pReachInfo[i].poly.pointList.AddTail((CObject*)&pt[1]);
			pDoc->lspc.pReachInfo[i].poly.pointList.AddTail((CObject*)&pt[2]);
			pDoc->lspc.pReachInfo[i].poly.pointList.AddTail((CObject*)&pt[3]);
			pDoc->lspc.pReachInfo[i].ftable(fh,fv,fq,qk,fa,fp,fr,fsa);			
			pDoc->lspc.pReachInfo[i].poly.pointList.RemoveAll();

			if (pt != NULL) delete []pt;          
		}
		
		if (pDoc->lspc.pReachFtable[i].nFTrecord > 0)	// user defined FTABLE
		{
			int nStart = pDoc->lspc.pReachFtable[i].nFTstart;
			int nRecord = pDoc->lspc.pReachFtable[i].nFTrecord;
			for (int j=nStart; j<nStart+nRecord; j++)
			{
				if (pDoc->lspc.pReachInfo[i].rchid == pDoc->lspc.pFtable[j].rchid)
				{
					double depth = pDoc->lspc.pFtable[j].depth;				// ft
					double sarea = pDoc->lspc.pFtable[j].area / 43559.66;	// ft2 to acre
					double volume = pDoc->lspc.pFtable[j].vol / 43559.66;	// ft3 to acre-ft
					strLine.Format("%d,%e,%e,%e",pDoc->lspc.pReachInfo[i].rchid,depth,sarea,volume);
									
					if (pDoc->lspc.pFtable[j].disch != NULL)
					{
						for (int k=0; k<pDoc->lspc.pChannelInfo[i].nDSchan; k++)
						{
							strTemp.Format(",%e",pDoc->lspc.pFtable[j].disch[k]);	//cfs
							strLine += strTemp;
						}
					}
					strLine += "\n";
					fputs(LPCSTR(strLine),fp1);
				}
			}
		}
		else
		{
			for (int j=0; j<60; j++)
			{
				fsa[j] /= 43559.66;	// ft2 to acre
				fv[j]  /= 43559.66;	// ft3 to acre-ft
				fprintf(fp1, "%d,%e,%e,%e,%e\n",pDoc->lspc.pReachInfo[i].rchid,fh[j],fsa[j],fv[j],fq[j]);
			}
		}
	}
		
	if (fh != NULL) delete []fh;      
	if (fv != NULL) delete []fv;     
	if (fq != NULL) delete []fq;      
	if (qk != NULL) delete []qk;      
	if (fa != NULL) delete []fa;     
	if (fp != NULL) delete []fp;      
	if (fr != NULL) delete []fr;			    
	if (fsa != NULL) delete []fsa;			    

	fclose(fp1);

	AfxMessageBox("The Ftables are saved in " + strFileName + " file",MB_ICONINFORMATION);
}
