// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "LSPC.h"
#include "MainFrm.h"
#include "LeftView.h"
#include "LSPCDoc.h"
#include "LSPCView.h"
#include "DlgWQ.h"
#include "CommentsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int nPageIndex[] =
{
	0,
	50,
	40,
	70,
	80,
	90,
	32,
	30,
	10,
	15,
	20,
	60,
	400,
	401,
	405,
	435,
	410,
	420,
	425,
	430,
	92,	
	93,	
	94,	
	96,	
	110,
	120,
	100,
	130,
	150,
	160,
	170,
	180,
	190,
	200,
	140,
	201,
	202,
	203,
	204,
	205,
	440,
	445,
	446,
	450,
	451,
	452,
	453,
	454,
	455,
	456,
	457,
	460,
	461,
	462,
	463,
	464,
	465,
	466,
	467,
	470,
	475,
	480,
	485,
	250,
	255,
	260,	
	270,
	271,	
	272,
	273,
	274,
	275,
	276,
	280,
	281,
	282,
	283,
	285,
	286,
	287,
	288,
	289,
	500,
	501,
	502,	
	503,	
	504,
	505,	
	506,	
	507,	
	510,	
	511,
	512,
	513,
	514,
	520,	
	521,
	522,
	523,
	524,
	530,	
	531,
	532,
	533,
	534,
	535,
	540,
	541,
	542,
	543,
	544,
	600,
	605,
	610,
	660,
	670,
	680,
	685,
	690,
	45
};


CRITICAL_SECTION myLock;


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_RUN, OnFileRun)
	ON_COMMAND(ID_FILE_GETFROMDATABASE, OnFileGetfromdatabase)
	ON_COMMAND(ID_FILE_LOADWEATHERDATA, OnFileLoadweatherdata)
	ON_COMMAND(ID_FILE_SAVEDEFAULTPARAMETERSTODATABASE, OnFileSavedefaultparameterstodatabase)
	ON_COMMAND(ID_FILE_LOADRELOADPOINTSOURCEDATAFROMATEXTFILE, OnLoadPointSourceFile)
	ON_COMMAND(ID_FILE_LOADRELOADPOINTSOURCEDATAFROMDATABASE, OnDBLoadPointSource)
	ON_COMMAND(ID_FILE_SAVEPOINTSOURCEDATA, OnFileSavePointSourceData)
	ON_COMMAND(ID_VIEW_COMMENTS, OnViewComments)
	ON_COMMAND(ID_VIEW_PARAMETERINFO, OnViewParameterinfo)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_COMMAND(ID_P_INFO, OnViewParameterinfo)
	ON_COMMAND(ID_USER_COMMENTS, OnViewComments)
	ON_COMMAND(ID_FILE_LOADRELOADLANDUSECHANGEDATAFROMATEXTFILE, OnFileLoadLanduseChangeTextfile)
	ON_COMMAND(ID_FILE_LOADRELOADLANDUSECHANGEDATAFROMDATABASE, OnFileLoadLanduseChangeDatabase)
	ON_COMMAND(ID_FILE_SAVELANDUSECHANGEDATA, OnFileSaveLanduseChangeData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_ELAPSED,
	ID_REMAIN,
	ID_PROGRESS
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	bsaveflag = false;
	bRunModel = false;
	bRunNIMS = false;
	InitializeCriticalSection(&myLock);
}

CMainFrame::~CMainFrame()
{
	DeleteCriticalSection(&myLock);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
/*
	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}
*/
	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar)/* ||
		!m_wndReBar.AddBar(&m_wndDlgBar)*/)
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)) )
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	//Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
	
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	m_wndStatusBar.SetPaneText(1,"");
	m_wndStatusBar.SetPaneText(2,"");
	m_wndStatusBar.SetPaneText(3,"");

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftView), CSize(240, 100), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CLSPCView), CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE;
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnFileNew() 
{
	/*
	// the wizard dialog
	CMyWizardDlg dlg(this);
	//dlg.SetWizardMode();
	dlg.DoModal();
	*/
}

void CMainFrame::OnFileOpen() 
{
	// open the input file
	CFileDialog dlg(TRUE,"*.inp",NULL,NULL,"Model Input Files (*.inp)|*.inp|All Files (*.*)|*.*||");
	
	if(dlg.DoModal() == IDOK)
	{
		// get file name
		CString strFilePath = dlg.GetPathName();
		CString strFileName = dlg.GetFileName();
		int nIndex = strFilePath.Find(LPCSTR(strFileName));
		if(nIndex != -1)
		{
			strPath = strFilePath.Left(nIndex);
		}
		EnterCriticalSection(&myLock);
		OpenInputFile(strFilePath);
		LeaveCriticalSection(&myLock);
		SetWindowText("LSPC Model - "+strFileName);		
	}	
}

void CMainFrame::OpenInputFile(CString strFileName)
{
	CLSPCView *pView = (CLSPCView *)m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;
	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;
	CLSPCApp *pApp = (CLSPCApp *) AfxGetApp();
	if(pApp == NULL)
		return;
	pDoc->lspc.nContorlGlobal = 0;
	pDoc->lspc.nWaterControl = 0;
	pDoc->lspc.m_strFileName = strFileName;
	pDoc->lspc.m_strPath = strPath;
	pDoc->lspc.strPointFilePath = "";
	pDoc->lspc.strPointFileName = "";
	pDoc->lspc.m_strPSDataLocation = "";
	pDoc->lspc.cLU.strLUchangeFilePath = "";
	pDoc->lspc.cLU.nLUchangeFlag = 0;
	pDoc->lspc.cLU.nNumLUchangeRecords = 0;

	// 12-18-05
	pDoc->lspc.ResetData();
	pDoc->lspc.cLU.ResetData();
	pDoc->lspc.cOP.ResetData();
	pDoc->lspc.cWD.ResetData();

	//read input file
	if(!pDoc->lspc.ReadInfo(strFileName))
		return;

	pDoc->SetPathName(strFileName);
//	pDoc->lspc.AllocateTSSpace();
	BeginWaitCursor();

	pDoc->lspc.bWeather = true;
	pDoc->lspc.deltm = 60;

	if (!pDoc->lspc.cWD.GetWeatherDateTimeandInterval(pDoc->lspc.deltm,pDoc->lspc.twStart,pDoc->lspc.twEnd))
		return;

	pDoc->lspc.delts = pDoc->lspc.deltm * 60.;	// sec	
	pDoc->lspc.delt60 = pDoc->lspc.deltm / 60.;	// hour
	pDoc->lspc.deltd = pDoc->lspc.deltm / 1440.;// day	

	if (pDoc->lspc.tStart < pDoc->lspc.twStart)	pDoc->lspc.tStart = pDoc->lspc.twStart;
	if (pDoc->lspc.tEnd > pDoc->lspc.twEnd)	pDoc->lspc.tEnd = pDoc->lspc.twEnd;

	if (pDoc->lspc.toStart < pDoc->lspc.tStart)	pDoc->lspc.toStart = pDoc->lspc.tStart;
	if (pDoc->lspc.toEnd > pDoc->lspc.tEnd)	pDoc->lspc.toEnd = pDoc->lspc.tEnd;

	if (bRunNIMS == false)
	{
		for(int i=0; i<pDoc->lspc.cWD.nNumWeatherFiles; i++) 
		{
			CString strFileName = pDoc->lspc.cWD.strWeatherFolderPath+pDoc->lspc.cWD.pWFILEINFO[i].strWFile;
			m_wndStatusBar.SetPaneText(0,"Reading File "+strFileName);
			
			int nIndex = strFileName.ReverseFind('.');
			CString strFileExtension = strFileName.Mid(nIndex, 4);
			if (strFileExtension.CompareNoCase(".air") == 0) 
			{
				if (!pDoc->lspc.cWD.ReadAirFile(pDoc->lspc.deltm,i,strFileName,pDoc->lspc.twStart,pDoc->lspc.twEnd))
				{
					pDoc->lspc.bWeather = false;
					m_wndStatusBar.SetPaneText(0,"Ready");
					break;
				}
			}
			else
			{
				if (!pDoc->lspc.cWD.ReadPreFile(pDoc->lspc.deltm,i,strFileName,pDoc->lspc.twStart,pDoc->lspc.twEnd))
				{
					pDoc->lspc.bWeather = false;
					m_wndStatusBar.SetPaneText(0,"Ready");
					break;
				}
			}
		}
	}

	// read point source
	CString strPSName = pDoc->lspc.strPointFilePath+pDoc->lspc.strPointFileName;	
	if (strPSName.GetLength() != 0 && pDoc->lspc.bWeather != false)
	{
		CString str = strPSName;
		str.MakeLower();
		str = str.Right(4);
		if(str.CompareNoCase(".mdb") == 0)
		{
			pApp->strLocation = strPSName;
			int nIndex = pApp->strLocation.ReverseFind('\\');
			if(nIndex != -1)
			{
				pApp->strDBFilePath = pApp->strLocation.Left(nIndex+1);
				pApp->strDBFileName = pApp->strLocation.Right(pApp->strLocation.GetLength() - nIndex - 1);
			}
			m_wndStatusBar.SetPaneText(0,"Reading point source data: File "+strPSName);
			pDoc->lspc.ReadDBPointSource();		
		}
		else
		{
			if (bRunNIMS == false)
			{
				m_wndStatusBar.SetPaneText(0,"Reading point source data: File "+strPSName);
				pDoc->lspc.ReadPointSource(strPSName);									
			}
		}
	}
	// read landuse change data
	CString strLUFileName = pDoc->lspc.cLU.strLUchangeFilePath;	
	if (strLUFileName.GetLength() != 0 && pDoc->lspc.bWeather != false)
	{
		CString str = strLUFileName;
		str.MakeLower();
		str = str.Right(4);
		if(str.CompareNoCase(".mdb") == 0)
		{
			pApp->strLocation = strLUFileName;
			int nIndex = pApp->strLocation.ReverseFind('\\');
			if(nIndex != -1)
			{
				pApp->strDBFilePath = pApp->strLocation.Left(nIndex+1);
				pApp->strDBFileName = pApp->strLocation.Right(pApp->strLocation.GetLength() - nIndex - 1);
			}
			m_wndStatusBar.SetPaneText(0,"Reading landuse change data: File "+strLUFileName);
			pDoc->lspc.cLU.ReadLUchangeDatabase(bRunNIMS);		
		}
		else
		{
			if (bRunNIMS == false)
			{
				m_wndStatusBar.SetPaneText(0,"Reading landuse change data: File "+strLUFileName);
				pDoc->lspc.cLU.ReadLUchangeTextFile(strLUFileName);									
			}
		}
	}

	pView->m_pMyPropSheet1->UpdateData(false);
	pView->m_pMyPropSheet2->UpdateData(false);
	pDoc->lspc.nContorlGlobal =0;
	pDoc->lspc.nWaterControl = 0;
	m_wndStatusBar.SetPaneText(0,"Ready");
	EndWaitCursor();
}

void CMainFrame::OnFileRun() 
{
	// read the weather station data first,
	CLSPCView *pView = (CLSPCView *)m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;
	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;

	CLSPCApp *pApp = (CLSPCApp*)AfxGetApp();
	//pApp->OpenLogFile(pDoc->lspc.m_strModelResultLocation+"LSPC.log");


	if((pDoc->lspc.m_strFileName.GetLength() == 0 && pDoc->lspc.m_strPath.GetLength() == 0) && !pDoc->lspc.bReadDB) 
	{
		AfxMessageBox("Read input file/database first.");
		pApp->Log("Read input file/database first.");
		return;
	}
//	if(pDoc->lspc.tmList == NULL || pDoc->lspc.bWeather == false)
	if(pDoc->lspc.cWD.pWFILEINFO == NULL || pDoc->lspc.bWeather == false)
	{
		AfxMessageBox("Weather station data is missing. Load weather data first before model run");
		pApp->Log("Weather station data is missing. Load weather data first before model run");
		return;
	}
	EnterCriticalSection(&myLock);

	//give me a chance to get out of here if data is missing 
	//or inappropriate
	if(!pView->m_pMyPropSheet1->UpdateData(TRUE))
		return;
	if(!pView->m_pMyPropSheet2->UpdateData(TRUE))
		return;
	if (!pDoc->lspc.cWD.WeatherDataMapping())
		return;

	for(int i=0; i<pDoc->lspc.cWD.nNumWeatherFiles; i++)
	{
		if (pDoc->lspc.cWD.FindDataIndex(pDoc->lspc.deltm ,i,pDoc->lspc.tStart,pDoc->lspc.twStart,pDoc->lspc.twEnd) == -1)
			return;
	}

	pDoc->lspc.lookupsws();
	BeginWaitCursor();

	if(m_wndStatusBar.GetSafeHwnd() != NULL && m_wndStatusBar.IsWindowVisible())
		m_wndStatusBar.SetPaneText(0,"Run PWater().");
	pDoc->lspc.UnitConvertion();
	pDoc->lspc.pwater();
	pDoc->lspc.ReverseUnitConvertion();
	EndWaitCursor();
	if(m_wndStatusBar.GetSafeHwnd() != NULL)
	m_wndStatusBar.SetPaneText(0,"The End");

	LeaveCriticalSection(&myLock);
}

void CMainFrame::OnFileGetfromdatabase() 
{
	EnterCriticalSection(&myLock);
	GetFromDatabase();
	LeaveCriticalSection(&myLock);
}

void CMainFrame::GetFromDatabase() 
{
	CLSPCView *pView = (CLSPCView *)m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;
	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;
	pDoc->lspc.nContorlGlobal = 0;
	pDoc->lspc.nWaterControl = 0;

	// (05-17-06)
	pDoc->lspc.strPointFilePath = "";
	pDoc->lspc.strPointFileName = "";
	pDoc->lspc.m_strPSDataLocation = "";
	pDoc->lspc.cLU.strLUchangeFilePath = "";
	pDoc->lspc.cLU.nLUchangeFlag = 0;
	pDoc->lspc.cLU.nNumLUchangeRecords = 0;

	// get flags from page 0
	pView->m_pMyPropSheet1->SetActivePage(0);
	pView->m_pMyPropSheet1->page0.UpdateData(true);
	if(bRunModel == false && bRunNIMS == false)
	{
		CDlgWQ dlg;
		if(dlg.DoModal() != IDOK)
			return;
	}
	BeginWaitCursor();
	// 12-18-05
	pDoc->lspc.ResetData();
	pDoc->lspc.cLU.ResetData();
	pDoc->lspc.cOP.ResetData();
	pDoc->lspc.cWD.ResetData();

	//read database file
	if(!pDoc->lspc.GetDBInfo())
		return;

	// TMDL PS deduction info
//	pDoc->lspc.AllocateTSSpace();

	pDoc->lspc.bWeather = true;
	pDoc->lspc.deltm = 60;

	if (!pDoc->lspc.cWD.GetWeatherDateTimeandInterval(pDoc->lspc.deltm,pDoc->lspc.twStart,pDoc->lspc.twEnd))
		return;

	pDoc->lspc.delts = pDoc->lspc.deltm * 60.;	// sec	
	pDoc->lspc.delt60 = pDoc->lspc.deltm / 60.;	// hour
	pDoc->lspc.deltd = pDoc->lspc.deltm / 1440.;// day	

	if (pDoc->lspc.tStart < pDoc->lspc.twStart)	pDoc->lspc.tStart = pDoc->lspc.twStart;
	if (pDoc->lspc.tEnd > pDoc->lspc.twEnd)	pDoc->lspc.tEnd = pDoc->lspc.twEnd;

	if (pDoc->lspc.toStart < pDoc->lspc.tStart)	pDoc->lspc.toStart = pDoc->lspc.tStart;
	if (pDoc->lspc.toEnd > pDoc->lspc.tEnd)	pDoc->lspc.toEnd = pDoc->lspc.tEnd;

	if (bRunNIMS == false)
	{
		for(int i=0; i<pDoc->lspc.cWD.nNumWeatherFiles; i++)
		{
			CString strFileName = pDoc->lspc.cWD.strWeatherFolderPath+pDoc->lspc.cWD.pWFILEINFO[i].strWFile;
			m_wndStatusBar.SetPaneText(0,"Reading File "+strFileName);

			int nIndex = strFileName.ReverseFind('.');
			CString strFileExtension = strFileName.Mid(nIndex, 4);
			if (strFileExtension.CompareNoCase(".air") == 0) 
			{
				if (!pDoc->lspc.cWD.ReadAirFile(pDoc->lspc.deltm,i,strFileName,pDoc->lspc.twStart,pDoc->lspc.twEnd))
				{
					pDoc->lspc.bWeather = false;
					m_wndStatusBar.SetPaneText(0,"Ready");
					break;
				}
			}
			else
			{
				if (!pDoc->lspc.cWD.ReadPreFile(pDoc->lspc.deltm,i,strFileName,pDoc->lspc.twStart,pDoc->lspc.twEnd))
				{
					pDoc->lspc.bWeather = false;
					m_wndStatusBar.SetPaneText(0,"Ready");
					break;
				}
			}
		}
	}
	// read point source
	if (pDoc->lspc.bWeather != false)
	{
		m_wndStatusBar.SetPaneText(0,"Reading point source data.");
		pDoc->lspc.ReadDBPointSource();	
		m_wndStatusBar.SetPaneText(0,"Ready");
	}
	// read landuse change source
	if (pDoc->lspc.bWeather != false)
	{
		m_wndStatusBar.SetPaneText(0,"Reading landuse change data.");
		pDoc->lspc.cLU.ReadLUchangeDatabase(bRunNIMS);	
		m_wndStatusBar.SetPaneText(0,"Ready");
	}

	pView->m_pMyPropSheet1->UpdateData(false);
	pView->m_pMyPropSheet2->UpdateData(false);
	pDoc->lspc.nContorlGlobal =0;
	pDoc->lspc.nWaterControl = 0;
	m_wndStatusBar.SetPaneText(0,"Ready");
	EndWaitCursor();	
}

void CMainFrame::OnFileLoadweatherdata() 
{
	CLSPCView *pView = (CLSPCView *)m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;
	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;
	pView->m_pMyPropSheet1->UpdateData(TRUE);
	pView->m_pMyPropSheet2->UpdateData(TRUE);

	BeginWaitCursor();

//	pDoc->lspc.AllocateTSSpace();

	pDoc->lspc.bWeather = true;
	pDoc->lspc.deltm = 60;

	if (!pDoc->lspc.cWD.GetWeatherDateTimeandInterval(pDoc->lspc.deltm,pDoc->lspc.twStart,pDoc->lspc.twEnd))
		return;

	pDoc->lspc.delts = pDoc->lspc.deltm * 60.;	// sec	
	pDoc->lspc.delt60 = pDoc->lspc.deltm / 60.;	// hour
	pDoc->lspc.deltd = pDoc->lspc.deltm / 1440.;// day	

	if (pDoc->lspc.tStart < pDoc->lspc.twStart)	pDoc->lspc.tStart = pDoc->lspc.twStart;
	if (pDoc->lspc.tEnd > pDoc->lspc.twEnd)	pDoc->lspc.tEnd = pDoc->lspc.twEnd;

	if (pDoc->lspc.toStart < pDoc->lspc.tStart)	pDoc->lspc.toStart = pDoc->lspc.tStart;
	if (pDoc->lspc.toEnd > pDoc->lspc.tEnd)	pDoc->lspc.toEnd = pDoc->lspc.tEnd;

	for(int i=0; i<pDoc->lspc.cWD.nNumWeatherFiles; i++)
	{
		CString strFileName = pDoc->lspc.cWD.strWeatherFolderPath+pDoc->lspc.cWD.pWFILEINFO[i].strWFile;
		m_wndStatusBar.SetPaneText(0,"Reading File "+strFileName);

		int nIndex = strFileName.ReverseFind('.');
		CString strFileExtension = strFileName.Mid(nIndex, 4);
		if (strFileExtension.CompareNoCase(".air") == 0) 
		{
			if (!pDoc->lspc.cWD.ReadAirFile(pDoc->lspc.deltm,i,strFileName,pDoc->lspc.twStart,pDoc->lspc.twEnd))
			{
				pDoc->lspc.bWeather = false;
				m_wndStatusBar.SetPaneText(0,"Ready");
				break;
			}
		}
		else
		{
			if (!pDoc->lspc.cWD.ReadPreFile(pDoc->lspc.deltm,i,strFileName,pDoc->lspc.twStart,pDoc->lspc.twEnd))
			{
				pDoc->lspc.bWeather = false;
				m_wndStatusBar.SetPaneText(0,"Ready");
				break;
			}
		}
	}

	pView->m_pMyPropSheet1->UpdateData(false);
	pView->m_pMyPropSheet2->UpdateData(false);

	m_wndStatusBar.SetPaneText(0,"Ready");
	EndWaitCursor();	
}

void CMainFrame::OnFileSavedefaultparameterstodatabase() 
{
	CLSPCView *pView = (CLSPCView *)m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;
	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;
	
	//set flag to avoid closing the project
	bsaveflag = true;
	EnterCriticalSection(&myLock);
	BeginWaitCursor();
	pView->m_pMyPropSheet1->UpdateData(TRUE);
	pView->m_pMyPropSheet2->UpdateData(TRUE);
	EndWaitCursor();
	BeginWaitCursor();
	pDoc->lspc.UpdateDBParm();
	EndWaitCursor();
	LeaveCriticalSection(&myLock);
	bsaveflag = false;
}

void CMainFrame::OnLoadPointSourceFile() 
{
	CLSPCView *pView = (CLSPCView *)m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;
	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;

	CFileDialog dlg(TRUE,"*.inp",NULL,NULL,"Point Source Text Files (*.inp)|*.inp|All Files (*.*)|*.*||");
	
	if(dlg.DoModal() == IDOK)
	{
		// get file name
		BeginWaitCursor();

		CString strFileName = dlg.GetPathName();
		int nIndex = strFileName.ReverseFind('\\');
		if(nIndex != -1)
		{
			pDoc->lspc.strPointFilePath = strFileName.Left(nIndex+1);
			pDoc->lspc.strPointFileName = strFileName.Right(strFileName.GetLength() - nIndex - 1);
			pDoc->lspc.m_strPSDataLocation = strFileName;
		}
		m_wndStatusBar.SetPaneText(0,"Read point source data: File "+strFileName);//   (02-2005)
		pDoc->lspc.ReadPointSource(strFileName);

		pView->m_pMyPropSheet1->UpdateData(false);		
		pView->m_pMyPropSheet2->UpdateData(false);		
		m_wndStatusBar.SetPaneText(0,"Ready");			
		
		EndWaitCursor();
	}
}

void CMainFrame::OnDBLoadPointSource() 
{
	CLSPCView *pView = (CLSPCView *)m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;
	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;
	
	// update point source file path
	CLSPCApp *pApp = (CLSPCApp *) AfxGetApp();
	pApp->strLocation.Format("");
	pApp->strDBFileName.Format("");
	pApp->strDBFilePath.Format("");

	EnterCriticalSection(&myLock);

	BeginWaitCursor();
	pView->m_pMyPropSheet1->UpdateData(true);
	pView->m_pMyPropSheet2->UpdateData(true);
	m_wndStatusBar.SetPaneText(0,"Reading point source data");  
	pDoc->lspc.ReadDBPointSource();
	pView->m_pMyPropSheet1->UpdateData(false);
	pView->m_pMyPropSheet2->UpdateData(false);
	m_wndStatusBar.SetPaneText(0,"Ready");  
	EndWaitCursor();	

	LeaveCriticalSection(&myLock);
}

void CMainFrame::OnFileSavePointSourceData() 
{
	CLSPCView *pView = (CLSPCView *)m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;
	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;

	CFileDialog dlg(FALSE,"*.inp",NULL,NULL,"Point Source Files (*.inp)|*.inp|All Files (*.*)|*.*||");
	
	if(dlg.DoModal() == IDOK)
	{
		//set flag to avoid closing the project
		bsaveflag = true;
		CString strFileName = dlg.GetPathName();
		BeginWaitCursor();
		pDoc->lspc.WritePointSource(strFileName);
		EndWaitCursor();		
		bsaveflag = false;
	}	
}

void CMainFrame::SetPercentage(double lfPerc)
{
	ASSERT(m_wndStatusBar.GetSafeHwnd() != NULL);
	CDC *pDC = m_wndStatusBar.GetDC();
	if(pDC == NULL)
	  return;

	CRect rect;
	m_wndStatusBar.GetItemRect(3,&rect);

	CBrush brushW(RGB(255,255,255));
	CBrush brushB(RGB(0,0,200));
	
	pDC->FillRect(&rect,&brushW);
    CRect rect1 = rect;
	rect1.right = (long)(rect1.left+rect.Width()*lfPerc);
	pDC->FillRect(&rect1,&brushB);

	CString str;
	int k = (int)(lfPerc*100);
	str.Format("%d%%",k);

	pDC->SetBkMode(TRANSPARENT);

    CRgn rgn;
    rgn.CreateRectRgn(rect1.left, rect1.top, rect1.right, rect1.bottom);
    pDC->SelectClipRgn(&rgn);
    pDC->SetTextColor(RGB(255,255,255));

    pDC->DrawText(str, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    rgn.DeleteObject();
    rgn.CreateRectRgn(rect1.right, rect1.top, rect.right, rect.bottom);
    pDC->SelectClipRgn(&rgn);
    pDC->SetTextColor(RGB(0,0,200));

    pDC->DrawText(str, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

}

BOOL CMainFrame::DestroyWindow() 
{
	//
	CLSPCView *pView = (CLSPCView *)m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;
	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;

	//pDoc->lspc.bCalcEnd = true;
	//Sleep(1000);

	BOOL bResult =  CFrameWnd::DestroyWindow();

	return bResult;
}

void CMainFrame::OnViewComments() 
{
	CLSPCView *pView = (CLSPCView *)m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;
	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;

	int nIndex = pView->GetGeneralIndex();

	CCommentsDlg dlg;
	dlg.strTitle = "User Comments";
	dlg.m_strComments = pDoc->lspc.strComments[nPageIndex[nIndex]];
	if(dlg.DoModal() == IDOK)
		pDoc->lspc.strComments[nPageIndex[nIndex]] = dlg.m_strComments;
}

void CMainFrame::OnViewParameterinfo() 
{
	CLSPCView *pView = (CLSPCView *)m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;
	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;

	int nIndex = pView->GetGeneralIndex();

	CCommentsDlg dlg;
	dlg.strTitle = "Parameter Information";
	dlg.m_strComments = pDoc->lspc.strDesc[nPageIndex[nIndex]];
	dlg.DoModal();
}

void CMainFrame::OnClose() 
{
	//check the save flag
	if(bsaveflag == true)
	{
		//give warning message
		//AfxMessageBox("Saving data... please wait and close after some time");
		return;
	}
	else
	{
		EnterCriticalSection(&myLock);	
		CFrameWnd::OnClose();
		LeaveCriticalSection(&myLock);
	}
}

int CMainFrame::GetGeneralIndex(int nSheetNum, int nPageIndex)
{
	CString strKey;
	strKey.Format("%ld", nSheetNum*1000+nPageIndex);

	CString strValue;
	if (m_indexMap.Lookup(strKey, strValue))
		return atoi((LPCSTR)strValue);

	return 0;
}

void CMainFrame::OnFileLoadLanduseChangeTextfile() 
{
	CLSPCView *pView = (CLSPCView *)m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;
	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;

	CFileDialog dlg(TRUE,"*.inp",NULL,NULL,"Landuse Change Text Files (*.inp)|*.inp|All Files (*.*)|*.*||");
	
	if(dlg.DoModal() == IDOK)
	{
		// get file name
		BeginWaitCursor();

		CString strFileName = dlg.GetPathName();
		int nIndex = strFileName.ReverseFind('\\');
		if(nIndex != -1)
		{
			pDoc->lspc.cLU.nLUchangeFlag = 1;
			pDoc->lspc.cLU.strLUchangeFilePath = strFileName;
		}
		m_wndStatusBar.SetPaneText(0,"Read landuse change data: File "+strFileName);
		pDoc->lspc.cLU.ReadLUchangeTextFile(strFileName);

		pView->m_pMyPropSheet1->UpdateData(false);		
		pView->m_pMyPropSheet2->UpdateData(false);		
		m_wndStatusBar.SetPaneText(0,"Ready");			
		
		EndWaitCursor();
	}
}

void CMainFrame::OnFileLoadLanduseChangeDatabase() 
{
	CLSPCView *pView = (CLSPCView *)m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;
	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;

	// update landuse change file path
	CLSPCApp *pApp = (CLSPCApp *) AfxGetApp();
	pApp->strLocation.Format("");
	pApp->strDBFileName.Format("");
	pApp->strDBFilePath.Format("");

	EnterCriticalSection(&myLock);

	BeginWaitCursor();
	pView->m_pMyPropSheet1->UpdateData(true);
	pView->m_pMyPropSheet2->UpdateData(true);
	m_wndStatusBar.SetPaneText(0,"Reading landuse change data"); 
	pDoc->lspc.cLU.nLUchangeFlag = 1;
	pDoc->lspc.cLU.ReadLUchangeDatabase(bRunNIMS);
	pView->m_pMyPropSheet1->UpdateData(false);
	pView->m_pMyPropSheet2->UpdateData(false);
	m_wndStatusBar.SetPaneText(0,"Ready"); 
	EndWaitCursor();	

	LeaveCriticalSection(&myLock);
}

void CMainFrame::OnFileSaveLanduseChangeData() 
{
	CLSPCView *pView = (CLSPCView *)m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;
	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;

	CFileDialog dlg(FALSE,"*.inp",NULL,NULL,"Landuse Change Text Files (*.inp)|*.inp|All Files (*.*)|*.*||");
	
	if(dlg.DoModal() == IDOK)
	{
		//set flag to avoid closing the project
		bsaveflag = true;
		CString strFileName = dlg.GetPathName();
		BeginWaitCursor();
		pDoc->lspc.cLU.WriteLUchangeTextFile(strFileName);
		EndWaitCursor();		
		bsaveflag = false;
	}	
}
