// OUTPUT.cpp: implementation of the COUTPUT class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OUTPUT.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COUTPUT::COUTPUT()
{
	int i;
	for (i=0; i<PARA_NUM; i++)
		blistOutput[i] = false;

	for(i=0; i<11; i++)
		rqflag_s[i] = 0;

	rqname_s[0] = "BOD";
	rqname_s[1] = "NOX";
	rqname_s[2] = "TAM";
	rqname_s[3] = "SNH4";
	rqname_s[4] = "PO4";
	rqname_s[5] = "SPO4";
	rqname_s[6] = "ORN";
	rqname_s[7] = "ORP";
	rqname_s[8] = "ORC";
	rqname_s[9] = "DO";
	rqname_s[10] = "CO2";

	pHYDRO_LANDSMRY = NULL;
	pSED_LANDSMRY	= NULL;
	pGQUAL_LANDSMRY = NULL;
	pRQUAL_LANDSMRY = NULL;
	pHYDUP_STREAMSMRY = NULL;
	pHYDPS_STREAMSMRY = NULL;	
	pHYDIR_STREAMSMRY = NULL;
	pTEMPUP_STREAMSMRY = NULL;
	pTEMPPS_STREAMSMRY = NULL;
	pTEMPIR_STREAMSMRY = NULL;
	pSEDER_STREAMSMRY = NULL;
	pSEDUP_STREAMSMRY = NULL;
	pSEDPS_STREAMSMRY = NULL;
	pSEDIR_STREAMSMRY = NULL;
	pSEDDEPSCR_STREAMSMRY = NULL;
	pGQUALER_STREAMSMRY = NULL;
	pGQUALUP_STREAMSMRY = NULL;
	pGQUALPS_STREAMSMRY = NULL;
	pGQUALIR_STREAMSMRY = NULL;
	pRQUALUP_STREAMSMRY = NULL;
	pRQUALPS_STREAMSMRY = NULL;
	pRQUALIR_STREAMSMRY = NULL;
}

COUTPUT::~COUTPUT()
{
	ResetData();
}

void COUTPUT::ResetData()
{
	if (pHYDRO_LANDSMRY != NULL)
	{
		delete []pHYDRO_LANDSMRY;
		pHYDRO_LANDSMRY = NULL;
	}
	if (pSED_LANDSMRY != NULL)
	{
		delete []pSED_LANDSMRY;
		pSED_LANDSMRY	= NULL;
	}
	if (pGQUAL_LANDSMRY != NULL)
	{
		delete []pGQUAL_LANDSMRY;
		pGQUAL_LANDSMRY = NULL;
	}
	if (pRQUAL_LANDSMRY != NULL)
	{
		delete []pRQUAL_LANDSMRY;
		pRQUAL_LANDSMRY = NULL;
	}
	if (pHYDUP_STREAMSMRY != NULL)
	{
		delete []pHYDUP_STREAMSMRY;
		pHYDUP_STREAMSMRY = NULL;
	}
	if (pHYDPS_STREAMSMRY != NULL)
	{
		delete []pHYDPS_STREAMSMRY;
		pHYDPS_STREAMSMRY = NULL;	
	}
	if (pHYDIR_STREAMSMRY != NULL)
	{
		delete []pHYDIR_STREAMSMRY;
		pHYDIR_STREAMSMRY = NULL;
	}
	if (pTEMPUP_STREAMSMRY != NULL)
	{
		delete []pTEMPUP_STREAMSMRY;
		pTEMPUP_STREAMSMRY = NULL;
	}
	if (pTEMPPS_STREAMSMRY != NULL)
	{
		delete []pTEMPPS_STREAMSMRY;
		pTEMPPS_STREAMSMRY = NULL;
	}
	if (pTEMPIR_STREAMSMRY != NULL)
	{
		delete []pTEMPIR_STREAMSMRY;
		pTEMPIR_STREAMSMRY = NULL;
	}
	if (pSEDER_STREAMSMRY != NULL)
	{
		delete []pSEDER_STREAMSMRY;
		pSEDER_STREAMSMRY = NULL;
	}
	if (pSEDUP_STREAMSMRY != NULL)
	{
		delete []pSEDUP_STREAMSMRY;
		pSEDUP_STREAMSMRY = NULL;
	}
	if (pSEDPS_STREAMSMRY != NULL)
	{
		delete []pSEDPS_STREAMSMRY;
		pSEDPS_STREAMSMRY = NULL;
	}
	if (pSEDIR_STREAMSMRY != NULL)
	{
		delete []pSEDIR_STREAMSMRY;
		pSEDIR_STREAMSMRY = NULL;
	}
	if (pSEDDEPSCR_STREAMSMRY != NULL)
	{
		delete []pSEDDEPSCR_STREAMSMRY;
		pSEDDEPSCR_STREAMSMRY = NULL;
	}
	if (pGQUALER_STREAMSMRY != NULL)
	{
		delete []pGQUALER_STREAMSMRY;
		pGQUALER_STREAMSMRY = NULL;
	}
	if (pGQUALUP_STREAMSMRY != NULL)
	{
		delete []pGQUALUP_STREAMSMRY;
		pGQUALUP_STREAMSMRY = NULL;
	}
	if (pGQUALPS_STREAMSMRY != NULL)
	{
		delete []pGQUALPS_STREAMSMRY;
		pGQUALPS_STREAMSMRY = NULL;
	}
	if (pGQUALIR_STREAMSMRY != NULL)
	{
		delete []pGQUALIR_STREAMSMRY;
		pGQUALIR_STREAMSMRY = NULL;
	}
	if (pRQUALUP_STREAMSMRY != NULL)
	{
		delete []pRQUALUP_STREAMSMRY;
		pRQUALUP_STREAMSMRY = NULL;
	}
	if (pRQUALPS_STREAMSMRY != NULL)
	{
		delete []pRQUALPS_STREAMSMRY;
		pRQUALPS_STREAMSMRY = NULL;
	}
	if (pRQUALIR_STREAMSMRY != NULL)
	{
		delete []pRQUALIR_STREAMSMRY;
		pRQUALIR_STREAMSMRY = NULL;
	}
	return;
}

