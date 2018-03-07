// Page460.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page460.h"
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
// CPage460 property page


IMPLEMENT_DYNCREATE(CPage460, CPropertyPage)

CPage460::CPage460() : CPropertyPage(CPage460::IDD)
{
	//{{AFX_DATA_INIT(CPage460)
	m_msltfg = FALSE;
	m_miftfg = FALSE;
	m_mgwtfg = FALSE;
	//}}AFX_DATA_INIT
}

CPage460::~CPage460()
{
}

void CPage460::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage460)
	DDX_Check(pDX, IDC_CHECK1, m_msltfg);
	DDX_Check(pDX, IDC_CHECK2, m_miftfg);
	DDX_Check(pDX, IDC_CHECK3, m_mgwtfg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage460, CPropertyPage)
	//{{AFX_MSG_MAP(CPage460)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage460 message handlers
BOOL CPage460::UpdateData( BOOL bSaveAndValidate)
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
			m_msltfg = (pDoc->lspc.msltfg == 0)?FALSE:TRUE;
			m_miftfg = (pDoc->lspc.miftfg == 0)?FALSE:TRUE;
			m_mgwtfg = (pDoc->lspc.mgwtfg == 0)?FALSE:TRUE;
		}

		CWnd *pWnd;

		pWnd = this->GetDlgItem(IDC_CHECK1);
		pWnd->EnableWindow(true);
		if(!m_msltfg)
			pWnd->EnableWindow(false);
		pWnd = this->GetDlgItem(IDC_CHECK2);
		pWnd->EnableWindow(true);
		if(!m_miftfg)
			pWnd->EnableWindow(false);
		pWnd = this->GetDlgItem(IDC_CHECK3);
		pWnd->EnableWindow(true);
		if(!m_mgwtfg)
			pWnd->EnableWindow(false);
	}

	CPropertyPage::UpdateData(bSaveAndValidate);
	
	if(bSaveAndValidate)
	{
		if(pDoc != NULL)
		{
			pDoc->lspc.msltfg = (!m_msltfg)?0:1;
			pDoc->lspc.miftfg = (!m_miftfg)?0:1;
			pDoc->lspc.mgwtfg = (!m_mgwtfg)?0:1;
		}
	}
	return TRUE;
}
