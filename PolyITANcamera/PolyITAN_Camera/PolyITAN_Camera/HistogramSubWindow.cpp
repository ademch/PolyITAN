#include "HistogramSubWindow.h"
#include <GL/glut.h>
#include "../../!!adGlobals/adOpenGLUtilities.h"
#include "GLSL_Pipeline.h"
#include "../../!!adExtensions/extensions.h"
#include "Camera.h"
#include <string.h>


extern Camera cam;
extern GLSL_Pipeline glsl_pipeline;
extern TextureBank texBank;

HistogramSubWindow::HistogramSubWindow(int iBottomLeftX, int iBottomLeftY, int iWidth, int iHeight) :
	                                   OpenGLSubWindow(iBottomLeftX, iBottomLeftY, iWidth, iHeight)
{
	histogram = new Histogram("Histogram", -128,105, 256,100, 6);
	liGUI_Elements.push_back(histogram);

	label = new Label("Gradient", -128, -165, 6);
	liGUI_Elements.push_back(label);

	m_vfWidth    =  256;	// 2048/8
	m_vfHeight   =  150;	// 1500/10
	m_vfPosX     = -128;
	m_vfPosY     = -140;

	m_gradWidth  =  256;    // 2048/8
	m_gradHeight =   30;    // 1500/50
	m_gradPosX   = -128;
	m_gradPosY   = -200;

	bExposureIsAccomodating = false;
}


HistogramSubWindow::~HistogramSubWindow()
{
	std::vector<GUI_Element*>::iterator iterElement;
	for (iterElement = liGUI_Elements.begin(); iterElement != liGUI_Elements.end(); iterElement++)
		delete (*iterElement);
}

void HistogramSubWindow::Render()
{
	OpenGLSubWindow::Render();

	// Histogram ------------------------------------------------------------------------------------------------
	if (glsl_pipeline.bUseShaderPipeline)
	{
		glUseProgramObjectARB(glsl_pipeline.GPUPrograms["ReduceY"]->programObj);
		glUniform1iARB(glsl_pipeline.GPUPrograms["ReduceY"]->GetUniform("tex0"), 0);
		glUniform1iARB(glsl_pipeline.GPUPrograms["ReduceY"]->GetUniform("iTargetWidth"),  m_vfWidth);
		glUniform1iARB(glsl_pipeline.GPUPrograms["ReduceY"]->GetUniform("iTargetHeight"), m_vfHeight);

		RenderTexturedQuad(texBank[TEXTURE_BAYER2RGB_OUT]->m_uiTextureID,
			               m_vfPosX, m_vfPosY, m_vfWidth, m_vfHeight);

		glUseProgramObjectARB(FFP);
	}
	else
		RenderTexturedQuad(texBank[TEXTURE_CAM_BAYER_OUT]->m_uiTextureID,
			               m_vfPosX, m_vfPosY, m_vfWidth, m_vfHeight);

	ReadPixelsAndUpdateHistogram();
	AccomodateCamExposure();

	// Gradient ------------------------------------------------------------------------------------------------
	if (glsl_pipeline.bUseShaderPipeline)
	{
		glUseProgramObjectARB(glsl_pipeline.GPUPrograms["ReduceRed"]->programObj);
		glUniform1iARB(glsl_pipeline.GPUPrograms["ReduceRed"]->GetUniform("tex0"), 0);
		glUniform1iARB(glsl_pipeline.GPUPrograms["ReduceRed"]->GetUniform("iTargetWidth"),  m_gradWidth);
		glUniform1iARB(glsl_pipeline.GPUPrograms["ReduceRed"]->GetUniform("iTargetHeight"), m_gradHeight);

		RenderTexturedQuad(texBank[TEXTURE_SOBEL_OUT]->m_uiTextureID,
			               m_gradPosX, m_gradPosY, m_gradWidth, m_gradHeight);

		glUseProgramObjectARB(FFP);
	}
	else
		RenderTexturedQuad(texBank[TEXTURE_CAM_BAYER_OUT]->m_uiTextureID,
			               m_gradPosX, m_gradPosY, m_gradWidth, m_gradHeight);

	ReadPixelsAndUpdateGradient();

	std::vector<GUI_Element*>::iterator iterElement;
	for (iterElement = liGUI_Elements.begin(); iterElement != liGUI_Elements.end(); iterElement++)
		(*iterElement)->Draw();
}


void HistogramSubWindow::AccomodateCamExposure()
{
	if (!cam.m_streaming) return;
	if (!cam.bAutoExpo) return;

	if (!bExposureIsAccomodating)
	{
		int iCorrection = histogram->EstimateExposureCorrection();

		if (iCorrection > 0)
		{
			if (cam.ulExposure + iCorrection < cam.ulExpMax)
			{
				cam.SetExposureRows(cam.ulExposure + iCorrection);
				// when this command is sent, the frame has already started with the previous settings
				bExposureIsAccomodating = true;
			}
		}
		if (iCorrection < 0)
		{
			if (cam.ulExposure + iCorrection > cam.ulExpMin)
			{
				cam.SetExposureRows(cam.ulExposure + iCorrection);
				bExposureIsAccomodating = true;
			}
		}
	}
	else
		bExposureIsAccomodating = false;
}


void HistogramSubWindow::ReadPixelsAndUpdateHistogram()
{
	unsigned int nrChannels = 4;
	unsigned int len = int(m_vfWidth*m_vfHeight) * nrChannels;
	unsigned char *data = (unsigned char *)malloc(len);
	memset(data, 0, len);
	
	// in screen coordinates
	glReadPixels(m_iBottomLeftX + m_iWidth/2 + m_vfPosX, m_iBottomLeftY + m_iHeight/2 + m_vfPosY,
				 m_vfWidth, m_vfHeight,
			     GL_RGBA, GL_UNSIGNED_BYTE, data);

	histogram->UpdateHistogram(data, int(m_vfWidth*m_vfHeight), nrChannels);
	
	free(data);
}

void HistogramSubWindow::ReadPixelsAndUpdateGradient()
{
	unsigned int nrChannels = 1;
	unsigned int len = int(m_gradWidth*m_gradHeight) * nrChannels;
	unsigned char *data = (unsigned char *)malloc(len);
	memset(data, 0, len);

	// in screen coordinates
	glReadPixels(m_iBottomLeftX + m_iWidth/2 + m_gradPosX, m_iBottomLeftY + m_iHeight/2 + m_gradPosY,
		         m_gradWidth, m_gradHeight,
		         GL_RED, GL_UNSIGNED_BYTE, data);

	float fSum = 0.0f;
	for (unsigned int j = 0; j < len; j++)
	{
		fSum += data[j];
	}
	fSum /= len;


	char buf[40];
	sprintf(buf, "Gradient  %5.1f", fSum);
	label->_text = buf;

	free(data);
}


void HistogramSubWindow::PassiveMotionFunc(int x, int y)
{
	OpenGLSubWindow::PassiveMotionFunc(x, y);

	SetupGraphicsPipeline();

	Vec3d v3DCoords;
	gluUnProjectFriendly(x, y, 0, v3DCoords.X, v3DCoords.Y, v3DCoords.Z);

	std::vector<GUI_Element*>::iterator iterElement;
	for (iterElement = liGUI_Elements.begin(); iterElement != liGUI_Elements.end(); iterElement++)
		(*iterElement)->Hover(v3DCoords.X, v3DCoords.Y);
}


void HistogramSubWindow::MouseFunc(int button, int state, int x, int y)
{
	OpenGLSubWindow::MouseFunc(button, state, x, y);

	SetupGraphicsPipeline();

	Vec3d v3DCoords;
	gluUnProjectFriendly(x, y, 0, v3DCoords.X, v3DCoords.Y, v3DCoords.Z);

	std::vector<GUI_Element*>::iterator iterElement;
	for (iterElement = liGUI_Elements.begin(); iterElement != liGUI_Elements.end(); iterElement++)
		(*iterElement)->Clicked(button, state, v3DCoords.X, v3DCoords.Y);
}


void HistogramSubWindow::MotionFunc(int x, int y)
{
	OpenGLSubWindow::MotionFunc(x, y);

	SetupGraphicsPipeline();

	Vec3d v3DCoords;
	gluUnProjectFriendly(x, y, 0, v3DCoords.X, v3DCoords.Y, v3DCoords.Z);

	std::vector<GUI_Element*>::iterator iterElement;
	for (iterElement = liGUI_Elements.begin(); iterElement != liGUI_Elements.end(); iterElement++)
		(*iterElement)->Drag(v3DCoords.X, v3DCoords.Y);
}


