// Page30.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page30.h"
#include "DirDialog.h"
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
// CPage30 property page

IMPLEMENT_DYNCREATE(CPage30, CPropertyPage)

CPage30::CPage30() : CPropertyPage(CPage30::IDD)
{
	//{{AFX_DATA_INIT(CPage30)
	m_strWeatherLocation = _T("");
	//}}AFX_DATA_INIT
}

CPage30::~CPage30()
{
}

void CPage30::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage30)
	DDX_Text(pDX, IDC_WSEDIT, m_strWeatherLocation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage30, CPropertyPage)
	//{{AFX_MSG_MAP(CPage30)
	ON_BN_CLICKED(IDC_WSSB, OnWssb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage30 message handlers

void CPage30::OnWssb() 
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;
	CLSPCDoc *pDoc = NULL;
	pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;
	CDirDialog dlg;
	dlg.m_strSelDir = m_strWeatherLocation;
	dlg.m_strTitle = "Weather Station Data Directory:";
	if(dlg.DoBrowse() == 1)
	{
		m_strWeatherLocation = dlg.m_strPath;	
		pDoc->lspc.cWD.strWeatherFolderPath = m_strWeatherLocation;
		UpdateData(FALSE);	
	}
}

BOOL CPage30::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;
	CLSPCDoc *pDoc = NULL;
	pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return true;

	if(!bSaveAndValidate)
	{
		m_strWeatherLocation = pDoc->lspc.cWD.strWeatherFolderPath;
	}

	CPropertyPage::UpdateData(bSaveAndValidate);
	
	if(bSaveAndValidate)
	{
		if(m_strWeatherLocation.GetAt(m_strWeatherLocation.GetLength()-1) != '\\'
			&& m_strWeatherLocation.GetAt(m_strWeatherLocation.GetLength()-1) != '/')
			m_strWeatherLocation += "\\";
		pDoc->lspc.cWD.strWeatherFolderPath = m_strWeatherLocation;

	}
	return TRUE;
}
