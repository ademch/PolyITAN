
#include "stdafx.h"
#include "algorithms.h"

#define nabla 0.000001

Vec3d ParamCurveDerivU(double U, double V, Vec3d (*pfun)(double, double))
{	return (pfun(U+nabla,V)-pfun(U,V))/nabla;}

Vec3d ParamCurveDerivV(double U, double V, Vec3d (*pfun)(double, double))
{	return (pfun(U,V+nabla)-pfun(U,V))/nabla;}


Vec3d ParamSurfaceNormal(double U, double V, Vec3d (*pfun)(double, double))
{
	Vec3d vn1 = ParamCurveDerivU(U, V, pfun);
	Vec3d vn2 = ParamCurveDerivV(U, V, pfun);

return VecNormalize(vn1*vn2);
}


int NewtonRaphson(NewtonRaphsonZeroPointFunctionBase *obZeroPointFunc, float &fX, unsigned short int nMaxIterations, float fPrec)
{
float h;
unsigned short int i;

	for (i = nMaxIterations; i > 0; i--)
	{
		h = - obZeroPointFunc->FuncVal(fX)/obZeroPointFunc->FuncDerivVal(fX);
		if (fabs(h) < fPrec) break;
		fX += h;
	}
	
	if (i)
		return 0;	//success
	else
		return 1;   //iterations diverge
}
