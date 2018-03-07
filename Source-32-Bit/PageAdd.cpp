// PageAdd.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "PageAdd.h"
#include "MyPropertySheet1.h"
#include "MyPropertySheet2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageAdd property page

IMPLEMENT_DYNCREATE(CPageAdd, CPropertyPage)

CPageAdd::CPageAdd() : CPropertyPage(CPageAdd::IDD)
{
	//{{AFX_DATA_INIT(CPageAdd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPageAdd::CPageAdd(UINT nIDCaption) : CPropertyPage(CPageAdd::IDD,nIDCaption)
{

}

CPageAdd::~CPageAdd()
{

}

void CPageAdd::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageAdd)
	DDX_Control(pDX, IDC_INFO, m_cInfo);
	DDX_Control(pDX, IDC_ADD, m_cAdd);
	DDX_Control(pDX, IDC_DELETE, m_cDelete);
	DDX_Control(pDX, IDC_EDIT, m_cEdit);
	DDX_Control(pDX, IDC_EXPORT, m_cExport);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CPageAdd, CPropertyPage)
	//{{AFX_MSG_MAP(CPageAdd)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_EXPORT, OnExport)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageAdd message handlers

void CPageAdd::OnAdd() 
{
	m_Grid.InsertRow(_T(""), -1);
	CSize size = m_Grid.GetTextExtent(" ");
	m_Grid.SetRowHeight(m_Grid.GetRowCount()-1, size.cy);
	m_Grid.Invalidate();
}

void CPageAdd::OnDelete() 
{
	int nRow = m_Grid.GetFocusCell().row;
    if(nRow >= 0)
    {
	    m_Grid.DeleteRow(nRow);	
	    m_Grid.Invalidate();
    }
}

void CPageAdd::OnEdit() 
{
	EditRow();	
}

void CPageAdd::OnExport() 
{
	ExportFtable();	
}

void CPageAdd::OnEditCopy() 
{
	m_Grid.OnEditCopy();	
}

void CPageAdd::OnEditCut() 
{
	m_Grid.OnEditCut();
}

void CPageAdd::OnEditPaste() 
{
	m_Grid.OnEditPaste();
}

void CPageAdd::OnSize(UINT nType, int cx, int cy) 
{
	CPropertyPage::OnSize(nType, cx, cy);
	ArrangeCtrls();
}

void CPageAdd::ArrangeCtrls()
{
	if(GetSafeHwnd())
	{
		int totalHeight = 0, gridHeight;
		CRect rectClient, rectInfo, rectAdd, rectDelete, rectEdit, rectExport;
		GetClientRect(&rectClient);

		if(m_cInfo.GetSafeHwnd() && m_Grid.GetSafeHwnd() && m_cAdd.GetSafeHwnd()
			&& m_cDelete.GetSafeHwnd() && m_cEdit.GetSafeHwnd() && m_cExport.GetSafeHwnd())
		{
			m_cInfo.GetWindowRect(&rectInfo);
			totalHeight += rectInfo.Height();
			m_cAdd.GetWindowRect(&rectAdd);
			m_cDelete.GetWindowRect(&rectDelete);
			m_cEdit.GetWindowRect(&rectEdit);
			m_cExport.GetWindowRect(&rectExport);

			totalHeight += rectAdd.Height() + 10;
			gridHeight = rectClient.Height() - totalHeight;
			if(gridHeight < 10)
				gridHeight = 10;

			m_cInfo.MoveWindow(0, 0, rectInfo.Width(), rectInfo.Height());
			m_Grid.MoveWindow(0, rectInfo.Height(), rectClient.Width(), gridHeight);
			m_cAdd.MoveWindow(0, rectInfo.Height()+gridHeight+10, rectAdd.Width(), rectAdd.Height());
			m_cDelete.MoveWindow(rectAdd.Width()+20, rectInfo.Height()+gridHeight+10, rectDelete.Width(), rectDelete.Height());
//			m_cEdit.MoveWindow(rectAdd.Width()+rectDelete.Width()+40, rectInfo.Height()+gridHeight+10, rectEdit.Width(), rectEdit.Height());
//			m_cExport.MoveWindow(rectAdd.Width()+rectDelete.Width()+rectEdit.Width()+60, rectInfo.Height()+gridHeight+10, rectExport.Width(), rectExport.Height());
			m_cEdit.MoveWindow(0, rectInfo.Height()+gridHeight+10, rectEdit.Width(), rectEdit.Height());
			m_cExport.MoveWindow(rectAdd.Width()+50, rectInfo.Height()+gridHeight+10, rectExport.Width(), rectExport.Height());
		}
	}		
}

BOOL CPageAdd::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	ArrangeCtrls();

	EnableButtons(false);	
	
	TRY
	{
		InitGrid();
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		e->Delete();
		return FALSE;
	}
    END_CATCH

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageAdd::InitGrid()
{
	// initialize the grid control
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(TRUE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableTitleTips(TRUE);
	m_Grid.SetHeaderSort(FALSE);
	m_Grid.SetTextBkColor(RGB(0xFF, 0xFF, 0xFF));
//	m_Grid.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));
}

void CPageAdd::EditRow()
{

}

void CPageAdd::ExportFtable()
{

}

void CPageAdd::EnableButtons(bool bEnable)
{
	m_cAdd.EnableWindow(bEnable);
	m_cDelete.EnableWindow(bEnable);
}

void CPageAdd::ShowEditButton(bool bShow)
{
	m_cEdit.ShowWindow(bShow?SW_SHOW:SW_HIDE);
}

void CPageAdd::ShowExportButton(bool bShow)
{
	m_cExport.ShowWindow(bShow?SW_SHOW:SW_HIDE);
}

void CPageAdd::ShowInfoText(bool bShow)
{
	m_cInfo.ShowWindow(bShow?SW_SHOW:SW_HIDE);
}

BOOL CPageAdd::UpdateData(BOOL bSaveAndValidate)
{
	return TRUE;
}

void CPageAdd::Log(LPCTSTR lpszFormat, ...)
{
	CLSPCApp *pApp = (CLSPCApp*) AfxGetApp();
	va_list args;
	va_start(args, lpszFormat);

	int nBuf;
	TCHAR szBuffer[1024];

	nBuf = _vsntprintf(szBuffer, 1024, lpszFormat, args);
	pApp->Log(szBuffer);
	
	va_end(args);
}
