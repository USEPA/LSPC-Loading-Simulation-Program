// PageOutput.cpp : implementation file
//

#include "stdafx.h"
#include "lspc.h"
#include "PageOutput.h"
#include "MainFrm.h"
#include "LSPCView.h"
#include "LSPCDoc.h"
#include "LSPCMODEL.h"
#include "DirDialog.h"
#include "RecordsetEx.h"	 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString strParameters[]=
{
	"Prec             PREC        Precipitation (rain + snow) volume (in-acre)",
	"Snow             AIRTMP      Air temperature (F)",
	"Snow             SNOTMP      Temperature for snow fall (F)",
	"Snow             SNOWF       Precipitation (snow) volume (in-acre)",
	"Snow             RAINF       Precipitation (rain) volume (in-acre)",
	"Snow             PRAIN       Rainfall entering snow pack (in-acre)",
	"Snow             MELT        Snow melt from the snow pack (in-acre)",
	"Snow             SNOWE       Evaporation from pack (in-acre)",
	"Snow             WYIELD      Water yield from pack (in-acre)",
	"Snow             PACK        Total contents of pack (water equiv) (in-acre)",
	"Snow             PACKF       Frozen contents of pack, ie. snow + ice (water equiv) (in-acre)",
	"Snow             PACKW       Liquid water in pack (in-acre)",
	"Snow             PACKI       Ice in pack (water equiv) (in-acre)",
	"Snow             PDEPTH      Pack depth (in-acre)",
	"Snow             COVINDX     Snow cover index (in-acre)",
	"Snow             NEGHTS      Negative heat storage (water equiv) (in-acre)",
	"Snow             XLNMELT     Maximum increment to ice in pack (in-acre)",
	"Snow             RDENPKF     Relative density of frozen contents of pack (packf/pdepth)",
	"Snow             SKYCLEAR    Fraction of sky assumed clear",
	"Snow             SNOCOV      Fraction of land-segment covered by pack",
	"Snow             DULLNESS    Dullness index of the pack",
	"Snow             ALBEDO      Albedo of the pack",
	"Snow             PAKTEMP     Mean temperature of the pack (F)",
	"Snow             DEWTMP      Effective dew point temperature (F)",
	"Hydrology        SURS        Surface (overland flow) storage volume (in-acre)",
	"Hydrology        UZS         Upper zone storage volume (in-acre)",
	"Hydrology        LZS         Lower zone storage volume (in-acre)",
	"Hydrology        AGWS        Active groundwater storage volume (in-acre)",
	"Hydrology        SURO        Surface outflow volume (in-acre)",
	"Hydrology        IFWO        Interflow outflow volume (in-acre)",
	"Hydrology        AGWO        Active groundwater outflow volume (in-acre)",
	"Hydrology        PERO        Total outflow from PLS volume (in-acre)",
	"Hydrology        TAET        Total simulated E-T volume (in-acre)",
	"Hydrology        PERC        Percolation volume from upper to lower zone (in-acre)",
	"Hydrology        INFIL       Infiltration volume from the surface to lower zone (in-acre)",
	"Hydrology        GWI         Ground water inflow volume from the upper and lower zones (in-acre)",
	"Hydrology        IGWI        Deep percolation volume (in-acre)",
	"Hydrology        AGWI        Active ground water inflow volume (in-acre)",
	"Hydraulics       DEP         Stream depth (ft)",
	"Hydraulics       AVDEP       Stream average depth (ft)",
	"Hydraulics       HRAD        Hydraulic radius (ft)",
	"Hydraulics       AVVEL       Average velocity (ft/s)",
	"Hydraulics       SAREA       Average surface area (acres)",
	"Hydraulics       VOLUME      Total volume in RCHRES (ft3)",
	"Hydraulics       RO          Total rate of outflow from RCHRES (cfs)",
	"Sediment         TAU         Bed shear stress (kg/m^2)",
	"Sediment         WSSD        Washoff of detached sediment (tons)",
	"Sediment         SCRSD       Scour of matrix (attached) soil (tons)",
	"Sediment         SOSED       Total removal of soil and sediment (tons)",
	"Sediment         SOBER       Sediment removal of soil from stream bank erosion (tons)",
 	"Sediment         SSED        Suspended sediment concentrations in the RCHRES (mg/l)",
	"Sediment         SSEDOUT     Suspended sediment concentrations in outflow from the RCHRES (mg/l)",
	"Sediment         RSED        Sediment bed storage (tonne)",
	"Sediment         BEDDEP      Sediment bed depth (ft)",
	"Sediment         DEPSCR      Net sediment deposition (positive) or scour (negative) (tonne)",
	"Sediment         ROSED       Total outflows of sediment from the RCHRES (tonne)",
	"Water_Quality    SQO         Storage of QUALOF on the surface (lb)",
	"Water_Quality    WASHQS      Removal of QUALSD by association with detached sediment washoff",
	"Water_Quality    SCRQS       Removal of QUALSD by association with scour of matrix soil",
	"Water_Quality    SOQO        Washoff of QUALOF from surface",
	"Water_Quality    POQUAL      Total flux of QUAL from the land",
	"Water_Quality    SOQUAL      Surface flux of QUAL from the land",
	"Water_Quality    IOQUAL      Interflow flux of QUAL from the land",
	"Water_Quality    GOQUAL      Groundflow flux of QUAL from the land",
	"Water_Quality    POQC        Concentration of QUAL (total) in total outflow from the land",
	"Water_Quality    CONC        Pollutant average concentration in RCHRES",
	"Water_Quality    CONCOUT     Pollutant total (dissolved + sediment-associated) concentration in outflow from the RCHRES",
	"Water_Quality    CONCSQAL    Concentration of total suspended sediment-associated qual in RCHRES",
	"Water_Quality    MATSQAL     Storage of sediment-associated qual (lb)",
	"Water_Quality    MATIN       Pollutant mass in inflow to RCHRES",
	"Water_Quality    MATOUT      Pollutant mass in outflow from RCHRES",
	"Water_Quality    MATOSQAL    Total outflow of sediment-associated qual from RCHRES",
	"Water_Quality    DOX         Dissolved oxygen concentration in RCHRES",
	"Water_Quality    DOXMIN      Dissolved oxygen instream minimum concentration",
	"Water_Quality    DOXMAX      Dissolved oxygen instream maximum concentration",
	"Water_Quality    DOXAV       Dissolved oxygen instream average concentration",
	"Water_Quality    DOXX        Dissolved oxygen material in outflow from RCHRES",
	"Water_Quality    BOD         Biochemical oxygen demand concentration in RCHRES",
	"Water_Quality    BODX        Biochemical oxygen demand material in outflow from RCHRES",
	"Water_Quality    NO3         Nitrate concentration in RCHRES",
	"Water_Quality    NO3X        Nitrate material in outflow from RCHRES",
	"Water_Quality    TAM         Total dissolved ammonia concentration in RCHRES",
	"Water_Quality    TAMX        Total ammonia material in outflow from RCHRES",
	"Water_Quality    NO2         Nitrite concentration in RCHRES",
	"Water_Quality    NO2X        Nitrite material in outflow from RCHRES",
	"Water_Quality    PO4         Ortho-phosphorus concentration in RCHRES",
	"Water_Quality    PO4X        Ortho-phosphorus material in outflow from RCHRES",
	"Water_Quality    CONCSNH4    Particulate NH4-N total (sand+silt+clay) concentration in RCHRES (mg/l)",
	"Water_Quality    SNH4X       Outflows of particulate NH4 from RCHRES (kg)",
	"Water_Quality    CONCSPO4    Particulate PO4-P total (sand+silt+clay) concentration in RCHRES (mg/l)",
	"Water_Quality    SPO4X       Outflows of particulate PO4-P from RCHRES (kg)",
	"Water_Quality    PHYTO       Phytoplankton concentration in RCHRES",
	"Water_Quality    PHYTOX      Phytoplankton material in outflow from RCHRES",
	"Water_Quality    PHYCLA      Phytoplankton as chlorophyll a",
	"Water_Quality    BENAL       Benthic algae density, as biomass",
	"Water_Quality    ORN         Organic nitrogen concentration in RCHRES",
	"Water_Quality    ORNX        Organic nitrogen material in outflow from RCHRES",
	"Water_Quality    ORP         Organic phosphorus concentration in RCHRES",
	"Water_Quality    ORPX        Organic phosphorus material in outflow from RCHRES",
	"Water_Quality    ORC         Organic carbon concentration in RCHRES",
	"Water_Quality    ORCX        Organic carbon material in outflow from RCHRES",
	"Water_Quality    PH          pH in RCHRES",
	"Water_Quality    ALK         Alkalinity in RCHRES",
	"Water_Quality    TIC         Total inorganic carbon concentration in RCHRES",
	"Water_Quality    TICX        Total inorganic carbon material in outflow from RCHRES",
	"Water_Quality    CO2         Carbon dioxide (as carbon) concentration in RCHRES",
	"Water_Quality    CO2X        Carbon dioxide (as carbon) material in outflow from RCHRES",
	"Water_Quality    TEMP        Stream temperature (degree C)"
};

/////////////////////////////////////////////////////////////////////////////
// CPageOutput property page

IMPLEMENT_DYNCREATE(CPageOutput, CPropertyPage)

CPageOutput::CPageOutput() : CPropertyPage(CPageOutput::IDD)
{
	//{{AFX_DATA_INIT(CPageOutput)
	m_strLocation = _T("");
	m_bCustom = FALSE;
	m_bHydro = FALSE;
	m_bLanduse = FALSE;
	m_bSediment = FALSE;
	m_bWQ = FALSE;
	m_bDefault = TRUE;
	m_bSnow = FALSE;
	m_bWQR = FALSE;
	m_bStream = FALSE;
	m_bPointSource = FALSE;
	m_bThreshold = FALSE;
	m_bSUSTAIN = FALSE;
	//}}AFX_DATA_INIT
}

CPageOutput::~CPageOutput()
{
}

void CPageOutput::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageOutput)
	DDX_Control(pDX, IDC_COMBOLANDUSE, m_clanduse);
	DDX_Control(pDX, IDC_CUSTOMLIST, m_cPList);
	DDX_Text(pDX, IDC_RESULTLOC, m_strLocation);
	DDX_Check(pDX, IDC_CUSTOM, m_bCustom);
	DDX_Check(pDX, IDC_HYDRO, m_bHydro);
	DDX_Check(pDX, IDC_LANDUSE, m_bLanduse);
	DDX_Check(pDX, IDC_SEDI, m_bSediment);
	DDX_Check(pDX, IDC_WQ, m_bWQ);
	DDX_Check(pDX, IDC_DEFAULTCHK, m_bDefault);
	DDX_Check(pDX, IDC_SNOW, m_bSnow);
	DDX_Check(pDX, IDC_WQR, m_bWQR);
	DDX_Check(pDX, IDC_STREAM, m_bStream);
	DDX_Check(pDX, IDC_PointSource, m_bPointSource);
	DDX_Check(pDX, IDC_Threshold, m_bThreshold);
	DDX_Check(pDX, IDC_SUSTAIN, m_bSUSTAIN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageOutput, CPropertyPage)
	//{{AFX_MSG_MAP(CPageOutput)
	ON_BN_CLICKED(IDC_BOUT, OnBout)
	ON_BN_CLICKED(IDC_HYDRO, OnSeperate6)
	ON_BN_CLICKED(IDC_CUSTOM, OnCustom)
	ON_LBN_SELCHANGE(IDC_CUSTOMLIST, OnSelchangeCustomlist)
	ON_BN_CLICKED(IDC_DEFAULTCHK, OnDefaultchk)
	ON_BN_CLICKED(IDC_LANDUSE, OnLanduse)
	ON_BN_CLICKED(IDC_STREAM, OnStream)
	ON_BN_CLICKED(IDC_PointSource, OnPointSource)
	ON_BN_CLICKED(IDC_Threshold, OnThreshold)
	ON_BN_CLICKED(IDC_SUSTAIN, OnSUSTAIN)
	ON_BN_CLICKED(IDC_SEDI, OnSeperate6)
	ON_BN_CLICKED(IDC_WQ, OnSeperate6)
	ON_BN_CLICKED(IDC_SNOW, OnSeperate6)
	ON_BN_CLICKED(IDC_WQR, OnSeperate6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageOutput message handlers

void CPageOutput::OnBout() 
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return;

	CPropertyPage::UpdateData(true);	

	CDirDialog dlg;
	dlg.m_strSelDir = m_strLocation;
	dlg.m_strTitle = "Model Results Directory:";
	if(dlg.DoBrowse() == 1)
	{
		m_strLocation = dlg.m_strPath;
		pDoc->lspc.m_strModelResultLocation = m_strLocation;
	}	

	CPropertyPage::UpdateData(false);	
}

BOOL CPageOutput::UpdateData(BOOL bSaveAndValidate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;

	if(bSaveAndValidate)
	{
		CPropertyPage::UpdateData(bSaveAndValidate);

		if(!(m_bDefault || m_bHydro || m_bSediment || m_bWQ || m_bWQR || m_bSnow || m_bCustom || m_bLanduse || m_bStream || m_bPointSource || m_bThreshold || m_bSUSTAIN))
		{
			AfxMessageBox("Please specify output mode or parameters before running the model");
			return false;
		}

		if(m_strLocation.GetAt(m_strLocation.GetLength()-1) != '\\'
			&& m_strLocation.GetAt(m_strLocation.GetLength()-1) != '/')
			m_strLocation += "\\";

		pDoc->lspc.m_strModelResultLocation = m_strLocation;
		pDoc->lspc.nDefault = (m_bDefault?1:0);
		pDoc->lspc.nHydro = (m_bHydro?1:0);
		pDoc->lspc.nSed = (m_bSediment?1:0);
		pDoc->lspc.nGQUAL = (m_bWQ?1:0);
		pDoc->lspc.nRQUAL = (m_bWQR?1:0);
		pDoc->lspc.nSnow = (m_bSnow?1:0);		
		pDoc->lspc.nCustom = (m_bCustom?1:0);
		pDoc->lspc.nLUsummary = (m_bLanduse?1:0);
		pDoc->lspc.nCHsummary = (m_bStream?1:0);
		pDoc->lspc.nPSsummary = (m_bPointSource?1:0);
		pDoc->lspc.nThreshold = (m_bThreshold?1:0);
		pDoc->lspc.nSUSTAIN = (m_bSUSTAIN?1:0);
		pDoc->lspc.summarylevel = m_clanduse.GetCurSel();	
		if (pDoc->lspc.nLUsummary > 0)
			pDoc->lspc.nLUsummary += pDoc->lspc.summarylevel;	
		if (pDoc->lspc.nCHsummary > 0)
			pDoc->lspc.nCHsummary += pDoc->lspc.summarylevel;	
		if (pDoc->lspc.nPSsummary > 0)
			pDoc->lspc.nPSsummary += pDoc->lspc.summarylevel;	

		// save back to database
		CLSPCApp *pApp = (CLSPCApp *) AfxGetApp();
		if(pApp == NULL)
			return false;

		CString str = pApp->strDBFileName;
		str.MakeLower();
		str = str.Right(4);
		if(str.CompareNoCase(".mdb") == 0)
		{
			CString strSQL;
			int flags[12];

			CDatabase db;
			BOOL bCanOpen = FALSE;
			try
			{
				strSQL = CLSPCApp::GetMDBDriver();
				if(strSQL.IsEmpty())
				{
					AfxMessageBox("Cannot find the Microsoft Access ODBC driver.");
					return false;
				}
				bCanOpen = db.OpenEx(strSQL,CDatabase::noOdbcDialog);
			}
			catch(CException e)
			{
				AfxMessageBox("ODBC Error ---Cannot open database for reading.");
				return false;
			}
			if(!bCanOpen)
			{
				AfxMessageBox("ODBC Error ---Cannot open database for reading.");
				return false;
			}
			CRecordsetEx rs(&db);
			
			// card45
			flags[0] = (m_bDefault)?1:0;
			flags[1] = (m_bSnow)?1:0;
			flags[2] = (m_bHydro)?1:0;
			flags[3] = (m_bSediment)?1:0;
			flags[4] = (m_bWQ)?1:0;
			flags[5] = (m_bWQR)?1:0;
			flags[6] = (m_bCustom)?1:0;
			flags[7] = (m_bLanduse)?1:0;
			flags[8] = (m_bStream)?1:0;
			flags[9] = (m_bPointSource)?1:0;
			flags[10] = (m_bThreshold)?1:0;
			flags[11] = (m_bSUSTAIN)?1:0;

			for(int i=0; i<12; i++)
			{
				strSQL.Format("UPDATE ModelFlags SET VALUE1=%d WHERE PARMID=%d",flags[i],i+4501);
				db.ExecuteSQL(LPCSTR(strSQL));
			}
			rs.ExecDirect(strSQL);
			rs.Close();

			// card46
			int nCount = m_cPList.GetCount();
			for(i=0; i<nCount; i++)
			{
				pDoc->lspc.cOP.blistOutput[i] = (m_cPList.GetSel(i) > 0);
				strSQL.Format("UPDATE ModelFlags SET VALUE1=%d WHERE PARMID=%d",(pDoc->lspc.cOP.blistOutput[i])?1:0,i+4601);
				db.ExecuteSQL(LPCSTR(strSQL));
			}
			rs.ExecDirect(strSQL);	 
			rs.Close();
			db.Close();
		}

		if(m_bCustom)
		{
			int nCount = m_cPList.GetCount();
			for(int i=0; i<nCount; i++)
				pDoc->lspc.cOP.blistOutput[i] = (m_cPList.GetSel(i) > 0);
		}
		else
		{
			//update the parameter list here
			//first, remove all the existing parameters from the list
			for(int i=0; i<PARA_NUM; i++)
				pDoc->lspc.cOP.blistOutput[i] = false;

			// add the new ones
			if(m_bDefault)
			{
				pDoc->lspc.cOP.blistOutput[PRECP] = true;			
				pDoc->lspc.cOP.blistOutput[SURO] = true;
				pDoc->lspc.cOP.blistOutput[IFWO] = true;
				pDoc->lspc.cOP.blistOutput[AGWO] = true;
				pDoc->lspc.cOP.blistOutput[PERO] = true;
				pDoc->lspc.cOP.blistOutput[DEP] = true;
				pDoc->lspc.cOP.blistOutput[AVDEP] = true;						
				pDoc->lspc.cOP.blistOutput[TAU] = true;
				pDoc->lspc.cOP.blistOutput[RO] = true;
				pDoc->lspc.cOP.blistOutput[SOSED] = true;
				pDoc->lspc.cOP.blistOutput[SOBER] = true;
				pDoc->lspc.cOP.blistOutput[LSSED] = true;
				pDoc->lspc.cOP.blistOutput[POQUAL] = true;
				pDoc->lspc.cOP.blistOutput[CONC] = true;
				pDoc->lspc.cOP.blistOutput[DOXC] = true;
				pDoc->lspc.cOP.blistOutput[DOXCMIN] = true;
				pDoc->lspc.cOP.blistOutput[DOXCMAX] = true;
				pDoc->lspc.cOP.blistOutput[DOXCAV] = true;
				pDoc->lspc.cOP.blistOutput[BODC] = true;
				pDoc->lspc.cOP.blistOutput[NO3C] = true;
				pDoc->lspc.cOP.blistOutput[TAMC] = true;
				pDoc->lspc.cOP.blistOutput[NO2C] = true;
				pDoc->lspc.cOP.blistOutput[PO4C] = true;
				pDoc->lspc.cOP.blistOutput[SNH4C] = true;
				pDoc->lspc.cOP.blistOutput[SPO4C] = true;
				pDoc->lspc.cOP.blistOutput[PHYC] = true;
				pDoc->lspc.cOP.blistOutput[PHYCLAC] = true;
				pDoc->lspc.cOP.blistOutput[BENALC] = true;
				pDoc->lspc.cOP.blistOutput[ORNC] = true;
				pDoc->lspc.cOP.blistOutput[ORPC] = true;
				pDoc->lspc.cOP.blistOutput[ORCC] = true;
				pDoc->lspc.cOP.blistOutput[LPH] = true;
				pDoc->lspc.cOP.blistOutput[LALK] = true;
				pDoc->lspc.cOP.blistOutput[TICC] = true;
				pDoc->lspc.cOP.blistOutput[CO2C] = true;
				pDoc->lspc.cOP.blistOutput[TEMPC] = true;
			}

			if(m_bSnow)
			{
				pDoc->lspc.cOP.blistOutput[PRECP] = true;			
				pDoc->lspc.cOP.blistOutput[AIRTMP] = true;			
				pDoc->lspc.cOP.blistOutput[SNOTMP] = true;			
				pDoc->lspc.cOP.blistOutput[SNOWF] = true;			
				pDoc->lspc.cOP.blistOutput[RAINF] = true;			
				pDoc->lspc.cOP.blistOutput[PRAIN] = true;			
				pDoc->lspc.cOP.blistOutput[MELT] = true;			
				pDoc->lspc.cOP.blistOutput[SNOWE] = true;			
				pDoc->lspc.cOP.blistOutput[WYIELD] = true;			
				pDoc->lspc.cOP.blistOutput[PACK] = true;			
				pDoc->lspc.cOP.blistOutput[PACKF] = true;			
				pDoc->lspc.cOP.blistOutput[PACKW] = true;			
				pDoc->lspc.cOP.blistOutput[PACKI] = true;			
				pDoc->lspc.cOP.blistOutput[PDEPTH] = true;			
				pDoc->lspc.cOP.blistOutput[COVINDX] = true;			
				pDoc->lspc.cOP.blistOutput[NEGHTS] = true;			
				pDoc->lspc.cOP.blistOutput[XLNMELT] = true;			
				pDoc->lspc.cOP.blistOutput[RDENPKF] = true;			
				pDoc->lspc.cOP.blistOutput[SKYCLEAR] = true;		
				pDoc->lspc.cOP.blistOutput[SNOCOV] = true;			
				pDoc->lspc.cOP.blistOutput[DULLNESS] = true;		
				pDoc->lspc.cOP.blistOutput[ALBEDO] = true;			
				pDoc->lspc.cOP.blistOutput[PAKTEMP] = true;			
				pDoc->lspc.cOP.blistOutput[DEWTMP] = true;			
			}

			if(m_bHydro)
			{
				pDoc->lspc.cOP.blistOutput[PRECP] = true;		
				pDoc->lspc.cOP.blistOutput[SURS] = true;
				pDoc->lspc.cOP.blistOutput[UZS] = true;
				pDoc->lspc.cOP.blistOutput[LZS] = true;
				pDoc->lspc.cOP.blistOutput[AGWS] = true;
				pDoc->lspc.cOP.blistOutput[SURO] = true;
				pDoc->lspc.cOP.blistOutput[IFWO] = true;
				pDoc->lspc.cOP.blistOutput[AGWO] = true;
				pDoc->lspc.cOP.blistOutput[PERO] = true;
				pDoc->lspc.cOP.blistOutput[TAET] = true;
				pDoc->lspc.cOP.blistOutput[PERC] = true;
				pDoc->lspc.cOP.blistOutput[INFIL] = true;					
				pDoc->lspc.cOP.blistOutput[GWI]   = true;					
				pDoc->lspc.cOP.blistOutput[IGWI]  = true;					
				pDoc->lspc.cOP.blistOutput[AGWI]  = true;					
				pDoc->lspc.cOP.blistOutput[DEP] = true;
				pDoc->lspc.cOP.blistOutput[AVDEP] = true;					
				pDoc->lspc.cOP.blistOutput[HRAD] = true;					
				pDoc->lspc.cOP.blistOutput[AVVEL] = true;					
				pDoc->lspc.cOP.blistOutput[SAREA] = true;					
				pDoc->lspc.cOP.blistOutput[VOLUME] = true;
				pDoc->lspc.cOP.blistOutput[RO] = true;
			}

			if(m_bSediment)
			{
				pDoc->lspc.cOP.blistOutput[TAU] = true;
				pDoc->lspc.cOP.blistOutput[WSSD] = true;
				pDoc->lspc.cOP.blistOutput[SCRSD] = true;
				pDoc->lspc.cOP.blistOutput[SOSED] = true;
				pDoc->lspc.cOP.blistOutput[SOBER] = true;
				pDoc->lspc.cOP.blistOutput[SSEDC] = true;							
				pDoc->lspc.cOP.blistOutput[LSSED] = true;
				pDoc->lspc.cOP.blistOutput[LRSED] = true;							
				pDoc->lspc.cOP.blistOutput[LBEDDEP] = true;							
				pDoc->lspc.cOP.blistOutput[LDEPSCR] = true;
				pDoc->lspc.cOP.blistOutput[LROSED] = true;
			}
			if(m_bWQ)
			{
				pDoc->lspc.cOP.blistOutput[SQO] = true;
				pDoc->lspc.cOP.blistOutput[WASHQS] = true;
				pDoc->lspc.cOP.blistOutput[SCRQS] = true;
				pDoc->lspc.cOP.blistOutput[SOQO] = true;
				pDoc->lspc.cOP.blistOutput[POQUAL] = true;
				pDoc->lspc.cOP.blistOutput[SOQUAL] = true;							
				pDoc->lspc.cOP.blistOutput[IOQUAL] = true;							
				pDoc->lspc.cOP.blistOutput[GOQUAL] = true;							
				pDoc->lspc.cOP.blistOutput[POQC] = true;
				pDoc->lspc.cOP.blistOutput[CONC] = true;
				pDoc->lspc.cOP.blistOutput[CONCOUT] = true;
				pDoc->lspc.cOP.blistOutput[CONCSQAL] = true;
				pDoc->lspc.cOP.blistOutput[MATSQAL] = true;
				pDoc->lspc.cOP.blistOutput[MATIN] = true;
				pDoc->lspc.cOP.blistOutput[MATOUT] = true;
				pDoc->lspc.cOP.blistOutput[MATOSQAL] = true;
				pDoc->lspc.cOP.blistOutput[TEMPC] = true;
			}
			if(m_bWQR)
			{
				pDoc->lspc.cOP.blistOutput[DOXC] = true;
				pDoc->lspc.cOP.blistOutput[DOXCMIN] = true;
				pDoc->lspc.cOP.blistOutput[DOXCMAX] = true;
				pDoc->lspc.cOP.blistOutput[DOXCAV] = true;
				pDoc->lspc.cOP.blistOutput[DOXL] = true;
				pDoc->lspc.cOP.blistOutput[BODC] = true;
				pDoc->lspc.cOP.blistOutput[BODL] = true;
				pDoc->lspc.cOP.blistOutput[NO3C] = true;
				pDoc->lspc.cOP.blistOutput[NO3L] = true;
				pDoc->lspc.cOP.blistOutput[TAMC] = true;
				pDoc->lspc.cOP.blistOutput[TAML] = true;
				pDoc->lspc.cOP.blistOutput[NO2C] = true;
				pDoc->lspc.cOP.blistOutput[NO2L] = true;
				pDoc->lspc.cOP.blistOutput[PO4C] = true;
				pDoc->lspc.cOP.blistOutput[PO4L] = true;
				pDoc->lspc.cOP.blistOutput[SNH4C] = true;
				pDoc->lspc.cOP.blistOutput[SNH4L] = true;
				pDoc->lspc.cOP.blistOutput[SPO4C] = true;
				pDoc->lspc.cOP.blistOutput[SPO4L] = true;
				pDoc->lspc.cOP.blistOutput[PHYC] = true;
				pDoc->lspc.cOP.blistOutput[PHYL] = true;
				pDoc->lspc.cOP.blistOutput[PHYCLAC] = true;
				pDoc->lspc.cOP.blistOutput[BENALC] = true;
				pDoc->lspc.cOP.blistOutput[ORNC] = true;
				pDoc->lspc.cOP.blistOutput[ORNL] = true;
				pDoc->lspc.cOP.blistOutput[ORPC] = true;
				pDoc->lspc.cOP.blistOutput[ORPL] = true;
				pDoc->lspc.cOP.blistOutput[ORCC] = true;
				pDoc->lspc.cOP.blistOutput[ORCL] = true;
				pDoc->lspc.cOP.blistOutput[LPH] = true;
				pDoc->lspc.cOP.blistOutput[LALK] = true;
				pDoc->lspc.cOP.blistOutput[TICC] = true;
				pDoc->lspc.cOP.blistOutput[TICL] = true;
				pDoc->lspc.cOP.blistOutput[CO2C] = true;
				pDoc->lspc.cOP.blistOutput[CO2L] = true;
				pDoc->lspc.cOP.blistOutput[TEMPC] = true;
			}
		}
	}
	else
	{
		// set customized list to false
		int nCounts = sizeof(strParameters)/sizeof(CString);
		for(int i=0; i<nCounts; i++)
			m_cPList.SetSel(i, false);

		m_bDefault = (pDoc->lspc.nDefault != 0);
		m_bHydro = (pDoc->lspc.nHydro != 0);
		m_bSediment = (pDoc->lspc.nSed != 0);
		m_bWQ = (pDoc->lspc.nGQUAL != 0);
		m_bWQR = (pDoc->lspc.nRQUAL != 0);
		m_bSnow = (pDoc->lspc.nSnow != 0);						
		m_bCustom = (pDoc->lspc.nCustom != 0);
		
		if(m_bCustom)
		{
			//select the user specified list
			for(int i=0; i<PARA_NUM; i++)
				m_cPList.SetSel(i, pDoc->lspc.cOP.blistOutput[i]);
		}

		m_bLanduse = (pDoc->lspc.nLUsummary != 0);
		m_bStream = (pDoc->lspc.nCHsummary != 0);
		m_bPointSource = (pDoc->lspc.nPSsummary != 0);
		m_bThreshold = (pDoc->lspc.nThreshold != 0);
		m_bSUSTAIN = (pDoc->lspc.nSUSTAIN != 0);
		m_clanduse.SetCurSel(pDoc->lspc.summarylevel);
		m_strLocation = pDoc->lspc.m_strModelResultLocation;

		CPropertyPage::UpdateData(bSaveAndValidate);
	}

	return TRUE;
}

void CPageOutput::OnSeperate6() 
{
	CPropertyPage::UpdateData(true);
	if(m_bHydro || m_bSediment || m_bWQ || m_bWQR || m_bSnow)	
	{
		m_bDefault = false;
		m_bCustom = false;
		int nCounts = sizeof(strParameters)/sizeof(CString);
		for(int i=0; i<nCounts; i++)
			m_cPList.SetSel(i, false);
	}
	CPropertyPage::UpdateData(false);
}

void CPageOutput::OnCustom() 
{
	CPropertyPage::UpdateData(true);
	if(m_bCustom)
	{
		m_bDefault = false;
		m_bHydro = m_bSediment = m_bWQ = m_bWQR = m_bSnow = false;
	}
	CPropertyPage::UpdateData(false);
}

BOOL CPageOutput::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return false;

	CLSPCDoc *pDoc = pView->GetDocument();
	if(pDoc == NULL)
		return false;

	// set the content with the parameter list
	int nCounts = sizeof(strParameters)/sizeof(CString);
	for(int i = 0; i < nCounts; ++i)
		m_cPList.AddString(strParameters[i]);

	m_bDefault = (pDoc->lspc.nDefault != 0);
	m_bHydro = (pDoc->lspc.nHydro != 0);
	m_bSediment = (pDoc->lspc.nSed != 0);
	m_bWQ = (pDoc->lspc.nGQUAL != 0);
	m_bWQR = (pDoc->lspc.nRQUAL != 0);
	m_bSnow = (pDoc->lspc.nSnow != 0);						 
	m_bCustom = (pDoc->lspc.nCustom != 0);
	
	if(m_bCustom)
	{
		//select the user specified list
		for(int i=0; i<PARA_NUM; i++)
			m_cPList.SetSel(i, pDoc->lspc.cOP.blistOutput[i]);
	}

	m_bLanduse = (pDoc->lspc.nLUsummary != 0);
	m_bStream = (pDoc->lspc.nCHsummary != 0);
	m_bPointSource = (pDoc->lspc.nPSsummary != 0);
	m_bThreshold = (pDoc->lspc.nThreshold != 0);
	m_bSUSTAIN = (pDoc->lspc.nSUSTAIN != 0);
	m_clanduse.SetCurSel(pDoc->lspc.summarylevel);
	m_strLocation = pDoc->lspc.m_strModelResultLocation;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageOutput::OnSelchangeCustomlist() 
{
	CPropertyPage::UpdateData(true);
	if(!m_bCustom)
	{
		int nCounts = sizeof(strParameters)/sizeof(CString);
		for(int i = 0; i < nCounts; ++i)
			m_cPList.SetSel(i, false);
	}
	return;
}

void CPageOutput::OnDefaultchk() 
{
	CPropertyPage::UpdateData(true);
	if(m_bDefault)
	{
		m_bHydro = false;
		m_bSediment = false;
		m_bWQ = false;
		m_bWQR = false;
		m_bSnow = false;										
		m_bCustom = false;
		int nCounts = sizeof(strParameters)/sizeof(CString);
		for(int i=0; i<nCounts; i++)
			m_cPList.SetSel(i, false);
	}
	CPropertyPage::UpdateData(false);
}

void CPageOutput::OnLanduse() 
{
	CPropertyPage::UpdateData(true);
}

void CPageOutput::OnStream() 
{
	CPropertyPage::UpdateData(true);
}

void CPageOutput::OnThreshold() 
{
	CPropertyPage::UpdateData(true);
	
}

void CPageOutput::OnSUSTAIN() 
{
	CPropertyPage::UpdateData(true);
}

void CPageOutput::OnPointSource() 
{
	CPropertyPage::UpdateData(true);
}
