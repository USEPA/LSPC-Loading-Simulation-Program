// PageNoAdd.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "PageNoAdd.h"
#include "MainFrm.h"
#include "LSPCView.h"
#include "LSPCDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageNoAdd property page

IMPLEMENT_DYNCREATE(CPageNoAdd, CDialog)

CPageNoAdd::CPageNoAdd() : CDialog(CPageNoAdd::IDD)
{
	//{{AFX_DATA_INIT(CPageNoAdd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	nIndex = -1;
	fh  = NULL;
	fsa = NULL;
	fv  = NULL;
	fq  = NULL;
}

CPageNoAdd::~CPageNoAdd()
{

}

void CPageNoAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageNoAdd)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CPageNoAdd, CDialog)
	//{{AFX_MSG_MAP(CPageNoAdd)
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageNoAdd message handlers
void CPageNoAdd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&rect);

	if(m_Grid.GetSafeHwnd() != NULL)
	{
		m_Grid.MoveWindow(&rect);
		m_Grid.AutoSize();
	}
}

BOOL CPageNoAdd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// initialize the grid control
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(TRUE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.SetHeaderSort(FALSE);

	TRY
	{
		InitGrid();
		UpdateData(false);
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		e->Delete();
		return FALSE;
	}
    END_CATCH

	CRect rect;
	GetClientRect(&rect);

	if(GetSafeHwnd() && m_Grid.GetSafeHwnd())
	{
		m_Grid.MoveWindow(&rect);
		m_Grid.AutoSize();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageNoAdd::OnEditCopy() 
{
	m_Grid.OnEditCopy();	
}

void CPageNoAdd::OnEditCut() 
{
	m_Grid.OnEditCut();
}

void CPageNoAdd::OnEditPaste() 
{
	m_Grid.OnEditPaste();
}

void CPageNoAdd::InitGrid()
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
	m_Grid.SetColumnCount(4+maxDSchan);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString *str = NULL;
	str = new CString[4+maxDSchan];
	str[0]= "Layer";
	str[1]= "Depth (ft)";
	str[2]= "Surface Area (acre)";
	str[3]= "Volume (acre-ft)";
	if (maxDSchan == 1)
	{
		str[4].Format("Outflow (cfs)");
	}
	else
	{
		for (int j=0; j<maxDSchan; j++)
			str[j+4].Format("Outflow_%d (cfs)", j+1);
	}

	UINT *format = NULL;
	format = new UINT[4+maxDSchan];
	for (int j=0; j<4+maxDSchan; j++)
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

BOOL CPageNoAdd::UpdateData(BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);

	CLSPCDoc *pDoc = pView->GetDocument();

	if(!bSaveAndValidate)
	{
		if(fh == NULL)
			return true;

		CSize size = m_Grid.GetTextExtent(" ");
		
		if (pDoc->lspc.pReachFtable[nIndex].nFTrecord > 0)	// user defined FTABLE
		{
			int nStart = pDoc->lspc.pReachFtable[nIndex].nFTstart;
			int nRecord = pDoc->lspc.pReachFtable[nIndex].nFTrecord;
			for (int j=nStart; j<nStart+nRecord; j++)
			{
				if (pDoc->lspc.pReachInfo[nIndex].rchid == pDoc->lspc.pFtable[j].rchid)
				{
					m_Grid.InsertRow(_T(""), -1);	
					int nRow = m_Grid.GetRowCount() - 1;
					m_Grid.SetRowHeight(nRow, size.cy);
					m_Grid.SetItemInt(nRow,  0,j-nStart+1);
					m_Grid.SetItemFloat(nRow,1,pDoc->lspc.pFtable[j].depth);				//ft
					m_Grid.SetItemFloat(nRow,2,pDoc->lspc.pFtable[j].area / 43559.66);		//ft2 to acre
					m_Grid.SetItemFloat(nRow,3,pDoc->lspc.pFtable[j].vol / 43559.66);		//ft3 to acre-ft

					if (pDoc->lspc.pFtable[j].disch != NULL)
					{
						for (int k=0; k<pDoc->lspc.pChannelInfo[nIndex].nDSchan; k++)
							m_Grid.SetItemFloat(nRow,4+k,pDoc->lspc.pFtable[j].disch[k]);	//cfs
					}
				}
			}
		}
		else
		{
			for(int i=0; i<60; ++i)
			{
				m_Grid.InsertRow(_T(""), -1);	
				int nRow = m_Grid.GetRowCount() - 1;
				m_Grid.SetRowHeight(nRow, size.cy);
				m_Grid.SetItemInt(nRow,  0,i+1);
				m_Grid.SetItemFloat(nRow,1,fh[i]);	//ft
				m_Grid.SetItemFloat(nRow,2,fsa[i]);	//ft2 to acre
				m_Grid.SetItemFloat(nRow,3,fv[i]);	//ft3 to acre-ft
				m_Grid.SetItemFloat(nRow,4,fq[i]);	//cfs
			}
		}
		m_Grid.Invalidate();
		m_Grid.AutoSize();
	}
	return TRUE;
}

void CPageNoAdd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}
