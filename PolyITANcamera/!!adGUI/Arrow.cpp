#include "Arrow.h"
#include <GL/gl.h>

extern GLFONT font;

Arrow::Arrow(std::string caption, int px, int py, int length, float size)
{
	posx = px;
	posy = py;

	glFontGetLength(caption.c_str(), &font, m_TextWidth, m_Height, size);

	_size = size;
	_text = caption;
	_length = length;

	vColor = Vecc4(0.1f, 0.7f, 0.1f, 1.0f);
}


void Arrow::Draw()
{
	GUI_Element::Draw();

	glColor4fv(&vColor.X);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glFontBegin(&font);
		glFontTextOut(_text.c_str(), posx + _length/2.0f - m_TextWidth/2.0f, float(posy) + 5, 4.0f, _size);
	glFontEnd();

	glDisable(GL_BLEND);

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
		glVertex3i(posx,             posy,    4);
		glVertex3i(posx + _length,   posy,    4);

		glVertex3i(posx + _length-1, posy +1, 4);
		glVertex3i(posx + _length-7, posy +4, 4);

		glVertex3i(posx + _length-1, posy,    4);
		glVertex3i(posx + _length-7, posy -3, 4);
	glEnd();
}

