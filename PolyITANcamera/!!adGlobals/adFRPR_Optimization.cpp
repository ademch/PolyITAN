#include "stdafx.h"
#include "adFRPR_Optimization.h"
#include <limits>
#include <math.h>

/// Given a starting point p[1..n], Fletcher-Reeves-Polak-Ribiere minimization is performed on a function func, 
/// using its gradient as calculated by a routine dfunc. 
/// The convergence tolerance on the function value is input as ftol. 
/// Returned quantities are p (the location of the minimum),
/// iter (the number of iterations that were performed), 
/// and fret (the minimum value of the function). 
/// The routine linmin is called to perform line minimizations.
void adFletcherReevesPolakRibiereMinimization::frprmn(Vec_IO_DP &p,
													  const DP ftol, int &iter, DP &fret)
{
const DP EPS=1.0e-18;
int j,its;
DP gg,gam,fp,dgg;

	int n=p.size();
	Vec_DP g(n),h(n),xi(n);

	fp = EvalFunc(p);
	EvalFuncGradient(p,xi);
	for (j=0;j<n;j++)
	{
		g[j] = -xi[j];
		xi[j]=h[j]=g[j];
	}
	for (its=0;its<m_iMaxIterations;its++)
	{
		iter=its;
		linmin(p,xi,fret);
		if (2.0*fabs(fret-fp) <= ftol*(fabs(fret)+fabs(fp)+EPS))
			return;
		fp=fret;
		EvalFuncGradient(p,xi);
		dgg=gg=0.0;
		for (j=0;j<n;j++)
		{
			gg += g[j]*g[j];
			//		  dgg += xi[j]*xi[j];
			dgg += (xi[j]+g[j])*xi[j];
		}
		if (gg == 0.0)
			return;
		gam=dgg/gg;
		for (j=0;j<n;j++)
		{
			g[j] = -xi[j];
			xi[j]=h[j]=g[j]+gam*h[j];
		}
	}
	//ASSERT(0);// "Too many iterations in frprmn
}


/// Given an n-dimensional point p[1..n] and an n-dimensional direction xi[1..n], 
/// moves and resets p to where the function func(p) takes on a minimum along the direction xi from p,
/// and replaces xi by the actual vector displacement that p was moved. 
/// Also returns as fret the value of func at the returned location p. 
/// This is actually all accomplished by calling the routines mnbrak and brent.
void adFletcherReevesPolakRibiereMinimization::linmin(Vec_IO_DP &p, Vec_IO_DP &xi, DP &fret)
{
int j;
const DP TOL=1.0e-8;
DP xx,xmin,fx,fb,fa,bx,ax;

	int n=p.size();
	ncom=n;
	pcom_p=new Vec_DP(n);
	xicom_p=new Vec_DP(n);

	Vec_DP &pcom=*pcom_p,&xicom=*xicom_p;
	for (j=0;j<n;j++)
	{
		pcom[j]=p[j];
		xicom[j]=xi[j];
	}
	ax=0.0;
	xx=1.0;
	mnbrak(ax,xx,bx,fa,fx,fb);
	fret=brent(ax,xx,bx,TOL,xmin);
	for (j=0;j<n;j++)
	{
		xi[j] *= xmin;
		p[j] += xi[j];
	}
	delete xicom_p;
	delete pcom_p;
}


inline void shft3(DP &a, DP &b, DP &c, const DP d)
{
	a=b;
	b=c;
	c=d;
}

/// Given a function func, and given distinct initial points ax and bx, 
/// this routine searches in the downhill direction (defined by the function as evaluated at the initial points) 
/// and returns new points ax, bx, cx that bracket a minimum of the function. 
/// Also returned are the function values at the three points, fa, fb, and fc.
void adFletcherReevesPolakRibiereMinimization::mnbrak(DP &ax, DP &bx, DP &cx, DP &fa, DP &fb, DP &fc)
{
	const DP GOLD=1.618034,GLIMIT=100.0,TINY=1.0e-20;
	DP ulim,u,r,q,fu;

	fa=f1dim(ax);
	fb=f1dim(bx);
	if (fb > fa)
	{
		SWAP(ax,bx);
		SWAP(fb,fa);
	}
	cx=bx+GOLD*(bx-ax);
	fc=f1dim(cx);
	while (fb > fc)
	{
		r=(bx-ax)*(fb-fc);
		q=(bx-cx)*(fb-fa);
		u=bx-((bx-cx)*q-(bx-ax)*r)/(2.0*SIGN(MAX(fabs(q-r),TINY),q-r));
		ulim=bx+GLIMIT*(cx-bx);
		if ((bx-u)*(u-cx) > 0.0)
		{
			fu=f1dim(u);
			if (fu < fc)
			{
				ax=bx;
				bx=u;
				fa=fb;
				fb=fu;
				return;
			} else if (fu > fb) {
				cx=u;
				fc=fu;
				return;
			}
			u=cx+GOLD*(cx-bx);
			fu=f1dim(u);
		} else if ((cx-u)*(u-ulim) > 0.0) {
			fu=f1dim(u);
			if (fu < fc) {
				shft3(bx,cx,u,u+GOLD*(u-cx));
				shft3(fb,fc,fu,f1dim(u));
			}
		} else if ((u-ulim)*(ulim-cx) >= 0.0) {
			u=ulim;
			fu=f1dim(u);
		} else
		{
			u=cx+GOLD*(cx-bx);
			fu=f1dim(u);
		}
		shft3(ax,bx,cx,u);
		shft3(fa,fb,fc,fu);
	}
}

/// Given a function f, and given a bracketing triplet of abscissas ax, bx, cx 
/// (such that bx is between ax and cx, and f(bx) is less than both f(ax) and f(cx)), 
/// this routine isolates the minimum to a fractional precision of about tol using Brent's method. 
/// The abscissa of the minimum is returned as xmin, and the minimum function value is returned as brent, the returned function value.

DP adFletcherReevesPolakRibiereMinimization::brent(const DP ax, const DP bx, const DP cx, const DP tol, DP &xmin)
{
const int ITMAX=100;
const DP CGOLD=0.3819660;
const DP ZEPS=numeric_limits<DP>::epsilon()*1.0e-3;
int iter;
DP a,b,d=0.0,etemp,fu,fv,fw,fx;
DP p,q,r,tol1,tol2,u,v,w,x,xm;
DP e=0.0;

	a=(ax < cx ? ax : cx);
	b=(ax > cx ? ax : cx);
	x=w=v=bx;
	fw=fv=fx=f1dim(x);
	for (iter=0;iter<ITMAX;iter++) {
		xm=0.5*(a+b);
		tol2=2.0*(tol1=tol*fabs(x)+ZEPS);
		if (fabs(x-xm) <= (tol2-0.5*(b-a)))
		{
			xmin=x;
			return fx;
		}
		if (fabs(e) > tol1)
		{
			r=(x-w)*(fx-fv);
			q=(x-v)*(fx-fw);
			p=(x-v)*q-(x-w)*r;
			q=2.0*(q-r);
			if (q > 0.0) p = -p;
			q=fabs(q);
			etemp=e;
			e=d;
			if (fabs(p) >= fabs(0.5*q*etemp) || p <= q*(a-x) || p >= q*(b-x))
				d=CGOLD*(e=(x >= xm ? a-x : b-x));
			else
			{
				d=p/q;
				u=x+d;
				if (u-a < tol2 || b-u < tol2)
					d=SIGN(tol1,xm-x);
			}
		} else {
			d=CGOLD*(e=(x >= xm ? a-x : b-x));
		}
		u=(fabs(d) >= tol1 ? x+d : x+SIGN(tol1,d));
		fu=f1dim(u);
		if (fu <= fx)
		{
			if (u >= x) a=x; else b=x;
			shft3(v,w,x,u);
			shft3(fv,fw,fx,fu);
		} else {
			if (u < x) a=u; else b=u;
			if (fu <= fw || w == x) {
				v=w;
				w=u;
				fv=fw;
				fw=fu;
			} else if (fu <= fv || v == x || v == w) {
				v=u;
				fv=fu;
			}
		}
	}
	//nrerror("Too many iterations in brent");
	xmin=x;
	return fx;
}


DP adFletcherReevesPolakRibiereMinimization::f1dim(const DP x)
{
int j;

Vec_DP xt(ncom);
Vec_DP &pcom=*pcom_p,&xicom=*xicom_p;
	
	for (j=0;j<ncom;j++)
		xt[j]=pcom[j]+x*xicom[j];
	
return EvalFunc(xt);
}
