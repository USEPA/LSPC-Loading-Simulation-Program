// DlgWQ.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "DlgWQ.h"
#include "RecordsetEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgWQ dialog


CDlgWQ::CDlgWQ(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWQ::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgWQ)
	m_b8 = FALSE;		// 
	m_b12 = FALSE;
	m_b19 = FALSE;
	m_b20 = FALSE;
	m_b21 = FALSE;
	m_b22 = FALSE;
	m_b23 = FALSE;
	m_b24 = FALSE;
	m_b25 = FALSE;
	m_b1 = FALSE;
	m_b2 = FALSE;
	m_b3 = FALSE;
	m_b4 = FALSE;
	m_b5 = FALSE;
	m_b6 = FALSE;
	m_b7 = FALSE;
	m_b10 = FALSE;
	m_b9 = FALSE;
	m_b11 = FALSE;
	m_b13 = FALSE;
	m_b15 = FALSE;
	m_b14 = FALSE;
	m_b16 = FALSE;
	m_b17 = FALSE;
	m_b18 = FALSE;
	m_b26 = FALSE;
	m_b27 = FALSE;
	m_b28 = FALSE;
	m_b29 = FALSE;
	m_b36 = FALSE;
	m_b37 = FALSE;
	m_b38 = FALSE;
	m_b39 = FALSE;
	m_b40 = FALSE;
	m_b41 = FALSE;
	m_b42 = FALSE;
	m_b43 = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgWQ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgWQ)
	DDX_Check(pDX, IDC_CHECK8, m_b8);
	DDX_Check(pDX, IDC_CHECK12, m_b12);
	DDX_Check(pDX, IDC_CHECK19, m_b19);
	DDX_Check(pDX, IDC_CHECK20, m_b20);
	DDX_Check(pDX, IDC_CHECK21, m_b21);
	DDX_Check(pDX, IDC_CHECK22, m_b22);
	DDX_Check(pDX, IDC_CHECK23, m_b23);
	DDX_Check(pDX, IDC_CHECK24, m_b24);
	DDX_Check(pDX, IDC_CHECK25, m_b25);
	DDX_Check(pDX, IDC_CHECK1, m_b1);
	DDX_Check(pDX, IDC_CHECK2, m_b2);
	DDX_Check(pDX, IDC_CHECK3, m_b3);
	DDX_Check(pDX, IDC_CHECK4, m_b4);
	DDX_Check(pDX, IDC_CHECK5, m_b5);
	DDX_Check(pDX, IDC_CHECK6, m_b6);
	DDX_Check(pDX, IDC_CHECK7, m_b7);
	DDX_Check(pDX, IDC_CHECK10, m_b10);
	DDX_Check(pDX, IDC_CHECK9, m_b9);
	DDX_Check(pDX, IDC_CHECK11, m_b11);
	DDX_Check(pDX, IDC_CHECK13, m_b13);
	DDX_Check(pDX, IDC_CHECK15, m_b15);
	DDX_Check(pDX, IDC_CHECK14, m_b14);
	DDX_Check(pDX, IDC_CHECK16, m_b16);
	DDX_Check(pDX, IDC_CHECK17, m_b17);
	DDX_Check(pDX, IDC_CHECK18, m_b18);
	DDX_Check(pDX, IDC_CHECK26, m_b26);
	DDX_Check(pDX, IDC_CHECK27, m_b27);
	DDX_Check(pDX, IDC_CHECK28, m_b28);
	DDX_Check(pDX, IDC_CHECK29, m_b29);
	DDX_Check(pDX, IDC_CHECK36, m_b36);
	DDX_Check(pDX, IDC_CHECK37, m_b37);
	DDX_Check(pDX, IDC_CHECK38, m_b38);
	DDX_Check(pDX, IDC_CHECK39, m_b39);
	DDX_Check(pDX, IDC_CHECK40, m_b40);
	DDX_Check(pDX, IDC_CHECK41, m_b41);
	DDX_Check(pDX, IDC_CHECK42, m_b42);
	DDX_Check(pDX, IDC_CHECK43, m_b43);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgWQ, CDialog)
	//{{AFX_MSG_MAP(CDlgWQ)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWQ message handlers

void CDlgWQ::OnOK() 
{
	UpdateData();

	CString strSQL;
	int flags[15];

	CDatabase db;
	BOOL bCanOpen = FALSE;
	try
	{
		strSQL = CLSPCApp::GetMDBDriver();
		if(strSQL.IsEmpty())
		{
			AfxMessageBox("Cannot find the Microsoft Access ODBC driver.");
			return;
		}
		bCanOpen = db.OpenEx(strSQL,CDatabase::noOdbcDialog);
	}
	catch(CException e)
	{
		AfxMessageBox("ODBC Error ---Cannot open database for reading.");
		return;
	}
	if(!bCanOpen)
	{
		AfxMessageBox("ODBC Error ---Cannot open database for reading.");
		return;
	}
	CRecordsetEx rs(&db);
	
	// card0
	flags[0]  = (m_b8)?1:0;
	flags[1]  = (m_b12)?1:0;
	flags[2]  = (m_b19)?1:0;
	flags[3]  = (m_b21)?1:0;
	flags[4]  = (m_b20)?1:0;
	flags[5]  = (m_b22)?1:0;
	flags[6]  = (m_b23)?1:0;
	flags[7]  = (m_b24)?1:0;
	flags[8]  = (m_b25)?1:0;

	for(int i=0; i<9; i++)
	{
		strSQL.Format("UPDATE ModelFlags SET VALUE1=%d WHERE PARMID=%d",flags[i],i+1);
		db.ExecuteSQL(LPCSTR(strSQL));
	}
	rs.ExecDirect(strSQL);
	rs.Close();

	// card100
	flags[0] = (m_b1)?1:0;
	flags[1] = (m_b2)?1:0;
	flags[2] = (m_b3)?1:0;
	flags[3] = (m_b4)?1:0;
	flags[4] = (m_b5)?1:0;
	flags[5] = (m_b6)?1:0;

	for(i=0; i<6; i++)
	{
		strSQL.Format("UPDATE ModelFlags SET VALUE1=%d WHERE PARMID=%d",flags[i],i+10001);
		db.ExecuteSQL(LPCSTR(strSQL));
	}
	rs.ExecDirect(strSQL);
	rs.Close();

	// card201
	flags[0] = (m_b17)?1:0;
	flags[1] = (m_b26)?1:0;
	flags[2] = (m_b18)?1:0;

	for(i=0; i<3; i++)
	{
		strSQL.Format("UPDATE ModelFlags SET VALUE1=%d WHERE PARMID=%d",flags[i],i+20101);
		db.ExecuteSQL(LPCSTR(strSQL));
	}
	rs.ExecDirect(strSQL);
	rs.Close();

	// card255
	flags[0] = (m_b7)?1:0;
	flags[1] = (m_b10)?1:0;
	flags[2] = (m_b9)?1:0;
	flags[3] = (m_b11)?1:0;
	flags[4] = (m_b36)?1:0;
	flags[5] = (m_b37)?1:0;

	for(i=0; i<5; i++)
	{
		strSQL.Format("UPDATE ModelFlags SET VALUE1=%d WHERE PARMID=%d",flags[i],i*2+25501);
		db.ExecuteSQL(LPCSTR(strSQL));
	}
	strSQL.Format("UPDATE ModelFlags SET VALUE1=%d WHERE PARMID=%d",flags[5],25510);
	db.ExecuteSQL(LPCSTR(strSQL));
	rs.ExecDirect(strSQL);
	rs.Close();

	// card280
	flags[0] = (m_b38)?1:0;
	flags[1] = (m_b39)?1:0;

	for(i=0; i<2; i++)
	{
		strSQL.Format("UPDATE ModelFlags SET VALUE1=%d WHERE PARMID=%d",flags[i],i+28002);
		db.ExecuteSQL(LPCSTR(strSQL));
	}
	rs.ExecDirect(strSQL);
	rs.Close();

	// card400
	flags[0] = (m_b40)?1:0;
	flags[1] = (m_b41)?1:0;

	for(i=0; i<2; i++)
	{
		strSQL.Format("UPDATE ModelFlags SET VALUE1=%d WHERE PARMID=%d",flags[i],i+40003);
		db.ExecuteSQL(LPCSTR(strSQL));
	}
	rs.ExecDirect(strSQL);
	rs.Close();

	// card440
	flags[0] = (m_b42)?1:0;
	flags[1] = (m_b43)?1:0;

	for(i=0; i<2; i++)
	{
		strSQL.Format("UPDATE ModelFlags SET VALUE1=%d WHERE PARMID=%d",flags[i],i+44001);
		db.ExecuteSQL(LPCSTR(strSQL));
	}
	rs.ExecDirect(strSQL);
	rs.Close();

	// card460
	flags[0] = (m_b27)?1:0;
	flags[1] = (m_b28)?1:0;
	flags[2] = (m_b29)?1:0;

	for(i=0; i<3; i++)
	{
		strSQL.Format("UPDATE ModelFlags SET VALUE1=%d WHERE PARMID=%d",flags[i],i+46001);
		db.ExecuteSQL(LPCSTR(strSQL));
	}
	rs.ExecDirect(strSQL);
	rs.Close();

	// card502
	flags[0] = (m_b13)?1:0;
	flags[1] = (m_b14)?1:0;
	flags[2] = (m_b15)?1:0;
	flags[3] = (m_b16)?1:0;

	for(i=0; i<4; i++)
	{
		strSQL.Format("UPDATE ModelFlags SET VALUE1=%d WHERE PARMID=%d",flags[i],i+50201);
		db.ExecuteSQL(LPCSTR(strSQL));
	}
	rs.ExecDirect(strSQL);
	rs.Close();
	db.Close();

	CDialog::OnOK();
}

BOOL CDlgWQ::OnInitDialog() 
{
	CDialog::OnInitDialog();
	//get the default values from the database
	CString strSQL;
	CDatabase db;
	BOOL bCanOpen = FALSE;
	try
	{
		strSQL = CLSPCApp::GetMDBDriver();
		if(strSQL.IsEmpty())
		{
			AfxMessageBox("Cannot find the Microsoft Access ODBC driver.");
			return FALSE;
		}
		bCanOpen = db.OpenEx(strSQL,CDatabase::noOdbcDialog);
	}
	catch(CException e)
	{
		AfxMessageBox("ODBC Error ---Cannot open database for reading.");
		return FALSE;
	}
	if(!bCanOpen)
	{
		AfxMessageBox("ODBC Error ---Cannot open database for reading.");
		return FALSE;
	}

	CRecordsetEx rs(&db);		
	CString strTemp;
	int nTemp;
	//card0
	strSQL = "SELECT * FROM table0";
	BOOL bRet = rs.ExecDirect(strSQL);
	if(bRet)
	{
		rs.GetFieldValue((short)0, strTemp);
		// skip first value
		rs.GetFieldValue((short)1, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b8 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)2, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		//m_b12 = (nTemp == 0)?FALSE:TRUE; 
		m_b12 = TRUE; 
		rs.GetFieldValue((short)3, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b19 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)4, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b21 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)5, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b20 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)6, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b22 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)7, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b23 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)8, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b24 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)9, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b25 = (nTemp == 0)?FALSE:TRUE; 
		rs.Close();
	}
	//card100
	strSQL = "SELECT * FROM table100";
	bRet = rs.ExecDirect(strSQL);
	if(bRet)
	{
		rs.GetFieldValue((short)0, strTemp);
		// skip first value
		rs.GetFieldValue((short)1, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b1 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)2, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b2 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)3, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b3 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)4, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b4 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)5, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b5 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)6, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b6 = (nTemp == 0)?FALSE:TRUE; 
		rs.Close();
	}
	//card201
	strSQL = "SELECT * FROM table201";
	bRet = rs.ExecDirect(strSQL);
	if(bRet)
	{
		rs.GetFieldValue((short)0, strTemp);
		// ignore first value
		rs.GetFieldValue((short)1, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b17 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)2, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b26 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)3, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b18 = (nTemp == 0)?FALSE:TRUE; 
		rs.Close();
	}
	//card255
	strSQL = "SELECT * FROM table255";
	bRet = rs.ExecDirect(strSQL);
	if(bRet)
	{
		rs.GetFieldValue((short)0, strTemp);
		// ignore first value
		rs.GetFieldValue((short)1, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b7 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)3, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b10 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)5, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b9 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)7, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b11 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)9, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b36 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)10, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b37 = (nTemp == 0)?FALSE:TRUE; 
		rs.Close();
	}
	//card280
	strSQL = "SELECT * FROM table280";
	bRet = rs.ExecDirect(strSQL);
	if(bRet)
	{
		// ignore first two values
		rs.GetFieldValue((short)2, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b38 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)3, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b39 = (nTemp == 0)?FALSE:TRUE;
		rs.Close();
	}
	//card400
	strSQL = "SELECT * FROM table400";
	bRet = rs.ExecDirect(strSQL);
	if(bRet)
	{
		// ignore first three values
		rs.GetFieldValue((short)3, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b40 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)4, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b41 = (nTemp == 0)?FALSE:TRUE; 
		rs.Close();
	}
	//card440
	strSQL = "SELECT * FROM table440";
	bRet = rs.ExecDirect(strSQL);
	if(bRet)
	{
		// skip first line
		rs.GetFieldValue((short)1, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b42 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)2, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b43 = (nTemp == 0)?FALSE:TRUE; 
		rs.Close();
	}
	//card460
	strSQL = "SELECT * FROM table460";
	bRet = rs.ExecDirect(strSQL);
	if(bRet)
	{
		rs.GetFieldValue((short)0, strTemp);
		// ignore first value
		rs.GetFieldValue((short)1, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b27 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)2, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b28 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)3, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b29 = (nTemp == 0)?FALSE:TRUE; 
		rs.Close();
	}
	//card502
	strSQL = "SELECT * FROM table502";
	bRet = rs.ExecDirect(strSQL);
	if(bRet)
	{
		rs.GetFieldValue((short)0, strTemp);
		// skip first value
		rs.GetFieldValue((short)1, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b13 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)2, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b14 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)3, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b15 = (nTemp == 0)?FALSE:TRUE; 
		rs.GetFieldValue((short)4, strTemp);
		nTemp = atoi(LPCSTR(strTemp));
		m_b16 = (nTemp == 0)?FALSE:TRUE; 
		rs.Close();
	}

	db.Close();

	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
