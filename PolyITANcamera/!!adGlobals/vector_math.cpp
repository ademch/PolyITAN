#include <stdio.h>
#include <assert.h>

#include <iostream>
#include <string.h>
#include "vector_math.h"

inline float sqr(float a) {return a*a;}
inline double sqr(double a) {return a*a;}

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

// Artificial geometry pipeline --------------------------------------------------------------

Matr4 mCpuPipeline;


void cpuRotatef(float angle, float x, float y, float z)
{
Matr4 mRot = Mat4MakeRot(angle, Vecc3(x,y,z));

	mCpuPipeline = mCpuPipeline * mRot;
}

void cpuTranslatef(float x, float y, float z)
{
Matr4 mTrans;
	
	mTrans = Mat4MakeTrans(x, y, z);	

	mCpuPipeline = mCpuPipeline * mTrans;
}

void cpuScalef(float x, float y, float z)
{
Matr4 mScale;

	mScale = Mat4MakeScale(x, y, z);	

	mCpuPipeline = mCpuPipeline * mScale;
}


void cpuMultMatrixf(float* m)
{
Matr4 u;

	memcpy(u.m, m, sizeof(Matr4));
	mCpuPipeline = mCpuPipeline * u;
}

void cpuLoadIdentity()
{
	mCpuPipeline = Mat4MakeIdent();
}

void cpuVertex3fv(float *v)
{
Vec3 vGPU;
	
	memcpy(&vGPU, v, sizeof(Vec3));
	vGPU = mCpuPipeline * vGPU;
	//glVertex3fv(&vGPU.X);
}

Vec3 cpuPipelineVertex3fv(float* v)
{
Vec3 vCPU;

	memcpy(&vCPU, v, sizeof(Vec3));
	vCPU = mCpuPipeline * vCPU;

return vCPU;
}


// Vector operators --------------------------------------------------------------

Vec2 Vecc2(float x,float y)
{
Vec2 u;	
	u.X = x; 	u.Y = y;

return u;
}	

VecUB4 VeccUB4(unsigned char R,unsigned char G,unsigned char B,unsigned char A)
{
VecUB4 c;
	c.R = R; 	c.G = G; 	c.B = B;	c.A = A;

return c;
}

VecUB3 VeccUB3(unsigned char R,unsigned char G,unsigned char B)
{
VecUB3 c;
	c.R = R; 	c.G = G; 	c.B = B;

	return c;
}	

Vec3 Vecc3(float x,float y,float z)
{
Vec3 u;	
	u.X = x; 	u.Y = y; 	u.Z = z;

return u;
}			   

Vec3 Vecc3(Vec3d vec)
{	return Vecc3((float)vec.X, (float)vec.Y, (float)vec.Z);}			   

Vec3d Vecc3d(double x,double y,double z)
{
Vec3d u;	
	u.X = x; 	u.Y = y; 	u.Z = z;

return u;
}			   

Vec3d Vecc3d(Vec3 v)
{
	return Vecc3d(v.X, v.Y, v.Z);
}			   

Vec4 Vecc4(float x,float y,float z,float w)
{
Vec4 u;	
	u.X = x;	u.Y = y;	u.Z = z;	u.W = w;

return u;
}

Vec4 Vecc4(Vec3 v3, float w)
{
Vec4 u;	
	u.X = v3.X;	u.Y = v3.Y;	u.Z = v3.Z;	u.W = w;

return u;
}

Vec3 Vecc3(Vec2 v, float z)
{
	Vec3 u;
	u.X = v.X;	u.Y = v.Y;	u.Z = z;

	return u;
}

Vec4 Vecp4(float a[4])
{
	Vec4 u = Vecc4(a[0],a[1],a[2],a[3]);
	return u;
}

Matr4 Matrc4(Vec4 r0, Vec4 r1, Vec4 r2, Vec4 r3)
{
Matr4 u;

	u.m[0][0] = r0.X; u.m[0][1] = r0.Y; u.m[0][2] = r0.Z; u.m[0][3] = r0.W;
	u.m[1][0] = r1.X; u.m[1][1] = r1.Y; u.m[1][2] = r1.Z; u.m[1][3] = r1.W;
	u.m[2][0] = r2.X; u.m[2][1] = r2.Y; u.m[2][2] = r2.Z; u.m[2][3] = r2.W;
	u.m[3][0] = r3.X; u.m[3][1] = r3.Y; u.m[3][2] = r3.Z; u.m[3][3] = r3.W;
	
return u;
}


float VecLength(const Vec3& v)
{    return sqrt(sqr(v.X) + sqr(v.Y)+ sqr(v.Z));  }

float VecLength(const Vec2& v)
{	return sqrt(sqr(v.X) + sqr(v.Y));  }

double VecLength(const Vec3d& v)
{    return sqrt(sqr(v.X) + sqr(v.Y)+ sqr(v.Z));  }

float VecLengthSqr(const Vec3& v)
{    return sqr(v.X) + sqr(v.Y)+ sqr(v.Z);  }

float VecLengthSqr(const Vec2& v)
{	return sqr(v.X) + sqr(v.Y);  }

double VecLengthSqr(const Vec3d& v)
{    return sqr(v.X) + sqr(v.Y)+ sqr(v.Z);  }

float PointDist(Vec3 pt1, Vec3 pt2)
{    return sqrt(sqr(pt1.X-pt2.X) + sqr(pt1.Y-pt2.Y)+ sqr(pt1.Z-pt2.Z));  }

float PointDist(Vec2 pt1, Vec2 pt2)
{	return sqrt(sqr(pt1.X - pt2.X) + sqr(pt1.Y - pt2.Y));  }

double PointDist(Vec3d pt1, Vec3d pt2)
{    return sqrt(sqr(pt1.X-pt2.X) + sqr(pt1.Y-pt2.Y)+ sqr(pt1.Z-pt2.Z));  }

float PointDistSqr(Vec3 pt1, Vec3 pt2)
{    return sqr(pt1.X-pt2.X) + sqr(pt1.Y-pt2.Y)+ sqr(pt1.Z-pt2.Z);  }

float PointDistSqr(Vec2 pt1, Vec2 pt2)
{	return sqr(pt1.X - pt2.X) + sqr(pt1.Y - pt2.Y); }


Vec3 VecMix(Vec3 v1, Vec3 v2, float t)
{    return v1*(1.0f-t) + v2*t;  }

VecUB4 VecMix(VecUB4 v1, VecUB4 v2, float t)
{    return v1*(1.0f-t) + v2*t;  }

Vec3 VecClampOne(Vec3 v)
{
Vec3 v_res = v;
	if (v_res.X > 1.0) v_res.X = 1.0;
	if (v_res.Y > 1.0) v_res.Y = 1.0;
	if (v_res.Z > 1.0) v_res.Z = 1.0;
return v_res;
}

Vec3 VecClampBYTE(Vec3 v)
{
	Vec3 v_res = v;
	if (v_res.X > 255.0) v_res.X = 255.0;
	if (v_res.Y > 255.0) v_res.Y = 255.0;
	if (v_res.Z > 255.0) v_res.Z = 255.0;
return v_res;
}


Vec3 VecOrtho(const Vec3& v)
{
Vec3 y;

	if (v.X != 0.0)
	{   y.X = -v.Y;
		y.Y =  v.X;
 
	} else if (v.Y != 0.0)
	{   y.Y = -v.Z;
		y.Z =  v.Y;

	} else if (v.Z != 0.0)
	{   y.X = -v.Z;
	}

return y;
}

Vec2 VecNormalize(const Vec2& v)
{
float d = VecLength(v);
	
	if (d < 10e-6) d=0.0001f;
	
return v/d;
}

Vec3 VecNormalize(const Vec3& v)
{
float d = VecLength(v);
	
	if (d < 10e-6) d=0.0001f;
	
return v/d;
}

Vec3d VecNormalize(const Vec3d& v)
{
double d = VecLength(v);

	if (d < 10e-12) d=0.0000001f;

return v/d;
}

void VecNormalizeP(Vec3& v)
{
float d = VecLength(v);

	if (d < 10e-6) d=0.0001f;

	v = v/d;
}


Vec3 VecReflect(Vec3 vInput, Vec3 vNormal)
{	return vInput - 2.0f*(vNormal^vInput)*vNormal;
}

Vec3d VecReflect(Vec3d vInput, Vec3d vNormal)
{	return vInput - 2.0f*(vNormal^vInput)*vNormal;
}

Vec3 VecTranslate(Vec3 v, const float x, const float y, const float z)
{	return Mat4MakeTrans(x,y,z)*v;
}


Vec2 VecRotate(Vec2 v, float angle)
{
	return Vecc2( cos(angle)*v.X - sin(angle)*v.Y,
		          sin(angle)*v.X + cos(angle)*v.Y );
}

// Return value range [0, PI]
float AngleBetweenVectors(Vec2 vec1, Vec2 vec2)
{
	vec1 = VecNormalize(vec1);
	vec2 = VecNormalize(vec2);
	return acos(vec1^vec2);
}

Vec3 TriangleLinearInterpolate(Vec3 v0, Vec3 v1, Vec3 v2, float u, float v)
{
	assert(u<1);	assert(u>0);
	assert(v<1);	assert(v>0);

Vec3 Ra = v0*(1-u) + v1*u;
Vec3 Rb = v2*(1-u) + v1*u;

return Ra*(1-v) + Rb*v;
}

Vec2 operator-(Vec2 v1, Vec2 v2) { return Vecc2(v1.X - v2.X, v1.Y - v2.Y); }


Vec3 operator+(Vec3 v1,Vec3 v2)   {return Vecc3(v1.X + v2.X, v1.Y + v2.Y, v1.Z + v2.Z);}
Vec2 operator+(Vec2 v1,Vec2 v2)   {return Vecc2(v1.X + v2.X, v1.Y + v2.Y); }
Vec3 operator-(Vec3 v1,Vec3 v2)   {return Vecc3(v1.X - v2.X, v1.Y - v2.Y, v1.Z - v2.Z);}
Vec3 operator-(Vec3 v)            {return Vecc3(-v.X, -v.Y, -v.Z);}
Vec3 operator*(Vec3 v1,float s)   {return Vecc3(v1.X*s, v1.Y*s, v1.Z*s);}
Vec2 operator*(Vec2 v1, float s)  {return Vecc2(v1.X*s, v1.Y*s); }
Vec3 operator*(float s, Vec3 v1)  {return Vecc3(v1.X*s, v1.Y*s, v1.Z*s);}
Vec2 operator*(float s, Vec2 v1)  { return Vecc2(v1.X*s, v1.Y*s); }
Vec3 operator/(Vec3 v,float s)    {return v*(1.0f/s);}
Vec2 operator/(Vec2 v, float s)   {return v*(1.0f/s);}
float operator^(Vec3 v1,Vec3 v2)  {return v1.X*v2.X + v1.Y*v2.Y + v1.Z*v2.Z;} // DOT product
float operator^(Vec2 v1,Vec2 v2)  {return v1.X*v2.X + v1.Y*v2.Y;}             // DOT product
Vec3 operator*(Vec3 v1,Vec3 v2)	  // CROSS product
{    return Vecc3(
				v1.Y * v2.Z - v1.Z*v2.Y,
				v1.Z * v2.X - v1.X*v2.Z,
				v1.X * v2.Y - v1.Y*v2.X );
}

Vec3d operator+(Vec3d v1,Vec3d v2)   {return Vecc3d(v1.X + v2.X, v1.Y + v2.Y, v1.Z + v2.Z);}
Vec3d operator-(Vec3d v1,Vec3d v2)   {return Vecc3d(v1.X - v2.X, v1.Y - v2.Y, v1.Z - v2.Z);}
Vec3d operator-(Vec3d v)             {return Vecc3d(-v.X, -v.Y, -v.Z);}
Vec3d operator*(Vec3d v1,double s)   {return Vecc3d(v1.X*s, v1.Y*s, v1.Z*s);}
Vec3d operator*(double s, Vec3d v1)  {return Vecc3d(v1.X*s, v1.Y*s, v1.Z*s);}
Vec3d operator/(Vec3d v,double s)    {return v*(1.0/s);}
double operator^(Vec3d v1,Vec3d v2)  {return v1.X*v2.X + v1.Y*v2.Y + v1.Z*v2.Z;}
Vec3d operator*(Vec3d v1,Vec3d v2)	  // CROSS product
{    return Vecc3d(
				  v1.Y * v2.Z - v1.Z*v2.Y,
				  v1.Z * v2.X - v1.X*v2.Z,
				  v1.X * v2.Y - v1.Y*v2.X );
}

VecUB4 operator+(VecUB4 v1,VecUB4 v2)   {return VeccUB4(v1.R + v2.R, v1.G + v2.G, v1.B + v2.B, v1.A + v2.A);}
VecUB4 operator*(VecUB4 v1,double s)    {return VeccUB4((unsigned char)v1.R*s,
														(unsigned char)v1.G*s,
														(unsigned char)v1.B*s,
														(unsigned char)v1.A*s);}


Vec3 operator*(Matr4 m, Vec3 v)
{
Vec3 u;
Vec4 u4;
Vec4 v4 = Vecc4(v, 1.0);

	u4.X = v4.X * m.m[0][0] + v4.Y * m.m[1][0] + v4.Z * m.m[2][0] + v4.W * m.m[3][0];
	u4.Y = v4.X * m.m[0][1] + v4.Y * m.m[1][1] + v4.Z * m.m[2][1] + v4.W * m.m[3][1];
	u4.Z = v4.X * m.m[0][2] + v4.Y * m.m[1][2] + v4.Z * m.m[2][2] + v4.W * m.m[3][2];
	u4.W = v4.X * m.m[0][3] + v4.Y * m.m[1][3] + v4.Z * m.m[2][3] + v4.W * m.m[3][3];

	u = Vecc3(u4.X,u4.Y,u4.Z);
	u = u/u4.W;

return u;
}

Vec3 operator*(Vec3 v, Matr4 m)
{
	Vec3 u;
	Vec4 u4;
	Vec4 v4 = Vecc4(v, 1.0);

	u4.X = v4.X * m.m[0][0] + v4.X * m.m[1][0] + v4.X * m.m[2][0] + v4.X * m.m[3][0];
	u4.Y = v4.Y * m.m[0][1] + v4.Y * m.m[1][1] + v4.Y * m.m[2][1] + v4.Y * m.m[3][1];
	u4.Z = v4.Z * m.m[0][2] + v4.Z * m.m[1][2] + v4.Z * m.m[2][2] + v4.Z * m.m[3][2];
	u4.W = v4.W * m.m[0][3] + v4.W * m.m[1][3] + v4.W * m.m[2][3] + v4.W * m.m[3][3];

	u = Vecc3(u4.X, u4.Y, u4.Z);
	u = u / u4.W;
	return u;
}

Vec3 operator^(Matr4 m, Vec3 v)
{
Vec3 u;

	u.X = v.X * m.m[0][0] + v.Y * m.m[1][0] + v.Z * m.m[2][0];
	u.Y = v.X * m.m[0][1] + v.Y * m.m[1][1] + v.Z * m.m[2][1];
	u.Z = v.X * m.m[0][2] + v.Y * m.m[1][2] + v.Z * m.m[2][2];

	return u;
}

Vec3d operator^(Matr4d m, Vec3d v)
{
Vec3d u;

	u.X = v.X * m.m[0][0] + v.Y * m.m[1][0] + v.Z * m.m[2][0];
	u.Y = v.X * m.m[0][1] + v.Y * m.m[1][1] + v.Z * m.m[2][1];
	u.Z = v.X * m.m[0][2] + v.Y * m.m[1][2] + v.Z * m.m[2][2];

	return u;
}


Matr4 operator*(const Matr4& m1, const Matr4& m2)
{
unsigned char j;
Matr4 u;

	for ( j = 0 ; j < 4 ; j++ )
	{
		u.m[0][j] = m2.m[0][0] * m1.m[0][j] + m2.m[0][1] * m1.m[1][j] + m2.m[0][2] * m1.m[2][j] + m2.m[0][3] * m1.m[3][j];
		u.m[1][j] = m2.m[1][0] * m1.m[0][j] + m2.m[1][1] * m1.m[1][j] + m2.m[1][2] * m1.m[2][j] + m2.m[1][3] * m1.m[3][j];
		u.m[2][j] = m2.m[2][0] * m1.m[0][j] + m2.m[2][1] * m1.m[1][j] + m2.m[2][2] * m1.m[2][j] + m2.m[2][3] * m1.m[3][j];
		u.m[3][j] = m2.m[3][0] * m1.m[0][j] + m2.m[3][1] * m1.m[1][j] + m2.m[3][2] * m1.m[2][j] + m2.m[3][3] * m1.m[3][j];
	}

return u;
}

/**
* Tests if the given plane and ray intersect with each other.
*
*  @param ray - the Ray
*  @param plane - the Plane
*  @param t - t gives you the intersection point:
*         isect_point = ray.origin + ray.dir * t
*
*  @return true if the ray intersects the plane.
*  @note If ray is parallel to plane: t=0, ret:true -> on plane, ret:false -> No hit
*/
bool intersectRayPlane( const Vec3 ptPlane, const Vec3 vnPlane,
						const Vec3 ray_orig,const Vec3 ray_dir, float& t )
{
const float EPS = 0.00001f;
float plane_offset;

	// t = -(n·P + d)
	float denom = vnPlane ^ ray_dir;

	// All points on the plane satisfy the equation dot(Pt,Normal) = offset
	// (normal is assumed to be normalized)
	// Note that plane_offset = -D (neg dist from the origin).
	plane_offset = ptPlane ^ vnPlane;
	if (abs(denom) < EPS)
	{	// Ray parallel to plane
		t = 0;
		
		if ( (vnPlane ^ ray_orig) - plane_offset  < EPS)     // Test for ray on plane
			return true;
		else
			return false;
	}
	t = ( vnPlane ^ (vnPlane * plane_offset - ray_orig) ) / denom;

	return (float)0 <= t;
}


//Vec4 planelineintersection(Vec4 n,float d,Vec4 p1,Vec4 p2){
//	// returns the point where the line p1-p2 intersects the plane n&d
//        Vec4 dif  = p2-p1;
//        float dn= n^dif;
//        float t = -(d+(n^p1) )/dn;
//        return p1 + (dif*t);
//}
//int concurrent(Vec4 a,Vec4 b) {
//	return(a.x==b.x && a.y==b.y && a.z==b.z);
//}
//


Matr4 MatrTranspose(Matr4 m)
{	return Matrc4(	Vecc4(m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0]),
					Vecc4(m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1]),
					Vecc4(m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2]),
					Vecc4(m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]));
}
//Vec4 operator*(matr4 m,Vec4 v)
//{
//	m=transpose(m); // since column ordered
//	return Vec4(m.x^v,m.y^v,m.z^v,m.w^v);
//}
//matr4 operator*(matr4 m1,matr4 m2)
//{
//	m1=transpose(m1);
//	return matr4(m1*m2.x,m1*m2.y,m1*m2.z,m1*m2.w);
//}
//
//float dot_prod3(Vec4 v1, Vec4 v2, bool two_side)
//{
//	float dp3 = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
//	if (dp3 <=0)
//		if (two_side)
//			dp3 = -dp3;
//		else dp3 = 0.;
//
//return dp3;
//}


void normalCalcPackSmooth(TriVertex* v0,TriVertex* v1, TriVertex* v2)
{
float x10,y10,z10,x12,y12,z12;
float cpx,cpy,cpz;
float r;
Vec3 ar;

	/* Compute edge vectors */
	x10 = v1->v.X - v0->v.X;
	y10 = v1->v.Y - v0->v.Y;
	z10 = v1->v.Z - v0->v.Z;
	x12 = v1->v.X - v2->v.X;
	y12 = v1->v.Y - v2->v.Y;
	z12 = v1->v.Z - v2->v.Z;

	/* Compute the cross product */
	cpx = (z10 * y12) - (y10 * z12);
	cpy = (x10 * z12) - (z10 * x12);
	cpz = (y10 * x12) - (x10 * y12);

	/* Normalize the result to get the unit-length facet normal */
	r = sqrt(cpx * cpx + cpy * cpy + cpz * cpz);
	if (r < 10e-6)
	{
		ar=Vecc3(0.0,0.0,0.0);
		return;
	}
	ar = Vecc3(-cpx/r, -cpy/r, -cpz/r);

	v0->n = v0->n + ar;
	v1->n = v1->n + ar;
	v2->n = v2->n + ar;

	VecNormalizeP( v0->n );
	VecNormalizeP( v1->n );
	VecNormalizeP( v2->n );
}


void normalCalcPack(TriVertex* v0,TriVertex* v1, TriVertex* v2)
{
float x10,y10,z10,x12,y12,z12;
float cpx,cpy,cpz;
float r;
float ar[3];

	/* Compute edge vectors */
	x10 = v1->v.X - v0->v.X;
	y10 = v1->v.Y - v0->v.Y;
	z10 = v1->v.Z - v0->v.Z;
	x12 = v1->v.X - v2->v.X;
	y12 = v1->v.Y - v2->v.Y;
	z12 = v1->v.Z - v2->v.Z;

	/* Compute the cross product */
	cpx = (z10 * y12) - (y10 * z12);
	cpy = (x10 * z12) - (z10 * x12);
	cpz = (y10 * x12) - (x10 * y12);

	/* Normalize the result to get the unit-length facet normal */
	r = sqrt(cpx * cpx + cpy * cpy + cpz * cpz);
	if (r < 10e-6)
	{
		ar[0]=ar[1]=ar[2]=0.0;
		return;
	}
	ar[X] = -cpx / r;
	ar[Y] = -cpy / r;
	ar[Z] = -cpz / r;

	v0->n.X = v1->n.X = v2->n.X = ar[X];
	v0->n.Y = v1->n.Y = v2->n.Y = ar[Y];
	v0->n.Z = v1->n.Z = v2->n.Z = ar[Z];
}


Matr4 Mat4MakeIdent()
{
unsigned char i;
Matr4 u;

	memset(&u, 0, sizeof(u));
	for (i=0;i<4;i++)
		u.m[i][i] = 1.0;

return u;
}

Matr4 Mat4MakeTrans( const float x, const float y, const float z )
{
Matr4 u;
	u = Mat4MakeIdent();

	u.m[3][0] = x;
	u.m[3][1] = y;
	u.m[3][2] = z;

return u;
}

Matr4 Mat4MakeScale( const float x_scale, const float y_scale, const float z_scale )
{
Matr4 u;
	u = Mat4MakeIdent();

	u.m[0][0] = x_scale;
	u.m[1][1] = y_scale;
	u.m[2][2] = z_scale;

	return u;
}

Matr4 Mat4MakeRot( const float fAngleDeg, const Vec3& axis )
{
Matr4 u;
Vec3 an = VecNormalize(axis);
float fAngleRad = fAngleDeg * DEGREES_TO_RADIANS;
float s = (float) sin( fAngleRad );
float c = (float) cos( fAngleRad );
float t = 1.0f - c;

	u.m[0][0] = t * an.X * an.X + c;
	u.m[0][1] = t * an.X * an.Y + s * an.Z;
	u.m[0][2] = t * an.X * an.Z - s * an.Y;
	u.m[0][3] = 0.0;

	u.m[1][0] = t * an.Y * an.X - s * an.Z;
	u.m[1][1] = t * an.Y * an.Y + c;
	u.m[1][2] = t * an.Y * an.Z + s * an.X;
	u.m[1][3] = 0.0;

	u.m[2][0] = t * an.Z * an.X + s * an.Y;
	u.m[2][1] = t * an.Z * an.Y - s * an.X;
	u.m[2][2] = t * an.Z * an.Z + c;
	u.m[2][3] = 0.0;

	u.m[3][0] = 0.0;
	u.m[3][1] = 0.0;
	u.m[3][2] = 0.0;
	u.m[3][3] = 1.0;

return u;
}


Matr4d Mat4dMakeRot( const double fAngleDeg, const Vec3d& axis )
{
Matr4d u;
Vec3d an = VecNormalize(axis);
double temp_angle = fAngleDeg * DEGREES_TO_RADIANS ;
double s = (double) sin ( temp_angle ) ;
double c = (double) cos ( temp_angle ) ;
double t = 1.0f - c ;

	u.m[0][0] = t * an.X * an.X + c ;
	u.m[0][1] = t * an.X * an.Y + s * an.Z ;
	u.m[0][2] = t * an.X * an.Z - s * an.Y ;
	u.m[0][3] = 0.0;

	u.m[1][0] = t * an.Y * an.X - s * an.Z ;
	u.m[1][1] = t * an.Y * an.Y + c ;
	u.m[1][2] = t * an.Y * an.Z + s * an.X ;
	u.m[1][3] = 0.0;

	u.m[2][0] = t * an.Z * an.X + s * an.Y ;
	u.m[2][1] = t * an.Z * an.Y - s * an.X ;
	u.m[2][2] = t * an.Z * an.Z + c ;
	u.m[2][3] = 0.0;

	u.m[3][0] = 0.0;
	u.m[3][1] = 0.0;
	u.m[3][2] = 0.0;
	u.m[3][3] = 1.0;
return u;
}

//#define CULLING_ENABLED

// Tests if the given triangle and ray intersect with each other
//
// We present a clean algorithm for determining whether a ray intersects a triangle.
// The algorithm translates the origin of the ray and then changes the base to yield
// a vector (t u v)T, where t is the distance to the plane in which the triangle lies
// and (u,v) represents the coordinates inside the triangle.
// One advantage of this method is that the plane equation need not be computed on the fly nor
// be stored, which can amount to significant memory savings for triangle meshes.
// As we found our method to be comparable in speed to previous methods, we believe it is the
// fastest ray-triangle intersection routine for triangles that do not have precomputed plane equations.
//
//param v0,v1,v2 - the triangle (ccw ordering)
//param ray_orig - the ray origin
//param ray_dir - the ray direction
//param u,v - tangent space u/v coordinates of the intersection
//			  U,V coordinates: v0(0,0); v1(1,0); v2(0,1)
//param t - an indicator of the intersection location
//
//post t gives you the intersection point: isect = ray.dir * t + ray.origin
//return true if the ray intersects the triangle
//see from http://www.acm.org/jgt/papers/MollerTrumbore97/code.html
bool intersectRayTriangle( Vec3 v0, Vec3 v1, Vec3 v2, const Vec3 ray_orig, const Vec3 ray_dir,
						   float& u, float& v, float& t )
{
const float EPSILON = 0.00001f;
Vec3 edge1, edge2, tvec, pvec, qvec;
float det,inv_det;

	/* find vectors for two edges sharing vert0 */
	edge1 = v1 - v0;
	edge2 = v2 - v0;

	/* begin calculating determinant - also used to calculate U parameter */
	pvec = ray_dir * edge2;

	/* if determinant is near zero, ray lies in plane of triangle */
	det = edge1 ^ pvec;

#ifdef CULLING_ENABLED           /* define CULLING_ENABLED to calculate intersections only with CW triangles from view of ray origin */
	if (det < EPSILON)
		return false;

	/* calculate distance from vert0 to ray origin */
	tvec = ray_orig - v0;

	/* calculate U parameter and test bounds */
	u = tvec ^ pvec;
	if (u < 0.0 || u > det)
		return false;

	/* prepare to test V parameter */
	qvec = tvec*edge1;

	/* calculate V parameter and test bounds */
	v = ray_dir ^ qvec;
	if (v < 0.0 || u + v > det)
		return false;

	/* calculate t, scale parameters, ray intersects triangle */
	t = edge2 ^ qvec;
	inv_det = 1.0f / det;
	t *= inv_det;
	u *= inv_det;
	v *= inv_det;

	// test if t is within the ray boundary (when t >= 0)
	return t >= (float)0;

#else
	if (det > -EPSILON && det < EPSILON)
		return false;
	inv_det = 1.0f / det;

	/* calculate distance from vert0 to ray origin */
	tvec = ray_orig - v0;

	/* calculate U parameter and test bounds */
	u = (tvec ^ pvec) * inv_det;
	if ((u < 0.0) || (u > 1.0))
		return false;

	/* prepare to test V parameter */
	qvec = tvec*edge1;

	/* calculate V parameter and test bounds */
	v = (ray_dir ^ qvec) * inv_det;
	if ((v < 0.0) || (u + v > 1.0))
		return false;

	/* calculate t, scale parameters, ray intersects triangle */
	t = (edge2 ^ qvec) * inv_det;

	// test if t is within the ray boundary (when t >= 0)
	return t >= (float)0;
#endif

return true;
}


bool intersectAABoxRay(Vec3 box_ptMin, Vec3 box_ptMax,
					   Vec3 ray_orig,  Vec3 ray_dir,
					   float& tIn, float& tOut)
{
tIn  = -10e6;
tOut =  10e6;
float t0, t1;
const float epsilon = 0.000001f;

	// YZ plane
	if ( abs(ray_dir.X) < epsilon )
	{	// Ray parallel to plane
		if ( ray_orig.X < box_ptMin.X || ray_orig.X > box_ptMax.X )
			return false;
	}

	// XZ plane
	if ( abs(ray_dir.Y) < epsilon )
	{	// Ray parallel to plane
		if ( ray_orig.Y < box_ptMin.Y || ray_orig.Y > box_ptMax.Y )
			return false;
	}

	// XY plane
	if ( abs(ray_dir.Z) < epsilon )
	{	// Ray parallel to plane
		if ( ray_orig.Z < box_ptMin.Z || ray_orig.Z > box_ptMax.Z )
			return false;
	}

	// YZ plane
	t0 = (box_ptMin.X - ray_orig.X) / ray_dir.X;
	t1 = (box_ptMax.X - ray_orig.X) / ray_dir.X;

	if ( t0 > t1 )
		std::swap(t0, t1);

	if ( t0 > tIn )
		tIn = t0;

	if ( t1 < tOut )
		tOut = t1;

	if ( tIn > tOut || tOut < float(0) )
		return false;

	// XZ plane
	t0 = (box_ptMin.Y - ray_orig.Y) / ray_dir.Y;
	t1 = (box_ptMax.Y - ray_orig.Y) / ray_dir.Y;

	if ( t0 > t1 )
		std::swap(t0, t1);

	if ( t0 > tIn )
		tIn = t0;

	if ( t1 < tOut )
		tOut = t1;

	if ( tIn > tOut || tOut < float(0) )
		return false;

	// XY plane
	t0 = (box_ptMin.Z - ray_orig.Z) / ray_dir.Z;
	t1 = (box_ptMax.Z - ray_orig.Z) / ray_dir.Z;

	if ( t0 > t1 )
		std::swap(t0, t1);

	if ( t0 > tIn )
		tIn = t0;
	if ( t1 < tOut )
		tOut = t1;

	if ( tIn > tOut || tOut < float(0) )
		return false;

	return true;
}


/**
* intersect ray/sphere-shell
* note: after calling this, you can find the intersection point with: ray_orig + ray_dir * t
*
* @param sphere   the sphere to test
* @param ray      the ray to test
* @return returns intersection point in t, and the number of hits
				  (numhits, t0, t1 are undefined if return value is false)
*/
bool intersectRaySphere( Vec3 sphere_center, float sphere_radius,
						 Vec3 ray_orig, Vec3 ray_dir,
						 int& numhits, float& t0, float& t1 )
{
numhits = -1;

	// set up quadratic Q(t) = a*t^2 + 2*b*t + c
	const Vec3 offset = ray_orig - sphere_center;
	float a = VecLengthSqr( ray_dir );
	float b = offset ^ ray_dir;
	float c = VecLengthSqr( offset ) - sphere_radius * sphere_radius;

	// no intersection if Q(t) has no real roots
	const float discriminant = b * b - a * c;
	if (discriminant < 0.0f)
	{	numhits = 0;
		return false;
	}
	else if (discriminant > 0.0f)
	{
		float root = sqrt( discriminant );
		float invA = 1.0f / a;
		t0 = (-b - root) * invA;
		t1 = (-b + root) * invA;

		// assert: t0 < t1 since A > 0

		if (t0 >= 0.0f)
		{	numhits = 2;
			return true;
		}
		else if (t1 >= 0.0f)
		{	numhits = 1;
			t0 = t1;
			return true;
		}
		else
		{	numhits = 0;
			return false;
		}
	}
	else
	{
		t0 = -b / a;
		if (t0 >= 0.0f)
		{	numhits = 1;
			return true;
		}
		else
		{	numhits = 0;
			return false;
		}
	}
}


/** intersect LineSeg/Sphere-shell (not volume).
* does intersection on sphere surface, point inside sphere doesn't count as an intersection
* returns intersection point(s) in t
* find intersection point(s) with: ray.getOrigin() + ray.getDir() * t
* numhits, t0, t1 are undefined if return value is false
*/
bool intersectLineSegSphere( Vec3 sphere_center, float sphere_radius,
							 Vec3 lineSeg_orig, Vec3 lineSeg_end,
							 int& numhits, float& t0, float& t1)
{
Vec3 ray_dir = lineSeg_end - lineSeg_orig;

	if (intersectRaySphere( sphere_center, sphere_radius, lineSeg_orig,  ray_dir, numhits, t0, t1 ))
	{
		// throw out hits that are past 1 in segspace (off the end of the lineseg)
		while (0 < numhits && 1.0f < t0)
		{
			numhits--;
			t0 = t1;
		}
		if (2 == numhits && 1.0f < t1)
			numhits--;

		return 0 < numhits;
	}
	else
		return false;
}


float Determinant(float v1,float v2,float v3,float v4,float v5,float v6,float v7,float v8,float v9)
{	return v1*v5*v9 + v2*v6*v7 + v3*v4*v8 - v2*v4*v9 - v1*v6*v8 - v3*v5*v7;}

double Determinant(double v1,double v2,double v3,double v4,double v5,double v6,double v7,double v8,double v9)
{	return v1*v5*v9 + v2*v6*v7 + v3*v4*v8 - v2*v4*v9 - v1*v6*v8 - v3*v5*v7;}


void shadowMatrix(double shadowMat[4][4], double groundplane[4], double lightpos[4])
{
	double dot;

	/* find dot product between light position vector and ground plane normal */
	dot = groundplane[X] * lightpos[X] +
		  groundplane[Y] * lightpos[Y] +
		  groundplane[Z] * lightpos[Z] +
		  groundplane[W] * lightpos[W];

	shadowMat[0][0] = dot - lightpos[X] * groundplane[X];
	shadowMat[1][0] = 0.f - lightpos[X] * groundplane[Y];
	shadowMat[2][0] = 0.f - lightpos[X] * groundplane[Z];
	shadowMat[3][0] = 0.f - lightpos[X] * groundplane[W];

	shadowMat[X][1] = 0.f - lightpos[Y] * groundplane[X];
	shadowMat[1][1] = dot - lightpos[Y] * groundplane[Y];
	shadowMat[2][1] = 0.f - lightpos[Y] * groundplane[Z];
	shadowMat[3][1] = 0.f - lightpos[Y] * groundplane[W];

	shadowMat[X][2] = 0.f - lightpos[Z] * groundplane[X];
	shadowMat[1][2] = 0.f - lightpos[Z] * groundplane[Y];
	shadowMat[2][2] = dot - lightpos[Z] * groundplane[Z];
	shadowMat[3][2] = 0.f - lightpos[Z] * groundplane[W];

	shadowMat[X][3] = 0.f - lightpos[W] * groundplane[X];
	shadowMat[1][3] = 0.f - lightpos[W] * groundplane[Y];
	shadowMat[2][3] = 0.f - lightpos[W] * groundplane[Z];
	shadowMat[3][3] = dot - lightpos[W] * groundplane[W];
}

void findPlane(double plane[4], Vec3d v0, Vec3d v1, Vec3d v2)
{	findPlane(plane, &v0.X, &v1.X, &v2.X);
}

//plane- (out) plane coefficients
//v0,v1,v2- (input) points defining plane
void findPlane(double plane[4], double v0[3], double v1[3], double v2[3])
{
double vec0[3], vec1[3];

	/* need 2 vectors to find cross product */
	vec0[X] = v1[X] - v0[X];
	vec0[Y] = v1[Y] - v0[Y];
	vec0[Z] = v1[Z] - v0[Z];

	vec1[X] = v2[X] - v0[X];
	vec1[Y] = v2[Y] - v0[Y];
	vec1[Z] = v2[Z] - v0[Z];

	/* find cross product to get A, B, and C of plane equation */
	plane[A] = vec0[Y] * vec1[Z] - vec0[Z] * vec1[Y];
	plane[B] = -(vec0[X] * vec1[Z] - vec0[Z] * vec1[X]);
	plane[C] = vec0[X] * vec1[Y] - vec0[Y] * vec1[X];

	plane[D] = -(plane[A] * v0[X] + plane[B] * v0[Y] + plane[C] * v0[Z]);
}


void matrMult(double *matr, float &px,float &py,float &pz)
{
int i;
double cpx=0,cpy=0,cpz=0,w=0;
float coor[4];

	coor[0]=px;
	coor[1]=py;
	coor[2]=pz;
	coor[3]=1.;

	for (i=0;i<16;i+=4)
		cpx+=matr[i]*coor[int(i/4.0)];

	for (i=1;i<16;i+=4)
		cpy+=matr[i]*coor[int((i-1)/4.0)];

	for (i=2;i<16;i+=4)
		cpz+=matr[i]*coor[int((i-2)/4.0)];

	for (i=3;i<16;i+=4)
		w+=matr[i]*coor[int((i-3)/4.0)];

	px= float(cpx/w);py= float(cpy/w);pz= float(cpz/w);
}


void matrTransp(float* matr)
{
float trmatr[16]={0};
int i,j;

	for (i=0;i<16;i++)
		trmatr[i]=matr[i];

	j=0;
	for (i=0;i<16;i++)
	{
		matr[i]=trmatr[j];
		j+=4;
		if (j>15) j-=15;
	}
}

void normalize(float *coor)
{
float r;

	/* Normalize the result to get the unit-length facet normal */
	r = sqrt(coor[0] * coor[0] + coor[1] * coor[1] + coor[2] * coor[2]);
	if (r==0.0)
	{	coor[0]=coor[1]=coor[2]=0.0;
		return;
	}
	coor[0]/= r;
	coor[1]/= r;
	coor[2]/= r;
}



/*	Calculate the line segment PaPb that is the shortest route between
	two lines P1P2 and P3P4. Calculate also the values of mua and mub where
	Pa = P1 + mua (P2 - P1)
	Pb = P3 + mub (P4 - P3)
	Return FALSE if no solution exists. */
bool LineLineIntersect3D( Vec3d p1,Vec3d p2,Vec3d p3,Vec3d p4, Vec3d *pa,Vec3d *pb, double *mua, double *mub)
{
#define EPS 0.00001

Vec3d p13,p43,p21;
double d1343,d4321,d1321,d4343,d2121;
double numer,denom;

	p13 = p1 - p3;

	p43 = p4 - p3;
	if ((fabs(p43.X) < EPS) && (fabs(p43.Y) < EPS) && (fabs(p43.Z) < EPS))
		return false;

	p21 = p2 - p1;
	if ((fabs(p21.X) < EPS) && (fabs(p21.Y) < EPS) && (fabs(p21.X) < EPS))
		return false;

	d1343 = p13 ^ p43;
	d4321 = p43 ^ p21;
	d1321 = p13 ^ p21;
	d4343 = p43 ^ p43;
	d2121 = p21 ^ p21;

	denom = d2121 * d4343 - d4321 * d4321;
	if (fabs(denom) < EPS)
		return false;
	numer = d1343 * d4321 - d1321 * d4343;

	(*mua) = numer / denom;
	(*mub) = (d1343 + d4321 * (*mua)) / d4343;

	(*pa) = p1 + (*mua)*p21;
	(*pb) = p3 + (*mub)*p43;

return true;
}


// Given the line defined by the points A(Ax,Ay,Az) and B(Bx,By,Bz),
// and a point P(Px,Py,Pz) that is not on the line,
// find the coordinates of the point P'(P'x,P'y,P'z) which is on the line such that PP' is perpendicular onto the line AB.
//
// We can define the point P' as:
// P' = A + tAB
// where t is a parameter
//
// The dot product of perpendicular vectors is zero:
// AB • PP' = 0
// AB • <P' - P> = 0
// AB • <A + tAB - P> = 0
// AB • <A - P> + t || AB ||^2 = 0
// t || AB ||^2 = -AB • <A - P> 
// t || AB ||^2 = AB • <P - A> = AB • AP
//
// t = (AB • AP) / || AB ||^2
//
// Plug in the value for parameter t:
//P' = A + tAB
//P' = A + {(AB • AP) / || AB ||^2} AB
Vec3 PointLineProject(Vec3 pt, Vec3 ptLine, Vec3 vnLineDir)
{
	assert(fabs(VecLengthSqr(vnLineDir)-1.0) < 0.0001);

	Vec3 v = pt - ptLine;
	return ptLine + (vnLineDir^v)*vnLineDir;
}

Vec3 PointLineProject(Vec3 pt, Vec3 ptLine, Vec3 vnLineDir, bool& bPosProj)
{
Vec3 v;
float fDot;

	assert(fabs(VecLengthSqr(vnLineDir)-1.0) < 0.0001);

	v = pt - ptLine;
	fDot = vnLineDir^v;

	if (fDot > 0.0)
		bPosProj = true;
	else
		bPosProj = false;

	return ptLine + fDot*vnLineDir;
}

//-----------------------------------------------------------------------------
// Name: getRandomMinMax()
// Desc: Gets a random number between min/max boundaries
//-----------------------------------------------------------------------------
float getRandomMinMax( float fMin, float fMax )
{
	float fRandNum = (float)rand () / RAND_MAX;
	return fMin + (fMax - fMin) * fRandNum;
}

//-----------------------------------------------------------------------------
// Name: getRandomVector()
// Description: Generates a random vector on sphere
//-----------------------------------------------------------------------------
Vec3 getRandomVector( void )
{
Vec3 vVector;

	// Pick a random Z between -1.0f and 1.0f.
	vVector.Z = getRandomMinMax( -1.0f, 1.0f );

	// Get radius of this circle
	float radius = (float)sqrt(1.0 - vVector.Z * vVector.Z);

	// Pick a random point on a circle.
	float t = getRandomMinMax( -PI, PI );

	// Compute matching X and Y for our Z.
	vVector.X = (float)cosf(t) * radius;
	vVector.Y = (float)sinf(t) * radius;

return vVector;
}

Vec3 getRandomVector(Vec3 vInput, float fAngle )
{
Vec3 vVector;
Matr4 mRotvOrtho, mRotvInput;

	mRotvOrtho = Mat4MakeRot(getRandomMinMax( 0., fAngle ), VecOrtho(vInput));
	mRotvInput = Mat4MakeRot(getRandomMinMax( -180.0, 180.0 ), vInput);

	vVector = mRotvOrtho * vInput;
	vVector = mRotvInput * vVector;

return vVector;
}



// Parameters:
// pt- vector starting point
// vDir- direction of a vector
// ptPlane- point defining plane
// nPlane- plane normal
float VectorPlaneIntersect( Vec3 ptVector,Vec3 vDirVector,  Vec3 ptPlane,Vec3 nPlane )
{
#define EPS 0.00001
	
float fT, fN;
float t = 0;

	fN = vDirVector^nPlane;
	if ( fabs(fN) > EPS )
	{
		fT = (ptPlane-ptVector)^nPlane;
		t = fT/fN;
	}

	return t;
}

// Parameters:
// pt- vector starting point
// vDir- direction of a vector
// ptPlane- point defining plane
// nPlane- plane normal
double VectorPlaneIntersect( Vec3d ptVector,Vec3d vDirVector,  Vec3d ptPlane,Vec3d nPlane )
{
#define EPS 0.00001

double fT, fN;
double t = 0;

	fN = vDirVector^nPlane;
	if ( fabs(fN) > EPS )
	{
		fT = (ptPlane-ptVector)^nPlane;
		t = fT/fN;
	}

	return t;
}


Vec3 SphereCoordinatesToCartesian(float fTheta, float fPhi, float fR)
{
	return Vecc3(fR*sin(fTheta)*cos(fPhi), fR*sin(fTheta)*sin(fPhi), fR*cos(fTheta));
}

//
// vAngles.X - fTheta
// vAngles.Y - fPhi
Vec3 SphereCoordinatesToCartesian(Vec2 vAngles, float fR)
{
	return Vecc3(fR*sin(vAngles.X)*cos(vAngles.Y), fR*sin(vAngles.X)*sin(vAngles.Y), fR*cos(vAngles.X));
}

unsigned char Clamp(int val)
{
	if (val > 255) return 255;
	if (val < 0) return 0;

	return val;
}

unsigned char ClampF(float val)
{
	if (val > 255.0) return 255;
	else if (val < 0.0) return 0;

	return (unsigned char) val;
}


bool gluInvertMatrix(const float m[16], float invOut[16])
{
	double inv[16], det;
	int i;

	inv[0] = m[5]  * m[10] * m[15] - 
		m[5]  * m[11] * m[14] - 
		m[9]  * m[6]  * m[15] + 
		m[9]  * m[7]  * m[14] +
		m[13] * m[6]  * m[11] - 
		m[13] * m[7]  * m[10];

	inv[4] = -m[4]  * m[10] * m[15] + 
		m[4]  * m[11] * m[14] + 
		m[8]  * m[6]  * m[15] - 
		m[8]  * m[7]  * m[14] - 
		m[12] * m[6]  * m[11] + 
		m[12] * m[7]  * m[10];

	inv[8] = m[4]  * m[9] * m[15] - 
		m[4]  * m[11] * m[13] - 
		m[8]  * m[5] * m[15] + 
		m[8]  * m[7] * m[13] + 
		m[12] * m[5] * m[11] - 
		m[12] * m[7] * m[9];

	inv[12] = -m[4]  * m[9] * m[14] + 
		m[4]  * m[10] * m[13] +
		m[8]  * m[5] * m[14] - 
		m[8]  * m[6] * m[13] - 
		m[12] * m[5] * m[10] + 
		m[12] * m[6] * m[9];

	inv[1] = -m[1]  * m[10] * m[15] + 
		m[1]  * m[11] * m[14] + 
		m[9]  * m[2] * m[15] - 
		m[9]  * m[3] * m[14] - 
		m[13] * m[2] * m[11] + 
		m[13] * m[3] * m[10];

	inv[5] = m[0]  * m[10] * m[15] - 
		m[0]  * m[11] * m[14] - 
		m[8]  * m[2] * m[15] + 
		m[8]  * m[3] * m[14] + 
		m[12] * m[2] * m[11] - 
		m[12] * m[3] * m[10];

	inv[9] = -m[0]  * m[9] * m[15] + 
		m[0]  * m[11] * m[13] + 
		m[8]  * m[1] * m[15] - 
		m[8]  * m[3] * m[13] - 
		m[12] * m[1] * m[11] + 
		m[12] * m[3] * m[9];

	inv[13] = m[0]  * m[9] * m[14] - 
		m[0]  * m[10] * m[13] - 
		m[8]  * m[1] * m[14] + 
		m[8]  * m[2] * m[13] + 
		m[12] * m[1] * m[10] - 
		m[12] * m[2] * m[9];

	inv[2] = m[1]  * m[6] * m[15] - 
		m[1]  * m[7] * m[14] - 
		m[5]  * m[2] * m[15] + 
		m[5]  * m[3] * m[14] + 
		m[13] * m[2] * m[7] - 
		m[13] * m[3] * m[6];

	inv[6] = -m[0]  * m[6] * m[15] + 
		m[0]  * m[7] * m[14] + 
		m[4]  * m[2] * m[15] - 
		m[4]  * m[3] * m[14] - 
		m[12] * m[2] * m[7] + 
		m[12] * m[3] * m[6];

	inv[10] = m[0]  * m[5] * m[15] - 
		m[0]  * m[7] * m[13] - 
		m[4]  * m[1] * m[15] + 
		m[4]  * m[3] * m[13] + 
		m[12] * m[1] * m[7] - 
		m[12] * m[3] * m[5];

	inv[14] = -m[0]  * m[5] * m[14] + 
		m[0]  * m[6] * m[13] + 
		m[4]  * m[1] * m[14] - 
		m[4]  * m[2] * m[13] - 
		m[12] * m[1] * m[6] + 
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] + 
		m[1] * m[7] * m[10] + 
		m[5] * m[2] * m[11] - 
		m[5] * m[3] * m[10] - 
		m[9] * m[2] * m[7] + 
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] - 
		m[0] * m[7] * m[10] - 
		m[4] * m[2] * m[11] + 
		m[4] * m[3] * m[10] + 
		m[8] * m[2] * m[7] - 
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] + 
		m[0] * m[7] * m[9] + 
		m[4] * m[1] * m[11] - 
		m[4] * m[3] * m[9] - 
		m[8] * m[1] * m[7] + 
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] - 
		m[0] * m[6] * m[9] - 
		m[4] * m[1] * m[10] + 
		m[4] * m[2] * m[9] + 
		m[8] * m[1] * m[6] - 
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (det == 0)
		return false;

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		invOut[i] = float(inv[i] * det);

	return true;
}


// Find the distance between a point and a line segment in 3D
// Parameters:
//     ptX - point in question
//     ptA, ptB - line end points
//     ptOut - the closest point on the line
// Result: Distance in mm
float PointDistToLineSegment(const Vec3& ptX, const Vec3& ptA, const Vec3& ptB, Vec3& ptOut)
{
	float sqrDist = PointDistSqr(ptA, ptB);
	if (sqrDist > 1e-6)
	{
		Vec3 vX = ptX - ptA;
		Vec3 vL = ptB - ptA;

		float t = (vX^vL) / sqrDist;
		if (t <= 0)
			ptOut = ptA;
		else if (t >= 1)
			ptOut = ptB;
		else
			ptOut = ptA + t*vL;
	}
	else
		ptOut = ptA;

	return PointDist(ptOut, ptX);
}

// Test if a point is inside the closed polyline
// Note: The spline and point must be in 2D. This means that the Z-coordinate is not used.
bool PointInsideCurve2D(const Vec2& ptTest, std::vector<Vec2> sp)
{
	if (sp.size() < 3)
		return false;

	int nInter;
	float fMinX, fMinY;
	float fDX, fDY;
	Vec2 vePrev;

	nInter = 0;
	vePrev = sp.back();
	for (Vec2 ve : sp)
	{
		// test if intersecting (ignoring all vertices which are identical)
		if ( ((ptTest.Y >= vePrev.Y) && (ptTest.Y < ve.Y)) ||
		     ((ptTest.Y <  vePrev.Y) && (ptTest.Y >= ve.Y)) )
		{  
			// ptTest.Y is between vePrev.Y and ve.Y
			if ( (vePrev.X > ptTest.X) && (ve.X > ptTest.X) )
			{
				// if both points on right side of ptTest then intersection
				nInter++;
			}
			else if ( !((vePrev.X < ptTest.X) && (ve.X < ptTest.X)) )
			{
				// if not both points on the left side an explicit check for intersections is performed
				fMinX = min(vePrev.X, ve.X);
				fMinY = min(vePrev.Y, ve.Y);
				fDX   = vePrev.X - ve.X;
				fDY   = vePrev.Y - ve.Y;

				if ( abs(fDY) > 1E-6 )
				{  // dY is not zero
					if ( ((fDX > 0) && (fDY > 0)) || ((fDX < 0) && (fDY < 0)) )
					{
						if (ptTest.X - ((ptTest.Y - fMinY)*fDX/fDY + fMinX) < 0)
							nInter++;
					}
					else
					{
						//  if ptTest.X-(abs(fDX)+(ptTest.Y-fMinY)*fDX/fDY+fMinX)<0 then inc(nInter);
						if (ptTest.X - ((-abs(fDY) - fMinY + ptTest.Y)*fDX/fDY + fMinX) < 0)
							nInter++;
					}
				}
			}
		}

		vePrev = ve;
	}

	// if number of intersections is odd, the test point is inside
	if ((nInter % 2) == 0)
		return false;
	else
		return true;
}


Vec3 CatmullRom(Vec3 aptCP0, Vec3 aptCP1, Vec3 aptCP2, Vec3 aptCP3, float t)
{
	Vec3 ptNew =         (((-t*(t - 1.0f)*(t - 1.0f)) / 2.0f)       * aptCP0);
		 ptNew = ptNew + ((( 3.0f * t*t*t - 5.0f * t*t + 2) / 2.0f) * aptCP1);
		 ptNew = ptNew + (((-3.0f * t*t*t + 4.0f * t*t + t) / 2.0f) * aptCP2);
		 ptNew = ptNew + (((t*t*(t - 1.0f)) / 2.0f)                 * aptCP3);

	return ptNew;
}


Vec3 BezierCubic(Vec3 aptCP0, Vec3 aptCP1, Vec3 aptCP2, Vec3 aptCP3, float t)
{

	Vec3 ptNew =         (((1-t)*(1-t)*(1-t)) * aptCP0);
		 ptNew = ptNew + ((3*(1-t)*(1-t)*t)   * aptCP1);
		 ptNew = ptNew + ((3*(1-t)*t*t)       * aptCP2);
		 ptNew = ptNew + ((t*t*t)             * aptCP3);

	return ptNew;
}



void CatmullSubdivide(std::vector<Vec2>& list, std::vector<Vec2>& listOut, int iSubdCount)
{
	if (list.size() < 1) return;

	if (list.size() == 1) {
		listOut.push_back(list[0]);
		return;
	}

	Vec3 pt0, pt1, pt2, pt3;
	int i0, i1, i2, i3;
	for (int i=0; i < int(list.size())-1; i++)
	{
		i0 = max(i-1,0);
		i1 = i;
		i2 = i+1;
		i3 = min(i+2, int(list.size())-1);

		pt0 = Vecc3(list[i0]);
		pt1 = Vecc3(list[i1]);
		pt2 = Vecc3(list[i2]);
		pt3 = Vecc3(list[i3]);

		for (int j = 0; j < iSubdCount; j++)
		{
			Vec3 pt = CatmullRom(pt0, pt1, pt2, pt3, j/float(iSubdCount));
			listOut.push_back(Vecc2(pt.X, pt.Y));
		}
	}
	listOut.push_back(Vecc2(pt3.X, pt3.Y));
}


void BezierCubicSubdivide(std::vector<Vec2>& list, std::vector<Vec2>& listOut, int iSubdCount)
{
	if (list.size() != 4) return;

	Vec3 pt0, pt1, pt2, pt3;
	pt0 = Vecc3(list[0]);
	pt1 = Vecc3(list[1]);
	pt2 = Vecc3(list[2]);
	pt3 = Vecc3(list[3]);

	for (int j = 0; j <= iSubdCount; j++)
	{
		Vec3 pt = BezierCubic(pt0, pt1, pt2, pt3, j/float(iSubdCount));
		listOut.push_back(Vecc2(pt.X, pt.Y));
	}
}


Vec2 VecHatCW(const Vec2& v)
{
    return Vecc2(v.Y, -v.X);
}

Vec2 VecHatCCW(const Vec2& v)
{
    return Vecc2(-v.Y, v.X);
}
