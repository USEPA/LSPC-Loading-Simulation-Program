// Page540.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page540.h"
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
// CPage540 dialog

IMPLEMENT_DYNCREATE(CPage540, CPropertyPage)

CPage540::CPage540() : CPropertyPage(CPage540::IDD)
{
	//{{AFX_DATA_INIT(CPage540)
	m_nphffg2 = 0;
	m_nphffg1 = 0;
	m_lfphfrc1 = 0.0;
	m_lfphfrc2 = 0.0;
	//}}AFX_DATA_INIT
}

CPage540::~CPage540()
{
}

void CPage540::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage540)
	DDX_Text(pDX, IDC_PHFFG1, m_nphffg1);
	DDV_MinMaxInt(pDX, m_nphffg1, 0, 1);
	DDX_Text(pDX, IDC_PHFFG2, m_nphffg2);
	DDV_MinMaxInt(pDX, m_nphffg2, 0, 1);
	DDX_Text(pDX, IDC_PHFRC1, m_lfphfrc1);
	DDV_MinMaxDouble(pDX, m_lfphfrc1, 0., 1.);
	DDX_Text(pDX, IDC_PHFRC2, m_lfphfrc2);
	DDV_MinMaxDouble(pDX, m_lfphfrc2, 0., 1.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage540, CPropertyPage)
	//{{AFX_MSG_MAP(CPage540)
	ON_EN_KILLFOCUS(IDC_PHFFG1, OnChange)
	ON_EN_KILLFOCUS(IDC_PHFFG2, OnChange)
	ON_EN_KILLFOCUS(IDC_PHFRC1, OnChange)
	ON_EN_KILLFOCUS(IDC_PHFRC2, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage540 message handlers
BOOL CPage540::UpdateData( BOOL bSaveAndValidate)
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
			m_nphffg1 = pDoc->lspc.phffg1;
			m_nphffg2 = pDoc->lspc.phffg2;
			m_lfphfrc1 = pDoc->lspc.phfrc1;
			m_lfphfrc2 = pDoc->lspc.phfrc2;			
		}
	}

	CPropertyPage::UpdateData(bSaveAndValidate);
	
	if(bSaveAndValidate)
	{
		if(pDoc != NULL)
		{
			pDoc->lspc.phffg1 = m_nphffg1;
			pDoc->lspc.phffg2 = m_nphffg2;
			pDoc->lspc.phfrc1 = m_lfphfrc1;
			pDoc->lspc.phfrc2 = m_lfphfrc2;		
		}
	}
	return TRUE;
}

void CPage540::OnChange() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CPropertyPage::UpdateData(true);
	
}
