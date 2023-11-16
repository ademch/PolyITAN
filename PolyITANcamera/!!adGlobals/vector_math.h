#ifndef VECTOR_MATH_H
#define VECTOR_MATH_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

enum
{ X, Y, Z, W };

enum
{ A, B, C, D };

#ifndef M_PI
	#define PI  3.1415926535897932385f
#else
	#define PI  ((float) M_PI)
#endif

#define DEGREES_TO_RADIANS  (PI/180.0f)
#define RADIANS_TO_DEGREES  (180.0f/PI)
#define M_SQRT3             1.7320508075688772935274463415059

inline float sqr(float);
inline double sqr(double);


struct Vec2
{	float X; float Y; };

struct Vec3
{	union { struct {float X; float Y; float Z;}; 
            float ar[3]; 
          };
};

struct Vec3d
{	double X; double Y; double Z;  };

struct VecUB3
{	unsigned char R; unsigned char G; unsigned char B; };

struct VecUB4
{	unsigned char R; unsigned char G; unsigned char B;  unsigned char A; };

struct Vec4
{	float X; float Y; float Z; float W;	};

struct Matr4
{	float m[4][4];	};

struct Matr4d
{	double m[4][4];	};

struct Matr3
{	float m[3][3];	};

struct Matr3d
{	double m[3][3];	};

// GL_N3F_V3F
struct TriVertex
{	Vec3 n;
	Vec3 v;
};

//Helper structure defining triangle dereferenced from TriVertexIndex structure
struct Triangle
{	TriVertex v0,v1,v2;
};

struct TriVertexIndex
{	unsigned int ind[3];
};

// GL_C4UB_V3F
struct PointVertex
{	VecUB4 c;
	Vec3 v;
};

// GL_C4UB_V3F
struct PointVertexExt
{	VecUB4 c;
	Vec3 n;
	Vec3 v;
};

// Non standard user array type, used only as container
struct PointCloudVertex
{	Vec3 n;
	Vec3 v;
	float fMisc;
};

struct PointVertexIndex
{	unsigned int ind;
};

struct LineSegment
{	Vec3 vStart;
	Vec3 vEnd;
};

struct SMeshBezier4x4
{
	float x[4][4];
	float y[4][4];
	float z[4][4];
	float w[4][4];
};

float getRandomMinMax( float fMin, float fMax );
Vec3  getRandomVector( void );
Vec3  getRandomVector(Vec3 vInput, float fAngle );

Vec2 Vecc2(float x=0.0,float y=0.0);

VecUB4 VeccUB4(unsigned char R=0,unsigned char G=0,unsigned char B=0,unsigned char A=255);//constructor
VecUB3 VeccUB3(unsigned char R=0,unsigned char G=0,unsigned char B=0);//constructor


// single precision
Vec3 Vecc3(float x=0.0,float y=0.0,float z=0.0);			    //constructor
Vec3 Vecc3(Vec2 v, float z = 0.0);								//constructor
Vec4 Vecc4(float x=0.0,float y=0.0,float z=0.0,float w=1.0);	//constructor
Vec4 Vecc4(Vec3 v3=Vecc3(0.0,0.0,0.0), float w=1.0);			//constructor
Vec4 Vecp4(float a[4]);											//pointer caster

// double precision
Vec3  Vecc3(Vec3d vec);				                            //caster
Vec3d Vecc3d(Vec3 vec);				                            //caster
Vec3d Vecc3d(double x=0.0,double y=0.0,double z=0.0);			//constructor


inline Matr4 Matrc4(Vec4 r0=Vecc4(1.0,0.0,0.0,0.0),
					Vec4 r1=Vecc4(0.0,1.0,0.0,0.0),
					Vec4 r2=Vecc4(0.0,0.0,1.0,0.0),
					Vec4 r3=Vecc4(0.0,0.0,0.0,1.0));

Matr4 MatrTranspose(Matr4 m);


// Vector operators single precision
Vec3 operator+(Vec3 v1,Vec3 v2);
Vec2 operator+(Vec2 v1, Vec2 v2);
Vec3 operator-(Vec3 v);
Vec3 operator-(Vec3 v1,Vec3 v2);
Vec3 operator*(Vec3 v1,float s);
Vec2 operator*(Vec2 v1, float s);
Vec3 operator*(float s, Vec3 v1);
Vec2 operator*(float s, Vec2 v1);
Vec3  operator/(Vec3 v,float s);
Vec2  operator/(Vec2 v, float s);
float operator^(Vec3 v1,Vec3 v2);	  // DOT product
Vec3 operator*(Vec3 v1,Vec3 v2);      // CROSS product
Vec2 operator-(Vec2 v1, Vec2 v2);
float operator^(Vec2 v1,Vec2 v2);	  // DOT product
									 
// Vector operators double precision
double operator^(Vec3d v1,Vec3d v2);  // DOT product
Vec3d operator*(Vec3d v1,Vec3d v2);   // CROSS product
Vec3d operator-(Vec3d v1,Vec3d v2);
Vec3d operator+(Vec3d v1,Vec3d v2);
Vec3d operator-(Vec3d v);
Vec3d operator*(Vec3d v1,double s);
Vec3d operator*(double s, Vec3d v1);
Vec3d operator/(Vec3d v,double s);

VecUB4 operator*(VecUB4 v1,double s);
VecUB4 operator+(VecUB4 v1,VecUB4 v2);

unsigned char ClampF(float val);
unsigned char Clamp(int val);


//CVec3 planelineintersection(CVec3 n,float d,CVec3 p1,CVec3 p2);

Vec3  VecOrtho(const Vec3&);

float  VecLength(const Vec3&);
double VecLength(const Vec3d&);
float  VecLength(const Vec2&);

float  VecLengthSqr(const Vec3&);
float  VecLengthSqr(const Vec2&);
double VecLengthSqr(const Vec3d&);

float  PointDist(Vec3 pt1, Vec3 pt2);
float  PointDist(Vec2 pt1, Vec2 pt2);
double PointDist(Vec3d pt1, Vec3d pt2);
float  PointDistSqr(Vec3 pt1, Vec3 pt2);
float  PointDistSqr(Vec2 pt1, Vec2 pt2);

Vec2   VecNormalize(const Vec2&);
Vec3   VecNormalize(const Vec3&);
Vec3d  VecNormalize(const Vec3d&);
void   VecNormalizeP(Vec3&);

Vec3   VecReflect(Vec3 vInput, Vec3 vNormal);
Vec3d  VecReflect(Vec3d vInput, Vec3d vNormal);

Vec3   VecTranslate(Vec3 v, const float x, const float y, const float z);
Vec2   VecRotate(Vec2 v, float angle);

float  AngleBetweenVectors(Vec2 vec1, Vec2 vec2);

Vec3   VecMix(Vec3 v1, Vec3 v2, float t);
VecUB4 VecMix(VecUB4 v1, VecUB4 v2, float t);

Vec3   VecClampOne(Vec3 v);
Vec3   VecClampBYTE(Vec3 v);

Vec3   TriangleLinearInterpolate(Vec3 v0, Vec3 v1, Vec3 v2, float u, float v);

Vec3 SphereCoordinatesToCartesian(float fTheta, float fPhi, float fR);
Vec3 SphereCoordinatesToCartesian(Vec2 vAngles, float fR = 1.0f);

// Matrix operators
inline Vec3 operator*(Matr4 m, Vec3 v);
Vec3   operator*(Vec3 v, Matr4 m);
Vec3   operator^(Matr4 m, Vec3 v);
Vec3d  operator^(Matr4d m, Vec3d v);
Matr4  operator*(const Matr4& m1, const Matr4& m2);

// Matrix generators
Matr4  Mat4MakeIdent();
Matr4  Mat4MakeRot( const float angle, const Vec3& axis );
Matr4d Mat4dMakeRot( const double fAngleDeg, const Vec3d& axis );
Matr4  Mat4MakeTrans( const float x, const float y, const float z );
Matr4  Mat4MakeScale( const float x_scale, const float y_scale, const float z_scale );

void normalCalcPackSmooth(TriVertex* v0,TriVertex* v1, TriVertex* v2);
void normalCalcPack(TriVertex* v0,TriVertex* v1, TriVertex* v2);

// Intersection funcs
bool intersectRayTriangle( Vec3 v0, Vec3 v1, Vec3 v2, const Vec3 ray_orig, const Vec3 ray_dir,
				           float& u, float& v, float& t );

bool intersectAABoxRay( Vec3 box_ptMin, Vec3 box_ptMax,
					    Vec3 ray_orig,  Vec3 ray_dir,
					    float& tIn, float& tOut);

bool intersectRayPlane( const Vec3 ptPlane, const Vec3 vnPlane,
  		     		    const Vec3 ray_orig,const Vec3 ray_dir, float& t );

bool intersectRaySphere( Vec3 sphere_center, float sphere_radius,
						 Vec3 ray_orig, Vec3 ray_dir,
						 int& numhits, float& t0, float& t1 );

bool intersectLineSegSphere( Vec3 sphere_center, float sphere_radius,
							 Vec3 lineSeg_orig, Vec3 lineSeg_end,
							 int& numhits, float& t0, float& t1);

float  VectorPlaneIntersect(Vec3 ptVector,Vec3 vDirVector, Vec3 ptPlane,Vec3 nPlane);
double VectorPlaneIntersect(Vec3d ptVector,Vec3d vDirVector, Vec3d ptPlane,Vec3d nPlane);

float  Determinant(float v1,float v2,float v3,float v4,float v5,float v6,float v7,float v8,float v9);
double Determinant(double v1,double v2,double v3,double v4,double v5,double v6,double v7,double v8,double v9);

bool gluInvertMatrix(const float m[16], float invOut[16]);


//OpenGL to CPU functions
void cpuLoadIdentity();
void cpuRotatef(float angle, float x, float y, float z);
void cpuTranslatef(float x, float y, float z);
void cpuScalef(float x, float y, float z);
void cpuMultMatrixf(float*);
void cpuVertex3fv(float*);
Vec3 cpuPipelineVertex3fv(float*);

bool LineLineIntersect3D( Vec3d p1,Vec3d p2,Vec3d p3,Vec3d p4, Vec3d *pa,Vec3d *pb, double *mua, double *mub);
Vec3 PointLineProject(Vec3 pt, Vec3 ptLine, Vec3 vnLineDir);
Vec3 PointLineProject(Vec3 pt, Vec3 ptLine, Vec3 vnLineDir, bool& bPosProj);

// data array specific functions, added for backward compatibility during merge procedure
void findPlane(double plane[4], double v0[3], double v1[3], double v2[3]);
void findPlane(double plane[4], Vec3d v0, Vec3d v1, Vec3d v2);
void shadowMatrix(double shadowMat[4][4], double groundplane[4], double lightpos[4]);
void matrMult(double *matr, float &px,float &py,float &pz);
void matrTransp(float* matr);
void normalize(float *coor);

float PointDistToLineSegment(const Vec3& ptX, const Vec3& ptA, const Vec3& ptB, Vec3& ptOut);
bool  PointInsideCurve2D(const Vec2& ptTest, std::vector<Vec2> sp);

Vec3 CatmullRom(Vec3 aptCP0, Vec3 aptCP1, Vec3 aptCP2, Vec3 aptCP3, float t);
void CatmullSubdivide(std::vector<Vec2>& list, std::vector<Vec2>& listOut, int iSubdCount = 4);
void BezierCubicSubdivide(std::vector<Vec2>& list, std::vector<Vec2>& listOut, int iSubdCount);

Vec2 VecHatCW(const Vec2& v);
Vec2 VecHatCCW(const Vec2& v);

#endif
