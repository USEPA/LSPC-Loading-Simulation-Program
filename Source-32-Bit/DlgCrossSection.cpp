// DlgCrossSection.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "DlgCrossSection.h"
#include "MainFrm.h"
#include "LSPCView.h"
#include "LSPCDoc.h"
#include "LSPCMODEL.h"
#include "PageNoAdd.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define THRESHOLD 5
/*******************************************
What's to be done next:
now that we need to add functions so that points
can be moved. to do this, we have a memeber which
is called nMode, we can either have editmode or addmode.
currently for the "clear" button, we can keep the current
logic but nmode should be set to addmode. For mouse event
handling, if it is addmode, at mouse_down, just add a new 
point to the end of the list. If it is editmode, for mouse_down,
select a point, and set its index to a memeber called nSel;
for mouse_up, compare the location with MD location, if it within
threshold, forget about it, otherwise, replace the old one with
the new location.
For display, always show the node as a cross.
********************************************/

/////////////////////////////////////////////////////////////////////////////
// CDlgCrossSection dialog

CDlgCrossSection::CDlgCrossSection(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCrossSection::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCrossSection)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	poly = NULL;
	nIndex = -1;
	nSel = -1;
	nMode = ADDMODE;
	bDrawBasic = true;
}


void CDlgCrossSection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCrossSection)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCrossSection, CDialog)
	//{{AFX_MSG_MAP(CDlgCrossSection)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_FT, OnFtable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCrossSection message handlers


void CDlgCrossSection::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;

	CPoint pt;
	int i;

	CRect rect;
	CWnd *pWnd = this->GetDlgItem(IDC_MAIN);
	ASSERT(pWnd != NULL);

	pWnd->GetWindowRect(&rect);
	//pWnd->ClientToScreen(&rect);

	CPen penBlack(PS_SOLID,1,RGB(0,0,0));
	CPen *pOld = dc.SelectObject(&penBlack);

	//draw the rectangle and fill it with white color
	this->ScreenToClient(&rect);
	//rect.InflateRect(0,-8, 0,-2);
	CBrush brushW(RGB(255,255,255));
	
	dc.FillRect(&rect,&brushW);
	dc.Rectangle(&rect);
	//draw the grids and the min and max on the x and y axles
	CPen penGray(PS_SOLID,1,RGB(200,200,200));
	dc.SelectObject(&penGray);
	int nLayers = 20;
	double lfLayerHeight = rect.Height()/(double)nLayers;
	for (i = 1; i < nLayers; ++i)
	{
		dc.MoveTo(rect.left,(int)(rect.top+i*lfLayerHeight));
		dc.LineTo(rect.right,(int)(rect.top+i*lfLayerHeight));
	}
	int nHGrids = 40;
	double lfHWidth = rect.Width()/(double)nHGrids;
	for (i = 1; i < nHGrids; ++i)
	{
		dc.MoveTo((int)(rect.left+i*lfHWidth),rect.top);
		dc.LineTo((int)(rect.left+i*lfHWidth),rect.bottom);
	}
	// now the labels

	//IDC_MAXX IDC_MAXY
	if(pDoc->lspc.pReachInfo == NULL)
		return;

	CString str;
	str.Format("%.3lf",pDoc->lspc.pReachInfo[nIndex].width);
	this->SetDlgItemText(IDC_MAXX,str);
	str.Format("%.3lf",pDoc->lspc.pReachInfo[nIndex].depth);
	this->SetDlgItemText(IDC_MAXY,str);
	
	pDoc->lspc.rec_width = rect.Width();
    pDoc->lspc.rec_height = rect.Height();

	if((poly == NULL || poly->pointList.GetCount() == 0) && bDrawBasic)
	{
		//draw the default shape
		if(pDoc->lspc.pReachInfo[nIndex].lkfg == 0)
		{
			CPen penDRed(PS_SOLID,1,RGB(150,0,0));
			dc.SelectObject(&penDRed);
			dc.MoveTo(rect.left,rect.top);
			dc.LineTo(rect.left+(int)((1-pDoc->lspc.pReachInfo[nIndex].r1)/2*rect.Width()),rect.bottom);
			dc.LineTo(rect.right-(int)((1-pDoc->lspc.pReachInfo[nIndex].r1)/2*rect.Width()),rect.bottom);
			dc.LineTo(rect.right,rect.top);
		}
		dc.SelectObject(pOld);
		return;
	}

	int nCount = poly->pointList.GetCount();

	CPen penRed(PS_SOLID,1,RGB(150,0,0));
	
	for (i = 0;i < nCount;i++) 
	{
		dc.SelectObject(&penBlack);
		POSITION pos = poly->pointList.FindIndex(i);
		Point *pti = (Point *)poly->pointList.GetAt(pos);
		pt.x = 0;
		pt.y = 0;
		if (pDoc->lspc.pReachInfo[nIndex].width > 0)
			pt.x = int(pti->x / pDoc->lspc.pReachInfo[nIndex].width * rect.Width());
		if (pDoc->lspc.pReachInfo[nIndex].depth > 0)
			pt.y = int(rect.Height() - pti->y / pDoc->lspc.pReachInfo[nIndex].depth * rect.Height());
		  
		//this->ScreenToClient(&pt);
		if(i == 0)
			dc.MoveTo(pt);
		else
			dc.LineTo(pt);  
		// draw a cross at the pt position
		dc.SelectObject(&penRed);
		dc.MoveTo(pt.x-2,pt.y);
		dc.LineTo(pt.x+2,pt.y);
		dc.MoveTo(pt.x,pt.y-2);
		dc.LineTo(pt.x,pt.y+2);
		dc.MoveTo(pt);
	}
	dc.SelectObject(pOld);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgCrossSection::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// if the point is inside the destined rectangle area, add it to
	// the polygon
	//convert the point to screen coordinates
	CPoint pt = point;

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;

	//this->ClientToScreen(&pt);
	CWnd *pWnd = this->GetDlgItem(IDC_MAIN);
	ASSERT(pWnd != NULL);

	CRect rect;
	pWnd->GetWindowRect(&rect);
	this->ScreenToClient(&rect);

	//enable the edit button if there are pts in the list
	if(poly != NULL && poly->pointList.GetCount() == 0)
	{
		//disable the edit button
		CWnd *pWnd = NULL;
		pWnd = GetDlgItem(IDCANCEL);
		if(pWnd != NULL)
			pWnd->EnableWindow(TRUE);
	}

	if(rect.PtInRect(pt))
	{
		if(nMode == ADDMODE)
		{
			if(poly == NULL)
				poly = new CPolygon();
			Point *pti = new Point();
			pti->x = (double)pt.x;	//screen coordinates
			pti->y = (double)pt.y;	//screen coordinates
			//convert to actual units
			int rec_width = rect.Width();
			if (rec_width > 0)
				pti->x = pti->x / rec_width * pDoc->lspc.pReachInfo[nIndex].width;
			int rec_height = rect.Height();
			if (rec_height > 0)
				pti->y = (rec_height - pti->y) / rec_height * pDoc->lspc.pReachInfo[nIndex].depth;
			//end
			poly->pointList.AddTail((CObject *)pti);
		}
		else if(nMode == EDITMODE)
		{
			nSel = -1;
			//find the selected point
			int nCount = poly->pointList.GetCount();
			for (int i = 0;i < nCount;i++) 
			{
			  POSITION pos = poly->pointList.FindIndex(i);
			  Point *pti = (Point *)poly->pointList.GetAt(pos);

				//convert to screen coordinates
				double x = pti->x;
				if (pDoc->lspc.pReachInfo[nIndex].width > 0)
					x = pti->x / pDoc->lspc.pReachInfo[nIndex].width * rect.Width();
				double y = pti->y;
				if (pDoc->lspc.pReachInfo[nIndex].depth > 0)
					y = rect.Height() - pti->y / pDoc->lspc.pReachInfo[nIndex].depth * rect.Height();
				//end

			  if(fabs(x - pt.x) < THRESHOLD 
				  /*&& fabs(pti->y - pt.y) < THRESHOLD */)
			  {
				  nSel = i;
				  break;
			  }
			}
		}
		this->Invalidate();
	}

	if (poly != NULL && poly->pointList.GetCount() > 2)
		poly->bCustomized = TRUE;

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgCrossSection::OnClear() 
{
  if(poly != NULL)
  {
	  poly->CPoly_Free();
	  poly->bCustomized = false;
  }
  nMode = ADDMODE;
  bDrawBasic = false;
  this->Invalidate();
}

void CDlgCrossSection::OnMouseMove(UINT nFlags, CPoint point) 
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;
	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;

	if(pDoc->lspc.pReachInfo == NULL)
		return;
	CPoint pt = point;

	//this->ClientToScreen(&pt);
	CWnd *pWnd = this->GetDlgItem(IDC_MAIN);
	ASSERT(pWnd != NULL);

	CRect rect;
	pWnd->GetWindowRect(&rect);
	this->ClientToScreen(&pt);

	if(rect.PtInRect(pt))
	{
		//do the conversion and update the lable with width and depth info
		//IDC_INFO
		double lfToLeft = (pt.x-rect.left)/(double)rect.Width()*pDoc->lspc.pReachInfo[nIndex].width;
		double lfToRight = pDoc->lspc.pReachInfo[nIndex].width - lfToLeft;
		double lfDepth = (rect.bottom-pt.y)/(double)rect.Height()*pDoc->lspc.pReachInfo[nIndex].depth;
		CString str;
		str.Format("Depth = %.3lf  Distance to left = %.3lf  Distance to right = %.3lf",
					lfDepth, lfToLeft, lfToRight);
		this->SetDlgItemText(IDC_INFO,str);
	}
	else
		this->SetDlgItemText(IDC_INFO,"");
	
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgCrossSection::OnFtable() 
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;
	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;
	if(pDoc->lspc.pReachInfo == NULL)
		return;
	double *fh = new double[60];  
	double *fv = new double[60]; 
	double *fq = new double[60];
	double *qk = new double[60];
	double *fa = new double[60];
	double *fp = new double[60];
	double *fr = new double[60];				
	double *fsa = new double[60];				

	if(pDoc->lspc.pReachInfo[nIndex].poly.pointList.GetCount() > 3)
	{
		pDoc->lspc.pReachInfo[nIndex].ftable(fh,fv,fq,qk,fa,fp,fr,fsa);			
	}
	else
	{
		//add flood plane to this version
		//create 4 points and add them to the poly 
		Point *pt = new Point[4];
		pt[0].x = 0;
		pt[0].y = 0;
		pt[1].x = (1-pDoc->lspc.pReachInfo[nIndex].r1)*pDoc->lspc.pReachInfo[nIndex].width/2;
		pt[1].y = pDoc->lspc.pReachInfo[nIndex].depth;
		pt[2].x = pDoc->lspc.pReachInfo[nIndex].width*pDoc->lspc.pReachInfo[nIndex].r1+pt[1].x;
		pt[2].y = pDoc->lspc.pReachInfo[nIndex].depth;
		pt[3].x = pDoc->lspc.pReachInfo[nIndex].width;
		pt[3].y = 0;
		pDoc->lspc.pReachInfo[nIndex].poly.pointList.AddTail((CObject*)&pt[0]);
		pDoc->lspc.pReachInfo[nIndex].poly.pointList.AddTail((CObject*)&pt[1]);
		pDoc->lspc.pReachInfo[nIndex].poly.pointList.AddTail((CObject*)&pt[2]);
		pDoc->lspc.pReachInfo[nIndex].poly.pointList.AddTail((CObject*)&pt[3]);
		pDoc->lspc.pReachInfo[nIndex].ftable(fh,fv,fq,qk,fa,fp,fr,fsa);			
		pDoc->lspc.pReachInfo[nIndex].poly.pointList.RemoveAll();

		if (pt != NULL) delete []pt;          
	}

	CPageNoAdd dlg;
	for (int i=0; i<60; i++)
	{
		fsa[i] *= 2.29568411386593E-05;	// ft2 to acre
		fv[i] *= 2.29568411386593E-05;	// ft3 to acre-ft
	}
	dlg.nIndex = nIndex;
	dlg.fh  = fh;	// depth in ft
	dlg.fsa = fsa;	// ft2 to acre
	dlg.fv  = fv;	// ft3 to acre-ft
	dlg.fq  = fq;	// outflow in cfs
	dlg.DoModal();

	if (fh != NULL) delete []fh;      
	if (fv != NULL) delete []fv;     
	if (fq != NULL) delete []fq;      
	if (qk != NULL) delete []qk;      
	if (fa != NULL) delete []fa;     
	if (fp != NULL) delete []fp;      
	if (fr != NULL) delete []fr;			    
	if (fsa != NULL) delete []fsa;			    
}

void CDlgCrossSection::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;
	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;

	if(pDoc->lspc.pReachInfo == NULL)
		return;
	CPoint pt = point;

	//this->ClientToScreen(&pt);
	CWnd *pWnd = this->GetDlgItem(IDC_MAIN);
	ASSERT(pWnd != NULL);

	CRect rect;
	pWnd->GetWindowRect(&rect);
	this->ClientToScreen(&pt);

	//if the mode is edit and the point is inside our designated region
	if(nMode == EDITMODE && rect.PtInRect(pt) && nSel != -1)
	{
	  POSITION pos = poly->pointList.FindIndex(nSel);
	  Point *pti = (Point *)poly->pointList.GetAt(pos);
	  pti->x = (double)point.x;
	  pti->y = (double)point.y;
		//convert to actual units
		int rec_width = rect.Width();
		if (rec_width > 0)
			pti->x = pti->x / rec_width * pDoc->lspc.pReachInfo[nIndex].width;
		int rec_height = rect.Height();
		if (rec_height > 0)
			pti->y = (rec_height - pti->y) / rec_height * pDoc->lspc.pReachInfo[nIndex].depth;
		//end
	  this->Invalidate();
	}

	//enable the edit button if there are pts in the list
	if(poly != NULL && poly->pointList.GetCount() == 0)
	{
		//disable the edit button
		CWnd *pWnd = NULL;
		pWnd = GetDlgItem(IDCANCEL);
		if(pWnd != NULL)
			pWnd->EnableWindow(TRUE);
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgCrossSection::OnEdit() 
{
	nMode = EDITMODE;
}

BOOL CDlgCrossSection::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// if the ptlist contains no 
	if(poly == NULL || poly->pointList.GetCount() == 0)
	{
		//disable the edit button
		CWnd *pWnd = NULL;
		pWnd = GetDlgItem(IDCANCEL);
		if(pWnd != NULL)
			pWnd->EnableWindow(FALSE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
