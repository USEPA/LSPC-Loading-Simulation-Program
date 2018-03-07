/////////////////////////////////////////////////////////////////////////////
// MyPropertySheet1.h

#include "Page0.h"
#include "Page10.h"
#include "Page15.h"
#include "Page20.h"
#include "Page30.h"
#include "Page32.h"
#include "Page40.h"
#include "Page50.h"
#include "Page60.h"
#include "Page70.h"
#include "Page80.h"
#include "Page90.h"
#include "Page92.h"
#include "Page93.h"
#include "Page94.h"
#include "Page96.h"
#include "Page100.h"
#include "Page110.h"
#include "Page120.h"
#include "Page130.h"
#include "Page140.h"
#include "Page150.h"
#include "Page160.h"
#include "Page170.h"
#include "Page180.h"
#include "Page190.h"
#include "Page200.h"
#include "Page201.h"
#include "Page202.h"
#include "Page203.h"
#include "Page204.h"
#include "Page205.h"
#include "Page250.h"
#include "Page255.h"
#include "Page260.h"
#include "Page270.h"
#include "Page271.h"
#include "Page272.h"
#include "Page273.h"
#include "Page274.h"
#include "Page275.h"
#include "Page276.h"
#include "Page280.h"
#include "Page281.h"
#include "Page282.h"
#include "Page283.h"
#include "Page285.h"
#include "Page286.h"
#include "Page287.h"
#include "Page288.h"
#include "Page289.h"
#include "Page400.h"
#include "Page401.h"
#include "Page405.h"
#include "Page410.h"
#include "Page420.h"
#include "Page425.h"
#include "Page430.h"
#include "Page435.h"
#include "Page440.h"
#include "Page445.h"
#include "Page446.h"
#include "Page450.h"
#include "Page451.h"
#include "Page452.h"
#include "Page453.h"
#include "Page454.h"
#include "Page455.h"
#include "Page456.h"
#include "Page457.h"
#include "Page460.h"
#include "Page461.h"
#include "Page462.h"
#include "Page463.h"
#include "Page464.h"
#include "Page465.h"
#include "Page466.h"
#include "Page467.h"
#include "Page470.h"
#include "Page475.h"
#include "Page480.h"
#include "Page485.h"
#include "PageOutput.h"

#if !defined(AFX_MYPROPERTYSHEET_H__77DFDBE0_E941_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_MYPROPERTYSHEET_H__77DFDBE0_E941_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_RESIZEPAGE (WM_USER + 111)

struct XSleep_Structure
{
	int duration;
	HANDLE eventHandle;
};

DWORD WINAPI XSleepThread(LPVOID pWaitTime);
void XSleep(int nWaitInMSecs);

/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet1

class CMyPropertySheet1 : public CPropertySheet
{
	DECLARE_DYNAMIC(CMyPropertySheet1)

// Construction
public:
	CMyPropertySheet1(CWnd* pParentWnd);
	//CMyPropertySheet1(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	CPage0    page0;
	CPage10  *page10;
	CPage15  *page15;
	CPage20  *page20;
	CPage30   page30;
	CPage32   page32;
	CPage40   page40;
	CPage50   page50;
	CPage60  *page60;
	CPage70  *page70;
	CPage80  *page80;
	CPage90  *page90;
	CPage92  *page92;
	CPage93  *page93;
	CPage94  *page94;
	CPage96  *page96;
	CPage100  page100;
	CPage110 *page110;
	CPage120 *page120;
	CPage130 *page130;
	CPage140 *page140;
	CPage150 *page150;
	CPage160 *page160;
	CPage170 *page170;
	CPage180 *page180; 
	CPage190 *page190;  
	CPage200 *page200;  
	CPage201  page201;
	CPage202 *page202;
	CPage203 *page203;
	CPage204 *page204;
	CPage205 *page205;
	CPage250 *page250;
	CPage255  page255;
	CPage260 *page260;
	CPage270 *page270;
	CPage271 *page271;
	CPage272 *page272;
	CPage273 *page273;
	CPage274 *page274;
	CPage275 *page275;
	CPage276 *page276;
	CPage280  page280;		
	CPage281 *page281;		
	CPage282 *page282;		
	CPage283 *page283;		
	CPage285 *page285;		
	CPage286 *page286;		
	CPage287 *page287;		
	CPage288 *page288;		
	CPage289 *page289;		
	CPage400  page400;
	CPage401 *page401;
	CPage405 *page405;
	CPage410 *page410;
	CPage420  page420;
	CPage425 *page425;
	CPage430 *page430;
	CPage435 *page435;
	CPage440  page440;
	CPage445 *page445;
	CPage446 *page446;
	CPage450 *page450;
	CPage451 *page451;
	CPage452 *page452;
	CPage453 *page453;
	CPage454 *page454;
	CPage455 *page455;
	CPage456 *page456;
	CPage457 *page457;
	CPage460  page460;
	CPage461 *page461;
	CPage462 *page462;
	CPage463 *page463;
	CPage464 *page464;
	CPage465 *page465;
	CPage466 *page466;
	CPage467 *page467;
	CPage470 *page470;
	CPage475 *page475;
	CPage480 *page480;
	CPage485 *page485;
	CPageOutput pageoutput;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyPropertySheet1)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void EnablePageButtons(bool bEnable);
	virtual void Resize();
	virtual ~CMyPropertySheet1();
	virtual BOOL UpdateData( BOOL bSaveAndValidate = TRUE );


	// Generated message map functions
protected:
	//{{AFX_MSG(CMyPropertySheet1)
	//}}AFX_MSG
	virtual afx_msg LRESULT OnResizePage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPROPERTYSHEET_H__77DFDBE0_E941_11D3_B2F2_00104B9CFF37__INCLUDED_)
