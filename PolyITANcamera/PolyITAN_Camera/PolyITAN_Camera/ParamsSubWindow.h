#include "../../!!adGUI/SubWindow.h"
#include "../../!!adGUI/Slider.h"
#include "../../!!adGUI/OnOffFlipSwitch.h"
#include "../../!!adGUI/button.h"
#include <vector>

class SliderG : public Slider<SL_INT>
{
public:
	SliderG(std::string strCaption, int x, int y, float _v_min, float _v_max, float* _v_cur, float scale) :
     Slider(strCaption, x, y, _v_min, _v_max, _v_cur, scale) {};
	
	void Draw()
	{
		Slider::Draw();

		glFontBegin(&font);
			glFontTextOut("analog", posx + 20, posy +13, 5, 5);
			glFontTextOut("digital", posx + m_iBox_width/2.0f + 20, posy +13, 5, 5);
		glFontEnd();
		glDisable(GL_TEXTURE_2D);

		glColor3f(0.9,0.0,0.0);
		glBegin(GL_LINES);
			glVertex3f(posx + m_iBox_width / 2.0, posy, 5);
			glVertex3f(posx + m_iBox_width / 2.0, posy + _text_height, 5);
		glEnd();
	}
};

class SliderGainRGB : public Slider<SL_FLOAT>
{
public:
	SliderGainRGB(std::string strCaption, int x, int y, float _v_min, float _v_max, float* _v_cur, float scale) :
		     Slider(strCaption, x, y, _v_min, _v_max, _v_cur, scale) {};

	void Draw()
	{
		Slider::Draw();

		glColor3f(0.9, 0.0, 0.0);
		glBegin(GL_LINES);
			glVertex3f(posx + m_iBox_width / 2.0, posy, 5);
			glVertex3f(posx + m_iBox_width / 2.0, posy + _text_height, 5);
		glEnd();
	}
};

class ParamsSubWindow : public OpenGLSubWindow
{
public:
	ParamsSubWindow(int iBottomLeftX, int iBottomLeftY, int iWidth, int iHeight);
	~ParamsSubWindow();

	virtual	void Render();

	virtual void PassiveMotionFunc(int x, int y);
	virtual void MouseFunc(int button, int state, int x, int y);
	virtual void MotionFunc(int x, int y);
	
	bool CameraConnect(bool bON_Request);
	bool CameraBasicInfo();
	bool CameraInfo();
	bool CameraSetGain();
	bool CameraSetExposure();
	bool CameraSetFrameRate();
	bool SetDenoise();
	bool CameraStream(bool bON_Request);
	bool SaveFrame();
	bool Demosaic(bool bON);
	bool CameraSetGainRGB();
	bool CameraAutoExpo(bool bON_Request);

	virtual void SetupGraphicsPipelineOnly();

	float fCameraGain;
	float fCameraExposure_db;
	float fCameraFPS;
	float fDenoise;

	float fCameraR_Gain;
	float fCameraG_Gain;
	float fCameraB_Gain;
protected:

	int iPointBeginDragX;
	int iPointBeginDragY;
	float fZBegin;
	bool bPointDragInProgress;

	std::vector<GUI_Element*> liGUI_Elements;

private:
	SliderG *SliderGain;
	Slider<SL_FLOAT> *SliderExposure;
	Slider<SL_FLOAT> *SliderFPS;
	Slider<SL_FLOAT> *SliderDenoise;
	
	OnOffFlipSwitch* onoffswitchConnect;
	OnOffFlipSwitch* onoffswitchStream;
	OnOffFlipSwitch* onoffswitchAutoExpo;
	Button* buttonBasicInfo;
	Button* buttonInfo;
	Button* buttonSaveFrame;

	SliderGainRGB* SliderBiasR;
	SliderGainRGB* SliderBiasG;
	SliderGainRGB* SliderBiasB;

	float db2ms(float val);
	float ms2db(float val);
};
