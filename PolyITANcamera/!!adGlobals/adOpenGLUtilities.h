#ifndef ADOPENGLUTILITIES_H
#define ADOPENGLUTILITIES_H

#include "vector_math.h"

const Vec3 clRed  = Vecc3(255.0, 0.0, 0.0);
const Vec3 clBlue = Vecc3(0.0,   0.0, 255.0);
const Vec3 clBlack= Vecc3(0.0,   0.0, 0.0);


#define glError() { \
    GLenum err = glGetError(); \
    while (err != GL_NO_ERROR) \
    {   printf("glError: %s caught at %s:%u", \
           (char*)gluErrorString(err), __FILE__, __LINE__); \
        err = glGetError(); \
        exit(-1); \
	} \
}


void gluUnProjectFriendly(int mouse_x, int mouse_y, int height, double& px, double& py, double& pz);
void gluUnProjectFriendlyZ(int mouse_x, int mouse_y, float fZ, int height, double& px, double& py, double& pz);
void RenderAxes(float fSize, float fLineWidth = 1.0);
void DrawCircle(float fRadius);
void DrawCircle(Vec3 fCenter, float fRadius, int iSegments);
void DrawCylinder(float fHeight, float fRadius, int iDiv_h, int iDiv_r);

void glutSolidCylinder(float fRadius, float fHeight, int n, int m);
void glutSolidDisk(float fInnerRadius,float fOutterRadius, int iSlices, int iLoops);
void glutSolidCylinderClosed(float fRadius, float fHeight, int iSlices, int iCylinderStacks, float fDiskInnerRadius, int iLoops);
void glTexturedQuad(float fX_bottom, float fY_bottom, float fWidth, float fHeight, float fZ = 0.0f);
void glTexturedQuadMesh(float fX_bottom, float fY_bottom, float fWidth, float fHeight, float fZ = 0.0f, int iHorSlices = 10, int iVertSlices = 10);
void glQuad(float fX_bottom, float fY_bottom, float fWidth, float fHeight, float fZ = 0.0f);
void glWireRectangle(float fX_bottom, float fY_bottom, float fWidth, float fHeight, float fZ = 0.0f);

void RenderTexturedQuad(unsigned int tex,
		                float fX_bottom, float fY_bottom,
		                float fWidth, float fHeight,
		                float fZ = 0.0f);

void RenderTexturedQuadMesh(unsigned int tex,
							float fX_bottom, float fY_bottom,
							float fWidth, float fHeight,
							float fZ = 0.0f,
							int iHorSlices = 10, int iVertSlices = 10);

// deprecated: moved from vector_math.h
void normalCalc(float x0,float y0,float z0,float x1,float y1,float z1,float x2,float y2,float z2);
void normalcalcReturn(float x0,float y0,float z0,float x1,float y1,float z1,float x2,float y2,float z2, float &nx, float &ny, float &nz);

#endif