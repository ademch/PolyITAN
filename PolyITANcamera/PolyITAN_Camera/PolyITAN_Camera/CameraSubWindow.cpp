#include "CameraSubWindow.h"
#include <GL/glut.h>
#include "Camera.h"
#include "GLSL_Pipeline.h"
#include "../../!!adExtensions/extensions.h"
#include "../../!!adGlobals/TextureDescriptor.h"
#include "../../!!adGlobals/adOpenGLUtilities.h"


extern Camera cam;
extern GLSL_Pipeline glsl_pipeline;
extern TextureBank texBank;


CameraSubWindow::CameraSubWindow(int iBottomLeftX, int iBottomLeftY, int iWidth, int iHeight) :
	             OpenGLSubWindow(iBottomLeftX, iBottomLeftY, iWidth, iHeight)
{
	float fK = 7.5f;
	m_fQuadPixW = 4096.0f / fK;
	m_fQuadPixH = 3000.0f / fK;

	comboBox = new ComboBox("Default", iWidth/2 - 180, -iHeight/2 + 10, 170, 6.5);
	liGUI_Elements.push_back(comboBox);

	buttonResetView = new Button("Reset view", iWidth/2 -300, -iHeight/2 + 10, 100, 6);
	buttonResetView->OnClickThis = this;
	buttonResetView->OnClick = (bool( OpenGLSubWindow::*)())&CameraSubWindow::ResetView;
	liGUI_Elements.push_back(buttonResetView);

	// register duplicate entry (that is compulsory visualized with BayerR shader)
	texBank.bank[TEXTURE_RGBG_OUT] = new TextureDescriptor(*texBank[TEXTURE_CAM_BAYER_OUT]);

	typeTexBankIter iter;
	for (iter = texBank.bank.begin(); iter != texBank.bank.end(); ++iter)
		comboBox->items.push_back( iter->first.c_str() );

	comboBox->SetSelected(TEXTURE_CAM_BAYER_OUT);
}


CameraSubWindow::~CameraSubWindow()
{
	std::vector<GUI_Element*>::iterator iterElement;
	for (iterElement = liGUI_Elements.begin(); iterElement != liGUI_Elements.end(); iterElement++)
		delete (*iterElement);
}


void CameraSubWindow::Render()
{
	OpenGLSubWindow::Render();
	
	if ((comboBox->GetSelected() == TEXTURE_RGBG_OUT) && (glsl_pipeline.bUseShaderPipeline))
	{
		glUseProgramObjectARB(glsl_pipeline.GPUPrograms["BayerR"]->programObj);
		glUniform1iARB(glsl_pipeline.GPUPrograms["BayerR"]->GetUniform("tex0"), 0);
		glUniform3fARB(glsl_pipeline.GPUPrograms["BayerR"]->GetUniform("vGain"),
					   pow(10, cam.vGainRGB.X / 20), pow(10, cam.vGainRGB.Y / 20), pow(10, cam.vGainRGB.Z / 20));	// db to gain

		RenderTexturedQuad(texBank[TEXTURE_CAM_BAYER_OUT]->m_uiTextureID,
			               -m_fQuadPixW/2, -m_fQuadPixH/2, m_fQuadPixW, m_fQuadPixH);

		glUseProgramObjectARB(FFP);
	}
	else
	{
		// feed texture from user selected source
		std::string sSelected = comboBox->GetSelected();
		RenderTexturedQuad(texBank[sSelected]->m_uiTextureID,
						   -m_fQuadPixW/2, -m_fQuadPixH/2, m_fQuadPixW, m_fQuadPixH);
	}

	if (fUserScale >= 1.0)
		sprintf(m_strCaption, "%s (%g:1)", "Sensor", fUserScale);
	else
		sprintf(m_strCaption, "%s (1:%g)", "Sensor", 1.0f/fUserScale);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for (auto iterElement : liGUI_Elements)
		iterElement->Draw();
}


void CameraSubWindow::PassiveMotionFunc(int x, int y)
{
	OpenGLSubWindow::PassiveMotionFunc(x, y);

	SetupGraphicsPipeline();
	// reset transformation working with GUI
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vec3d v3DCoords;
	gluUnProjectFriendly(x, y, 0, v3DCoords.X, v3DCoords.Y, v3DCoords.Z);

	for (auto iterElement : liGUI_Elements)
		iterElement->Hover(v3DCoords.X, v3DCoords.Y);
}


void CameraSubWindow::MouseFunc(int button, int state, int x, int y)
{
	OpenGLSubWindow::MouseFunc(button, state, x, y);

	SetupGraphicsPipeline();
	// reset transformation working with GUI
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vec3d v3DCoords;
	gluUnProjectFriendly(x, y, 0, v3DCoords.X, v3DCoords.Y, v3DCoords.Z);

	for (auto iterElement : liGUI_Elements)
		iterElement->Clicked(button, state, v3DCoords.X, v3DCoords.Y);
}


void CameraSubWindow::MotionFunc(int x, int y)
{
	OpenGLSubWindow::MotionFunc(x, y);

	SetupGraphicsPipeline();
	// reset transformation working with GUI
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vec3d v3DCoords;
	gluUnProjectFriendly(x, y, 0, v3DCoords.X, v3DCoords.Y, v3DCoords.Z);

	for (auto iterElement : liGUI_Elements)
		iterElement->Drag(v3DCoords.X, v3DCoords.Y);
}

bool CameraSubWindow::ResetView()
{
	OpenGLSubWindow::ResetView();

	return true;
}


bool CameraSubWindow::KeyboardFunc(unsigned char key, int x, int y)
{
	bool res = false;

	if (OpenGLSubWindow::KeyboardFunc(key, x, y)) return true;

	switch (key)
	{
	case '0':
	{
		break;
	}
	break;
	}

	return res;
}






