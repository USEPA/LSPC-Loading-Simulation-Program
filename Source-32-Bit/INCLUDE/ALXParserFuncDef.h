/////////////////////////////////////////////////////////////////////////////
//
// FuncDef.h : header file
//
/////////////////////////////////////////////////////////////////////////////
/*
 Copyright© 2002, Alexey Dolgachov
 mail:	alxsoft@gazinter.net	
 web:	http://www.alxsoft.narod.ru
*/
/////////////////////////////////////////////////////////////////////////////
#if !defined(ALX_FUNCDEF_H_INCLUDED_)
#define ALX_FUNCDEF_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// Structures definition

// A structure with the description of argument of functions
typedef struct alxArgument
{
	UINT	m_nType;
	LPCSTR	m_szDefVal;
} ALXPRS_DEF_ARGUMENT;

// A structure with the description of function
typedef struct alxFunction
{
	LPCSTR		m_szName;
	void (FAR * m_lpfnFunction) (VARIANT* pvarArguments[], VARIANT* pvarResult);
	UINT		m_nTypeResult;
	short		m_nDefArgCount;
	short		m_nMinArgCount;
	ALXPRS_DEF_ARGUMENT* m_arDefArguments;
} ALXPRS_DEF_FUNCTION;


/////////////////////////////////////////////////////////////////////////////
// Helper functions

VARIANT_BOOL alxLike(LPCSTR szExpr, LPCSTR szWildExpr);


/////////////////////////////////////////////////////////////////////////////
// Functions definition

void FAR funABS(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funALLTRIM(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funANSITOOEM(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funASC(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funAT(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funATAN(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funATC(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funBETWEEN(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funCDOW(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funCEILING(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funCHR(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funCHRTRAN(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funCMONTH(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funCOS(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funCTOT(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funDATE(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funDATETIME(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funDAY(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funDMY(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funDTOC(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funDTOS(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funEMPTY(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funEXP(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funFLOOR(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funGOMONTH(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funHOUR(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funIIF(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funINT(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funISALPHA(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funISDIGIT(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funISLOWER(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funISUPPER(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funLEFT(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funLEN(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funLIKE(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funLOG(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funLOG10(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funLOWER(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funLTRIM(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funMAX(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funMDY(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funMIN(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funMINUTE(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funMOD(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funMONTH(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funOCCURS(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funOEMTOANSI(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funPADC(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funPADL(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funPADR(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funPI(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funREPLICATE(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funRIGHT(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funROUND(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funRTRIM(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funSEC(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funSECONDS(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funSIGN(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funSIN(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funSPACE(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funSQRT(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funSTR(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funSTRTRAN(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funSTUFF(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funSUBSTR(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funTAN(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funTIME(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funUPPER(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funVARTYPE(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funVAL(VARIANT* pvarArguments[], VARIANT* pvarResult);
void FAR funYEAR(VARIANT* pvarArguments[], VARIANT* pvarResult);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_FUNCDEF_H_INCLUDED_)
