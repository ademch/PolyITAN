#include "../../!!adGUI/SubWindow.h"
#include "../../!!adExtensions/Shaders.h"


class Bayer2rgbFBO : public OpenGLSubWindow
{
public:
	Bayer2rgbFBO(int iBottomLeftX, int iBottomLeftY, int iWidth, int iHeight);
	~Bayer2rgbFBO();

	virtual	void Render();

	FrameBufferObject* fbo;

protected:
private:
};