// Page420.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page420.h"
#include "MainFrm.h"
#include "LSPCView.h"
#include "LSPCDoc.h"
#include "LSPCMODEL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage420 property page

IMPLEMENT_DYNCREATE(CPage420, CPropertyPage)

CPage420::CPage420() : CPropertyPage(CPage420::IDD)
{
	//{{AFX_DATA_INIT(CPage420)
	m_nPtSource = 0;
	m_nPtQuals = 0;
	m_strPSLocation = _T("");
	//}}AFX_DATA_INIT
}

CPage420::~CPage420()
{
}

void CPage420::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage420)
	DDX_Text(pDX, IDC_nPtSource, m_nPtSource);
	DDV_MinMaxInt(pDX, m_nPtSource, 0, 10000);
	DDX_Text(pDX, IDC_nPtQuals, m_nPtQuals);
	DDV_MinMaxInt(pDX, m_nPtQuals, 0, 10000);
	DDX_Text(pDX, IDC_PSEDIT, m_strPSLocation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage420, CDialog)
	//{{AFX_MSG_MAP(CPage420)
	ON_EN_KILLFOCUS(IDC_nPtSource, OnChange)
	ON_EN_KILLFOCUS(IDC_nPtQuals, OnChange)
	ON_BN_CLICKED(IDC_PSSB, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage420 message handlers
BOOL CPage420::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;

	CLSPCApp *pApp = (CLSPCApp *) AfxGetApp();
	if(pApp == NULL)
		return false;

	if(!bSaveAndValidate)
	{
		m_nPtSource = pDoc->lspc.nPtSource;
		m_nPtQuals = pDoc->lspc.nPtQuals;
		m_strPSLocation = pDoc->lspc.m_strPSDataLocation;
	}

	CPropertyPage::UpdateData(bSaveAndValidate);
	
	if(bSaveAndValidate)
	{
		pDoc->lspc.nPtSource = m_nPtSource;
		pDoc->lspc.nPtQuals = m_nPtQuals;
		int nIndex = m_strPSLocation.ReverseFind('\\');
		if(nIndex != -1)
		{
			pDoc->lspc.strPointFilePath = m_strPSLocation.Left(nIndex+1);
			pDoc->lspc.strPointFileName = m_strPSLocation.Right(m_strPSLocation.GetLength() - nIndex - 1);
			pDoc->lspc.m_strPSDataLocation = m_strPSLocation;
		}
	}
	return TRUE;
}

void CPage420::OnChange() 
{
	CPropertyPage::UpdateData(true);
}

void CPage420::OnBrowse() 
{
	CMainFrame *pWnd = (CMainFrame *)AfxGetMainWnd();
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;

	CLSPCDoc *pDoc = NULL;
	pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;

	CLSPCApp *pApp = (CLSPCApp *) AfxGetApp();
	if(pApp == NULL)
		return;

	if((pDoc->lspc.m_strFileName.GetLength() == 0 && pDoc->lspc.m_strPath.GetLength() == 0) && !pDoc->lspc.bReadDB) 
	{
		AfxMessageBox("Read input file/database first.");
		pApp->Log("Read input file/database first.");
		return;
	}
	
	if (m_nPtSource <= 0)
	{
		AfxMessageBox("Number of individual point sources should be greater than zero to browse the file.",MB_ICONINFORMATION);
		return;
	}

	static char BASED_CODE szFilter[] = "Point Source Input File (*.inp)|*.inp|Model Database File (*.mdb)|*.mdb|All Files (*.*)|*.*||";
	CFileDialog dlgPSFile(TRUE, "*.inp", m_strPSLocation, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);
	if (dlgPSFile.DoModal() == IDOK)
	{
		m_strPSLocation = dlgPSFile.GetPathName();
		int nIndex = m_strPSLocation.ReverseFind('\\');
		if(nIndex != -1)
		{
			pDoc->lspc.strPointFilePath = m_strPSLocation.Left(nIndex+1);
			pDoc->lspc.strPointFileName = m_strPSLocation.Right(m_strPSLocation.GetLength() - nIndex - 1);
			pDoc->lspc.m_strPSDataLocation = m_strPSLocation;

			// read point source
			CString strPSName = pDoc->lspc.strPointFilePath+pDoc->lspc.strPointFileName;	
			if (strPSName != "")
			{
				CString str = strPSName;
				str.MakeLower();
				str = str.Right(4);
				if(str.CompareNoCase(".mdb") == 0)
				{
					pWnd->m_wndStatusBar.SetPaneText(0,"Reading point source data from the database: File "+strPSName);
					pDoc->lspc.ReadDBPointSource();		
					pWnd->m_wndStatusBar.SetPaneText(0,"Ready");
				}
				else
				{
					pWnd->m_wndStatusBar.SetPaneText(0,"Reading point source data from the text file: File "+strPSName);
					pDoc->lspc.ReadPointSource(strPSName);									
					pWnd->m_wndStatusBar.SetPaneText(0,"Ready");
				}
			}
		}

		CPropertyPage::UpdateData(FALSE);
	}
}
