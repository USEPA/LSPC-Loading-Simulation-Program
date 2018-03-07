// Page440.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page440.h"
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
// CPage440 dialog

IMPLEMENT_DYNCREATE(CPage440, CPropertyPage)

CPage440::CPage440() : CPropertyPage(CPage440::IDD)
{
	//{{AFX_DATA_INIT(CPage440)
	m_ncrvfg = 0;
	m_nvsivfg = 0;
	m_nsandfg = 0;
	m_nsweepfg = 0;
	//}}AFX_DATA_INIT
}

CPage440::~CPage440()
{
}

void CPage440::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CPage440)
	DDX_Text(pDX, IDC_1, m_ncrvfg);
	DDV_MinMaxInt(pDX, m_ncrvfg, 0, 100000);
	DDX_Text(pDX, IDC_2, m_nvsivfg);
	DDV_MinMaxInt(pDX, m_nvsivfg, 0, 100000);
	DDX_Text(pDX, IDC_3, m_nsandfg);
	DDV_MinMaxInt(pDX, m_nsandfg, 0, 100000);
	DDX_Text(pDX, IDC_4, m_nsweepfg);
	DDV_MinMaxInt(pDX, m_nsweepfg, 0, 100000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage440, CPropertyPage)
	//{{AFX_MSG_MAP(CPage440)
	ON_EN_CHANGE(IDC_1, OnChange)
	ON_EN_CHANGE(IDC_2, OnChange)
	ON_EN_CHANGE(IDC_3, OnChange)
	ON_EN_CHANGE(IDC_4, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage440 message handlers
BOOL CPage440::UpdateData( BOOL bSaveAndValidate)
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
			m_ncrvfg = pDoc->lspc.crvfg;
			m_nvsivfg = pDoc->lspc.vsivfg;				 
			m_nsandfg = pDoc->lspc.sandfg;				 
			m_nsweepfg = pDoc->lspc.sweepfg;				 
		}

		CWnd *pWnd;

		pWnd = this->GetDlgItem(IDC_1);
		pWnd->EnableWindow(true);
		if(m_ncrvfg != 1)
			pWnd->EnableWindow(false);

		pWnd = this->GetDlgItem(IDC_2);
		pWnd->EnableWindow(true);
		if(m_nvsivfg != 1)
			pWnd->EnableWindow(false);

		pWnd = this->GetDlgItem(IDC_3);
		pWnd->EnableWindow(true);
		if(pDoc->lspc.sedfg != 1)
			pWnd->EnableWindow(false);

		pWnd = this->GetDlgItem(IDC_4);
		pWnd->EnableWindow(true);
//		if(pDoc->lspc.sweepfg != 1)
//			pWnd->EnableWindow(false);
	}

	CPropertyPage::UpdateData(bSaveAndValidate);
	
	if(bSaveAndValidate)
	{
		if(pDoc != NULL)
		{
			pDoc->lspc.crvfg = m_ncrvfg;
			pDoc->lspc.vsivfg = m_nvsivfg;				 
			if (m_nsandfg > 3)	
				m_nsandfg = 0;	
			pDoc->lspc.sandfg = m_nsandfg;	
			pDoc->lspc.sweepfg = m_nsweepfg;	
		}
	}
	return TRUE;
}

void CPage440::OnChange() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	CPropertyPage::UpdateData(true);
	
}
