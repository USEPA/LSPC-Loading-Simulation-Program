// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__472C2798_E940_11D3_B2F2_00104B9CFF37__INCLUDED_)
#define AFX_MAINFRM_H__472C2798_E940_11D3_B2F2_00104B9CFF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLSPCView;
class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CSplitterWnd m_wndSplitter;
	CString strPath;
	bool bsaveflag;
	bool bRunModel;
	bool bRunNIMS;		//create input text file for NIMS
	CMapStringToString m_indexMap;
		
public:

// Operations
public:
	void GetFromDatabase();
	int GetGeneralIndex(int nSheetNum, int nPageIndex);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void OpenInputFile(CString strFileName);
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // control bar embedded members
	void SetPercentage(double lfPerc);
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CReBar      m_wndReBar;
	CDialogBar  m_wndDlgBar;

// Generated message map functions
//protected:
public:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileRun();
	afx_msg void OnFileGetfromdatabase();
	afx_msg void OnFileLoadweatherdata();
	afx_msg void OnFileSavedefaultparameterstodatabase();
	afx_msg void OnLoadPointSourceFile();
	afx_msg void OnDBLoadPointSource();
	afx_msg void OnFileSavePointSourceData();
	afx_msg void OnViewComments();
	afx_msg void OnViewParameterinfo();
	afx_msg void OnClose();
	afx_msg void OnFileLoadLanduseChangeTextfile();
	afx_msg void OnFileLoadLanduseChangeDatabase();
	afx_msg void OnFileSaveLanduseChangeData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__472C2798_E940_11D3_B2F2_00104B9CFF37__INCLUDED_)
