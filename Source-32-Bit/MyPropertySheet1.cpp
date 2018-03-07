// MyPropertySheet1.cpp : implementation file
//

#include "stdafx.h"
#include "LSPC.h"
#include "MyPropertySheet1.h"
#include "LeftView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////
// Function  : XSleepThread()
// Purpose   : The thread which will sleep for the given duration
// Returns   : DWORD WINAPI
// Parameters:       
//  1. pWaitTime -
//////////////////////////////////////////////////////////////////////
DWORD WINAPI XSleepThread(LPVOID pWaitTime)
{
	XSleep_Structure *sleep = (XSleep_Structure *)pWaitTime;

	Sleep(sleep->duration);
	SetEvent(sleep->eventHandle);

	return 0;
}

//////////////////////////////////////////////////////////////////////
// Function  : XSleep()
// Purpose   : To make the application sleep for the specified time
//			   duration.
//			   Duration the entire time duration XSleep sleeps, it
//			   keeps processing the message pump, to ensure that all
//			   messages are posted and that the calling thread does
//			   not appear to block all threads!
// Returns   : none
// Parameters:       
//  1. nWaitInMSecs - Duration to sleep specified in miliseconds.
//////////////////////////////////////////////////////////////////////
void XSleep(int nWaitInMSecs)
{
	XSleep_Structure sleep;
	
	sleep.duration		= nWaitInMSecs;
	sleep.eventHandle	= CreateEvent(NULL, TRUE, FALSE, NULL);

	DWORD threadId;
	CreateThread(NULL, 0, &XSleepThread, &sleep, 0, &threadId);

	MSG msg;
	
	while(::WaitForSingleObject(sleep.eventHandle, 0) == WAIT_TIMEOUT)
	{
		//get and dispatch messages
		if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	CloseHandle(sleep.eventHandle);
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet1

IMPLEMENT_DYNAMIC(CMyPropertySheet1, CPropertySheet)

CMyPropertySheet1::CMyPropertySheet1(CWnd* pParentWnd)
	:CPropertySheet(AFX_IDS_APP_TITLE,pParentWnd)
{
	EnableStackedTabs(FALSE);

	page10  = new CPage10(IDS_WFILEINFO);
	page15  = new CPage15(IDS_WSTATIONINFO);
	page20  = new CPage20(IDS_STATIONINFO);
	page60  = new CPage60(IDS_GROUPINFO);
	page70  = new CPage70(IDS_PEVIOUSLANDUSEINFO);
	page80  = new CPage80(IDS_STRING80);
	page90  = new CPage90(IDS_LANDUSEINFO);
	page92  = new CPage92(IDS_STRING92);
	page93  = new CPage93(IDS_STRING93);
	page94  = new CPage94(IDS_STRING94);
	page96  = new CPage96(IDS_STRING96);
	page110 = new CPage110(IDS_PWATPARAM2);
	page120 = new CPage120(IDS_PWATPARAM3);
	page130 = new CPage130(IDS_PWATPARAM4);
	page140 = new CPage140(IDS_PWATSTATE);
	page150 = new CPage150(IDS_PWATINTC);
	page160 = new CPage160(IDS_PMONUZN);
	page170 = new CPage170(IDS_PMONMANN);
	page180 = new CPage180(IDS_PMONINTFLW);
	page190 = new CPage190(IDS_PMONINTREC);
	page200 = new CPage200(IDS_PMONLZET);  
	page202 = new CPage202(IDS_STRING202) ;
	page203 = new CPage203(IDS_STRING203) ;
	page204 = new CPage204(IDS_STRING204) ;
	page205 = new CPage205(IDS_STRING205) ;
	page250 = new CPage250(IDS_PQUAL);
	page260 = new CPage260(IDS_PQUALINPUT);
	page270 = new CPage270(IDS_PQUALMACC);
	page271 = new CPage271(IDS_PQUALMONSQO);
	page272 = new CPage272(IDS_PQUALMONSUROCON);
	page273 = new CPage273(IDS_PQUALMONINTCON);
	page274 = new CPage274(IDS_PQUALMONGRD);	
	page275 = new CPage275(IDS_PQUALMONDRYDEP);	
	page276 = new CPage276(IDS_PQUALMONWETDEP);	
	page281 = new CPage281(IDS_STRING281);
	page282 = new CPage282(IDS_STRING282);
	page283 = new CPage283(IDS_STRING283);
	page285 = new CPage285(IDS_STRING285);
	page286 = new CPage286(IDS_STRING286);
	page287 = new CPage287(IDS_STRING287);
	page288 = new CPage288(IDS_STRING288);
	page289 = new CPage289(IDS_STRING289);
	page401 = new CPage401(IDS_STRING401);
	page405 = new CPage405(IDS_NETWORK);
	page410 = new CPage410(IDS_REACHINFO);
	page425 = new CPage425(IDS_PTSOURCE);
	page430 = new CPage430(IDS_PTWITHDRAWAL);
	page435 = new CPage435(IDS_STRING435);
	page445 = new CPage445(IDS_STRING445);
	page446 = new CPage446(IDS_STRING446);
	page450 = new CPage450(IDS_STRING450);
	page451 = new CPage451(IDS_STRING451);
	page452 = new CPage452(IDS_STRING452);
	page453 = new CPage453(IDS_STRING453);
	page454 = new CPage454(IDS_STRING454);
	page455 = new CPage455(IDS_STRING455);
	page456 = new CPage456(IDS_STRING456);
	page457 = new CPage457(IDS_STRING457);
	page461 = new CPage461(IDS_STRING461);
	page462 = new CPage462(IDS_STRING462);
	page463 = new CPage463(IDS_STRING463);
	page464 = new CPage464(IDS_STRING464);
	page465 = new CPage465(IDS_STRING465);
	page466 = new CPage466(IDS_STRING466);
	page467 = new CPage467(IDS_STRING467);
	page470 = new CPage470(IDS_STRING470);
	page475 = new CPage475(IDS_STRING475);
	page480 = new CPage480(IDS_STRING480);
	page485 = new CPage485(IDS_STRING485);

	// add pages 
	AddPage(&page0);
	AddPage(&page50);
	AddPage(&page40);
	AddPage(page70);
	AddPage(page80);
	AddPage(page90);
	AddPage(&page32);
	AddPage(&page30);
	AddPage(page10);
	AddPage(page15);
	AddPage(page20);
	AddPage(page60);
	AddPage(&page400);
	AddPage(page401);
	AddPage(page405);
	AddPage(page435);
	AddPage(page410);
	AddPage(&page420);
	AddPage(page425);
	AddPage(page430);
	AddPage(page92);
	AddPage(page93);
	AddPage(page94);
	AddPage(page96);
	AddPage(page110);
	AddPage(page120);
	AddPage(&page100);
	AddPage(page130);
	AddPage(page150);
	AddPage(page160);
	AddPage(page170);
	AddPage(page180);
	AddPage(page190);
	AddPage(page200);
	AddPage(page140);
	AddPage(&page201); 
	AddPage(page202);
	AddPage(page203);
	AddPage(page204);
	AddPage(page205);
	AddPage(&page440);
	AddPage(page445);
	AddPage(page446);
	AddPage(page450);
	AddPage(page451);
	AddPage(page452);
	AddPage(page453);
	AddPage(page454);
	AddPage(page455);
	AddPage(page456);
	AddPage(page457);
	AddPage(&page460);
	AddPage(page461);
	AddPage(page462);
	AddPage(page463);
	AddPage(page464);
	AddPage(page465);
	AddPage(page466);
	AddPage(page467);
	AddPage(page470);
	AddPage(page475);
	AddPage(page480);
	AddPage(page485);
	AddPage(page250);
	AddPage(&page255);
	AddPage(page260);	
	AddPage(page270);
	AddPage(page271);
	AddPage(page272);
	AddPage(page273);
	AddPage(page274);
	AddPage(page275);
	AddPage(page276);
	AddPage(&page280);
	AddPage(page281);
	AddPage(page282);
	AddPage(page283);
	AddPage(page285);
	AddPage(page286);
	AddPage(page287);
	AddPage(page288);
	AddPage(page289);
	AddPage(&pageoutput);
	//EnablePageButtons(bEnable);
}

CMyPropertySheet1::~CMyPropertySheet1()
{
	if (page10 != NULL)
		delete page10;
	if (page15 != NULL)
		delete page15;
	if (page20 != NULL)
		delete page20;
	if (page60 != NULL)
		delete page60;
	if (page70 != NULL)
		delete page70;
	if (page80 != NULL)
		delete page80;
	if (page90 != NULL)
		delete page90;
	if (page92 != NULL)
		delete page92;
	if (page93 != NULL)
		delete page93;
	if (page94 != NULL)
		delete page94;
	if (page96 != NULL)
		delete page96;
	if (page110 != NULL)
		delete page110;
	if (page120 != NULL)
		delete page120;
	if (page130 != NULL)
		delete page130;
	if (page140 != NULL)
		delete page140;
	if (page150 != NULL)
		delete page150;
	if (page160 != NULL)
		delete page160;
	if (page170 != NULL)
		delete page170;
	if (page180 != NULL)
		delete page180;
	if (page190 != NULL)
		delete page190;
	if (page200 != NULL)
		delete page200;
	if (page202 != NULL)
		delete page202;
	if (page203 != NULL)
		delete page203;
	if (page204 != NULL)
		delete page204;
	if (page205 != NULL)
		delete page205;
	if (page250 != NULL)
		delete page250;
	if (page260 != NULL)
		delete page260;
	if (page270 != NULL)
		delete page270;
	if (page271 != NULL)
		delete page271;
	if (page272 != NULL)
		delete page272;
	if (page273 != NULL)
		delete page273;
	if (page274 != NULL)
		delete page274;
	if (page275 != NULL)
		delete page275;
	if (page276 != NULL)
		delete page276;
	if (page281 != NULL)
		delete page281;
	if (page282 != NULL)
		delete page282;
	if (page283 != NULL)
		delete page283;
	if (page285 != NULL)
		delete page285;
	if (page286 != NULL)
		delete page286;
	if (page287 != NULL)
		delete page287;
	if (page288 != NULL)
		delete page288;
	if (page289 != NULL)
		delete page289;
	if (page401 != NULL)
		delete page401;
	if (page405 != NULL)
		delete page405;
	if (page410 != NULL)
		delete page410;
	if (page425 != NULL)
		delete page425;
	if (page430 != NULL)
		delete page430;
	if (page435 != NULL)
		delete page435;
	if (page445 != NULL)
		delete page445;
	if (page446 != NULL)
		delete page446;
	if (page450 != NULL)
		delete page450;
	if (page451 != NULL)
		delete page451;
	if (page452 != NULL)
		delete page452;
	if (page453 != NULL)
		delete page453;
	if (page454 != NULL)
		delete page454;
	if (page455 != NULL)
		delete page455;
	if (page456 != NULL)
		delete page456;
	if (page457 != NULL)
		delete page457;
	if (page461 != NULL)
		delete page461;
	if (page462 != NULL)
		delete page462;
	if (page463 != NULL)
		delete page463;
	if (page464 != NULL)
		delete page464;
	if (page465 != NULL)
		delete page465;
	if (page466 != NULL)
		delete page466;
	if (page467 != NULL)
		delete page467;
	if (page470 != NULL)
		delete page470;
	if (page475 != NULL)
		delete page475;
	if (page480 != NULL)
		delete page480;
	if (page485 != NULL)
		delete page485;
}


BEGIN_MESSAGE_MAP(CMyPropertySheet1, CPropertySheet)
	//{{AFX_MSG_MAP(CMyPropertySheet1)
	ON_WM_SIZE()
	ON_MESSAGE(WM_RESIZEPAGE, OnResizePage) 
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet1 message handlers

void CMyPropertySheet1::Resize()
{
	// Find parent
	CWnd* pParent;
	CRect rectParent;

	pParent = GetParent();
	if(pParent == NULL)
	{
		AfxMessageBox("Cannot resize property sheet. Sheet has no parent", MB_ICONEXCLAMATION | MB_OK | MB_APPLMODAL);
		return;
	}

	// Get parents client area
	pParent->GetClientRect(&rectParent);

	// Resize the sheet
	// First find relative change
	CSize sizeRelChange;
	CRect rectWindow;

	GetWindowRect(&rectWindow);
	ScreenToClient(&rectWindow);
	sizeRelChange.cx = rectWindow.Width() - rectParent.Width();
	sizeRelChange.cy = rectWindow.Height() - rectParent.Height();

	rectWindow.right -= sizeRelChange.cx;
	rectWindow.bottom -= sizeRelChange.cy;
	// Then resize the sheet
	MoveWindow(&rectWindow);

	// Resize the CTabCtrl
	CTabCtrl* pTab = GetTabControl();
	ASSERT(pTab);
	pTab->GetWindowRect(&rectWindow);
	ScreenToClient(&rectWindow);
	rectWindow.right -= sizeRelChange.cx;
	rectWindow.bottom -= sizeRelChange.cy;
	pTab->MoveWindow(&rectWindow);

	// Resize the active page
	CPropertyPage* pPage = GetActivePage();
	ASSERT(pPage);
	// Store page size in m_rectPage
	CRect m_rectPage;
	pPage->GetWindowRect(&m_rectPage);
	ScreenToClient(&m_rectPage);
	m_rectPage.right -= sizeRelChange.cx;
	m_rectPage.bottom -= sizeRelChange.cy;
	pPage->MoveWindow(&m_rectPage);
}

BOOL CMyPropertySheet1::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	Resize();	
	CTabCtrl* tab = GetTabControl();
	if(tab != NULL)
		tab->ShowWindow(SW_HIDE);

	return bResult;
}

LRESULT CMyPropertySheet1::OnResizePage(WPARAM wParam, LPARAM lParam)
{
	// Resize the page using m_rectPage, which was set in
	// Resize().
	CPropertyPage* pPage = GetActivePage();
	ASSERT(pPage);

	CRect rectParent;
	CWnd* pParent;

	pParent = GetParent();
	if(pParent == NULL)
	{
		AfxMessageBox("Cannot resize property sheet. Sheet has no parent", MB_ICONEXCLAMATION | MB_OK | MB_APPLMODAL);
		return 0;
	}

	// Get parents client area
	pParent->GetClientRect(&rectParent);

	CSize sizeRelChange;
	CRect rectWindow;

	GetWindowRect(&rectWindow);
	ScreenToClient(&rectWindow);
	sizeRelChange.cx = rectWindow.Width() - rectParent.Width();
	sizeRelChange.cy = rectWindow.Height() - rectParent.Height();

	CRect m_rectPage;
	pPage->GetWindowRect(&m_rectPage);
	ScreenToClient(&m_rectPage);
	m_rectPage.right -= sizeRelChange.cx;
	m_rectPage.bottom -= sizeRelChange.cy;

	pPage->MoveWindow(&m_rectPage);

	return 0;
}

BOOL CMyPropertySheet1::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR* pNMHDR = (LPNMHDR) lParam;
	
	// The sheet resizes the page whenever it is activated
	// so we need to resize it to what we want
	if (pNMHDR->code == TCN_SELCHANGE)
	{
		CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
		if(pMain != NULL)
		{
			CLeftView *pView = (CLeftView *)pMain->m_wndSplitter.GetPane(0,0);
			if(pView != NULL)
			{
				int nIndex = GetActiveIndex();
				pView->SelectItem(pMain->GetGeneralIndex(0, nIndex));
			}
		}
		SendMessage(WM_RESIZEPAGE);
	}
	
	return CPropertySheet::OnNotify(wParam, lParam, pResult);
}

BOOL CMyPropertySheet1::UpdateData( BOOL bSaveAndValidate)
{
	int i = 0;
	this->SetActivePage(i++);
	if(!page0.UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page50.UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page40.UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page70->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page80->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page90->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page32.UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page30.UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page10->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page15->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page20->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page60->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);	
	if(!page400.UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page401->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page405->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page435->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page410->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page420.UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page425->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page430->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page92->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page93->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page94->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page96->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page110->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page120->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page100.UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page130->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page150->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page160->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page170->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page180->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page190->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page200->UpdateData(bSaveAndValidate))
		return false;  
	this->SetActivePage(i++);
	if(!page140->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page201.UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page202->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page203->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page204->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page205->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);	
	if(!page440.UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page445->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page446->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page450->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page451->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page452->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page453->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page454->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page455->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page456->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page457->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page460.UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page461->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page462->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page463->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page464->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page465->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page466->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page467->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page470->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page475->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page480->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page485->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page250->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page255.UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page260->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page270->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page271->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page272->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page273->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page274->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page275->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page276->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page280.UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page281->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page282->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page283->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page285->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page286->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page287->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page288->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page289->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!pageoutput.UpdateData(bSaveAndValidate))
		return false;
	
	this->SetActivePage(0);
	//this->SendMessage(WM_UPDATE);
	//page0.SendMessage(WM_SHOWWINDOW);
	XSleep(1000);

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	if(pMain != NULL)
	{
		CLeftView *pView = (CLeftView *)pMain->m_wndSplitter.GetPane(0,0);
		if(pView != NULL)
		{
//			int nIndex = this->GetActiveIndex();
			pView->SelectItem(0);
		}
	}

	return true;
}

void CMyPropertySheet1::EnablePageButtons(bool bEnable)
{
}
