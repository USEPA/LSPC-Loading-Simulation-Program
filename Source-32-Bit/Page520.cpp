// Page520.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page520.h"
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
// CPage520 property page

IMPLEMENT_DYNCREATE(CPage520, CPropertyPage)

CPage520::CPage520() : CPropertyPage(CPage520::IDD)
{
	//{{AFX_DATA_INIT(CPage520)
	m_btamfg  = FALSE;
	m_bno2fg  = FALSE;
	m_bpo4fg  = FALSE;
	m_bamvfg  = FALSE;
	m_bdenfg  = FALSE;
	m_badnhfg = FALSE;
	m_badpofg = FALSE;
	m_bmphfg  = FALSE;
	//}}AFX_DATA_INIT
}

CPage520::~CPage520()
{
}

void CPage520::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage520)
	DDX_Check(pDX, IDC_CHECK1, m_btamfg);
	DDX_Check(pDX, IDC_CHECK2, m_bno2fg);
	DDX_Check(pDX, IDC_CHECK3, m_bpo4fg);
	DDX_Check(pDX, IDC_CHECK4, m_bamvfg);
	DDX_Check(pDX, IDC_CHECK5, m_bdenfg);
	DDX_Check(pDX, IDC_CHECK6, m_badnhfg);
	DDX_Check(pDX, IDC_CHECK7, m_badpofg);
	DDX_Check(pDX, IDC_CHECK8, m_bmphfg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage520, CPropertyPage)
	//{{AFX_MSG_MAP(CPage520)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage520 message handlers
BOOL CPage520::UpdateData( BOOL bSaveAndValidate)
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
			m_btamfg  = (pDoc->lspc.tamfg == 0)?FALSE:TRUE;
			m_bno2fg  = (pDoc->lspc.no2fg == 0)?FALSE:TRUE;
			m_bpo4fg  = (pDoc->lspc.po4fg == 0)?FALSE:TRUE;
			m_bamvfg  = (pDoc->lspc.amvfg == 0)?FALSE:TRUE;
			m_bdenfg  = (pDoc->lspc.denfg == 0)?FALSE:TRUE;
			m_badnhfg = (pDoc->lspc.adnhfg == 0)?FALSE:TRUE;
			m_badpofg = (pDoc->lspc.adpofg == 0)?FALSE:TRUE;
			m_bmphfg  = (pDoc->lspc.mphfg == 0)?FALSE:TRUE;
		}
	}

	CPropertyPage::UpdateData(bSaveAndValidate);
	
	if(bSaveAndValidate)
	{
		if(pDoc != NULL)
		{
			pDoc->lspc.tamfg  = (!m_btamfg)?0:1;
			pDoc->lspc.no2fg  = (!m_bno2fg)?0:1;
			pDoc->lspc.po4fg  = (!m_bpo4fg)?0:1;
			pDoc->lspc.amvfg  = (!m_bamvfg)?0:1; 
			pDoc->lspc.denfg  = (!m_bdenfg)?0:1;
			pDoc->lspc.adnhfg = (!m_badnhfg)?0:1;
			pDoc->lspc.adpofg = (!m_badpofg)?0:1; 
			pDoc->lspc.mphfg  = (!m_bmphfg)?0:1;  
		}
	}
	return TRUE;
}
