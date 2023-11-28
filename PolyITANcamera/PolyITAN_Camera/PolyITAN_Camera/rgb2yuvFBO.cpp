#include "rgb2yuvFBO.h"
#include <GL/glut.h>
#include "Camera.h"
#include "GLSL_Pipeline.h"
#include "../../!!adGlobals/bmpHandler.h"
#include "../../!!adExtensions/extensions.h"
#include "../../!!adGlobals/TextureDescriptor.h"
#include "../../!!adGlobals/wdir.h"
#include "../../!!adGlobals/adOpenGLUtilities.h"


extern GLSL_Pipeline glsl_pipeline;
extern Camera cam;
extern TextureBank texBank;

rgb2yuvFBO::rgb2yuvFBO(int iBottomLeftX, int iBottomLeftY, int iWidth, int iHeight) :
	                 OpenGLSubWindow(iBottomLeftX, iBottomLeftY, iWidth, iHeight)
{
	bRenderGUIdecoration  = false;

	bSceneRotationAllowed = false;
	bSceneDragAllowed     = false;
	bSceneZoomAllowed     = false;

	clrBackground = Vecc3(0.2f, 0.2f, 0.2f);
	
	TextureDescriptor* texDesc;
	fbo = new FrameBufferObject(iWidth, iHeight);
	texDesc = fbo->Init();
	texBank.bank[TEXTURE_RGB2YUV_OUT] = texDesc;
}

rgb2yuvFBO::~rgb2yuvFBO()
{
	delete fbo;
}


void rgb2yuvFBO::Render()
{
	if (!glsl_pipeline.bUseShaderPipeline) return;
	
	fbo->Activate();

	OpenGLSubWindow::Render();

	glUseProgramObjectARB(glsl_pipeline.GPUPrograms["rgb2yuv"]->programObj);
	glUniform1iARB(glsl_pipeline.GPUPrograms["rgb2yuv"]->GetUniform("tex0"), 0);

		RenderTexturedQuad(texBank[TEXTURE_BILATERAL_OUTPUT]->m_uiTextureID,
			               -m_iWidth/2.0f, -m_iHeight/2.0f, m_iWidth, m_iHeight);

	glUseProgramObjectARB(FFP);
	fbo->Deactivate();
}

bool rgb2yuvFBO::KeyboardFunc(unsigned char key, int x, int y)
{
	bool res = false;

	if (OpenGLSubWindow::KeyboardFunc(key, x, y)) return true;

	switch (key)
	{
	case '1':
	{
		unsigned char* data;

		glutSetCursor(GLUT_CURSOR_WAIT);

		struct timespec T0;
		clock_gettime(CLOCK_REALTIME, &T0);
		
		char filename[400];
		unsigned char* downsized;

		sprintf(filename, "Frames/%ld%ldY.bmp", T0.tv_sec, T0.tv_nsec);
		data = fbo->DownloadTexture(GL_RED);
		saveGreyscaleBMP(FullPathToFile(filename), data, fbo->m_iWidth, fbo->m_iHeight, 8);

		sprintf(filename, "Frames/%ld%ldU.bmp", T0.tv_sec, T0.tv_nsec);
		data = fbo->DownloadTexture(GL_GREEN);
		downsized = BitmapResizeGreyscale05x(data, fbo->m_iWidth, fbo->m_iHeight);
		saveGreyscaleBMP(FullPathToFile(filename), downsized, fbo->m_iWidth/2, fbo->m_iHeight/2, 4);
		delete[] downsized;

		sprintf(filename, "Frames/%ld%ldV.bmp", T0.tv_sec, T0.tv_nsec);
		data = fbo->DownloadTexture(GL_BLUE);
		downsized = BitmapResizeGreyscale05x(data, fbo->m_iWidth, fbo->m_iHeight);
		saveGreyscaleBMP(FullPathToFile(filename), downsized, fbo->m_iWidth/2, fbo->m_iHeight/2, 4);
		delete[] downsized;

		sprintf(filename, "Frames/%ld%ldY1.bmp", T0.tv_sec, T0.tv_nsec);
		data = fbo->DownloadTexture(GL_ALPHA);
		saveGreyscaleBMP(FullPathToFile(filename), data, fbo->m_iWidth, fbo->m_iHeight, 4);

		glutSetCursor(GLUT_CURSOR_INHERIT);

		res = true;

		break;
	}
	break;
	}

	return res;
}
