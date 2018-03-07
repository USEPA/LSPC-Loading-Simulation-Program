// LANDUSE.h: interface for the CLANDUSE class.
//
//////////////////////////////////////////////////////////////////////
#include <afxdb.h>
#include <afxtempl.h>

#if !defined(AFX_LANDUSE_H__A3D833A8_BD4A_11D3_A495_002078143FF2__INCLUDED_)
#define AFX_LANDUSE_H__A3D833A8_BD4A_11D3_A495_002078143FF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct
{
	CString luname;
	bool	bLUchange;	// flag indicating the landuse change
	int		sindex;		// starting index in landuse chage table
	int		records;	// number of records in landuse change table
	int		luid;		// landuse ID		
	int		subbasin;	// subbasin ID
	int		piid;		// pervious/impervious flag
	double	area;		// landuse area (acre)	
	double	slsur;		// overland flow slope
	double	lsur;		// overland flow length
	FILE* m_fileOut;	// used for outputing simulation result 
}LUInfo;

typedef struct tagLANDUSEDATA
{
	COleDateTime tDATE;	// date for the landuse change record
	int		nSUBBASIN;	// subbasin ID
	int		nDELUID;	// landuse ID
	double	lfAREA;		// landuse area (acre)
	double	lfSLSUR;	// overland flow slope
	double	lfLSUR;		// overland flow length
}LANDUSEDATA;

class CLANDUSE  
{
public:
	CLANDUSE();
	virtual ~CLANDUSE();

public:
	int		nLUchangeFlag;			// flag indicating the landuse change data
	long	nNumLUchangeRecords;	// number of records in landuse change table
	CString strLUchangeFilePath;	// landuse change file path
	LANDUSEDATA	*pLUDATA;			// array of landuse change data

public:
	void	ResetData();
	void	SkipCommentLine(FILE *fp);
	long	GetNumberOfRecords(FILE *fp);
	void	ReadLUchangeDatabase(bool bRunNIMS);
	void	ReadLUchangeTextFile(CString strFileName);
	void	WriteLUchangeTextFile(CString strFileName);
	void	GetLanduseInfo(COleDateTime tCurrent,LUInfo *pluinfop,double& lfSLSUR,
			double& lfLSUR);
	double	GetLanduseArea(COleDateTime tCurrent,LUInfo *pluinfop);
	int		FindLanduseIndex(int nIndex,int nRecord,COleDateTime tCurrent);
	int		CanSweep(int month, int StartSweepMonth, int EndSweepMonth);
	int		BuildupSweep(double& buildup, double& frac_sand, double& frac_silt, 
		    double& frac_clay, double remeff_sand, double remeff_silt,
			double remeff_clay, double area, double percent_area,double runoff, 
			double frequency, COleDateTime tCurrent, COleDateTime& tLastSwept);
	int		BuildupSweep_GQ(double& buildup, double rem_efficiency, double area, 
			double percent_area, double runoff, double frequency,
			COleDateTime tCurrent, COleDateTime& tLastSwept);
};

#endif // !defined(AFX_LANDUSE_H__A3D833A8_BD4A_11D3_A495_002078143FF2__INCLUDED_)
