// Page600.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page600.h"
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
// CPage600 property page

IMPLEMENT_DYNCREATE(CPage600, CPropertyPage)

CPage600::CPage600() : CPropertyPage(CPage600::IDD)
{
	//{{AFX_DATA_INIT(CPage600)
	m_ncpt = 0;
	m_ncland = 0;
	m_ncrch = 0;
	m_ntrgp = 0;
	m_ntnum = 0;
	//}}AFX_DATA_INIT
}

CPage600::~CPage600()
{
}

void CPage600::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage600)
	DDX_Text(pDX, IDC_EDIT1, m_ncpt);
	DDV_MinMaxInt(pDX, m_ncpt, 0, 100000);
	DDX_Text(pDX, IDC_EDIT2, m_ncland);
	DDV_MinMaxInt(pDX, m_ncland, 0, 100000);
	DDX_Text(pDX, IDC_EDIT3, m_ncrch);
	DDV_MinMaxInt(pDX, m_ncrch, 0, 100000);
	DDX_Text(pDX, IDC_EDIT4, m_ntrgp);
	DDV_MinMaxInt(pDX, m_ntrgp, 0, 100000);
	DDX_Text(pDX, IDC_EDIT5, m_ntnum);
	DDV_MinMaxInt(pDX, m_ntnum, 0, 100000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage600, CPropertyPage)
	//{{AFX_MSG_MAP(CPage600)
	ON_EN_KILLFOCUS(IDC_EDIT1, OnChange)
	ON_EN_KILLFOCUS(IDC_EDIT2, OnChange)
	ON_EN_KILLFOCUS(IDC_EDIT3, OnChange)
	ON_EN_KILLFOCUS(IDC_EDIT4, OnChange)
	ON_EN_KILLFOCUS(IDC_EDIT5, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage600 message handlers
BOOL CPage600::UpdateData( BOOL bSaveAndValidate)
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
			m_ncpt		 = pDoc->lspc.ncpt;
			m_ncland	 = pDoc->lspc.ncland;
			m_ncrch		 = pDoc->lspc.ncrch;
			m_ntrgp 	 = pDoc->lspc.ntrgp;
			m_ntnum		 = pDoc->lspc.ntnum;
		}
	}

	CPropertyPage::UpdateData(bSaveAndValidate);
	
	if(bSaveAndValidate)
	{
		if(pDoc != NULL)
		{
			pDoc->lspc.ncpt		  = m_ncpt;
			pDoc->lspc.ncland	  = m_ncland;
			pDoc->lspc.ncrch      = m_ncrch;
			pDoc->lspc.ntrgp	  = m_ntrgp;
			pDoc->lspc.ntnum      = m_ntnum;
		}
	}
	return TRUE;
}

void CPage600::OnChange() 
{
	CPropertyPage::UpdateData(true);
}
