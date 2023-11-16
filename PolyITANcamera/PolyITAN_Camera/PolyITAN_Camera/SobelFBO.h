#include "../../!!adGUI/SubWindow.h"
#include "../../!!adExtensions/Shaders.h"


class SobelFBO : public OpenGLSubWindow
{
public:
	SobelFBO(int iBottomLeftX, int iBottomLeftY, int iWidth, int iHeight);
	~SobelFBO();

	virtual	void Render();

	FrameBufferObject* fbo;

protected:
private:
};