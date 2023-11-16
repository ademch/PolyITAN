/* -------------------------------------------------------------------- */ 
/* Z++ Version 1.10               complex.h       Last revised 10/10/92 */ 
/*                                                                      */ 
/* Complex number class for Turbo C++/Borland C++.                      */ 
/* Copyright 1992 by Carl W. Moreland                                   */ 
/* -------------------------------------------------------------------- */ 
 
#ifndef COMPLEXdotH 
#define COMPLEXdotH

#include <math.h>
#include "vector_math.h"

const unsigned char Z_RADIANS = 0;
const unsigned char Z_DEGREES = 1; 
const unsigned char Z_COMMA   = 0;	// (x, y) 
const unsigned char Z_LETTER  = 1;	// x + iy 

#define PURE_I  compleX(0, 1)

enum Complex_Part
{
	Complex_Real,
	Complex_Imaginary
};
 
class compleX
{ 
public: 
  double re, im;

  double Value(Complex_Part part)
  {
	  if (part == Complex_Real)
		  return re;
	  else
		  return im;
  }
 
private: 
  static unsigned char zArgMode; 
  static unsigned char zPrintMode; 
  static unsigned char zLetter; 
 
public: 
  compleX(void): re(0), im(0) {} 
  compleX(const double real, const double imag=0): re(real), im(imag) {} 
  compleX(const compleX& z): re(z.re), im(z.im) {} 
 
  friend double    re(const compleX& z) {	// real part 
    return z.re; 
  } 
  friend double    im(const compleX& z) {	// imaginary part 
    return z.im; 
  } 
  friend double  real(const compleX& z) {	// real part 
    return z.re; 
  } 
  friend double  imag(const compleX& z) {	// imaginary part 
    return z.im; 
  } 
  friend double   mag(const compleX& z) {	// magnitude |z| 
    return sqrt(z.re*z.re + z.im*z.im); 
  } 
  friend double   arg(const compleX& z);	// argument 
  friend double   ang(const compleX& z) {	// angle 
    return arg(z); 
  } 
  friend double    ph(const compleX& z) {	// phase 
    return arg(z); 
  } 
  friend compleX conj(const compleX& z) {	// complex conjugate 
    return compleX(z.re, -z.im); 
  } 
  friend double  norm(const compleX& z) {	// norm 
    return z.re*z.re + z.im*z.im; 
  } 
 
  friend compleX rtop(double x,   double y=0); 
  friend compleX ptor(double mag, double angle=0); 
  compleX& topolar(void); 
  compleX& torect(void); 
 
  void operator = (const compleX& z) {		// z1 = z2 
    re = z.re; 
    im = z.im; 
  } 
  compleX& operator += (const compleX& z) {	// z1 += z2 
    re += z.re; 
    im += z.im; 
    return *this; 
  } 
  compleX& operator -= (const compleX& z) {	// z1 -= z2 
    re -= z.re; 
    im -= z.im; 
    return *this; 
  } 
  compleX& operator *= (const compleX& z) {	// z1 *= z2 
    *this = *this * z; 
    return *this; 
  } 
  compleX& operator /= (const compleX& z) {	// z1 /= z2 
    *this = *this / z; 
    return *this; 
  } 
  compleX operator + (void) const {		// +z1 
    return *this; 
  } 
  compleX operator - (void) const {		// -z1 
    return compleX(-re, -im); 
  } 
 
  friend compleX operator + (const compleX& z1, const compleX& z2) { 
    return compleX(z1.re + z2.re, z1.im + z2.im); 
  } 
  friend compleX operator + (const compleX& z, const double x) { 
    return compleX(z.re+x, z.im); 
  } 
  friend compleX operator + (const double x, const compleX& z) { 
    return compleX(z.re+x, z.im); 
  } 
  friend compleX operator - (const compleX& z1, const compleX& z2) { 
    return compleX(z1.re - z2.re, z1.im - z2.im); 
  } 
  friend compleX operator - (const compleX&, const double); 
  friend compleX operator - (const double x, const compleX& z) { 
    return compleX(x-z.re, -z.im); 
  } 
  friend compleX operator * (const compleX& z1, const compleX& z2) { 
    double re = z1.re*z2.re - z1.im*z2.im; 
    double im = z1.re*z2.im + z1.im*z2.re; 
    return compleX(re, im); 
  } 
  friend compleX operator * (const compleX& z, const double x) { 
    return compleX(z.re*x, z.im*x); 
  } 
  friend compleX operator * (const double x, const compleX& z) { 
    return compleX(z.re*x, z.im*x); 
  } 
  friend compleX operator / (const compleX&, const compleX&); 
  friend compleX operator / (const compleX& z, const double x) { 
    return compleX(z.re/x, z.im/x); 
  } 
  friend compleX operator / (const double, const compleX&); 
  friend compleX operator ^ (const compleX& z1, const compleX& z2) { 
    return pow(z1, z2); 
  } 
 
  friend int operator == (const compleX& z1, const compleX& z2) { 
    return (z1.re == z2.re) && (z1.im == z2.im); 
  } 
  friend int operator != (const compleX& z1, const compleX& z2) { 
    return (z1.re != z2.re) || (z1.im != z2.im); 
  } 
 
  friend double   abs(const compleX& z);
  friend compleX sqrt(const compleX& z);
  friend compleX sqr(const compleX& z); 
  friend compleX pow(const compleX& base, const compleX& exp); 
  friend compleX pow(const compleX& base, const double   exp); 
  friend compleX pow(const double base, const compleX& exp);
  friend compleX mix(const compleX& val1, const compleX& val2, const float t); 
 
  friend compleX   exp(const compleX& z); 
  friend compleX   log(const compleX& z); 
  friend compleX    ln(const compleX& z); 
  friend compleX log10(const compleX& z); 
 
  friend compleX  cos(const compleX& z); 
  friend compleX  sin(const compleX& z); 
  friend compleX  tan(const compleX& z); 
 
  friend compleX asin(const compleX& z); 
  friend compleX acos(const compleX& z); 
  friend compleX atan(const compleX& z); 
 
  friend compleX sinh(const compleX& z); 
  friend compleX cosh(const compleX& z); 
  friend compleX tanh(const compleX& z);

  void SetArgMode(unsigned char mode) const { 
    if(mode == Z_RADIANS || mode == Z_DEGREES) 
      zArgMode = mode; 
  } 
  void SetPrintMode(unsigned char mode) const { 
    if(mode == Z_COMMA || mode == Z_LETTER) 
      zPrintMode = mode; 
  } 
  void SetLetter(unsigned char letter) const { 
    zLetter = letter; 
  } 
 
  //friend ostream& operator<<(ostream&, const compleX&); 
  //friend istream& operator>>(istream&, const compleX&); 
};

static const compleX Z0(0, 0);		// compleX number 0 
static const compleX Z1(1, 0);		// compleX number 1 
static const compleX Zi(0, 1);		// compleX number i 
static const compleX Zinf(HUGE_VAL, HUGE_VAL); // compleX number infinity 
//static const compleX Complex; 
 
struct Vec3complex
{	
	compleX X;
	compleX Y;
	compleX Z;

	Vec3 Re()
	{		return Vecc3(float(X.re), float(Y.re), float(Z.re));	}
	Vec3 Im()
	{		return Vecc3(float(X.im), float(Y.im), float(Z.im));	}
	Vec3d ReD()
	{		return Vecc3d(X.re, Y.re, Z.re);	}
};

struct Vec2complex
{	compleX X;
	compleX Y;

	Vec2 Re()
	{		return Vecc2(float(X.re), float(Y.re));	}
};


compleX isotropicZ(const Vec3complex& c0, const Vec3complex& c1, float fSign);
compleX isotropicY(const Vec2complex& c0, const Vec2complex& c1, float fSign);

#endif
