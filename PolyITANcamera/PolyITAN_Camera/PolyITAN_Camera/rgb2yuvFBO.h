#include "../../!!adGUI/SubWindow.h"
#include "../../!!adExtensions/Shaders.h"


class rgb2yuvFBO : public OpenGLSubWindow
{
public:
	rgb2yuvFBO(int iBottomLeftX, int iBottomLeftY, int iWidth, int iHeight);
	~rgb2yuvFBO();

	virtual	void Render();

	FrameBufferObject* fbo;

	virtual bool KeyboardFunc(unsigned char key, int x, int y);

protected:
private:
};