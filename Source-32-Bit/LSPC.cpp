// LSPC.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "LSPC.h"
#include "LSPCView.h"
#include "MainFrm.h"
#include "LSPCDoc.h"
#include "LeftView.h"
#include <odbcinst.h>
#include <io.h>
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLSPCApp

BEGIN_MESSAGE_MAP(CLSPCApp, CWinApp)
	//{{AFX_MSG_MAP(CLSPCApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLSPCApp construction
CString CLSPCApp::strLocation = "";
CString CLSPCApp::strDBFilePath = "";
CString CLSPCApp::strDBFileName = "";
//static _HFILE hFileReport = 0;

CLSPCApp::CLSPCApp()
{
	fpDebug = NULL;
}

CLSPCApp::~CLSPCApp()
{
/*
	_CrtDumpMemoryLeaks();

	if(hFileReport != NULL)
		CloseHandle(hFileReport);
	*/
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CLSPCApp object

CLSPCApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CLSPCApp initialization

BOOL CLSPCApp::InitInstance()
{
	OpenLogFile();

	AfxInitRichEdit(); 
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
/*
	char szPathBuf[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char filename[_MAX_FNAME];
	char ext[_MAX_EXT];

	GetModuleFileName(AfxGetInstanceHandle(),szPathBuf,_MAX_PATH);
		

	_splitpath(szPathBuf, drive, dir, filename, ext );
	_makepath( szPathBuf, drive, dir, "lspc", ".err");

	hFileReport = NULL;

	hFileReport= CreateFile(szPathBuf,
						GENERIC_WRITE,
						FILE_SHARE_WRITE,
						NULL,
						OPEN_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

	if(hFileReport != NULL)
	{
	 _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	 _CrtSetReportFile(_CRT_ERROR,::hFileReport);
	 _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	 _CrtSetReportFile(_CRT_WARN,::hFileReport);
	 _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	 _CrtSetReportFile(_CRT_ASSERT,::hFileReport);
	}
*/
	AddDocTemplate(new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CLSPCDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CLeftView)));

	// Parse command line for standard shell commands, DDE, file open
	
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
//	if (!ProcessShellCommand(cmdInfo))
//		return FALSE;
	OnFileNew();
	
	// The one and only window has been initialized, so show and update it.
	//m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	
	//m_pMainWnd->CenterWindow();
	//m_pMainWnd->UpdateWindow();
	//
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();	
	if(pMain == NULL)
		return false;
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;
	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;
	
	CString str;

	if(!cmdInfo.m_strFileName.IsEmpty())
	{
		str = cmdInfo.m_strFileName;
		str.MakeLower();
		CString str_run = str.Right(4);
		CString str_nims = str.Right(5);
		if(str_run.CompareNoCase("_run") == 0)
		{
			pMain->bRunModel = true;
			cmdInfo.m_strFileName = cmdInfo.m_strFileName.Left(cmdInfo.m_strFileName.GetLength() - 4);
		}
		else if(str_nims.CompareNoCase("_nims") == 0)
		{
			pMain->bRunNIMS = true;
			pDoc->lspc.bRunNIMS = true;
			cmdInfo.m_strFileName = cmdInfo.m_strFileName.Left(cmdInfo.m_strFileName.GetLength() - 5);
		}

		// does this file exist
		if( (_access(LPCSTR(cmdInfo.m_strFileName), 0 )) != -1 )
			strLocation = cmdInfo.m_strFileName;
	}
	
	// The one and only window has been initialized, so show and update it.
	if (pMain->bRunNIMS == true || pMain->bRunModel == true)
		m_pMainWnd->ShowWindow(SW_HIDE);
	else
		m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);

	m_pMainWnd->UpdateWindow();

	if(!strLocation.IsEmpty())
	{
		int nIndex = strLocation.ReverseFind('\\');
		if(nIndex != -1)
		{
			strDBFilePath = strLocation.Left(nIndex+1);
			strDBFileName = strLocation.Right(strLocation.GetLength() - nIndex - 1);
		}
		else if (pMain->bRunNIMS == true || pMain->bRunModel == true)
		{
			//get the current working directory path
			char cwdpath[_MAX_PATH];  // This is a buffer for the text
			_getcwd(cwdpath, _MAX_PATH);

			//CString str1(cwdpath);
			strDBFilePath = LPCSTR(cwdpath);
			strDBFilePath += "\\";
			strDBFileName = strLocation;
		}

		pMain->strPath = strDBFilePath;
		pMain->SetWindowText("LSPC Model - "+strDBFileName);
		
		str = strLocation;
		str.MakeLower();
		str = str.Right(4);
		if(str.CompareNoCase(".mdb") == 0)
		{
			//open database
			pMain->GetFromDatabase();
			if(pMain->bRunModel == true)
			{
				//run the model
				pMain->OnFileRun();
				//close the application
				pMain->OnClose();
			}
			if(pMain->bRunNIMS == true)
			{
				//save the input file
				CString strFilePath = strDBFilePath + "LSPC_NIMS.inp";
				pDoc->lspc.WriteInputFile(strFilePath);
				//close the application
				pMain->OnClose();
			}
		}
		else if(str.CompareNoCase(".inp") == 0)
		{
			//open text file
			pMain->OpenInputFile(strLocation);
			if(pMain->bRunModel == true)
			{
				//run the model
				pMain->OnFileRun();
				//close the application
				pMain->OnClose();
			}
			if(pMain->bRunNIMS == true)
			{
				//save the input file
				CString strFilePath = strDBFilePath + "LSPC_NIMS.inp";
				pDoc->lspc.WriteInputFile(strFilePath);
				//close the application
				pMain->OnClose();
			}
		}
	}
//	while(strLocation == "")
//		GetDBLocation();
	return TRUE;
}

CString CLSPCApp::GetMDBDriver()
{
	char szBuf[2048];
	WORD cbBufMax = 2000;
	WORD cbBufOut;
	char *pszBuf = szBuf;
	CString sDriver;
	CString strSQL;

	// Get the names of the installed drivers ("odbcinst.h" has to be included )
   if(!SQLGetInstalledDrivers(szBuf,cbBufMax,& cbBufOut))
		return "";
	
	// Search for the driver...
	do
	{
		if( strstr( pszBuf, "Microsoft Access Driver" ) != 0 )
		{
			// Found !
			sDriver = CString( pszBuf );
			break;
		}
		pszBuf = strchr( pszBuf, '\0' ) + 1;
	}
	while( pszBuf[1] != '\0' );

	if(sDriver.IsEmpty())
		return sDriver;
	//
	//char szBuf[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char filename[_MAX_FNAME];
	char ext[_MAX_EXT];

	//GetModuleFileName(AfxGetInstanceHandle(),szBuf,_MAX_PATH);
	if(strLocation == "")
		GetDBLocation();

	if(strLocation == "")
		return "";
		

	_splitpath(LPCSTR(strLocation), drive, dir, filename, ext );


	strSQL.Format("DRIVER={%s};DSN='';READONLY=FALSE;DBQ=%s;DEFAULTDIR=%s%s", 
		sDriver,strLocation, drive,dir);

	return strSQL;
}

CString CLSPCApp::GetResultMDBDriver()
{
	char szBuf[2048];
	WORD cbBufMax = 2000;
	WORD cbBufOut;
	char *pszBuf = szBuf;
	CString sDriver;
	CString strSQL;

	// Get the names of the installed drivers ("odbcinst.h" has to be included )
   if(!SQLGetInstalledDrivers(szBuf,cbBufMax,& cbBufOut))
		return "";
	
	// Search for the driver...
	do
	{
		if( strstr( pszBuf, "Microsoft Access Driver" ) != 0 )
		{
			// Found !
			sDriver = CString( pszBuf );
			break;
		}
		pszBuf = strchr( pszBuf, '\0' ) + 1;
	}
	while( pszBuf[1] != '\0' );

	if(sDriver.IsEmpty())
		return sDriver;
	//
	char szPathBuf[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char filename[_MAX_FNAME];
	char ext[_MAX_EXT];

	GetModuleFileName(AfxGetInstanceHandle(),szPathBuf,_MAX_PATH);
		

	_splitpath(szPathBuf, drive, dir, filename, ext );
	_makepath( szPathBuf, drive, dir, "lspc", ".mdb");



	strSQL.Format("DRIVER={%s};DSN='';READONLY=FALSE;DBQ=%s;DEFAULTDIR=%s%s", 
		sDriver,szPathBuf, drive,dir);

	return strSQL;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CLSPCApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CLSPCApp message handlers


bool CLSPCApp::GetDBLocation()
{
	if(strLocation == "")
	{
		CFileDialog dlg(TRUE,"*.mdb",NULL,NULL,"MS Access File(*.mdb)|*.mdb");
		
		if(dlg.DoModal() == IDOK)
		{
			// get file name
			strLocation = dlg.GetPathName();
			CString strFileOnly = dlg.GetFileName();			// (01-2005)
			strDBFileName = strFileOnly;
			strDBFilePath = strLocation.Left(strLocation.GetLength()-strFileOnly.GetLength());
			CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();	// (01-2005)
			pMain->SetWindowText("LSPC Model - "+strFileOnly);	// (01-2005)
		}
	}
	return false;
}

int CLSPCApp::ExitInstance() 
{
	if(fpDebug != NULL)
		fclose(fpDebug);
	return CWinApp::ExitInstance();
}
/*
void CLSPCApp::Log(CString strMessage)
{
	if(fpDebug != NULL)
	{
		COleDateTime dt = COleDateTime::GetCurrentTime();
		fputs(LPCSTR(dt.Format("%c")),fpDebug);
		fputs("\t",fpDebug);
		fputs(LPCSTR(strMessage),fpDebug);
		fputs("\n",fpDebug);
	}
}
*/
void CLSPCApp::Log(LPCTSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

	int nBuf;
	TCHAR szBuffer[1024];

	nBuf = _vsntprintf(szBuffer, 1024, lpszFormat, args);

	COleDateTime dt = COleDateTime::GetCurrentTime();
	fputs(LPCSTR(dt.Format("%c")),fpDebug);
	fputs("\t",fpDebug);
	fputs(szBuffer,fpDebug);
	fputs("\n",fpDebug);
	
	va_end(args);
}


void CLSPCApp::OpenLogFile()
{
	if(fpDebug != NULL)
		fclose(fpDebug);

	char szBuf[_MAX_PATH];
	GetModuleFileName(AfxGetInstanceHandle(),szBuf,_MAX_PATH);
	CString strBat(szBuf);
	int nIndex = strBat.ReverseFind('\\');
	if(nIndex != -1)
		strBat = strBat.Left(nIndex+1);
	strBat += "LSPC.log";
	fpDebug = fopen(LPCSTR(strBat),"a+t");
}
