#include "../../!!adGUI/SubWindow.h"
#include <vector>
#include "../../!!adGUI/gui_element.h"
#include "../../!!adGUI/ComboBox.h"
#include "../../!!adGUI/button.h"


class CameraSubWindow : public OpenGLSubWindow
{
public:
	CameraSubWindow(int iBottomLeftX, int iBottomLeftY, int iWidth, int iHeight);
	~CameraSubWindow();

	virtual	void Render();

	virtual void PassiveMotionFunc(int x, int y);
	virtual void MouseFunc(int button, int state, int x, int y);
	virtual void MotionFunc(int x, int y);
	virtual bool KeyboardFunc(unsigned char key, int x, int y);

protected:
	std::vector<GUI_Element*> liGUI_Elements;

	ComboBox* comboBox;
	Button* buttonResetView;

	bool ResetView();

private:

	float m_fQuadPixW;
	float m_fQuadPixH;
};