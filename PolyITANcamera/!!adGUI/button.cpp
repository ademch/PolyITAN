#include "button.h"
#include <GL/gl.h>
#include <GL/glut.h>
#include "../!!adGlobals/adOpenGLUtilities.h"

extern GLFONT font;

Button::Button(std::string caption, int px, int py, int width, float size)
{
	posx = px;
	posy = py;

	glFontGetLength(caption.c_str(),&font, m_TextWidth, m_Height, size);

	m_TextSize = size;
	_text      = caption;
	m_Width    = width;

	bEnabled = true;
	bFocused = false;
	iGUIpushed = 0;

	vColor_focused   = Vecc4(0.1, 0.8, 0.1, 0.7);
	vColor_defocused = Vecc4(0.1, 0.5, 0.1, 0.7);

	OnClickThis = NULL;
}

Button::~Button()
{
}

void Button::Hover(int x, int y)
{
	GUI_Element::Hover(x, y);

	if ((x<posx + m_Width) && (x>posx) && (y<posy+m_Height) && (y>posy) && bEnabled)
		bFocused=true;
	else
		bFocused=false;
}


void Button::Draw()
{
	GUI_Element::Draw();

	// clear screen under button
	glColor3f(0, 0, 0);
	glQuad(posx + iGUIpushed, posy - iGUIpushed, m_Width, m_Height+1, 3);

	if (bFocused && bEnabled)
		glColor4fv(&vColor_focused.X);
	else
		glColor4fv(&vColor_defocused.X);

	glFontBegin(&font);
		glFontTextOut(_text.c_str(),posx+ (m_Width-m_TextWidth)/2.0 + iGUIpushed,posy - iGUIpushed, 4,m_TextSize);
	glFontEnd();

	glDisable(GL_TEXTURE_2D);

	glLineWidth(1);
	glWireRectangle(posx + iGUIpushed, posy - iGUIpushed, m_Width, m_Height+1, 4.5);
}

bool Button::Clicked(int button, int state, int x, int y)
{

	if (GUI_Element::Clicked(button, state, x, y)) return true;

	if (!bEnabled) return false;

	if ((state == GLUT_DOWN) && (x < posx + m_Width) && (x > posx) && (y < posy + m_Height) && (y > posy))
	{
		iGUIpushed = 1;
		return true;
	}
	else
	{
		if (iGUIpushed)
		{
			if ((x < posx + m_Width) && (x > posx) && (y < posy + m_Height) && (y > posy))
			{
				if (OnClickThis != NULL) (OnClickThis->*OnClick)();
				
				iGUIpushed = 0;
				return true;
			}

			iGUIpushed = 0;
		}
	}

	return false;
}
