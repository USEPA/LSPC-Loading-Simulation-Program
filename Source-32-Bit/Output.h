// OUTPUT.h: interface for the COUTPUT class.
//
//////////////////////////////////////////////////////////////////////
#include <afxdb.h>
#include <afxtempl.h>

#if !defined(AFX_OUTPUT_H__A3D833A8_BD4A_11D3_A495_002078143FF2__INCLUDED_)
#define AFX_OUTPUT_H__A3D833A8_BD4A_11D3_A495_002078143FF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PARA_NUM 108

enum para_list
{
	PRECP,							
	AIRTMP,							
	SNOTMP,							
	SNOWF,							
	RAINF,							
	PRAIN,							
	MELT,							
	SNOWE,							
	WYIELD,														
	PACK,							
	PACKF,							
	PACKW,							
	PACKI,							
	PDEPTH,							
	COVINDX,						
	NEGHTS,							
	XLNMELT,						
	RDENPKF,														
	SKYCLEAR,						
	SNOCOV,							
	DULLNESS,						
	ALBEDO,							
	PAKTEMP,						
	DEWTMP,							
	SURS,
	UZS,
	LZS,
	AGWS,
	SURO,
	IFWO,
	AGWO,
	PERO,
	TAET,
	PERC,
	INFIL,							
	GWI,							
	IGWI,							
	AGWI,							
	DEP,							
	AVDEP,							
	HRAD,							
	AVVEL,							
	SAREA,							
	VOLUME,
	RO,
	TAU,
	WSSD,
	SCRSD,
	SOSED,
	SOBER,	
 	SSEDC,			 							 
	LSSED,			
	LRSED,
	LBEDDEP,
	LDEPSCR,
	LROSED,
	SQO,
	WASHQS,
	SCRQS,
	SOQO,
	POQUAL,
	SOQUAL,							
	IOQUAL,							
	GOQUAL,							
	POQC,
	CONC,
	CONCOUT,
	CONCSQAL,
	MATSQAL,
	MATIN,
	MATOUT,
	MATOSQAL,
	DOXC,							
	DOXCMIN,						
	DOXCMAX,						
	DOXCAV,							
	DOXL,
	BODC,							
	BODL,
	NO3C,							
	NO3L,
	TAMC,							
	TAML,
	NO2C,							
	NO2L,
	PO4C,							
	PO4L,
	SNH4C,
	SNH4L,
	SPO4C,
	SPO4L,
	PHYC,							
	PHYL,
	PHYCLAC,						
	BENALC,
	ORNC,							
	ORNL,
	ORPC,							
	ORPL,
	ORCC,							
	ORCL,
	LPH,
	LALK,
	TICC,								
	TICL,
	CO2C,								
	CO2L,
	TEMPC
};

typedef struct
{
	double	area_s;
	double	suro_s;
	double  ifwo_s;
	double  agwo_s;
	double  pero_s;
	double  taet_s;
} HYDRO_LANDSMRY;

typedef struct
{
	double  sedload_s;
} SED_LANDSMRY;

typedef struct
{
	double  suroload_s;
	double  ifwoload_s;
	double  agwoload_s;
	double  uniload_s;
	double  suniload_s;
} GQUAL_LANDSMRY;

typedef struct
{
	double  rqload_s;
} RQUAL_LANDSMRY;

typedef struct
{
	double  usif_s;		// total upstream inflow (ft^3)
	double  luif_s;		// total land inflow (ft^3)
	double  usifbp_s;	// total upstream bypass inflow (ft^3)
	double  luifbp_s;	// total land bypass inflow (ft^3)
	double	ivol_s;		// total inflow (ft^3)
} HYDUP_STREAMSMRY;

typedef struct
{
	double  psif_s;		// pointsource inflow (ft^3)
	double  pswd_s;		// pointsource withdrawal (ft^3)
	double  pswddi_s;	// pointsource withdrawal diverted (ft^3)
} HYDPS_STREAMSMRY;

typedef struct
{
	double  irwd_s;		// irrigation withdrawal (ft^3)
} HYDIR_STREAMSMRY;

typedef struct
{
	double  ustemp_s;
	double  lutemp_s;
	double  ustempbp_s;
	double  lutempbp_s;
	double  itemp_s;
} TEMPUP_STREAMSMRY;

typedef struct
{
	double  pstemp_s;
	double  pswdtemp_s;
	double  pswdtempdi_s;
} TEMPPS_STREAMSMRY;

typedef struct
{
	double  irwdtemp_s;
} TEMPIR_STREAMSMRY;

typedef struct
{
	double  bersed_s;
} SEDER_STREAMSMRY;

typedef struct
{
	double  ussed_s;
	double  lused_s;
	double  ussedbp_s;
	double  lusedbp_s;
	double  ised_s;
} SEDUP_STREAMSMRY;

typedef struct
{
	double  pssed_s;
	double  pswdsed_s;
	double  pswdseddi_s;
} SEDPS_STREAMSMRY;

typedef struct
{
	double  irwdsed_s;
} SEDIR_STREAMSMRY;

typedef struct
{
	double  depscr_ss;
} SEDDEPSCR_STREAMSMRY;

typedef struct
{
	double  berqual_s;
} GQUALER_STREAMSMRY;

typedef struct
{
	double  berqual_s;
	double  usqual_s;
	double  luqual_s;
	double  usqualbp_s;
	double  luqualbp_s;
	double  iqual_s;
} GQUALUP_STREAMSMRY;

typedef struct
{
	double  psqual_s;
	double  pswdqual_s;
	double  pswdqualdi_s;
} GQUALPS_STREAMSMRY;

typedef struct
{
	double  irwdqual_s;
} GQUALIR_STREAMSMRY;

typedef struct
{
	double  usrqual_s;
	double  lurqual_s;
	double  usrqualbp_s;
	double  lurqualbp_s;
	double  irqual_s;
} RQUALUP_STREAMSMRY;

typedef struct
{
	double  psrqual_s;
	double  pswdrqual_s;
	double  pswdrqualdi_s;
} RQUALPS_STREAMSMRY;

typedef struct
{
	double  irwdrqual_s;
} RQUALIR_STREAMSMRY;

class COUTPUT  
{
public:
	COUTPUT();
	virtual ~COUTPUT();

public:
	int		rqflag_s[11];
	CString rqname_s[11];
	bool	blistOutput[PARA_NUM];

	HYDRO_LANDSMRY		*pHYDRO_LANDSMRY;
	SED_LANDSMRY		*pSED_LANDSMRY;
	GQUAL_LANDSMRY		*pGQUAL_LANDSMRY;
	RQUAL_LANDSMRY		*pRQUAL_LANDSMRY;
	HYDUP_STREAMSMRY	*pHYDUP_STREAMSMRY;
	HYDPS_STREAMSMRY	*pHYDPS_STREAMSMRY;			
	HYDIR_STREAMSMRY	*pHYDIR_STREAMSMRY;
	TEMPUP_STREAMSMRY	*pTEMPUP_STREAMSMRY;
	TEMPPS_STREAMSMRY	*pTEMPPS_STREAMSMRY;
	TEMPIR_STREAMSMRY	*pTEMPIR_STREAMSMRY;
	SEDER_STREAMSMRY	*pSEDER_STREAMSMRY;
	SEDUP_STREAMSMRY	*pSEDUP_STREAMSMRY;
	SEDPS_STREAMSMRY	*pSEDPS_STREAMSMRY;
	SEDIR_STREAMSMRY	*pSEDIR_STREAMSMRY;
	SEDDEPSCR_STREAMSMRY *pSEDDEPSCR_STREAMSMRY;
	GQUALER_STREAMSMRY	*pGQUALER_STREAMSMRY;
	GQUALUP_STREAMSMRY	*pGQUALUP_STREAMSMRY;
	GQUALPS_STREAMSMRY	*pGQUALPS_STREAMSMRY;
	GQUALIR_STREAMSMRY	*pGQUALIR_STREAMSMRY;
	RQUALUP_STREAMSMRY	*pRQUALUP_STREAMSMRY;
	RQUALPS_STREAMSMRY	*pRQUALPS_STREAMSMRY;
	RQUALIR_STREAMSMRY	*pRQUALIR_STREAMSMRY;

public:
	void	ResetData();
};

#endif // !defined(AFX_OUTPUT_H__A3D833A8_BD4A_11D3_A495_002078143FF2__INCLUDED_)
