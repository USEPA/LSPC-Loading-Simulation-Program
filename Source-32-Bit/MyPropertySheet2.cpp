// MyPropertySheet2.cpp : implementation file
//

#include "stdafx.h"
#include "LSPC.h"
#include "MyPropertySheet1.h"
#include "MyPropertySheet2.h"
#include "LeftView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet2

IMPLEMENT_DYNAMIC(CMyPropertySheet2, CPropertySheet)

CMyPropertySheet2::CMyPropertySheet2(CWnd* pParentWnd)
	:CPropertySheet(AFX_IDS_APP_TITLE,pParentWnd)
{
	EnableStackedTabs(FALSE);

	page500 = new CPage500(IDS_STRING500);
	page501 = new CPage501(IDS_STRING501);
	page503 = new CPage503(IDS_STRING503);
	page504 = new CPage504(IDS_STRING504);
	page505 = new CPage505(IDS_STRING505);
	page506 = new CPage506(IDS_STRING506);
	page507 = new CPage507(IDS_STRING507);
	page511 = new CPage511(IDS_STRING511);
	page512 = new CPage512(IDS_STRING512);
	page513 = new CPage513(IDS_STRING513);
	page514 = new CPage514(IDS_STRING514);
	page521 = new CPage521(IDS_STRING521);
	page522 = new CPage522(IDS_STRING522);
	page523 = new CPage523(IDS_STRING523);
	page524 = new CPage524(IDS_STRING524);
	page531 = new CPage531(IDS_STRING531);
	page532 = new CPage532(IDS_STRING532);
	page533 = new CPage533(IDS_STRING533);
	page534 = new CPage534(IDS_STRING534);
	page535 = new CPage535(IDS_STRING535);
	page541 = new CPage541(IDS_STRING541);		
	page542 = new CPage542(IDS_STRING542);		
	page543 = new CPage543(IDS_STRING543);		
	page544 = new CPage544(IDS_STRING544);		
	page605 = new CPage605(IDS_STRING605);
	page610 = new CPage610(IDS_STRING610);
	page660 = new CPage660(IDS_STRING660);
	page670 = new CPage670(IDS_STRING670);
	page680 = new CPage680(IDS_STRING680);
	page685 = new CPage685(IDS_STRING685);
	page690 = new CPage690(IDS_STRING690);

	// add pages 
	AddPage(page500);
	AddPage(page501);
	AddPage(&page502);
	AddPage(page503);
	AddPage(page504);
	AddPage(page505);
	AddPage(page506);
	AddPage(page507);
	AddPage(&page510);
	AddPage(page511);
	AddPage(page512);
	AddPage(page513);
	AddPage(page514);
	AddPage(&page520);
	AddPage(page521);
	AddPage(page522);
	AddPage(page523);
	AddPage(page524);
	AddPage(&page530);
	AddPage(page531);
	AddPage(page532);
	AddPage(page533);
	AddPage(page534);
	AddPage(page535);
	AddPage(&page540);
	AddPage(page541);
	AddPage(page542);
	AddPage(page543);
	AddPage(page544);
	AddPage(&page600);
	AddPage(page605);
	AddPage(page610);
	AddPage(page660);
	AddPage(page670);
	AddPage(page680);
	AddPage(page685);
	AddPage(page690);
}

CMyPropertySheet2::~CMyPropertySheet2()
{
	if (page500 != NULL)
		delete page500;
	if (page501 != NULL)
		delete page501;
	if (page503 != NULL)
		delete page503;
	if (page504 != NULL)
		delete page504;
	if (page505 != NULL)
		delete page505;
	if (page506 != NULL)
		delete page506;
	if (page507 != NULL)
		delete page507;
	if (page511 != NULL)
		delete page511;
	if (page512 != NULL)
		delete page512;
	if (page513 != NULL)
		delete page513;
	if (page514 != NULL)
		delete page514;
	if (page521 != NULL)
		delete page521;
	if (page522 != NULL)
		delete page522;
	if (page523 != NULL)
		delete page523;
	if (page524 != NULL)
		delete page524;
	if (page531 != NULL)
		delete page531;
	if (page532 != NULL)
		delete page532;
	if (page533 != NULL)
		delete page533;
	if (page534 != NULL)
		delete page534;
	if (page535 != NULL)
		delete page535;
	if (page541 != NULL)
		delete page541;
	if (page542 != NULL)
		delete page542;
	if (page543 != NULL)
		delete page543;
	if (page544 != NULL)
		delete page544;
	if (page605 != NULL)
		delete page605;
	if (page610 != NULL)
		delete page610;
	if (page660 != NULL)
		delete page660;
	if (page670 != NULL)
		delete page670;
	if (page680 != NULL)
		delete page680;
	if (page685 != NULL)
		delete page685;
	if (page690 != NULL)
		delete page690;
}


BEGIN_MESSAGE_MAP(CMyPropertySheet2, CPropertySheet)
	//{{AFX_MSG_MAP(CMyPropertySheet2)
	ON_WM_SIZE()
	ON_MESSAGE(WM_RESIZEPAGE, OnResizePage) 
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet2 message handlers

void CMyPropertySheet2::Resize()
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

BOOL CMyPropertySheet2::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	Resize();	
	CTabCtrl* tab = GetTabControl();
	if(tab != NULL)
		tab->ShowWindow(SW_HIDE);

	return bResult;
}

LRESULT CMyPropertySheet2::OnResizePage(WPARAM wParam, LPARAM lParam)
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

BOOL CMyPropertySheet2::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
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
				int nIndex = this->GetActiveIndex();
				pView->SelectItem(pMain->GetGeneralIndex(1, nIndex));
			}
		}
		SendMessage(WM_RESIZEPAGE);
	}
	
	return CPropertySheet::OnNotify(wParam, lParam, pResult);
}

BOOL CMyPropertySheet2::UpdateData( BOOL bSaveAndValidate)
{
	int i = 0;
	this->SetActivePage(i++);
	if(!page500->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page501->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page502.UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page503->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page504->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page505->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page506->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page507->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page510.UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page511->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page512->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page513->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page514->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page520.UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page521->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page522->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page523->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page524->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page530.UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page531->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page532->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page533->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page534->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page535->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page540.UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page541->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page542->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page543->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page544->UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page600.UpdateData(bSaveAndValidate))
		return false;
	this->SetActivePage(i++);
	if(!page605->UpdateData(bSaveAndValidate))
		return false;	
	this->SetActivePage(i++);
	if(!page610->UpdateData(bSaveAndValidate))
		return false;	
	this->SetActivePage(i++);
	if(!page660->UpdateData(bSaveAndValidate))
		return false;	
	this->SetActivePage(i++);
	if(!page670->UpdateData(bSaveAndValidate))
		return false;	
	this->SetActivePage(i++);
	if(!page680->UpdateData(bSaveAndValidate))
		return false;	
	this->SetActivePage(i++);
	if(!page685->UpdateData(bSaveAndValidate))
		return false;	
	this->SetActivePage(i++);
	if(!page690->UpdateData(bSaveAndValidate))
		return false;	
	
//	this->SetActivePage(0);
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

void CMyPropertySheet2::EnablePageButtons(bool bEnable)
{
}
