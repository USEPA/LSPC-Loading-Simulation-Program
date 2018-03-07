// Page400.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "Page400.h"
#include "MainFrm.h"
#include "LSPCView.h"
#include "LSPCDoc.h"
#include "LSPCMODEL.h"
//#include "RecordsetEx.h"	 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage400 property page

IMPLEMENT_DYNCREATE(CPage400, CPropertyPage)

CPage400::CPage400() : CPropertyPage(CPage400::IDD)
{
	//{{AFX_DATA_INIT(CPage400)
	m_nadmod = 1;
	m_fkc = 0.0;
	m_nsedber = 0;
	m_nvconfg = 0;
	//}}AFX_DATA_INIT
}

CPage400::~CPage400()
{
}

void CPage400::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage400)
	DDX_Text(pDX, IDC_ADMOD, m_nadmod);
	DDV_MinMaxInt(pDX, m_nadmod, 0, 10000);
	DDX_Text(pDX, IDC_KC, m_fkc);
	DDV_MinMaxDouble(pDX, m_fkc, 0., 1.e+300);
	DDX_Text(pDX, IDC_sedber, m_nsedber);
	DDV_MinMaxInt(pDX, m_nsedber, 0, 10000);
	DDX_Text(pDX, IDC_vconfg, m_nvconfg);
	DDV_MinMaxInt(pDX, m_nvconfg, 0, 10000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage400, CPropertyPage)
	//{{AFX_MSG_MAP(CPage400)
	ON_EN_KILLFOCUS(IDC_ADMOD, OnChange)
	ON_EN_KILLFOCUS(IDC_KC, OnChange)
	ON_EN_KILLFOCUS(IDC_sedber, OnChange)
	ON_EN_KILLFOCUS(IDC_vconfg, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage400 message handlers
BOOL CPage400::UpdateData( BOOL bSaveAndValidate)
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
			m_nadmod  = pDoc->lspc.admod;
			m_fkc     = pDoc->lspc.kc;				 
			m_nsedber = pDoc->lspc.sedber;				 
			m_nvconfg = pDoc->lspc.vconfg;				 
		}

		CWnd *pWnd;

		pWnd = this->GetDlgItem(IDC_sedber);
		pWnd->EnableWindow(true);
		if(m_nsedber != 1)
			pWnd->EnableWindow(false);

		pWnd = this->GetDlgItem(IDC_vconfg);
		pWnd->EnableWindow(true);
		if(m_nvconfg != 1)
			pWnd->EnableWindow(false);
	}

	CPropertyPage::UpdateData(bSaveAndValidate);
	
	if(bSaveAndValidate)
	{
		if(pDoc != NULL)
		{
			pDoc->lspc.admod  = m_nadmod;
			pDoc->lspc.kc     = m_fkc;				 
			pDoc->lspc.sedber = m_nsedber;				 
			pDoc->lspc.vconfg = m_nvconfg;	
			
			if (pDoc->lspc.admod != 2)	
				pDoc->lspc.admod = 1;
			if (pDoc->lspc.sedber != 1)	
				pDoc->lspc.sedber = 0;
			if (pDoc->lspc.sedber == 1 && pDoc->lspc.sedfg == 0)	
			{
				CString strError;
				strError.Format("Sediment flag must be ON (sedfg=1) to simulate stream bank erosion (sedber=1)");
				AfxMessageBox(strError);
				TRACE(strError);
			}
		}

		// save back to database
/*		CLSPCApp *pApp = (CLSPCApp *) AfxGetApp();
		if(pApp == NULL)
			return false;

		if (pApp->strDBFileName.Right(3) == "mdb")
		{
			CString strSQL;
			CDatabase db;
			BOOL bCanOpen = FALSE;
			try
			{
				strSQL = CLSPCApp::GetMDBDriver();
				if(strSQL.IsEmpty())
				{
					AfxMessageBox("Cannot find the Microsoft Access ODBC driver.");
					return false;
				}
				bCanOpen = db.OpenEx(strSQL,CDatabase::noOdbcDialog);
			}
			catch(CException e)
			{
				AfxMessageBox("ODBC Error ---Cannot open database for reading.");
				return false;
			}
			if(!bCanOpen)
			{
				AfxMessageBox("ODBC Error ---Cannot open database for reading.");
				return false;
			}
			CRecordsetEx rs(&db);
			
			//card 400		Table "modelflags"
			strSQL.Format("UPDATE modelflags set VALUE1=%d WHERE PARMID=%d",
				pDoc->lspc.admod,40001);
			db.ExecuteSQL(LPCSTR(strSQL));
			strSQL.Format("UPDATE modelflags set VALUE1=%lf WHERE PARMID=%d",
				pDoc->lspc.kc,40002);
			db.ExecuteSQL(LPCSTR(strSQL));
			strSQL.Format("UPDATE modelflags set VALUE1=%d WHERE PARMID=%d",
				pDoc->lspc.sedber,40003);
			db.ExecuteSQL(LPCSTR(strSQL));
			strSQL.Format("UPDATE modelflags set VALUE1=%d WHERE PARMID=%d",
				pDoc->lspc.vconfg,40004);
			db.ExecuteSQL(LPCSTR(strSQL));
			rs.ExecDirect(strSQL);
			rs.Close();
			db.Close();
		}
*/
	}
	return TRUE;
}

void CPage400::OnChange() 
{
	CPropertyPage::UpdateData(true);
}
