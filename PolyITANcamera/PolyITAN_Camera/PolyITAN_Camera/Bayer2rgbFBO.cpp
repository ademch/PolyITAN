#include "Bayer2rgbFBO.h"
#include "Camera.h"
#include "GLSL_Pipeline.h"
#include "../../!!adExtensions/extensions.h"
#include "../../!!adGlobals/TextureDescriptor.h"
#include "../../!!adGlobals/adOpenGLUtilities.h"


extern GLSL_Pipeline glsl_pipeline;
extern Camera cam;
extern TextureBank texBank;

Bayer2rgbFBO::Bayer2rgbFBO(int iBottomLeftX, int iBottomLeftY, int iWidth, int iHeight) :
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
	texBank.bank[TEXTURE_BAYER2RGB_OUT] = texDesc;
}

Bayer2rgbFBO::~Bayer2rgbFBO()
{
	delete fbo;
}


void Bayer2rgbFBO::Render()
{
	if (!glsl_pipeline.bUseShaderPipeline) return;
	
	fbo->Activate();

	OpenGLSubWindow::Render();

	glUseProgramObjectARB(glsl_pipeline.GPUPrograms["BayerF"]->programObj);
	glUniform1iARB(glsl_pipeline.GPUPrograms["BayerF"]->GetUniform("tex0"), 0);

		RenderTexturedQuad(cam.m_texture,
			               -m_iWidth/2.0f, -m_iHeight/2.0f, m_iWidth, m_iHeight);

	glUseProgramObjectARB(FFP);
	fbo->Deactivate();
}

