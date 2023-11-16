
#ifndef FRPR_H 
#define FRPR_H

#include "Numerics/recipes/nrutil_nr.h"

typedef const NRVec<DP> Vec_I_DP;
typedef NRVec<DP> Vec_DP, Vec_O_DP, Vec_IO_DP;

class adFletcherReevesPolakRibiereMinimization
{
public:
	
	adFletcherReevesPolakRibiereMinimization()
	{
		m_MinDeltaAbs = 1e-4;
		m_DeltaRel    = 1e-4;

		m_iMaxIterations = 2000;
		m_iIterations    = 0;
		m_fTolerance     = 1.0e-6;
		m_fMinValue      = 0;
	}

	virtual DP EvalFunc(Vec_I_DP& aInputParams)
	{
		return 0;
	}

	virtual void EvalFuncGradient(Vec_DP &aInputParams, Vec_O_DP &aGradient)
	{
		unsigned int n = aGradient.size();
		DP dOriginalValue;
		DP delta;
		DP fRight, fLeft;

		for (unsigned int iPar = 0; iPar < n; iPar++)
		{
			dOriginalValue = aInputParams[iPar];
			
			delta = abs(aInputParams[iPar]*m_DeltaRel);
			if (delta < m_MinDeltaAbs)
				delta = m_MinDeltaAbs;
				
				aInputParams[iPar] = dOriginalValue + delta;
				fRight = EvalFunc(aInputParams);
				
				aInputParams[iPar] = dOriginalValue - delta;
				fLeft = EvalFunc(aInputParams);
				
				aGradient[iPar] = (fRight-fLeft) / (2.0*delta);
			
			aInputParams[iPar] = dOriginalValue; // setback
		}
	}

	void Optimize(Vec_IO_DP &p)
	{
		frprmn(p, m_fTolerance, m_iIterations, m_fMinValue );
	}

private:
	int ncom;
	Vec_DP *pcom_p,*xicom_p;

	void frprmn(Vec_IO_DP &p, const DP ftol, int &iter, DP &fret);
	void linmin(Vec_IO_DP &p, Vec_IO_DP &xi, DP &fret);
	void mnbrak(DP &ax, DP &bx, DP &cx, DP &fa, DP &fb, DP &fc);
	DP brent(const DP ax, const DP bx, const DP cx, const DP tol, DP &xmin);
	DP f1dim(const DP x);

public:
	// the minimal absolute perturbation for each parameter in calculating the numerical
	//   gradient. default 1e-4.
	DP m_MinDeltaAbs;
	// the default relative perturbation of each parameter in calculating the numerical
	//   gradient. default 1e-4.
	DP m_DeltaRel;

	// Maximum number of iterations (<I>default 200</I>).
	// See Also: MaxIterations
	int m_iMaxIterations;
	// The number of iterations from the last optimization.
	// See Also: Iterations
	int m_iIterations;
	// The fractional convergence tolerance to be achieved in the function value (<I>default 1.0e-6</I>).
	// See Also: Tolerance
	DP m_fTolerance;
	// The minimum value of the function from the last optimization.
	// See Also: MinValue
	DP m_fMinValue;
};


class TestConjugatedGradientMethod : public adFletcherReevesPolakRibiereMinimization
{
public:
	virtual DP EvalFunc(Vec_I_DP& aInputParams)
	{
		DP fX, fY;

		fX = aInputParams[0];
		fY = aInputParams[1];

		// RosenbrockFunctionEval
		return pow(1 - fX, 2) + 100 * pow(fY - pow(fX, 2), 2);
	}

	virtual void EvalFuncGradient(Vec_DP &aInputParams, Vec_O_DP &aGradient)
	{
		DP fX, fY;

		fX = aInputParams[0];
		fY = aInputParams[1];
		
		aGradient[0] = 2*fX - 2 - 400 * fX * (fY - pow(fX, 2));
		aGradient[1] = 200*(fY - pow(fX, 2));
	}

	TestConjugatedGradientMethod()
	{
		Vec_DP vInitialGuess(2);

		vInitialGuess[0] = -10.0;
		vInitialGuess[1] = -10.25;

		Optimize(vInitialGuess);
	}
};


#endif