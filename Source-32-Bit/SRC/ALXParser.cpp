/////////////////////////////////////////////////////////////////////////////
//
// ALXParser.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
/*
 Copyright© 2002, Alexey Dolgachov
 mail:	alxsoft@gazinter.net	
 web:	http://www.alxsoft.narod.ru
*/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../INCLUDE/ALXParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LEFT_OPR		0
#define RIGHT_OPR		1

#define ANY_ARG_COUNT	-1
#define MIN_DATE                (-657434L)  // about year 100

/////////////////////////////////////////////////////////////////////////////
// Function list

extern ALXPRS_DEF_FUNCTION alxFunList[];
extern alxFuncCount;

/////////////////////////////////////////////////////////////////////////////
// ID Operators

const short c_nOprNULL		=	 0; //	+
const short c_nOprAdd		=	 1; //	+
const short c_nOprSub		=	 2; //	-
const short c_nOprMult		=	 3; //	*
const short c_nOprDiv		=	 4; //	/
const short c_nOprExp		=	 5; //	^ or **
const short c_nOprMod		=	 6; //	%
const short c_nOprEqual		=	 7;	//	=
const short c_nOprExEqual	=	 8; //	==
const short c_nOprNoEqual	=	 9; //	!= or #
const short c_nOprMore		=	10;	//	>
const short c_nOprMoreEqual	=	11;	//	>=
const short c_nOprLess		=	12;	//	<
const short c_nOprLessEqual	=	13;	//	<=
const short c_nOprNo		=	14; //	! or NOT
const short c_nOprAnd		=	15;	//	AND
const short c_nOprOr		=	16;	//	OR
const short c_nOprFind		=	17; //	$

/////////////////////////////////////////////////////////////////////////////
// Helper functions

BOOL alxIsBoolExpr(LPCSTR szExpr)
{
	if(_stricmp(szExpr, "T") == 0 || _stricmp(szExpr, "Y") == 0)
		return TRUE;
	else if(_stricmp(szExpr, "F") == 0 || _stricmp(szExpr, "N") == 0)
		return TRUE;

	return FALSE;
}

BOOL alxIsLogicOprExpr(LPCSTR szExpr, short& nOprID, int& nOprPrioritet)
{
	if(_stricmp(szExpr, "NOT") == 0)
	{
		nOprID = c_nOprNo;
		nOprPrioritet = 5;
		return TRUE;
	}
	else if(_stricmp(szExpr, "AND") == 0)
	{
		nOprID = c_nOprAnd;
		nOprPrioritet = 6;
		return TRUE;
	}
	else if(_stricmp(szExpr, "OR") == 0)
	{
		nOprID = c_nOprOr;
		nOprPrioritet = 7;
		return TRUE;
	}

	return FALSE;
}

void prsCopyVariant(VARIANT* pvarDst, VARIANT* pvarSrc)
{
	ASSERT(pvarDst != NULL && pvarSrc != NULL);
	ASSERT(pvarDst != pvarSrc);

	::VariantClear(pvarDst);

	memcpy(pvarDst, pvarSrc, sizeof(VARIANT));

	switch(pvarSrc->vt)
	{
	case VT_R8: case VT_DATE: case VT_BOOL: case VT_NULL:
		break;
	case VT_BSTR:
		pvarDst->bstrVal = ::SysAllocString(pvarSrc->bstrVal);
		break;
	default:
		AfxCheckError(::VariantChangeType(pvarDst, pvarSrc, 0, VT_R8));
	}
}

/////////////////////////////////////////////////////////////////////////////
// CALXParser

IMPLEMENT_DYNAMIC(CALXParser, CObject)

CALXParser::CALXParser()
{
	m_alxFunList = alxFunList;
	m_alxFuncCount = alxFuncCount;

	m_pstOperFuncs = NULL;
	m_pOperandArguments = NULL;
	m_pvarResult = NULL;

	m_pLastUsedVariable = NULL;
	m_pLastVariable = NULL;

	m_pCurentVariable = NULL;
	m_bCurVariableUsed = FALSE;

	m_pLastError = NULL;

	m_nOperatorCount = 0;
	m_nFunctionCount = 0;
}

CALXParser::~CALXParser()
{
	ClearData();

	ALXPRS_VARIABLE_ITEM* pDelVariable;
	while(m_pLastVariable != NULL)
	{
		pDelVariable = m_pLastVariable;
		m_pLastVariable = m_pLastVariable->m_pliPrevItem;

		::VariantClear(&pDelVariable->m_stItem.m_varValue);
		delete[] pDelVariable->m_stItem.m_szName;

		delete pDelVariable;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CALXParser diagnostics

#ifdef _DEBUG
void CALXParser::AssertValid() const
{
	CObject::AssertValid();
}

void CALXParser::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CALXParser commands

void CALXParser::Parse(LPCSTR lpszExpression)
{
	ClearData();

	m_lpszExp = lpszExpression;   

	void* pResult = DoParse();
	if(*m_lpszExp == ')')
		AddError(ALX_PRS_ERROR_MISSING, '(');
	else if(m_pLastError == NULL && *(m_lpszExp - 1) == ',')
		AddError(ALX_PRS_ERROR_SYNTAX_ERROR, m_lpszExp-1);

	int nValueCount = m_nValueCount + m_nTmpValueCount + m_nFunctionCount;
	m_pOperandArguments = new VARIANT[nValueCount];
	memset(m_pOperandArguments, 0, sizeof(VARIANT)*nValueCount);
	// the additional operand for negative numbers is selected
	m_pOperandArguments[0].vt = VT_R8;

	if(m_nOperatorCount + m_nFunctionCount > 0)
		m_pstOperFuncs = new ALXPRS_OPERFUNC[m_nOperatorCount + m_nFunctionCount];
	else if(m_nValueCount <= 0)
		AddError(ALX_PRS_ERROR_MISSING_EXPR);

	m_indCurPtrOperFunc = 0;
	m_nValueCount = 1;

	m_pvarResult = GetResult(pResult);

	if(m_pLastError != NULL)
	{
		CALXParserException* pException = new CALXParserException(m_pLastError);
		m_pLastError = NULL;

		THROW(pException);
	}
}

void* CALXParser::DoParse(int nPrioritet, ALXPRS_OPERATOR_INFO_ITEM* pCurOperatorInfo, void* pCurOperandInfo)
{
	void* pResult = NULL;
	ALXPRS_CONST_VAR_INFO* pCurConstVarInfo = NULL;

//	int nPrioritet = INT_MAX;
	m_nTmpValueCount++;

	SetTmpResult(&pResult, pCurOperatorInfo, pCurOperandInfo);

	for(;;)
	{
		switch(*m_lpszExp)
		{
		case ' ':	// ignore
		case '\t':	// ignore
			m_lpszExp++;
			continue;
		case ';':	// go to next line
			m_lpszExp++;
			while(*m_lpszExp == ' ')
				m_lpszExp++;
			if(*m_lpszExp == '\r')
				m_lpszExp++;
			if(*m_lpszExp == '\n')
				m_lpszExp++;
			continue;
		case ',':	// next function argument
			m_lpszExp++;
		case '\0': case '\n': case '\r': // end expression
		case ')':	// go to top level
			return SelTmpResult(&pResult, pCurOperatorInfo, pCurOperandInfo);
		case '(':	// receive result of expression in brackets
			if(pCurOperandInfo != NULL)
			{
				AddError(ALX_PRS_ERROR_MISSING_OPERATOR);
				int nPrior = 0;
				AddOperatorsInfo(c_nOprNULL, 0, nPrior, &pCurOperatorInfo, &pCurOperandInfo);
			}
			m_lpszExp++;
			pCurOperandInfo = DoParse();
			if(*m_lpszExp != ')')
			{
				AddError(ALX_PRS_ERROR_MISSING, ')');
				return SelTmpResult(&pResult, pCurOperatorInfo, pCurOperandInfo);
			}
			break;
		// Operators
		case '+':
			AddOperatorsInfo(c_nOprAdd, pCurOperandInfo == NULL ? 8 : 2, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);
			break;
		case '-':
			{
				if(pCurOperandInfo == NULL)
				{
					CHAR chNext = m_lpszExp[1];
					if(chNext >= '0' && chNext <= '9')
					{
						LPSTR szExp = GetNumberExpr();
			
						pCurOperandInfo = AddValueInfo(szExp, VT_R8, &pCurOperatorInfo, &pCurOperandInfo);
						SetTmpResult(&pResult, pCurOperatorInfo, pCurOperandInfo);
						continue;
					}
					else
						AddOperatorsInfo(c_nOprSub, 8, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);
				}
				else
				{
					if(nPrioritet < 2)
						AddOperatorsInfo(c_nOprAdd, 2, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);
					else
					{
						AddOperatorsInfo(c_nOprAdd, 2, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);

						CHAR chNext = m_lpszExp[1];
						if(chNext >= '0' && chNext <= '9')
						{
							LPSTR szExp = GetNumberExpr();
			
							pCurOperandInfo = AddValueInfo(szExp, VT_R8, &pCurOperatorInfo, &pCurOperandInfo);
							SetTmpResult(&pResult, pCurOperatorInfo, pCurOperandInfo);
							continue;
						}
						else
							AddOperatorsInfo(c_nOprSub, 8, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);
					}
				}
				break;
			}
		case '$':
			AddOperatorsInfo(c_nOprFind, 2, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);
			break;
		case '*':
			if(m_lpszExp[1] == '*')
			{
				m_lpszExp++;
				AddOperatorsInfo(c_nOprExp, 4, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);
			}
			else
				AddOperatorsInfo(c_nOprMult, 3, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);
			break;
		case '^':
			AddOperatorsInfo(c_nOprExp, 4, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);
			break;
		case '/':
			AddOperatorsInfo(c_nOprDiv, 3, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);
			break;
		case '%':
			AddOperatorsInfo(c_nOprMod, 3, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);
			break;
		case '=':
			if(m_lpszExp[1] == '=')
			{
				m_lpszExp++;
				AddOperatorsInfo(c_nOprExEqual, 0, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);
			}
			else
				AddOperatorsInfo(c_nOprEqual, 0, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);
			break;
		case '>':
			if(m_lpszExp[1] == '=')
			{
				m_lpszExp++;
				AddOperatorsInfo(c_nOprMoreEqual, 1, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);
			}
			else
				AddOperatorsInfo(c_nOprMore, 0, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);
			break;
		case '<':
			if(m_lpszExp[1] == '=')
			{
				m_lpszExp++;
				AddOperatorsInfo(c_nOprLessEqual, 1, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);
			}
			else if(m_lpszExp[1] == '>')
			{
				m_lpszExp++;
				AddOperatorsInfo(c_nOprNoEqual, 0, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);
			}
			else
				AddOperatorsInfo(c_nOprLess, 1, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);
			break;
		case '!':
			if(m_lpszExp[1] == '=')
			{
				m_lpszExp++;
				AddOperatorsInfo(c_nOprNoEqual, 0, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);
			}
			else
				AddOperatorsInfo(c_nOprNo, 5, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);
			break;
		case '#':
			AddOperatorsInfo(c_nOprNoEqual, 0, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);
			break;
		// Values 
		case '.':
			{
			LPSTR szExp = GetNumberExpr();

			if(szExp != NULL)
			{
				pCurOperandInfo = AddValueInfo(szExp, VT_R8, &pCurOperatorInfo, &pCurOperandInfo);
				SetTmpResult(&pResult, pCurOperatorInfo, pCurOperandInfo);
				continue;
			}
			else
			{
				m_lpszExp++;

				szExp = GetExpr('.');

				if(alxIsBoolExpr(szExp))
					pCurOperandInfo = AddValueInfo(szExp, VT_BOOL, &pCurOperatorInfo, &pCurOperandInfo);
				else
				{
					short nOprID;
					int nOprPrioritet;
					if(alxIsLogicOprExpr(szExp, nOprID, nOprPrioritet))
						AddOperatorsInfo(nOprID, nOprPrioritet, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);
					else
						AddError(ALX_PRS_ERROR_SYNTAX_ERROR,  szExp);

				}
			}
			break;
			}
		case '"':
			{
			m_lpszExp++;
			LPSTR szExp = GetExpr('"');

			pCurOperandInfo = AddValueInfo(szExp, VT_BSTR, &pCurOperatorInfo, &pCurOperandInfo);
			break;
			}
		case '\'':
			{
			m_lpszExp++;
			LPSTR szExp = GetExpr('\'');

			pCurOperandInfo = AddValueInfo(szExp, VT_BSTR, &pCurOperatorInfo, &pCurOperandInfo);
			break;
			}
		case '{':
			{
			m_lpszExp++;
			LPSTR szExp = GetExpr('}');

			pCurOperandInfo = AddValueInfo(szExp, VT_DATE, &pCurOperatorInfo, &pCurOperandInfo);
			break;
			}
		default:
			{
				LPSTR szExp = GetNumberExpr();
				// if number
				if(szExp != NULL)
				{
					pCurOperandInfo = AddValueInfo(szExp, VT_R8, &pCurOperatorInfo, &pCurOperandInfo);
					SetTmpResult(&pResult, pCurOperatorInfo, pCurOperandInfo);
					continue;
				}
				szExp = GetSymbolExpr();
				// if variable, function or operator
				if(szExp != NULL)
				{
					// if function
					if(*m_lpszExp == '(')
					{
						if(pCurOperandInfo != NULL)
						{
							AddError(ALX_PRS_ERROR_MISSING_OPERATOR);
							int nPrior = 0;
							AddOperatorsInfo(c_nOprNULL, 0, nPrior, &pCurOperatorInfo, &pCurOperandInfo);
						}
						m_lpszExp++;
						pCurOperandInfo = AddFunctionInfo(szExp);
						delete[] szExp;
						break;
					}
					else
					{
						short nOprID;
						int nOprPrioritet;
						if(alxIsLogicOprExpr(szExp, nOprID, nOprPrioritet))
							AddOperatorsInfo(nOprID, nOprPrioritet, nPrioritet, &pCurOperatorInfo, &pCurOperandInfo);
						else
						{
							pCurOperandInfo = AddValueInfo(szExp, VT_UNKNOWN, &pCurOperatorInfo, &pCurOperandInfo);
							SetTmpResult(&pResult, pCurOperatorInfo, pCurOperandInfo);
							continue;
						}
					}
					break;
				}
/*				else
				{
					SetTmpResult(&pResult, pCurOperatorInfo, pCurOperandInfo);

					continue;
				}
*/			}
		}

		SetTmpResult(&pResult, pCurOperatorInfo, pCurOperandInfo);

		if(*m_lpszExp != '\0' && *m_lpszExp != '\n' && *m_lpszExp != '\r')
			m_lpszExp++;
		else
			break;
	}

	return SelTmpResult(&pResult, pCurOperatorInfo, pCurOperandInfo);
}

void CALXParser::AddOperatorsInfo(short nOperID, int nPrioritet, int& nCurPrioritet, ALXPRS_OPERATOR_INFO_ITEM** ppOperInfo, void** ppOperandInfo)
{
	ALXPRS_OPERATOR_INFO_ITEM* pNewOperInfo = new ALXPRS_OPERATOR_INFO_ITEM;

	pNewOperInfo->m_stItem.m_nOperID = nOperID;
	pNewOperInfo->m_stItem.m_pLeftOper = *ppOperandInfo;
	pNewOperInfo->m_stItem.m_pRightOper = NULL;

	pNewOperInfo->m_pliNextItem = NULL;

	if(*ppOperInfo != NULL)
		(*ppOperInfo)->m_stItem.m_pRightOper = *ppOperandInfo;

	if(pNewOperInfo->m_stItem.m_pLeftOper == NULL && nOperID != c_nOprNo && nOperID != c_nOprSub && nOperID != c_nOprAdd)
	{
		AddError(ALX_PRS_ERROR_MISSING_OPERAND);

		if(*ppOperInfo != NULL && (*ppOperInfo)->m_stItem.m_pRightOper == NULL)
			AddError(ALX_PRS_ERROR_MISSING_OPERAND);
	}

	m_nOperatorCount++;

	*ppOperandInfo = NULL;

	if(nPrioritet > nCurPrioritet)
	{
		pNewOperInfo->m_pliPrevItem = NULL;

		ASSERT(*ppOperInfo != NULL);

		m_lpszExp++;
		*ppOperandInfo = DoParse(nPrioritet, pNewOperInfo, *ppOperandInfo);

		if(*m_lpszExp == ')')
			m_lpszExp--;
		else if(*(m_lpszExp - 1) == ',')
			m_lpszExp -= 2;
	}
	else
	{
		nCurPrioritet = nPrioritet;

		pNewOperInfo->m_pliPrevItem = *ppOperInfo;

		if(*ppOperInfo != NULL)
			(*ppOperInfo)->m_pliNextItem = pNewOperInfo;

		*ppOperInfo = pNewOperInfo;
	}
}

void* CALXParser::AddValueInfo(LPSTR szExpr, UINT nType, ALXPRS_OPERATOR_INFO_ITEM** ppOperInfo, void** ppOperandInfo)
{
	if(*ppOperandInfo != NULL)
	{
		AddError(ALX_PRS_ERROR_MISSING_OPERATOR);
		int nPrioritet = 0;
		AddOperatorsInfo(c_nOprNULL, 0, nPrioritet, ppOperInfo, ppOperandInfo);
	}

	ALXPRS_CONST_VAR_INFO* pNewConstVarInfo = new ALXPRS_CONST_VAR_INFO;

	pNewConstVarInfo->m_nTypeStruct = AOI_TYPE_CONSTVAR;

	pNewConstVarInfo->m_szExpr = szExpr;
	pNewConstVarInfo->m_nType = nType;

	m_nValueCount++;

	return pNewConstVarInfo;
}

VARIANT* CALXParser::NewVariable()
{
	VARIANT* pvarVar = &m_pOperandArguments[m_nValueCount];
	m_nValueCount++;

	return pvarVar;
}

void* CALXParser::AddFunctionInfo(LPSTR szName)
{
	ALXPRS_ARGUMENT_INFO_ITEM* pArgumentItem = NULL;
	ALXPRS_FUNCTION_INFO* pNewFunctionInfo = new ALXPRS_FUNCTION_INFO;

	pNewFunctionInfo->m_nTypeStruct = AOI_TYPE_FUNCTION;

	pNewFunctionInfo->m_pstFuncInfo = FindFunction(szName);

	pNewFunctionInfo->m_nArgCount = 0;
	pNewFunctionInfo->m_pFirstArgItem = NULL;

	while(*m_lpszExp == ' ')
		m_lpszExp++;

	// process other arguments
	while(*m_lpszExp != '\0' && *m_lpszExp != '\n' && *m_lpszExp != '\r' && *m_lpszExp != ')')
	{
		void* pCurArgumentInfo = DoParse();

		pArgumentItem = AddArgumentInfo(pArgumentItem, pCurArgumentInfo);

		if(pNewFunctionInfo->m_nArgCount == 0)
			pNewFunctionInfo->m_pFirstArgItem = pArgumentItem;

		pNewFunctionInfo->m_nArgCount++;
	}


	ALXPRS_DEF_FUNCTION* pstDefFuncInfo = FindFunction(szName);
	pNewFunctionInfo->m_pstFuncInfo = pNewFunctionInfo->m_pstFuncInfo;

	if(pstDefFuncInfo != NULL)
	{
		if(*m_lpszExp != ')')
			AddError(ALX_PRS_ERROR_FUNCTION_MISSING, pstDefFuncInfo->m_szName);

		if(pstDefFuncInfo->m_nMinArgCount > pNewFunctionInfo->m_nArgCount)
			AddError(ALX_PRS_ERROR_TOO_FEW_ARGUMENTS, pstDefFuncInfo->m_szName);

		// if not all arguments are given
		if(pstDefFuncInfo->m_nDefArgCount > pNewFunctionInfo->m_nArgCount)
		{
			m_nValueCount += pstDefFuncInfo->m_nDefArgCount - pNewFunctionInfo->m_nArgCount;
			pNewFunctionInfo->m_nArgCount = pstDefFuncInfo->m_nDefArgCount;
		}
		// if function with any number of arguments
		else if(pstDefFuncInfo->m_nDefArgCount == ANY_ARG_COUNT)
		{
			// the shall transfer empty argument for definition of the end of the list of arguments
			pNewFunctionInfo->m_nArgCount++;
			m_nTmpValueCount++;
		}
		// is given arguments more than it is necessary
		else if(pstDefFuncInfo->m_nDefArgCount < pNewFunctionInfo->m_nArgCount)
			AddError(ALX_PRS_ERROR_TOO_MANY_ARGUMENTS, pstDefFuncInfo->m_szName);
	}
	else
		AddError(ALX_PRS_ERROR_FUNCTION_NOT_FOUND, szName);

	m_nFunctionCount++;

	return pNewFunctionInfo;
}

ALXPRS_ARGUMENT_INFO_ITEM* CALXParser::AddArgumentInfo(ALXPRS_ARGUMENT_INFO_ITEM* pArgumentItem, void* pArgumentInfo)
{
	ALXPRS_ARGUMENT_INFO_ITEM* pNewArgumentItem = new ALXPRS_ARGUMENT_INFO_ITEM;

	pNewArgumentItem->m_pliPrevItem = pArgumentItem;
	pNewArgumentItem->m_pliNextItem = NULL;

	pNewArgumentItem->m_stItem = pArgumentInfo;

	if(pArgumentItem != NULL)
		pArgumentItem->m_pliNextItem = pNewArgumentItem;

	return pNewArgumentItem;
}


void CALXParser::ClearData()
{
	if(m_pOperandArguments != NULL)
	{
		delete[] m_pOperandArguments;
		m_pOperandArguments = NULL;
	}

	for(int i = 0; i < m_nOperatorCount + m_nFunctionCount; i++)
	{
		delete[] m_pstOperFuncs[i].pvarArguments;
	}

	if(m_pstOperFuncs != NULL)
	{
		delete[] m_pstOperFuncs;
		m_pstOperFuncs = NULL;
	}

	if(m_pLastUsedVariable != NULL)
	{
		if(m_pLastVariable == NULL)
			m_pLastVariable = m_pLastUsedVariable;
		else
		{
			ALXPRS_VARIABLE_ITEM* pFirstUsedVariable = m_pLastUsedVariable;
			while(pFirstUsedVariable->m_pliPrevItem != NULL)
				pFirstUsedVariable = pFirstUsedVariable->m_pliPrevItem;
			pFirstUsedVariable->m_pliPrevItem = m_pLastVariable;
			m_pLastVariable = m_pLastUsedVariable;
		}
		m_pLastUsedVariable = NULL;
	}

	m_nOperatorCount = 0;
	m_nFunctionCount = 0;
	m_nValueCount = 0;
	m_nTmpValueCount = 1;

	m_pCurentVariable = NULL;
	m_bCurVariableUsed = FALSE;
}

void CALXParser::AddOperator(const short nOperID, ALXPRS_OPERFUNC* pstOperator)
{
	switch(nOperID)
	{
	case c_nOprAdd:		//	+
		if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_R8 && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_R8)
		{
			pstOperator->m_lpfnOperFunc = oprAddNumNum;
			pstOperator->pvarResult->vt = VT_R8;
		}
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_BSTR && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_BSTR)
		{
			pstOperator->m_lpfnOperFunc = oprAddStrStr;
			pstOperator->pvarResult->vt = VT_BSTR;
		}
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_DATE && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_R8)
		{
			pstOperator->m_lpfnOperFunc = oprAddDateNum;
			pstOperator->pvarResult->vt = VT_DATE;
		}
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_R8 && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_DATE)
		{
			pstOperator->m_lpfnOperFunc = oprAddNumDate;
			pstOperator->pvarResult->vt = VT_DATE;
		}
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) != VT_UNKNOWN && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_UNKNOWN)
		{
			pstOperator->m_lpfnOperFunc = oprAdd;
			pstOperator->pvarResult->vt = VT_UNKNOWN;

			AddError(ALX_PRS_ERROR_OPERAND_MISMATCH);
		}
		else
		{
			pstOperator->m_lpfnOperFunc = oprAdd;
			pstOperator->pvarResult->vt = VT_UNKNOWN;
		}
		break;
	case c_nOprSub:		//	-
		if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_R8 && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_R8)
		{
			pstOperator->m_lpfnOperFunc = oprSubNumNum;
			pstOperator->pvarResult->vt = VT_R8;
		}
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_BSTR && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_BSTR)
		{
			pstOperator->m_lpfnOperFunc = oprSubStrStr;
			pstOperator->pvarResult->vt = VT_BSTR;
		}
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_DATE && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_R8)
		{
			pstOperator->m_lpfnOperFunc = oprSubDateNum;
			pstOperator->pvarResult->vt = VT_DATE;
		}
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) != VT_UNKNOWN && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_UNKNOWN)
		{
			pstOperator->m_lpfnOperFunc = oprSub;
			pstOperator->pvarResult->vt = VT_UNKNOWN;
	
			AddError(ALX_PRS_ERROR_OPERAND_MISMATCH);
		}
		else
		{
			pstOperator->m_lpfnOperFunc = oprSub;
			pstOperator->pvarResult->vt = VT_UNKNOWN;
		}

		break;
	case c_nOprMult:	//	*
		if((V_VT(pstOperator->pvarArguments[LEFT_OPR]) != VT_R8 && V_VT(pstOperator->pvarArguments[LEFT_OPR]) != VT_UNKNOWN) || (V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_R8 && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_UNKNOWN))
			AddError(ALX_PRS_ERROR_OPERAND_MISMATCH);

		pstOperator->m_lpfnOperFunc = oprMult;
		pstOperator->pvarResult->vt = VT_R8;
		break;
	case c_nOprDiv:		//	/
		if((V_VT(pstOperator->pvarArguments[LEFT_OPR]) != VT_R8 && V_VT(pstOperator->pvarArguments[LEFT_OPR]) != VT_UNKNOWN) || (V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_R8 && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_UNKNOWN))
			AddError(ALX_PRS_ERROR_OPERAND_MISMATCH);

		pstOperator->m_lpfnOperFunc = oprDiv;
		pstOperator->pvarResult->vt = VT_R8;
		break;
	case c_nOprExp:		//	^ or **
		if((V_VT(pstOperator->pvarArguments[LEFT_OPR]) != VT_R8 && V_VT(pstOperator->pvarArguments[LEFT_OPR]) != VT_UNKNOWN) || (V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_R8 && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_UNKNOWN))
			AddError(ALX_PRS_ERROR_OPERAND_MISMATCH);

		pstOperator->m_lpfnOperFunc = oprExp;
		pstOperator->pvarResult->vt = VT_R8;
		break;
	case c_nOprMod:		//	%
		if((V_VT(pstOperator->pvarArguments[LEFT_OPR]) != VT_R8 && V_VT(pstOperator->pvarArguments[LEFT_OPR]) != VT_UNKNOWN) || (V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_R8 && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_UNKNOWN))
			AddError(ALX_PRS_ERROR_OPERAND_MISMATCH);

		pstOperator->m_lpfnOperFunc = oprMult;
		pstOperator->pvarResult->vt = VT_R8;
		break;
	case c_nOprEqual:	//	=
	case c_nOprExEqual:	//	==
		if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_R8 && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_R8)
			pstOperator->m_lpfnOperFunc = oprEqualNumNum;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_BSTR && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_BSTR)
		{
			if(nOperID == c_nOprEqual)
				pstOperator->m_lpfnOperFunc = oprEqualStrStr;
			else
				pstOperator->m_lpfnOperFunc = oprExEqual;
		}
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_DATE && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_DATE)
			pstOperator->m_lpfnOperFunc = oprEqualDateDate;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_BOOL && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_BOOL)
			pstOperator->m_lpfnOperFunc = oprEqualBoolBool;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) != VT_UNKNOWN && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_UNKNOWN)
		{
			pstOperator->m_lpfnOperFunc = oprEqual;

			AddError(ALX_PRS_ERROR_OPERAND_MISMATCH);
		}
		else
			pstOperator->m_lpfnOperFunc = oprEqual;

		pstOperator->pvarResult->vt = VT_BOOL;
		break;
	case c_nOprNoEqual:		//	!= , # or <>
		if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_R8 && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_R8)
			pstOperator->m_lpfnOperFunc = oprNoEqualNumNum;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_BSTR && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_BSTR)
				pstOperator->m_lpfnOperFunc = oprNoEqualStrStr;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_DATE && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_DATE)
			pstOperator->m_lpfnOperFunc = oprNoEqualDateDate;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_BOOL && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_BOOL)
			pstOperator->m_lpfnOperFunc = oprNoEqualBoolBool;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) != VT_UNKNOWN && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_UNKNOWN)
		{
			pstOperator->m_lpfnOperFunc = oprNoEqual;

			AddError(ALX_PRS_ERROR_OPERAND_MISMATCH);
		}
		else
			pstOperator->m_lpfnOperFunc = oprNoEqual;

		pstOperator->pvarResult->vt = VT_BOOL;
		break;
	case c_nOprMore:	//	>
		if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_R8 && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_R8)
			pstOperator->m_lpfnOperFunc = oprMoreNumNum;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_BSTR && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_BSTR)
				pstOperator->m_lpfnOperFunc = oprMoreStrStr;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_DATE && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_DATE)
			pstOperator->m_lpfnOperFunc = oprMoreDateDate;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_BOOL && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_BOOL)
			pstOperator->m_lpfnOperFunc = oprMoreBoolBool;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) != VT_UNKNOWN && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_UNKNOWN)
		{
			pstOperator->m_lpfnOperFunc = oprMore;

			AddError(ALX_PRS_ERROR_OPERAND_MISMATCH);
		}
		else
			pstOperator->m_lpfnOperFunc = oprMore;

		pstOperator->pvarResult->vt = VT_BOOL;
		break;
	case c_nOprLess:	//	<
		if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_R8 && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_R8)
			pstOperator->m_lpfnOperFunc = oprLessNumNum;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_BSTR && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_BSTR)
				pstOperator->m_lpfnOperFunc = oprLessStrStr;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_DATE && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_DATE)
			pstOperator->m_lpfnOperFunc = oprLessDateDate;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_BOOL && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_BOOL)
			pstOperator->m_lpfnOperFunc = oprLessBoolBool;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) != VT_UNKNOWN && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_UNKNOWN)
		{
			pstOperator->m_lpfnOperFunc = oprLess;

			AddError(ALX_PRS_ERROR_OPERAND_MISMATCH);
		}
		else
			pstOperator->m_lpfnOperFunc = oprLess;

		pstOperator->pvarResult->vt = VT_BOOL;
		break;
	case c_nOprMoreEqual:	//	>=
		if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_R8 && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_R8)
			pstOperator->m_lpfnOperFunc = oprMoreEqualNumNum;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_BSTR && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_BSTR)
				pstOperator->m_lpfnOperFunc = oprMoreEqualStrStr;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_DATE && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_DATE)
			pstOperator->m_lpfnOperFunc = oprMoreEqualDateDate;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_BOOL && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_BOOL)
			pstOperator->m_lpfnOperFunc = oprMoreEqualBoolBool;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) != VT_UNKNOWN && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_UNKNOWN)
		{
			pstOperator->m_lpfnOperFunc = oprMoreEqual;

			AddError(ALX_PRS_ERROR_OPERAND_MISMATCH);
		}
		else
			pstOperator->m_lpfnOperFunc = oprMoreEqual;

		pstOperator->pvarResult->vt = VT_BOOL;
		break;
	case c_nOprLessEqual:	//	<=
		if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_R8 && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_R8)
			pstOperator->m_lpfnOperFunc = oprLessEqualNumNum;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_BSTR && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_BSTR)
				pstOperator->m_lpfnOperFunc = oprLessEqualStrStr;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_DATE && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_DATE)
			pstOperator->m_lpfnOperFunc = oprLessEqualDateDate;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) == VT_BOOL && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) == VT_BOOL)
			pstOperator->m_lpfnOperFunc = oprLessEqualBoolBool;
		else if(V_VT(pstOperator->pvarArguments[LEFT_OPR]) != VT_UNKNOWN && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_UNKNOWN)
		{
			pstOperator->m_lpfnOperFunc = oprLessEqual;

			AddError(ALX_PRS_ERROR_OPERAND_MISMATCH);
		}
		else
			pstOperator->m_lpfnOperFunc = oprLessEqual;

		pstOperator->pvarResult->vt = VT_BOOL;
		break;
	case c_nOprNo:		//	! or NOT
		if(V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_BOOL && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_UNKNOWN)
			AddError(ALX_PRS_ERROR_OPERAND_MISMATCH);

		pstOperator->m_lpfnOperFunc = oprNo;
		pstOperator->pvarResult->vt = VT_BOOL;
		break;
	case c_nOprAnd:		//	AND
		if((V_VT(pstOperator->pvarArguments[LEFT_OPR]) != VT_BOOL && V_VT(pstOperator->pvarArguments[LEFT_OPR]) != VT_UNKNOWN) || (V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_BOOL && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_UNKNOWN))
			AddError(ALX_PRS_ERROR_OPERAND_MISMATCH);

		pstOperator->m_lpfnOperFunc = oprAnd;
		pstOperator->pvarResult->vt = VT_BOOL;
		break;
	case c_nOprOr:		//	OR
		if((V_VT(pstOperator->pvarArguments[LEFT_OPR]) != VT_BOOL && V_VT(pstOperator->pvarArguments[LEFT_OPR]) != VT_UNKNOWN) || (V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_BOOL && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_UNKNOWN))
			AddError(ALX_PRS_ERROR_OPERAND_MISMATCH);

		pstOperator->m_lpfnOperFunc = oprOr;
		pstOperator->pvarResult->vt = VT_BOOL;
		break;
	case c_nOprFind:		//	$
		if((V_VT(pstOperator->pvarArguments[LEFT_OPR]) != VT_BSTR && V_VT(pstOperator->pvarArguments[LEFT_OPR]) != VT_UNKNOWN) || (V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_BSTR && V_VT(pstOperator->pvarArguments[RIGHT_OPR]) != VT_UNKNOWN))
			AddError(ALX_PRS_ERROR_OPERAND_MISMATCH);

		pstOperator->m_lpfnOperFunc = oprFind;
		pstOperator->pvarResult->vt = VT_BSTR;
		break;
	case c_nOprNULL:
		pstOperator->m_lpfnOperFunc = oprNULL;
		pstOperator->pvarResult->vt = VT_UNKNOWN;
		break;
	default:
		AddError(ALX_PRS_ERROR_OPERAND_MISMATCH);
	}
}

VARIANT* CALXParser::AddOperators(ALXPRS_OPERATOR_INFO_ITEM* pFirstOperatorInfo)
{
	ALXPRS_OPERATOR_INFO_ITEM* pSelOperatorInfo;

	VARIANT *pvarLeftOpr, *pvarRightOpr, *pvarResult;

	pSelOperatorInfo = pFirstOperatorInfo;
	while(pSelOperatorInfo != NULL)
	{
		pvarResult = NULL;

		int iPrev = m_indCurPtrOperFunc - 1;

		// if at the left there is no operator
		if(pSelOperatorInfo->m_pliPrevItem == NULL)
		{
			// get value of the left operand
			pvarLeftOpr = GetResult(pSelOperatorInfo->m_stItem.m_pLeftOper);
			pvarResult = NewVariable();
		}
		else
		{
			// value of the left operand is the result of performance of the previous operator
			pvarLeftOpr = m_pstOperFuncs[iPrev].pvarResult;
			pvarResult = pvarLeftOpr;
		}

		// get value of the right operand
		pvarRightOpr = GetResult(pSelOperatorInfo->m_stItem.m_pRightOper);

		// select memory for storage of pointers on operands
		m_pstOperFuncs[m_indCurPtrOperFunc].pvarArguments = new VARIANT*[2];

		m_pstOperFuncs[m_indCurPtrOperFunc].pvarArguments[LEFT_OPR] = pvarLeftOpr;
		m_pstOperFuncs[m_indCurPtrOperFunc].pvarArguments[RIGHT_OPR] = pvarRightOpr;
		m_pstOperFuncs[m_indCurPtrOperFunc].pvarResult = pvarResult;
		
		AddOperator(pSelOperatorInfo->m_stItem.m_nOperID, &m_pstOperFuncs[m_indCurPtrOperFunc]);

		m_indCurPtrOperFunc++;

		pSelOperatorInfo = pSelOperatorInfo->m_pliNextItem;
	}

	// delete list
	ALXPRS_OPERATOR_INFO_ITEM* pCurOperatorInfo = pFirstOperatorInfo;
	while(pCurOperatorInfo != NULL)
	{
		pSelOperatorInfo = pCurOperatorInfo;
		pCurOperatorInfo = pCurOperatorInfo->m_pliNextItem;

		delete pSelOperatorInfo;
	}

	return pvarResult;
}

VARIANT* CALXParser::GetResult(void* pInfo)
{
	VARIANT* pvarResult = &m_pOperandArguments[0];;
	if(pInfo != NULL)
	{
		switch(*((short*)pInfo))
		{
		case AOI_TYPE_CONSTVAR:
			pvarResult = GetValue((ALXPRS_CONST_VAR_INFO*) pInfo);
			break;
		case AOI_TYPE_FUNCTION:
			pvarResult = AddFunction((ALXPRS_FUNCTION_INFO*)pInfo);
			break;
		case AOI_TYPE_LIST_OPERATOR:
			{
				ALXPRS_OPER_INFO_PTR* pOperatorsPtr = (ALXPRS_OPER_INFO_PTR*) pInfo;
				pvarResult = AddOperators(pOperatorsPtr->m_pFirstOperator);
				delete pOperatorsPtr;
				break;
			}
		}
	}

	return pvarResult;
}

VARIANT* CALXParser::AddFunction(ALXPRS_FUNCTION_INFO* pFunctionInfo)
{
	VARIANT **pvarArguments, *pvarResult = NewVariable();
	void (FAR * lpfnOperFunc) (VARIANT* pvarArguments[], VARIANT* pvarResult) = NULL;

	ALXPRS_DEF_FUNCTION* pstDefFuncInfo = pFunctionInfo->m_pstFuncInfo;
	int arg_count = pFunctionInfo->m_nArgCount;	// argument count

	if(pstDefFuncInfo != NULL)
	{
		lpfnOperFunc = pstDefFuncInfo->m_lpfnFunction;
		pvarResult->vt = pstDefFuncInfo->m_nTypeResult;

		// if there should be arguments
		if(arg_count > 0)
		{
			// process the arguments

			pvarArguments = new VARIANT*[arg_count];

			ALXPRS_ARGUMENT_INFO_ITEM* pCurArgItem = pFunctionInfo->m_pFirstArgItem;

			if(pstDefFuncInfo->m_nDefArgCount == ANY_ARG_COUNT)
				pvarArguments[arg_count-1] = NULL;

			int j = 0;
			// process the transferred arguments
			while(pCurArgItem != NULL)
			{
				if(pCurArgItem->m_stItem == NULL)
					// get default value
					pvarArguments[j] = GetValue(pstDefFuncInfo->m_arDefArguments[j].m_nType, (LPTSTR)pstDefFuncInfo->m_arDefArguments[j].m_szDefVal);
				else
					pvarArguments[j] = GetResult(pCurArgItem->m_stItem);

				if(j < pstDefFuncInfo->m_nDefArgCount)
				{
					if(pstDefFuncInfo->m_nDefArgCount != ANY_ARG_COUNT)
					{
						UINT nArgType = pstDefFuncInfo->m_arDefArguments[j].m_nType;
						if(pvarArguments[j]->vt != nArgType && nArgType != VT_UNKNOWN)
							AddError(ALX_PRS_ERROR_ARGTYPE_MISMATCH, pstDefFuncInfo->m_szName, j + 1);
					}
					else if(j > 0 && pvarArguments[j]->vt != pvarArguments[0]->vt && pvarArguments[0]->vt != VT_UNKNOWN && pvarArguments[j]->vt != VT_UNKNOWN)
						AddError(ALX_PRS_ERROR_ARGTYPE_MISMATCH, pstDefFuncInfo->m_szName, j + 1);
				}

				pCurArgItem = pCurArgItem->m_pliNextItem;
				j++;
			}
			// process the arguments by default
			while(j < pstDefFuncInfo->m_nDefArgCount)
			{
				// get default value
				pvarArguments[j] = GetValue(pstDefFuncInfo->m_arDefArguments[j].m_nType, (LPTSTR)pstDefFuncInfo->m_arDefArguments[j].m_szDefVal);
				j++;
			}
		}
		else
			pvarArguments = NULL;
	}
	else
	{
		lpfnOperFunc = oprNULL;
		pvarResult->vt = VT_UNKNOWN;

		// if there should be arguments
		if(arg_count > 0)
		{
			// process the arguments

			pvarArguments = new VARIANT*[arg_count];

			ALXPRS_ARGUMENT_INFO_ITEM* pCurArgItem = pFunctionInfo->m_pFirstArgItem;

			int j = 0;
			// process the transferred arguments
			while(pCurArgItem != NULL)
			{
				if(pCurArgItem->m_stItem == NULL)
					// get default value
					pvarArguments[j] = GetValue(pstDefFuncInfo->m_arDefArguments[j].m_nType, (LPTSTR)pstDefFuncInfo->m_arDefArguments[j].m_szDefVal);
				else
					pvarArguments[j] = GetResult(pCurArgItem->m_stItem);

				pCurArgItem = pCurArgItem->m_pliNextItem;
				j++;
			}
		}
		else
			pvarArguments = NULL;
	}

	m_pstOperFuncs[m_indCurPtrOperFunc].m_lpfnOperFunc = lpfnOperFunc;
	m_pstOperFuncs[m_indCurPtrOperFunc].pvarArguments = pvarArguments;
	m_pstOperFuncs[m_indCurPtrOperFunc].pvarResult = pvarResult;

	m_indCurPtrOperFunc++;


	// delete temporary info
	ALXPRS_ARGUMENT_INFO_ITEM* pCurArgItem = pFunctionInfo->m_pFirstArgItem;
	while(pCurArgItem != NULL)
	{
		pFunctionInfo->m_pFirstArgItem = pCurArgItem;
		pCurArgItem = pCurArgItem->m_pliNextItem;

		delete pFunctionInfo->m_pFirstArgItem;
	}

	delete pFunctionInfo;

	return pvarResult;
}

VARIANT* CALXParser::GetValue(ALXPRS_CONST_VAR_INFO* pConstVarInfo)
{
	VARIANT* pvarResult = GetValue(pConstVarInfo->m_nType, pConstVarInfo->m_szExpr);
	delete[] pConstVarInfo->m_szExpr;
	delete pConstVarInfo;

	return pvarResult;
}

VARIANT* CALXParser::GetValue(UINT nType, LPTSTR szExpr)
{
	VARIANT* pvarValue = &m_pOperandArguments[m_nValueCount];
	m_nValueCount++;

	switch(nType)
	{
	case VT_UNKNOWN:
		{
			ALXPRS_VARIABLE_ITEM* pVariable = GetVariableItem(m_pLastUsedVariable, szExpr);
			if(pVariable != NULL)
				pvarValue = &pVariable->m_stItem.m_varValue;
			else
			{
				pVariable = SetVariableUsed(szExpr);
				if(pVariable != NULL)
					pvarValue = &pVariable->m_stItem.m_varValue;
				else
				{
					AddError(ALX_PRS_ERROR_VARIABLE_NOT_FOUND, szExpr);
					break;
				}
			}
			m_nValueCount--;
		break;
		}
	case VT_R8:
		V_R8(pvarValue) = atof(szExpr);
		V_VT(pvarValue) = VT_R8;
		break;
	case VT_DATE:
		{
			COleDateTime dte;
			dte.ParseDateTime(szExpr);
			V_DATE(pvarValue) = dte;
			V_VT(pvarValue) = VT_DATE;
		break;
		}
	case VT_BSTR:
		{
			CString strExpr(szExpr);
			V_BSTR(pvarValue) = strExpr.AllocSysString();
			V_VT(pvarValue) = VT_BSTR;
		break;
		}
	case VT_BOOL:
		if(_stricmp(szExpr, "T") == 0 || _stricmp(szExpr, "Y") == 0)
			V_BOOL(pvarValue) = AFX_OLE_TRUE;
		else
			V_BOOL(pvarValue) = AFX_OLE_FALSE;
		V_VT(pvarValue) = VT_BOOL;
		break;
	}

	return pvarValue;
}

VARIANT* CALXParser::Execute()
{
	// execute the opearators and functions
	for(int i = 0; i < m_nOperatorCount + m_nFunctionCount; i++)
	{
		(*m_pstOperFuncs[i].m_lpfnOperFunc)(m_pstOperFuncs[i].pvarArguments,m_pstOperFuncs[i].pvarResult);
	}
	
	return m_pvarResult;
}

LPSTR CALXParser::GetExpr(CHAR cTerminator)
{
	LPSTR szResult = NULL;
	LPCSTR szCurChar = m_lpszExp;

	while(*szCurChar != '\0' && *szCurChar != '\n' && *szCurChar != '\r' && *szCurChar != cTerminator)
		szCurChar++;

	int nLen = (szCurChar - m_lpszExp)/sizeof(CHAR);
	if(nLen > 0)
	{
		szResult = new CHAR[nLen + 1];
		memcpy(szResult, m_lpszExp, nLen);	szResult[nLen] = '\0';
		m_lpszExp = szCurChar;

		if(*m_lpszExp != cTerminator)
			AddError(ALX_PRS_ERROR_MISSING, cTerminator);
	}
	
	return szResult;
}

LPSTR CALXParser::GetNumberExpr()
{
	LPSTR szResult = NULL;
	LPCSTR szCurChar = m_lpszExp;

	if(*szCurChar >= '0' && *szCurChar <= '9' || *szCurChar == '.' || *szCurChar == '-')
	{
		szCurChar++;

		while(*szCurChar >= '0' && *szCurChar <= '9' || *szCurChar == '.')
			szCurChar++;

		int nLen = (szCurChar - m_lpszExp)/sizeof(CHAR);

		if(nLen > 1 || (nLen == 1 && *m_lpszExp != '.' && *m_lpszExp != '-') || (nLen == 2 && *m_lpszExp != '-' && m_lpszExp[1] != '.'))
		{
			szResult = new CHAR[nLen + 1];
			memcpy(szResult, m_lpszExp, nLen);	szResult[nLen] = '\0';
			m_lpszExp = szCurChar;
		}
	}

	return szResult;
}

LPSTR CALXParser::GetSymbolExpr()
{
	LPSTR szResult = NULL;
	LPCSTR szCurChar = m_lpszExp;

	CHAR ch = *szCurChar;
	while((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || ch == '_')
	{
		szCurChar++;
		ch = *szCurChar;
	}

	int nLen = (szCurChar - m_lpszExp)/sizeof(CHAR);
	if(nLen > 0)
	{
		szResult = new CHAR[nLen + 1];
		memcpy(szResult, m_lpszExp, nLen);	szResult[nLen] = '\0';
		m_lpszExp = szCurChar;
	}
	else
		AddError(ALX_PRS_ERROR_SYNTAX_ERROR, m_lpszExp);

	return szResult;
}

ALXPRS_VARIABLE_ITEM* CALXParser::GetVariableItem(ALXPRS_VARIABLE_ITEM* pLastVariable, LPCSTR szName)
{
	ALXPRS_VARIABLE_ITEM* pVariable = pLastVariable;
	while(pVariable != NULL)
	{
		if(_stricmp(pVariable->m_stItem.m_szName, szName) == 0)
			break;

		pVariable = pVariable->m_pliPrevItem;
	}

	return pVariable;
}

ALXPRS_VARIABLE_ITEM* CALXParser::GetVariableItem(LPCSTR szName)
{
	ALXPRS_VARIABLE_ITEM* pVariable = GetVariableItem(m_pLastUsedVariable, szName);

	if(pVariable == NULL)
		pVariable = GetVariableItem(m_pLastVariable, szName);

	return pVariable;
}

BOOL CALXParser::AddVariable(LPCSTR szName, VARIANT& varValue, DWORD dwKey /* = 0*/)
{
	ALXPRS_VARIABLE_ITEM* pVariable = GetVariableItem(szName);

	if(pVariable != NULL)
		return FALSE;

	ALXPRS_VARIABLE_ITEM* pPrevVariable = m_pLastVariable;

	m_pLastVariable = new ALXPRS_VARIABLE_ITEM;

	m_pLastVariable->m_stItem.m_szName = new CHAR[strlen(szName)+1];
	strcpy(m_pLastVariable->m_stItem.m_szName, szName);
	prsCopyVariant(&m_pLastVariable->m_stItem.m_varValue, &varValue);
	m_pLastVariable->m_stItem.m_dwKey = dwKey;

	m_pLastVariable->m_pliPrevItem = pPrevVariable;

	return TRUE;
}

BOOL CALXParser::DelVariable(LPCSTR szName)
{
	BOOL bFind = FALSE;
	ALXPRS_VARIABLE_ITEM *pVariable, *pNextVariable;
	pNextVariable = NULL;
	pVariable = m_pLastVariable;
	while(pVariable != NULL)
	{
		if(_stricmp(pVariable->m_stItem.m_szName, szName) == 0)
		{
			bFind = TRUE;
			break;
		}

		pNextVariable = pVariable;
		pVariable = pVariable->m_pliPrevItem;
	}

	if(bFind)
	{
		if(pNextVariable != NULL)
			pNextVariable->m_pliPrevItem = pVariable->m_pliPrevItem;

		::VariantClear(&pVariable->m_stItem.m_varValue);
		delete[] pVariable->m_stItem.m_szName;

		delete pVariable;
	}

	return bFind;
}

void CALXParser::DelUnusedVariables()
{
	ALXPRS_VARIABLE_ITEM* pDelVariable;

	while(m_pLastVariable != NULL)
	{
		pDelVariable = m_pLastVariable;
		m_pLastVariable = m_pLastVariable->m_pliPrevItem;

		::VariantClear(&pDelVariable->m_stItem.m_varValue);
		delete[] pDelVariable->m_stItem.m_szName;

		delete pDelVariable;
	}
}

BOOL CALXParser::SetVariableValue(LPCSTR szName, VARIANT& varValue)
{
	ALXPRS_VARIABLE_ITEM* pVariable = GetVariableItem(szName);

	if(pVariable == NULL)
		return FALSE;

	prsCopyVariant(&pVariable->m_stItem.m_varValue, &varValue);

	return TRUE;
}

BOOL CALXParser::SetVariableValue(VARIANT& varValue)
{
	if(m_pCurentVariable == NULL)
		return FALSE;

	prsCopyVariant(&m_pCurentVariable->m_stItem.m_varValue, &varValue);

	return TRUE;
}

BOOL CALXParser::GetVariableValue(LPCSTR szName, VARIANT& varValue)
{
	ALXPRS_VARIABLE_ITEM* pVariable = GetVariableItem(szName);

	if(pVariable == NULL)
		return FALSE;

	::VariantClear(&varValue);
	::VariantCopy(&varValue, &pVariable->m_stItem.m_varValue);

	return TRUE;
}

BOOL CALXParser::GetVariableValue(VARIANT& varValue)
{
	if(m_pCurentVariable == NULL)
		return FALSE;

	VariantCopy(&varValue, &m_pCurentVariable->m_stItem.m_varValue);

	return TRUE;
}

DWORD CALXParser::GetVariableKey()
{
	return m_pCurentVariable->m_stItem.m_dwKey;
}

LPCSTR CALXParser::GetFirstVariableName()
{
	m_pCurentVariable = NULL;
	m_bCurVariableUsed = FALSE;

	if(m_pLastUsedVariable != NULL)
	{
		m_pCurentVariable = m_pLastUsedVariable;
		m_bCurVariableUsed = TRUE;
	}
	else
		m_pCurentVariable = m_pLastVariable;

	return m_pCurentVariable != NULL ? m_pCurentVariable->m_stItem.m_szName : NULL;
}

LPCSTR CALXParser::GetNextVariableName()
{
	if(m_pCurentVariable != NULL)
	{
		m_pCurentVariable = m_pCurentVariable->m_pliPrevItem;

		if(m_pCurentVariable == NULL && m_bCurVariableUsed)
		{
			m_pCurentVariable = m_pLastVariable;
			m_bCurVariableUsed = FALSE;
		}
	}
	return m_pCurentVariable != NULL ? m_pCurentVariable->m_stItem.m_szName : NULL;
}

ALXPRS_VARIABLE_ITEM* CALXParser::SetVariableUsed(LPCSTR szName)
{
	ALXPRS_VARIABLE_ITEM* pNextVariable = NULL;
	ALXPRS_VARIABLE_ITEM* pVariable = m_pLastVariable;
	while(pVariable != NULL)
	{
		if(_stricmp(pVariable->m_stItem.m_szName, szName) == 0)
		{
			// delete from the list of not used variables
			if(pNextVariable != NULL)
				pNextVariable->m_pliPrevItem = pVariable->m_pliPrevItem;
			else
				m_pLastVariable = pVariable->m_pliPrevItem;

			// add in the list of used variables
			pVariable->m_pliPrevItem = m_pLastUsedVariable;
			m_pLastUsedVariable = pVariable;

			break;
		}

		pNextVariable = pVariable;
		pVariable = pVariable->m_pliPrevItem;
	}

	return pVariable;
}

ALXPRS_DEF_FUNCTION* CALXParser::FindFunction(LPCSTR szName)
{
	int i = 0;
	int comlen = max(4,strlen(szName));

	// touch functions
	while(i < alxFuncCount)
	{
		// if it is that function
		if(_strnicmp(m_alxFunList[i].m_szName, szName, comlen) == 0)
			return &m_alxFunList[i];
		i++;
	}

	return NULL;
}

void AFX_CDECL CALXParser::AddError(UINT nIDS, ...)
{
	CString strError;
	if(strError.LoadString(nIDS))
	{
		va_list argList;
		va_start(argList, nIDS);
		strError.FormatV(CString(strError), argList);
		va_end(argList);
	}
	else
	{
		strError.Format("00000000%X",nIDS);
		strError = "Error number 0x" + strError.Right(8) + ".";
	}

	TRACE(strError+"\n");

	CALXParserErrorInfoItem* pErrorItem = new CALXParserErrorInfoItem; 
	pErrorItem->m_stErrorInfo.m_nErrorCode = nIDS;
	pErrorItem->m_stErrorInfo.m_strDescription = strError;
	pErrorItem->m_pstPrevErrorItem = m_pLastError;
	m_pLastError = pErrorItem;
}

void CALXParser::SetTmpResult(void** pResult, ALXPRS_OPERATOR_INFO_ITEM* pOperInfo, void* pOperandInfo)
{
	if(pOperInfo == NULL)
		return;

	if(pOperandInfo != NULL && pOperInfo->m_stItem.m_pRightOper == NULL)
		pOperInfo->m_stItem.m_pRightOper = pOperandInfo;

	if(*pResult != NULL)
		return;

	ALXPRS_OPER_INFO_PTR* pNewListOperPtr = new ALXPRS_OPER_INFO_PTR;
	pNewListOperPtr->m_nTypeStruct = AOI_TYPE_LIST_OPERATOR;
	pNewListOperPtr->m_pFirstOperator = pOperInfo;
	
	*pResult = pNewListOperPtr;
}

void* CALXParser::SelTmpResult(void** pResult, ALXPRS_OPERATOR_INFO_ITEM* pOperInfo, void* pOperandInfo)
{
	if(pOperInfo != NULL)
	{
		if(*pResult == NULL)
			*pResult = pOperInfo;

		if(pOperandInfo == NULL)
			AddError(ALX_PRS_ERROR_MISSING_OPERAND);
		else
			pOperInfo->m_stItem.m_pRightOper = pOperandInfo;

		return *pResult;
	}

	ASSERT((*pResult) == NULL);

	return pOperandInfo;
}