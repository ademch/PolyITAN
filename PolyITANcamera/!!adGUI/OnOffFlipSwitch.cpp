#include "OnOffFlipSwitch.h"
#include <GL/glut.h>


OnOffFlipSwitch::OnOffFlipSwitch(std::string strCaption, int px, int py, float scale)
{
	posx = px;
	posy = py;

	glFontGetLength(strCaption.c_str(), &font, _text_width, _text_height, scale);

	_size = scale;
	_text = strCaption;

	m_iBox_width = 40;
	m_iBox_sep = 20;

	bON = false;
	bEnabled = true;

	bPushButton = false;

	OnPreClickThis = NULL;
	OnPreClick     = NULL;

	vColor_focused   = Vecc4(0.1, 0.8, 0.1, 0.7);
	vColor_defocused = Vecc4(0.1, 0.5, 0.1, 0.7);
	
	bFocused=false;
	iGUIpushed = 0;
}

OnOffFlipSwitch::~OnOffFlipSwitch() { }

void OnOffFlipSwitch::Hover(int x, int y)
{
	if ((x < posx + m_iBox_width) && (x > posx) &&
		(y < posy + _text_height) && (y > posy) && bEnabled)
		bFocused=true;
	else
		bFocused=false;
}


void OnOffFlipSwitch::Draw()
{
	GUI_Element::Draw();

	if (bFocused && bEnabled)
		glColor4fv(&vColor_focused.X);
	else
		glColor4fv(&vColor_defocused.X);

	glEnable(GL_TEXTURE_2D);
	glFontBegin(&font);
		glFontTextOut("I", posx + m_iBox_width + 5, posy, 4, _size);
		glFontTextOut("0", posx - 14,               posy, 4, _size);
		glFontTextOut(_text.c_str(), posx + m_iBox_width + m_iBox_sep, posy, 4, _size);
		glFontEnd();
	glDisable(GL_TEXTURE_2D);

	float fBoxHeightScale = 1.0;

	glLineWidth(1);
	glBegin(GL_LINE_LOOP);
		glVertex3f(posx, posy, 4);
		glVertex3f(posx + m_iBox_width, posy, 4);
		glVertex3f(posx + m_iBox_width, posy + _text_height*fBoxHeightScale, 4);
		glVertex3f(posx, posy + _text_height*fBoxHeightScale, 4);
	glEnd();

	if (bON)
	{
		glBegin(GL_QUADS);
			glVertex3f(posx + 3+m_iBox_width*0.5 + iGUIpushed, posy + 3 - iGUIpushed,                4);
			glVertex3f(posx +   m_iBox_width-4 + iGUIpushed,   posy + 3 - iGUIpushed,                4);
			glVertex3f(posx +   m_iBox_width-4 + iGUIpushed,   posy + _text_height - 3 - iGUIpushed, 4);
			glVertex3f(posx + 3+m_iBox_width*0.5 + iGUIpushed, posy + _text_height - 3 - iGUIpushed, 4);
		glEnd();

		glColor3f(0,0.3,0);
		glBegin(GL_LINES);
			glVertex3f(posx + (3 + m_iBox_width*0.5 + m_iBox_width - 4 )/2.0 + iGUIpushed, posy + 3 - iGUIpushed,                5);
			glVertex3f(posx + (3 + m_iBox_width*0.5 + m_iBox_width - 4) /2.0 + iGUIpushed, posy + _text_height - 3 - iGUIpushed, 5);
		glEnd();
	}
	else
	{
		glBegin(GL_QUADS);
			glVertex3f(posx+3 + iGUIpushed,                    posy + 3 - iGUIpushed,                4);
			glVertex3f(posx + m_iBox_width*0.5-4 + iGUIpushed, posy + 3 - iGUIpushed,                4);
			glVertex3f(posx + m_iBox_width*0.5-4 + iGUIpushed, posy + _text_height - 3 - iGUIpushed, 4);
			glVertex3f(posx+3 + iGUIpushed,                    posy + _text_height - 3 - iGUIpushed, 4);
		glEnd();

		glColor3f(0, 0.3, 0);
		glBegin(GL_LINES);
			glVertex3f(posx + (3 + m_iBox_width*0.5 - 4) / 2.0 + iGUIpushed, posy + 3 - iGUIpushed, 5);
			glVertex3f(posx + (3 + m_iBox_width*0.5 - 4) / 2.0 + iGUIpushed, posy + _text_height - 3 - iGUIpushed, 5);
		glEnd();
	}
}


bool OnOffFlipSwitch::Clicked(int button, int state, int x, int y)
{
	if (GUI_Element::Clicked(button, state, x, y)) return true;

	if (!bEnabled) return false;

	if ((state == GLUT_DOWN) &&
		(x < posx + m_iBox_width) && (x > posx) && (y < posy + _text_height) && (y > posy))
	{
		iGUIpushed = 1;

		if (bPushButton)
		{
			if ((OnPreClickThis != NULL) && (OnPreClick != NULL)) {
				if ((OnPreClickThis->*OnPreClick)(!bON))
					bON = !bON;
			}
			else
				bON = !bON;
		}

		return true;
	}
	else
	{
		if (iGUIpushed)
		{
			if ( ((x < posx + m_iBox_width) && (x > posx) && (y < posy + _text_height) && (y > posy) ) ||
				 bPushButton )
			{
				// on/off is triggered on mouse up

				if ((OnPreClickThis != NULL) && (OnPreClick != NULL)) {
					if ((OnPreClickThis->*OnPreClick)(!bON))
						bON = !bON;
				}
				else
					bON = !bON;
				
				iGUIpushed = 0;
				return true;
			}
			iGUIpushed = 0;
		}
	}

	return false;
}

void OnOffFlipSwitch::SetOnOff(bool _bOn, bool bCallCallback)
{
	if (bCallCallback) {
		if ((OnPreClickThis != NULL) && (OnPreClick != NULL) && (_bOn != bON))
		{
			if ((OnPreClickThis->*OnPreClick)(_bOn))
				bON = _bOn;
		}
	}
	else
		bON = _bOn;
}
