// WEATHER.h: interface for the CWEATHER class.
//
//////////////////////////////////////////////////////////////////////
#include <afxdb.h>
#include <afxtempl.h>

#if !defined(AFX_WEATHER_H__A3D833A8_BD4A_11D3_A495_002078143FF2__INCLUDED_)
#define AFX_WEATHER_H__A3D833A8_BD4A_11D3_A495_002078143FF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	W_PREC	1	//PRECIPITATION (IN/TIMESTEP)
#define	W_PET	2	//POT ET (IN/TIMESTEP)
#define	W_TEMP	3	//AIR TEMP (DEG F)
#define	W_WIND	4	//WIND SPEED (MI/TIMESTEP)
#define	W_SRAD	5	//SOLAR RAD (LY/TIMESTEP)
#define	W_DEWP	6	//DEW POINT (DEG F)
#define	W_CLOU	7	//CLOUD COVER (TENTHS)

typedef struct tagWDATA
{
	int nFileIndex;				
	int nParmIndex;
} WDATA;

class WEATHERDATA
{
public:
	double	*lfVALUE;// array of wetaher parameter values
	WEATHERDATA()
	{
		lfVALUE = NULL;
	}
	~WEATHERDATA()
	{
		if (lfVALUE != NULL)
			delete[]lfVALUE;
	}
};

class WFILEINFO
{
public:
	CString		strWFile;		// weather file name
	long		lIndex;			// index of record to read data
	long		lRecords;		// number of records in the file
	int			nWFileID;		// weather file ID
	int			nWParam;		// number of weather parameters
	int			*m_nParamID;	// array of wetaher parameter ID
	WEATHERDATA	*pWEATHERDATA;	// array of weather data
//	WEATHERDATA	*iWEATHERDATA;	// array of interpolated weather data
	
	WFILEINFO()
	{
		nWFileID = -1;
		strWFile = "";
		nWParam = 0;
		lIndex = 0;
		lRecords = 0;
		m_nParamID = NULL;
		pWEATHERDATA = NULL;
//		iWEATHERDATA = NULL;
	}
	~WFILEINFO()
	{
		if(m_nParamID!= NULL)	
			delete[]m_nParamID;
		if(pWEATHERDATA!= NULL)	
			delete[]pWEATHERDATA;
//		if(iWEATHERDATA!= NULL)	
//			delete[]iWEATHERDATA;
	}
};

class WSTATIONINFO
{
public:
	int		nWStationID;	// weather station ID
	int		nWFile;			// number of weather files
	int		*m_nWFileIndex;	// array of weather file Index (not actual ID)
	double	*m_lfMultiplier;// weather parameter multiplier
	WDATA   nWDATA[7];		// array of weather file index and parameter index 
	WSTATIONINFO()
	{
		nWStationID = -1;
		nWFile = 0;
		for (int i=0; i<7; i++)
		{
			nWDATA[i].nFileIndex = -1;
			nWDATA[i].nParmIndex = -1;
		}
		m_nWFileIndex = NULL;
		m_lfMultiplier = NULL;
	}
	~WSTATIONINFO()
	{
		if(m_nWFileIndex!= NULL)	
			delete[]m_nWFileIndex;
		if(m_lfMultiplier!= NULL)	
			delete[]m_lfMultiplier;
	}
};

class CWEATHER  
{
public:
	CWEATHER();
	virtual ~CWEATHER();

public:
	int	nNumWeatherParam;			// number of weather parameters
	int	nNumWeatherFiles;			// number of weather files
	int	nNumWeatherStations;		// number of weather stations
	CString strWeatherFolderPath;	// weather folder path
	WFILEINFO		*pWFILEINFO;	// array of weather file info
	WSTATIONINFO	*pWSTATIONINFO;	// array of weather stations info

public:
	void	ResetData();
	bool	WeatherDataMapping();
	int		FindFileIndex(int nFileID);
	int		FindStationIndex(int nStationID);
//	long	GetNumberOfRecords(FILE *fp);
	bool	SkipCommentLines(FILE *fp);	
	bool	ReadAirFile(int delt,int nFileIndex,CString strFileName,COleDateTime twStart,COleDateTime twEnd);
	bool	ReadPreFile(int delt,int nFileIndex,CString strFileName,COleDateTime twStart,COleDateTime twEnd);
//	bool	ProcessWeatherFiles(int delt,COleDateTime twStart,COleDateTime twEnd);
//	bool	GetWeatherDateTime(COleDateTime& twStart,COleDateTime& twEnd);
	bool	GetWeatherDateTimeandInterval(int& delt,COleDateTime& twStart,COleDateTime& twEnd);
	long	FindDataIndex(int delt,int nFileIndex,COleDateTime tCurrent,COleDateTime tStart,COleDateTime tEnd);
//	bool	InterpolateWeatherData(int delt);
//	double	GetParamValue(int delt,int nParamID,int nStationIndex,COleDateTime tCurrent);
};

#endif // !defined(AFX_WEATHER_H__A3D833A8_BD4A_11D3_A495_002078143FF2__INCLUDED_)
