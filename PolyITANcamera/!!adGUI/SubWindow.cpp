#include <GL/glut.h>
#include "glfont.h"
#include "../!!adGlobals/adOpenGLUtilities.h"

extern GLFONT font;

#include "SubWindow.h"

void OpenGLSubWindow::Render()
{
GLfloat m[4][4];

	// setup window specific params
	glViewport(m_iBottomLeftX, m_iBottomLeftY, m_iWidth, m_iHeight);

	// setup scissor test to make glClear operate selectively
	glScissor(m_iBottomLeftX, m_iBottomLeftY, m_iWidth, m_iHeight);
	glEnable(GL_SCISSOR_TEST);

	glClearColor(clrBackground.X, clrBackground.Y, clrBackground.Z, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-m_iWidth/2.0f, m_iWidth/2.0f, -m_iHeight/2.0f, m_iHeight/2.0f, -3000, 3000);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	if (bRenderGUIdecoration)
	{
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		glLineWidth(1.0);
		glColor3f(0.9f, 0.9f, 0.9f);

		glBegin(GL_LINE_LOOP);
			glVertex2f(-m_iWidth / 2.0 + 1, -m_iHeight / 2.0 + 1);
			glVertex2f(-m_iWidth / 2.0 + 1,  m_iHeight / 2.0 - 1);
			glVertex2f( m_iWidth / 2.0 - 1,  m_iHeight / 2.0 - 1);
			glVertex2f( m_iWidth / 2.0 - 1, -m_iHeight / 2.0 + 1);
		glEnd();

		glFontBegin(&font);
			glFontTextOut(m_strCaption, -m_iWidth / 2.0, -m_iHeight / 2.0, 0, 7);
		glFontEnd();

		glDisable(GL_TEXTURE_2D);
	}

	glDisable(GL_SCISSOR_TEST);

	glTranslatef(vUserSceneTranslation.X, vUserSceneTranslation.Y, vUserSceneTranslation.Z);

	glMultMatrixf(&matrUserScale.m[0][0]);

	build_rotmatrix(m, curquat);
	glMultMatrixf(&m[0][0]);
}


void OpenGLSubWindow::SetupGraphicsPipeline()
{
GLfloat m[4][4];

	// setup window specific params
	glViewport(m_iBottomLeftX, m_iBottomLeftY, m_iWidth, m_iHeight);

	glMatrixMode(GL_PROJECTION);
	//инициализация верхней матрицы на стеке единичной матрицой
	glLoadIdentity();
	//умножение верхней матрицы на стеке на матрицу параллельного проецирования
	glOrtho(-m_iWidth/2.0f, m_iWidth/2.0f, -m_iHeight/2.0f, m_iHeight/2.0f, -3000, 3000);

	//выбор стека матриц модельно-видовых преобразований
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(vUserSceneTranslation.X, vUserSceneTranslation.Y, vUserSceneTranslation.Z);
	glMultMatrixf(&matrUserScale.m[0][0]);

	build_rotmatrix(m, curquat);
	glMultMatrixf(&m[0][0]);
}

void OpenGLSubWindow::ResetView()
{
	vUserSceneTranslation = Vecc3();
	fUserScale = 1.f;
	matrUserScale = Mat4MakeIdent();
	trackball(curquat, 0.0, 0.0, 0.0, 0.0);
}

// x,y window coordinates from (0,0) to (w,h) (having y flipped of cause)
void OpenGLSubWindow::MotionFunc(int x,int y)
{
	if (bMouseSceneRotationInProgress)
	{	
		trackballSubWindow(lastquat,
						   iBeginRotateX,iBeginRotateY, x,y,
					       m_iBottomLeftX,m_iBottomLeftY, m_iWidth,m_iHeight);
		iBeginRotateX = x;
		iBeginRotateY = y;
		add_quats(lastquat, curquat, curquat);
		return;
	}

	if (bMouseSceneDragInProgress)
	{
		if ((x > m_iBottomLeftX) && (x < m_iBottomLeftX + m_iWidth) &&
			(y > m_iBottomLeftY) && (y < m_iBottomLeftY + m_iHeight))
		{
			vUserSceneTranslation.X += (x-iBeginDragX);
			vUserSceneTranslation.Y += (y-iBeginDragY);

			iBeginDragX = x;
			iBeginDragY = y;
			return;
		}
	}
}

// x,y window coordinates from (0,0) to (w,h) (having y flipped of cause)
void OpenGLSubWindow::MouseFunc(int button,int state,int x,int y)
{
	if ((x > m_iBottomLeftX) && (x < m_iBottomLeftX + m_iWidth) &&
		(y > m_iBottomLeftY) && (y < m_iBottomLeftY + m_iHeight))
	{

		if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && bSceneRotationAllowed)
		{	bMouseSceneRotationInProgress = true;

			iBeginRotateX = x;
			iBeginRotateY = y;
		}

		if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN && bSceneDragAllowed)
		{	bMouseSceneDragInProgress = true;

			iBeginDragX = x;
			iBeginDragY = y;
		}
	}

	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
		bMouseSceneDragInProgress = false;
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
		bMouseSceneRotationInProgress = false;
}


void OpenGLSubWindow::PassiveMotionFunc(int x,int y)
{
}

bool OpenGLSubWindow::KeyboardFunc(unsigned char key, int x, int y)
{
	return false;
}

// x,y window coordinates from (0,0) to (w,h) (having y flipped of cause)
void OpenGLSubWindow::MouseWheelFunc(int state,int delta,int x,int y)
{
float fDelta;

	if (!bSceneZoomAllowed) return;

	if ((x > m_iBottomLeftX) && (x < m_iBottomLeftX + m_iWidth) &&
		(y > m_iBottomLeftY) && (y < m_iBottomLeftY + m_iHeight))
	{
		fDelta = float(delta)/120.0;

		// prevent infinite scaling
		if ( (fDelta < 0) && (fUserScale < pow(fZoomFactor, 10)) ) return;
		if ( (fDelta > 0) && (fUserScale > pow(1.0/ fZoomFactor, 10)) )  return;

		if (fDelta < 0)
			fUserScale *= (1.0/-fDelta)*fZoomFactor;
		else
			fUserScale *= fDelta*1.0/fZoomFactor;

		// 0. Subwindow coordinates
		Vec2 vUserScalePoint2D;
		// make the lower left corner to be zero window coordinate
		vUserScalePoint2D = Vecc2(x - m_iBottomLeftX, y - m_iBottomLeftY);
		// -w/2 to w/2, -h/2 to h/2
		vUserScalePoint2D = vUserScalePoint2D - Vecc2(m_iWidth / 2.0, m_iHeight / 2.0);

		// 1. remove user translation
		matrUserScale = Mat4MakeTrans(vUserSceneTranslation.X, vUserSceneTranslation.Y, 0.0)*matrUserScale;
		// 2. move to the center
		matrUserScale = Mat4MakeTrans(-vUserScalePoint2D.X, -vUserScalePoint2D.Y, 0.0)*matrUserScale;

		// 3. scale
		if (fDelta < 0)
			matrUserScale = Mat4MakeScale((1.0 / -fDelta)*fZoomFactor, (1.0 / -fDelta)*fZoomFactor, 1.0f)*matrUserScale;
		else
			matrUserScale = Mat4MakeScale(fDelta*1.0 / fZoomFactor, fDelta*1.0 / fZoomFactor, 1.0f)*matrUserScale;

		// 4. move back
		matrUserScale = Mat4MakeTrans(vUserScalePoint2D.X, vUserScalePoint2D.Y, 0.0)*matrUserScale;
		// 5. reapply user translation
		matrUserScale = Mat4MakeTrans(-vUserSceneTranslation.X, -vUserSceneTranslation.Y, 0.0)*matrUserScale;
	}	
}
