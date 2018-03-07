// Page32.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page32.h"
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
// CPage32 dialog


IMPLEMENT_DYNCREATE(CPage32, CPropertyPage)

CPage32::CPage32() : CPropertyPage(CPage32::IDD)
{
	//{{AFX_DATA_INIT(CPage32)
	m_nLUchangeFlag = 0;
	m_strLUchangeFilePath = _T("");
	//}}AFX_DATA_INIT
}

CPage32::~CPage32()
{
}


void CPage32::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage32)
	DDX_Text(pDX, IDC_LUchangeFlag, m_nLUchangeFlag);
	DDV_MinMaxInt(pDX, m_nLUchangeFlag, 0, 10000);
	DDX_Text(pDX, IDC_LUchangeFilePath, m_strLUchangeFilePath);
	DDV_MaxChars(pDX, m_strLUchangeFilePath, 10240);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage32, CDialog)
	//{{AFX_MSG_MAP(CPage32)
	ON_EN_KILLFOCUS(IDC_LUchangeFlag, OnChange)
	ON_BN_CLICKED(IDC_LUchangeButton, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage32 message handlers
BOOL CPage32::UpdateData( BOOL bSaveAndValidate)
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
		m_nLUchangeFlag = pDoc->lspc.cLU.nLUchangeFlag;
		m_strLUchangeFilePath = pDoc->lspc.cLU.strLUchangeFilePath;
	}

	CPropertyPage::UpdateData(bSaveAndValidate);
	
	if(bSaveAndValidate)
	{
		pDoc->lspc.cLU.nLUchangeFlag = (m_nLUchangeFlag == 0)?0:1;
		pDoc->lspc.cLU.strLUchangeFilePath = m_strLUchangeFilePath;
	}
	return TRUE;
}

void CPage32::OnChange() 
{
	CPropertyPage::UpdateData(true);
}

void CPage32::OnBrowse() 
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
	
	if (m_nLUchangeFlag == 0)
	{
		AfxMessageBox("Landuse Change Flag must be ON to browse the file.",MB_ICONINFORMATION);
		return;
	}

	static char BASED_CODE szFilter[] = "Landuse Change File (*.inp)|*.inp|Model Database File (*.mdb)|*.mdb|All Files (*.*)|*.*||";
	CFileDialog dlgLUchangeFile(TRUE, "*.inp", m_strLUchangeFilePath, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);
	if (dlgLUchangeFile.DoModal() == IDOK)
	{
		m_strLUchangeFilePath = dlgLUchangeFile.GetPathName();
		int nIndex = m_strLUchangeFilePath.ReverseFind('\\');
		if(nIndex != -1)
		{
			pDoc->lspc.cLU.nLUchangeFlag = (m_nLUchangeFlag == 0)?0:1;
			pDoc->lspc.cLU.strLUchangeFilePath = m_strLUchangeFilePath;

			// read landuse change file
			CString strFileName = pDoc->lspc.cLU.strLUchangeFilePath;	
			if (strFileName != "")
			{
				CString str = strFileName;
				str.MakeLower();
				str = str.Right(4);
				if(str.CompareNoCase(".mdb") == 0)
				{
					pApp->strLocation = strFileName;
					int nIndex = pApp->strLocation.ReverseFind('\\');
					if(nIndex != -1)
					{
						pApp->strDBFilePath = pApp->strLocation.Left(nIndex+1);
						pApp->strDBFileName = pApp->strLocation.Right(pApp->strLocation.GetLength() - nIndex - 1);
					}
					pWnd->m_wndStatusBar.SetPaneText(0,"Reading landuse change data from the database: File "+strFileName);
					pDoc->lspc.cLU.ReadLUchangeDatabase(pDoc->lspc.bRunNIMS);		
					pWnd->m_wndStatusBar.SetPaneText(0,"Ready");
				}
				else
				{
					pWnd->m_wndStatusBar.SetPaneText(0,"Reading landuse change data from the text file: File "+strFileName);
					pDoc->lspc.cLU.ReadLUchangeTextFile(strFileName);									
					pWnd->m_wndStatusBar.SetPaneText(0,"Ready");
				}
			}
		}

		CPropertyPage::UpdateData(FALSE);
	}
}
