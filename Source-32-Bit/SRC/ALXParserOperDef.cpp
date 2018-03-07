/////////////////////////////////////////////////////////////////////////////
//
// OperDef.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
/*
 Copyright© 2002, Alexey Dolgachov
 mail:	alxsoft@gazinter.net	
 web:	http://www.alxsoft.narod.ru
*/
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "../INCLUDE/ALXParserOperDef.h"
#include "../INCLUDE/ALXParserException.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FIRST_ARG	0
#define SECOND_ARG	1

#define LEFT_OPR	0
#define RIGHT_OPR	1

/////////////////////////////////////////////////////////////////////////////
// Operators realization

void FAR oprNULL(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
}

//	+ 
void FAR oprAddNumNum(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_R8(pvarResult) = V_R8(pvarArguments[LEFT_OPR]) + V_R8(pvarArguments[RIGHT_OPR]);
	V_VT(pvarResult) = VT_R8;
}

void FAR oprAddDateNum(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dteResult(V_DATE(pvarArguments[LEFT_OPR]));
	COleDateTimeSpan dteSpan(V_R8(pvarArguments[RIGHT_OPR]));
	dteResult += dteSpan;
	V_DATE(pvarResult) = dteResult;
	V_VT(pvarResult) = VT_DATE;
}

void FAR oprAddNumDate(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dteResult(V_DATE(pvarArguments[RIGHT_OPR]));
	COleDateTimeSpan dteSpan(V_R8(pvarArguments[LEFT_OPR]));
	dteResult += dteSpan;
	V_DATE(pvarResult) = dteResult;
	V_VT(pvarResult) = VT_DATE;
}

void FAR oprAddStrStr(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult(V_BSTR(pvarArguments[LEFT_OPR]));
	strResult += V_BSTR(pvarArguments[RIGHT_OPR]);
	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

void FAR oprAdd(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_BSTR || V_VT(pvarArguments[RIGHT_OPR]) == VT_BSTR)
		oprAddStrStr(pvarArguments, pvarResult);
	else if(V_VT(pvarArguments[LEFT_OPR]) == VT_DATE)
		oprAddDateNum(pvarArguments, pvarResult);
	else if(V_VT(pvarArguments[RIGHT_OPR]) == VT_DATE)
		oprAddDateNum(pvarArguments, pvarResult);
	else
		oprAddNumNum(pvarArguments, pvarResult);
}

//	-
void FAR oprSubNumNum(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_R8(pvarResult) = V_R8(pvarArguments[LEFT_OPR]) - V_R8(pvarArguments[RIGHT_OPR]);
	V_VT(pvarResult) = VT_R8;
}

void FAR oprSubDateNum(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dteResult(V_DATE(pvarArguments[LEFT_OPR]));
	COleDateTimeSpan dteSpan;
	dteSpan.SetDateTimeSpan((long)V_R8(pvarArguments[RIGHT_OPR]),0,0,0);
	dteResult -= dteSpan;
	V_DATE(pvarResult) = dteResult;
	V_VT(pvarResult) = VT_DATE;
}

void FAR oprSubStrStr(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strResult(V_BSTR(pvarArguments[LEFT_OPR]));
	strResult.TrimRight();
	strResult += V_BSTR(pvarArguments[RIGHT_OPR]);
	V_BSTR(pvarResult) = strResult.AllocSysString();
	V_VT(pvarResult) = VT_BSTR;
}

void FAR oprSub(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_BSTR || V_VT(pvarArguments[RIGHT_OPR]) == VT_BSTR)
		oprAddStrStr(pvarArguments, pvarResult);
	else if(V_VT(pvarArguments[LEFT_OPR]) == VT_DATE)
		oprSubDateNum(pvarArguments, pvarResult);
	else
		oprAddNumNum(pvarArguments, pvarResult);
}

//	*
void FAR oprMult(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_R8(pvarResult) = V_R8(pvarArguments[LEFT_OPR]) * V_R8(pvarArguments[RIGHT_OPR]);
	V_VT(pvarResult) = VT_R8;
}

//	/
void FAR oprDiv(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	if(V_R8(pvarArguments[RIGHT_OPR]) == 0)
		ALXThrowFunOperException(ALX_PRS_ERROR_DEVIDE_BY_ZERO);

	V_R8(pvarResult) = V_R8(pvarArguments[LEFT_OPR]) / V_R8(pvarArguments[RIGHT_OPR]);
	V_VT(pvarResult) = VT_R8;
}

//	^ or **
void FAR oprExp(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	if(V_R8(pvarArguments[LEFT_OPR]) <= 0)
		ALXThrowFunOperException(ALX_PRS_ERROR_EXP_DOMAIN_ERROR);

	V_R8(pvarResult) = pow(V_R8(pvarArguments[LEFT_OPR]), V_R8(pvarArguments[RIGHT_OPR]));
	V_VT(pvarResult) = VT_R8;
}

//	%
void FAR oprMod(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_R8(pvarResult) = fmod(V_R8(pvarArguments[LEFT_OPR]), V_R8(pvarArguments[RIGHT_OPR]));
	V_VT(pvarResult) = VT_R8;
}

//	=
void FAR oprEqualNumNum(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_BOOL(pvarResult) = V_R8(pvarArguments[LEFT_OPR]) == V_R8(pvarArguments[RIGHT_OPR]) ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprEqualDateDate(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dteLeftOpr(V_DATE(pvarArguments[LEFT_OPR]));
	COleDateTime dteRightOpr(V_DATE(pvarArguments[RIGHT_OPR]));

	V_BOOL(pvarResult) = dteLeftOpr == dteRightOpr ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprEqualBoolBool(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_BOOL(pvarResult) = V_BOOL(pvarArguments[LEFT_OPR]) == V_BOOL(pvarArguments[RIGHT_OPR]) ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprEqualStrStr(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strLeftOpr(V_BSTR(pvarArguments[LEFT_OPR]));
	CString strRightOpr(V_BSTR(pvarArguments[RIGHT_OPR]));

	strLeftOpr.TrimRight();
	strRightOpr.TrimRight();
	V_BOOL(pvarResult) = strLeftOpr == strRightOpr ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprEqual(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_R8 || V_VT(pvarArguments[RIGHT_OPR]) == VT_R8)
		oprEqualNumNum(pvarArguments, pvarResult);
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_BOOL || V_VT(pvarArguments[RIGHT_OPR]) == VT_BOOL)
		oprEqualBoolBool(pvarArguments, pvarResult);
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_DATE || V_VT(pvarArguments[RIGHT_OPR]) == VT_DATE)
		oprEqualDateDate(pvarArguments, pvarResult);
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_BSTR || V_VT(pvarArguments[RIGHT_OPR]) == VT_BSTR)
		oprEqualStrStr(pvarArguments, pvarResult);
	else
		oprEqualNumNum(pvarArguments, pvarResult);
}

//	== 
void FAR oprExEqual(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strLeftOpr(V_BSTR(pvarArguments[LEFT_OPR]));
	CString strRightOpr(V_BSTR(pvarArguments[RIGHT_OPR]));

	V_BOOL(pvarResult) = strLeftOpr == strRightOpr ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

//	!= , <> or #
void FAR oprNoEqualNumNum(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_BOOL(pvarResult) = V_R8(pvarArguments[LEFT_OPR]) != V_R8(pvarArguments[RIGHT_OPR])  ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprNoEqualDateDate(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dteLeftOpr(V_DATE(pvarArguments[LEFT_OPR]));
	COleDateTime dteRightOpr(V_DATE(pvarArguments[RIGHT_OPR]));

	V_BOOL(pvarResult) = dteLeftOpr != dteRightOpr ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprNoEqualBoolBool(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_BOOL(pvarResult) = V_BOOL(pvarArguments[LEFT_OPR]) != V_BOOL(pvarArguments[RIGHT_OPR]) ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprNoEqualStrStr(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strLeftOpr(V_BSTR(pvarArguments[LEFT_OPR]));
	CString strRightOpr(V_BSTR(pvarArguments[RIGHT_OPR]));

	V_BOOL(pvarResult) = strLeftOpr != strRightOpr ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprNoEqual(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_R8 || V_VT(pvarArguments[RIGHT_OPR]) == VT_R8)
		oprNoEqualNumNum(pvarArguments, pvarResult);
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_BOOL || V_VT(pvarArguments[RIGHT_OPR]) == VT_BOOL)
		oprNoEqualBoolBool(pvarArguments, pvarResult);
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_DATE || V_VT(pvarArguments[RIGHT_OPR]) == VT_DATE)
		oprNoEqualDateDate(pvarArguments, pvarResult);
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_BSTR || V_VT(pvarArguments[RIGHT_OPR]) == VT_BSTR)
		oprNoEqualStrStr(pvarArguments, pvarResult);
	else
		oprNoEqualNumNum(pvarArguments, pvarResult);
}

//	> 
void FAR oprMoreNumNum(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_BOOL(pvarResult) = V_R8(pvarArguments[LEFT_OPR]) > V_R8(pvarArguments[RIGHT_OPR]) ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprMoreDateDate(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dteLeftOpr(V_DATE(pvarArguments[LEFT_OPR]));
	COleDateTime dteRightOpr(V_DATE(pvarArguments[RIGHT_OPR]));

	V_BOOL(pvarResult) = dteLeftOpr > dteRightOpr ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprMoreBoolBool(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_BOOL(pvarResult) = V_BOOL(pvarArguments[LEFT_OPR]) > V_BOOL(pvarArguments[RIGHT_OPR]) ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprMoreStrStr(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strLeftOpr(V_BSTR(pvarArguments[LEFT_OPR]));
	CString strRightOpr(V_BSTR(pvarArguments[RIGHT_OPR]));

	V_BOOL(pvarResult) = strLeftOpr > strRightOpr ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprMore(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_R8 || V_VT(pvarArguments[RIGHT_OPR]) == VT_R8)
		oprMoreNumNum(pvarArguments, pvarResult);
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_BOOL || V_VT(pvarArguments[RIGHT_OPR]) == VT_BOOL)
		oprMoreBoolBool(pvarArguments, pvarResult);
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_DATE || V_VT(pvarArguments[RIGHT_OPR]) == VT_DATE)
		oprMoreDateDate(pvarArguments, pvarResult);
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_BSTR || V_VT(pvarArguments[RIGHT_OPR]) == VT_BSTR)
		oprMoreStrStr(pvarArguments, pvarResult);
	else
		oprMoreNumNum(pvarArguments, pvarResult);
}

//	< 
void FAR oprLessNumNum(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_BOOL(pvarResult) = V_R8(pvarArguments[LEFT_OPR]) < V_R8(pvarArguments[RIGHT_OPR]) ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprLessDateDate(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dteLeftOpr(V_DATE(pvarArguments[LEFT_OPR]));
	COleDateTime dteRightOpr(V_DATE(pvarArguments[RIGHT_OPR]));

	V_BOOL(pvarResult) = dteLeftOpr < dteRightOpr ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprLessBoolBool(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_BOOL(pvarResult) = V_BOOL(pvarArguments[LEFT_OPR]) < V_BOOL(pvarArguments[RIGHT_OPR]) ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprLessStrStr(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strLeftOpr(V_BSTR(pvarArguments[LEFT_OPR]));
	CString strRightOpr(V_BSTR(pvarArguments[RIGHT_OPR]));

	V_BOOL(pvarResult) = strLeftOpr < strRightOpr ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprLess(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_R8 || V_VT(pvarArguments[RIGHT_OPR]) == VT_R8)
		oprLessNumNum(pvarArguments, pvarResult);
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_BOOL || V_VT(pvarArguments[RIGHT_OPR]) == VT_BOOL)
		oprLessBoolBool(pvarArguments, pvarResult);
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_DATE || V_VT(pvarArguments[RIGHT_OPR]) == VT_DATE)
		oprLessDateDate(pvarArguments, pvarResult);
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_BSTR || V_VT(pvarArguments[RIGHT_OPR]) == VT_BSTR)
		oprLessStrStr(pvarArguments, pvarResult);
	else
		oprLessNumNum(pvarArguments, pvarResult);
}

//	>= 
void FAR oprMoreEqualNumNum(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_BOOL(pvarResult) = V_R8(pvarArguments[LEFT_OPR]) > V_R8(pvarArguments[RIGHT_OPR]) ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprMoreEqualDateDate(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dteLeftOpr(V_DATE(pvarArguments[LEFT_OPR]));
	COleDateTime dteRightOpr(V_DATE(pvarArguments[RIGHT_OPR]));

	V_BOOL(pvarResult) = dteLeftOpr > dteRightOpr ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprMoreEqualBoolBool(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_BOOL(pvarResult) = V_BOOL(pvarArguments[LEFT_OPR]) > V_BOOL(pvarArguments[RIGHT_OPR]) ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprMoreEqualStrStr(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strLeftOpr(V_BSTR(pvarArguments[LEFT_OPR]));
	CString strRightOpr(V_BSTR(pvarArguments[RIGHT_OPR]));

	V_BOOL(pvarResult) = strLeftOpr > strRightOpr ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprMoreEqual(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_R8 || V_VT(pvarArguments[RIGHT_OPR]) == VT_R8)
		oprMoreEqualNumNum(pvarArguments, pvarResult);
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_BOOL || V_VT(pvarArguments[RIGHT_OPR]) == VT_BOOL)
		oprMoreEqualBoolBool(pvarArguments, pvarResult);
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_DATE || V_VT(pvarArguments[RIGHT_OPR]) == VT_DATE)
		oprMoreEqualDateDate(pvarArguments, pvarResult);
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_BSTR || V_VT(pvarArguments[RIGHT_OPR]) == VT_BSTR)
		oprMoreEqualStrStr(pvarArguments, pvarResult);
	else
		oprMoreEqualNumNum(pvarArguments, pvarResult);
}

//	<=
void FAR oprLessEqualNumNum(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_BOOL(pvarResult) = V_R8(pvarArguments[LEFT_OPR]) < V_R8(pvarArguments[RIGHT_OPR]) ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprLessEqualDateDate(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	COleDateTime dteLeftOpr(V_DATE(pvarArguments[LEFT_OPR]));
	COleDateTime dteRightOpr(V_DATE(pvarArguments[RIGHT_OPR]));

	V_BOOL(pvarResult) = dteLeftOpr < dteRightOpr ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprLessEqualBoolBool(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_BOOL(pvarResult) = V_BOOL(pvarArguments[LEFT_OPR]) < V_BOOL(pvarArguments[RIGHT_OPR]) ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprLessEqualStrStr(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strLeftOpr(V_BSTR(pvarArguments[LEFT_OPR]));
	CString strRightOpr(V_BSTR(pvarArguments[RIGHT_OPR]));

	V_BOOL(pvarResult) = strLeftOpr < strRightOpr ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

void FAR oprLessEqual(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_R8 || V_VT(pvarArguments[RIGHT_OPR]) == VT_R8)
		oprLessNumNum(pvarArguments, pvarResult);
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_BOOL || V_VT(pvarArguments[RIGHT_OPR]) == VT_BOOL)
		oprLessBoolBool(pvarArguments, pvarResult);
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_DATE || V_VT(pvarArguments[RIGHT_OPR]) == VT_DATE)
		oprLessDateDate(pvarArguments, pvarResult);
	if(V_VT(pvarArguments[LEFT_OPR]) == VT_BSTR || V_VT(pvarArguments[RIGHT_OPR]) == VT_BSTR)
		oprLessStrStr(pvarArguments, pvarResult);
	else
		oprLessNumNum(pvarArguments, pvarResult);
}

//	! or NOT
void FAR oprNo(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_BOOL(pvarResult) = V_BOOL(pvarArguments[RIGHT_OPR]) == AFX_OLE_FALSE ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

//	AND
void FAR oprAnd(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_BOOL(pvarResult) = V_BOOL(pvarArguments[LEFT_OPR]) && V_BOOL(pvarArguments[RIGHT_OPR]) ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

//	OR
void FAR oprOr(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	V_BOOL(pvarResult) = V_BOOL(pvarArguments[LEFT_OPR]) || V_BOOL(pvarArguments[RIGHT_OPR]) ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}

//	$
void FAR oprFind(VARIANT* pvarArguments[], VARIANT* pvarResult)
{
	CString strLeftOpr(V_BSTR(pvarArguments[LEFT_OPR]));
	CString strRightOpr(V_BSTR(pvarArguments[RIGHT_OPR]));

	V_BOOL(pvarResult) = strRightOpr.Find(strLeftOpr) > 0 ? AFX_OLE_TRUE : AFX_OLE_FALSE;
	V_VT(pvarResult) = VT_BOOL;
}


