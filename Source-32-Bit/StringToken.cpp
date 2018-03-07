// StringToken.cpp: implementation of the CStringToken class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StringToken.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStringToken::CStringToken(CString str, CString delim)
{
	currentPosition = 0;
	this->str = str;
	maxPosition = str.GetLength();
	delimiters = delim;
}

CStringToken::CStringToken(CString str)
{
	currentPosition = 0;
	this->str = str;
	maxPosition = str.GetLength();
	delimiters = " \t\n\r";
}

CStringToken::~CStringToken()
{

}


void CStringToken::SkipDelimiters()
{
	while ((currentPosition < maxPosition) &&
	       (delimiters.Find(str.GetAt(currentPosition)) >= 0)) 
	{
	    currentPosition++;
	}

}


bool CStringToken::HasMoreTokens()
{
	SkipDelimiters();
	return (currentPosition < maxPosition);
}

CString CStringToken::NextToken()
{
	SkipDelimiters();

	if (currentPosition >= maxPosition) 
		return "";

	int start = currentPosition;
	while ((currentPosition < maxPosition) && 
	       (delimiters.Find(str.GetAt(currentPosition)) < 0)) 
	{
	    currentPosition++;
	}
	
	return str.Mid(start, currentPosition-start);
}

CString CStringToken::LeftOut()
{
	SkipDelimiters();
	if (currentPosition >= maxPosition) 
		return "";
	return str.Mid(currentPosition);
}
