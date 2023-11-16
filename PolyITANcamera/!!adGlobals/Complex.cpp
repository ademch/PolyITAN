/* -------------------------------------------------------------------- */   
/* Z++ Version 1.10              complex.cpp      Last revised 10/10/92 */   
/*                                                                      */   
/* Complex number class for Turbo C++/Borland C++.          */   
/* Copyright 1992 by Carl W. Moreland                   */   
/* -------------------------------------------------------------------- */   
   
#include "stdafx.h"
#include "complex.h"

#define _USE_MATH_DEFINES

#include <math.h>

unsigned char compleX::zArgMode   = Z_RADIANS;   
unsigned char compleX::zPrintMode = Z_COMMA;   
unsigned char compleX::zLetter    = 'i';   
   
/* -------------------------------------------------------------------- */   
   
double arg(const compleX& z)        // argument (angle)   
{   
  if(z.re == 0 && z.im == 0)        // this is actually a domain error   
    return 0;   
  if(compleX::zArgMode == Z_RADIANS)   
    return atan2(z.im, z.re);   
  return atan2(z.im, z.re)/M_PI*180;   
}   
   
compleX ptor(double mag, double angle)  // polar-to-rectangular   
{   
  if(compleX::zArgMode == Z_RADIANS)   
    return compleX(mag*cos(angle), mag*sin(angle));   
  return compleX(mag*cos(angle/180*M_PI), mag*sin(angle/180*M_PI));   
}   
   
compleX rtop(double x, double y)    // rectangular-to-polar   
{   
  if(x == 0 && y == 0)				// this is actually a domain error   
    return Z0;   
  if(compleX::zArgMode == Z_RADIANS)   
    return compleX(sqrt(x*x + y*y), atan2(y, x));   
  return compleX(sqrt(x*x + y*y), atan2(y, x)*180/M_PI);   
}   
   
compleX& compleX::topolar(void)   
{   
  double re_tmp = re;   
   
  if(re != 0 || im != 0)			// z = (0,0) is a domain error   
  {   
    re = sqrt(re*re + im*im);   
    im = atan2(im, re_tmp);   
  }   
   
  if(compleX::zArgMode == Z_DEGREES)   
    im *= (180/M_PI);   
   
  return *this;   
}   
   
compleX& compleX::torect(void)   
{   
  double re_tmp = re;   
   
  re = re_tmp*cos(im);   
  im = re_tmp*sin(im);   
   
  return *this;   
}   
   
/* ----- Operators ---------------------------------------------------- */   
   
compleX operator/(const compleX& z1, const compleX& z2)   
{   
  if(z2 == Z0)   
    return compleX(Zinf);       // z2 = Z0 is an error!   
   
  double denom = z2.re*z2.re + z2.im*z2.im;   
  double re = (z1.re*z2.re + z1.im*z2.im)/denom;   
  double im = (z2.re*z1.im - z2.im*z1.re)/denom;   
  return compleX(re, im);   
}   
   
compleX operator/(const double x, const compleX& z)   
{   
  if(z == Z0)   
    return compleX(Zinf);       // z = Z0 is an error!   
   
  double denom = z.re*z.re + z.im*z.im;   
  return compleX(x*z.re/denom, -z.im*x/denom);   
}   
   
/* ----- Math functions ----------------------------------------------- */   
   
double abs(const compleX& z)   
{   
  return sqrt(z.re*z.re + z.im*z.im);   
}   
   
compleX sqrt(const compleX& z)   
{   
  return ptor(sqrt(abs(z)), arg(z)/2);   
}

compleX sqr(const compleX& z)   
{   
	return compleX(z.re*z.re - z.im*z.im, 2*z.re*z.im);   
}
   
compleX pow(const compleX& base, const double exponent)   
{   
  if(base != Z0 && exponent == 0.0)   
    return compleX(1,0);   
   
  if (base == Z0 && exponent > 0)   
    return Z0;   
   
  // base == Z0 && exponent == 0 is undefined!   
   
  return ptor(pow(abs(base), exponent), exponent*arg(base));   
}   
   
compleX pow(const double base, const compleX& exponent)   
{   
  if(base != 0.0 && exponent == Z0)   
    return compleX(1,0);   
   
  if (base == 0 && re(exponent) > 0)   
    return compleX(0,0);   
   
  // base == 0 && re(exponent) == 0 is undefined!   
   
  if(base > 0.0)   
    return exp(exponent * log(fabs(base)));   
   
  return exp(exponent * compleX(log(fabs(base)), M_PI));   
}   
   
compleX pow(const compleX& base, const compleX& exponent)   
{   
  if(base != Z0 && exponent == Z0)   
    return compleX(1,0);   
   
  if(base == Z0 && re(exponent) > 0)   
    return compleX(0,0);   
   
  // base == Z0 && re(exponent) == 0 is undefined!   
   
  return exp(exponent * log(base));   
}

compleX mix(const compleX& val1, const compleX& val2, const float t)
{
	return val1*(1-t) + val2*t;
}

   
/* ----- Trig functions ----------------------------------------------- */   
   
compleX exp(const compleX& z)   
{   
  double mag = exp(z.re);   
  return compleX(mag*cos(z.im), mag*sin(z.im));   
}   
   
compleX log(const compleX& z)   
{   
  return compleX(log(mag(z)), atan2(z.im, z.re));   
}   
   
compleX log10(const compleX& z)   
{   
  return log(z) * M_LOG10E;   
}   
   
compleX sin(const compleX& z)   
{   
  return (exp(Zi*z) - exp(-Zi*z))/(2*Zi);   
}   
   
compleX cos(const compleX& z)   
{   
  return (exp(Zi*z) + exp(-Zi*z))/2;   
}   
   
compleX tan(const compleX& z)   
{   
  return sin(z)/cos(z);   
}   
   
compleX asin(const compleX& z)   
{   
  return -Zi*log(Zi*z+sqrt(1-z*z));   
}   
   
compleX acos(const compleX& z)   
{   
  return -Zi*log(z+Zi*sqrt(1-z*z));   
}   

compleX atan(const compleX& z)   
{   
  return -0.5*Zi * log((1+Zi*z)/(1-Zi*z));
}

compleX sinh(const compleX& z)
{
  return (exp(z) - exp(-z))/2; 
}
   
compleX cosh(const compleX& z)   
{
  return (exp(z) + exp(-z))/2;   
}
   
compleX tanh(const compleX& z)   
{   
  return sinh(z)/cosh(z);   
}

compleX isotropicZ(const Vec3complex& c0, const Vec3complex& c1, float fSign)    
{   
	return c0.Z + fSign*Zi*sqrt((c1.X-c0.X)*(c1.X-c0.X) + (c1.Y-c0.Y)*(c1.Y-c0.Y));
}

compleX isotropicY(const Vec2complex& c0, const Vec2complex& c1, float fSign)    
{   
	return c0.Y + fSign*Zi*(c1.X-c0.X);
}
   