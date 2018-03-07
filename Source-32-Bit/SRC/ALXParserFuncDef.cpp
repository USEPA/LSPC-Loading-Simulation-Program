/////////////////////////////////////////////////////////////////////////////
//
// FuncDef.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
/*
 Copyright© 2002, Alexey Dolgachov
 mail:	alxsoft@gazinter.net	
 web:	http://www.alxsoft.narod.ru
*/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../INCLUDE/ALXParserFuncDef.h"
#include "../INCLUDE/ALXParserException.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FIRST_ARG		0
#define SECOND_ARG		1
#define THIRD_ARG		2
#define FOURTH_ARG		3
#define FIFTH_ARG		4
#define SIXTH_ARG		5

#define ANY_ARG_COUNT	-1

#define MIN_DATE                (-657434L)  // about year 100
#define MAX_DATE                2958465L    // about year 9999

/////////////////////////////////////////////////////////////////////////////
// Arguments by default definition

// FUNCTION(nExpression)
ALXPRS_DEF_ARGUMENT arArgNum1[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_R8,			"",				//	nExpression
};

// FUNCTION(dExpression)
ALXPRS_DEF_ARGUMENT arArgDate1[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_DATE,		"",				//	dExpression
};

// FUNCTION(cExpression)
ALXPRS_DEF_ARGUMENT arArgStr1[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_BSTR,		"",				//	cExpression
};

// FUNCTION(eExpression)
ALXPRS_DEF_ARGUMENT arArgUnk1[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_UNKNOWN,		"",				//	eExpression
};

// AT(cSearchExpression, cExpressionSearched [, nOccurrence])
// ATC(cSearchExpression, cExpressionSearched [, nOccurrence])
ALXPRS_DEF_ARGUMENT arArgAT[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_BSTR,		"",				//	cSearchExpression
	VT_BSTR,		"",				//	cExpressionSearched
	VT_R8,			"1",			//	nOccurrence
};

// BETWEEN(eTestValue, eLowValue, eHighValue)
ALXPRS_DEF_ARGUMENT arArgBETWEEN[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_UNKNOWN,		"",				//	eTestValue
	VT_UNKNOWN,		"",				//	eLowValue
	VT_UNKNOWN,		"",				//	eHighValue
};

// CHRTRAN(cSearchedExpression, cSearchExpression, cReplacementExpression)
ALXPRS_DEF_ARGUMENT arArgCHRTRAN[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_BSTR,		"",				//	cSearchedExpression
	VT_BSTR,		"",				//	cSearchExpression
	VT_BSTR,		"",				//	cReplacementExpression
};

// DATE([nYear, nMonth, nDay])
ALXPRS_DEF_ARGUMENT arArgDATE[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_R8,			"0",			//	nYear
	VT_R8,			"0",			//	nMonth
	VT_R8,			"0",			//	nDay
};

// DATETIME([nYear, nMonth, nDay [, nHours [, nMinutes [, nSeconds]]]])
ALXPRS_DEF_ARGUMENT arArgDATETIME[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_R8,			"0",			//	nYear
	VT_R8,			"0",			//	nMonth
	VT_R8,			"0",			//	nDay
	VT_R8,			"0",			//	nHours
	VT_R8,			"0",			//	nMinutes
	VT_R8,			"0",			//	nSeconds
};

// DTOC(dExpression [, 1])
ALXPRS_DEF_ARGUMENT arArgDTOC[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_DATE,		"",				//	dExpression
	VT_R8,			"0",			//	1
};

// GOMONTH(dExpression, nNumberOfMonths)
ALXPRS_DEF_ARGUMENT arArgGOMONTH[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_DATE,		"",				//	dExpression
	VT_R8,			"",				//	nNumberOfMonths
};

// IIF(lExpression, eExpression1, eExpression2)
ALXPRS_DEF_ARGUMENT arArgIIF[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_BOOL,		"",				//	lExpression
	VT_UNKNOWN,		"",				//	eExpression1
	VT_UNKNOWN,		"",				//	eExpression2
};

// LEFT(cExpression, nCharacters)
// RIGHT(cExpression, nCharacters)
ALXPRS_DEF_ARGUMENT arArgLEFT_RIGHT[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_BSTR,		"",				//	cExpression
	VT_R8,			"",				//	nCharacters
};

// LIKE(cExpression1, cExpression2)
ALXPRS_DEF_ARGUMENT arArgLIKE[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_BSTR,		"",				//	cExpression1
	VT_BSTR,		"",				//	cExpression2
};

// MAX(eExpression1, eExpression2 [, eExpression3 ...])
// MIN(eExpression1, eExpression2 [, eExpression3 ...])
ALXPRS_DEF_ARGUMENT arArgMAX_MIN[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_UNKNOWN,			"",				//	eExpression1
};

// MOD(nDividend, nDivisor)
ALXPRS_DEF_ARGUMENT arArgMOD[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_R8,			"",				//	nDividend
	VT_R8,			"",				//	nDivisor
};

// OCCURS(cSearchExpression, cExpressionSearched)
ALXPRS_DEF_ARGUMENT arArgOCCURS[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_BSTR,		"",				//	cExpression
	VT_BSTR,		"",				//	cExpressionSearched
};

// PADC(eExpression, nResultSize [, cPadCharacter])
// PADL(eExpression, nResultSize [, cPadCharacter])
// PADR(eExpression, nResultSize [, cPadCharacter])
ALXPRS_DEF_ARGUMENT arArgPAD[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_UNKNOWN,		"",				//	eExpression
	VT_R8,			"",				//	nResultSize
	VT_BSTR,		" ",			//	cPadCharacter
};

// REPLICATE(cExpression, nTimes)
ALXPRS_DEF_ARGUMENT arArgREPLICATE[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_BSTR,		"",				//	cExpression
	VT_R8,			"",				//	nTimes
};

// ROUND(nExpression, nDecimalPlaces)
ALXPRS_DEF_ARGUMENT arArgROUND[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_R8,			"",				//	nExpression
	VT_R8,			"",				//	nDecimalPlaces
};

// STR(nExpression [, nLength [, nDecimalPlaces]])
ALXPRS_DEF_ARGUMENT arArgSTR[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_R8,			"",				//	nExpression
	VT_R8,			"10",			//	nLength	
	VT_R8,			"0",			//	nDecimalPlaces
};

// STRTRAN(cSearched, cSearchFor [, cReplacement] [, nStartOccurrence] [, nNumberOfOccurrences])
ALXPRS_DEF_ARGUMENT arArgSTRTRAN[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_BSTR,		"",				//	cSearched
	VT_BSTR,		"",				//	cSearchFor	
	VT_BSTR,		"",				//	cReplacement
	VT_R8,			"1",			//	nStartOccurrence
	VT_R8,			"-1",			//	nNumberOfOccurrences
};

// STUFF(cExpression, nStartReplacement, nCharactersReplaced, cReplacement)
ALXPRS_DEF_ARGUMENT arArgSTUFF[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_BSTR,		"",				//	cExpression
	VT_R8,			"",				//	nStartReplacement	
	VT_R8,			"",				//	nCharactersReplaced
	VT_BSTR,		"",				//	cReplacement
};

// SUBSTR(cExpression, nStartPosition [, nCharactersReturned])
ALXPRS_DEF_ARGUMENT arArgSUBSTR[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_BSTR,		"",				//	cExpression
	VT_R8,			"1",			//	nStartPosition	
	VT_R8,			"-1",			//	nCharactersReturned
};

// VARTYPE(eExpression [, lNullDataType])
ALXPRS_DEF_ARGUMENT arArgVARTYPE[] =
{
//	TypeArgument	DefaultValue	ArgumentName
	VT_UNKNOWN,		"",				//	eExpression
	VT_BOOL,		".F.",			//	lNullDataType - unused
};

/////////////////////////////////////////////////////////////////////////////
// List function

ALXPRS_DEF_FUNCTION alxFunList[] =
{
//	FunctionName	FunctionPointer	TypeResult	ArgumentCount	MinAgrumentCount	DefaultArgumentList
	"ABS",			funABS,			VT_R8,		1,				1,					arArgNum1,
	"ALLTRIM",		funALLTRIM,		VT_BSTR,	1,				1,					arArgStr1,
	"ANSITOOEM",	funANSITOOEM,	VT_BSTR,	1,				1,					arArgStr1,
	"ASC",			funASC,			VT_R8,		1,				1,					arArgStr1,
	"AT",			funAT,			VT_R8,		3,				2,					arArgAT,
	"ATAN",			funATAN,		VT_R8,		1,				1,					arArgNum1,
	"ATC",			funATC,			VT_R8,		3,				2,					arArgAT,
	"BETWEEN",		funBETWEEN,		VT_BOOL,	3,				3,					arArgBETWEEN,
	"CDOW",			funCDOW,		VT_BSTR,	1,				1,					arArgDate1,
	"CEILING",		funCEILING,		VT_R8,		1,				1,					arArgNum1,
	"CHR",			funCHR,			VT_BSTR,	1,				1,					arArgNum1,
	"CHRTRAN",		funCHRTRAN,		VT_BSTR,	3,				3,					arArgCHRTRAN,
	"CMONTH",		funCMONTH,		VT_BSTR,	1,				1,					arArgDate1,
	"COS",			funCOS,			VT_R8,		1,				1,					arArgNum1,
	"CTOD",			funCTOT,		VT_DATE,	1,				1,					arArgStr1,
	"CTOT",			funCTOT,		VT_DATE,	1,				1,					arArgStr1,
	"DATE",			funDATE,		VT_DATE,	3,				0,					arArgDATE,
	"DATETIME",		funDATETIME,	VT_DATE,	6,				0,					arArgDATETIME,
	"DAY",			funDAY,			VT_R8,		1,				1,					arArgDate1,
	"DMY",			funDMY,			VT_BSTR,	1,				1,					arArgDate1,
	"DTOC",			funDTOC,		VT_BSTR,	2,				1,					arArgDTOC,
	"DTOS",			funDTOS,		VT_BSTR,	1,				1,					arArgDate1,
	"EMPTY",		funEMPTY,		VT_BOOL,	1,				1,					arArgUnk1,
	"EXP",			funEXP,			VT_R8,		1,				1,					arArgNum1,
	"FLOOR",		funFLOOR,		VT_R8,		1,				1,					arArgNum1,
	"GOMONTH",		funGOMONTH,		VT_DATE,	2,				2,					arArgGOMONTH,
	"HOUR",			funHOUR,		VT_R8,		1,				1,					arArgDate1,
	"IIF",			funIIF,			VT_UNKNOWN,	3,				3,					arArgIIF,
	"INT",			funINT,			VT_R8,		1,				1,					arArgNum1,
	"ISALPHA",		funISALPHA,		VT_BOOL,	1,				1,					arArgStr1,
	"ISDIGIT",		funISDIGIT,		VT_BOOL,	1,				1,					arArgStr1,
	"ISLOWER",		funISLOWER,		VT_BOOL,	1,				1,					arArgStr1,
	"ISUPPER",		funISUPPER,		VT_BOOL,	1,				1,					arArgStr1,
	"LEFT",			funLEFT,		VT_BSTR,	2,				2,					arArgLEFT_RIGHT,
	"LEN",			funLEN,			VT_R8,		1,				1,					arArgStr1,
	"LIKE",			funLIKE,		VT_BOOL,	2,				2,					arArgLIKE,
	"LOG",			funLOG,			VT_R8,		1,				1,					arArgNum1,
	"LOG10",		funLOG10,		VT_R8,		1,				1,					arArgNum1,
	"LOWER",		funLOWER,		VT_BSTR,	1,				1,					arArgStr1,
	"LTRIM",		funLTRIM,		VT_BSTR,	1,				1,					arArgStr1,
	"MAX",			funMAX,			VT_UNKNOWN,	ANY_ARG_COUNT,	2,					arArgMAX_MIN,
	"MDY",			funMDY,			VT_BSTR,	1,				1,					arArgDate1,
	"MIN",			funMIN,			VT_UNKNOWN,	ANY_ARG_COUNT,	2,					arArgMAX_MIN,
	"MINUTE",		funMINUTE,		VT_R8,		1,				1,					arArgDate1,
	"MOD",			funMOD,			VT_R8,		2,				2,					arArgMOD,
	"MONTH",		funMONTH,		VT_R8,		1,				1,					arArgDate1,
	"OCCURS",		funOCCURS,		VT_R8,		2,				2,					arArgOCCURS,
	"OEMTOANSI",	funOEMTOANSI,	VT_BSTR,	1,				1,					arArgStr1,
	"PADC",			funPADC,		VT_BSTR,	3,				2,					arArgPAD,
	"PADL",			funPADL,		VT_BSTR,	3,				2,					arArgPAD,
	"PADR",			funPADR,		VT_BSTR,	3,				2,					arArgPAD,
	"PI",			funPI,			VT_R8,		0,				0,					NULL,
	"REPLICATE",	funREPLICATE,	VT_BSTR,	2,				2,					arArgREPLICATE,
	"RIGHT",		funRIGHT,		VT_BSTR,	2,				2,					arArgLEFT_RIGHT,
	"ROUND",		funROUND,		VT_R8,		2,				2,					arArgROUND,
	"RTRIM",		funRTRIM,		VT_BSTR,	1,				1,					arArgStr1,
	"SEC",			funSEC,			VT_R8,		1,				1,					arArgDate1,
	"SECONDS",		funSECONDS,		VT_R8,		0,				0,					NULL,
	"SIGN",			funSIGN,		VT_R8,		1,				1,					arArgNum1,
	"SIN",			funSIN,			VT_R8,		1,				1,					arArgNum1,
	"SPACE",		funSPACE,		VT_BSTR,	1,				1,					arArgNum1,
	"SQRT",			funSQRT,		VT_R8,		1,				1,					arArgNum1,
	"STR",			funSTR,			VT_BSTR,	3,				1,					arArgSTR,
	"STRTRAN",		funSTRTRAN,		VT_BSTR,	5,				2,					arArgSTRTRAN,
	"STUFF",		funSTUFF,		VT_BSTR,	4,				4,					arArgSTUFF,
	"SUBSTR",		funSUBSTR,		VT_BSTR,	3,				2,					arArgSUBSTR,
	"TAN",			funTAN,			VT_R8,		1,				1,					arArgNum1,
	"TIME",			funTIME,		VT_BSTR,	1,				0,					arArgNum1,
	"TRIM",			funRTRIM,		VT_BSTR,	1,				1,					arArgStr1,
	"UPPER",		funUPPER,		VT_BSTR,	1,				1,					arArgStr1,
	"VAL",			funVAL,			VT_R8,		1,				1,					arArgStr1,
	"VARTYPE",		funVARTYPE,		VT_BSTR,	2,				1,					arArgVARTYPE,
	"YEAR",			funYEAR,		VT_R8,		1,				1,					arArgDate1,
};

int alxFuncCount = sizeof(alxFunList)/sizeof(ALXPRS_DEF_FUNCTION);

/////////////////////////////////////////////////////////////////////////////
// Functions realization

// ABS(nExpression)
void FAR funABS(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_R8(pvarResult) = fabs(V_R8(pvarArguments[FIRST_ARG]));
	V_VT(pvarResult) = VT_R8;
}

// ALLTRIM(cExpression)
void FAR funALLTRIM(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult(V_BSTR(pvarArguments[FIRST_ARG]));
	strResult.TrimRight();
	strResult.TrimLeft();
	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// ANSITOOEM(cExpression)
void FAR funANSITOOEM(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult(V_BSTR(pvarArguments[FIRST_ARG]));
	strResult.AnsiToOem();

	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// ASC(cExpression)
void FAR funASC(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString cExpression(V_BSTR(pvarArguments[FIRST_ARG]));
	if(cExpression.IsEmpty())
		V_R8(pvarResult) = 0;
	else
		V_R8(pvarResult) = cExpression[0];

	V_VT(pvarResult) = VT_R8;
}

// AT(cSearchExpression, cExpressionSearched [, nOccurrence])
void FAR funAT(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString cSearchExpression(V_BSTR(pvarArguments[FIRST_ARG]));
	CString cExpressionSearched(V_BSTR(pvarArguments[SECOND_ARG]));
	int nOccurrence = (int)V_R8(pvarArguments[THIRD_ARG]);
	int nPos = 0;
	while(nOccurrence > 0)
	{
		nPos = cExpressionSearched.Find(cSearchExpression, nPos) + 1;
		if(nPos < 0)
			break;
		nOccurrence--;
	}
	V_R8(pvarResult) = nPos;
	V_VT(pvarResult) = VT_R8;
}

// ATAN(nExpression)
void FAR funATAN(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_R8(pvarResult) = atan(V_R8(pvarArguments[FIRST_ARG]));
	V_VT(pvarResult) = VT_R8;
}

// ATC(cSearchExpression, cExpressionSearched [, nOccurrence])
void FAR funATC(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString cSearchExpression(V_BSTR(pvarArguments[FIRST_ARG]));
	CString cExpressionSearched(V_BSTR(pvarArguments[SECOND_ARG]));
	int nOccurrence = (int)V_R8(pvarArguments[THIRD_ARG]);
	cSearchExpression.MakeUpper();
	cExpressionSearched.MakeUpper();
	int nPos = 0;
	while(nOccurrence > 0)
	{
		nPos = cExpressionSearched.Find(cSearchExpression, nPos) + 1;
		if(nPos < 0)
			break;
		nOccurrence--;
	}
	V_R8(pvarResult) = nPos;
	V_VT(pvarResult) = VT_R8;
}

// BETWEEN(eTestValue, eLowValue, eHighValue)
void FAR funBETWEEN(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	switch(V_VT(pvarArguments[FIRST_ARG]))
	{
	case VT_R8:
		if(V_R8(pvarArguments[FIRST_ARG]) > V_R8(pvarArguments[SECOND_ARG]) && V_R8(pvarArguments[FIRST_ARG]) < V_R8(pvarArguments[THIRD_ARG]))
			V_BOOL(pvarResult) = AFX_OLE_TRUE;
		else
			V_BOOL(pvarResult) = AFX_OLE_FALSE;
		break;
	case VT_BSTR:
		{
			CString eTestValue(V_BSTR(pvarArguments[FIRST_ARG]));
			CString eLowValue(V_BSTR(pvarArguments[SECOND_ARG]));
			CString eHighValue(V_BSTR(pvarArguments[THIRD_ARG]));
			if(eTestValue > eLowValue && eTestValue < eHighValue)
				V_BOOL(pvarResult) = AFX_OLE_TRUE;
			else
				V_BOOL(pvarResult) = AFX_OLE_FALSE;
		break;
		}
	case VT_DATE:
		if(V_DATE(pvarArguments[FIRST_ARG]) > V_DATE(pvarArguments[SECOND_ARG]) && V_DATE(pvarArguments[FIRST_ARG]) < V_DATE(pvarArguments[THIRD_ARG]))
			V_BOOL(pvarResult) = AFX_OLE_TRUE;
		else
			V_BOOL(pvarResult) = AFX_OLE_FALSE;
		break;
	case VT_BOOL:
		if(V_BOOL(pvarArguments[FIRST_ARG]) > V_BOOL(pvarArguments[SECOND_ARG]) && V_BOOL(pvarArguments[FIRST_ARG]) < V_BOOL(pvarArguments[THIRD_ARG]))
			V_BOOL(pvarResult) = AFX_OLE_TRUE;
		else
			V_BOOL(pvarResult) = AFX_OLE_FALSE;
		break;
	default:
		V_BOOL(pvarResult) = AFX_OLE_FALSE;
	}

	V_VT(pvarResult) = VT_BOOL;
}

// CDOW(dExpression)
void FAR funCDOW(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dte(V_DATE(pvarArguments[FIRST_ARG]));
	V_BSTR(pvarResult) = dte.Format("%A").AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// CEILING(nExpression)
void FAR funCEILING(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_R8(pvarResult) = ceil(V_R8(pvarArguments[FIRST_ARG]));
	V_VT(pvarResult) = VT_R8;
}

// CHR(nANSICode)
void FAR funCHR(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult((TCHAR)V_R8(pvarArguments[FIRST_ARG]));

	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// CHRTRAN(cSearchedExpression, cSearchExpression, cReplacementExpression)
void FAR funCHRTRAN(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult;
	CString cSearchedExpression(V_BSTR(pvarArguments[FIRST_ARG]));
	CString cSearchExpression(V_BSTR(pvarArguments[SECOND_ARG]));
	CString cReplacementExpression(V_BSTR(pvarArguments[THIRD_ARG]));

	for(int i = 0; i < cSearchedExpression.GetLength(); i++)
	{
		int nPos = cSearchExpression.Find(cSearchedExpression[i]);
		if(nPos < 0)
			strResult += cSearchedExpression[i];
		else if(nPos < cReplacementExpression.GetLength())
			strResult += cReplacementExpression[nPos];
	}

	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// CMONTH(dExpression)
void FAR funCMONTH(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dExpression(V_DATE(pvarArguments[FIRST_ARG]));

	V_BSTR(pvarResult) = dExpression.Format("%B").AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// COS(nExpression)
void FAR funCOS(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_R8(pvarResult) = cos(V_R8(pvarArguments[FIRST_ARG]));
	V_VT(pvarResult) = VT_R8;
}


// CTOT(cCharacterExpression)
void FAR funCTOT(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dteResult;
	CString cCharacterExpression(V_BSTR(pvarArguments[FIRST_ARG]));
	dteResult.ParseDateTime(cCharacterExpression);

	V_DATE(pvarResult) = dteResult;
	V_VT(pvarResult) = VT_DATE;
}

// DATE([nYear, nMonth, nDay])
void FAR funDATE(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dteResult;
	int nYear = (int)V_R8(pvarArguments[FIRST_ARG]);
	int nMonth = (int)V_R8(pvarArguments[SECOND_ARG]);
	int nDay = (int)V_R8(pvarArguments[THIRD_ARG]);

	if(nYear == 0 && nMonth == 0 && nDay == 0)
	{
		dteResult = COleDateTime::GetCurrentTime();
		dteResult.SetDate(dteResult.GetYear(), dteResult.GetMonth(), dteResult.GetDay());
	}
	else
		dteResult.SetDate(nYear, nMonth, nDay);

	if(dteResult.GetStatus() != COleDateTime::valid)
		ALXThrowFunOperException(ALX_PRS_ERROR_INVALID_ARGUMENT, "DATE");

	V_DATE(pvarResult) = dteResult;
	V_VT(pvarResult) = VT_DATE;
}

// DATETIME([nYear, nMonth, nDay [, nHours [, nMinutes [, nSeconds]]]])
void FAR funDATETIME(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dteResult;
	int nYear = (int)V_R8(pvarArguments[FIRST_ARG]);
	int nMonth = (int)V_R8(pvarArguments[SECOND_ARG]);
	int nDay = (int)V_R8(pvarArguments[THIRD_ARG]);
	int nHours = (int)V_R8(pvarArguments[FOURTH_ARG]);
	int nMinutes = (int)V_R8(pvarArguments[FIFTH_ARG]);
	int nSeconds = (int)V_R8(pvarArguments[SIXTH_ARG]);

	if(nYear == 0 && nMonth == 0 && nDay == 0)
		if(nHours == 0 && nMinutes == 0 && nSeconds == 0)
			dteResult = COleDateTime::GetCurrentTime();
		else
		{
			dteResult = COleDateTime::GetCurrentTime();
			dteResult.SetDateTime(dteResult.GetYear(), dteResult.GetMonth(), dteResult.GetDay(), nHours, nMinutes, nSeconds);
		}
	else
		dteResult.SetDateTime(nYear, nMonth, nDay, nHours, nMinutes, nSeconds);

	if(dteResult.GetStatus() != COleDateTime::valid)
		ALXThrowFunOperException(ALX_PRS_ERROR_INVALID_ARGUMENT, "DATETIME");

	V_DATE(pvarResult) = dteResult;
	V_VT(pvarResult) = VT_DATE;
}

// DAY(dExpression)
void FAR funDAY(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dte(V_DATE(pvarArguments[FIRST_ARG]));
	V_R8(pvarResult) = dte.GetDay();
	V_VT(pvarResult) = VT_R8;
}

// DMY(dExpression)
void FAR funDMY(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dte(V_DATE(pvarArguments[FIRST_ARG]));
	V_BSTR(pvarResult) = dte.Format("%d %B %y").AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// DTOC(dExpression [, 1])
void FAR funDTOC(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dte(V_DATE(pvarArguments[FIRST_ARG]));
	if(V_R8(pvarArguments[SECOND_ARG]) != 1)
		V_BSTR(pvarResult) = dte.Format().AllocSysString();
	else
		V_BSTR(pvarResult) = dte.Format("%Y%m%d").AllocSysString();

	V_VT(pvarResult) = VT_BSTR;
}

// DTOS(dExpression)
void FAR funDTOS(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dte(V_DATE(pvarArguments[FIRST_ARG]));
	V_BSTR(pvarResult) = dte.Format("%Y%m%d").AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// EMPTY(eExpression)
void FAR funEMPTY(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	switch(V_VT(pvarArguments[FIRST_ARG]))
	{
	case VT_R8:
		V_BOOL(pvarResult) = V_R8(pvarArguments[FIRST_ARG]) == 0 ? AFX_OLE_TRUE : AFX_OLE_FALSE;
		break;
	case VT_BSTR:
		{
			CString strExpr(V_BSTR(pvarArguments[FIRST_ARG]));
			V_BOOL(pvarResult) = strExpr.IsEmpty() ? AFX_OLE_TRUE : AFX_OLE_FALSE;
		break;
		}
	case VT_BOOL:
		V_BOOL(pvarResult) = V_BOOL(pvarArguments[FIRST_ARG]) == AFX_OLE_FALSE ? AFX_OLE_TRUE : AFX_OLE_FALSE;
		break;
	case VT_DATE:
		{
			if(V_DATE(pvarArguments[FIRST_ARG]) == 0 || V_DATE(pvarArguments[FIRST_ARG]) > MAX_DATE || V_DATE(pvarArguments[FIRST_ARG]) < MIN_DATE) // about year 100 to about 9999
				V_BOOL(pvarResult) = AFX_OLE_TRUE;
			else
				V_BOOL(pvarResult) = AFX_OLE_FALSE;
		break;
		}
	default:
		V_BOOL(pvarResult) = AFX_OLE_FALSE;
	}
	V_VT(pvarResult) = VT_BOOL;
}

// EXP(nExpression)
void FAR funEXP(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_R8(pvarResult) = exp(V_R8(pvarArguments[FIRST_ARG]));
	V_VT(pvarResult) = VT_R8;
}

// FLOOR(nExpression)
void FAR funFLOOR(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_R8(pvarResult) = floor(V_R8(pvarArguments[FIRST_ARG]));
	V_VT(pvarResult) = VT_R8;
}

// GOMONTH(dExpression, nNumberOfMonths)
void FAR funGOMONTH(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dExpression(V_DATE(pvarArguments[FIRST_ARG]));
	int nNumberOfMonths = (int) V_R8(pvarArguments[SECOND_ARG]);

	COleDateTimeSpan dteDays(dExpression.GetDay() - 1,0,0,0);
	COleDateTime dteBeginMonth(dExpression - dteDays);

	while(nNumberOfMonths != 0)
	{
		if(nNumberOfMonths > 0)
		{
			dteDays.SetDateTimeSpan(31,0,0,0);
			dteBeginMonth += dteDays;
			nNumberOfMonths--;
		}
		else
		{
			dteDays.SetDateTimeSpan(dteBeginMonth.GetDay(),0,0,0);
			dteBeginMonth -= dteDays;
			nNumberOfMonths++;
		}
		dteDays.SetDateTimeSpan(dteBeginMonth.GetDay() - 1,0,0,0);
		dteBeginMonth -= dteDays;
	}
	
	dteDays.SetDateTimeSpan(31,0,0,0);
	dteBeginMonth += dteDays;
	dteDays.SetDateTimeSpan(dteBeginMonth.GetDay(),0,0,0);
	COleDateTime dteEndMonth(dteBeginMonth - dteDays);

	COleDateTime dteResult(dteEndMonth.GetYear(), dteEndMonth.GetMonth(), min(dteEndMonth.GetDay(), dExpression.GetDay()),dExpression.GetHour(), dExpression.GetMinute(), dExpression.GetSecond());

	V_DATE(pvarResult) = dteResult;
	V_VT(pvarResult) = VT_DATE;
}

// HOUR(tExpression)
void FAR funHOUR(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dte(V_DATE(pvarArguments[FIRST_ARG]));
	V_R8(pvarResult) = dte.GetHour();
	V_VT(pvarResult) = VT_R8;
}

// IIF(lExpression, eExpression1, eExpression2)
void FAR funIIF(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	if(V_BOOL(pvarArguments[FIRST_ARG]))
		::VariantCopy(pvarResult, pvarArguments[SECOND_ARG]);
	else
		::VariantCopy(pvarResult, pvarArguments[THIRD_ARG]);
}

// INT(nExpression)
void FAR funINT(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_R8(pvarResult) = (int)V_R8(pvarArguments[FIRST_ARG]);
	V_VT(pvarResult) = VT_R8;
}

// ISALPHA(cExpression)
void FAR funISALPHA(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString cExpression(V_BSTR(pvarArguments[FIRST_ARG]));
	if(cExpression.IsEmpty())
		V_BOOL(pvarResult) = AFX_OLE_FALSE;
	else
		V_BOOL(pvarResult) = isalpha(cExpression[0]) ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

// ISDIGIT(cExpression)
void FAR funISDIGIT(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString cExpression(V_BSTR(pvarArguments[FIRST_ARG]));
	if(cExpression.IsEmpty())
		V_BOOL(pvarResult) = AFX_OLE_FALSE;
	else
		V_BOOL(pvarResult) = isdigit(cExpression[0]) ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

// ISLOWER(cExpression)
void FAR funISLOWER(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString cExpression(V_BSTR(pvarArguments[FIRST_ARG]));
	if(cExpression.IsEmpty())
		V_BOOL(pvarResult) = AFX_OLE_FALSE;
	else
	{
		CString strFirst(cExpression[0]);
		strFirst.MakeLower();
		V_BOOL(pvarResult) = strFirst[0] == cExpression[0] ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	}
	V_VT(pvarResult) = VT_BOOL;
}

// ISUPPER(cExpression)
void FAR funISUPPER(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString cExpression(V_BSTR(pvarArguments[FIRST_ARG]));
	if(cExpression.IsEmpty())
		V_BOOL(pvarResult) = AFX_OLE_FALSE;
	else
	{
		CString strFirst(cExpression[0]);
		strFirst.MakeUpper();
		V_BOOL(pvarResult) = strFirst[0] == cExpression[0] ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	}
	V_VT(pvarResult) = VT_BOOL;
}

// LEFT(cExpression, nCharacters)
void FAR funLEFT(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult(V_BSTR(pvarArguments[FIRST_ARG]));
	strResult = strResult.Left((int)V_R8(pvarArguments[SECOND_ARG]));

	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// LEN(cExpression)
void FAR funLEN(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult(V_BSTR(pvarArguments[FIRST_ARG]));

	V_R8(pvarResult) = strResult.GetLength();
	V_VT(pvarResult) = VT_R8;
}

// LIKE(cExpression1, cExpression2)
void FAR funLIKE(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString cExpression1(V_BSTR(pvarArguments[FIRST_ARG]));
	CString cExpression2(V_BSTR(pvarArguments[SECOND_ARG]));

	V_BOOL(pvarResult) = alxLike(cExpression1,cExpression2);
	V_VT(pvarResult) = VT_BOOL;
}

// LOG(nExpression)
void FAR funLOG(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	if(V_R8(pvarArguments[FIRST_ARG]) <= 0)
		ALXThrowFunOperException(ALX_PRS_ERROR_INVALID_ARGUMENT_LOG);
	else
		V_R8(pvarResult) = log(V_R8(pvarArguments[FIRST_ARG]));
	V_VT(pvarResult) = VT_R8;
}


// LOG10(nExpression)
void FAR funLOG10(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	if(V_R8(pvarArguments[FIRST_ARG]) <= 0)
		ALXThrowFunOperException(ALX_PRS_ERROR_INVALID_ARGUMENT_LOG10);
	else
		V_R8(pvarResult) = log10(V_R8(pvarArguments[FIRST_ARG]));
	V_VT(pvarResult) = VT_R8;
}

// LOWER(cExpression)
void FAR funLOWER(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult(V_BSTR(pvarArguments[FIRST_ARG]));
	strResult.MakeLower();
	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// LTRIM(cExpression)
void FAR funLTRIM(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult(V_BSTR(pvarArguments[FIRST_ARG]));
	strResult.TrimLeft();
	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// MAX(eExpression1, eExpression2 [, eExpression3 ...])
void FAR funMAX(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	switch(V_VT(pvarArguments[FIRST_ARG]))
	{
	case VT_R8:
		{
			V_R8(pvarResult) = V_R8(pvarArguments[FIRST_ARG]);
			int nArg = SECOND_ARG;
			while(pvarArguments[nArg] != NULL)
			{
				if(V_R8(pvarArguments[nArg]) > V_R8(pvarResult))
					V_R8(pvarResult) = V_R8(pvarArguments[nArg]);
				nArg++;
			}
			
			V_VT(pvarResult) = VT_R8;
			break;
		}
	case VT_BSTR:
		{
			CString strResult(V_BSTR(pvarArguments[FIRST_ARG]));
			int nArg = SECOND_ARG;
			while(pvarArguments[nArg] != NULL)
			{
				CString strArg(V_BSTR(pvarArguments[FIRST_ARG]));
				if(strResult > strArg)
					strResult = strArg;
				nArg++;
			}
			
			V_BSTR(pvarResult) = strResult.AllocSysString();
			V_VT(pvarResult) = VT_BSTR;
			break;
		}
	case VT_DATE:
		{
			V_DATE(pvarResult) = V_DATE(pvarArguments[FIRST_ARG]);
			int nArg = SECOND_ARG;
			while(pvarArguments[nArg] != NULL)
			{
				if(V_DATE(pvarArguments[nArg]) > V_DATE(pvarResult))
					V_DATE(pvarResult) = V_DATE(pvarArguments[nArg]);
				nArg++;
			}
			
			V_VT(pvarResult) = VT_DATE;
			break;
		}
	case VT_BOOL:
		{
			V_BOOL(pvarResult) = V_BOOL(pvarArguments[FIRST_ARG]);
			int nArg = SECOND_ARG;
			while(pvarArguments[nArg] != NULL)
			{
				if(V_BOOL(pvarArguments[nArg]) > V_BOOL(pvarResult))
					V_BOOL(pvarResult) = V_BOOL(pvarArguments[nArg]);
				nArg++;
			}
			
			V_VT(pvarResult) = VT_BOOL;
			break;
		}
	default:
		ALXThrowFunOperException(ALX_PRS_ERROR_ARGTYPE_MISMATCH, "MAX", 1);
	}
}

// MDY(dExpression)
void FAR funMDY(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dte(V_DATE(pvarArguments[FIRST_ARG]));
	V_BSTR(pvarResult) = dte.Format("%B %d, %y").AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// MIN(eExpression1, eExpression2 [, eExpression3 ...])
void FAR funMIN(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	switch(V_VT(pvarArguments[FIRST_ARG]))
	{
	case VT_R8:
		{
			V_R8(pvarResult) = V_R8(pvarArguments[FIRST_ARG]);
			int nArg = SECOND_ARG;
			while(pvarArguments[nArg] != NULL)
			{
				if(V_R8(pvarArguments[nArg]) < V_R8(pvarResult))
					V_R8(pvarResult) = V_R8(pvarArguments[nArg]);
				nArg++;
			}
			
			V_VT(pvarResult) = VT_R8;
			break;
		}
	case VT_BSTR:
		{
			CString strResult(V_BSTR(pvarArguments[FIRST_ARG]));
			int nArg = SECOND_ARG;
			while(pvarArguments[nArg] != NULL)
			{
				CString strArg(V_BSTR(pvarArguments[FIRST_ARG]));
				if(strResult < strArg)
					strResult = strArg;
				nArg++;
			}
			
			V_BSTR(pvarResult) = strResult.AllocSysString();
			V_VT(pvarResult) = VT_BSTR;
			break;
		}
	case VT_DATE:
		{
			V_DATE(pvarResult) = V_DATE(pvarArguments[FIRST_ARG]);
			int nArg = SECOND_ARG;
			while(pvarArguments[nArg] != NULL)
			{
				if(V_DATE(pvarArguments[nArg]) < V_DATE(pvarResult))
					V_DATE(pvarResult) = V_DATE(pvarArguments[nArg]);
				nArg++;
			}
			
			V_VT(pvarResult) = VT_DATE;
			break;
		}
	case VT_BOOL:
		{
			V_BOOL(pvarResult) = V_BOOL(pvarArguments[FIRST_ARG]);
			int nArg = SECOND_ARG;
			while(pvarArguments[nArg] != NULL)
			{
				if(V_BOOL(pvarArguments[nArg]) < V_BOOL(pvarResult))
					V_BOOL(pvarResult) = V_BOOL(pvarArguments[nArg]);
				nArg++;
			}
			
			V_VT(pvarResult) = VT_BOOL;
			break;
		}
	default:
		ALXThrowFunOperException(ALX_PRS_ERROR_ARGTYPE_MISMATCH, "MIN", 1);
	}
}

// MINUTE(tExpression)
void FAR funMINUTE(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dte(V_DATE(pvarArguments[FIRST_ARG]));
	V_R8(pvarResult) = dte.GetMinute();
	V_VT(pvarResult) = VT_R8;
}

// MOD(nDividend, nDivisor)
void FAR funMOD(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_R8(pvarResult) = fmod(V_R8(pvarArguments[FIRST_ARG]), V_R8(pvarArguments[SECOND_ARG]));
	V_VT(pvarResult) = VT_R8;
}

// MONTH(dExpression)
void FAR funMONTH(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dte(V_DATE(pvarArguments[FIRST_ARG]));
	V_R8(pvarResult) = dte.GetMonth();
	V_VT(pvarResult) = VT_R8;
}

// OCCURS(cSearchExpression, cExpressionSearched)
void FAR funOCCURS(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString cSearchExpression(V_BSTR(pvarArguments[FIRST_ARG]));
	CString cExpressionSearched(V_BSTR(pvarArguments[SECOND_ARG]));
	int nStart = 0;
	int nFindCount = 0;
	while(true)
	{
		nStart = cExpressionSearched.Find(cSearchExpression, nStart);
		if(nStart >= 0)
		{
			nStart += cSearchExpression.GetLength();
			nFindCount++;
		}
		else
			break;
	}

	V_R8(pvarResult) = nFindCount;
	V_VT(pvarResult) = VT_R8;
}

// OEMTOANSI(cExpression)
void FAR funOEMTOANSI(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult(V_BSTR(pvarArguments[FIRST_ARG]));
	strResult.OemToAnsi();

	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// PADC(eExpression, nResultSize [, cPadCharacter])
void FAR funPADC(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult;
	int nResultSize = (int) V_R8(pvarArguments[SECOND_ARG]);
	CString cPadCharacter(V_BSTR(pvarArguments[THIRD_ARG]));
	if(cPadCharacter.IsEmpty())
		// The third argument in function PADC() cannot be emty.
		ALXThrowFunOperException(ALX_PRS_ERROR_INVALID_ARGUMENT, "PADC");
	else
	{
		switch(V_VT(pvarArguments[FIRST_ARG]))
		{
		case VT_BSTR:
			strResult = V_BSTR(pvarArguments[FIRST_ARG]);
			break;
		case VT_R8:
			strResult.Format("%.0f",V_R8(pvarArguments[FIRST_ARG]));
			break;
		case VT_DATE:
			{
				COleDateTime dte(V_DATE(pvarArguments[FIRST_ARG]));
				strResult = dte.Format();
			break;
			}
		case VT_BOOL:
			strResult = V_BOOL(pvarArguments[FIRST_ARG]) == AFX_OLE_TRUE ? ".T." : ".F.";
			break;
		}

		while(strResult.GetLength() < nResultSize)
		{
			strResult += cPadCharacter;
			if(strResult.GetLength() < nResultSize)
				strResult = cPadCharacter + strResult; 
		}
		strResult.Left(nResultSize);
	}

	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// PADL(eExpression, nResultSize [, cPadCharacter])
void FAR funPADL(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult;
	int nResultSize = (int) V_R8(pvarArguments[SECOND_ARG]);
	CString cPadCharacter(V_BSTR(pvarArguments[THIRD_ARG]));
	if(cPadCharacter.IsEmpty())
		// The third argument in function PADL() cannot be emty.;
		ALXThrowFunOperException(ALX_PRS_ERROR_INVALID_ARGUMENT, "PADL");
	else
	{
		switch(V_VT(pvarArguments[FIRST_ARG]))
		{
		case VT_BSTR:
			strResult = V_BSTR(pvarArguments[FIRST_ARG]);
			break;
		case VT_R8:
			strResult.Format("%.0f",V_R8(pvarArguments[FIRST_ARG]));
			break;
		case VT_DATE:
			{
				COleDateTime dte(V_DATE(pvarArguments[FIRST_ARG]));
				strResult = dte.Format();
			break;
			}
		case VT_BOOL:
			strResult = V_BOOL(pvarArguments[FIRST_ARG]) == AFX_OLE_TRUE ? ".T." : ".F.";
			break;
		}

		if(strResult.GetLength() > nResultSize)
			strResult.Left(nResultSize);
		else
		{
			while(strResult.GetLength() < nResultSize)
				strResult = cPadCharacter + strResult;
			
			strResult.Right(nResultSize);
		}
	}

	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// PADR(eExpression, nResultSize [, cPadCharacter])
void FAR funPADR(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult;
	int nResultSize = (int) V_R8(pvarArguments[SECOND_ARG]);
	CString cPadCharacter(V_BSTR(pvarArguments[THIRD_ARG]));
	if(cPadCharacter.IsEmpty())
		// The third argument in function PADR() cannot be emty.
		ALXThrowFunOperException(ALX_PRS_ERROR_INVALID_ARGUMENT, "PADR");
	else
	{
		switch(V_VT(pvarArguments[FIRST_ARG]))
		{
		case VT_BSTR:
			strResult = V_BSTR(pvarArguments[FIRST_ARG]);
			break;
		case VT_R8:
			strResult.Format("%.0f",V_R8(pvarArguments[FIRST_ARG]));
			break;
		case VT_DATE:
			{
				COleDateTime dte(V_DATE(pvarArguments[FIRST_ARG]));
				strResult = dte.Format();
			break;
			}
		case VT_BOOL:
			strResult = V_BOOL(pvarArguments[FIRST_ARG]) == AFX_OLE_TRUE ? ".T." : ".F.";
			break;
		}

		while(strResult.GetLength() < nResultSize)
			strResult += cPadCharacter; 

		strResult.Left(nResultSize);
	}

	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// PI()
void FAR funPI(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_R8(pvarResult) = 3.1415926535;
	V_VT(pvarResult) = VT_R8;
}

// REPLICATE(cExpression, nTimes)
void FAR funREPLICATE(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult;
	CString cExpression(V_BSTR(pvarArguments[FIRST_ARG]));
	int nStep = 0;
	int nTimes = (int)V_R8(pvarArguments[SECOND_ARG]);
	while(nStep < nTimes)
	{
		strResult += nStep > 0 ? ' ' + cExpression : cExpression;
		nStep++;
	}

	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// RIGHT(cExpression, nCharacters)
void FAR funRIGHT(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult(V_BSTR(pvarArguments[FIRST_ARG]));
	strResult = strResult.Right((int)V_R8(pvarArguments[SECOND_ARG]));

	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// ROUND(nExpression, nDecimalPlaces)
void FAR funROUND(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
  V_R8(pvarResult) = floor(V_R8(pvarArguments[FIRST_ARG]) * pow(10, V_R8(pvarArguments[SECOND_ARG])) + 0.5);
  V_R8(pvarResult) = V_R8(pvarResult) / pow(10, V_R8(pvarArguments[SECOND_ARG]));
  V_VT(pvarResult) = VT_R8;
}

// RTRIM(cExpression)
void FAR funRTRIM(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult(V_BSTR(pvarArguments[FIRST_ARG]));
	strResult.TrimRight();
	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// SEC(tExpression)
void FAR funSEC(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dte(V_DATE(pvarArguments[FIRST_ARG]));
	V_R8(pvarResult) = dte.GetSecond();
	V_VT(pvarResult) = VT_R8;
}

// SECONDS()
void FAR funSECONDS(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CTime time1;
	time1 = CTime::GetCurrentTime();
	CTime time0(time1.GetYear(),time1.GetMonth(),time1.GetDay(),0,0,0);
	V_R8(pvarResult) = (long)(time1.GetTime() - time0.GetTime());
	V_VT(pvarResult) = VT_R8;
/*	SYSTEMTIME stTime;
	::GetSystemTime(&stTime);
	V_R8(pvarResult) = stTime.wHour*60*60 + stTime.wMinute*60 + stTime.wSecond + stTime.wMilliseconds*0.001;
	V_VT(pvarResult) = VT_R8;
*/
}

// SIGN(nExpression)
void FAR funSIGN(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_R8(pvarResult) = V_R8(pvarArguments[FIRST_ARG]) < 0 ? -1 : (V_R8(pvarArguments[FIRST_ARG]) == 0 ? 0 : 1);
	V_VT(pvarResult) = VT_R8;
}

// SIN(nExpression)
void FAR funSIN(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_R8(pvarResult) = sin(V_R8(pvarArguments[FIRST_ARG]));
	V_VT(pvarResult) = VT_R8;
}

// SPACE(nSpaces)
void FAR funSPACE(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult(' ', (int)V_R8(pvarArguments[FIRST_ARG]));

	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// SQRT(nExpression)
void FAR funSQRT(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	if(V_R8(pvarArguments[FIRST_ARG]) < 0)
		// SQRT() argument cannot be negative.
		ALXThrowFunOperException(ALX_PRS_ERROR_INVALID_ARGUMENT_SQRT);
	else
		V_R8(pvarResult) = sqrt(V_R8(pvarArguments[FIRST_ARG]));
	V_VT(pvarResult) = VT_R8;
}

// STR(nExpression [, nLength [, nDecimalPlaces]])
void FAR funSTR(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strFormat,strResult;
	int nLength = (int)V_R8(pvarArguments[SECOND_ARG]);
	int nDecimalPlaces = (int)V_R8(pvarArguments[THIRD_ARG]);
	strFormat.Format("%%.%df",nDecimalPlaces);
	strResult.Format(strFormat,V_R8(pvarArguments[FIRST_ARG]));

	if(strResult.GetLength() < nLength)
		strResult = CString(' ', nLength - strResult.GetLength()) + strResult;
	else if (strResult.GetLength() > nLength)
	{
		if(strResult.GetLength() - nDecimalPlaces - 1 > nLength)
			strResult = "*";
		else if(strResult.GetLength() - nDecimalPlaces == nLength)
			strResult = strResult.Left(nLength - 1);
		else
			strResult = strResult.Left(nLength);
	}

	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// STRTRAN(cSearched, cSearchFor [, cReplacement] [, nStartOccurrence] [, nNumberOfOccurrences])
void FAR funSTRTRAN(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult;
	CString strSearched(V_BSTR(pvarArguments[FIRST_ARG]));
	CString strSearchFor(V_BSTR(pvarArguments[SECOND_ARG]));
	CString strReplacement(V_BSTR(pvarArguments[THIRD_ARG]));
	int nLeft = 0;
	int nLen = strSearched.GetLength();
	int nStart = (int)V_R8(pvarArguments[FOURTH_ARG]) - 1;
	int nOccur = (int)V_R8(pvarArguments[FIFTH_ARG]) >= 0 ? (int)V_R8(pvarArguments[FIFTH_ARG]) : nLen;
	while(nStart < nLen && nOccur > 0)
	{
		nLeft = strSearched.Find(strSearchFor, nStart);
		if(nLeft < 0)
		{
			if(nStart == (int)V_R8(pvarArguments[FOURTH_ARG]) - 1)
				strResult = strSearched;
			break;
		}

		strResult += strSearched.Mid(nStart, nLeft - nStart) + strReplacement;
		nLeft += strSearchFor.GetLength();
		nStart = nLeft;
		nOccur--;
	}

	if(nOccur <= 0)
		strResult += strSearched.Mid(nStart, nLen - nStart);

	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// STUFF(cExpression, nStartReplacement, nCharactersReplaced, cReplacement)
void FAR funSTUFF(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult;
	CString strExpression(V_BSTR(pvarArguments[FIRST_ARG]));
	CString strReplacement(V_BSTR(pvarArguments[FOURTH_ARG]));
	int nLen = strExpression.GetLength();
	int nLeft = (int)V_R8(pvarArguments[SECOND_ARG]) - 1;
	int nRight = nLen - nLeft - (int)V_R8(pvarArguments[THIRD_ARG]);
	strResult = strExpression.Left(nLeft) + strReplacement + strExpression.Right(nRight);

	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// SUBSTR(cExpression, nStartPosition [, nCharactersReturned])
void FAR funSUBSTR(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult(V_BSTR(pvarArguments[FIRST_ARG]));
	if(V_R8(pvarArguments[THIRD_ARG]) > 0)
		strResult = strResult.Mid((int)(V_R8(pvarArguments[SECOND_ARG])-1),(int)V_R8(pvarArguments[THIRD_ARG]));
	else
		strResult = strResult.Mid((int)(V_R8(pvarArguments[SECOND_ARG])-1));

	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// TAN(nExpression)
void FAR funTAN(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_R8(pvarResult) = tan(V_R8(pvarArguments[FIRST_ARG]));
	V_VT(pvarResult) = VT_R8;
}

// TIME([nExpression]) -  nExpression - unused
void FAR funTIME(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dte = COleDateTime::GetCurrentTime();
	V_BSTR(pvarResult) = dte.Format("%H:%M:%S").AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// UPPER(cExpression)
void FAR funUPPER(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult(V_BSTR(pvarArguments[FIRST_ARG]));
	strResult.MakeUpper();
	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// VAL(cExpression)
void FAR funVAL(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strExpr(V_BSTR(pvarArguments[FIRST_ARG]));
	V_R8(pvarResult) = atof(strExpr);
	V_VT(pvarResult) = VT_R8;
}

// VARTYPE(eExpression [, lNullDataType])
void FAR funVARTYPE(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult;
	switch(V_VT(pvarArguments[FIRST_ARG]))
	{
	case VT_BSTR:
		strResult = "C";
		break;
	case VT_R8:
		strResult = "N";
		break;
	case VT_BOOL:
		strResult = "L";
		break;
	case VT_DATE:
		strResult = "D";
		break;
	case VT_NULL:
		strResult = "X";
		break;
	default:
		strResult = "U";
	}
	
	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

// YEAR(dExpression)
void FAR funYEAR(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dte(V_DATE(pvarArguments[FIRST_ARG]));
	V_R8(pvarResult) = dte.GetYear();
	V_VT(pvarResult) = VT_R8;
}

/////////////////////////////////////////////////////////////////////////////
// Helper functions

VARIANT_BOOL alxLike(LPCSTR szWildExpr, LPCSTR szExpr)
{
	while(*szWildExpr != '\0')
	{
		// while there are no managing symbols
		while(*szWildExpr != '*' && *szWildExpr != '?')
		{
			if(*szExpr == '\0' || *szWildExpr != *szExpr)
				return AFX_OLE_FALSE;

			szWildExpr++;
			szExpr++;

			if(*szWildExpr == '\0')
				return (*szExpr == '\0') ? AFX_OLE_TRUE : AFX_OLE_FALSE;
		}
   
		// on this place there may be any symbol
		while(*szWildExpr == '?')
		{
			if(*szExpr == '\0')
				return AFX_OLE_FALSE;

			szWildExpr++;
			szExpr++;
		}
  
		// on this place there may be any symbols
		if(*szWildExpr == '*')
		{
			while(*szWildExpr == '*')
				szWildExpr++;

			if(*szWildExpr == '\0')
				return AFX_OLE_TRUE;
		
			if(*szExpr == '\0')
				return AFX_OLE_FALSE;

			do
			{
				while (*szExpr != '\0' && *szExpr != *szWildExpr && *szWildExpr != '?')
					szExpr++;
			
				if(alxLike(szWildExpr, szExpr))
					return AFX_OLE_TRUE;
		
			} while(*szExpr++ != '\0');

			return AFX_OLE_FALSE;
		}
	}

	return (*szExpr == '\0') ? AFX_OLE_TRUE : AFX_OLE_FALSE;
}

