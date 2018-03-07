/////////////////////////////////////////////////////////////////////////////
//
// OperDef.h : header file
//
/////////////////////////////////////////////////////////////////////////////
/*
 Copyright© 2002, Alexey Dolgachov
 mail:	alxsoft@gazinter.net	
 web:	http://www.alxsoft.narod.ru
*/
/////////////////////////////////////////////////////////////////////////////
#if !defined(ALX_OPERDEF_H_INCLUDED_)
#define ALX_OPERDEF_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Operators
void FAR oprNULL(VARIANT* pvarArguments[], VARIANT* pvarResult);
//	+
void FAR oprAdd(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprAddNumNum(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprAddDateNum(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprAddNumDate(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprAddStrStr(VARIANT* pvarArguments[], VARIANT* pvarResult);
//	-
void FAR oprSub(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprSubNumNum(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprSubDateNum(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprSubStrStr(VARIANT* pvarArguments[], VARIANT* pvarResult);
//	*
void FAR oprMult(VARIANT* pvarArguments[], VARIANT* pvarResult);
//	/
void FAR oprDiv(VARIANT* pvarArguments[], VARIANT* pvarResult);
//	^ or **
void FAR oprExp(VARIANT* pvarArguments[], VARIANT* pvarResult);
//	%
void FAR oprMod(VARIANT* pvarArguments[], VARIANT* pvarResult);
//	=
void FAR oprEqual(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprEqualNumNum(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprEqualDateDate(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprEqualBoolBool(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprEqualStrStr(VARIANT* pvarArguments[], VARIANT* pvarResult);
//	==
void FAR oprExEqual(VARIANT* pvarArguments[], VARIANT* pvarResult);
//	!= , <> or #
void FAR oprNoEqual(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprNoEqualNumNum(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprNoEqualDateDate(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprNoEqualBoolBool(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprNoEqualStrStr(VARIANT* pvarArguments[], VARIANT* pvarResult);
//	>
void FAR oprMore(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprMoreNumNum(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprMoreDateDate(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprMoreBoolBool(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprMoreStrStr(VARIANT* pvarArguments[], VARIANT* pvarResult);
//	<
void FAR oprLess(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprLessNumNum(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprLessDateDate(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprLessBoolBool(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprLessStrStr(VARIANT* pvarArguments[], VARIANT* pvarResult);
//	>=
void FAR oprMoreEqual(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprMoreEqualNumNum(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprMoreEqualDateDate(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprMoreEqualBoolBool(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprMoreEqualStrStr(VARIANT* pvarArguments[], VARIANT* pvarResult);
//	<=
void FAR oprLessEqual(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprLessEqualNumNum(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprLessEqualDateDate(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprLessEqualBoolBool(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR oprLessEqualStrStr(VARIANT* pvarArguments[], VARIANT* pvarResult);
//	! or NOT
void FAR oprNo(VARIANT* pvarArguments[], VARIANT* pvarResult);
//	AND
void FAR oprAnd(VARIANT* pvarArguments[], VARIANT* pvarResult);
//	OR
void FAR oprOr(VARIANT* pvarArguments[], VARIANT* pvarResult);
//	$
void FAR oprFind(VARIANT* pvarArguments[], VARIANT* pvarResult);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_OPERDEF_H_INCLUDED_)
