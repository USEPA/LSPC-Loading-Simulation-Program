// Page0.cpp : implementation of the CPage0 class.
//

#include "stdafx.h"
#include "LSPC.h"
#include "Page0.h"
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
// CPage0 property page

IMPLEMENT_DYNCREATE(CPage0, CPropertyPage)

CPage0::CPage0() : CPropertyPage(CPage0::IDD)
{
	//{{AFX_DATA_INIT(CPage0)
	m_bPWater = FALSE;
	m_bSediment = FALSE;
	m_bQual = FALSE;
	m_bTemp = FALSE;
	m_bPH = FALSE;
	m_bSnow = FALSE;
	m_bOX = FALSE;
	m_bNUT = FALSE;
	m_bPLK = FALSE;
	//}}AFX_DATA_INIT
	
}

CPage0::~CPage0()
{
}

void CPage0::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage0)
	DDX_Check(pDX, IDC_CHECK1, m_bPWater);
	DDX_Check(pDX, IDC_CHECK2, m_bSediment);
	DDX_Check(pDX, IDC_CHECK3, m_bQual);
	DDX_Check(pDX, IDC_CHECK4, m_bTemp);
	DDX_Check(pDX, IDC_CHECK5, m_bPH);
	DDX_Check(pDX, IDC_CHECK9, m_bSnow);
	DDX_Check(pDX, IDC_CHECK10, m_bOX);
	DDX_Check(pDX, IDC_CHECK11, m_bNUT);
	DDX_Check(pDX, IDC_CHECK12, m_bPLK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage0, CPropertyPage)
	//{{AFX_MSG_MAP(CPage0)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck)
	ON_BN_CLICKED(IDC_CHECK9, OnCheck)
	ON_BN_CLICKED(IDC_CHECK10, OnCheck)
	ON_BN_CLICKED(IDC_CHECK11, OnCheck)
	ON_BN_CLICKED(IDC_CHECK12, OnCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage0 message handlers
BOOL CPage0::UpdateData( BOOL bSaveAndValidate)
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
			m_bSnow = (pDoc->lspc.snowfg == 0)?FALSE:TRUE;
			m_bPWater = (pDoc->lspc.pwatfg == 0)?FALSE:TRUE;
			m_bSediment = (pDoc->lspc.sedfg == 0)?FALSE:TRUE;
			m_bQual = (pDoc->lspc.pqalfg == 0)?FALSE:TRUE;
			m_bTemp = (pDoc->lspc.tempfg == 0)?FALSE:TRUE;
			m_bOX = (pDoc->lspc.oxfg == 0)?FALSE:TRUE;
			m_bNUT = (pDoc->lspc.nutfg == 0)?FALSE:TRUE;
			m_bPLK = (pDoc->lspc.plkfg == 0)?FALSE:TRUE;
			m_bPH = (pDoc->lspc.phfg == 0)?FALSE:TRUE;
			if(pDoc->lspc.nContorlGlobal == 0)
			{
				CWnd *pWnd;

				pWnd = this->GetDlgItem(IDC_CHECK1);
				pWnd->EnableWindow(true);
				if(!m_bPWater)
					pWnd->EnableWindow(false);
				pWnd = this->GetDlgItem(IDC_CHECK2);
				pWnd->EnableWindow(true);
				if(!m_bSediment)
					pWnd->EnableWindow(false);
				pWnd = this->GetDlgItem(IDC_CHECK3);
				pWnd->EnableWindow(true);
				if(!m_bQual)
					pWnd->EnableWindow(false);
				pWnd = this->GetDlgItem(IDC_CHECK4);
				pWnd->EnableWindow(true);
				if(!m_bTemp)
					pWnd->EnableWindow(false);
				pWnd = this->GetDlgItem(IDC_CHECK5);
				pWnd->EnableWindow(true);
				if(!m_bPH)
					pWnd->EnableWindow(false);
				pWnd = this->GetDlgItem(IDC_CHECK9);
				pWnd->EnableWindow(true);
				if(!m_bSnow)
					pWnd->EnableWindow(false);
				pWnd = this->GetDlgItem(IDC_CHECK10);
				pWnd->EnableWindow(true);
				if(!m_bOX)
					pWnd->EnableWindow(false);
				pWnd = this->GetDlgItem(IDC_CHECK11);
				pWnd->EnableWindow(true);
				if(!m_bNUT)
					pWnd->EnableWindow(false);
				pWnd = this->GetDlgItem(IDC_CHECK12);
				pWnd->EnableWindow(true);
				if(!m_bPLK)
					pWnd->EnableWindow(false);

				++pDoc->lspc.nContorlGlobal;
			}
		}
	}

	CPropertyPage::UpdateData(bSaveAndValidate);
	
	if(bSaveAndValidate)
	{
		if(pDoc != NULL)
		{
			pDoc->lspc.snowfg = m_bSnow?1:0;
			pDoc->lspc.pwatfg = m_bPWater?1:0;
			pDoc->lspc.sedfg = m_bSediment?1:0;
			pDoc->lspc.pqalfg = m_bQual?1:0;
			pDoc->lspc.tempfg = m_bTemp?1:0;
			pDoc->lspc.oxfg = m_bOX?1:0;
			pDoc->lspc.nutfg = m_bNUT?1:0;
			pDoc->lspc.plkfg = m_bPLK?1:0;
			pDoc->lspc.phfg = m_bPH?1:0;
		}
	}
	return TRUE;
}

LRESULT CPage0::OnWizardNext() 
{
	UpdateData(true);
	return CPropertyPage::OnWizardNext();
}

void CPage0::OnCancel() 
{
	CPropertyPage::OnCancel();

	CDialog *pDlg = (CDialog *)(GetParent()->GetParent());
	int nResult = 0;
	pDlg->EndDialog(nResult);
}

void CPage0::OnCheck() 
{
	CPropertyPage::UpdateData(true);

	if(m_bPH == 1)
	{
		m_bPWater = 1;
		m_bQual = 1;
		m_bTemp = 1;
		m_bOX = 1;
		m_bNUT = 1;
		m_bPLK = 1;
	}
	else if(m_bPLK == 1)
	{
		m_bPWater = 1;
		m_bQual = 1;
		m_bTemp = 1;
		m_bOX = 1;
		m_bNUT = 1;
	}
	else if(m_bNUT == 1)
	{
		m_bPWater = 1;
		m_bQual = 1;
		m_bTemp = 1;
		m_bOX = 1;
	}
	else if(m_bOX == 1)
	{
		m_bPWater = 1;
		m_bQual = 1;
		m_bTemp = 1;
	}
	else if(m_bSnow == 1)
	{
		m_bPWater = 1;
		m_bTemp = 1;
	}
	else
		m_bPWater = 1;


	CPropertyPage::UpdateData(false);
}

