
#ifndef OPENGLSUBWINDOW_H
#define OPENGLSUBWINDOW_H

#include "../!!adGlobals/vector_math.h"
#include "../!!adGlobals/trackball.h"

class OpenGLSubWindow
{
public:
	char m_strCaption[255];

	Vec3 clrBackground;

	OpenGLSubWindow(int iBottomLeftX, int iBottomLeftY, int iWidth, int iHeight)
	{
		m_iBottomLeftX = iBottomLeftX;
		m_iBottomLeftY = iBottomLeftY;
		m_iWidth  = iWidth;
		m_iHeight = iHeight;

		m_strCaption[0] = 0;
		clrBackground = Vecc3(0.1f, 0.1f, 0.1f);

		bMouseSceneRotationInProgress = false;
		vUserSceneTranslation = Vecc3();
		bMouseSceneDragInProgress = false;

		bSceneRotationAllowed = true;
		bSceneDragAllowed     = true;
		bSceneZoomAllowed     = true;

		bRenderGUIdecoration  = true;

		fUserScale = 1.f;
		fZoomFactor = 0.5f;

		trackball(curquat, 0.0, 0.0, 0.0, 0.0);

		matrUserScale = Mat4MakeIdent();
	}
	virtual ~OpenGLSubWindow() {}

	virtual void Render();

	virtual void MotionFunc(int x,int y);
	virtual void PassiveMotionFunc(int x,int y);
	virtual void MouseFunc(int button,int state,int x,int y);
	virtual bool KeyboardFunc(unsigned char key, int x, int y);
	virtual void MouseWheelFunc(int state,int delta,int x,int y);

	virtual void Reshape(int iBottomLeftX, int iBottomLeftY, int iWidth, int iHeight)
	{
		m_iBottomLeftX = iBottomLeftX;	m_iBottomLeftY = iBottomLeftY;
		m_iWidth = iWidth;	m_iHeight = iHeight;
	}
	virtual void SetupGraphicsPipeline();

	void ResetView();

	bool bSceneRotationAllowed;
	bool bSceneDragAllowed;
	bool bSceneZoomAllowed;

	bool bRenderGUIdecoration;

	float fZoomFactor;

protected:
	int m_iBottomLeftX;
	int m_iBottomLeftY;

	int m_iWidth;
	int m_iHeight;

	float curquat[4];

	float fUserScale;
	Matr4 matrUserScale;

private:
	float lastquat[4];
	bool bMouseSceneRotationInProgress;			//used for quaternion camera rotation
	int  iBeginRotateX, iBeginRotateY;

	Vec3 vUserSceneTranslation;
	bool bMouseSceneDragInProgress;
	int  iBeginDragX, iBeginDragY;
};


#endif
