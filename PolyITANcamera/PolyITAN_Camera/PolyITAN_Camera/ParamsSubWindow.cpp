#include "ParamsSubWindow.h"
#include <GL/glut.h>
#include "../../!!adGlobals/adOpenGLUtilities.h"
#include "Camera.h"
#include "../../!!adGUI/fps.h"
#include "GLSL_Pipeline.h"
#include "../../!!adExtensions/extensions.h"


extern Camera cam;
extern GLSL_Pipeline glsl_pipeline;
void keyboard(unsigned char, int, int);

ParamsSubWindow::ParamsSubWindow(int iBottomLeftX, int iBottomLeftY, int iWidth, int iHeight) :
								 OpenGLSubWindow(iBottomLeftX, iBottomLeftY, iWidth, iHeight)
{
	bPointDragInProgress = false;

	fCameraGain = 1.0;
	SliderGain = new SliderG("x0.1db Gain", -m_iWidth/2 + 30, m_iHeight/2-40, 0,480, &fCameraGain, 7);
	SliderGain->SetBoxWidth(200);
	SliderGain->SetBoxSeparation(1);
	SliderGain->fTickGranularity = 10;
	SliderGain->bEnabled = false;
	SliderGain->OnClickThis = this;
	SliderGain->OnClick = (bool(OpenGLSubWindow::*)())&ParamsSubWindow::CameraSetGain;
	liGUI_Elements.push_back(SliderGain);

	// using linear scale -20 to 40 and applying log function to it yields decent adaptive granularity of ms value and its magnitude
	// E.g.:
	fCameraExposure_db = ms2db(1.0);	// 0db stands for 1ms
	SliderExposure = new Slider<SL_FLOAT>("ms Expo", -m_iWidth / 2 + 30, m_iHeight / 2 - 80, -20, 40, &fCameraExposure_db, 7);
	SliderExposure->SetBoxWidth(200);
	SliderExposure->SetBoxSeparation(1);
	SliderExposure->fTickGranularity = 1;
	SliderExposure->bEnabled = false;
	SliderExposure->OnClickThis = this;
	SliderExposure->OnDrawValue = (float(OpenGLSubWindow::*)(float))&ParamsSubWindow::db2ms;
	SliderExposure->OnClick = (bool(OpenGLSubWindow::*)())&ParamsSubWindow::CameraSetExposure;
	liGUI_Elements.push_back(SliderExposure);

	fCameraFPS = 1.0;
	// 100ms (40db) set as the upper limit for exposure slider put the upper limit of 10fps
	SliderFPS = new Slider<SL_FLOAT>("fps", -m_iWidth / 2 + 30, m_iHeight / 2 - 120, 1,10, &fCameraFPS, 7);
	SliderFPS->SetBoxWidth(200);
	SliderFPS->SetBoxSeparation(1);
	SliderFPS->fValueGranularity = 0.5;
	SliderFPS->bEnabled = false;
	SliderFPS->OnClickThis = this;
	SliderFPS->OnClick = (bool(OpenGLSubWindow::*)())&ParamsSubWindow::CameraSetFrameRate;
	liGUI_Elements.push_back(SliderFPS);

	fDenoise = 3.0;
	glsl_pipeline.fDenoiseSqr = fDenoise*fDenoise;  //!! unfortunately stored also here
	SliderDenoise = new Slider<SL_FLOAT>("Denoise", -m_iWidth / 2 + 30, m_iHeight / 2 - 360, 1,10, &fDenoise, 7);
	SliderDenoise->SetBoxWidth(200);
	SliderDenoise->SetBoxSeparation(1);
	SliderDenoise->fValueGranularity = 0.1;
	//SliderDenoise->bEnabled = false;
	SliderDenoise->OnClickThis = this;
	SliderDenoise->OnClick = (bool(OpenGLSubWindow::*)())&ParamsSubWindow::SetDenoise;
	SliderDenoise->OnClickDrag = (bool(OpenGLSubWindow::*)())&ParamsSubWindow::SetDenoise;
	liGUI_Elements.push_back(SliderDenoise);


	onoffswitchConnect = new OnOffFlipSwitch("Connect", -m_iWidth / 2 + 30, m_iHeight / 2 - 160, 6);
	onoffswitchConnect->OnPreClickThis = this;
	onoffswitchConnect->OnPreClick = (bool(OpenGLSubWindow::*)(bool))&ParamsSubWindow::CameraConnect;
	liGUI_Elements.push_back(onoffswitchConnect);

	onoffswitchStream = new OnOffFlipSwitch("Stream", -m_iWidth / 2 + 30, m_iHeight / 2 - 190, 6);
	onoffswitchStream->bEnabled = false;
	onoffswitchStream->OnPreClickThis = this;
	onoffswitchStream->OnPreClick = (bool(OpenGLSubWindow::*)(bool))&ParamsSubWindow::CameraStream;
	liGUI_Elements.push_back(onoffswitchStream);

	onoffswitchAutoExpo = new OnOffFlipSwitch("Auto expo", -m_iWidth / 2 + 190, m_iHeight / 2 - 160, 6);
	onoffswitchAutoExpo->bEnabled = false;
	onoffswitchAutoExpo->OnPreClickThis = this;
	onoffswitchAutoExpo->OnPreClick = (bool(OpenGLSubWindow::*)(bool))&ParamsSubWindow::CameraAutoExpo;
	liGUI_Elements.push_back(onoffswitchAutoExpo);

	OnOffFlipSwitch* onoffswitchDemosaic = new OnOffFlipSwitch("Demosaic", -m_iWidth / 2 + 190, m_iHeight / 2 - 190, 6);
	onoffswitchDemosaic->OnPreClickThis = this;
	onoffswitchDemosaic->OnPreClick = (bool(OpenGLSubWindow::*)(bool))&ParamsSubWindow::Demosaic;
	liGUI_Elements.push_back(onoffswitchDemosaic);

	buttonBasicInfo = new Button("Basic info", -m_iWidth / 2 + 200, m_iHeight / 2 - 430, 120, 6);
	buttonBasicInfo->bEnabled = false;
	buttonBasicInfo->OnClickThis = this;
	buttonBasicInfo->OnClick = (bool(OpenGLSubWindow::*)())&ParamsSubWindow::CameraBasicInfo;
	liGUI_Elements.push_back(buttonBasicInfo);

	buttonInfo = new Button("Dynamic info", -m_iWidth / 2 + 200, m_iHeight / 2 - 400, 120, 6);
	buttonInfo->bEnabled = false;
	buttonInfo->OnClickThis = this;
	buttonInfo->OnClick = (bool(OpenGLSubWindow::*)())&ParamsSubWindow::CameraInfo;
	liGUI_Elements.push_back(buttonInfo);

	buttonSaveFrame = new Button("Save frame", -m_iWidth / 2 + 30, m_iHeight / 2 - 430, 120, 6);
	//buttonSaveFrame->bEnabled = false;
	buttonSaveFrame->OnClickThis = this;
	buttonSaveFrame->OnClick = (bool(OpenGLSubWindow::*)())&ParamsSubWindow::SaveFrame;
	liGUI_Elements.push_back(buttonSaveFrame);

	FPS* fpsElement = new FPS(-m_iWidth / 2 + 30, m_iHeight / 2 - 400, 6);
	liGUI_Elements.push_back(fpsElement);

	// Color bias----------------------------------------------------------------------------------------

	fCameraR_Gain = 1.77;
	SliderBiasR = new SliderGainRGB("db Red", -m_iWidth / 2 + 30, m_iHeight / 2 - 230, -3, 3, &fCameraR_Gain, 7);
	SliderBiasR->SetBoxWidth(200);
	SliderBiasR->SetBoxSeparation(1);
	SliderBiasR->fTickGranularity = 0.5;
	SliderBiasR->bEnabled = false;
	SliderBiasR->OnClickThis = this;
	SliderBiasR->OnClick     = (bool(OpenGLSubWindow::*)())&ParamsSubWindow::CameraSetGainRGB;
	SliderBiasR->OnClickDrag = (bool(OpenGLSubWindow::*)())&ParamsSubWindow::CameraSetGainRGB;
	liGUI_Elements.push_back(SliderBiasR);

	fCameraG_Gain = -2.76;
	SliderBiasG = new SliderGainRGB("db Green", -m_iWidth / 2 + 30, m_iHeight / 2 - 270, -6, 6, &fCameraG_Gain, 7);
	SliderBiasG->SetBoxWidth(200);
	SliderBiasG->SetBoxSeparation(1);
	SliderBiasG->fTickGranularity = 0.5;
	SliderBiasG->bEnabled = false;
	SliderBiasG->OnClickThis = this;
	SliderBiasG->OnClick     = (bool(OpenGLSubWindow::*)())&ParamsSubWindow::CameraSetGainRGB;
	SliderBiasG->OnClickDrag = (bool(OpenGLSubWindow::*)())&ParamsSubWindow::CameraSetGainRGB;
	liGUI_Elements.push_back(SliderBiasG);

	fCameraB_Gain = 1.77;
	SliderBiasB = new SliderGainRGB("db Blue", -m_iWidth / 2 + 30, m_iHeight / 2 - 310, -3, 3, &fCameraB_Gain, 7);
	SliderBiasB->SetBoxWidth(200);
	SliderBiasB->SetBoxSeparation(1);
	SliderBiasB->fTickGranularity = 0.5;
	SliderBiasB->bEnabled = false;
	SliderBiasB->OnClickThis = this;
	SliderBiasB->OnClick     = (bool(OpenGLSubWindow::*)())&ParamsSubWindow::CameraSetGainRGB;
	SliderBiasB->OnClickDrag = (bool(OpenGLSubWindow::*)())&ParamsSubWindow::CameraSetGainRGB;
	liGUI_Elements.push_back(SliderBiasB);

	// Sliders only pass values upon event, that's why we need to manually pass their values to the camera object
	CameraSetGainRGB();
}

ParamsSubWindow::~ParamsSubWindow()
{
	std::vector<GUI_Element*>::iterator iterElement;
	for (iterElement = liGUI_Elements.begin(); iterElement != liGUI_Elements.end(); iterElement++)
		delete (*iterElement);
}

void ParamsSubWindow::Render()
{
	OpenGLSubWindow::Render();

	if (cam.bAutoExpo)
		SliderExposure->SetValue(ms2db(cam.ulExposure*0.021713f), /*cam.fExpMinMs*/-20.0f, ms2db(cam.fExpMaxMs));

	glDisable(GL_LIGHTING);

	std::vector<GUI_Element*>::iterator iterElement;
	for (iterElement = liGUI_Elements.begin(); iterElement != liGUI_Elements.end(); iterElement++)
		(*iterElement)->Draw();
}

void ParamsSubWindow::SetupGraphicsPipelineOnly()
{
	OpenGLSubWindow::SetupGraphicsPipeline();
}

void ParamsSubWindow::PassiveMotionFunc(int x, int y)
{
	OpenGLSubWindow::PassiveMotionFunc(x, y);

	SetupGraphicsPipelineOnly();

	Vec3d v3DCoords;
	gluUnProjectFriendly(x, y, 0, v3DCoords.X, v3DCoords.Y, v3DCoords.Z);

	std::vector<GUI_Element*>::iterator iterElement;
	for (iterElement = liGUI_Elements.begin(); iterElement != liGUI_Elements.end(); iterElement++)
		(*iterElement)->Hover(v3DCoords.X, v3DCoords.Y);
}

void ParamsSubWindow::MouseFunc(int button, int state, int x, int y)
{
	OpenGLSubWindow::MouseFunc(button, state, x, y);

	SetupGraphicsPipelineOnly();

	Vec3d v3DCoords;
	gluUnProjectFriendly(x, y, 0, v3DCoords.X, v3DCoords.Y, v3DCoords.Z);

	std::vector<GUI_Element*>::iterator iterElement;
	for (iterElement = liGUI_Elements.begin(); iterElement != liGUI_Elements.end(); iterElement++)
		(*iterElement)->Clicked(button, state, v3DCoords.X, v3DCoords.Y);
}


void ParamsSubWindow::MotionFunc(int x, int y)
{
	OpenGLSubWindow::MotionFunc(x, y);

	SetupGraphicsPipelineOnly();

	Vec3d v3DCoords;
	gluUnProjectFriendly(x, y, 0, v3DCoords.X, v3DCoords.Y, v3DCoords.Z);

	std::vector<GUI_Element*>::iterator iterElement;
	for (iterElement = liGUI_Elements.begin(); iterElement != liGUI_Elements.end(); iterElement++)
		(*iterElement)->Drag(v3DCoords.X, v3DCoords.Y);
}


bool ParamsSubWindow::CameraConnect(bool bON_Request)
{
	bool result;

	if (bON_Request)
	{
		result = cam.Connect();

		onoffswitchStream->bEnabled = result;
		buttonBasicInfo->bEnabled = result;
		buttonInfo->bEnabled = result;
		SliderGain->bEnabled = result;
		SliderExposure->bEnabled = result;
		SliderFPS->bEnabled = result;

		if (result)
		{
			// Assign real value to the slider
			if (cam.GetGain())
				SliderGain->SetValue(cam.fGainA, 0, 480);

			// Force 5 fps at the start
			printf("Starting at 5fps...\n");
			fCameraFPS = 5;
			this->CameraSetFrameRate();
		}
	}
	else
	{
		result = cam.Disconnect();
		if (result)
		{
			onoffswitchStream->SetOnOff(false, false);
			onoffswitchAutoExpo->SetOnOff(false, false);

			// assigning false instead of result to prevent disable on unsuccessful disconnect
			onoffswitchStream->bEnabled = false;
			onoffswitchAutoExpo->bEnabled = false;
			buttonBasicInfo->bEnabled = false;
			buttonInfo->bEnabled = false;
			SliderGain->bEnabled = false;
			SliderExposure->bEnabled = false;
			SliderFPS->bEnabled = false;
			buttonSaveFrame->bEnabled = false;
		}
	}
	return result;
}


bool ParamsSubWindow::CameraStream(bool bON_Request)
{
	bool result;

	if (bON_Request)
	{
		result = cam.StartStream();
		onoffswitchAutoExpo->bEnabled = result;
		buttonSaveFrame->bEnabled = result;
	}
	else
	{
		result = cam.StopStream();
		onoffswitchAutoExpo->SetOnOff(false, false);
		onoffswitchAutoExpo->bEnabled = false;
		buttonSaveFrame->bEnabled = false;
	}

	return result;
}

bool ParamsSubWindow::CameraAutoExpo(bool bON_Request)
{
	if (bON_Request)
	{
		cam.bAutoExpo = true;
		SliderExposure->bEnabled = false;
	}
	else
	{
		cam.bAutoExpo = false;
		SliderExposure->bEnabled = true;
	}

	return true;
}

bool ParamsSubWindow::CameraBasicInfo()
{
	cam.GetCameraInfo();
	cam.GetFrequency();
	cam.GetFrameParams();

	return true;
}

bool ParamsSubWindow::CameraInfo()
{
	cam.GetFrameRateMinMax();
	cam.GetFrameRate();
	cam.GetExposureRowsMinMax();
	cam.GetExposureRows();
	cam.GetGain();

	return true;
}


bool ParamsSubWindow::SaveFrame()
{
	//cam.bSaveFramePending = true;

	//get the HWND
//	HWND handle = WindowFromDC(wglGetCurrentDC());
//	PostMessage(handle, WM_KEYDOWN, 0x31, 0);	// 1

	keyboard(0x31, 0, 0);

	return true;
}

bool ParamsSubWindow::Demosaic(bool bON_Request)
{
	if (!glsl_pipeline.bUseShaderPipeline)
	{
		assert(bON_Request);

		printf("Compiling shaders...\n");
			if (!glsl_pipeline.init_shaders()) return false;
		printf("done\n\n");

		glsl_pipeline.bUseShaderPipeline = true;
	}
	else
	{
		assert(!bON_Request);

		printf("Deleting shaders...\n");
			glsl_pipeline.destroy_shaders();
		printf("done\n\n");

		glsl_pipeline.bUseShaderPipeline = false;
	}

	SliderBiasR->bEnabled = bON_Request;
	SliderBiasG->bEnabled = bON_Request;
	SliderBiasB->bEnabled = bON_Request;

	return true;
}

bool ParamsSubWindow::CameraSetGain()
{
	cam.fGainA = fCameraGain;
	cam.SetGain();
	//cam.GetGain();	// no sence, usually evrth is ok

	return true;
}

bool ParamsSubWindow::CameraSetGainRGB()
{
	cam.vGainRGB = Vecc3(fCameraR_Gain, fCameraG_Gain, fCameraB_Gain);

	return true;
}

bool ParamsSubWindow::CameraSetExposure()
{
	cam.SetExposureRows( db2ms(fCameraExposure_db)/RowMs );
	// read the real value set
	fCameraExposure_db = ms2db(cam.fExposureMs);

	return true;
}

float ParamsSubWindow::db2ms(float val)
{
	return Camera::db2ms(val);
}

float ParamsSubWindow::ms2db(float val)
{
	return Camera::ms2db(val);
}


bool ParamsSubWindow::CameraSetFrameRate()
{
	cam.SetFrameRate(fCameraFPS);
	fCameraFPS = cam.fFrameRate;	// may be lower than the slider lower threshold

	// Force 1 ms as the starting value, as the camera sets some awkward value when it changes Frame rate
	cam.SetExposureRows(int(1.0f / RowMs));	// ms/ms
	cam.GetExposureRowsMinMax();

	SliderExposure->SetValue(ms2db(cam.fExposureMs), /*cam.fExpMinMs*/-20.0f, ms2db(cam.fExpMaxMs));

	return true;
}


bool ParamsSubWindow::SetDenoise()
{
	glsl_pipeline.fDenoiseSqr = fDenoise*fDenoise;

	return true;
}
