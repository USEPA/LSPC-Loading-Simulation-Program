/////////////////////////////////////////////////////////////////////////////
//
// ALXParser.h : header file
//
/////////////////////////////////////////////////////////////////////////////
/*
 Copyright© 2002, Alexey Dolgachov
 mail:	alxsoft@gazinter.net	
 web:	http://www.alxsoft.narod.ru
*/
/////////////////////////////////////////////////////////////////////////////
#if !defined(ALX_PARSER_H_INCLUDED_)
#define ALX_PARSER_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ALXParserDef.h"
#include "ALXParserOperDef.h"
#include "ALXParserFuncDef.h"
#include "ALXParserException.h"

#define AOI_TYPE_CONSTVAR		0x00
#define AOI_TYPE_FUNCTION		0x01
#define AOI_TYPE_LIST_OPERATOR	0x02

// Temporary structures used at syntactic analysis

	// The information about constant/variable
	typedef struct prsConstVarInfo
	{
		short	m_nTypeStruct;	// type
		LPTSTR	m_szExpr;		// symbolical expression
		UINT	m_nType;		// type of the data
	} ALXPRS_CONST_VAR_INFO;

	// The information on the operator
	typedef struct prsOperatorInfo
	{
		short	m_nOperID;		// operator ID
		void*	m_pLeftOper;	// the pointer on structure with the information on the left operand
		void*	m_pRightOper;	// the pointer on structure with the information on the right operand
	} ALXPRS_OPERATOR_INFO;

	// List operators item
	typedef struct prliOperatorInfoListItem
	{
		ALXPRS_OPERATOR_INFO m_stItem;				// operator info
		prliOperatorInfoListItem* m_pliPrevItem;	// the pointer on the prev item
		prliOperatorInfoListItem* m_pliNextItem;	// the pointer on the next item
	} ALXPRS_OPERATOR_INFO_ITEM;

	// Pointer on the list operators
	typedef struct prsOperatorsPtrInfo
	{
		short	m_nTypeStruct;							// type
		ALXPRS_OPERATOR_INFO_ITEM*	m_pFirstOperator;	// pointer on the first operator
	} ALXPRS_OPER_INFO_PTR;

	// Function argument list item
	typedef struct prliArgumentInfoListItem
	{
		void* m_stItem;								// argument info
		prliArgumentInfoListItem* m_pliPrevItem;	// the pointer on the prev item
		prliArgumentInfoListItem* m_pliNextItem;	// the pointer on the next item
	} ALXPRS_ARGUMENT_INFO_ITEM;

	// Èíôîðìàöèÿ î ôóíêöèè
	typedef struct prsFunctionInfo
	{
		short		m_nTypeStruct;					// type
		short		m_nArgCount;					// the received arguments count
		ALXPRS_DEF_FUNCTION*	m_pstFuncInfo;		// pointer on the function info
		ALXPRS_ARGUMENT_INFO_ITEM*	m_pFirstArgItem;// pointer on first argument
	} ALXPRS_FUNCTION_INFO;


// The saved structures for execute expression

	// structure for save pointer on function/operator
	// and pointer on the list arguments
	typedef struct prsOperatorFunction
	{
		void (FAR * m_lpfnOperFunc) (VARIANT* pvarArguments[], VARIANT* pvarResult);
		VARIANT** pvarArguments;
		VARIANT* pvarResult;
	} ALXPRS_OPERFUNC;

// Structures for storage of variables

	// structures for storage of variables value
	typedef struct prsVariable
	{
		LPTSTR	m_szName;	// nane variable
		VARIANT m_varValue;	// value
		DWORD	m_dwKey;	// key
	} ALXPRS_VARIABLE;

	// List variables item
	typedef struct prliVariableListItem
	{
		ALXPRS_VARIABLE m_stItem;				// variable info
		prliVariableListItem* m_pliPrevItem;	// the pointer on the prev item
	} ALXPRS_VARIABLE_ITEM;


/////////////////////////////////////////////////////////////////////////////
// Helper functions

BOOL alxIsBoolExpr(LPCSTR szExpr);
BOOL alxIsLogicOprExpr(LPCSTR szExpr, short& nOprID, int& nOprPrioritet);

/////////////////////////////////////////////////////////////////////////////
// CALXParser definition

class ALX_EXT_CLASS CALXParser : public CObject
{
	DECLARE_DYNAMIC(CALXParser)

public:
	CALXParser();

// Attributes
private:
	LPCSTR	m_lpszExp;	// pointer on the expression

// list function info
	ALXPRS_DEF_FUNCTION* m_alxFunList;
	int m_alxFuncCount;

// ready structures remain after the ending of analysis
	VARIANT*			m_pvarResult;
	VARIANT*			m_pOperandArguments;
	ALXPRS_OPERFUNC*	m_pstOperFuncs;

//	counters
	int m_nFunctionCount;
	int m_nOperatorCount;
	int m_nValueCount;
	int m_nTmpValueCount;
	int	m_indCurPtrOperFunc;

// define variables (may be used in expression)
	ALXPRS_VARIABLE_ITEM* m_pLastUsedVariable;	// variables used in expression
	ALXPRS_VARIABLE_ITEM* m_pLastVariable;		// others
	ALXPRS_VARIABLE_ITEM* m_pCurentVariable;	// curent variable
	BOOL	m_bCurVariableUsed;					// whether the current variable in expression is used

	// pointer on last error
	CALXParserErrorInfoItem*	m_pLastError;

// Operations
public:
// Base function
	void Parse(LPCSTR lpszExpression);	// analysis of expression
	VARIANT* Execute(); // process execute 

// The functions for work witch variables

	BOOL AddVariable(LPCSTR szName, VARIANT& varValue, DWORD dwKey = 0);
	BOOL DelVariable(LPCSTR szName);
	void DelUnusedVariables();

	BOOL SetVariableValue(LPCSTR szName, VARIANT& varValue);
	BOOL SetVariableValue(VARIANT& varValue);
	BOOL GetVariableValue(LPCSTR szName, VARIANT& varValue);
	BOOL GetVariableValue(VARIANT& varValue);
	DWORD GetVariableKey();

	LPCSTR GetFirstVariableName();
	LPCSTR GetNextVariableName();

// Overrides

// Implementation
public:

	virtual ~CALXParser();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	void ClearData();		// clear data

// Syntactic analysis
	void* DoParse(int nPrioritet = INT_MAX, ALXPRS_OPERATOR_INFO_ITEM* pOperInfo = NULL, void* pOperandInfo = NULL);
	LPSTR	GetExpr(CHAR cTerminator); // returns substring terminating in the specified symbol
	LPSTR	GetNumberExpr();	// returns numerical expression
	LPSTR	GetSymbolExpr();	// returns symbol expression

// Creation of time structures at syntactic analysis
	void	SetTmpResult(void** pResult, ALXPRS_OPERATOR_INFO_ITEM* pOperInfo, void* pOperandInfo);
	void*	SelTmpResult(void** pResult, ALXPRS_OPERATOR_INFO_ITEM* pOperInfo, void* pOperandInfo);
	void*	AddValueInfo(LPSTR szExpr, UINT nType, ALXPRS_OPERATOR_INFO_ITEM** ppOperInfo, void** ppOperandInfo);
	void	AddOperatorsInfo(short nOperID, int nPrioritet, int& nCurPrioritet, ALXPRS_OPERATOR_INFO_ITEM** ppOperInfo, void** ppOperandInfo);
	void*	AddFunctionInfo(LPSTR szName);
	ALXPRS_ARGUMENT_INFO_ITEM*
		AddArgumentInfo(ALXPRS_ARGUMENT_INFO_ITEM* pArgumentItem, void* pArgumentInfo);

	ALXPRS_DEF_FUNCTION*	FindFunction(LPCSTR szName);

// Transformation of temporary structures to structures for fast execution
	VARIANT*	GetResult(void* pInfo);
	VARIANT*	GetValue(UINT nType, LPTSTR szExpr);
	VARIANT*	GetValue(ALXPRS_CONST_VAR_INFO* pConstVarInfo);
	VARIANT*	AddOperators(ALXPRS_OPERATOR_INFO_ITEM* pFirstOperator);
	void		AddOperator(const short nOperID, ALXPRS_OPERFUNC* pstOperator);
	VARIANT*	AddFunction(ALXPRS_FUNCTION_INFO* pFunctionInfo);

	VARIANT* NewVariable();	// returns the pointer on yet not used variable

// Manipulations with variables
	ALXPRS_VARIABLE_ITEM* GetVariableItem(ALXPRS_VARIABLE_ITEM* pFirstVariable, LPCSTR szName);
	ALXPRS_VARIABLE_ITEM* GetVariableItem(LPCSTR szName);
	ALXPRS_VARIABLE_ITEM* SetVariableUsed(LPCSTR szName);

	// addition of a error in the list of errors
	void AFX_CDECL AddError(UINT nIDS, ...);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_PARSER_H_INCLUDED_)
