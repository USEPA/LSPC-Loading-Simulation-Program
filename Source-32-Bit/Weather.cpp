// WEATHER.cpp: implementation of the CWEATHER class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WEATHER.h"
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

CWEATHER::CWEATHER()
{
	nNumWeatherParam = 0;
	nNumWeatherFiles = 0;
	nNumWeatherStations = 0;
	strWeatherFolderPath = "";

	pWFILEINFO = NULL;
	pWSTATIONINFO = NULL;
}

CWEATHER::~CWEATHER()
{
	ResetData();
}

void CWEATHER::ResetData()
{
	if(pWFILEINFO != NULL)
	{
		delete []pWFILEINFO;
		pWFILEINFO = NULL;
	}
	if(pWSTATIONINFO != NULL)
	{
		delete []pWSTATIONINFO;
		pWSTATIONINFO = NULL;
	}
	return;
}

bool CWEATHER::SkipCommentLines(FILE *fp)
{
	char strLine[MAXLINE];
	long nStart = ftell (fp);

	while (fgets (strLine, MAXLINE, fp) != NULL)
    {
		CString str(strLine);
		str.MakeLower();
		// skip anoter two lines once found "Date/time"
		if(str.Find("date/time") != -1)
		{
			for(int i=0; i<1; i++)
			{
				if(fgets(strLine, MAXLINE, fp) == NULL)
				{
					AfxMessageBox("Check weather file");
					return false;
				}
			}
			nStart = ftell (fp);
			break;	
		}
	}

    fseek (fp, nStart, SEEK_SET);
	return true;
}
/*
long CWEATHER::GetNumberOfRecords(FILE *fp)
{
	char strLine[MAXLINE];
	long nNumRecords = 0;
	long nStart = ftell (fp);

	if (!SkipCommentLines(fp))
		return -1;

    // scan the file to see how many records are in the file
	while (fgets(strLine, MAXLINE, fp) != NULL)
    {
		CString str(strLine);
		str.TrimLeft();
		str.TrimRight();
		if(str.GetLength() < 3)
			continue;
		nNumRecords++;
	}

    fseek (fp, nStart, SEEK_SET);
	return nNumRecords;
}
*/
bool CWEATHER::ReadPreFile(int delt,int nFileIndex,CString strFileName,COleDateTime twStart,COleDateTime twEnd)
{
	if(pWFILEINFO == NULL || strFileName.GetLength() == 0)
		return false;

	if (pWFILEINFO[nFileIndex].nWParam == 0)	// no weather parameter is defined
		return false;

	FILE *fpin = NULL;
	char strLine[MAXLINE];
	
	// open the file for reading
	fpin = fopen (strFileName, "rt");
	if(fpin == NULL)
	{
		AfxMessageBox("Cannot open file "+strFileName+" for reading");
		return false;
	}

	// skip the comment lines
	if (!SkipCommentLines(fpin))
		return false;

	COleDateTime tDATE(1900,1,1,0,0,0);
	COleDateTimeSpan tspan = twEnd - twStart;

	//find the number of required records
	long lRecord = (long)tspan.GetTotalMinutes()/delt + 1;
	
	//assign memory here
	if (pWFILEINFO[nFileIndex].pWEATHERDATA != NULL)
		delete[]pWFILEINFO[nFileIndex].pWEATHERDATA;

	if (lRecord > 0)
		pWFILEINFO[nFileIndex].pWEATHERDATA = new WEATHERDATA[lRecord];

	//initialize the values
	for (long j=0; j<lRecord; j++)
	{
		//assign memory here
		if (pWFILEINFO[nFileIndex].pWEATHERDATA[j].lfVALUE != NULL)
			delete[]pWFILEINFO[nFileIndex].pWEATHERDATA[j].lfVALUE;

		if (pWFILEINFO[nFileIndex].nWParam > 0)
			pWFILEINFO[nFileIndex].pWEATHERDATA[j].lfVALUE = new double[pWFILEINFO[nFileIndex].nWParam];
		
		for (int k=0; k<pWFILEINFO[nFileIndex].nWParam; k++)
			pWFILEINFO[nFileIndex].pWEATHERDATA[j].lfVALUE[k] = 0.0;
	}

	// read the data
	while (fgets(strLine, MAXLINE, fpin) != NULL)
	{
		CString str(strLine);
		str.TrimLeft();
		str.TrimRight();
		if(str.GetLength() < 3)
			continue;

		CStringToken strToken(str, ",");//comma delimited
		CString strDateTime = strToken.NextToken();

		//read the month
		int nIndex = strDateTime.Find("/");
		int nMonth = atoi(LPCSTR(strDateTime.Left(nIndex)));
		strDateTime = strDateTime.Mid(nIndex+1);
		//read the day
		nIndex = strDateTime.Find("/");
		int nDay = atoi(LPCSTR(strDateTime.Left(nIndex)));
		strDateTime = strDateTime.Mid(nIndex+1);
		//read the year
		nIndex = 4;//year always has four digits
		int nYear = atoi(LPCSTR(strDateTime.Left(nIndex)));
		strDateTime = strDateTime.Mid(nIndex);
		//read the hour and minute
		int nHour = 0;
		int nMinute = 0;
		nIndex = strDateTime.Find(":");
		if(nIndex != -1)
		{
			nHour = atoi(LPCSTR(strDateTime.Left(nIndex)));
			strDateTime = strDateTime.Mid(nIndex+1);
			nMinute = atoi(LPCSTR(strDateTime));
		}
		if(nHour == 24)																	
		{                          
			nHour = 0;                  
			tDATE = COleDateTime(nYear,nMonth,nDay,nHour,nMinute,0) + COleDateTimeSpan(1,0,0,0);
		}
		else
		{
			tDATE = COleDateTime(nYear,nMonth,nDay,nHour,nMinute,0);
		}

		if (tDATE >= twStart && tDATE <= twEnd)
		{
			//find the index
			tspan = tDATE - twStart;
			long lIndex = (long)tspan.GetTotalMinutes()/delt;
			//insert the values
			for (int k=0; k<pWFILEINFO[nFileIndex].nWParam; k++)
				pWFILEINFO[nFileIndex].pWEATHERDATA[lIndex].lfVALUE[k] = atof(LPCSTR(strToken.NextToken()));

			if (lIndex >= lRecord)
				break;
		}
	}

	fclose(fpin);
	return true;
}
/*
bool CWEATHER::ProcessWeatherFiles(int delt,COleDateTime twStart,COleDateTime twEnd)
{
	if(pWSTATIONINFO == NULL || pWFILEINFO == NULL)
		return false;

	for(int i=0; i<nNumWeatherFiles; i++)
	{
		if (pWFILEINFO[i].nWParam == 0 || pWFILEINFO[i].lRecords == 0)
			return false;

		int nWParam = pWFILEINFO[i].nWParam;
		COleDateTime t1 = twStart;
		COleDateTime t2 = twEnd;
		COleDateTimeSpan tspan = t2 - t1;

		//find the number of required records
		long lRecord = (long)tspan.GetTotalMinutes()/delt + 1;
		
		//assign memory here
		if (pWFILEINFO[i].pWEATHERDATA != NULL)
			delete[]pWFILEINFO[i].pWEATHERDATA;
		pWFILEINFO[i].pWEATHERDATA = new WEATHERDATA[lRecord];

		//check the file extension
		CString strFileName = pWFILEINFO[i].strWFile;
		int nIndex = strFileName.ReverseFind('.');
		CString strFileExtension = strFileName.Mid(nIndex, 4);
		if (strFileExtension.CompareNoCase(".pre") == 0) 
		{
			//initialize the values
			for (long j=0; j<lRecord; j++)
			{
				//assign memory here
				if (pWFILEINFO[i].pWEATHERDATA[j].lfVALUE != NULL)
					delete[]pWFILEINFO[i].pWEATHERDATA[j].lfVALUE;
				pWFILEINFO[i].pWEATHERDATA[j].lfVALUE = new double[nWParam];
				
				for (int k=0; k<nWParam; k++)
					pWFILEINFO[i].pWEATHERDATA[j].lfVALUE[k] = 0.0;
			}
			
			//get the value from the input data
			for (j=0; j<pWFILEINFO[i].lRecords; j++)
			{
				//find the intervals between current and next record
				COleDateTime t3 = pWFILEINFO[i].tWEATHERDATA[j].tDATE; 
				
				if (t3 >= t1 && t3 <= t2)
				{
					//find the index
					tspan = t3 - t1;
					long lIndex = (long)tspan.GetTotalMinutes()/delt;
					//insert the values
					for (int k=0; k<nWParam; k++)
						pWFILEINFO[i].pWEATHERDATA[lIndex].lfVALUE[k] = pWFILEINFO[i].tWEATHERDATA[j].lfVALUE[k];
				}
				//release the memory
				if (pWFILEINFO[i].tWEATHERDATA[j].lfVALUE != NULL)
				{
					delete[]pWFILEINFO[i].tWEATHERDATA[j].lfVALUE;
					pWFILEINFO[i].tWEATHERDATA[j].lfVALUE = NULL;
				}
			}
			//update the records
			pWFILEINFO[i].lRecords = lRecord;

			//release the memory
			if (pWFILEINFO[i].tWEATHERDATA != NULL)
			{
				delete[]pWFILEINFO[i].tWEATHERDATA;
				pWFILEINFO[i].tWEATHERDATA = NULL;
			}
		}
		else if (strFileExtension.CompareNoCase(".air") == 0) 
		{
			//find the index
			COleDateTime t3 = pWFILEINFO[i].tWEATHERDATA[0].tDATE; 
			COleDateTime t4 = pWFILEINFO[i].tWEATHERDATA[pWFILEINFO[i].lRecords-1].tDATE; 
			long lIndex = FindDataIndex(delt ,i ,t1, t3, t4);
			if (lIndex == -1)
				return false;

			//get the value from the input data
			for (long j=0; j<pWFILEINFO[i].lRecords; j++)
			{
				if (j >= lIndex && j < lRecord+lIndex)
				{
					//assign memory here
					if (pWFILEINFO[i].pWEATHERDATA[j-lIndex].lfVALUE != NULL)
						delete[]pWFILEINFO[i].pWEATHERDATA[j-lIndex].lfVALUE;
					pWFILEINFO[i].pWEATHERDATA[j-lIndex].lfVALUE = new double[nWParam];
					//insert the values
					for (int k=0; k<nWParam; k++)
						pWFILEINFO[i].pWEATHERDATA[j-lIndex].lfVALUE[k] = pWFILEINFO[i].tWEATHERDATA[j].lfVALUE[k];
				}
				//release the memory
				if (pWFILEINFO[i].tWEATHERDATA[j].lfVALUE != NULL)
				{
					delete[]pWFILEINFO[i].tWEATHERDATA[j].lfVALUE;
					pWFILEINFO[i].tWEATHERDATA[j].lfVALUE = NULL;
				}
			}
			//update the records
			pWFILEINFO[i].lRecords = lRecord;

			//release the memory
			if (pWFILEINFO[i].tWEATHERDATA != NULL)
			{
				delete[]pWFILEINFO[i].tWEATHERDATA;
				pWFILEINFO[i].tWEATHERDATA = NULL;
			}
		}
	}

	return true;
}
*/
bool CWEATHER::ReadAirFile(int delt,int nFileIndex,CString strFileName,COleDateTime twStart,COleDateTime twEnd)
{
	if(pWFILEINFO == NULL || strFileName.GetLength() == 0)
		return false;

	if (pWFILEINFO[nFileIndex].nWParam == 0)	// no weather parameter is defined
		return false;

	FILE *fpin = NULL;
	char strLine[MAXLINE];
	
	// open the file for reading
	fpin = fopen (strFileName, "rt");
	if(fpin == NULL)
	{
		AfxMessageBox("Cannot open file "+strFileName+" for reading");
		return false;
	}

	// skip the comment lines
	if (!SkipCommentLines(fpin))
		return false;

	COleDateTime tDATE(1900,1,1,0,0,0);
	COleDateTimeSpan tspan = twEnd - twStart;

	//find the number of required records
	long lRecord = (long)tspan.GetTotalMinutes()/delt + 1;
	
	//assign memory here
	if (pWFILEINFO[nFileIndex].pWEATHERDATA != NULL)
		delete[]pWFILEINFO[nFileIndex].pWEATHERDATA;

	if (lRecord > 0)
		pWFILEINFO[nFileIndex].pWEATHERDATA = new WEATHERDATA[lRecord];

	// read the data
	long ii = 0;
	while (fgets(strLine, MAXLINE, fpin) != NULL)
	{
		CString str(strLine);
		str.TrimLeft();
		str.TrimRight();
		if(str.GetLength() < 3)
			continue;

		CStringToken strToken(str);
		//skip the first dummy number
		strToken.NextToken();
		//read the date
		int nYear = atoi(LPCSTR(strToken.NextToken()));
		int nMonth = atoi(LPCSTR(strToken.NextToken()));
		int nDay = atoi(LPCSTR(strToken.NextToken()));
		int nHour = atoi(LPCSTR(strToken.NextToken()));
		int nMinute = atoi(LPCSTR(strToken.NextToken()));
		if(nHour == 24)																	
		{                          
			nHour = 0;                  
			tDATE = COleDateTime(nYear,nMonth,nDay,nHour,nMinute,0) + COleDateTimeSpan(1,0,0,0);
		}
		else
		{
			tDATE = COleDateTime(nYear,nMonth,nDay,nHour,nMinute,0);
		}

		if (tDATE >= twStart && tDATE <= twEnd)
		{
			//read the parameter values
			if (pWFILEINFO[nFileIndex].pWEATHERDATA[ii].lfVALUE != NULL)
				delete[]pWFILEINFO[nFileIndex].pWEATHERDATA[ii].lfVALUE;
			if (pWFILEINFO[nFileIndex].nWParam > 0)
				pWFILEINFO[nFileIndex].pWEATHERDATA[ii].lfVALUE = new double[pWFILEINFO[nFileIndex].nWParam];
			
			for (int i=0; i<pWFILEINFO[nFileIndex].nWParam; i++)
				pWFILEINFO[nFileIndex].pWEATHERDATA[ii].lfVALUE[i] = atof(LPCSTR(strToken.NextToken()));
			ii++;
			if (ii >= lRecord)
				break;
		}
	}

	//assign first record value equal to the second record
	for (int i=0; i<pWFILEINFO[nFileIndex].nWParam; i++)
		pWFILEINFO[nFileIndex].pWEATHERDATA[0].lfVALUE[i] = pWFILEINFO[nFileIndex].pWEATHERDATA[1].lfVALUE[i];

	pWFILEINFO[nFileIndex].lRecords = lRecord;

	fclose(fpin);
	return true;
}

bool CWEATHER::GetWeatherDateTimeandInterval(int& delt,COleDateTime& twStart,COleDateTime& twEnd)
{
	if (pWFILEINFO == NULL)
		return false;

	FILE *fpin = NULL;
	char strLine[MAXLINE];
	bool bfirstairfile = true;
	CString strLastLine = "";
	COleDateTime tDATE(1900,1,1,0,0,0);

	//read air files only to get the time interval and common start and end date time
	for(int i=0; i<nNumWeatherFiles; i++) 
	{
		if (pWFILEINFO[i].nWParam == 0)	// no weather parameter is defined
			return false;

		bool bfirstrecord = true;
		bool bsecondrecord = false;
		CString strFileName = strWeatherFolderPath + pWFILEINFO[i].strWFile;
		int nIndex = strFileName.ReverseFind('.');
		CString strFileExtension = strFileName.Mid(nIndex, 4);
		if (strFileExtension.CompareNoCase(".air") == 0) 
		{
			// open the file for reading
			fpin = fopen (strFileName, "rt");
			if(fpin == NULL)
			{
				AfxMessageBox("Cannot open file "+strFileName+" for reading");
				return false;
			}

			// skip the comment lines
			if (!SkipCommentLines(fpin))
				return false;

			// scan the file
			while (fgets(strLine, MAXLINE, fpin) != NULL)
			{
				CString str(strLine);
				str.TrimLeft();
				str.TrimRight();
				if(str.GetLength() < 3)
					continue;

				strLastLine = strLine;
				if (bfirstrecord == true)
				{
					//it is the first record of this file 
					//get the start date of the weather data
					CStringToken strToken(str);
					//skip the first dummy number
					strToken.NextToken();
					//read the date
					int nYear = atoi(LPCSTR(strToken.NextToken()));
					int nMonth = atoi(LPCSTR(strToken.NextToken()));
					int nDay = atoi(LPCSTR(strToken.NextToken()));
					int nHour = atoi(LPCSTR(strToken.NextToken()));
					int nMinute = atoi(LPCSTR(strToken.NextToken()));
					if(nHour == 24)																	
					{                          
						nHour = 0;                  
						tDATE = COleDateTime(nYear,nMonth,nDay,nHour,nMinute,0) + COleDateTimeSpan(1,0,0,0);
					}
					else
					{
						tDATE = COleDateTime(nYear,nMonth,nDay,nHour,nMinute,0);
					}

					if (bfirstairfile == true)
					{
						twStart = tDATE;
					}
					else
					{
						if(tDATE > twStart)
							twStart = tDATE;    
					}

					bfirstrecord = false;
					bsecondrecord = true;
				}
				else if (bsecondrecord == true && bfirstairfile == true)
				{
					//it is the second record of the first file 
					//get the interval of weather data
					CStringToken strToken(str);
					//skip the first dummy number
					strToken.NextToken();
					//read the date
					int nYear = atoi(LPCSTR(strToken.NextToken()));
					int nMonth = atoi(LPCSTR(strToken.NextToken()));
					int nDay = atoi(LPCSTR(strToken.NextToken()));
					int nHour = atoi(LPCSTR(strToken.NextToken()));
					int nMinute = atoi(LPCSTR(strToken.NextToken()));
					if(nHour == 24)																	
					{                          
						nHour = 0;                  
						tDATE = COleDateTime(nYear,nMonth,nDay,nHour,nMinute,0) + COleDateTimeSpan(1,0,0,0);
					}
					else
					{
						tDATE = COleDateTime(nYear,nMonth,nDay,nHour,nMinute,0);
					}

					COleDateTimeSpan deltatimestep;
					deltatimestep = tDATE - twStart;
					delt = int(deltatimestep.GetTotalMinutes()+0.5);
					bsecondrecord = false;
				}
			}

			//it is the last record of this file 
			//get the end date of the weather data
			CString str(strLastLine);
			str.TrimLeft();
			str.TrimRight();
			CStringToken strToken(str);
			//skip the first dummy number
			strToken.NextToken();
			//read the date
			int nYear = atoi(LPCSTR(strToken.NextToken()));
			int nMonth = atoi(LPCSTR(strToken.NextToken()));
			int nDay = atoi(LPCSTR(strToken.NextToken()));
			int nHour = atoi(LPCSTR(strToken.NextToken()));
			int nMinute = atoi(LPCSTR(strToken.NextToken()));
			if(nHour == 24)																	
			{                          
				nHour = 0;                  
				tDATE = COleDateTime(nYear,nMonth,nDay,nHour,nMinute,0) + COleDateTimeSpan(1,0,0,0);
			}
			else
			{
				tDATE = COleDateTime(nYear,nMonth,nDay,nHour,nMinute,0);
			}

			if (bfirstairfile == true)
			{
				twEnd = tDATE;
				bfirstairfile = false;
			}
			else
			{
				if(tDATE < twEnd)
					twEnd = tDATE;    
			}

			//close the file
			fclose(fpin);
		}
	}
	return true;
}
/*
bool CWEATHER::GetWeatherDateTime(COleDateTime& twStart,COleDateTime& twEnd)
{
	if (pWFILEINFO == NULL)
		return false;

	int nFirstEntry = 0;

	for(int i=0; i<nNumWeatherFiles; i++)
	{
		if(pWFILEINFO[i].tWEATHERDATA != NULL && pWFILEINFO[i].lRecords > 0 && pWFILEINFO[i].strWFile.Right(3).CompareNoCase("air") == 0)
		{
			if (nFirstEntry == 0)	// initialize
			{
				twStart = pWFILEINFO[i].tWEATHERDATA[0].tDATE;
				twEnd = pWFILEINFO[i].tWEATHERDATA[pWFILEINFO[i].lRecords-1].tDATE;
				nFirstEntry = 1;
			}
			else
			{
				if(pWFILEINFO[i].tWEATHERDATA[0].tDATE > twStart)
					twStart = pWFILEINFO[i].tWEATHERDATA[0].tDATE;    
				
				if(pWFILEINFO[i].tWEATHERDATA[pWFILEINFO[i].lRecords-1].tDATE < twEnd)
					twEnd = pWFILEINFO[i].tWEATHERDATA[pWFILEINFO[i].lRecords-1].tDATE;    
			}
		}
	}

	return true;
}
*/
int CWEATHER::FindFileIndex(int nFileID)
{
	if(pWFILEINFO == NULL)
		return -1;

	for(int i=0; i<nNumWeatherFiles; i++)
	{
		if(nFileID == pWFILEINFO[i].nWFileID)
			return i;
	}

	//can not find the data
	return -1;	
}

int CWEATHER::FindStationIndex(int nStationID)
{
	if(pWSTATIONINFO == NULL)
		return -1;

	for(int i=0; i<nNumWeatherStations; i++)
	{
		if(nStationID == pWSTATIONINFO[i].nWStationID)
			return i;
	}

	//can not find the data
	return -1;	
}

long CWEATHER::FindDataIndex(int delt,int nFileIndex,COleDateTime tCurrent,COleDateTime tStart,COleDateTime tEnd)
{
	if(pWFILEINFO == NULL)
		return -1;

	//find the intervals between current and next record
	COleDateTime t1 = tStart;
	COleDateTime t2 = tEnd;
	COleDateTime t3 = tCurrent; 
	
	if (t3 >= t1 && t3 <= t2)
	{
		//find the index
		COleDateTimeSpan tspan = t3 - t1;
		pWFILEINFO[nFileIndex].lIndex = (long)tspan.GetTotalMinutes()/delt;
		return pWFILEINFO[nFileIndex].lIndex;
	}

/*	long nRecord = pWFILEINFO[nFileIndex].lRecords;

	for(long i=0; i<nRecord; i++)
	{
//		if(tCurrent.GetYear() == pWFILEINFO[nFileIndex].pWEATHERDATA[i].tDATE.GetYear() &&
//		   tCurrent.GetMonth() == pWFILEINFO[nFileIndex].pWEATHERDATA[i].tDATE.GetMonth() &&
//		   tCurrent.GetDay() == pWFILEINFO[nFileIndex].pWEATHERDATA[i].tDATE.GetDay() &&
//		   tCurrent.GetHour() == pWFILEINFO[nFileIndex].pWEATHERDATA[i].tDATE.GetHour() &&
//		   tCurrent.GetMinute() == pWFILEINFO[nFileIndex].pWEATHERDATA[i].tDATE.GetMinute())

		if(tCurrent <= pWFILEINFO[nFileIndex].pWEATHERDATA[i].tDATE)
		{
			pWFILEINFO[nFileIndex].lIndex = i;
			return i;
		}
	}
*/
	//can not find the data
	return -1;	
}

bool CWEATHER::WeatherDataMapping()
{
	if(pWFILEINFO == NULL || pWSTATIONINFO == NULL)
		return false;

	for(int i=0; i<nNumWeatherStations; i++)
	{
		for(int j=0; j<pWSTATIONINFO[i].nWFile; j++)
		{
			//get the file index
			int ii = pWSTATIONINFO[i].m_nWFileIndex[j];
			for(int k=0; k<pWFILEINFO[ii].nWParam; k++)
			{
				switch(pWFILEINFO[ii].m_nParamID[k])
				{
				case W_PREC:
					{
						pWSTATIONINFO[i].nWDATA[0].nFileIndex = ii;
						pWSTATIONINFO[i].nWDATA[0].nParmIndex = k;
					}
					break;
				case W_PET:
					{
						pWSTATIONINFO[i].nWDATA[1].nFileIndex = ii;
						pWSTATIONINFO[i].nWDATA[1].nParmIndex = k;
					}
					break;
				case W_TEMP:
					{
						pWSTATIONINFO[i].nWDATA[2].nFileIndex = ii;
						pWSTATIONINFO[i].nWDATA[2].nParmIndex = k;
					}
					break;
				case W_WIND:
					{
						pWSTATIONINFO[i].nWDATA[3].nFileIndex = ii;
						pWSTATIONINFO[i].nWDATA[3].nParmIndex = k;
					}
					break;
				case W_SRAD:
					{
						pWSTATIONINFO[i].nWDATA[4].nFileIndex = ii;
						pWSTATIONINFO[i].nWDATA[4].nParmIndex = k;
					}
					break;
				case W_DEWP:
					{
						pWSTATIONINFO[i].nWDATA[5].nFileIndex = ii;
						pWSTATIONINFO[i].nWDATA[5].nParmIndex = k;
					}
					break;
				case W_CLOU:
					{
						pWSTATIONINFO[i].nWDATA[6].nFileIndex = ii;
						pWSTATIONINFO[i].nWDATA[6].nParmIndex = k;
					}
					break;
				default:
					break;
				}
			}
		}
	}

	return true;
}
/*
double CWEATHER::GetParamValue(int delt,int nParamID,int nStationIndex,COleDateTime tCurrent)
{
	double lfValue = 0.0;

	if(pWFILEINFO == NULL || pWSTATIONINFO == NULL)
		return -1;

	//find the weather file index that has the parameter id
	int nFileIndex = pWSTATIONINFO[nStationIndex].nWDATA[nParamID-1].nFileIndex;
	int nParamIndex = pWSTATIONINFO[nStationIndex].nWDATA[nParamID-1].nParmIndex;

	if (nFileIndex == -1 || nParamIndex == -1)
	{
		AfxMessageBox("Unable to find the data for weather parameter ID: %d",nParamID);
		return -1;
	}

	double lfMultiplier = pWSTATIONINFO[nStationIndex].m_lfMultiplier[nParamID-1];

	//find the record index in the weather file
	long lStart = pWFILEINFO[nFileIndex].lIndex;
	long lIndex = FindDataIndex(delt,nFileIndex,tCurrent);

	if(lIndex >= 0)
	{
		if(lIndex == lStart)
		{
			lfValue = pWFILEINFO[nFileIndex].pWEATHERDATA[lIndex].lfVALUE[nParamIndex];
		}
		else
		{
			COleDateTimeSpan t1 = tCurrent - pWFILEINFO[nFileIndex].pWEATHERDATA[lIndex-1].tDATE;    
			COleDateTimeSpan t2 = pWFILEINFO[nFileIndex].pWEATHERDATA[lIndex].tDATE - tCurrent; 
			double d1 = t1.GetTotalHours();        
			double d2 = t2.GetTotalHours();      
			if(d1 == 0 || d2 == 0)
			{
				if(d1 == 0)
				{
					lfValue = pWFILEINFO[nFileIndex].pWEATHERDATA[lIndex-1].lfVALUE[nParamIndex];
				}
				else
				{
					lfValue = pWFILEINFO[nFileIndex].pWEATHERDATA[lIndex].lfVALUE[nParamIndex];
				}
			}
			else
			{
				lfValue  = (d2*pWFILEINFO[nFileIndex].pWEATHERDATA[lIndex-1].lfVALUE[nParamIndex]
							+d1*pWFILEINFO[nFileIndex].pWEATHERDATA[lIndex].lfVALUE[nParamIndex])
							/(d1+d2);
			}
		}

		return lfValue*lfMultiplier;
	}

	return -1;
}

bool CWEATHER::InterpolateWeatherData(int delt)
{
	if(pWSTATIONINFO == NULL || pWFILEINFO == NULL)
		return false;

	for(int i=0; i<nNumWeatherFiles; i++)
	{
		COleDateTime t1 = pWFILEINFO[i].pWEATHERDATA[0].tDATE;
		COleDateTime t2 = pWFILEINFO[i].pWEATHERDATA[pWFILEINFO[i].lRecords-1].tDATE;
		COleDateTimeSpan tspan = t2 - t1;
		
		//find the number of required records
		long lRecord = (long)tspan.GetTotalMinutes()/delt + 1;

		//assign memory here
		if (pWFILEINFO[i].iWEATHERDATA != NULL)
			delete pWFILEINFO[i].iWEATHERDATA;
		pWFILEINFO[i].iWEATHERDATA = new WEATHERDATA[lRecord];

		//get the value from the input data and interpolate if necessary
		long lIndex = 0;
		int nWParam = pWFILEINFO[i].nWParam;

		for (long j=0; j<pWFILEINFO[i].lRecords; j++)
		{
			//find the intervals between current and next record
			t1 = pWFILEINFO[i].pWEATHERDATA[j].tDATE;    
			t2 = pWFILEINFO[i].pWEATHERDATA[j+1].tDATE; 
			tspan = t2 - t1;
			int nInterval = (int)tspan.GetTotalMinutes()/delt;
			
			if (nInterval == 0)
			{
				//error message (two records have same date)
			}

			for (int k=0; k<nInterval; k++)
			{
				//assign memory here
				pWFILEINFO[i].iWEATHERDATA[lIndex].lfVALUE = new double[nWParam];
				
				pWFILEINFO[i].iWEATHERDATA[lIndex].tDATE = t1 + COleDateTimeSpan(0,0,k*delt,0);
				//interpolate the value
				for (int l=0; l<nWParam; l++)
				{
					double lfValue  = pWFILEINFO[i].pWEATHERDATA[j].lfVALUE[l] + k * (pWFILEINFO[i].pWEATHERDATA[j+1].lfVALUE[l] - pWFILEINFO[i].pWEATHERDATA[j].lfVALUE[l])/nInterval;
				}
			}
		}
	}

	return true;	
}
*/
