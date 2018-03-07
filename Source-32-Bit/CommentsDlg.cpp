// CommentsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "CommentsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommentsDlg dialog


CCommentsDlg::CCommentsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommentsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommentsDlg)
	m_strComments = _T("");
	//}}AFX_DATA_INIT
	strTitle = _T("");
}


void CCommentsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommentsDlg)
	DDX_Control(pDX, IDOK, m_cExit);
	DDX_Control(pDX, IDC_RICHEDIT, m_cEdit);
	DDX_Text(pDX, IDC_RICHEDIT, m_strComments);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommentsDlg, CDialog)
	//{{AFX_MSG_MAP(CCommentsDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommentsDlg message handlers

void CCommentsDlg::OnOK() 
{
	CDialog::OnOK();
}


BOOL CCommentsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(!strTitle.IsEmpty())
		SetWindowText(strTitle);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCommentsDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_cEdit.GetSafeHwnd() == NULL)
		return;

	CRect cr, cr1;

	m_cEdit.GetWindowRect(&cr);
	ScreenToClient(&cr);

	m_cExit.GetWindowRect(&cr1);
	ScreenToClient(&cr1);

	int xDelta = cx-2*cr.left+1;
	if (xDelta < 50)
		xDelta = 50;
	int yDelta = cy-2*cr.top-(cr1.bottom-cr.bottom);
	if (yDelta < 50)
		yDelta = 50;

	m_cEdit.MoveWindow(cr.left, cr.top, xDelta, yDelta, TRUE);
	yDelta = yDelta-cr.Height();
	m_cExit.GetWindowRect(&cr);
	ScreenToClient(&cr);
	m_cExit.MoveWindow(cr.left, cr.top+yDelta, cr.Width(), cr.Height(), TRUE);		
}
