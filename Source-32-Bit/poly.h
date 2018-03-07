// Poly.h : header file
#include <afxcoll.h>

#ifndef POLY_AREA_JIAN_OUYANG
#define POLY_AREA_JIAN_OUYANG

class Point : public CObject
{
public:
   double x, y;
   Point() { x = 0; y = 0; };
   virtual ~Point() {};
};


class CPolygon
{
public:
	CPolygon();
	virtual ~CPolygon();
	CPolygon const& operator=(CPolygon const &src);
	CObList pointList;
	BOOLEAN bCustomized;

	//memeber functions
	void   CPoly_Free();
	double GetArea();
	double GetWettedPerimeter();
	double GetTopWidth();					
	CPolygon* GetNewPolygon(double y);
	void Print();
};

#endif