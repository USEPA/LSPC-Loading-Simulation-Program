// LSPCMODEL.h: interface for the CLSPCMODEL class.
//
//////////////////////////////////////////////////////////////////////
#include <afxdb.h>
#include "poly.h"
#include <afxtempl.h>

#include "Landuse.h"
#include "Output.h"
#include "Weather.h"

#include <list>
#include <queue>

using namespace std;

#if !defined(AFX_LSPCMODEL_H__A3D833A8_BD4A_11D3_A495_002078143FF2__INCLUDED_)
#define AFX_LSPCMODEL_H__A3D833A8_BD4A_11D3_A495_002078143FF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define LSPC_VERSION    "Version 5.0 - March 31, 2014" 

#define MAXLINE 10240
#define FLOODPLANEDEPTHRATIO 40	
#define SMALLNUM	1.0E-18
#define LIMITNUM	1.0E-6
#define UNKNOWN		-999

#define	POLUT_BOD	1001
#define	POLUT_CO2	1002 
#define	POLUT_DOX	1003
#define	POLUT_NO2	1004
#define	POLUT_NO3	1005
#define	POLUT_ORC	1006
#define	POLUT_ORN	1007
#define	POLUT_ORP	1008
#define	POLUT_PHY	1009
#define	POLUT_PO4	1010
#define	POLUT_TAM	1011
#define	POLUT_TIC	1012
#define	POLUT_ZOO	1013
#define	POLUT_TEMP	1014
//point sources
#define	PS_SAND		101
#define	PS_SILT		102
#define	PS_CLAY		103
#define	PS_TEMP		104
#define	PS_DOX		105
#define	PS_BOD		106
#define	PS_NOX		107
#define	PS_TAM		108
#define	PS_PO4		109
#define	PS_ORN		110
#define	PS_ORP		111
#define	PS_ORC		112
#define	PS_CLA		113
//point source summary
#define	Index_SAND	0
#define	Index_SILT	1
#define	Index_CLAY	2
#define	Index_TEMP	3
#define	Index_DOX	4
#define	Index_BOD	5
#define	Index_NOX	6
#define	Index_TAM	7
#define	Index_PO4	8
#define	Index_ORN	9
#define	Index_ORP	10
#define	Index_ORC	11
#define	Index_CLA	12
//unit conversion 
#define GRAM_TO_TONS	1.10231e-006
#define TONS_TO_GRAM	907184.7
#define LBS_TO_GRAM		453.5924
#define FT_TO_M			0.3048
#define CM3_TO_M3		1e-006
#define FT3_TO_M3		0.02831685
#define MILE_TO_METER	1609.344
#define MILE_TO_FEET	5280.0
#define IN_ACRE_TO_FT3	3630.0
#define LB_FT3_TO_MG_L	16018.46
#define ACRE_TO_FT2		43560.0

class swsinfo
{
public:
	int lksws;
	int subbasin;
	int gid;			// land parameters group id
	int gindex;			// land parameters group index (sorted serial number start with 0)
	int nwst;			// number of weather station assign to
	int *wstindex;		// array of weather station index	
	double *wti;		// array of weighting to calculate input
	FILE* m_fileOut;	// used for outputing simulation result 
	FILE* m_file_IFWO;	// used for outputing baseflow for SUSTAIN linkage 
	FILE* m_file_AGWO;	// used for outputing baseflow for SUSTAIN linkage 
	swsinfo()
	{
		lksws = -1;
		subbasin = -1;
		gid = -1;
		gindex = -1;
		nwst = 0;
		wstindex = NULL;
		wti = NULL;
		m_fileOut = NULL;
		m_file_IFWO = NULL;
		m_file_AGWO = NULL;
	}
	virtual ~swsinfo()
	{
		if (wstindex != NULL)
			delete[]wstindex;
		if (wti != NULL)
			delete[]wti;
	}
};

typedef struct
{
	int luidp;
	CString plusname;
	double luPREmult;	//landuse precipitation multiplier
	double luPETmult;	//landuse PET multiplier
} LandUsePInfo;

typedef struct
{
	int gid;			// land parameters group id
	int luid;
	double input_suro;
	double input_ifwo;
	double input_agwo;
	double route_suro;
	double route_ifwo;
	double route_agwo;
	double bypass_suro;
	double bypass_ifwo;
	double bypass_agwo;
}LURoute;

//Irrigation Application Options Info - Sabu Paul (9-2004)
typedef struct
{
    int gid;
    int luid;
	int startmonth;		//starting month of the growing season
	int endmonth;	
    double fraction1;   //fraction of irrigation requirement applied over the canopy
    double fraction2;   //fraction of irrigation water applied directly to the soil surface
    double fraction3;   //fraction of irrigation water applied to the upper soil zone via buried systems
    double fraction4;   //fraction of irrigation water likewise applied to the lower soil zone
    double fraction5;   //fraction of irrigation water entering directly into the local groundwater, such as seepage irrigation
    double etcoeff;
	int etdays;
}IrrigInfo;

//Irrigation Application Options constant PET 
typedef struct
{
    int gid;
    double petval;
}IrrigPET;

//Irrigation Application Options Monthly ET coefficients - Sabu Paul (9-2004)
typedef struct
{
    int gid;
    int luid;
	double monETCs[12] ;
}MonETCInfo;

//Irrigation Water Withdrawal Information - Sabu Paul (9-2004)
typedef struct
{
//	bool extsrc;		//irrigation demand is met by external source
	int wsid;			//Watershed ID
	int streamid;		//ID of the stream where water is withdrawn for irrigation (0 for external source)
	double irrigdep;	//depth of irrigation withdrawal pipe
}IrrigWDInfo;

class Ftable_Selected
{
public:
	int rchid;
	double depth;
	double area;
	double vol;
	double disch1;					 
	double *disch;					 
	Ftable_Selected()
	{
		rchid	= -1;
		depth	= 0.0;
		area	= 0.0;
		vol		= 0.0;
		disch1	= 0.0;				 
		disch	= NULL;
	}
	virtual ~Ftable_Selected()
	{
		if(disch != NULL)
			delete []disch;
	}
};

typedef struct
{
	int gid;
	int lid;
	double lzsn_p;
	double infilt_p;
	double kvary_p;
	double agwr_p;
} PW_PARA2;

typedef struct
{
	int gid;
	int lid;
	double petmax_p;     
	double petmin_p;     
	double infexp_p;     
	double infild_p;     
	double deepfr_p;     
	double basetp_p;     
	double agwetp_p;
} PW_PARA3;

typedef struct
{
	int gid; 
	int lid;   
	double cepsc_p;      
	double uzsn_p;    
	double nsur_p;       
	double intfw_p;       
	double irc_p;        
	double lzetp_p;
} PW_PARA4;

typedef struct
{
	int gid; 
	int lid; 
	double ceps_p;      
	double surs_p;       
	double uzs_p;      
	double ifws_p;       
	double lzs_p;       
	double agws_p;      
	double gwvs_p;
} PW_STATE;

typedef struct
{
	int gid; 
	int lid; 
	double fnum[12];
} MON_DATE;

typedef struct 
{
	int gid;
	int qualid;
	int qsdfg;
	int gqsdfg;
	int qsofg;	
	int potfcfg;
	CString qname;
	CString qunit;
} QUALS;

typedef struct 
{
	int gid,lid,qualid;
	double sqo,potfw,potfs,potfc,acqop,sqolim,wsqop,soqc,ioqc,aoqc,addc,awdc;
} QUAL_INPUT;

typedef struct 
{
	int gid;
	int qualid;
	int qsdfg;
	double inicon;
	double fstdec;
	double thfst;
	double addc;
	double awdc;
	double potber;
} QUALSWQ;

typedef struct 
{
	int gid,qualid;
	double addcpm[4];
} GQ_SEDDECAY;

typedef struct 
{
	int gid,qualid;
	double adpm[6];
} GQ_KD;

typedef struct 
{
	int gid,qualid;
	double adpm[6];
} GQ_ADRATE;

typedef struct 
{
	int gid,qualid;
	double adpm[6];
} GQ_ADTHETA;

typedef struct 
{
	int gid,qualid;
	double sqal[6];
} GQ_SEDCONC;

typedef struct
{
	int gid; 
	int lid; 
	int qualid;
	double fnum[12];
} IMON_DATE;

typedef struct
{
	int rgid; 
	int qualid;
	double fnum[12];
} MON_DATA;

typedef struct
{
	int trgid; 
	int tnum;
	int ttype; 
	int tdays;
	double fnum[12];
} MON_TDATA;

class THR_RESULTS
{
public:
	int trigcount;
	queue<double> q;

	int count;
	double min;
	double max;
	double sum;
	double sumofmean;
	list<double> listmonth;

	int simcount;
	double simmin;
	double simmax;
	double simsum;
	double simsumofmean;
	list<double> listsim;

	THR_RESULTS()
	{
		trigcount = 0;

		count = 0;
		min = 0.0;
		max = 0.0;
		sum = 0.0;
		sumofmean = 0.0;

		simcount = 0;
		simmin = 0.0;
		simmax = 0.0;
		simsum = 0.0;
		simsumofmean = 0.0;
	}
	virtual ~THR_RESULTS()
	{
		while (!q.empty())
			q.pop();

		listmonth.clear();
		listsim.clear();
	}
};

class THR_MAPPING
{
public:
	int tnum;
	int tqsd;
	int tqnum;
	int *tqid;					 
	THR_MAPPING()
	{
		tnum	= 0;
		tqsd	= 0;
		tqnum	= 0;
		tqid	= NULL;
	}
	virtual ~THR_MAPPING()
	{
		if(tqid != NULL)
			delete []tqid;
	}
};

class TimeSeries
{
public:
	COleDateTime tm;
	double fNum[7];

	TimeSeries()
	{
		for(int i=0; i<7; ++i)
			fNum[i] = 0.0;
	}
	virtual ~TimeSeries()
	{
	}
};
// for channel and watershed saving
typedef struct
{
	COleDateTime tm;
	double fNum[14]; 
} TempTimeSeries;

class ReachFtable
{
public:
	int rchid;
	int nFTstart;		// starting record number in Ftable
	int nFTrecord;		// total number of records in Ftable
	double *bypass_fr;	// bypass fraction to the multiple outlets 
	ReachFtable()
	{
		rchid	  = -1;
		nFTstart  = -1;
		nFTrecord = 0;				 
		bypass_fr = NULL;
	}
	virtual ~ReachFtable()
	{
		if(bypass_fr != NULL)
			delete []bypass_fr;
	}
};

class ReachInfo
{
public:
//	CString strRName;
	int rchid;
	int gid;
	int rgid;		// stream group id 	
	int tgid;
	int trgid;		// threshold stream group id 	
	int lkfg;		// lake flag (0 for reach and 1 for lake)
	int routefg;	// routing flag (0 for no routing and 1 for routing)
	double idepth;	// initial water depth
	double length;
	double depth;
	double width;
	double slope;
	double mann;
	double r1;
	double r2;
	double w1;
	double crrat;
	double ks;	
	CPolygon poly;

	void BuildDefaultPoly();
	void BuildPoly(CPolygon* newpoly);
	void ftable(double *fh, double *fv,double *fq, double *qk,double *fa, double *fp, double *fr, double *fsa);		//   (04-28-04)
	
	ReachInfo()
	{
		rchid     = -1;
		gid		  = 0;
		rgid      = 0;
		tgid      = 0;
		trgid     = 0;
		lkfg	  = 0;
		routefg   = 1;
		idepth	  = 0.0;
		length    = 0.0;
		depth     = 0.0;
		width     = 0.0;
		slope     = 0.0;
		mann      = 0.0;
		r1        = 0.0;
		r2        = 0.0;
		w1        = 0.0;
		crrat     = 1.5;
		ks        = 0.0;
	}

	virtual ~ReachInfo()
	{
	}
};

//for linkage file
class LINKAGEDATA
{
public:
	COleDateTime tTime;
	double flow;
	double temp;
	double dox;
	double bod;
	double no3;
	double tam;
	double no2;
	double po4;
	double snh4[3];
	double spo4[3];
	double phy;
	double zoo;
	double orn;
	double orp;
	double orc;
	double tic;
	double co2;
	double *pSed;
	double *pQual;
	double *pSedQual;

	LINKAGEDATA()
	{
		tTime = COleDateTime(1919,5,4,0,0,0);
		flow = 0.0;
		temp = 0.0;
		dox = 0.0;
		bod = 0.0;
		no3 = 0.0;
		tam = 0.0;
		no2 = 0.0;
		po4 = 0.0;
		for (int i=0; i<3; i++)
		{
			snh4[i] = 0.0;
			spo4[i] = 0.0;
		}
		phy = 0.0;
		zoo = 0.0;
		orn = 0.0;
		orp = 0.0;
		orc = 0.0;
		tic = 0.0;
		co2 = 0.0;
		pSed = NULL;
		pQual = NULL;
		pSedQual = NULL;
	}

	virtual ~LINKAGEDATA()
	{
		if(pSed != NULL)
			delete []pSed;
		if(pQual != NULL)
			delete []pQual;
		if(pSedQual != NULL)
			delete []pSedQual;
	}
};

class Link435
{
public:
	int nrchid;				// reach id
	int nOutlet;			// outlet id (0 for upstream and rchid for downstream)
	CString strFilePath;	// linkage file path
	FILE* m_linkfile;		// downstream outlet file
	LINKAGEDATA* pLinkData;	// upstream linkage data

	Link435()
	{
		nrchid = -1;
		nOutlet = -1;
		strFilePath = "";
		m_linkfile = NULL;
		pLinkData = NULL;
	}

	virtual ~Link435()
	{
		if(pLinkData != NULL)
			delete []pLinkData;
	}
};

class PS_SUMMARY
{
public:
	CString strPermit;
	CString strPipe;
	double  lfFlow;
	double  psQual[13];
	double *pGQual;

	PS_SUMMARY()
	{
		strPermit = "";
		strPipe = "";
		lfFlow = 0.;
		for (int i=0; i<13; i++)
			psQual[i] = 0.;
		pGQual = NULL;
	}

	virtual ~PS_SUMMARY()
	{
		if(pGQual != NULL)
			delete []pGQual;
	}
};

class ChannelInfo
{
public:
//	CString strRName;
	int rchid;
	int uright;
	int uleft;
	int nDSchan;					
	int *down_ch;
	//routing arrays
	double *down_flow;				
	double *down_matqal;			
	double *down_sedqal;			
	double *down_temp;			
	double *down_matsed;			
//	double *down_matco2;			
	double *down_dox;				
	double *down_bod;				
 	double *down_no3;				
 	double *down_tam;				
	double *down_no2;				
 	double *down_po4;				
 	double *down_spo4;				
 	double *down_snh4;				
	double *down_phy;				
 	double *down_zoo;				
 	double *down_orn;				
	double *down_orp;				
 	double *down_orc;				
	double *down_tic;				
 	double *down_co2;

	// no routing arrays			
	double *bypass_flow;				
	double *bypass_matqal;			
	double *bypass_sedqal;			
	double *bypass_temp;			
	double *bypass_matsed;			
	double *bypass_dox;				
	double *bypass_bod;				
 	double *bypass_no3;				
 	double *bypass_tam;				
	double *bypass_no2;				
 	double *bypass_po4;				
 	double *bypass_spo4;				
 	double *bypass_snh4;				
	double *bypass_phy;				
 	double *bypass_zoo;				
 	double *bypass_orn;				
	double *bypass_orp;				
 	double *bypass_orc;				
	double *bypass_tic;				
 	double *bypass_co2;				
	// stream summary
	double *down_flow_s;				
	double *down_matqal_s;			
	double *down_temp_s;			
	double *down_matsed_s;			
	double *down_dox_s;				
	double *down_bod_s;				
 	double *down_no3_s;				
 	double *down_tam_s;				
	double *down_no2_s;				
 	double *down_po4_s;				
 	double *down_orn_s;				
	double *down_orp_s;				
 	double *down_orc_s;				
	double *down_tic_s;				
 	double *down_co2_s;	
 	double *down_SNH4_s;				
 	double *down_SPO4_s;				
	// no routing
	double *bypass_flow_s;				
	double *bypass_matqal_s;			
	double *bypass_temp_s;			
	double *bypass_matsed_s;			
	double *bypass_dox_s;				
	double *bypass_bod_s;				
 	double *bypass_no3_s;				
 	double *bypass_tam_s;				
	double *bypass_no2_s;				
 	double *bypass_po4_s;				
 	double *bypass_orn_s;				
	double *bypass_orp_s;				
 	double *bypass_orc_s;				
	double *bypass_tic_s;				
 	double *bypass_co2_s;	
 	double *bypass_SNH4_s;				
 	double *bypass_SPO4_s;				

	int down;
	int comp;
	// added for calculation
	int nSelfOrder;
	bool bSave; // whether to save this channel or not for network connection

	//added for new routing, which allows more than two upper streams
	CArray<int, int> cListUppersOriginal;
	CArray<int, int> cListUppersOrder;

	//added for timeseries result linkage files
	CList<Link435, Link435&> cListUpStream;
	CList<Link435, Link435&> cListDownStream;

	//added for point source summary file
	CList<PS_SUMMARY, PS_SUMMARY&> cListPointSources;

	ChannelInfo()
	{
		rchid      = -1;
		uright     = -1;
		uleft      = -1;
		nDSchan    = 1;
		down       = -1;
		comp       = -1;
		nSelfOrder = -1;
		bSave      = false; // whether to save this channel or not for network connection

		down_ch			= NULL;					
		down_flow		= NULL;					
		down_matqal		= NULL;					
		down_sedqal		= NULL;					
		down_temp	    = NULL;					
		down_matsed		= NULL;					
		down_dox		= NULL;					
		down_bod		= NULL;					
		down_no3		= NULL;					
		down_tam		= NULL;					
		down_no2		= NULL;					
		down_po4		= NULL;					
		down_spo4		= NULL;					
		down_snh4		= NULL;					
		down_phy		= NULL;					
		down_zoo		= NULL;					
		down_orn		= NULL;					
		down_orp		= NULL;					
		down_orc		= NULL;					
		down_tic		= NULL;					
		down_co2		= NULL;

		//no routing arrays
		bypass_flow		= NULL;					
		bypass_matqal	= NULL;					
		bypass_sedqal	= NULL;					
		bypass_temp	    = NULL;					
		bypass_matsed	= NULL;					
		bypass_dox		= NULL;					
		bypass_bod		= NULL;					
		bypass_no3		= NULL;					
		bypass_tam		= NULL;					
		bypass_no2		= NULL;					
		bypass_po4		= NULL;					
		bypass_spo4		= NULL;					
		bypass_snh4		= NULL;					
		bypass_phy		= NULL;					
		bypass_zoo		= NULL;					
		bypass_orn		= NULL;					
		bypass_orp		= NULL;					
		bypass_orc		= NULL;					
		bypass_tic		= NULL;					
		bypass_co2		= NULL;
		// stream summary
		down_flow_s		= NULL;					
		down_matqal_s	= NULL;					
		down_temp_s  	= NULL;					
		down_matsed_s	= NULL;					
		down_dox_s		= NULL;					
		down_bod_s		= NULL;					
		down_no3_s		= NULL;					
		down_tam_s		= NULL;					
		down_no2_s		= NULL;					
		down_po4_s		= NULL;					
		down_orn_s		= NULL;					
		down_orp_s		= NULL;					
		down_orc_s		= NULL;					
		down_tic_s		= NULL;					
		down_co2_s		= NULL;	
		down_SNH4_s		= NULL;					
		down_SPO4_s		= NULL;					
		// no routing
		bypass_flow_s	= NULL;					
		bypass_matqal_s	= NULL;					
		bypass_temp_s  	= NULL;					
		bypass_matsed_s	= NULL;					
		bypass_dox_s	= NULL;					
		bypass_bod_s	= NULL;					
		bypass_no3_s	= NULL;					
		bypass_tam_s	= NULL;					
		bypass_no2_s	= NULL;					
		bypass_po4_s	= NULL;					
		bypass_orn_s	= NULL;					
		bypass_orp_s	= NULL;					
		bypass_orc_s	= NULL;					
		bypass_tic_s	= NULL;					
		bypass_co2_s	= NULL;	
		bypass_SNH4_s	= NULL;					
		bypass_SPO4_s	= NULL;					
	}

	virtual ~ChannelInfo()
	{
		cListUppersOriginal.RemoveAll();
		cListUppersOrder.RemoveAll();

		//added for card 435
		cListUpStream.RemoveAll();
		cListDownStream.RemoveAll();

		//added for point source summary (c660)
		cListPointSources.RemoveAll();

		if(down_ch != NULL)					
			delete []down_ch;
		if(down_flow != NULL)				
			delete []down_flow;
		if(down_matqal != NULL)				
			delete []down_matqal;
		if(down_sedqal != NULL)				
			delete []down_sedqal;
		if(down_temp != NULL)			
			delete []down_temp;
		if(down_matsed != NULL)				
			delete []down_matsed;
//		if(down_matco2 != NULL)				
//			delete []down_matco2;
		if(down_dox != NULL)				
			delete []down_dox;
		if(down_bod != NULL)				
			delete []down_bod;
		if(down_no3 != NULL)				
			delete []down_no3;
		if(down_tam != NULL)				
			delete []down_tam;
		if(down_no2 != NULL)				
			delete []down_no2;
		if(down_po4 != NULL)				
			delete []down_po4;
		if(down_spo4 != NULL)				
			delete []down_spo4;
		if(down_snh4 != NULL)				
			delete []down_snh4;
		if(down_phy != NULL)				
			delete []down_phy;
		if(down_zoo != NULL)				
			delete []down_zoo;
		if(down_orn != NULL)				
			delete []down_orn;
		if(down_orp != NULL)				
			delete []down_orp;
		if(down_orc != NULL)				
			delete []down_orc;
		if(down_tic != NULL)				
			delete []down_tic;
		if(down_co2 != NULL)				
			delete []down_co2;

		// no routing arrays
		if(bypass_flow != NULL)				
			delete []bypass_flow;
		if(bypass_matqal != NULL)				
			delete []bypass_matqal;
		if(bypass_sedqal != NULL)				
			delete []bypass_sedqal;
		if(bypass_temp != NULL)			
			delete []bypass_temp;
		if(bypass_matsed != NULL)				
			delete []bypass_matsed;
		if(bypass_dox != NULL)				
			delete []bypass_dox;
		if(bypass_bod != NULL)				
			delete []bypass_bod;
		if(bypass_no3 != NULL)				
			delete []bypass_no3;
		if(bypass_tam != NULL)				
			delete []bypass_tam;
		if(bypass_no2 != NULL)				
			delete []bypass_no2;
		if(bypass_po4 != NULL)				
			delete []bypass_po4;
		if(bypass_spo4 != NULL)				
			delete []bypass_spo4;
		if(bypass_snh4 != NULL)				
			delete []bypass_snh4;
		if(bypass_phy != NULL)				
			delete []bypass_phy;
		if(bypass_zoo != NULL)				
			delete []bypass_zoo;
		if(bypass_orn != NULL)				
			delete []bypass_orn;
		if(bypass_orp != NULL)				
			delete []bypass_orp;
		if(bypass_orc != NULL)				
			delete []bypass_orc;
		if(bypass_tic != NULL)				
			delete []bypass_tic;
		if(bypass_co2 != NULL)				
			delete []bypass_co2;
		// stream summary
		if(down_flow_s != NULL)				
			delete []down_flow_s;
		if(down_matqal_s != NULL)				
			delete []down_matqal_s;
		if(down_temp_s != NULL)			
			delete []down_temp_s;
		if(down_matsed_s != NULL)				
			delete []down_matsed_s;
		if(down_dox_s != NULL)				
			delete []down_dox_s;
		if(down_bod_s != NULL)				
			delete []down_bod_s;
		if(down_no3_s != NULL)				
			delete []down_no3_s;
		if(down_tam_s != NULL)				
			delete []down_tam_s;
		if(down_no2_s != NULL)				
			delete []down_no2_s;
		if(down_po4_s != NULL)				
			delete []down_po4_s;
		if(down_orn_s != NULL)				
			delete []down_orn_s;
		if(down_orp_s != NULL)				
			delete []down_orp_s;
		if(down_orc_s != NULL)				
			delete []down_orc_s;
		if(down_tic_s != NULL)				
			delete []down_tic_s;
		if(down_co2_s != NULL)				
			delete []down_co2_s;
		if(down_SNH4_s != NULL)				
			delete []down_SNH4_s;
		if(down_SPO4_s != NULL)				
			delete []down_SPO4_s;

		if(bypass_flow_s != NULL)				
			delete []bypass_flow_s;
		if(bypass_matqal_s != NULL)				
			delete []bypass_matqal_s;
		if(bypass_temp_s != NULL)			
			delete []bypass_temp_s;
		if(bypass_matsed_s != NULL)				
			delete []bypass_matsed_s;
		if(bypass_dox_s != NULL)				
			delete []bypass_dox_s;
		if(bypass_bod_s != NULL)				
			delete []bypass_bod_s;
		if(bypass_no3_s != NULL)				
			delete []bypass_no3_s;
		if(bypass_tam_s != NULL)				
			delete []bypass_tam_s;
		if(bypass_no2_s != NULL)				
			delete []bypass_no2_s;
		if(bypass_po4_s != NULL)				
			delete []bypass_po4_s;
		if(bypass_orn_s != NULL)				
			delete []bypass_orn_s;
		if(bypass_orp_s != NULL)				
			delete []bypass_orp_s;
		if(bypass_orc_s != NULL)				
			delete []bypass_orc_s;
		if(bypass_tic_s != NULL)				
			delete []bypass_tic_s;
		if(bypass_co2_s != NULL)				
			delete []bypass_co2_s;
		if(bypass_SNH4_s != NULL)				
			delete []bypass_SNH4_s;
		if(bypass_SPO4_s != NULL)				
			delete []bypass_SPO4_s;
	}
};

typedef struct
{
	int index;
	int UR;
	int UL;
} Channel;

typedef struct
{
	int qualindex;
	int qualid;
	CString qualname;
	double sqalfr;
} PtQual;

typedef struct
{
	int wdrawlrchid;
	CString strPermit;
	CString strPipe;
	int targetrchid;
} PtWithDrawal;

class PSREALDATA
{
public:
	COleDateTime tTime;
	double flow;
	double withdrawal;
	double THDN;  	// total hydrogen dissolved Neg-H (mole/m3)
	double THDP;	// total hydrogen dissolved Pos-H (mole/m3)
	double THADSN;	// total hydrogen adsorbed Neg-H (mole/m3)
	double THADSP;	// total hydrogen adsorbed Pos-H (mole/m3)
	double *pPollutant;

	PSREALDATA()
	{
		flow = 0.0;
		withdrawal = 0.0;
		THDN = 0.0;
		THDP = 0.0;
		THADSN = 0.0;
		THADSP = 0.0;
		tTime = COleDateTime(1919,5,4,0,0,0);
		pPollutant = NULL;
	}

	virtual ~PSREALDATA()
	{
		if(pPollutant != NULL)
			delete []pPollutant;
	}
};

//point source data
class PSData
{
public:
//	int nPID;
	int rchid;
	CString strPermit;
	CString strPipe;
	int nRecord;
	PSREALDATA *pData;

public:
	PSData()
	{
		pData   = NULL;
		nRecord = 0;
//		nPID    = -1;
		rchid = -1;
		strPermit = "";
		strPipe = "";
	}

	virtual ~PSData()
	{
		if(pData != NULL)
			delete []pData;
	}
};

//sediment
typedef struct
{
	int defid;
	int deluid;
	CString luname;
	int start_month;
	int end_month;
	double frequency;
	double percent_area;
	double effic_sand;
	double effic_silt;
	double effic_clay;
} SD_SWEEP;

class GQ_SWEEP
{
public:
	int defid;
	int deluid;
	CString luname;
	double *effic_GQual;

	GQ_SWEEP()
	{
		defid = 0;
		deluid = 0;
		luname = "";
		effic_GQual = NULL;
	}

	virtual ~GQ_SWEEP()
	{
		if(effic_GQual != NULL)
			delete []effic_GQual;
	}
};

typedef struct
{
	int gid;
	int lid;
	double smpf_p;
	double krer_p;
	double jrer_p;
	double affix_p;
	double cover_p;
	double nvsi_p;
	double kser_p;
	double jser_p;
	double kger_p;
	double jger_p;
	double accsdp_p;
	double remsdp_p;
} SD_PARA1;

class SD_PARA2
{
public:
	int gid;
	int lid;
	double sed_suro;
	double sed_ifwo;
	double sed_agwo;
	double *sed;

	SD_PARA2()
	{
		gid			= 0;
		lid			= 0;
		sed_suro	= 0.0;
		sed_ifwo	= 0.0;
		sed_agwo	= 0.0;
		sed 		= NULL;
	}

	virtual ~SD_PARA2()
	{
		if(sed != NULL)
			delete []sed;
	}
};

class SD_BER
{
public:
	int     rchid;
	double  kber;    
	double  jber;    
	double  qber; 
	double  *sed;

	SD_BER()
	{
		rchid = -1;
		kber  = 0.0;
		jber  = 0.0;
		qber  = 0.0;
		sed   = NULL;
	}

	virtual ~SD_BER()
	{
		if(sed != NULL)
			delete []sed;
	}
};

// temperature parameters for land group
typedef struct
{
	int gid; 
	int lid; 
	double fnum[12];
} MONTH_TMP;	

typedef struct
{
//	CString strRName;
	int subbasin;
	double elev;                       
	double eldat;
	double relev;                       
	double reldat;
} TMP_PARA1;

// temperature parameters for stream group
typedef struct
{
	int gid;			// stream parameter group id (07-12-05)
	double cfsaex_p;
	double katrad_p;
	double kcond_p;
	double kevap_p;
} TMPS_PARA1;

// bed conduction parameters for stream group
typedef struct
{
	int rgid;
	int preflg;
	int bedflg;
	int tgflg;
	double muddep;
	double tgrnd;
	double kmud;
	double kgrnd;
} TMPS_PARA2;

typedef struct
{
	int rgid; 
	double fnum[12];
} MONTH_PARM;

//soil temp
typedef struct
{
	int gid;
	int lid;
	int tsopfg;
	double aslt;
	double bslt;
	double aift;
	double bift;
	double agwt;
	double bgwt;
	double islt;
	double iift;
	double igwt;
} STMP_PARA1;                          

// DO CO2
typedef struct
{
	int gid;
	int lid;
	double sdoxp;              
	double sco2p;
	double idoxp;              
	double ico2p;
	double adoxp;
	double aco2p;
//	double ialk_p;
//	double galk_p;
} DOCO_PARA1; 

typedef struct
{
	int gid; 
	int lid; 
	double fnum[12];
} MONTH_OX;

typedef struct
{
	int gid; 
	double fnum[12];
} MONTH_PH;

//RQUAL (03-2005)
typedef struct
{                     
	int    gid;
	double kbod20;              
	double tcbod;
	double kodset;
	double supsat;
	double tcginv;
	double reak;
	double expred;
	double exprev;
	double cforea;
} OX_PARA1; 

typedef struct
{                            
	int    gid;
	double benod;              
	double tcben;
	double expod;
	double brbod;
	double brbod_inc;
	double exprel;
} OX_PARA2; 

typedef struct
{           
	int    gid;
	double dox;
	double bod;
	double satdo;
} OXRX_INIT; 

typedef struct
{           
	int    gid;
	double scrvel;
	double scrmul;
} SCOUR_PARMS; 

class SEDGQUAL_MAPP
{
public:
	int    gid;
	int    lutype;		// 1 for impervious and 2 for pervious
	int    qualid;
	double *sed;
	SEDGQUAL_MAPP()
	{
		gid = -1;
		lutype = 0;
		qualid = 0;
		sed	 = NULL;
	}

	virtual ~SEDGQUAL_MAPP()
	{
		if(sed != NULL)
			delete []sed;
	}
}; 

typedef struct
{
	int    gid;
	int    qualid;
	int    lutype;	
	double bod;             
	double nox;
	double tam;              
	double snh4;              
	double po4;
	double spo4;
	double orn;
	double orp;
	double orc;
} LAND_MAPP; 

typedef struct
{
	int    gid;
	int    qualid;
	double bod;             
	double nox;
	double tam;              
	double snh4;              
	double po4;
	double spo4;
	double orn;
	double orp;
	double orc;
} ATMS_MAPP; 

typedef struct
{                     
	int    gid;
	double cvbo;              
	double cvbpc;
	double cvbpn;
	double bpcntc;
	double ktam20;
	double kno220;
	double tcnit;
	double kno320;
	double tcden;
	double denoxt;
} NUT_PARA1; 

typedef struct
{                            
	int    gid;
	double brtam_1;              
	double brtam_2;              
	double brpo4_1;
	double brpo4_2;
	double bnh4_1;              
	double bnh4_2;              
	double bnh4_3;              
	double bpo4_1;
	double bpo4_2;
	double bpo4_3;
} NUT_PARA2; 

typedef struct
{                            
	int    gid;
	double anaer;
	double adnhpm_1;
	double adnhpm_2;
	double adnhpm_3;
	double adpopm_1;
	double adpopm_2;
	double adpopm_3;
	double expnvg;
	double expnvl;
} NUT_PARA3; 

typedef struct
{           
	int    gid;
	double no3;
	double tam;
	double no2;
	double po4;
	double snh4_1;
	double snh4_2;
	double snh4_3;
	double spo4_1;
	double spo4_2;
	double spo4_3;
} NUT_INIT; 

typedef struct
{                     
	int    gid;
	double ratclp;              
	double nonref;
	double litsed;
	double alnpr;
	double extb;
	double malgr;
} PLANK_PARA1; 

typedef struct
{                            
	int    gid;
	double cmmlt;              
	double cmmn;              
	double cmmnp;
	double cmmp;
	double talgrh;
	double talgrl;
	double talgrm;
} PLANK_PARA2; 

typedef struct
{                            
	int    gid;
	double alr20;              
	double aldh;              
	double aldl;              
	double oxald;
	double naldh;
	double paldh;
} PLANK_PARA3; 

typedef struct
{                            
	int    gid;
	double phycon;
	double seed;              
	double mxstay;              
	double oref;              
	double claldh;
	double physet;
	double refset;
	double cfsaex;
	double mbal;
	double cfbalr;
	double cfbalg;
} PLANK_PARA4; 

typedef struct
{           
	int    gid;
	double phyto;
	double zoo;
	double benal;
	double orn;
	double orp;
	double orc;
} PLANK_INIT; 

// pH carbon
typedef struct
{           
	int		gid;
	int		phcnt;
	int		alkcon;
	double	cfcinv;
	double	brco2_1;
	double	brco2_2;
} PH_PARA; 
typedef struct
{           
	int    gid;
	double tic;
	double co2;
	double ph;
} PH_INIT; 

typedef struct
{
   int    gid;
   double bedwid;
   double beddep;
   double por;
   double burial;
} STREAMSED_GENPARM;

typedef struct
{
   int    gid;
   int    sed_id;
   int    sedflg;
   double sedo;
   double sedfrac;
   double d;
   double w;
   double rho;
   double taucd;
   double taucs;
   double m;
   double burial;
} STREAMSED_PARM;

typedef struct
{
	int luid;
//	CString gswsname;
	int subbasin;
	CString strLUName;
	double fRatio;
} TMDL_LU_QUAL_CPARAM;


typedef struct
{
	int nSourceGID;
	int nDestGID;
} DBStruct;

class PSReduction
{
public:
	int rchid;
	CString strPermitName;
	CString strPipeName;
	int nPSindex;				// point source index
	double fReduction_flow;
	double *fReduction_qual;

	PSReduction()
	{
		nPSindex = 0;
		rchid = -1;
		strPermitName = "";
		strPipeName = "";
		fReduction_flow = 0.0;
		fReduction_qual = NULL;
	}

	virtual ~PSReduction()
	{
		if(fReduction_qual != NULL)
		{
			delete []fReduction_qual;
			fReduction_qual = NULL;
		}
	}
};

typedef struct
{
	int rchid;
	int nOutletId;				
	int nSwitch[12];
} MON_SWITCH;

class BMPReduction
{
public:
	int rchid;
	int nOutletId;				
	double fReduction_flow;
	double *fReduction_qual;

	BMPReduction()
	{
		rchid = -1;
		nOutletId = 0;
		fReduction_flow = 0.0;
		fReduction_qual = NULL;
	}

	virtual ~BMPReduction()
	{
		if(fReduction_qual != NULL)
			delete []fReduction_qual;
	}
};

class BMPLimit
{
public:
	int rchid;
	int nOutletId;				
	double fLimit_flow;
	double *fLimit_qual;

	BMPLimit()
	{
		rchid = -1;
		nOutletId = 0;
		fLimit_flow = -1.0;
		fLimit_qual = NULL;
	}

	virtual ~BMPLimit()
	{
		if(fLimit_qual != NULL)
			delete []fLimit_qual;
	}
};

class CSnowControl2
{
public:
	int nGrpid;
	int luid;
	int ICEFLAG;
	double lfForest;
	double fzg;
	double fzgl;

	CSnowControl2()
	{
		nGrpid = 0;
		luid = 0;
		ICEFLAG = 0;
		lfForest = 0;
		fzg = 1.0;
		fzgl = 0.1;
	}

	virtual ~CSnowControl2()
	{
	}
};

class CSnowParam1
{
public:
	int nGrpid;
	int luid;
	double LAT;
	double MELEV;
	double SHADE;
	double SNOWCF;
	double COVIND;
	CSnowParam1()
	{
		nGrpid = 0;
		luid = 0;
		LAT = 39.0;
		MELEV = 110;
		SHADE = 0.3;
		SNOWCF = 1.45;
		COVIND = 0.5;
	}
	
	virtual ~CSnowParam1()
	{
	}
};

class CSnowParam2
{
public:
	int nGrpid;
	int luid;
	double RDCSN;
	double TSNOW;
	double SNOEVP;
	double CCFACT;
	double MWATER;
	double MGMELT;

	CSnowParam2()
	{
		nGrpid = 0;
		luid = 0;
		RDCSN = 0.12;
		TSNOW = 32;
		SNOEVP = 0.1;
		CCFACT = 1;
		MWATER = 0.08;
		MGMELT = 0.0001;
	}

	virtual ~CSnowParam2()
	{
	}
};

// snow pack related 
class CSnowInit1
{
public:
	int nGrpid;
	int luid;
	double packsnow;
	double packice;
	double packwatr;
	double RDENPF;
	double DULL;
    double PAKTMP;
	double COVINX;
    double XLNMLT;
    double SKYCLR;

	CSnowInit1()
	{
		nGrpid = 0;
		luid = 0;
		packsnow = 0.0;;
		packice = 0.0;
		packwatr = 0.0;
		RDENPF = 0.2;
		DULL = 400;
		PAKTMP = 27.5;
		COVINX = 0.01;
		XLNMLT = 0;
		SKYCLR = 0.5;
	}

	virtual ~CSnowInit1()
	{
	}
};

class CLSPCMODEL  
{
public:
	CLSPCMODEL();
	virtual ~CLSPCMODEL();
	void pwater();
	void ResetData();
	void Log(LPCTSTR lpszFormat, ...);
//	void AllocateTSSpace();
//	int  ReadTimeSeries(CString strFileName, int nStation);
	void SkipCommentLine(FILE *fp);
    bool ReadFileSection(FILE *fpin, int nSection);
	bool ReadInfo(CString strFileName);
	bool ReadInfo2(CString strFileName, int nCard, FILE *fpin);
	void WriteCardComments(FILE *fpin, int nSection);
	int  GetPSIndex(int subbasin,CString strPermit,CString strPipe);
	void UnitConvertion();
	void ReverseUnitConvertion();
	void UpdateDBParm();
//	void AddNewDefaultGroup();
	int  GetDefLUID(CDatabase& db, int nGID, int nLUID);
	bool SaveParm2DB(CDatabase& db);
	void InitialControls();
//	bool WriteLine(CString strFileName, CString strContent, int j);
	bool WriteOutputLine(CString strContent, FILE *fp);
//	bool WriteSummaryLine(CString strFileName, CString strContent, FILE *fp);
//	bool WriteThresholdSummaryLine(CString strFileName, CString strContent);
//	bool WriteThresholdStatLine(CString strFileName, CString strContent);
	bool WriteOutputHeader(CString strFileName, int j);
	bool WriteLandOutputHeader(CString strFileName, int j, int k);
	bool WriteBaseFlowOutputHeader(CString strFileName, int j, int k);
	bool WriteLanduseSummaryHeader(CString strFileName,COleDateTime toStart,COleDateTime toEnd);
	bool WriteStreamSummaryHeader(CString strFileName,COleDateTime toStart,COleDateTime toEnd);
	bool WritePointSourceSummaryHeader(CString strFileName,COleDateTime toStart,COleDateTime toEnd);
	bool WriteThresholdAnalysisSummaryHeader(CString strFileName,COleDateTime toStart,COleDateTime toEnd);
	bool WriteThresholdStatSummaryHeader(CString strFileName,COleDateTime toStart,COleDateTime toEnd);
	bool NetworkProcess();
//	bool ReadTempRecord(CString strFileName, int nOrder, TempTimeSeries& wsdr);
//	bool AddTempRecord(CString strFileName, TempTimeSeries wsdr);
//	int  TempRecordNum(CString strFileName);
	bool ReadLinkageFile(CString strFileName, Link435& plink435);
	bool WriteLinkageFileHeader(CString strFileName, Link435& plink435);
//	bool WriteLinkageFileLine(CString strFileName, CString strContent, Link435& plink435);

	bool GetDBInfo();
	int  FindPSIndex(int i,COleDateTime tCurrent);
//	int  FindWeatherShift(int ii, COleDateTime tCurrent);
	double GetLoad(int sqalfg,int nIndex,int nQualid,COleDateTime tCurrent);
	double GetLoad2(int sqalfg,int nIndex,int nQualid,COleDateTime tCurrent);
	double GetTargetWithDrawal(int nPSIndex,COleDateTime tCurrent);
	double GetFlow(int nIndex,COleDateTime tCurrent);
	void GetTotalH(int nIndex,COleDateTime tCurrent,double& THDN,double& THDP,double& THADSN,double& THADSP);
	double GetWithdrawal(int nIndex,COleDateTime tCurrent);
//	int GetWeatherData(int nIndex, int tempfg, COleDateTime tCurrent, double *precip, double *pet, double *airtemp, double *windvel, double *solarrad, double *dewtemp, double *cloudcover);
	void ReadPointSource(CString strFileName);
	void WritePointSource(CString strFileName);
	void ReadDBPointSource();
	void WriteInputFile(CString strFileName);
	void WriteFileSection(FILE *fpin, int nSection);
	int  FindReach(int nOrder);
	int  FindReachIndex(int nRchID);
	int  FindSelfOrder(int nReach);
	int  FindIndexOrder(int nReach);
	bool AssignReachOrder();
//	void MapReach2SWS();
	bool ExistReach(int nReach);
	int  qualsd(double *wssd, double *scrsd, double *potfw, double *potfs, double *washqs, double *scrqs, double *soqs);
	int  BUILDUP(int DAYFG,int VQOFG,int ADCNFG,int ADFXFG,int QSOFG,
				 int DAY,int NDAYS,int MON,int NXTMON,double DELT60,
				 double PREC,double SLIQO,double ADFLX,double ADCNC,
				 double *ADFXMN,double *ADCNMN,double *ADFXFX,double *ADCNFX,
				 double *ACQOP,double *REMQOP,double *ACQOPM,double *REMQOM,
				 double *ADTOT,double *SQO);
	int  WASHOFF(double SURO,double WSFAC,double *SQO,double *SOQO,double *SOQOC);
	int  WASHOF(int DAYFG,int VQOFG, double *ACQOPM,double *REMQOM,
					double SURO,double WSFAC,int MON,int NXTMON,int DAY,
					int NDAYS,double PREC,int ADFXFG,int ADCNFG,double *ADFXMN,
					double *ADCNMN,double ADFLX,double ADCNC,double SLIQO,
					double DELT60,int QSOFG,double *ACQOP,double *REMQOP,
					double *SQO,double *SOQO,double *SOQOC,double *ADFXFX,
					double *ADCNFX,double *ADTOT);
	int  uzone(int *nblks, double *uzsn, double *uzi,double *infilt, double *inffac, double *lzrat, double *uzs, double *perc);
/*	
	// mstlay section
	int SUBLAY(double LZS,double IGWI,double AGWI,double CFINMA,double LZSN,
			   double LLPF,double AGWS,double AGWO,double *MST,double *FRAC);
	int TOPLAY(double SURSS,double SURI,double CFINMA,double SURO,double INFIL,
			   double IFWI,double UZI,double SLMPF,double UZS,double ULPF,double PERC,
			   double UZSN,double IFWS,double IFWO,double *MST,double *FRAC);
	int TOPMOV(double *FRAC, double& SSCM, double& USCM, double& ISCM, double *TSCM);
	int SUBMOV(double UPCM,double FLP,double FLDP,double FAO,double& LSCM,double& ASCM,double *SSCM);
	int SEDMOV(double FSD,double& SSCM,double& SDCM);
	int SDFRAC(double SOSED,double SLME,double& FSD);
	int FIRORD(double TMP,double MOISTM,double KDS,double KAD,double THKDS,
			   double THKAD,double CMSU,double CMAD,double& ADS,double& DES);
*/
	double dayval(double *mval1, double *mval2, int *day, int *ndays);
	int  uzinf2(double *pdro, double *uzsn, double *uzs, double *uzi);
	int  etuzon(int *nblks, double *uzsn, double *rempet, double *uzs, double *taet, double *uzet);
	int  surfac(double *lzrat, double *infilt, double *infexp, double *inffac, double *infild, int *fsmsfg, int *dayfg, int *vnnfg, double *nsurm, double *lsur, double *slsur, int *vifwfg, double *intfwm, int *nblks, double *msupy, double *uzsn, double *uzs, double *delt60,int *rtopfg, int *uzfg, int *mon, int *nxtmon, int *day, int *ndays,  double *dec, double *src, double *nsur,double *intfw, double *surs, double *infil, double *uzi, double *ifwi, double *suro);
	int  dispos(double *imin, double *imax, double *ratio, double *msupy, double *uzsn, double *uzs, double *delt60, double *dec, double *src,  int *rtopfg, int *uzfg, double *surs, double *infil, double *uzi, double *ifwi, double *suro);
	int  divisn(double *min, double *max, double *msupy, double *over, double *under);
	int  etagw(double *agwetp, double *kvary, double *rempet, double *agws, double *taet, double *gwvs, double *agwet);
	int  qualgw(int *dayfg, int *vaqcfg, double *agwo, double *aoqcm, int *mon, int *nxtmon, int *day, int *ndays, double *aoqc, double *aoqual);
	int  uzones(double *uzsn, double *uzi, double *infilt, double *inffac, double *lzrat, double *uzs, double *perc);
	int  etbase(double *basetp, double *agwo, double *rempet, double *taet, double *baset);
	int  etlzon(int *dayfg, int *vlefg, int *mon, int *day, double *lzetpm, int *nxtmon, int *ndays, double *lzsn, double *delt60, double *lzetp, double *rempet, double *rparm, double *lzs, double *taet, double *lzet);
	int  etuzs(double *uzsn, double *rempet, double *uzs, double *uzet);
	int  evapt(double *pet, double *basetp, int *nblks, double *uzsn, double *agwetp, double *kvary, int *dayfg, int *vlefg, double *lzetpm, int *mon, int *nxtmon, int *day, int *ndays, double *lzsn, double *delt60,  double *agwo, double *ceps, double *uzs,  double *agws, double *gwvs, double *lzetp, double *rparm, double *lzs, double *rempet, double *taet, double *baset, double *cepe, double *uzet, double *agwet, double *lzet);
	int  evretn(double *pet, double *rets, double *impev);
	int  gwater(double *deepfr, double *gwi, double *kvary, int *dayfg, double *kgw, double *agwli, double *agws, double *gwvs, double *igwi, double *agwi, double *agwo);
	int  icept(int *vcsfg, int *dayfg, double *cepscm, int *mon, int *nxtmon, int *day, int *ndays, double *supy, double *cepsc, double *ceps, double *cepo);
	int  intflw(int *dayfg, int *vircfg, double *ircm, int *mon, int *nxtmon, int *day, int *ndays, double *delt60, int *nblks, double *ifwi,double *ifwli, double *irc, double *ifwk1, double *ifwk2, double *ifws, double *uzs, double *ifwo);
	int  lzone(double *iperc, double *lzrat, double *lzfrac, double *lzs, double *rlzrat, double *lzi);
	int  evicep(double *ceps, double *rempet, double *taet, double *cepe);
	int  proute(double *psur, int *rtopfg, double *delt60, double *dec, double *src,double *surs, double *suro);
	int  qualsuro(int *dayfg, int *vsqcfg, double *sowo, double *soqcm, int *mon, int *nxtmon, int *day, int *ndays, double *soqc, double *soqual);	// (03-2005)
	int  qualif(int *dayfg, int *viqcfg, double *ifwo, double *ioqcm, int *mon, int *nxtmon, int *day,int *ndays, double *ioqc, double *ioqual);
	// newly added 
	int  DISCH(int nIndex,double CONVF,int *nFTindex,double V,double *RO);
	int  ROUTE(int nIndex,int DAYFG,int VCONFG,double *CONVFM,double DELTS,double KS,
		       double COKS,double ROS,double VOLT,double FACTA1,int MON,int NXTMON, 
			   int DAY,int NDAYS,double& CONVF,int *nFTindex,double *VOL,double *RO);
	int  SOLVE(int nIndex,double CONVF,double FACTA1,double OINT,int *nFTindex,
			   double *VOL,double *RO);
	int  DEMAND(int nIndex,int nFTindex,double CONVF,double *OD,double *ROD);
	int  AUXIL(int nFTindex,double VOL,double LEN,double *DEP,double *SAREA,double *XAREA,
			   double *HRAD);
	int route(int nf,int nIndex,double vin,int dayfg,int vconfg,double *convfm,int mon,
		int nxtmon,int day,int ndays,double& convf,double delts,int *st,double *fq,
		double *fh,double *fv,double *fa,double *fr,double *fsa,double *qh,double *XA,
		double *HR,double *SA,double *v2,double *q2);

	int routine(double volt,int dayfg,int vconfg,double *convfm,int mon,int nxtmon,
		int day,int ndays,double& convf,double delts,double *fq,double *fh,double *fv,
		double *fa,double *fr,double *fsa,double *XA,double *HR,double *SA,double *q1,
		double *q2,double *v1,double *v2,double *qh,double *qk,int *st,int *nf, 
		int *nIndex,int routemethod);
	int routinelake(int nIndex,double qin,double ptwd,double irrwd,double delt60,
		double prec,double pet,double v1,double& v2,double& q2,double& hh);
//	int  ftable(int *nt, double *fh, double *fv, double *fq, double *fs, double *fa, double *fr, double *fb, double *fp, double *qk, double *b0, double *h0, double *r1, double *r2, double *w1, double *dx, double *s, double *mn);
	int  ADCALC(double VOLS, double ROS, double RO, double CRRAT, double DELTS,
		        double& SROVOL, double& EROVOL);
	int  advect(int admod, double volt, double *imat, double *vols, double *srovol, 
		        double *vol, double *erovol, double *conc, double *romat, double delts, 
				double *js, double crrat);
	int  ADVECT(int ADMOD, double VOLT, double IMAT, double VOLS, double SROVOL, 
		        double VOL, double EROVOL, double *CONC, double *ROMAT);	

	int ADECAYSS(double *ADDCPM, double TW20, double *RSED, double *SQAL, double *SQDEC);
	int ADECAYBS(double *ADDCPM, double TW20, double *RSED, double *SQAL, double *SQDEC);
	int ADSDES(double VOL, double *RSED, double *ADPM1, double *ADPM2,double *ADPM3, 
			   double TW20, double& DQAL, double *SQAL, double *ADQAL);
	int ADVQAL(double ISQAL, double RSED, double BSED, double DEPSCR, double ROSED,
		       double RSQALS, double RBQALS, double& SQAL, double& BQAL, double& DSQAL, 
			   double& ROSQAL);
	int DDECAY(int *QALFG, double TW20, double *HYDPM, double PHVAL, double *ROXPM,
			   double ROC, double *FACT2, double FACT1, double *PHOTPM, double KOREA,
			   double CFGAS, double *BIOPM, double BIO, double *GENPM, double VOLSP,
			   double DQAL, int HR, double DELT60, double *DDQAL);
	
	// sediment
	int  accum(int *dayfg, double *prec, int *drydfg, double *accsdp, double *remsdp, double *slds);
	int  accum2(int dayfg, double prec, int& drydfg, double accsdp, double remsdp, 
				double& slds, double fr_sand_bs, double fr_silt_bs, double fr_clay_bs,
				double& fr_sand, double& fr_silt, double& fr_clay);
	int  atach(double *affix, double *dets);
	int  BDEXCH(double AVDEPM,double W,double TAU,double TAUCD,double TAUCS,double M,double VOL,double FRCSED,double *SUSP,double *BED,double *DEPSCR);
//	int  cohesv(double *ised, double *vols, double *srovol, double *vol, double *erovol, double *sovol, double *eovol, int *nexits, double *avdepm, double *avdepe, double *sdpm, double *tau, double *ssed, double *rsed, double *rosed, double *osed, double *depscr);
	int  colby(double *v, double *db50m, double *fhrad, double *fsl, double *tempr, double *gsi, int *ferror, int *d50err, int *hrerr, int *velerr);
	int  detach(int *dayfg, int *crvfg, double *coverm, int *mon, int *nxtmon, int *day, int *ndays, double *rain, int *csnofg, double *snocov, double *delt60, double *smpf, double *krer, double *jrer, /*int *nblks,*/ double *cover, double *dets, /* double *detsb,*/ double *det);		
	int  sandld(double isand,double vols,double srovol,double vol,double erovol,
		        double ksand,double avvele,double expsnd,double rom,int sandfg, 
				double db50e,double hrade,double slope,double tw,double wsande, 
				double twide,double db50m,double fsl,double avdepe,double& sand, 
				double& rsand,double& bdsand,double& depscr,double& rosand);
	int  sosed1(/*int *nblks,*/ double *suro, /*double *surob,*/ double *surs, /*double *sursb,*/ double *delt60, double *kser, double *jser, double *kger, double *jger, /*double *nblksi,*/ double *dets, /*double *detsb,*/ double *wssd, double *scrsd, double *sosed /*, double *wssdb, double *scrsdb, double *sosdb, double *stcap, double *stcapb */);
//	int  sosed2(/*int *nblks,*/ double *suro, /*double *surob,*/ double *delt60, double *kser, double *jser, double *kger, double *jger, /*double *nblksi,*/ double *dets, /*double *detsb,*/ double *wssd, double *scrsd, double *sosed /* , double *wssdb, double *scrsdb, double *sosdb, double *stcap, double *stcapb */);
	int  sosld1(double *suro, double *surs, double *delt60, double *keim, double *jeim, double *slds, double *sosld);
//	int  sosld2(double *suro, double *delt60, double *keim, double *jeim, double *slds, double *sosld);
	int  toffal(double *v, double *fdiam, double *fhrad, double *slope, double *tempr, double *vset, double *gsi);
//	int  washsd(int *dayfg, int *vpfwfg, double *sosld, double *potfwm, int *mon, int *nxtmon, int *day, int *ndays, double *potfw, double *soqs);
	int  berflow(int nIndex, double delts, int nRecord, int nf, double vin, double *fv, double *fq, double *fa, double *fr, int *st, double *XA, double *HR, double *q2);
	int  bersed(double jber, double kber, double rchlen, double delt60,double q_act, double q_thr, double xa_act, double xa_thr,double hr_act, double hr_thr, double *bersd);

	// for PH model 
	int  pstemp(int hrfg,int dayfg,int tsopfg,int msltfg,int miftfg,
					int mgwtfg,int *mon,int *nxtmon,int *day,int *ndays,
					double& aslt,double *asltm,double& bslt,double *bsltm,
					double& aift,double *aiftm,double& bift,double *biftm,
					double& agwt,double *agwtm,double& bgwt,double *bgwtm,
					double airtmp,double& sltmp,double& iftmp,double& gwtmp);
	int  adjtemp(double *eldat, double *gatmp, double *airtmp, int hrfg, double *prec, int *delt, int hr);
	int  caltemp(int dayfg,int day,int ndays,int mon,int nxtmon,int preflg,int bedflg,int tgflg,double solrad,double dewtmp,double airtmp,
				double wind,double cfsaex,double katrad,double kcond,double delt60,double kevap,double avdepe,double prec,double kmud,
				double muddep, double kgrnd,double tws,double cfpres,double *tgrndm,double& cloud,double& tw,double& tgrnd,double& tmud,
				double& tmuddt);
	int  bedht2(double tws,double tw,double kmud,double kgrnd,double muddep,double tgrnd,double& tmud,double& tmuddt,double& qbed);
//	int  updateco2(double *delt60, double *co1, double *co2, double *avdepe, double *avvele, double *tw, int *reamfg, int *decfg, double *p, double *r__, double *ben, double *deltcd, double *co2sa);
	int  phcalc(int *phdbfg, int *phcnt, double *coeff1, double *coeff2, double *coeff3, double *coeff4, int *messu, double *hest, double *hllim, double *hulim, double *ph, int *convfg, double *hplus);
	int  PHCALC(int PHDBFG,int PHCNT,double COEFF1,double COEFF2,double COEFF3,double COEFF4,double *HEST,double *HLLIM,double *HULIM,double *PH,int *CONVFG,double *HPLUS);
	int  phcarb(double *ph, double *alk, double *tic, double *co2, double *tw, double *vol, int *convfg, int *phdbfg);
	int  soilgas(double elevgc,int midofg,int madofg,int mico2fg,int maco2fg,int dayfg,double *idoxp,double *adoxp,double *ico2p,
				double *aco2p,double *idoconcm,double *adoconcm,double *ico2concm,double *aco2concm,int mon,int nxtmon,int day,int ndays,double wt,
				double suro,double ifwo,double agwo,double route_suro,double route_ifwo,double route_agwo,double bypass_suro,double bypass_ifwo,double bypass_agwo,
				double *pdoxout,double *pco2out,double *pdoxout_route,double *pco2out_route,double *pdoxout_bypass,double *pco2out_bypass);
	int  inocarbon(double *co2, double *wt, double *ioc, double *ph);
	int  lookupsws();
	void AssignIndex();

	//RQUAL (03-2005)
	int SINK(double VOL,double AVDEPE,double KSET,double *CONC,double *SNKMAT);
	int OXBEN(double *BRBOD,double BENOD,double TW,double DEPCOR,
				   double SCRFAC,double TCBEN,double EXPOD,double EXPREL,
				   double *DOX,double *BOD,double *DOBEN,double *BODBNR);
	int OXREA(int LKFG,	double WIND, double	CFOREA,	double AVVELE,
				double	AVDEPE,	double	TCGINV,	int		REAMFG,
				double	*REAK,	double	REAKT,	double	*EXPRED,	double	*EXPREV,
				double	LEN,	double	DELTH,	double	TW,		double	DELTS,
				double	DELT60,	double	*KOREA);
	int BODDEC(double TW,double KBOD20,double TCBOD,double *BOD,double *DOX,double *BODOX);
	int NH3VOL(double EXPNVG,double EXPNVL,double KOREA,double WIND,
					double DELT60,double DELTS,double AVDEPM,double TWKELV,
					double TW,double PHVAL,double *TAM,double *NH3VLT);
	int HCINTP(double PHVAL,double TW,double *HCNH3);
	int INTRP1(double *XARR,double *YARR,int LEN,double XVAL,double *YVAL);
	int ADVNUT(double ISNUT,double RSED,double BSED,double DEPSCR,double ROSED,double RSNUTS,
			   double RBNUTS,double BNUT,double &SNUT,double &DSNUT,double &ROSNUT);
	int AMMION(double TW,double PH,double TAM,double *NH3,double *NH4);
	int BENTH(double DOX,double ANAER,double *BRCON,double SCRFAC,
				   double DEPCOR,double *CONC,double *RELEAS);
	int NITRIF(double KTAM20,double TCNIT,double TW,int NO2FG,
					double KNO220,double *TAM,double *NO2,double *NO3,
					double *DOX,double *DODEMD,double *TAMNIT,
					double *NO2NTC,double *NO3NIT);
	int DENIT(double KNO320,double TCDEN,double TW,double DOX,
				   double DENOXT,double *NO3,double *DENNO3);
	int DECBAL(int TAMFG,int PO4FG,double DECNIT,double DECPO4,
					double *TAM,double *NO3,double *PO4);
	int ADDSNU(double VOL,double *RSED,double *ADPM,double *DNUT,
					double *SNUT,double *DNUTXX,double *ADNUT);
	int ADVPLK(double IPLANK,double VOLS,double SROVOL,double VOL,double EROVOL,
				int ADMOD, double VOLT, double AVVELM, double OREF,double MXSTAY,
				double SEED,double DELTS,double *PLANK,double *ROPLK, double *JS, 
				double crrat);
	int LITRCH(double INLIT,double EXTB,double EXTCLA,double EXTSED,
					double AVDEPE,int PHYFG,int BALFG,double *PHYLIT,
					double *BALLIT,double *CFLIT);
	int PHYRX(double PHYLIT,double TW,double TALGRL,double TALGRH,
				   double TALGRM,double MALGR,double CMMP,double CMMNP,
				   int TAMFG,int AMRFG,int NSFG,double CMMN,double CMMLT,
				   double DELT60,double CFLIT,double ALR20,double CVBPN,
				   int PHFG,int DECFG,double CVBPC,double PALDH,double NALDH,
				   double CLALDH,double ALDL,double ALDH,double ANAER,
				   double OXALD,double ALNPR,double CVBO,double REFR,
				   double CVNRBO,double CVPB,double CVBCL,CString *LIMIT,
				   double CO2,double *PO4,double *NO3,double *TAM,double *DOX,
				   double *ORN,double *ORP,double *ORC,double *BOD,double *PHYTO,
				   CString *LIMPHY,double *PHYCO2,double *PHYCLA,double *DOPHY,
				   double *BODPHY,double *TAMPHY,double *NO3PHY,double *PO4PHY);
	int BALRX(double BALLIT,double TW,double TALGRL,double TALGRH,double TALGRM,
				   double MALGR,double CMMP,double CMMNP,int TAMFG,int AMRFG,int NSFG,
				   double CMMN,double CMMLT,double DELT60,double CFLIT,double ALR20,
				   double CVBPN,int PHFG,int DECFG,double CVBPC,double PALDH,
				   double NALDH,double ALDL,double ALDH,double ANAER,double OXALD,
				   double CFBALG,double CFBALR,double ALNPR,double CVBO,double REFR,
				   double CVNRBO,double CVPB,double MBAL,double DEPCOR,CString *LIMIT,
				   double CVBCL,double CO2,double *PO4,double *NO3,double *TAM,double *DOX,
				   double *ORN,double *ORP,double *ORC,double *BOD,double *BENAL,CString *LIMBAL,
				   double *BALCO2,double *BALCLA,double *DOBALG,double *BODBAL,double *TAMBAL,
				   double *NO3BAL,double *PO4BAL);
	int ALGRO(double LIGHT,double *PO4,double *NO3,double TW,double TALGRL,
				   double TALGRH,double TALGRM,double MALGR,double CMMP,
				   double CMMNP,int TAMFG,int AMRFG,double *TAM,int NSFG,
				   double CMMN,double CMMLT,double ALR20,double CFLIT,double DELT60,
				   CString *LIMIT,CString *LIMR,double *GRO,double *RES);
	int GROCHK(double *PO4,double *NO3,double *TAM,int PHFG,int DECFG,double CO2,
					double CVBPC,double CVBPN,int NSFG,double *GROW);
	int PHYDTH(int NSFG,double *NO3,double *TAM,double *PO4,double PALDH,
					double NALDH,double *PHYCLA,double CLALDH,double ALDL,
					double ALDH,double *DOX,double ANAER,double OXALD,
					double STC,double *DTHPHY);
	int ORGBAL(double DTHORN,double DTHORP,double DTHORC,double DTHBOD,
					double *ORN,double *ORP,double *ORC,double *BOD);
	int NUTRUP(double GROW,int NSFG,double CVBPN,double ALNPR,double CVBPC,
					int PHFG,int DECFG,double *PO4,double *TAM,double *NO3,
					double *ALCO2,double *TAMALG,double *NO3ALG,double *PO4ALG);
	int BALDTH(int NSFG,double *NO3,double *TAM,double *PO4,double PALDH,
					double NALDH,double ALDL,double ALDH,double MBAL,double *DOX,
					double ANAER,double OXALD,double *BAL,double DEPCOR,double *DTHBAL);
	//snow related functions
	int  albed(int *mon, double *lat, double *dull, double *albedo);
	int  compac(int *iregfg, double *delt60, double *packf, double *pdepth, double *compct);
	int  cooler(int *iregfg, double *airtmp, double *packf, double *paktmp, double *delt60, double *sumht, double *mneghs, double *neght, double *neghts);
	int  effprc(int *snoffg, double *snowf, int *rainfg, double *rainf, double *rdnsn, double *covind, double *delt60, double *snocov, double *covinx, double *packf, double *pdepth, double *dull, double *prain);
	int  gmelt(int *iregfg, double *paktmp, double *mgmelt, double *packf, double *wyield, double *packw, double *pdepth, double *packi, double *gmeltr, double *neghts);
	int  hexchr(double *ccfact, double *winmov, double *vap, double *airtmp, double *melev, int *mon, double *lat, double *dull, double *solrad, double *shade, double *delt60, double *skyclr, double *mostht, double *albedo);
	int  icing(int *hr, double *snocov, double *airtmp, int *hr6fg, double *xlnmlt, double *wyield, double *packf, double *packi, double *pdepth);
	int  liquid(int *iregfg, double *pdepth, double *mwater, double *packf, double *melt, double *prain, double *rnfrz, double *delt60, double *packw, double *packwc, double *wyield);
	int  melter(double *sumht, double *packf, double *packi, double *pdepth, double *melt);
	int  meteor(double *prec, int *hrfg, double *airtmp, double *tsnow, double *dtmpg, double *snowcf, double *rdcsn, int *delt, int *ccfp, /*int *ivl1,*/ double *snotmp, int *dryfg, double *skyclr, double *dewtmp, int *snoffg, double *snowf, int *rainfg, double *rainf, double *rdnsn,double *cloud);
	int  nopack(double *covind, int *hr6fg, double *packf, double *packi, double *packw, double *pack, double *pdepth, double *rdenpf, double *covinx, double *snocov, double *dull, double *albedo, double *xlnmlt, double *mneghs, double *paktmp, double *neghts, double *packwc, double *neght, double *gmeltr, double *compct, double *snowep, double *mostht, double *vap);
	int  snowev(int *iregfg, double *svp, double *dewtmp, double *airtmp, double *snocov, double *snoevp, double *winmov, /*int *lsno, int *messu, int *msgfl, int *datim,*/ double *snowep, double *packf, double *pdepth, double *packi, /*int *ecnt1,*/ double *snowe, double *vap);
	int  vapor(double *svp, double *tmp,/* int *lsno, int *messu, int *msgfl, int *datim, int *ecnt1,*/ double *vap);
	int  warmup(double *prain, double *sumht, double *neghts, double *pdepth, double *packf, double *rnfrz);

	// whether to write the result to db or text file
	bool bWeather;
	bool bCalcEnd;
	CString strComments[1000]; //user comments
	CString strDesc[1000]; //parameter description of each card

	//cross-section related card 413
	double rec_width;
	double rec_height;

	// time series
//	WeatherStationData *tmList;

	// Parameters used in Pwater
	// General Information
	// nsws = number of subwatershed
	// ngroup   = number of land groups to assign parameters
	// nlnadp   = maximum number of land use (pervious land)
	// nrgroup   = number of stream groups to assign parameters
	// optlevel = general output control
	// deltm  = input time step (in min)     
	
	int nsws,ngroup,nlandp,nrgroup,optlevel,summarylevel,nlandimp;
	COleDateTime twStart, twEnd;	// start and end time from weather file
	COleDateTime tStart, tEnd;		// start and end time for simulation
	COleDateTime toStart, toEnd;	// start and end time for output file  
	int deltm;					// input time step (min)
	double delts,delt60,deltd;	// model time step (sec, hour, and day)		
	int FtableRecord;										 
	CString infomsg;			 
	CString m_strFileName;		// input file name
	CString m_strPath;			// input file location
	CString strPointFileName;	// input point file name		 
	CString strPointFilePath;	// input file location			 
	bool bReadDB;
	bool bRunNIMS;
	//optional output folder path and weather folder path (card 30)
	int nReadc30;

	//optional land-based TMDL card path (card 670)
	bool bReadc670;

	CString m_strModelResultLocation;
	CString m_strPSDataLocation;
	swsinfo *pswsinfo; 
    
	//  Land use
	LandUsePInfo *ppluinfo;
	LUInfo *pluinfop;
	LURoute *pLURoute;

	Ftable_Selected *pFtable;
	
	// Control Flags
	//
	//   pwatfg = 1 run pwater
	//   sedfg = 1 run sediment
	//   pqalfg = 1 run general quality
	//   rtopfg = set to 2, use new method.
	//   vcsfg    interception storage capacity  
	//   vuzfg    upper zone nominal storage     
	//   vnnfg    manning's n for the overland flow plane 
	//   vifwfg   interflow inflow parameter  
	//   vircfg   interflow recession constant  
	//   vlefg    lower zone evapotranspiration (e-t) parameter
	
	int snowfg,pwatfg,sedfg,gqsdfg,pqalfg,tempfg,oxfg,nutfg,plkfg,phfg,
		vcsfg,vuzfg,vnnfg,vifwfg,vircfg,vlefg,uzfg,rtopfg;
	
	// parameters
	PW_PARA2 *ppw_param2;
	PW_PARA3 *ppw_param3;
	PW_PARA4 *ppw_param4;
	PW_STATE *ppw_state;
	
	// monthly vary parmaters
	MON_DATE *cepscm_p;
	MON_DATE *uzsnm_p;
	MON_DATE *nsurm_p;
	MON_DATE *intfwm_p;
	MON_DATE *ircm_p;
	MON_DATE *lzetpm_p;
	int nQuals;
	QUALS *pQual;
	QUALSWQ *pQualwq;			 
	QUAL_INPUT  *pQualInput;
	GQ_SEDDECAY *pGQ_SEDDECAY;
	GQ_KD       *pGQ_KD;
	GQ_ADRATE   *pGQ_ADRATE;
	GQ_ADTHETA  *pGQ_ADTHETA;
	GQ_SEDCONC  *pGQ_SEDCONC;

	IMON_DATE	*pIMonAccum;
	IMON_DATE	*pIMonSQOLIM;
	IMON_DATE	*pIMonSUROCONC;				 
	IMON_DATE	*pIMonINTERCONC;
	IMON_DATE	*pIMonGRNDCONC;
	IMON_DATE	*pIMonDRYDEP;
	IMON_DATE	*pIMonWETDEP;

	MON_DATA	*pMDDrch;		// monthly atmospheric dry deposition on reach
	MON_DATA	*pMWDrch;		// monthly atmospheric wet deposition on reach

	MON_TDATA	*pMTDATA;		// monthly threshold standard value
	THR_RESULTS	*pTHR_RESULTS;	// monthly threshold results 
	THR_MAPPING	*pThreshold;

	int vqofg,qsowfg,vsqcfg,qifwfg,viqcfg, qagwfg,vaqcfg;
	int adfglnd,maddfglnd,mawdfglnd,adfgrch,maddfgrch,mawdfgrch;

	// card 400
	int vconfg; // monthly conversion factor for FTable
	int sedber;	// stream bank erosion flag
	int nch;
	int admod;
	double kc;
	double js;

	// card 415
	ReachFtable *pReachFtable;
	// card 410
	ReachInfo *pReachInfo;
	// card 405  network
	ChannelInfo *pChannelInfo;
	// card 420 
	int nPtSource,nPtWithDrawal,nPtQuals;	// (08-15-05)

	// card 435
	int nLinkFiles;

	// summary file
	long counter;

	// card 440
	int crvfg; // monthly cover factors
	int vsivfg;	
	int sandfg;	
	int sweepfg;
	MONTH_TMP *pCRVm;
	MONTH_TMP *pVSIVm;

	PtQual *pPtQual;		// (09-23-05)
	PtWithDrawal *pPtWithDrawal;
	// point source data
	PSData *pPSData;

	// sediment parm
	SD_SWEEP  *psd_sweep;
	GQ_SWEEP  *pGQ_sweep;
	SD_PARA1  *psd_param1;
	SD_PARA2  *psd_param2;
	SD_BER    *rsd_berparam;
    STREAMSED_GENPARM *rsd_genparam;
    STREAMSED_PARM *rsd_param;

	// temperature
	int msltfg,miftfg,mgwtfg;
	double lapse[24];	//dry period lapse rate expressed as deg F/ft 

	MONTH_TMP *asltm;
	MONTH_TMP *bsltm;
	MONTH_TMP *aiftm;
	MONTH_TMP *biftm;
	MONTH_TMP *agwtm;
	MONTH_TMP *bgwtm;

	bool bedflg, mtgflg;
	TMP_PARA1 *ptmp_param1;
	TMPS_PARA1 *ptmps_param1;	
	TMPS_PARA2 *ptmps_param2;
	MONTH_PARM *ptmps_mon;
	// soil temperature
	STMP_PARA1 *pstmp_parm1;

	// DO CO2 paramters
	int midofg,mico2fg,madofg,maco2fg;

	DOCO_PARA1  *doco_parm1; 
	MONTH_OX *sdoconc;		// monthly DO conc in surface flow 
	MONTH_OX *idoconc;		// monthly DO conc in interflow 
	MONTH_OX *adoconc;		// monthly DO conc in groundwater
	MONTH_OX *sco2conc;		// monthly CO2 conc in surface flow
	MONTH_OX *ico2conc;		// monthly CO2 conc in interflow
	MONTH_OX *aco2conc;		// monthly CO2 conc in groundwater

	//RQUAL (03-2005)
	SEDGQUAL_MAPP *sed_mapp;
	LAND_MAPP  *land_mapp; 
	ATMS_MAPP  *atms_mapp; 
	int benrfg, reamfg;
	OX_PARA1  *ox_parm1; 
	OX_PARA2  *ox_parm2; 
	OXRX_INIT *oxrx_init; 
	SCOUR_PARMS *scour_parms;
	// nutrients
	int tamfg, no2fg, po4fg, amvfg, denfg, adnhfg, adpofg, mphfg;
	NUT_PARA1  *nut_parm1; 
	NUT_PARA2  *nut_parm2; 
	NUT_PARA3  *nut_parm3; 
	NUT_INIT   *nut_init; 
	// planks
	int phyfg, zoofg, balfg, sdltfg, amrfg, decfg, nsfg, orefg;
	PLANK_PARA1  *plank_parm1; 
	PLANK_PARA2  *plank_parm2; 
	PLANK_PARA3  *plank_parm3; 
	PLANK_PARA4  *plank_parm4; 
	PLANK_INIT   *plank_init; 
	// ph
	int phffg1, phffg2;
	double phfrc1, phfrc2;
	PH_PARA   *ph_para; 
	PH_INIT   *ph_init; 
	MONTH_PH  *mtic_rf;		// monthly removal fraction for total inorganic carbon
	MONTH_PH  *mco2_rf;		// monthly removal fraction for dissolved carbon dioxide
	MONTH_PH  *pMONTH_COV;

	// card 600
	int ncpt, ncland, ncrch, ntrgp, ntnum;

	// card 670
	TMDL_LU_QUAL_CPARAM *pTMDLDistrib;
	//	
	double *pFlowControls;
	double *pSedControls;
	double *pQualControls;

	Channel *stream;
	//added for point source related qual conversion
	double *fPSQualDeduction;
	PSReduction *pPSReduction;
	MON_SWITCH  *pMON_SWITCH;
	BMPReduction *pBMPReduction;
	BMPLimit *pBMPLimit;

	// snow related parameters
	CSnowControl2 *pSnowCtl2;
	CSnowParam1 *pSnowParam1;
	CSnowParam2 *pSnowParam2;
	CSnowInit1 *pSnowInit1;
//	bool blistOutput[PARA_NUM];
	bool bDataValid;
	// for output flags
	int nHydro, nSed, nGQUAL, nRQUAL, nSnow, nCustom, nDefault, NumSed, 
		nLUsummary, nCHsummary, nThreshold, nSUSTAIN, nPSsummary; 
	// for control enable/disable
	int nContorlGlobal;
	int nWaterControl;
	FILE* m_filelanduse;		// used for outputing landuse summary 
	FILE* m_filestream;			// used for outputing stream summary
	FILE* m_filepointsource;	// used for outputing point source summary
	FILE* m_filethreshold;		// used for outputing threshold analysis summary
	FILE* m_filethresholdstat;	// used for outputing threshold analysis statistics
	CString *rqual_s;
	CString *rqname_s;
	int *rqflag_s;			// to get the selecetd rquals
	int ncrchOutlets;
	double *q_bfd;			// flow at the bank full depth

	//Irrigation Application Options
	int irrigfg, petfg, monVaryIrrig ; //Irrigation application option flags
	IrrigInfo *pirriginfop ;        //added by Sabu Paul (9-2004) 
	IrrigPET *pirrigpet;		// constant pet value
	MonETCInfo *pmonetcinfop;  //added by Sabu Paul (10-2004)
	IrrigWDInfo *pirrigwdinfo;		//Irrigation withdrawal info -- Sabu Paul (9-2004)
	double *cumulativePrec;
	double *cumulativeET; 
	double *curPeriodPrec;
	double *curPeriodET;
	double *irrigDemand;
	double *wsIrrigDemand;
	double GetIrrWD(int wsIndex, double delts,double vol1, COleDateTime tCurrent);
	double GetPointSourceWithDrawal(int nIndex,double lfSecs,double *lfVol,double *lfConc,COleDateTime tCurrent);
	bool   GetUpStreamAndUpBypassLoad(int nPolType,int nIndex,double &mup,double &mup_bypass);
	double GetBMPReductionLoad(int nPolType,int nQualIndex,int nStreamIndex,int nMonth,double lfSeconds);
	bool   ProcessLanduseData();
	bool   ProcessPointSourceSummaryData();
	bool   CalculateSandSiltClayFractions(double *fr_sand, double *fr_silt, double *fr_clay, double *fr_sand_bs, double *fr_silt_bs, double *fr_clay_bs);
	bool   ApplyStreamIceMelt(int nIndex, double tw, double wyield, double pdepth, double *q2, double *q2_frozen);
	CLANDUSE	cLU;	// landuse
	COUTPUT		cOP;	// output manager
	CWEATHER	cWD;	// weather data
};

//global
int compare (const void *a, const void *b);
CString GetValueText(double fNum);
bool FolderExists(CString strFolderPath);
bool DuplicateExists(int nArraySize, CString* strArray);

#endif // !defined(AFX_LSPCMODEL_H__A3D833A8_BD4A_11D3_A495_002078143FF2__INCLUDED_)
