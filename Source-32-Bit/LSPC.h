// LSPC.h : main header file for the LSPC application
//

#if !defined(AFX_LSPC_H__472C2794_E940_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_LSPC_H__472C2794_E940_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CLSPCApp:
// See LSPC.cpp for the implementation of this class
//

class CLSPCApp : public CWinApp
{
public:
	void OpenLogFile();
	//void Log(CString strMessage);
	void Log(LPCTSTR lpszFormat, ...);
	static bool GetDBLocation();
	CLSPCApp();
	virtual ~CLSPCApp();
	static CString GetMDBDriver();
	static CString strLocation;
	static CString strDBFilePath;				
	static CString strDBFileName;				
	FILE *fpDebug;
	static CString GetResultMDBDriver();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLSPCApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CLSPCApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LSPC_H__472C2794_E940_11D3_B2F2_00104B9CFF37__INCLUDED_)
