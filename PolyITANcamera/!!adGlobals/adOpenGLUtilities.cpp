#include <GL/gl.h>
#include <GL/glu.h>
#include "adOpenGLUtilities.h"

// mouse_x, mouse_y      - оконные координаты курсора мыши.
// px, py, pz            - возвращаемые параметры,
void gluUnProjectFriendly(int mouse_x, int mouse_y, int height, double& px, double& py, double& pz)
{
GLint    viewport[4];       // параметры viewport-a.
GLdouble projection[16];    // матрица проекции.
GLdouble modelview[16];     // видовая матрица.
GLfloat  vx,vy,vz;          // координаты курсора мыши в системе координат viewport-a.

	glGetIntegerv(GL_VIEWPORT,viewport);           // узнаём параметры viewport-a.
	glGetDoublev(GL_PROJECTION_MATRIX,projection); // узнаём матрицу проекции.
	glGetDoublev(GL_MODELVIEW_MATRIX,modelview);   // узнаём видовую матрицу.

	// переводим оконные координаты курсора в систему координат viewport-a.
	vx = float(mouse_x);
	if (height > 0)
		vy = float(height - mouse_y); // где _text_height - текущая высота окна
	else
		vy = float(mouse_y);

	glReadPixels(GLint(vx), GLint(vy), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &vz);

	gluUnProject(vx, vy, vz, modelview, projection, viewport, &px, &py, &pz);
}

void gluUnProjectFriendlyZ(int mouse_x, int mouse_y, float fZ, int height, double& px, double& py, double& pz)
{
GLint viewport[4];       // параметры viewport-a.
GLdouble projection[16]; // матрица проекции.
GLdouble modelview[16];  // видовая матрица.
GLfloat vx,vy;           // координаты курсора мыши в системе координат viewport-a.

	glGetIntegerv(GL_VIEWPORT,viewport);           // узнаём параметры viewport-a.
	glGetDoublev(GL_PROJECTION_MATRIX,projection); // узнаём матрицу проекции.
	glGetDoublev(GL_MODELVIEW_MATRIX,modelview);   // узнаём видовую матрицу.

	// переводим оконные координаты курсора в систему координат viewport-a.
	vx = float(mouse_x);
	if (height > 0)
		vy = float(height - mouse_y); // где _text_height - текущая высота окна.
	else
		vy = float(mouse_y);

	gluUnProject(vx, vy, fZ, modelview, projection, viewport, &px, &py, &pz);
}


void RenderAxes(float fSize, float fLineWidth)
{
	glLineWidth(fLineWidth);
	glBegin(GL_LINES);
		glColor3f(1,0,0);
		glVertex3f(0,0,0);
		glVertex3f(fSize,0,0);

		glColor3f(0,1,0);
		glVertex3f(0,0,0);
		glVertex3f(0,fSize,0);

		glColor3f(0,0,1);
		glVertex3f(0,0,0);
		glVertex3f(0,0,fSize);
	glEnd();
}

void DrawCircle(float fRadius)
{
	glBegin(GL_LINE_LOOP);
		for (unsigned short int i=0; i<360; i++)
			glVertex3f(fRadius*cos(float(i)*DEGREES_TO_RADIANS), fRadius*sin(float(i)*DEGREES_TO_RADIANS), 0);
	glEnd();
}

void DrawCircle(Vec3 vCenter, float fRadius, int iSegments)
{
	glBegin(GL_LINE_LOOP);
	for (unsigned short int i = 0; i<360; i+=360/iSegments)
		glVertex3f(vCenter.X + fRadius*cos(float(i)*DEGREES_TO_RADIANS),
			       vCenter.Y + fRadius*sin(float(i)*DEGREES_TO_RADIANS), vCenter.Z);
	glEnd();
}

void DrawCylinder( float fHeight, float fRadius, int iDiv_h, int iDiv_r )
{
int i, j;
float h0, h1;
float theta, s, c;

	for ( i = 0; i < iDiv_h; i++ )
	{
		h0 = fHeight * i / iDiv_h;
		h1 = fHeight * ( i + 1 ) / iDiv_h;
		glBegin( GL_QUAD_STRIP );
		for ( j = 0; j <= iDiv_r; j++ )
		{
			theta = float(2.0 * 3.141592654f * ( j % iDiv_r ) / iDiv_r);
			s = sin( theta );
			c = cos( theta );
			glNormal3f( c, s, 1.0f );
			glVertex3f( fRadius * c, fRadius * s, h1 );
			glVertex3f( fRadius * c, fRadius * s, h0 );
		}
		glEnd();
	}
}


void glutSolidCylinder(float fRadius, float fHeight, int n, int m)
{
	glPushMatrix();
		glRotatef(90, 1.0f,0.0f,0.0f);
		glTranslatef(0.0f,0.0f,-fHeight/2);
		GLUquadricObj *qobj = gluNewQuadric();
			gluQuadricDrawStyle(qobj,GLU_FILL);
			gluCylinder(qobj,fRadius,fRadius,fHeight,n,m);
		gluDeleteQuadric(qobj);  
	glPopMatrix();
}

void glutSolidDisk(float fInnerRadius,float fOutterRadius,int iSlices,int iLoops)
{
	glPushMatrix();
		glRotatef(90, 1.0f,0.0f,0.0f);
		GLUquadricObj *qobj = gluNewQuadric();
			gluQuadricDrawStyle(qobj,GLU_FILL);
			gluDisk(qobj,fInnerRadius,fOutterRadius,iSlices,iLoops);
		gluDeleteQuadric(qobj);  
	glPopMatrix();
}

void glutSolidCylinderClosed(float fRadius, float fHeight, int iSlices, int iCylinderStacks, float fDiskInnerRadius, int iLoops)
{
	GLUquadricObj *qobj;

	glPushMatrix();
		glRotatef(90, 1.0f,0.0f,0.0f);
		glTranslatef(0.0f,0.0f,-fHeight/2);
		qobj = gluNewQuadric();
			gluQuadricDrawStyle(qobj,GLU_FILL);
			gluCylinder(qobj,fRadius,fRadius,fHeight,iSlices,iCylinderStacks);
		gluDeleteQuadric(qobj);  
		qobj = gluNewQuadric();
			gluQuadricDrawStyle(qobj,GLU_FILL);
			gluDisk(qobj,fDiskInnerRadius,fRadius,iSlices,iLoops);
		gluDeleteQuadric(qobj);  
		glTranslatef(0.0f,0.0f, fHeight);
		qobj = gluNewQuadric();
			gluQuadricDrawStyle(qobj,GLU_FILL);
			gluDisk(qobj,fDiskInnerRadius,fRadius,iSlices,iLoops);
		gluDeleteQuadric(qobj);  
	glPopMatrix();
}


void normalcalcReturn(float x0,float y0,float z0,float x1,float y1,float z1,float x2,float y2,float z2, float &nx, float &ny, float &nz)
{
	float x10,y10,z10,x12,y12,z12;
	float cpx,cpy,cpz;
	float r;
	float ar[3];

	/* Compute edge vectors */
	x10 = x1 - x0;
	y10 = y1 - y0;
	z10 = z1 - z0;
	x12 = x1 - x2;
	y12 = y1 - y2;
	z12 = z1 - z2;

	/* Compute the cross product */
	cpx = (z10 * y12) - (y10 * z12);
	cpy = (x10 * z12) - (z10 * x12);
	cpz = (y10 * x12) - (x10 * y12);

	/* Normalize the result to get the unit-length facet normal */
	r = sqrt(cpx * cpx + cpy * cpy + cpz * cpz);
	if (r==0.0)
	{
		ar[0]=ar[1]=ar[2]=0.0;
		return;
	}
	ar[0] = cpx / r;
	ar[1] = cpy / r;
	ar[2] = cpz / r;

	glNormal3fv(ar);
	nx=ar[0];	ny=ar[1];	nz=ar[2];
}



void normalCalc(float x0,float y0,float z0,float x1,float y1,float z1,float x2,float y2,float z2)
{
	float x10,y10,z10,x12,y12,z12;
	float cpx,cpy,cpz;
	float r;
	float ar[3];

	/* Compute edge vectors */
	x10 = x1 - x0;
	y10 = y1 - y0;
	z10 = z1 - z0;
	x12 = x1 - x2;
	y12 = y1 - y2;
	z12 = z1 - z2;

	/* Compute the cross product */
	cpx = (z10 * y12) - (y10 * z12);
	cpy = (x10 * z12) - (z10 * x12);
	cpz = (y10 * x12) - (x10 * y12);

	/* Normalize the result to get the unit-length facet normal */
	r = sqrt(cpx * cpx + cpy * cpy + cpz * cpz);
	if (r==0.0)
	{
		ar[0]=ar[1]=ar[2]=0.0;
		return;
	}
	ar[0] = cpx / r;
	ar[1] = cpy / r;
	ar[2] = cpz / r;

	glNormal3fv(ar);
}


void glTexturedQuad(float fX_bottom, float fY_bottom, float fWidth, float fHeight, float fZ)
{
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(fX_bottom, fY_bottom,          fZ);

		glTexCoord2f(1.0, 0.0);
		glVertex3f(fX_bottom + fWidth, fY_bottom, fZ);

		glTexCoord2f(1.0, 1.0);
		glVertex3f(fX_bottom + fWidth, fY_bottom + fHeight, fZ);

		glTexCoord2f(0.0, 1.0);
		glVertex3f(fX_bottom, fY_bottom + fHeight, fZ);
	glEnd();
}


void glTexturedQuadMesh(float fX_bottom, float fY_bottom, float fWidth, float fHeight, float fZ, int iHorSlices, int iVertSlices)
{
	for(int j=0; j<iVertSlices; j++)
	{			
		glBegin(GL_TRIANGLE_STRIP);

			for (int i=0; i<=iHorSlices; i++)
			{
				Vec2 u01 = Vecc2(fX_bottom + fWidth*(i/float(iHorSlices)),  fY_bottom + fHeight*((j + 1)/float(iVertSlices)) );
				Vec2 u00 = Vecc2(fX_bottom + fWidth*(i/float(iHorSlices)),  fY_bottom + fHeight* (j     /float(iVertSlices)) );

				glTexCoord2f(i/float(iHorSlices), (j + 1)/float(iVertSlices) );
				glVertex3f(u01.X, u01.Y, fZ);

				glTexCoord2f(i/float(iHorSlices),      j /float(iVertSlices) );
				glVertex3f(u00.X, u00.Y, fZ);
			}

		glEnd();
	}
}

void RenderTexturedQuad(unsigned int tex,
	                   float fX_bottom, float fY_bottom,
	                   float fWidth, float fHeight,
	                   float fZ)
{
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, tex);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glColor3f(0.0, 1.0, 0.0);
	glTexturedQuad(fX_bottom, fY_bottom, fWidth, fHeight, fZ);

	glDisable(GL_TEXTURE_2D);
}

void RenderTexturedQuadMesh(unsigned int tex,
	                        float fX_bottom, float fY_bottom,
	                        float fWidth, float fHeight,
	                        float fZ,
	                        int iHorSlices, int iVertSlices)
{
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, tex);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glColor3f(0.0, 1.0, 0.0);
	glTexturedQuadMesh(fX_bottom, fY_bottom, fWidth, fHeight, fZ, iHorSlices, iVertSlices);

	glDisable(GL_TEXTURE_2D);
}

void glQuad(float fX_bottom, float fY_bottom, float fWidth, float fHeight, float fZ)
{
	glBegin(GL_QUADS);
		glVertex3f(fX_bottom,          fY_bottom,           fZ);
		glVertex3f(fX_bottom + fWidth, fY_bottom,           fZ);
		glVertex3f(fX_bottom + fWidth, fY_bottom + fHeight, fZ);
		glVertex3f(fX_bottom,          fY_bottom + fHeight, fZ);
	glEnd();
}

void glWireRectangle(float fX_bottom, float fY_bottom, float fWidth, float fHeight, float fZ)
{
	glBegin(GL_LINE_LOOP);
		glVertex3f(fX_bottom,          fY_bottom,           fZ);
		glVertex3f(fX_bottom + fWidth, fY_bottom,           fZ);
		glVertex3f(fX_bottom + fWidth, fY_bottom + fHeight, fZ);
		glVertex3f(fX_bottom,          fY_bottom + fHeight, fZ);
	glEnd();
}
