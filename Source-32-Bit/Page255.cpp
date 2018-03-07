// Page255.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page255.h"
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
// CPage255 property page

IMPLEMENT_DYNCREATE(CPage255, CPropertyPage)

CPage255::CPage255() : CPropertyPage(CPage255::IDD)
{
	//{{AFX_DATA_INIT(CPage255)
	m_bvqofg = FALSE;
	m_bqifwfg = FALSE;
	m_bviqcfg = FALSE;
	m_bqagwfg = FALSE;
	m_bvaqcfg = FALSE;
	m_badfxfg = FALSE;
	m_bqsowfg = FALSE;
	m_bvsqcfg = FALSE;
	m_bmaddfg = FALSE;
	m_bmawdfg = FALSE;
	//}}AFX_DATA_INIT
}

CPage255::~CPage255()
{
}

void CPage255::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage255)
	DDX_Check(pDX, IDC_CHECK1, m_bvqofg);
	DDX_Check(pDX, IDC_CHECK2, m_bqifwfg);
	DDX_Check(pDX, IDC_CHECK3, m_bviqcfg);
	DDX_Check(pDX, IDC_CHECK4, m_bqagwfg);
	DDX_Check(pDX, IDC_CHECK5, m_bvaqcfg);
	DDX_Check(pDX, IDC_CHECK6, m_badfxfg);
	DDX_Check(pDX, IDC_CHECK7, m_bqsowfg);
	DDX_Check(pDX, IDC_CHECK8, m_bvsqcfg);
	DDX_Check(pDX, IDC_CHECK9, m_bmaddfg);
	DDX_Check(pDX, IDC_CHECK10, m_bmawdfg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage255, CPropertyPage)
	//{{AFX_MSG_MAP(CPage255)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage255 message handlers
BOOL CPage255::UpdateData( BOOL bSaveAndValidate)
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
			m_bvqofg = (pDoc->lspc.vqofg == 0)?FALSE:TRUE;
			m_bqsowfg = (pDoc->lspc.qsowfg == 0)?FALSE:TRUE;
			m_bvsqcfg = (pDoc->lspc.vsqcfg == 0)?FALSE:TRUE;
			m_bqifwfg = (pDoc->lspc.qifwfg == 0)?FALSE:TRUE;
			m_bviqcfg = (pDoc->lspc.viqcfg == 0)?FALSE:TRUE;
			m_bqagwfg = (pDoc->lspc.qagwfg == 0)?FALSE:TRUE;
			m_bvaqcfg = (pDoc->lspc.vaqcfg == 0)?FALSE:TRUE;
			m_badfxfg = (pDoc->lspc.adfglnd   == 0)?FALSE:TRUE;
			m_bmaddfg = (pDoc->lspc.maddfglnd == 0)?FALSE:TRUE;
			m_bmawdfg = (pDoc->lspc.mawdfglnd == 0)?FALSE:TRUE;
		}
		if(pDoc->lspc.nWaterControl == 0)
		{
			CWnd *pWnd;

			pWnd = this->GetDlgItem(IDC_CHECK1);
			pWnd->EnableWindow(true);
			if(!m_bvqofg)
				pWnd->EnableWindow(false);
			pWnd = this->GetDlgItem(IDC_CHECK3);
			pWnd->EnableWindow(true);
			if(!m_bviqcfg)
				pWnd->EnableWindow(false);
			pWnd = this->GetDlgItem(IDC_CHECK5);
			pWnd->EnableWindow(true);
			if(!m_bvaqcfg)
				pWnd->EnableWindow(false);
			pWnd = this->GetDlgItem(IDC_CHECK8);
			pWnd->EnableWindow(true);
			if(!m_bvsqcfg)
				pWnd->EnableWindow(false);
			pWnd = this->GetDlgItem(IDC_CHECK9);
			pWnd->EnableWindow(true);
			if(!m_bmaddfg)
				pWnd->EnableWindow(false);
			pWnd = this->GetDlgItem(IDC_CHECK10);
			pWnd->EnableWindow(true);
			if(!m_bmawdfg)
				pWnd->EnableWindow(false);

			++pDoc->lspc.nWaterControl;
		}

	}

	CPropertyPage::UpdateData(bSaveAndValidate);
	
	if(bSaveAndValidate)
	{
		if(pDoc != NULL)
		{
			pDoc->lspc.vqofg = (!m_bvqofg)?FALSE:TRUE;
			pDoc->lspc.qsowfg = (!m_bqsowfg)?FALSE:TRUE;			 
			pDoc->lspc.vsqcfg = (!m_bvsqcfg)?FALSE:TRUE;
			pDoc->lspc.qifwfg = (!m_bqifwfg)?FALSE:TRUE;
			pDoc->lspc.viqcfg = (!m_bviqcfg)?FALSE:TRUE;
			pDoc->lspc.qagwfg = (!m_bqagwfg)?FALSE:TRUE;
			pDoc->lspc.vaqcfg = (!m_bvaqcfg)?FALSE:TRUE;
			pDoc->lspc.adfglnd   = (!m_badfxfg)?FALSE:TRUE;
			pDoc->lspc.maddfglnd = (!m_bmaddfg)?FALSE:TRUE;
			pDoc->lspc.mawdfglnd = (!m_bmawdfg)?FALSE:TRUE;
		}
	}
	return TRUE;
}
