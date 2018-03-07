// Page100.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page100.h"
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
// CPage100 property page

IMPLEMENT_DYNCREATE(CPage100, CPropertyPage)

CPage100::CPage100() : CPropertyPage(CPage100::IDD)
{
	//{{AFX_DATA_INIT(CPage100)
	m_bvcsfg = FALSE;
	m_bvuzfg = FALSE;
	m_bvnnfg = FALSE;
	m_bvifwfg = FALSE;
	m_bvircfg = FALSE;
	m_bvlefg = FALSE;
	//}}AFX_DATA_INIT
}

CPage100::~CPage100()
{
}

void CPage100::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage100)
	DDX_Check(pDX, IDC_CHECK1, m_bvcsfg);
	DDX_Check(pDX, IDC_CHECK2, m_bvuzfg);
	DDX_Check(pDX, IDC_CHECK3, m_bvnnfg);
	DDX_Check(pDX, IDC_CHECK4, m_bvifwfg);
	DDX_Check(pDX, IDC_CHECK5, m_bvircfg);
	DDX_Check(pDX, IDC_CHECK6, m_bvlefg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage100, CPropertyPage)
	//{{AFX_MSG_MAP(CPage100)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage100 message handlers
BOOL CPage100::UpdateData( BOOL bSaveAndValidate)
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
			m_bvcsfg = (pDoc->lspc.vcsfg == 0)?FALSE:TRUE;
			m_bvuzfg = (pDoc->lspc.vuzfg == 0)?FALSE:TRUE;
			m_bvnnfg = (pDoc->lspc.vnnfg == 0)?FALSE:TRUE;
			m_bvifwfg = (pDoc->lspc.vifwfg == 0)?FALSE:TRUE;
			m_bvircfg = (pDoc->lspc.vircfg == 0)?FALSE:TRUE;
			m_bvlefg = (pDoc->lspc.vlefg == 0)?FALSE:TRUE;
		}

		CWnd *pWnd;

		pWnd = this->GetDlgItem(IDC_CHECK1);
		pWnd->EnableWindow(true);
		if(!m_bvcsfg)
			pWnd->EnableWindow(false);
		pWnd = this->GetDlgItem(IDC_CHECK2);
		pWnd->EnableWindow(true);
		if(!m_bvuzfg)
			pWnd->EnableWindow(false);
		pWnd = this->GetDlgItem(IDC_CHECK3);
		pWnd->EnableWindow(true);
		if(!m_bvnnfg)
			pWnd->EnableWindow(false);
		pWnd = this->GetDlgItem(IDC_CHECK4);
		pWnd->EnableWindow(true);
		if(!m_bvifwfg)
			pWnd->EnableWindow(false);
		pWnd = this->GetDlgItem(IDC_CHECK5);
		pWnd->EnableWindow(true);
		if(!m_bvircfg)
			pWnd->EnableWindow(false);
		pWnd = this->GetDlgItem(IDC_CHECK6);
		pWnd->EnableWindow(true);
		if(!m_bvlefg)
			pWnd->EnableWindow(false);
	}

	CPropertyPage::UpdateData(bSaveAndValidate);
	
	if(bSaveAndValidate)
	{
		if(pDoc != NULL)
		{
			 pDoc->lspc.vcsfg = (!m_bvcsfg)?0:1;
			 pDoc->lspc.vuzfg = (!m_bvuzfg)?0:1;
			 pDoc->lspc.vnnfg = (!m_bvnnfg)?0:1;
			 pDoc->lspc.vifwfg = (!m_bvifwfg)?0:1;
			 pDoc->lspc.vircfg = (!m_bvircfg)?0:1;
			 pDoc->lspc.vlefg = (!m_bvlefg)?0:1;
		}
	}
	return TRUE;
}