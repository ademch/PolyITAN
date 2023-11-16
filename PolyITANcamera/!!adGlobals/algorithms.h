#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "vector_math.h"

// Parametric surface analysis

Vec3d ParamSurfaceDerivU(double U, double V, Vec3d (*pfun)(double, double));
Vec3d ParamSurfaceDerivV(double U, double V, Vec3d (*pfun)(double, double));

Vec3d ParamSurfaceNormal(double U, double V, Vec3d (*pfun)(double, double));

class NewtonRaphsonZeroPointFunctionBase
{
public:
	virtual float FuncVal(float) = 0;
	virtual float FuncDerivVal(float) = 0;
protected:
private:
};

// Problem solution iterational algorithms
// Parameters:
//        obZeroPointFunc- Class providing functions of:
//                         * single variable target equation to be solved for a root
//                         * analytic derivative of a target equation
//        fXInitialGuess- (in/out) value to be used as a first guess root and refined root afterwards
//        nMaxIterations
//        fPrec
int NewtonRaphson(NewtonRaphsonZeroPointFunctionBase *obZeroPointFunc, float &fX, unsigned short int nMaxIterations = 20, float fPrec = 1e-5);

#endif