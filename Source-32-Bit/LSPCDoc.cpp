// LSPCDoc.cpp : implementation of the CLSPCDoc class

#include "stdafx.h"
#include "LSPC.h"

#include "LSPCDoc.h"
#include "MainFrm.h"
#include "LSPCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLSPCDoc

IMPLEMENT_DYNCREATE(CLSPCDoc, CDocument)

BEGIN_MESSAGE_MAP(CLSPCDoc, CDocument)
	//{{AFX_MSG_MAP(CLSPCDoc)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLSPCDoc construction/destruction

CLSPCDoc::CLSPCDoc()
{
	// TODO: add one-time construction code here
}

CLSPCDoc::~CLSPCDoc()
{
}

BOOL CLSPCDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CLSPCDoc serialization

void CLSPCDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CLSPCDoc diagnostics

#ifdef _DEBUG
void CLSPCDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLSPCDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLSPCDoc commands


BOOL CLSPCDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CString str = lpszPathName;
	str.MakeLower();
	str = str.Right(4);
	if(str.CompareNoCase(".mdb") == 0)
		return true;

	lspc.ReadInfo(lpszPathName);
	//SetPathName(lpszPathName);
	CMainFrame *pWnd = (CMainFrame *)AfxGetMainWnd();
	if(pWnd == NULL)
		return FALSE;
	// get the path in the file name
	//CString str = lpszPathName;
	int nIndex = str.ReverseFind('\\');
	if(nIndex < 0)
		return FALSE;
	pWnd->strPath = str.Left(nIndex+1);
	pWnd->OpenInputFile(lpszPathName);
	/*
	CLSPCView *pView = (CLSPCView *)pWnd->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return FALSE;
	pView->m_pMyPropSheet1->UpdateData(false);
	pView->m_pMyPropSheet2->UpdateData(false);
	*/
	return FALSE;
}

BOOL CLSPCDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	CMainFrame *pWnd = (CMainFrame *)AfxGetMainWnd();
	if(pWnd == NULL)
		return FALSE;
	CLSPCView *pView = (CLSPCView *)pWnd->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return FALSE;
	//set flag to avoid closing the project
	pWnd->bsaveflag = true;
	pView->m_pMyPropSheet1->UpdateData(true);
	pView->m_pMyPropSheet2->UpdateData(true);
	lspc.WriteInputFile(lpszPathName);
	SetPathName(lpszPathName);
	pWnd->bsaveflag = false;
	return TRUE;
}

void CLSPCDoc::OnFileSaveAs() 
{
	CString str = GetPathName();		
	CFileDialog dlg(false,"*.inp",str,NULL,"Model Input Files (*.inp)|*.inp|All Files (*.*)|*.*||");
	if(dlg.DoModal() == IDOK)
	{
		// get file name
		CString strFileName = dlg.GetPathName();
		CString strFileOnly = dlg.GetFileName();
		int nIndex = strFileName.Find(LPCSTR(strFileOnly));
		if(nIndex != -1)
		{
			CMainFrame *pWnd = (CMainFrame *)AfxGetMainWnd();
			pWnd->strPath = strFileName.Left(nIndex);
			pWnd->SetWindowText("LSPC Model - "+strFileOnly);	// (01-2005)
		}
		OnSaveDocument(strFileName);
	}			
}

void CLSPCDoc::OnFileSave() 
{
	CString str = GetPathName();	
	if(str.IsEmpty())
		OnFileSaveAs();
	else
		OnSaveDocument(str);
}
