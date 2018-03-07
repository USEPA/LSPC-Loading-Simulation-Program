// ALXSyntaxEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "../INCLUDE/ALXSyntaxEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Function list

extern ALXPRS_DEF_FUNCTION alxFunList[];
extern alxFuncCount;

/////////////////////////////////////////////////////////////////////////////
// Helper functions

BOOL alxIsLogicOprExpr(LPCSTR szExpr)
{
	short nOprID;
	int nOprPrioritet;

	return alxIsLogicOprExpr(szExpr, nOprID, nOprPrioritet);
}

/////////////////////////////////////////////////////////////////////////////
// CALXSyntaxEditCtrl

CALXSyntaxEditCtrl::CALXSyntaxEditCtrl()
{
	m_alxFunList = alxFunList;
	m_alxFuncCount = alxFuncCount;

	m_bInForcedChange = FALSE;

	m_nPrevLine = 0;

	memset(&m_cfDefault, 0, sizeof(m_cfDefault));
	m_cfDefault.cbSize = sizeof(m_cfDefault);
	m_cfDefault.dwEffects = CFE_PROTECTED; 
	m_cfDefault.dwMask = CFM_FACE | CFM_SIZE | CFM_PROTECTED | CFM_COLOR;
	m_cfDefault.yHeight = 200;
//	m_cfDefault.crTextColor = RGB(0,0, 255);
	strcpy(m_cfDefault.szFaceName, _T("Courier New")); 
}

CALXSyntaxEditCtrl::~CALXSyntaxEditCtrl()
{
}


BEGIN_MESSAGE_MAP(CALXSyntaxEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CALXSyntaxEditCtrl)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(EN_PROTECTED, OnProtected)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CALXSyntaxEditCtrl message handlers

void CALXSyntaxEditCtrl::OnChange() 
{
	CHARRANGE cr;

	SetRedraw(FALSE);

	GetSel(cr);

	CString str;
	str.Format("Min = %d, Max = %d\n",cr.cpMin,cr.cpMax);
	TRACE(str);

	int nCurLineStart, nCurLineEnd;
	// get first select line
	nCurLineStart = LineFromChar(cr.cpMin);
	// get last select line
	nCurLineEnd = cr.cpMax >= 0 ? LineFromChar(cr.cpMax) : max(0, GetLineCount() - 1);

	int nFirstLine, nLastLine;	// colorize line range 
	if(GetStyle() & ES_AUTOHSCROLL)
	{
		nFirstLine = min(m_nPrevLine, nCurLineStart);
		nLastLine = max(0,min(max(m_nPrevLine, nCurLineEnd), GetLineCount() - 1));

		for(int nLine = nFirstLine; nLine <= nLastLine; nLine++)
		{
			CString strLineText;
			int nFirstLineChar = LineIndex(nLine);
			int nLen = LineLength(nFirstLineChar);

			GetLine(nLine, strLineText.GetBuffer(nLen+2));
			strLineText.ReleaseBuffer();

			//call the colorizer
			Colorize(strLineText, nFirstLineChar);
		}
	}
	else // colorize all
	{
		nFirstLine = 0;
		nLastLine = max(0, GetLineCount() - 1);

		SetSel(0, -1); // select all
		CString strText = GetSelText();

		//call the colorizer
		Colorize(strText, 0);
	}

/*
	// test
	int nStart	= LineIndex(nFirstLine);
	int nEnd	= LineIndex(nLastLine) + LineLength(LineIndex(nLastLine));

	SetSel(nStart, nEnd);

	SetDefaultCharFormat(m_cfDefault);
	SetFormatRange(0, -1, m_cfDefault.crTextColor);
	SetFormatRange(nStart, nEnd, TXT_CLR_ERROR);
*/
	SetSel(cr);

	SetRedraw(TRUE);

	if(GetLineCount() > 1)
	{
		CRect rcInvalid;
		GetClientRect(rcInvalid);
		rcInvalid.top = GetCharPos(LineIndex(nFirstLine)).y;
		int nHeightLine = GetCharPos(LineIndex(1)).y - GetCharPos(LineIndex(0)).y;
		rcInvalid.bottom = rcInvalid.top + (nLastLine - nFirstLine + 1) * nHeightLine;

		InvalidateRect(rcInvalid,FALSE);
	}
	else
		Invalidate(FALSE);
}

void CALXSyntaxEditCtrl::Init()
{
	SetDefaultCharFormat(m_cfDefault);
	SetEventMask(GetEventMask() | ENM_CHANGE | ENM_PROTECTED);
}

void CALXSyntaxEditCtrl::Colorize(LPCSTR lpszBuf, int iOffset)
{
	int iStart;
	int i = 0;

	//do the work
	while(lpszBuf[i] != '\0')
	{
		switch(lpszBuf[i])
		{
		case '\r': case '\n':
			break;
		case '\0':
			continue;
		case '(':
			{
			iStart = (i++);
			LPCSTR lpszTemp = &lpszBuf[i];
			int nLevel = 0;
			while(*lpszTemp != '\0' && *lpszTemp != '\r' && *lpszTemp != '\n')
			{
				if(*lpszTemp == ')')
					if(nLevel == 0)
						break;
					else
						nLevel--;
				else if(*lpszTemp == '(')
					nLevel++;

				lpszTemp++;
			}

			if(*lpszTemp != ')' ||  nLevel != 0)
				SetFormatRange(iStart+iOffset,iStart+iOffset+1, TXT_CLR_ERROR);
			else
				SetFormatRange(iStart+iOffset,iStart+iOffset+1, TXT_CLR_DEFAULT);

			continue;
			}
		case ')':
			{
			LPCSTR lpszTemp = &lpszBuf[i-1];
			iStart = (i++);
			int nLevel = 0;
			while(lpszTemp != lpszBuf && *lpszTemp != '\r' && *lpszTemp != '\n')
			{
				if(*lpszTemp == '(')
					if(nLevel == 0)
						break;
					else
						nLevel--;
				else if(*lpszTemp == ')')
					nLevel++;

				lpszTemp--;
			}

			if(*lpszTemp != '(' ||  nLevel != 0)
				SetFormatRange(iStart+iOffset,iStart+iOffset+1, TXT_CLR_ERROR);
			else
				SetFormatRange(iStart+iOffset,iStart+iOffset+1, TXT_CLR_DEFAULT);

			continue;
			}
		case '"':
			iStart = (i++);
			while(lpszBuf[i] != '"' && lpszBuf[i] != '\0' && lpszBuf[i] != '\r' && lpszBuf[i] != '\n')
				i++;
			if(lpszBuf[i] == '"')
			{
				i++;
				SetFormatRange(iStart+iOffset,i+iOffset, TXT_CLR_STRING);
			}
			else
				SetFormatRange(iStart+iOffset,iStart+iOffset+1, TXT_CLR_ERROR);

			continue;
		case '\'':
			iStart = (i++);
			while(lpszBuf[i] != '\'' && lpszBuf[i] != '\0' && lpszBuf[i] != '\r' && lpszBuf[i] != '\n')
				i++;
			if(lpszBuf[i] == '\'')
			{
				i++;
				SetFormatRange(iStart+iOffset,i+iOffset, TXT_CLR_STRING);
			}
			else
				SetFormatRange(iStart+iOffset,iStart+iOffset+1, TXT_CLR_ERROR);

			continue;
		case '{':
			iStart = (i++);
			while(lpszBuf[i] != '{' && lpszBuf[i] != '}' && lpszBuf[i] != '\0' && lpszBuf[i] != '\r' && lpszBuf[i] != '\n')
				i++;
			if(lpszBuf[i] == '}')
			{
				i++;
				SetFormatRange(iStart+iOffset,i+iOffset, TXT_CLR_DATE);
			}
			else
				SetFormatRange(iStart+iOffset,iStart+iOffset+1, TXT_CLR_ERROR);

			continue;
		case '}':
			iStart = (i++);
			SetFormatRange(iStart+iOffset,iStart+iOffset+1, TXT_CLR_ERROR);
			continue;
		case '.':
			iStart = (i++);
			if(lpszBuf[i] >= '0' && lpszBuf[i] <= '9')
			{
				while(lpszBuf[i] >= '0' && lpszBuf[i] <= '9')
					i++;

				SetFormatRange(iStart+iOffset,i+iOffset, TXT_CLR_NUM);

				continue;
			}
			else if((lpszBuf[i] >= 'A' && lpszBuf[i] <= 'Z') || (lpszBuf[i] >= 'a' && lpszBuf[i] <= 'z'))
			{
				i++;
				while((lpszBuf[i] >= 'A' && lpszBuf[i] <= 'Z') || (lpszBuf[i] >= 'a' && lpszBuf[i] <= 'z'))
					i++;

				if(lpszBuf[i] == '.')
				{
					int nLen = i - iStart - 1;
					LPSTR lpszTemp = new CHAR[nLen + 1];
					memcpy(lpszTemp, &lpszBuf[iStart + 1], nLen); lpszTemp[nLen] = '\0';

					if(alxIsBoolExpr(lpszTemp))
					{
						i++;
						SetFormatRange(iStart+iOffset,i+iOffset, TXT_CLR_BOOL);
					}
					else if(alxIsLogicOprExpr(lpszTemp))
					{
						i++;
						SetFormatRange(iStart+iOffset,i+iOffset, TXT_CLR_LOGIC_OPR);
					}

					delete[] lpszTemp;
				}
				else
				{
					SetFormatRange(iStart+iOffset,iStart+iOffset+1, TXT_CLR_ERROR);
					i++;
					SetFormatRange(iStart+iOffset+1,i+iOffset, TXT_CLR_DEFAULT);
				}

			}
			else
				SetFormatRange(iStart+iOffset,i+iOffset, TXT_CLR_ERROR);

			continue;
		default:
			// function, variable or operator
			if((lpszBuf[i] >= 'a' && lpszBuf[i] <= 'z') || (lpszBuf[i] >= 'A' && lpszBuf[i] <= 'Z') || lpszBuf[i] == '_')
			{
				iStart = (i++);
				while((lpszBuf[i] >= 'a' && lpszBuf[i] <= 'z') || (lpszBuf[i] >= 'A' && lpszBuf[i] <= 'Z') || (lpszBuf[i] >= '0' && lpszBuf[i] <= '9') || lpszBuf[i] == '_')
					i++;

				int j = 0;
				int comlen = max(4,i-iStart);

				LPSTR szTmp = new CHAR[i-iStart+1];
				strncpy(szTmp, lpszBuf + iStart, i-iStart);
				szTmp[i-iStart] = '\0';

				// function
				if(lpszBuf[i] == '(')
				{
					// touch functions
					while(j < alxFuncCount)
					{
						// if it is that function
						if(_strnicmp(m_alxFunList[j].m_szName, szTmp, comlen) == 0)
						{
							SetFormatRange(iStart+iOffset,i+iOffset, TXT_CLR_FUNCTION);
							break;
						}
						j++;
					}

					if(j == alxFuncCount) // if not found
					{
						if(alxIsLogicOprExpr(szTmp))
							SetFormatRange(iStart+iOffset,i+iOffset, TXT_CLR_LOGIC_OPR);
						else
							SetFormatRange(iStart+iOffset,i+iOffset, TXT_CLR_DEFAULT);
					}
				}
				else if(alxIsLogicOprExpr(szTmp))
					SetFormatRange(iStart+iOffset,i+iOffset, TXT_CLR_LOGIC_OPR);
				else
					SetFormatRange(iStart+iOffset,i+iOffset, TXT_CLR_DEFAULT);

				delete[] szTmp;

				continue;
			}
			else if(lpszBuf[i] >= '0' && lpszBuf[i] <= '9')
			{
				iStart = (i++);
				BOOL bDot = FALSE;
				while(lpszBuf[i] >= '0' && lpszBuf[i] <= '9' || lpszBuf[i] == '.')
				{
					if(lpszBuf[i] == '.')
						if(bDot)
							break;
						else
							bDot = TRUE;
					i++;
				}
				SetFormatRange(iStart+iOffset,i+iOffset, TXT_CLR_NUM);
				continue;
			}
			else
			{
				iStart = (i++);
				SetFormatRange(iStart+iOffset,i+iOffset, TXT_CLR_DEFAULT);
				continue;
			}

		}
		i++;
	}
}

void CALXSyntaxEditCtrl::SetFormatRange(int nStart, int nEnd, COLORREF clr)
{
	SetSel(nStart, nEnd);

	CHARFORMAT cfm;
	cfm.cbSize = sizeof(cfm);
    GetSelectionCharFormat(cfm);
	cfm.dwMask |= CFM_COLOR;
	cfm.crTextColor = clr;
	SetSelectionCharFormat(cfm);
	
/*	CString strTrace;
	strTrace.Format("Start = %d, End = %d \n",nStart, nEnd);
	TRACE(strTrace);
*/
//	if(cfm.crTextColor != clr)
//	{
//		cfm.crTextColor = clr;
//		SetSelectionCharFormat(cfm);
//	}
}

void CALXSyntaxEditCtrl::OnProtected(NMHDR* pNMHDR, LRESULT* pResult)
{
	CHARRANGE cr;
	GetSel(cr);
	// get first select line
	m_nPrevLine = LineFromChar(cr.cpMin);
}