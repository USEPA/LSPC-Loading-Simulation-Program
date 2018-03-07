// Page40.cpp : implementation file
//

#include "stdafx.h"
#include "LSPC.h"
#include "Page40.h"
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
// CPage40 property page

IMPLEMENT_DYNCREATE(CPage40, CPropertyPage)

CPage40::CPage40() : CPropertyPage(CPage40::IDD)
{
	//{{AFX_DATA_INIT(CPage40)
	m_nWaterShed = 1;
	m_nRGroup = 1;
	m_nChan = 1;
	m_nGroup = 1;
	m_nPLand = 1;
	//}}AFX_DATA_INIT
}

CPage40::~CPage40()
{
}

void CPage40::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage40)
	DDX_Text(pDX, IDC_EDIT1, m_nWaterShed);
	DDV_MinMaxInt(pDX, m_nWaterShed, 1, 100000);
	DDX_Text(pDX, IDC_EDIT2, m_nChan);
	DDV_MinMaxInt(pDX, m_nChan, 0, 10000);
	DDX_Text(pDX, IDC_EDIT3, m_nRGroup);
	DDV_MinMaxInt(pDX, m_nRGroup, 1, 10000);
	DDX_Text(pDX, IDC_EDIT4, m_nGroup);
	DDV_MinMaxInt(pDX, m_nGroup, 1, 10000);
	DDX_Text(pDX, IDC_EDIT5, m_nPLand);
	DDV_MinMaxInt(pDX, m_nPLand, 0, 10000);
	//}}AFX_DATA_MAP
	// now the combo box
}


BEGIN_MESSAGE_MAP(CPage40, CPropertyPage)
	//{{AFX_MSG_MAP(CPage40)
	ON_EN_KILLFOCUS(IDC_EDIT1, OnKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT2, OnKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT3, OnKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT4, OnKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT5, OnKillfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage40 message handlers

BOOL CPage40::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPage40::UpdateData( BOOL bSaveAndValidate)
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
			m_nWaterShed = pDoc->lspc.nsws;
			m_nChan = pDoc->lspc.nch;
			m_nRGroup = pDoc->lspc.nrgroup;		 
			m_nGroup = pDoc->lspc.ngroup;
			m_nPLand = pDoc->lspc.nlandp;
		}
	}

	CPropertyPage::UpdateData(bSaveAndValidate);
	
	if(bSaveAndValidate)
	{
		if(pDoc != NULL)
		{
			 pDoc->lspc.nsws = m_nWaterShed;
			 pDoc->lspc.nch = m_nChan;
			 pDoc->lspc.nrgroup = m_nRGroup;		 
			 pDoc->lspc.ngroup = m_nGroup;
			 pDoc->lspc.nlandp = m_nPLand;
		}
	}
	return TRUE;
}

void CPage40::OnKillfocus() 
{
	CPropertyPage::UpdateData(true);
}

