// Page510.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page510.h"
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
// CPage510 property page

IMPLEMENT_DYNCREATE(CPage510, CPropertyPage)

CPage510::CPage510() : CPropertyPage(CPage510::IDD)
{
	//{{AFX_DATA_INIT(CPage510)
	m_bBenrfg = FALSE;
	m_nReamfg = 2;
	//}}AFX_DATA_INIT
}

CPage510::~CPage510()
{
}

void CPage510::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage510)
	DDX_Check(pDX, IDC_CHECK1, m_bBenrfg);
	DDX_Text(pDX, IDC_EDIT1, m_nReamfg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage510, CPropertyPage)
	//{{AFX_MSG_MAP(CPage510)
	ON_EN_CHANGE(IDC_EDIT1, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage510 message handlers
BOOL CPage510::UpdateData( BOOL bSaveAndValidate)
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
			m_bBenrfg = (pDoc->lspc.benrfg == 0)?FALSE:TRUE;
			m_nReamfg = pDoc->lspc.reamfg;
		}
	}

	CPropertyPage::UpdateData(bSaveAndValidate);
	
	if(bSaveAndValidate)
	{
		if(pDoc != NULL)
		{
			pDoc->lspc.benrfg = (!m_bBenrfg)?0:1;
			pDoc->lspc.reamfg = m_nReamfg;
		}
	}
	return TRUE;
}

void CPage510::OnChange() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	CPropertyPage::UpdateData(true);
}
