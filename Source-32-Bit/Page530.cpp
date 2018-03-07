// Page530.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page530.h"
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
// CPage530 property page

IMPLEMENT_DYNCREATE(CPage530, CPropertyPage)

CPage530::CPage530() : CPropertyPage(CPage530::IDD)
{
	//{{AFX_DATA_INIT(CPage530)
	m_bphyfg = FALSE;
	m_bzoofg = FALSE;
	m_bbalfg = FALSE;
	m_bsdltfg = FALSE;
	m_bamrfg = FALSE;
	m_bdecfg = FALSE;
	m_bnsfg = FALSE;
	m_borefg = FALSE;
	//}}AFX_DATA_INIT
}

CPage530::~CPage530()
{
}

void CPage530::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage530)
	DDX_Check(pDX, IDC_CHECK1, m_bphyfg);
	DDX_Check(pDX, IDC_CHECK2, m_bzoofg);
	DDX_Check(pDX, IDC_CHECK3, m_bbalfg);
	DDX_Check(pDX, IDC_CHECK4, m_bsdltfg);
	DDX_Check(pDX, IDC_CHECK5, m_bamrfg);
	DDX_Check(pDX, IDC_CHECK6, m_bdecfg);
	DDX_Check(pDX, IDC_CHECK7, m_bnsfg);
	DDX_Check(pDX, IDC_CHECK8, m_borefg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage530, CPropertyPage)
	//{{AFX_MSG_MAP(CPage530)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage530 message handlers
BOOL CPage530::UpdateData( BOOL bSaveAndValidate)
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
			m_bphyfg  = (pDoc->lspc.phyfg == 0)?FALSE:TRUE;
			m_bzoofg  = (pDoc->lspc.zoofg == 0)?FALSE:TRUE;
			m_bbalfg  = (pDoc->lspc.balfg == 0)?FALSE:TRUE;
			m_bsdltfg = (pDoc->lspc.sdltfg == 0)?FALSE:TRUE;
			m_bamrfg  = (pDoc->lspc.amrfg == 0)?FALSE:TRUE;
			m_bdecfg  = (pDoc->lspc.decfg == 0)?FALSE:TRUE;
			m_bnsfg   = (pDoc->lspc.nsfg == 0)?FALSE:TRUE;
			m_borefg  = (pDoc->lspc.orefg == 0)?FALSE:TRUE;
		}
	}

	CPropertyPage::UpdateData(bSaveAndValidate);
	
	if(bSaveAndValidate)
	{
		if(pDoc != NULL)
		{
			pDoc->lspc.phyfg  = (!m_bphyfg)?0:1;
			pDoc->lspc.zoofg  = (!m_bzoofg)?0:1;
			pDoc->lspc.balfg  = (!m_bbalfg)?0:1;
			pDoc->lspc.sdltfg = (!m_bsdltfg)?0:1; 
			pDoc->lspc.amrfg  = (!m_bamrfg)?0:1;
			pDoc->lspc.decfg  = (!m_bdecfg)?0:1;
			pDoc->lspc.nsfg   = (!m_bnsfg)?0:1; 
			pDoc->lspc.orefg  = (!m_borefg)?0:1;  
		}
	}
	return TRUE;
}
