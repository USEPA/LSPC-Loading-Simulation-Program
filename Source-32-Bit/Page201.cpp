// Page201.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page201.h"
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
// CPage201 property page

IMPLEMENT_DYNCREATE(CPage201, CPropertyPage)

CPage201::CPage201() : CPropertyPage(CPage201::IDD)
{
	//{{AFX_DATA_INIT(CPage201)
		m_bIrrig = FALSE ;
		m_bMonVary = FALSE ;
		m_bPET = FALSE;
	//}}AFX_DATA_INIT
}

CPage201::~CPage201()
{
}

void CPage201::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage201)
		DDX_Check(pDX, IDC_CHECK1, m_bIrrig);
		DDX_Check(pDX, IDC_CHECK2, m_bMonVary);
		DDX_Check(pDX, IDC_CHECK3, m_bPET);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage201, CPropertyPage)
	//{{AFX_MSG_MAP(CPage201)
		// NOTE: the ClassWizard will add message map macros here
		ON_BN_CLICKED(IDC_CHECK1, OnCheck)
		ON_BN_CLICKED(IDC_CHECK2, OnCheck)
		ON_BN_CLICKED(IDC_CHECK3, OnCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage201 message handlers
BOOL CPage201::UpdateData( BOOL bSaveAndValidate)
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
                m_bIrrig = (pDoc->lspc.irrigfg == 0)?FALSE:TRUE; 
				m_bMonVary = (pDoc->lspc.monVaryIrrig == 0)?FALSE:TRUE;
				m_bPET = (pDoc->lspc.petfg == 0)?FALSE:TRUE;
            }

			CWnd *pWnd;

			pWnd = this->GetDlgItem(IDC_CHECK1);
			pWnd->EnableWindow(true);
			if(!m_bIrrig)
				pWnd->EnableWindow(false);
			pWnd = this->GetDlgItem(IDC_CHECK2);
			pWnd->EnableWindow(true);
			if(!m_bMonVary)
				pWnd->EnableWindow(false);
			pWnd = this->GetDlgItem(IDC_CHECK3);
			pWnd->EnableWindow(true);
			if(!m_bPET)
				pWnd->EnableWindow(false);
        }

        CPropertyPage::UpdateData(bSaveAndValidate);
        
        if(bSaveAndValidate)
        {
			if(pDoc != NULL)
			{
				pDoc->lspc.irrigfg = m_bIrrig?1:0;
				pDoc->lspc.monVaryIrrig = m_bMonVary?1:0;
				pDoc->lspc.petfg = m_bPET?1:0;
			}
        }
        return TRUE;
}

LRESULT CPage201::OnWizardNext() 
{
        UpdateData(true);
        return CPropertyPage::OnWizardNext();
}

void CPage201::OnCancel() 
{
        CPropertyPage::OnCancel();

        CDialog *pDlg = (CDialog *)(GetParent()->GetParent());
        int nResult = 0;
        pDlg->EndDialog(nResult);
}

void CPage201::OnCheck() 
{
        CPropertyPage::UpdateData(true);
}
