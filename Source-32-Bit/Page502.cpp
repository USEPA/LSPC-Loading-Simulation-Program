// Page502.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page502.h"
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
// CPage502 property page

IMPLEMENT_DYNCREATE(CPage502, CPropertyPage)

CPage502::CPage502() : CPropertyPage(CPage502::IDD)
{
	//{{AFX_DATA_INIT(CPage502)
	m_midofg = FALSE;
	m_mico2fg = FALSE;
	m_madofg = FALSE;
	m_maco2fg = FALSE;
	//}}AFX_DATA_INIT
}

CPage502::~CPage502()
{
}

void CPage502::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage502)
	DDX_Check(pDX, IDC_CHECK1, m_midofg);
	DDX_Check(pDX, IDC_CHECK2, m_mico2fg);
	DDX_Check(pDX, IDC_CHECK3, m_madofg);
	DDX_Check(pDX, IDC_CHECK4, m_maco2fg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage502, CPropertyPage)
	//{{AFX_MSG_MAP(CPage502)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage502 message handlers
BOOL CPage502::UpdateData( BOOL bSaveAndValidate)
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
			m_midofg = (pDoc->lspc.midofg == 0)?FALSE:TRUE;
			m_mico2fg = (pDoc->lspc.mico2fg == 0)?FALSE:TRUE;
			m_madofg = (pDoc->lspc.madofg == 0)?FALSE:TRUE;
			m_maco2fg = (pDoc->lspc.maco2fg == 0)?FALSE:TRUE;
		}

		CWnd *pWnd;

		pWnd = this->GetDlgItem(IDC_CHECK1);
		pWnd->EnableWindow(true);
		if(!m_midofg)
			pWnd->EnableWindow(false);
		pWnd = this->GetDlgItem(IDC_CHECK2);
		pWnd->EnableWindow(true);
		if(!m_mico2fg)
			pWnd->EnableWindow(false);
		pWnd = this->GetDlgItem(IDC_CHECK3);
		pWnd->EnableWindow(true);
		if(!m_madofg)
			pWnd->EnableWindow(false);
		pWnd = this->GetDlgItem(IDC_CHECK4);
		pWnd->EnableWindow(true);
		if(!m_maco2fg)
			pWnd->EnableWindow(false);
	}

	CPropertyPage::UpdateData(bSaveAndValidate);
	
	if(bSaveAndValidate)
	{
		if(pDoc != NULL)
		{
			pDoc->lspc.midofg = (!m_midofg)?0:1;
			pDoc->lspc.mico2fg = (!m_mico2fg)?0:1;
			pDoc->lspc.madofg = (!m_madofg)?0:1;
			pDoc->lspc.maco2fg = (!m_maco2fg)?0:1;
		}
	}
	return TRUE;
}
