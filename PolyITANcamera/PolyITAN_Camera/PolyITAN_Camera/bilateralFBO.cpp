#include "bilateralFBO.h"
#include <GL/glut.h>
#include "Camera.h"
#include "GLSL_Pipeline.h"
#include "../../!!adExtensions/extensions.h"
#include "../../!!adGlobals/TextureDescriptor.h"
#include "../../!!adGlobals/adOpenGLUtilities.h"


extern GLSL_Pipeline glsl_pipeline;
extern Camera cam;
extern TextureBank texBank;

bilateralFBO::bilateralFBO(int iBottomLeftX, int iBottomLeftY, int iWidth, int iHeight) :
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
	texBank.bank[TEXTURE_BILATERAL_OUTPUT] = texDesc;
}

bilateralFBO::~bilateralFBO()
{
	delete fbo;
}


void bilateralFBO::Render()
{
	if (!glsl_pipeline.bUseShaderPipeline) return;
	
	fbo->Activate();

	OpenGLSubWindow::Render();

	glUseProgramObjectARB(glsl_pipeline.GPUPrograms["bilateral"]->programObj);
	glUniform1iARB(glsl_pipeline.GPUPrograms["bilateral"]->GetUniform("tex0"), 0);
	glUniform3fARB(glsl_pipeline.GPUPrograms["bilateral"]->GetUniform("vGain"),
		           pow(10, cam.vGainRGB.X / 20), pow(10, cam.vGainRGB.Y / 20), pow(10, cam.vGainRGB.Z / 20));	// db to gain
	glUniform1fARB(glsl_pipeline.GPUPrograms["bilateral"]->GetUniform("fSigmaSqr"), glsl_pipeline.fDenoiseSqr);

		RenderTexturedQuad(texBank[TEXTURE_BAYER2RGB_OUT]->m_uiTextureID,
			               -m_iWidth/2.0f, -m_iHeight/2.0f, m_iWidth, m_iHeight);

	glUseProgramObjectARB(FFP);
	fbo->Deactivate();
}

