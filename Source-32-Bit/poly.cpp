// Poly.cpp : implementation file
//

#include "stdafx.h"
#include "poly.h"
#include <stdio.h>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/**********************************************
the cpp content
*********************************************/
CPolygon::CPolygon()
{
	ASSERT(pointList.GetCount() == 0);
	bCustomized = FALSE;
}


CPolygon::~CPolygon()
{
	CPoly_Free();
}


void CPolygon::CPoly_Free()
{
	while(!pointList.IsEmpty())
		delete (Point *) pointList.RemoveTail();
}

double CPolygon::GetArea()
{
	int i, j;
	double lfArea = 0;
	POSITION pos;


	int nCount = pointList.GetCount();
	if(nCount < 3)
		return lfArea;

	for(i=0; i<nCount; i++)
	{
		//as our polygon are not closed, do the following
		j = (i + 1) % nCount;

		pos = pointList.FindIndex(i);
		Point *pti = (Point *) pointList.GetAt(pos);

		pos = pointList.FindIndex(j);
		Point *ptj = (Point *) pointList.GetAt(pos);

		lfArea += ptj->x*pti->y;
		lfArea -= pti->x*ptj->y;
	}

	lfArea /= 2;
	return (lfArea<0)?-lfArea:lfArea;
}

double CPolygon::GetWettedPerimeter()					
{
	int nCount = pointList.GetCount();
    if(nCount < 3)
	   return NULL;

	POSITION pos = pointList.FindIndex(0);
	Point *pt = (Point *)pointList.GetAt(pos);

	double lfPerimeter = 0.0;
	for (int i = 0;i < nCount-1;i++) 
	{
		pos = pointList.FindIndex(i);
		Point *pti_0 = (Point *)pointList.GetAt(pos);
		pos = pointList.FindIndex(i+1);
		Point *pti_1 = (Point *)pointList.GetAt(pos);
		if(pti_0->y == pt->y && pti_1->y == pt->y)
		{
		}
		else
		{
			lfPerimeter += sqrt((pti_0->x-pti_1->x)*(pti_0->x-pti_1->x)+
				(pti_0->y-pti_1->y)*(pti_0->y-pti_1->y));
		}
	}
	return lfPerimeter;
}

	// added to get top width for ftable to calculate surface area				//   (04-28-04)

double CPolygon::GetTopWidth()						
{
	int nCount = pointList.GetCount();
    if(nCount < 3)
	   return NULL;

	POSITION pos = pointList.FindIndex(0);

	double lfTopWidth = 0.0;

		pos = pointList.FindIndex(0);
		Point *pt0 = (Point *)pointList.GetAt(pos);
		pos = pointList.FindIndex(nCount-1);
		Point *pt1 = (Point *)pointList.GetAt(pos);

		lfTopWidth = sqrt((pt0->x-pt1->x)*(pt0->x-pt1->x)+
				(pt0->y-pt1->y)*(pt0->y-pt1->y));

		return lfTopWidth;
}

/*
CPolygon* CPolygon::GetNewPolygon(double y)
{
	int nCount = pointList.GetCount();
    if(nCount < 3)
	   return NULL;

	CPolygon *polynew = new CPolygon(); 

	// assume that y will increase first and then descrease
	// if polygon[i].y > y and polygon[i-1].y < y, do intersect
	for (int i = 0;i < nCount;i++) 
	{
		POSITION pos = pointList.FindIndex(i);
		Point *pti = (Point *)pointList.GetAt(pos);
		

		//for the first and last point, add it to the new list
		if(i == 0)
		{
			Point *ptNew = new Point();
			ptNew->x = pti->x;
			ptNew->y = pti->y;
			polynew->pointList.AddTail((CObject *)ptNew);
		}
		else if(i == nCount-1)
		{
				POSITION pos = pointList.FindIndex(i-1);
				Point *pt_pre = (Point *)pointList.GetAt(pos);
				if(pt_pre->y > y)
				{
					// get the intersection point
					// add this new point to the list
					Point *ptNew = new Point();
					ptNew->y = y;
					ptNew->x = (pt_pre->x == pti->x)?pti->x:((y-pt_pre->y)*(pti->x-pt_pre->x)/(pti->y-pt_pre->y)+pt_pre->x);
					polynew->pointList.AddTail((CObject *)ptNew);
				}
				Point *ptNew = new Point();
				ptNew->x = pti->x;
				ptNew->y = pti->y;
				polynew->pointList.AddTail((CObject *)ptNew);
		}
		else
		{
			if(pti->y == y)
			{
				Point *ptNew = new Point();
				ptNew->x = pti->x;
				ptNew->y = pti->y;
				polynew->pointList.AddTail((CObject *)ptNew);
			}
			else if(pti->y < y)
			{
				//
				POSITION pos = pointList.FindIndex(i-1);
				Point *pt_pre = (Point *)pointList.GetAt(pos);
				if(pt_pre->y > y)
				{
					// get the intersection point
					// add this new point to the list
					Point *ptNew = new Point();
					ptNew->y = y;
					ptNew->x = (pt_pre->x == pti->x)?pti->x:((y-pt_pre->y)*(pti->x-pt_pre->x)/(pti->y-pt_pre->y)+pt_pre->x);
					polynew->pointList.AddTail((CObject *)ptNew);
				}
				Point *ptNew = new Point();
				ptNew->x = pti->x;
				ptNew->y = pti->y;
				polynew->pointList.AddTail((CObject *)ptNew);
			}
			else
			{
				// get the (i-1)
				POSITION pos = pointList.FindIndex(i-1);
				Point *pt_pre = (Point *)pointList.GetAt(pos);
				if(pt_pre->y < y)
				{
					// get the intersection point
					// add this new point to the list
					Point *ptNew = new Point();
					ptNew->y = y;
					ptNew->x = (pt_pre->x == pti->x)?pti->x:((y-pt_pre->y)*(pti->x-pt_pre->x)/(pti->y-pt_pre->y)+pt_pre->x);
					polynew->pointList.AddTail((CObject *)ptNew);
				}
			}

		}
	}

	if(polynew->pointList.GetCount() >= 3)
		return polynew;
	else
	{
		delete polynew;
		return NULL;
	}
}
*/
CPolygon* CPolygon::GetNewPolygon(double y)
{
	int nCount = pointList.GetCount();
    if(nCount < 3)
	   return NULL;

	CPolygon *polynew = new CPolygon(); 

	// assume that y will increase first and then descrease
	// if polygon[i].y > y and polygon[i-1].y < y, do intersect
	for (int i = 0;i < nCount;i++) 
	{
		POSITION pos = pointList.FindIndex(i);
		Point *pti = (Point *)pointList.GetAt(pos);

		if(pti->y == y)
		{
			Point *ptNew = new Point();
			ptNew->x = pti->x;
			ptNew->y = pti->y;
			polynew->pointList.AddTail((CObject *)ptNew);
		}
		else if(pti->y < y)
		{
			//
			if(i >= 1)
			{
				POSITION pos = pointList.FindIndex(i-1);
				Point *pt_pre = (Point *)pointList.GetAt(pos);
				if(pt_pre->y > y)
				{
					// get the intersection point
					// add this new point to the list
					Point *ptNew = new Point();
					ptNew->y = y;
					ptNew->x = (pt_pre->x == pti->x)?pti->x:((y-pt_pre->y)*(pti->x-pt_pre->x)/(pti->y-pt_pre->y)+pt_pre->x);
					polynew->pointList.AddTail((CObject *)ptNew);
				}
			}
				Point *ptNew = new Point();
				ptNew->x = pti->x;
				ptNew->y = pti->y;
				polynew->pointList.AddTail((CObject *)ptNew);
		}
		else
		{
			// get the (i-1)
			if(i >= 1)
			{
				POSITION pos = pointList.FindIndex(i-1);
				Point *pt_pre = (Point *)pointList.GetAt(pos);
				if(pt_pre->y < y)
				{
					// get the intersection point
					// add this new point to the list
					Point *ptNew = new Point();
					ptNew->y = y;
					ptNew->x = (pt_pre->x == pti->x)?pti->x:((y-pt_pre->y)*(pti->x-pt_pre->x)/(pti->y-pt_pre->y)+pt_pre->x);
					polynew->pointList.AddTail((CObject *)ptNew);
				}
			}
		}
	}

	if(polynew->pointList.GetCount() >= 3)
		return polynew;
	else
	{
		delete polynew;
		return NULL;
	}
}

void CPolygon::Print()
{
	int nCount = pointList.GetCount();
	//fprintf(stderr,"Total points:%d\n",nCount);
	TRACE("Polygon Begin:\n");
	TRACE("Total points:%d\n",nCount);
	for (int i = 0;i < nCount;i++) 
	{
		POSITION pos = pointList.FindIndex(i);
		Point *pti = (Point *)pointList.GetAt(pos);
		//fprintf(stderr,"Point %d:(%lf,%lf)\n",i,pti->x,pti->y);
		TRACE("(%lf,%lf)\n",pti->x,pti->y);
	}
	//fprintf(stderr,"Area = %lf\n",GetArea());
	TRACE("Polygon End\n");
}

CPolygon const& CPolygon::operator=(CPolygon const &src)
{
	if(this != &src)
	{
		CPoly_Free();

		POSITION pos;
		int nCount = src.pointList.GetCount();
		for(int i=0; i<nCount; i++)
		{
			pos = src.pointList.FindIndex(i);
			Point *pti = (Point *) src.pointList.GetAt(pos);
			Point *pt = new Point();
			pt->x = pti->x;
			pt->y = pti->y;
			pointList.AddTail((CObject *)pt);
		}

		bCustomized = src.bCustomized;
	}

	return *this;
}
