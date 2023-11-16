#include "label.h"
#include <GL/gl.h>

extern GLFONT font;

Label::Label(std::string caption, int px, int py, float size)
{
	posx = px;
	posy = py;

	_text=caption;
	_size=size;

	vColor = Vecc4(0.1f, 0.5f, 0.1f, 1.0f);
}


Label::~Label()
{
}


void Label::Draw()
{
	GUI_Element::Draw();

	glColor4fv(&vColor.X);

	glFontBegin(&font);
		glFontTextOut(_text.c_str(), float(posx), float(posy), -4.0f, _size);
	glFontEnd();

	glDisable(GL_TEXTURE_2D);

}

void Label::UpdateCaption(std::string caption)
{
	_text=caption;
}

