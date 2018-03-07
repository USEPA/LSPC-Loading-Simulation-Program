// StringToken.h: interface for the CStringToken class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGTOKEN_H__A3D833AB_BD4A_11D3_A495_002078143FF2__INCLUDED_)
#define AFX_STRINGTOKEN_H__A3D833AB_BD4A_11D3_A495_002078143FF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStringToken  
{
public:
	void SkipDelimiters();
	int currentPosition;
    int maxPosition;
    CString str;
    CString delimiters;

public:
	CString NextToken();
	bool HasMoreTokens();

public:
	CString LeftOut();
	CStringToken(CString str);
	CStringToken(CString str, CString delim);
	virtual ~CStringToken();

};

#endif // !defined(AFX_STRINGTOKEN_H__A3D833AB_BD4A_11D3_A495_002078143FF2__INCLUDED_)
