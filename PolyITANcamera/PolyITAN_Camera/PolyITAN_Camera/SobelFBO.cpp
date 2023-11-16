#include "SobelFBO.h"
#include <GL/glut.h>
#include "Camera.h"
#include "GLSL_Pipeline.h"
#include "../../!!adExtensions/extensions.h"
#include "../../!!adGlobals/TextureDescriptor.h"
#include "../../!!adGlobals/adOpenGLUtilities.h"


extern GLSL_Pipeline glsl_pipeline;
extern TextureBank texBank;

SobelFBO::SobelFBO(int iBottomLeftX, int iBottomLeftY, int iWidth, int iHeight) :
	                                 OpenGLSubWindow(iBottomLeftX, iBottomLeftY, iWidth, iHeight)
{
	bRenderGUIdecoration = false;

	bSceneRotationAllowed = false;
	bSceneDragAllowed = false;
	bSceneZoomAllowed = false;

	clrBackground = Vecc3(0.2f, 0.2f, 0.2f);

	TextureDescriptor* texDesc;
	fbo = new FrameBufferObject(iWidth, iHeight);
	texDesc = fbo->Init();
	texBank.bank[TEXTURE_SOBEL_OUT] = texDesc;
}

SobelFBO::~SobelFBO()
{
	delete fbo;
}


void SobelFBO::Render()
{
	if (!glsl_pipeline.bUseShaderPipeline) return;

	fbo->Activate();

	OpenGLSubWindow::Render();

	glUseProgramObjectARB(glsl_pipeline.GPUPrograms["Sobel"]->programObj);
	glUniform1iARB(glsl_pipeline.GPUPrograms["Sobel"]->GetUniform("tex0"), 0);

	RenderTexturedQuad(texBank[TEXTURE_BAYER2RGB_OUT]->m_uiTextureID,
		               -m_iWidth/2.0f, -m_iHeight/2.0f, m_iWidth, m_iHeight);

	glUseProgramObjectARB(FFP);
	fbo->Deactivate();
}


