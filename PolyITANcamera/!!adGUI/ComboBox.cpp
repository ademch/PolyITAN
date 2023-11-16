#include "ComboBox.h"
#include <GL/gl.h>
#include <GL/glut.h>
#include "../!!adGlobals/adOpenGLUtilities.h"

extern GLFONT font;

ComboBox::ComboBox(std::string caption, int px, int py, int width, float size)
{
	posx = px;
	posy = py;

	glFontGetLength(caption.c_str(), &font, m_TextWidth, m_Height, size);

	m_TextSize=size;
	_text   = caption;
	m_Width = width;

	iListPixelShift = 5;

	bEnabled = true;
	bVisible = true;
	bFocused = false;
	iGUIpushed = 0;
	bExpanded = false;
	iSelected = 0;
	iHovered  = 0;

	vColor_focused   = Vecc4(0.1, 0.8, 0.1, 0.7);
	vColor_defocused = Vecc4(0.1, 0.5, 0.1, 0.7);

	OnClickThis = NULL;
	OnClick     = NULL;
	OnSelect    = NULL;
}

ComboBox::~ComboBox()
{
}


void ComboBox::Draw()
{
	GUI_Element::Draw();

	if (!bVisible) return;

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	// clear area under combobox selection
	glColor3f(0,0,0);
	glQuad(posx, posy, m_Width, m_Height, 3);

	glColor4fv(&vColor_defocused.X);
	glLineWidth(1);
	glWireRectangle(posx, posy, m_Width, m_Height, 5);

	glFontBegin(&font);
		if (items.size() == 0)
			glFontTextOut(_text.c_str(),posx+ m_Height/2.0,posy, 4, m_TextSize);
		else
			glFontTextOut(items[iSelected].c_str(),posx+ m_Height/2.0,posy, 4, m_TextSize);
	glFontEnd();

	glDisable(GL_TEXTURE_2D);


	// draw arrow

	if ((bFocused || bExpanded) && bEnabled)
		glColor4fv(&vColor_focused.X);
	else
		glColor4fv(&vColor_defocused.X);

	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
		glVertex3f(posx + m_Width - 1.0*m_Height + iGUIpushed, posy + 0.65f*m_Height - iGUIpushed, 5);
		glVertex3f(posx + m_Width - 0.7*m_Height + iGUIpushed, posy + 0.35f*m_Height - iGUIpushed, 5);
		glVertex3f(posx + m_Width - 0.4*m_Height + iGUIpushed, posy + 0.65f*m_Height - iGUIpushed, 5);
	glEnd();

	if (bExpanded)
	{
		// clear screen under list
		glColor3f(0, 0, 0);
		glQuad(posx, posy + m_Height + iListPixelShift, m_Width, items.size()*m_Height, 3);

		// hovered item
		glColor4fv(&vColor_defocused.X);
		glQuad(posx, posy + m_Height + iListPixelShift + iHovered*m_Height, m_Width, m_Height, 3.5);

		// draw frame
		glLineWidth(1);
		glWireRectangle(posx, posy + m_Height + iListPixelShift, m_Width, items.size()*m_Height, 5);

		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);	// not sure why it works
		glEnable(GL_BLEND);
		
		glFontBegin(&font);
		unsigned int iListItem = 0;
		std::vector<std::string>::iterator iter;
		for (iter = items.begin(); iter != items.end(); iter++, iListItem++)
		{
			if (iListItem == iHovered) glColor4f(0,0,0,1);
			else glColor4fv(&vColor_defocused.X);

			glFontTextOut(iter->c_str(), posx + m_Height/2, posy + m_Height + iListPixelShift + iListItem*m_Height, 4, m_TextSize);
		}
		glFontEnd();

		glDisable(GL_BLEND);
	}
	glDisable(GL_TEXTURE_2D);

}

bool ComboBox::Clicked(int button, int state, int x, int y)
{
	if (GUI_Element::Clicked(button, state, x, y)) return true;

	if (!bEnabled) return false;

	if (state == GLUT_DOWN)
	{
		// button
		if ((x < posx + m_Width) && (x > posx) && (y < posy + m_Height) && (y > posy))
		{
			iGUIpushed = 1;
			bExpanded = !bExpanded;

			if (bExpanded) iHovered = iSelected;

			return true;
		}

		// list
		if (bExpanded &&
			(x < posx + m_Width) && (x > posx) &&
			(y < posy + iListPixelShift + m_Height + m_Height*items.size()) && (y > posy + m_Height + iListPixelShift))
		{
			int iYLocal = y - posy - m_Height - iListPixelShift;
			iSelected = iYLocal/m_Height;

			bExpanded = false;

			if ((OnClickThis != NULL) && (OnSelect != NULL)) (OnClickThis->*OnSelect)(iSelected);

			return true;
		}
	}
	else
	{
		if (iGUIpushed)
		{
			if ((x < posx + m_Width) && (x > posx) && (y < posy + m_Height) && (y > posy))
			{
				if ((OnClickThis != NULL) && (OnClick != NULL)) (OnClickThis->*OnClick)();
				
				iGUIpushed = 0;
				return true;
			}

			iGUIpushed = 0;
		}

		bExpanded = false;
	}

	return false;
}


void ComboBox::Hover(int x, int y)
{
	GUI_Element::Hover(x, y);

	if (!bEnabled) return;

	if ((x<posx + m_Width) && (x>posx) && (y<posy+m_Height) && (y>posy))
		bFocused=true;
	else
		bFocused=false;

	if (bExpanded)
	{
		if ((x < posx + m_Width) && (x > posx) &&
			(y < posy + iListPixelShift + m_Height + m_Height*items.size()) && (y > posy + m_Height + iListPixelShift))
		{
			int iYLocal = y - posy - m_Height - iListPixelShift;
			iHovered = iYLocal/m_Height;
		}
	}
}


std::string ComboBox::GetSelected()
{
	std::string result("");

	if (items.size() > 0)
		result = items[iSelected];

	return result;
}

 bool ComboBox::SetSelected(std::string _str)
{
	int iListItem = 0;
	std::vector<std::string>::iterator iter;
	for (iter = items.begin(); iter != items.end(); iter++, iListItem++)
	{
		if (iter->compare(_str) == 0)
		{
			iSelected = iListItem;
			return true;
		}
	}

	return false;
}
