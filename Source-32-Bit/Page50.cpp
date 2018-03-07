// Page50.cpp : implementation file
//

#include "stdafx.h"
#include "LSPC.h"
#include "Page50.h"
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
// CPage50 dialog
IMPLEMENT_DYNCREATE(CPage50, CPropertyPage)

CPage50::CPage50()
	: CPropertyPage(CPage50::IDD)
{
	//{{AFX_DATA_INIT(CPage50)
	m_nMinutes = 60;										
	tStart = COleDateTime(1997,1,1,0,0,0);
	tEnd = COleDateTime(1999,1,1,0,0,0);
	toStart = COleDateTime(1997,1,1,0,0,0);
	toEnd = COleDateTime(1999,1,1,0,0,0);
	//}}AFX_DATA_INIT
}

CPage50::~CPage50()
{
}


void CPage50::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage50)
	DDX_Control(pDX, IDC_COMBOOUTPUT, m_oCombo);
	DDX_Text(pDX, IDC_EDIT1, m_nMinutes);					
	DDV_MinMaxInt(pDX, m_nMinutes, 1, 43200);
	//}}AFX_DATA_MAP
	// now the datetime 
	if(pDX->m_bSaveAndValidate)
	{
			m_cStart.GetTime(tStart);
			m_cEnd.GetTime(tEnd);

			m_oStart.GetTime(toStart);		
			m_oEnd.GetTime(toEnd);
	}
	else
	{
		if(m_cStart.GetSafeHwnd() != NULL && m_cEnd.GetSafeHwnd() != NULL)
		{
			m_cStart.SetTime(tStart);
			m_cEnd.SetTime(tEnd);
		}
		if(m_oStart.GetSafeHwnd() != NULL && m_oEnd.GetSafeHwnd() != NULL)
		{
			m_oStart.SetTime(toStart);	
			m_oEnd.SetTime(toEnd);
		}
	}

}


BEGIN_MESSAGE_MAP(CPage50, CPropertyPage)
	//{{AFX_MSG_MAP(CPage50)
	ON_EN_KILLFOCUS(IDC_EDIT1, OnKillfocusEdit1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage50 message handlers

BOOL CPage50::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CRect rc;
	CWnd *pWnd = GetDlgItem(IDC_DT1);
	if(!pWnd) 
		return TRUE;
	pWnd->GetWindowRect(rc);
	pWnd->DestroyWindow(); //pWnd was just a placeholder;
	ScreenToClient(rc);
	m_cStart.Create(WS_CHILD|WS_VISIBLE|DTS_SHORTDATEFORMAT,rc,this,IDC_DT1);
	m_cStart.SetMonthCalColor(MCSC_BACKGROUND,RGB(192,192,192));
	m_cStart.SetMonthCalColor(MCSC_MONTHBK,RGB(192,192,192));
	m_cStart.SetMonthCalColor(MCSC_TEXT ,RGB(0,0,0));
 
	pWnd = GetDlgItem(IDC_DT2);
	if(!pWnd) 
		return TRUE;
	pWnd->GetWindowRect(rc);
	pWnd->DestroyWindow(); //pWnd was just a placeholder;
	ScreenToClient(rc);
	m_cEnd.Create(WS_CHILD|WS_VISIBLE|DTS_SHORTDATEFORMAT,rc,this,IDC_DT2);
	m_cEnd.SetMonthCalColor(MCSC_BACKGROUND,RGB(192,192,192));
	m_cEnd.SetMonthCalColor(MCSC_MONTHBK,RGB(192,192,192));
	m_cEnd.SetMonthCalColor(MCSC_TEXT ,RGB(0,0,0));
	m_cStart.SetTime(tStart);
	m_cEnd.SetTime(tEnd);

	pWnd = GetDlgItem(IDC_DT3);			
	if(!pWnd) 
		return TRUE;
	pWnd->GetWindowRect(rc);
	pWnd->DestroyWindow(); //pWnd was just a placeholder;
	ScreenToClient(rc);
	m_oStart.Create(WS_CHILD|WS_VISIBLE|DTS_SHORTDATEFORMAT,rc,this,IDC_DT3);
	m_oStart.SetMonthCalColor(MCSC_BACKGROUND,RGB(192,192,192));
	m_oStart.SetMonthCalColor(MCSC_MONTHBK,RGB(192,192,192));
	m_oStart.SetMonthCalColor(MCSC_TEXT ,RGB(0,0,0));

	pWnd = GetDlgItem(IDC_DT4);		
	if(!pWnd) 
		return TRUE;
	pWnd->GetWindowRect(rc);
	pWnd->DestroyWindow(); //pWnd was just a placeholder;
	ScreenToClient(rc);
	m_oEnd.Create(WS_CHILD|WS_VISIBLE|DTS_SHORTDATEFORMAT,rc,this,IDC_DT4);
	m_oEnd.SetMonthCalColor(MCSC_BACKGROUND,RGB(192,192,192));
	m_oEnd.SetMonthCalColor(MCSC_MONTHBK,RGB(192,192,192));
	m_oEnd.SetMonthCalColor(MCSC_TEXT ,RGB(0,0,0));
	m_oStart.SetTime(toStart);
	m_oEnd.SetTime(toEnd);

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();		
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return true;
	CLSPCDoc *pDoc = NULL;
	if(pView != NULL)
	{
		pDoc = pView->GetDocument();
	}
	m_oCombo.SetCurSel(pDoc->lspc.optlevel-1);		
	m_cStart.SetRange(&pDoc->lspc.twStart,&pDoc->lspc.twEnd);
	m_cEnd.SetRange(&pDoc->lspc.twStart,&pDoc->lspc.twEnd);
	m_oStart.SetRange(&pDoc->lspc.twStart,&pDoc->lspc.twEnd);
	m_oEnd.SetRange(&pDoc->lspc.twStart,&pDoc->lspc.twEnd);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPage50::UpdateData( BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return true;
	CLSPCDoc *pDoc = NULL;
	if(pView != NULL)
		pDoc = pView->GetDocument();

	if(!bSaveAndValidate)
	{
		if(pDoc != NULL)
		{
			m_oCombo.SetCurSel(pDoc->lspc.optlevel-1);		
			m_nMinutes = pDoc->lspc.deltm;									        
			tStart = pDoc->lspc.tStart;
			tEnd = pDoc->lspc.tEnd;
			toStart = pDoc->lspc.toStart;			
			toEnd = pDoc->lspc.toEnd;
			m_cStart.SetRange(&pDoc->lspc.twStart,&pDoc->lspc.twEnd);
			m_cEnd.SetRange(&pDoc->lspc.twStart,&pDoc->lspc.twEnd);
			m_oStart.SetRange(&pDoc->lspc.twStart,&pDoc->lspc.twEnd);
			m_oEnd.SetRange(&pDoc->lspc.twStart,&pDoc->lspc.twEnd);
		}
	}

	CPropertyPage::UpdateData(bSaveAndValidate);
	
	if(bSaveAndValidate)
	{
		if(pDoc != NULL)
		{
			CString str = "";
			CString str1 = "";
			pDoc->lspc.optlevel = m_oCombo.GetCurSel()+1;
			pDoc->lspc.deltm = m_nMinutes;										         
			pDoc->lspc.tStart = tStart;
			if (toStart >= tStart)
				pDoc->lspc.toStart = toStart;			
			else
			{
				pDoc->lspc.toStart = tStart;			
				m_oStart.SetTime(tStart);
				str1.Format("Start time for output is smaller than simulation start time\n");
				str += str1;
			}

			// add another day
			if(tEnd.GetHour() == 0)
			{
				COleDateTimeSpan tspan = COleDateTimeSpan(0,23,59,59);     
				tEnd += tspan;
			}
			pDoc->lspc.tEnd = tEnd;
			if(tEnd < tStart)
			{
				AfxMessageBox("Simulation end time should be later than start time");
				return false;
			}
			// add another day					
			if(toEnd.GetHour() == 0)
			{
				COleDateTimeSpan tspan = COleDateTimeSpan(0,23,59,59);     
				toEnd += tspan;
			}
			if (toEnd <= tEnd)
			{
				pDoc->lspc.toEnd = toEnd;
				pDoc->lspc.tEnd = toEnd;		
				m_cEnd.SetTime(toEnd);
			}
			else
			{
				pDoc->lspc.toEnd = tEnd;
				m_oEnd.SetTime(tEnd);
				str1.Format("End time for output is greater than simulation end time\n");
				str += str1;
			}
			if(toEnd < toStart)
			{
				AfxMessageBox("Output end time should be later than start time");
				return false;
			}
			if (str != "")
			{
				str1.Format("Do you want output time to be adjusted to simulation time?");
				str +=str1;
				int response = AfxMessageBox(str,MB_YESNO,MB_ICONWARNING);
				if (response != IDYES)
					return false;
			}

			pDoc->lspc.delts = pDoc->lspc.deltm * 60.;	// sec	
			pDoc->lspc.delt60 = pDoc->lspc.deltm / 60.;	// hour
			pDoc->lspc.deltd = pDoc->lspc.deltm / 1440.;// day	
		}
	}
	return TRUE;
}

void CPage50::OnKillfocusEdit1() 
{
	CPropertyPage::UpdateData(true);
}

