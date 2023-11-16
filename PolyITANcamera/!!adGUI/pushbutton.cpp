#include "pushbutton.h"
#include <GL/gl.h>
#include <GL/glut.h>

const float i_alpha=0.4;

PushButton::PushButton(std::string caption, float size, float r, float g,float b, int px, int py, bool active):
Button(caption, size, r, g,b, px, py)
{
	if (active)
		flare=true;
	else
		flare=false;
}

PushButton::PushButton()
{
	_color.alpha=i_alpha;
	flare=false;
}

PushButton::~PushButton()
{
}

void PushButton::Hover(int x, int y)
{
	if ((x<posx + width) && (x>posx) && (y<posy+height) && (y>posy))
	{	glutSetCursor(GLUT_CURSOR_INHERIT);}
}

bool PushButton::ClickedD(int button, int state, int x, int y)
{
bool result;
	
	if ((state==GLUT_DOWN) && (x<posx + width) && (x>posx) && (y<posy+height) && (y>posy))
		result=true;
	else
		result=false;

	if (result)
	{
		if (flare)
			flare=false;
		else
			flare=true;
	}

return result;
}

bool PushButton::IsActive()
{ return flare; }

