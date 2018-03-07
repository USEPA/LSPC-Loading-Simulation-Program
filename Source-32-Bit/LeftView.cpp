// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "LSPC.h"

#include "LSPCDoc.h"
#include "LeftView.h"
#include "MainFrm.h"
#include "LSPCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	//{{AFX_MSG_MAP(CLeftView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftView construction/destruction

CLeftView::CLeftView()
{
	pTreeLeaves = NULL;
}

CLeftView::~CLeftView()
{
	if(pTreeLeaves != NULL)
		delete []pTreeLeaves;
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView drawing

void CLeftView::OnDraw(CDC* pDC)
{
	CLSPCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


/////////////////////////////////////////////////////////////////////////////
// CLeftView printing

BOOL CLeftView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLeftView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLeftView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	if(pMain == NULL)
		return;
	pMain->m_indexMap.RemoveAll();

	HTREEITEM hL1Item,hL2Item,hL3Item,hL4Item;

	// add more information to the following structure so that we 
	// can get the corresponding care numbers
	CString str[] ={
					"#0Global Settings",
					"*0Simulation Options",
					"*0Temporal Settings",
					"*0Watershed Information",
					"$0Watershed Settings",
					"$0Landuse Data",
					"@0Landuse Categories",
					"@0Landuse Routing",
					"@0Landuse Attributes",
					"@0Landuse Changes",
					"$0Weather Data",
					"@0Data Location",
					"@0File Information",
					"@0Station Information",
					"@0Station Selection",
					"$0Default Group Assignment",
					"*0Stream Information",
					"$0Stream Settings",
					"$0Monthly F(vol) Adjustment Factors",
					"$0Stream Network",
					"$0Stream Linkage",
					"$0Stream Characteristics",
					"*0Point Source Information",
					"$0Point Sources Summary",
					"$0Point Sources Quals",
					"$0Point Sources WithDrawal",
					"#0Snow",
					"*0General Settings",
					"*0Snow pack/melt Parameter Group 1",
					"*0Snow pack/melt Parameter Group 2",
					"*0Snow pack/melt Initial Condition Parameter",
					"#0Water Budget",
					"*0Water Budget Parameter Group 1",
					"*0Water Budget Parameter Group 2",
					"*0Water Budget Parameter Group 3",
					"$0Temporal Parameter Variability",
					"$0Constant Parameters",
					"$0Monthly Parameters",
					"@0Interception storage capacity",
					"@0Upper zone nominal storage",
					"@0Manning's overland coefficent",
					"@0Interflow inflow",
					"@0Interflow recession constant",
					"@0Lower zone evapotranspiration",
					"*0Water Budget Initial Conditions",
					"*0Irrigation Application Options",
					"$0Irrigation Flag",
					"$0Irrigation PET Values",
					"$0Irrigation Application Methods",
					"$0Monthly-variable ET coefficients",
					"$0Irrigation Withdrawal Options",
					"#0Sediment",
					"*0Sediment Controls",
					"*0Street Sweeping for Sediment (Impervious Only)",
					"*0Street Sweeping for GQual (Impervious Only)",
					"*0Land-based Sediment Contributions",
					"$0Sediment Parameter Group 1",
					"$0Sediment Parameter Group 2",
					"*0GQUAL-Sediment to Stream Mapping",
					"*0Monthly Land Parameters",
					"$0Erosion-related cover values",
					"$0Net vertical sediment input",
					"*0Stream-based Sediment Contributions",	
					"$0Sediment Parameter Group 3",
					"$0Sediment Parameter Group 4",
					"$0Streambank Erosion Parameters",
					"#0Temperature",
					"*0Soil Temperature Monthly Controls",
					"*0Soil Temperature Constant Parameters",
					"*0Soil Temperature Monthly Parameters",
					"$0ASLT for Surface Temperature",
					"$0BSLT for Surface Temperature",
					"$0AIFT for Interflow Temperature",
					"$0BIFT for Interflow Temperature",
					"$0AGWT for Groundwater Temperature",
					"$0BGWT for Groundwater Temperature",
					"*0Surface Temperature Parameters",
					"*0Stream Temperature Parameters",
					//"*0Hourly Dry Lapse Rate",
					"*0Bed Heat Conduction Parameters",
					"*0Monthly Ground Temperatures",
					"#0Water Quality Constituents - GQUAL",
					"*0Land-based Pollutant Contributions",
					"$0General Settings",
					"$0Water Quality Controls",
					"$0Constant Loading Parameters",
					"$0Monthly Loading Parameters",
					"@0Accumulation Rate",
					"@0Limiting Storage",
					"@0Surface flow Concentration",
					"@0Interflow Concentration",
					"@0Groundwater Outflow Concentration",
					"@0Atmospheric Dry Deposition",
					"@0Atmospheric Wet Deposition",
					"*0Stream-based Pollutant Contributions",	
					"$0Water Quality Controls",
					"$0Water Quality Parameters",
					"$0Monthly Loading Parameters",
					"@0Atmospheric Dry Deposition",
					"@0Atmospheric Wet Deposition",
					"$0Sediment Associated Parameters",
					"@0GQ-SEDDECAY",
					"@0GQ-KD",
					"@0GQ-ADRATE",
					"@0GQ-ADTHETA",
					"@0GQ-SEDCONC",
					"#1Water Quality Constituents - RQUAL",
					"*1Land to Stream Mapping",
					"*1Atmosphere to Stream Mapping",
					"*1Land-based DO and CO2 Contributions",
					"$1General Settings",
					"$1Constant Parameters",
					"$1Monthly Parameters",
					"@1DO Interflow Concentrations",
					"@1DO Groundwater Concentrations",
					"@1CO2 Interflow Concentrations",
					"@1CO2 Groundwater Concentrations",
					"*1OXRX Parameters",
					"$1DO-BOD Controls",
					"$1DO-BOD parm1",
					"$1DO-BOD parm2",
					"$1DO-BOD init",
					"$1SCOUR parm",
					"*1NUTRX Parameters",
					"$1Nutrient Controls",
					"$1NUT parm1",
					"$1NUT parm2",
					"$1NUT parm3",
					"$1NUT init",
					"*1PLANK Parameters",
					"$1Plankton Controls",
					"$1PLANK parm1",
					"$1PLANK parm2",
					"$1PLANK parm3",
					"$1PLANK parm4",
					"$1PLANK init",
					"*1PHCARB Parameters",
					"$1pH-Carbon Controls",
					"$1PH parm",
					"$1PH init",
					"$1Monthly Parameters",
					"@1pH total inorganic carbon",
					"@1pH dissolved carbon dioxide",
					"#1TMDL Control Options",
					"*1General Settings",
					"*1Threshold Mapping",
					"*1Threshold Definitions",
					"*1Point Source Controls",
					"*1Land-based Controls",
					"*1Stream-switch Controls",
					"*1Stream-limit Controls",
					"*1Stream-based Controls",
					"#0Output"
					};

	DWORD nIndex = 0;
	DWORD dwListItemData = 0;
	int nPageCount[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int nNum = sizeof(str)/sizeof(CString);

	if (nNum > 0)
		pTreeLeaves = new HTREEITEM[nNum];
	if(pTreeLeaves == NULL)
		return;
	
	CString strShow, strKey, strValue;
	int nPropertySheetNum;


	for(int i = 0; i < nNum;++i)
	{
		strShow = str[i].Right(str[i].GetLength()-2);
		nPropertySheetNum = atoi((LPCSTR)str[i].Mid(1,1));

		if(str[i].GetAt(0) == '#') // level one
		{
			hL1Item = GetTreeCtrl().InsertItem(strShow);
			//GetTreeCtrl().EnsureVisible(hL1Item);
			if(i == nNum-1)
			{
				dwListItemData = nPropertySheetNum*1000+nPageCount[nPropertySheetNum];
				GetTreeCtrl().SetItemData(hL1Item,dwListItemData);
				pTreeLeaves[nIndex] = hL1Item;
				strKey.Format("%ld", dwListItemData);
				strValue.Format("%ld", nIndex);
				pMain->m_indexMap.SetAt(strKey, strValue);
				nPageCount[nPropertySheetNum]++;
				nIndex++;
			}
		}
		else if(str[i].GetAt(0) == '*') // level two 
		{
			hL2Item = GetTreeCtrl().InsertItem(strShow,hL1Item);
			//GetTreeCtrl().EnsureVisible(hL2Item);
			if(i != nNum-1 && str[i+1].GetAt(0) != '$' || i == nNum-1)
			{
				dwListItemData = nPropertySheetNum*1000+nPageCount[nPropertySheetNum];
				GetTreeCtrl().SetItemData(hL2Item,dwListItemData);
				pTreeLeaves[nIndex] = hL2Item;
				strKey.Format("%ld", dwListItemData);
				strValue.Format("%ld", nIndex);
				pMain->m_indexMap.SetAt(strKey, strValue);
				nPageCount[nPropertySheetNum]++;
				nIndex++;
			}
		}
		else if(str[i].GetAt(0) == '$') // level three
		{
			hL3Item = GetTreeCtrl().InsertItem(strShow,hL2Item);
			//GetTreeCtrl().EnsureVisible(hL3Item);
			if(i != nNum-1 && str[i+1].GetAt(0) != '@' || i == nNum-1)
			{
				dwListItemData = nPropertySheetNum*1000+nPageCount[nPropertySheetNum];
				GetTreeCtrl().SetItemData(hL3Item,dwListItemData);
				pTreeLeaves[nIndex] = hL3Item;
				strKey.Format("%ld", dwListItemData);
				strValue.Format("%ld", nIndex);
				pMain->m_indexMap.SetAt(strKey, strValue);
				nPageCount[nPropertySheetNum]++;
				nIndex++;
			}
		}
		else if(str[i].GetAt(0) == '@') // level four
		{
			hL4Item = GetTreeCtrl().InsertItem(strShow,hL3Item);
			//GetTreeCtrl().EnsureVisible(hL4Item);
			dwListItemData = nPropertySheetNum*1000+nPageCount[nPropertySheetNum];
			GetTreeCtrl().SetItemData(hL4Item,dwListItemData);
			pTreeLeaves[nIndex] = hL4Item;
			strKey.Format("%ld", dwListItemData);
			strValue.Format("%ld", nIndex);
			pMain->m_indexMap.SetAt(strKey, strValue);
			nPageCount[nPropertySheetNum]++;
			nIndex++;
		}
	}

	// by default, select the first item
	//SelectItem(0);
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CLSPCDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLSPCDoc)));
	return (CLSPCDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLeftView message handlers

BOOL CLeftView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	dwStyle  = dwStyle | TVS_HASLINES | TVS_SHOWSELALWAYS | TVS_LINESATROOT |TVS_HASBUTTONS |TVS_SINGLEEXPAND;  
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CLeftView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	*pResult = 0;

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	if(pMain == NULL)
		return;
	CLSPCView *pView = (CLSPCView *)pMain->m_wndSplitter.GetPane(0,1);
	if(pView == NULL)
		return;
	
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	if(hItem == NULL) 
	{
		::MessageBeep(0);
		return;
	}
	if(GetTreeCtrl().ItemHasChildren(hItem))
		return;

	DWORD nIndex = 0;
	nIndex = GetTreeCtrl().GetItemData(hItem);

	int nPropertySheetNum;
	int nPageIndex;
	
	nPropertySheetNum = int(nIndex/1000);
	nPageIndex = nIndex%1000;
	// set the active page
	pView->SetActivePage(nPropertySheetNum, nPageIndex);

}

void CLeftView::SelectItem(int nIndex)
{
	if(pTreeLeaves == NULL)
		return;
	//int nNum = sizeof(pTreeLeaves)/sizeof(HTREEITEM);
	//if(nIndex >= nNum)
	//	return;
	GetTreeCtrl().SelectItem(pTreeLeaves[nIndex]);
}
