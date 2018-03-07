// LANDUSE.cpp: implementation of the CLANDUSE class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LANDUSE.h"
#include "LSPC.h"
#include "LSPCMODEL.h"
#include "StringToken.h"
#include "RecordsetEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLANDUSE::CLANDUSE()
{
	nLUchangeFlag = 0;
	nNumLUchangeRecords = 0;
	strLUchangeFilePath = "";
	pLUDATA = NULL;
}

CLANDUSE::~CLANDUSE()
{
	ResetData();
}

void CLANDUSE::ResetData()
{
	if(pLUDATA != NULL)
	{
		delete []pLUDATA;
		pLUDATA = NULL;
	}
	return;
}

void CLANDUSE::ReadLUchangeDatabase(bool bRunNIMS)
{
	if(pLUDATA != NULL)
	{
		delete []pLUDATA;
		pLUDATA = NULL;
	}

	if(nLUchangeFlag == 0)
		return;

	CDatabase db;
	BOOL bRet = FALSE;
	BOOL bCanOpen = FALSE;
	CString strSQL, strCard;
	
	try
	{
		strSQL = CLSPCApp::GetMDBDriver();
		if(strSQL.IsEmpty())
		{
			AfxMessageBox("Cannot find the Microsoft Access ODBC driver.");
			return;
		}

		// update landuse change flag and file path
		CLSPCApp *pApp = (CLSPCApp *) AfxGetApp();
		strLUchangeFilePath = pApp->strLocation;

		if (bRunNIMS == true)
			return;

		bCanOpen = db.OpenEx(strSQL,CDatabase::noOdbcDialog);
	}
	catch(CException e)
	{
		AfxMessageBox("ODBC Error ---Cannot open database for reading.");
		return;
	}
	if(!bCanOpen)
	{
		AfxMessageBox("ODBC Error ---Cannot open  database for reading.");
		return;
	}
	
	TRY
	{
		CRecordsetEx rs1(&db);
		CString strTemp;
		
		//count the rows in a table
		strSQL = "select count(*) from Landuse_TS";
		strCard = "Table Landuse_TS";
		TRY
		{
			bRet = rs1.ExecDirect(strSQL);
			rs1.GetFieldValue((short)0, strTemp);
			nNumLUchangeRecords = atol(LPCSTR(strTemp));
		}
		CATCH(CException, e)
		{
			nLUchangeFlag = 0;
			nNumLUchangeRecords = 0;
			strLUchangeFilePath = "";
			rs1.Close();
			db.Close();
			AfxMessageBox("Error reading "+strCard+" landuse change data from the database.");
			return;
		}
		END_CATCH;

		rs1.Close();

		// allocate memory
		if (nNumLUchangeRecords > 0)
			pLUDATA = new LANDUSEDATA[nNumLUchangeRecords];

		//read the data
		CRecordset rs(&db);
		CDBVariant dbVar;
		strSQL = "SELECT * FROM Landuse_TS";
		rs.Open(CRecordset::snapshot, strSQL, CRecordset::readOnly);
		int nRecord = 0;
		while(!rs.IsEOF())
		{
			rs.GetFieldValue("SUBBASIN", strTemp);
			pLUDATA[nRecord].nSUBBASIN = atoi(LPCSTR(strTemp));
			rs.GetFieldValue("DELUID", strTemp);
			pLUDATA[nRecord].nDELUID = atoi(LPCSTR(strTemp));
			rs.GetFieldValue("DATE", dbVar, SQL_C_TIMESTAMP);
			int nYear = dbVar.m_pdate->year;
			int nMonth = dbVar.m_pdate->month;
			int nDay = dbVar.m_pdate->day;
			int nHour = dbVar.m_pdate->hour;
			int nMinute = 	dbVar.m_pdate->minute;
			pLUDATA[nRecord].tDATE = COleDateTime(nYear,nMonth,nDay,nHour,nMinute,0);
			rs.GetFieldValue("AREA_AC", strTemp);
			pLUDATA[nRecord].lfAREA = atof(LPCSTR(strTemp));
			rs.GetFieldValue("SLSUR", strTemp);
			pLUDATA[nRecord].lfSLSUR = atof(LPCSTR(strTemp));
			rs.GetFieldValue("LSUR", strTemp);
			pLUDATA[nRecord].lfLSUR = atof(LPCSTR(strTemp));

			nRecord++;
			if(nRecord >= nNumLUchangeRecords)
				break;
			rs.MoveNext();
		}

		rs.Close();
		db.Close();
	}
	CATCH(CException, e)
	{
		db.Close();
		AfxMessageBox("Error reading "+strCard+" landuse change data from the database.");
		return;
	}
	END_CATCH;
	return;
}

long CLANDUSE::GetNumberOfRecords(FILE *fp)
{
	char strLine[MAXLINE];
	long nStart = ftell (fp);
	long nNumRecords = 0;

	while (fgets (strLine, MAXLINE, fp) != NULL)
    {
		CString str(strLine);
		CStringToken strToken(str);
		CString str0 = strToken.NextToken();

		if(str0[0] != 'C' && str0[0] != 'c')
		{
			nNumRecords++;
		}
		else
		{
			// it is a comment line (ignore)
		}
	}

	fseek (fp, nStart, SEEK_SET);
	return nNumRecords;
}

void CLANDUSE::SkipCommentLine(FILE *fp)
{
	char strLine[MAXLINE];
	long nStart = ftell (fp);

	while (fgets (strLine, MAXLINE, fp) != NULL)
    {
		CString str(strLine);
		CStringToken strToken(str);
		CString str0 = strToken.NextToken();

		if(str0[0] != 'C' && str0[0] != 'c')
		{
			fseek (fp, nStart, SEEK_SET);
			return;
		}
		else
		{
			if(str.GetLength() >= 2)
			{
				nStart = ftell (fp);
			}
		}
	}
}

void CLANDUSE::ReadLUchangeTextFile(CString strFileName)
{
	if(pLUDATA != NULL)
	{
		delete []pLUDATA;
		pLUDATA = NULL;
	}

	if(nLUchangeFlag == 0 || strFileName.GetLength() == 0)
		return;

	FILE *fpin = NULL;
	char strLine[MAXLINE];
	CString strError = "";
	
	// open the file for reading
	fpin = fopen (strFileName, "rt");
	if(fpin == NULL)
	{
		AfxMessageBox("Cannot open file "+strFileName+" for reading");
		return;
	}

	// get the number of records
	nNumLUchangeRecords = GetNumberOfRecords(fpin);

	if (nNumLUchangeRecords <= 0)
		return;

	// allocate memory
	pLUDATA = new LANDUSEDATA[nNumLUchangeRecords];
	
	// skip the line start with a 'C'
	SkipCommentLine(fpin);

	int nRecord = 0;
	while(true)
	{
		memset (strLine, 0, MAXLINE);
		// read this line
		if(fgets (strLine, MAXLINE, fpin) == NULL)
		{
			TRACE("\nLanduse change file reading done!\n");
			break;
		}
		CString str(strLine);
		CStringToken strToken(str);
		pLUDATA[nRecord].nSUBBASIN = atoi(LPCSTR(strToken.NextToken()));
		pLUDATA[nRecord].nDELUID = atoi(LPCSTR(strToken.NextToken()));
		int nYear = atoi(LPCSTR(strToken.NextToken()));
		int nMonth = atoi(LPCSTR(strToken.NextToken()));
		int nDay = atoi(LPCSTR(strToken.NextToken()));
		int nHour = atoi(LPCSTR(strToken.NextToken()));
		int nMinute = atoi(LPCSTR(strToken.NextToken()));
		pLUDATA[nRecord].tDATE = COleDateTime(nYear,nMonth,nDay,nHour,nMinute,0);
		pLUDATA[nRecord].lfAREA = atof(LPCSTR(strToken.NextToken()));
		pLUDATA[nRecord].lfSLSUR = atof(LPCSTR(strToken.NextToken()));
		pLUDATA[nRecord].lfLSUR = atof(LPCSTR(strToken.NextToken()));
		nRecord++;
		if (nRecord >= nNumLUchangeRecords)
			break;
	}

	fclose(fpin);
	return;
}

void CLANDUSE::WriteLUchangeTextFile(CString strFileName)
{
	if(nLUchangeFlag == 0 || nNumLUchangeRecords == 0 || pLUDATA == NULL)
		return;

	FILE *fpin = NULL;
	
	// open the file for writing
	fpin = fopen (strFileName, "wt");
	if(fpin == NULL)
	{
		AfxMessageBox("Cannot open file "+strFileName+" for writing");
		return;
	}
	CString strOut;
	CString strTemp;
	CString str;
    str.Format("c %s\n",LSPC_VERSION);

    fputs("c-----------------------------------------------------------------------------------------\n",fpin);
	fputs("c LSPC -- Loading Simulation Program, C++\n",fpin);
    fputs(LPCSTR(str),fpin);
	fputs("c\n",fpin);
	fputs("c Designed and maintained by:\n",fpin);
	fputs("c     Tetra Tech, Inc.\n",fpin);
	fputs("c     10306 Eaton Place, Suite 340\n",fpin);
	fputs("c     Fairfax, VA 22030\n",fpin);
	fputs("c     (703) 385-6000\n",fpin);
    fputs("c-----------------------------------------------------------------------------------------\n",fpin);
	fputs("c LSPC MODEL LANDUSE CHANGE FILE\n", fpin);

    SYSTEMTIME tm;
	GetLocalTime(&tm);

    str.Format("c This input file was created at %02d:%02d:%02d%s on %02d/%02d/%04d\n",(tm.wHour>12)?tm.wHour-12:tm.wHour,tm.wMinute,tm.wSecond,(tm.wHour>=12)?"pm":"am",tm.wMonth,tm.wDay,tm.wYear);
    fputs(LPCSTR(str),fpin);
	fputs("c-----------------------------------------------------------------------------------------\n",fpin);
	
	fputs("c\n",fpin);										
	fputs("c     SUBBASIN: subbasin id\n",fpin);
	fputs("c     DELUID:   land use id\n",fpin);
	fputs("c     YEAR:     year\n",fpin);
	fputs("c     MONTH:    month\n",fpin);
	fputs("c     DAY:      day\n",fpin);
	fputs("c     HOUR:     hour\n",fpin);
	fputs("c     MINUTE:   minute\n",fpin);
	fputs("c     AREA:     area (acres)\n",fpin);
	fputs("c     SLSUR:    slope of overland flow plane (none)\n",fpin);
	fputs("c     LSUR:     length of overland flow plane (feet)\n",fpin);
	fputs("c\n",fpin);
	fputs("c     SUBBASIN  DELUID  YEAR  MONTH  DAY  HOUR  MINUTE  AREA  SLSUR  LSUR\n",fpin);

	for(int i=0; i<nNumLUchangeRecords; i++)
	{
		strOut.Format("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%lf\t%lf\t%lf\n",
			pLUDATA[i].nSUBBASIN,
			pLUDATA[i].nDELUID,
			pLUDATA[i].tDATE.GetYear(),
			pLUDATA[i].tDATE.GetMonth(),
			pLUDATA[i].tDATE.GetDay(),
			pLUDATA[i].tDATE.GetHour(),
			pLUDATA[i].tDATE.GetMinute(),
			pLUDATA[i].lfAREA,
			pLUDATA[i].lfSLSUR,
			pLUDATA[i].lfLSUR);
		fputs(strOut,fpin);
	}
	
	fclose(fpin);
	return;
}

void CLANDUSE::GetLanduseInfo(COleDateTime tCurrent,LUInfo *pluinfop,
							  double& lfSLSUR,double& lfLSUR)
{
	if(pLUDATA == NULL || pluinfop == NULL)
		return;

	if(pluinfop->sindex >= 0)
	{
		int ii = pluinfop->sindex;
		int nIndex = FindLanduseIndex(ii,pluinfop->records,tCurrent);
		if(nIndex != -1)
		{
			if(nIndex == ii)
			{
				lfSLSUR = pLUDATA[nIndex].lfSLSUR;
				lfLSUR  = pLUDATA[nIndex].lfLSUR;
			}
			else
			{
				COleDateTimeSpan t1 = tCurrent - pLUDATA[nIndex-1].tDATE;    
				COleDateTimeSpan t2 = pLUDATA[nIndex].tDATE - tCurrent; 
				double d1 = t1.GetTotalHours();        
				double d2 = t2.GetTotalHours();      
				if(d1 == 0 || d2 == 0)
				{
					if(d1 == 0)
					{
						lfSLSUR = pLUDATA[nIndex-1].lfSLSUR;
						lfLSUR  = pLUDATA[nIndex-1].lfLSUR;
					}
					else
					{
						lfSLSUR = pLUDATA[nIndex].lfSLSUR;
						lfLSUR  = pLUDATA[nIndex].lfLSUR;
					}
				}
				else
				{
					lfSLSUR = (d2*pLUDATA[nIndex-1].lfSLSUR+d1*pLUDATA[nIndex].lfSLSUR)/(d1+d2);
					lfLSUR  = (d2*pLUDATA[nIndex-1].lfLSUR+d1*pLUDATA[nIndex].lfLSUR)/(d1+d2);
				}
			}
		}
	}

	return;
}

double CLANDUSE::GetLanduseArea(COleDateTime tCurrent,LUInfo *pluinfop)
{
	double lfAREA = 0.0;

	if(pLUDATA == NULL || pluinfop == NULL)
		return 0;

	if(pluinfop->sindex >= 0)
	{
		int ii = pluinfop->sindex;
		int nIndex = FindLanduseIndex(ii,pluinfop->records,tCurrent);
		if(nIndex != -1)
		{
			if(nIndex == ii)
			{
				lfAREA  = pLUDATA[nIndex].lfAREA;
			}
			else
			{
				COleDateTimeSpan t1 = tCurrent - pLUDATA[nIndex-1].tDATE;    
				COleDateTimeSpan t2 = pLUDATA[nIndex].tDATE - tCurrent; 
				double d1 = t1.GetTotalHours();        
				double d2 = t2.GetTotalHours();      
				if(d1 == 0 || d2 == 0)
				{
					if(d1 == 0)
					{
						lfAREA  = pLUDATA[nIndex-1].lfAREA;
					}
					else
					{
						lfAREA  = pLUDATA[nIndex].lfAREA;
					}
				}
				else
				{
					lfAREA  = (d2*pLUDATA[nIndex-1].lfAREA+d1*pLUDATA[nIndex].lfAREA)/(d1+d2);
				}
			}
		}
	}

	return lfAREA;
}

int CLANDUSE::FindLanduseIndex(int nIndex, int nRecord, COleDateTime tCurrent)
{
	if(pLUDATA == NULL)
		return -1;

	for(int i=nIndex; i<nIndex+nRecord; i++)
	{
		if(tCurrent <= pLUDATA[i].tDATE)
			return i;
	}

	// current date is beyond the available data (use the last value)
	return nIndex+nRecord-1;	
}

int CLANDUSE::BuildupSweep(double& buildup, double& frac_sand, double& frac_silt, 
						   double& frac_clay, double remeff_sand, double remeff_silt, 
						   double remeff_clay, double area, double percent_area,
						   double runoff, double frequency, 
						   COleDateTime tCurrent, COleDateTime& tLastSwept)
//
//  Input:   buildup = current buildup over land use (tons/ac)
//           frac_sand = fraction of sand in solid storage (0-1)
//           frac_silt = fraction of silt in solid storage (0-1)
//           frac_clay = fraction of clay in solid storage (0-1)
//           remeff_sand = fraction of sand that is available for removal by sweeping (0-1)
//           remeff_silt = fraction of silt that is available for removal by sweeping (0-1)
//           remeff_clay = fraction of clay that is available for removal by sweeping (0-1)
//           area = area devoted to land use (acre)
//           percent_area = area subject to sweeping (fraction)
//           runoff = current runoff on land use (ft/ivl)
//           frequency = days between street sweeping within the landuse
//  Output:  returns the updated buildup after sweeping(tons/ac)
//  Purpose: reduces buildup over a land use if sweeping occurs.
//
{
    double  oldBuildup_sand; // amount of sand in buildup before sweeping (tons/ac)
    double  oldBuildup_silt; // amount of silt in buildup before sweeping (tons/ac)
    double  oldBuildup_clay; // amount of clay in buildup before sweeping (tons/ac)
    double  newBuildup_sand; // amount of sand in buildup after sweeping (tons/ac)
    double  newBuildup_silt; // amount of silt in buildup after sweeping (tons/ac)
    double  newBuildup_clay; // amount of clay in buildup after sweeping (tons/ac)

    // --- see if land use is subject to sweeping
    if ( frequency == 0 ) return FALSE;

    // --- skip land use if area is zero 
    if ( area == 0.0 ) return FALSE;

    // --- no sweeping occurs if land surface is not dry
    if ( runoff > 0.004167 ) return FALSE;

    // --- see if sweep interval has been reached
	COleDateTimeSpan tspan = tCurrent - tLastSwept;
    if ( tspan.GetTotalDays() >= frequency )
    {
        // --- update time when last swept
        tLastSwept = tCurrent;

        // --- reduce buildup by the fraction available
        //     times the sweeping effic.
        oldBuildup_sand = buildup * frac_sand;
        oldBuildup_silt = buildup * frac_silt;
        oldBuildup_clay = buildup * frac_clay;
        newBuildup_sand = oldBuildup_sand * (1.0 - percent_area * remeff_sand);
        newBuildup_silt = oldBuildup_silt * (1.0 - percent_area * remeff_sand);
        newBuildup_clay = oldBuildup_clay * (1.0 - percent_area * remeff_sand);
        newBuildup_sand = min(oldBuildup_sand, newBuildup_sand);
        newBuildup_sand = max(0.0, newBuildup_sand);
        newBuildup_silt = min(oldBuildup_silt, newBuildup_silt);
        newBuildup_silt = max(0.0, newBuildup_silt);
        newBuildup_clay = min(oldBuildup_clay, newBuildup_clay);
        newBuildup_clay = max(0.0, newBuildup_clay);

		//update buildup and fractions
		buildup = newBuildup_sand+newBuildup_silt+newBuildup_clay;
		frac_sand = 0.0;
		if (buildup > 0)
			frac_sand = newBuildup_sand/buildup;
		frac_silt = 0.0;
		if (buildup > 0)
			frac_silt = newBuildup_silt/buildup;
		frac_clay = 0.0;
		if (buildup > 0)
			frac_clay = newBuildup_clay/buildup;
        return TRUE;
    }
	return FALSE;
}

int CLANDUSE::BuildupSweep_GQ(double& buildup, double rem_efficiency, double area, 
							  double percent_area, double runoff, double frequency,
							  COleDateTime tCurrent, COleDateTime& tLastSwept)
//
//  Input:   buildup = current buildup over land use (tons/ac)
//           rem_efficiency = fraction of GQual that is available for removal by sweeping (0-1)
//           area = area devoted to land use (acre)
//           percent_area = area subject to sweeping (fraction)
//           runoff = current runoff on land use (ft/ivl)
//           frequency = days between street sweeping within the landuse
//  Output:  returns the updated buildup after sweeping(tons/ac)
//  Purpose: reduces buildup over a land use if sweeping occurs.
//
{
    double  oldBuildup; // amount of pollutant in buildup before sweeping (tons/ac)
    double  newBuildup; // amount of pollutant in buildup after sweeping (tons/ac)

    // --- see if land use is subject to sweeping
    if ( frequency == 0 ) return FALSE;

    // --- skip land use if area is zero 
    if ( area == 0.0 ) return FALSE;

    // --- no sweeping occurs if land surface is not dry
    if ( runoff > 0.004167 ) return FALSE;

    // --- see if sweep interval has been reached
	COleDateTimeSpan tspan = tCurrent - tLastSwept;
    if ( tspan.GetTotalDays() >= frequency )
    {
        // --- update time when last swept
        tLastSwept = tCurrent;

        // --- reduce buildup by the fraction available
        //     times the sweeping effic.
        oldBuildup = buildup;
        newBuildup = oldBuildup * (1.0 - percent_area * rem_efficiency);
        newBuildup = min(oldBuildup, newBuildup);
        newBuildup = max(0.0, newBuildup);

		//update buildup
		buildup = newBuildup;
        return TRUE;
    }
	return FALSE;
}

int CLANDUSE::CanSweep(int month, int StartSweepMonth, int EndSweepMonth)
{
	// --- see if street sweeping can occur on current month
	int nCanSweep = FALSE;
	if (StartSweepMonth <=  EndSweepMonth)
	{
		if ( month >= StartSweepMonth && month <= EndSweepMonth ) 
			nCanSweep = TRUE;
	}
	else
	{
		if ( (month >= StartSweepMonth && month >= EndSweepMonth)
			|| 
			(month <= StartSweepMonth && month <= EndSweepMonth)) 
			nCanSweep = TRUE;
	}
	return nCanSweep;
}