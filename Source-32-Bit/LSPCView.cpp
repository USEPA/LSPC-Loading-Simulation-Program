// LSPCView.cpp : implementation of the CLSPCView class
//

#include "stdafx.h"
#include "LSPC.h"

#include "MainFrm.h"
#include "LSPCDoc.h"
#include "LSPCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLSPCView

IMPLEMENT_DYNCREATE(CLSPCView, CFormView)

BEGIN_MESSAGE_MAP(CLSPCView, CFormView)
	//{{AFX_MSG_MAP(CLSPCView)
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLSPCView construction/destruction

CLSPCView::CLSPCView()
	: CFormView(CLSPCView::IDD)
{
	//{{AFX_DATA_INIT(CLSPCView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_nPropertySheetNum = 0;
	m_pMyPropSheet1 = NULL;
	m_pMyPropSheet2 = NULL;
}

CLSPCView::~CLSPCView()
{
	if(m_pMyPropSheet1 != NULL)
		delete m_pMyPropSheet1;
	if(m_pMyPropSheet2 != NULL)
		delete m_pMyPropSheet2;
}

void CLSPCView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLSPCView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CLSPCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CLSPCView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	
	// create and asociated the property sheet with the "place holder" window
	m_pMyPropSheet1 = new CMyPropertySheet1(this);
	if (!m_pMyPropSheet1->Create(this, WS_CHILD | WS_VISIBLE, 0))
	{
		delete m_pMyPropSheet1;
		m_pMyPropSheet1 = NULL;
		return;
	}

	// fit the property sheet into the place holder window, and show it
	CRect rectPropSheet;
	GetWindowRect(rectPropSheet);
	m_pMyPropSheet1->SetWindowPos(NULL, 0, 0,
		rectPropSheet.Width(), rectPropSheet.Height(),
		SWP_NOZORDER | SWP_NOACTIVATE);

	// create and asociated the property sheet with the "place holder" window
	m_pMyPropSheet2 = new CMyPropertySheet2(this);
	if (!m_pMyPropSheet2->Create(this, WS_CHILD | WS_VISIBLE, 0))
	{
		delete m_pMyPropSheet2;
		m_pMyPropSheet2 = NULL;
		return;
	}

	// fit the property sheet into the place holder window, and show it
	m_pMyPropSheet2->SetWindowPos(NULL, 0, 0,
		rectPropSheet.Width(), rectPropSheet.Height(),
		SWP_NOZORDER | SWP_NOACTIVATE | SWP_HIDEWINDOW);

	// set the first property sheet to be the default
	m_nPropertySheetNum = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CLSPCView printing

BOOL CLSPCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLSPCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLSPCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLSPCView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CLSPCView diagnostics

#ifdef _DEBUG
void CLSPCView::AssertValid() const
{
	CFormView::AssertValid();
}

void CLSPCView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CLSPCDoc* CLSPCView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLSPCDoc)));
	return (CLSPCDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLSPCView message handlers

void CLSPCView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	// change the property sheet position
	switch (m_nPropertySheetNum)
	{
	case 0:
		if(m_pMyPropSheet1 != NULL)
			m_pMyPropSheet1->Resize();
		break;
	case 1:
		if(m_pMyPropSheet2 != NULL)
			m_pMyPropSheet2->Resize();
		break;
	}
}

void CLSPCView::OnEditCut() 
{
	CPropertyPage *pPage;
	
	switch (m_nPropertySheetNum)
	{
	case 0:
		pPage = m_pMyPropSheet1->GetActivePage();
		break;
	case 1:
		pPage = m_pMyPropSheet2->GetActivePage();
		break;
	}

	if(pPage != NULL)
	{
		if(pPage->IsKindOf(RUNTIME_CLASS(CPageAdd)))
		{
			((CPageAdd *)pPage)->OnEditCut();
		}
	}
}

void CLSPCView::OnEditCopy() 
{
	CPropertyPage *pPage;
	
	switch (m_nPropertySheetNum)
	{
	case 0:
		pPage = m_pMyPropSheet1->GetActivePage();
		break;
	case 1:
		pPage = m_pMyPropSheet2->GetActivePage();
		break;
	}

	if(pPage != NULL)
	{
		if(pPage->IsKindOf(RUNTIME_CLASS(CPageAdd)))
		{
			((CPageAdd *)pPage)->OnEditCopy();
		}
	}
}

void CLSPCView::OnEditPaste() 
{
	CPropertyPage *pPage;
	
	switch (m_nPropertySheetNum)
	{
	case 0:
		pPage = m_pMyPropSheet1->GetActivePage();
		break;
	case 1:
		pPage = m_pMyPropSheet2->GetActivePage();
		break;
	}

	if(pPage != NULL)
	{
		if(pPage->IsKindOf(RUNTIME_CLASS(CPageAdd)))
		{
			((CPageAdd *)pPage)->OnEditPaste();
		}
	}
}

void CLSPCView::SetActivePage(int nSheetNum, int nPageIndex)
{
	switch (nSheetNum)
	{
	case 0:
		if(m_pMyPropSheet1 != NULL) {
			m_nPropertySheetNum = 0;
			m_pMyPropSheet1->ShowWindow(SW_SHOW);
			m_pMyPropSheet1->SetActivePage(nPageIndex);
			m_pMyPropSheet1->Resize();

			m_pMyPropSheet2->ShowWindow(SW_HIDE);
		}
		break;
	case 1:
		if(m_pMyPropSheet2 != NULL) {
			m_nPropertySheetNum = 1;
			m_pMyPropSheet1->ShowWindow(SW_HIDE);
			
			m_pMyPropSheet2->ShowWindow(SW_SHOW);
			m_pMyPropSheet2->SetActivePage(nPageIndex);
			m_pMyPropSheet2->Resize();
		}
		break;
	}
}

int CLSPCView::GetGeneralIndex()
{
	int nPageIndex;
	
	switch (m_nPropertySheetNum)
	{
	case 0:
		nPageIndex = m_pMyPropSheet1->GetActiveIndex();
		break;
	case 1:
		nPageIndex = m_pMyPropSheet2->GetActiveIndex();
		break;
	}

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	if(pMain == NULL)
		return 0;

	return pMain->GetGeneralIndex(m_nPropertySheetNum, nPageIndex);
}