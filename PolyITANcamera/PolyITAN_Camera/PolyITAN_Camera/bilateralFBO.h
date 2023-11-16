#include "../../!!adGUI/SubWindow.h"
#include "../../!!adExtensions/Shaders.h"


class bilateralFBO : public OpenGLSubWindow
{
public:
	bilateralFBO(int iBottomLeftX, int iBottomLeftY, int iWidth, int iHeight);
	~bilateralFBO();

	virtual	void Render();

	FrameBufferObject* fbo;

protected:
private:
};