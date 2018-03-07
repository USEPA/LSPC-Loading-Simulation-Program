// Page280.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page280.h"
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
// CPage280 dialog


IMPLEMENT_DYNCREATE(CPage280, CPropertyPage)

CPage280::CPage280() : CPropertyPage(CPage280::IDD)
{
	//{{AFX_DATA_INIT(CPage280)
	m_badfg = FALSE;
	m_baddfg = FALSE;
	m_bawdfg = FALSE;
	//}}AFX_DATA_INIT
}

CPage280::~CPage280()
{
}

void CPage280::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage280)
	DDX_Check(pDX, IDC_CHECK1, m_badfg);
	DDX_Check(pDX, IDC_CHECK2, m_baddfg);
	DDX_Check(pDX, IDC_CHECK3, m_bawdfg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage280, CPropertyPage)
	//{{AFX_MSG_MAP(CPage280)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage280 message handlers
BOOL CPage280::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;
	CLSPCDoc *pDoc = NULL;
	if(pView != NULL)
	{
		pDoc = pView->GetDocument();
	}

	if(!bSaveAndValidate)
	{
		if(pDoc != NULL)
		{
			m_badfg = (pDoc->lspc.adfgrch   == 0)?FALSE:TRUE;
			m_baddfg = (pDoc->lspc.maddfgrch == 0)?FALSE:TRUE;
			m_bawdfg = (pDoc->lspc.mawdfgrch == 0)?FALSE:TRUE;
		}
			
		CWnd *pWnd;

//		pWnd = this->GetDlgItem(IDC_CHECK1);
//		if(!m_badfg)
//			pWnd->EnableWindow(false);
		pWnd = this->GetDlgItem(IDC_CHECK2);
		pWnd->EnableWindow(true);
		if(!m_baddfg)
			pWnd->EnableWindow(false);
		pWnd = this->GetDlgItem(IDC_CHECK3);
		pWnd->EnableWindow(true);
		if(!m_bawdfg)
			pWnd->EnableWindow(false);
	}

	CPropertyPage::UpdateData(bSaveAndValidate);
	
	if(bSaveAndValidate)
	{
		if(pDoc != NULL)
		{
			pDoc->lspc.adfgrch   = (!m_badfg)?FALSE:TRUE;
			pDoc->lspc.maddfgrch = (!m_baddfg)?FALSE:TRUE;
			pDoc->lspc.mawdfgrch = (!m_bawdfg)?FALSE:TRUE;
		}
	}
	return TRUE;
}
