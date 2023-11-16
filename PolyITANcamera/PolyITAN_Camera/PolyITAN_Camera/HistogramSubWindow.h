#include "../../!!adGUI/SubWindow.h"
#include "../../!!adGUI/OnOffFlipSwitch.h"
#include "../../!!adGUI/button.h"
#include "../../!!adGUI/Histogram.h"
#include "../../!!adGUI/label.h"
#include <vector>

class HistogramSubWindow : public OpenGLSubWindow
{
public:
	HistogramSubWindow(int iBottomLeftX, int iBottomLeftY, int iWidth, int iHeight);
	~HistogramSubWindow();

	virtual	void Render();

	void AccomodateCamExposure();

	virtual void PassiveMotionFunc(int x, int y);
	virtual void MouseFunc(int button, int state, int x, int y);
	virtual void MotionFunc(int x, int y);

protected:

	Histogram* histogram;
	Label* label;

	std::vector<GUI_Element*> liGUI_Elements;

	void ReadPixelsAndUpdateHistogram();
	void ReadPixelsAndUpdateGradient();

	// view finder
	float m_vfWidth;
	float m_vfHeight;
	float m_vfPosX;
	float m_vfPosY;

	// view finder
	float m_gradWidth;
	float m_gradHeight;
	float m_gradPosX;
	float m_gradPosY;

private:
	bool bExposureIsAccomodating;
};