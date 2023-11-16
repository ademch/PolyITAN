
#include "ColorMap.h"
#include <GL/glut.h>
#include "../!!adGUI/glfont.h"

extern GLFONT font;
extern int window_w;
extern int window_h;


ColorMap::ColorMap(std::string str, int x, int y, float _v_min, float _v_max):
posx(x), posy(y), _text(str), val_min(_v_min),val_max(_v_max)
{
	assert(_v_min < _v_max);

	_text_scale = 5;
	_text_sep = 10;

	glFontGetLength(str.c_str(), &font, _text_width, _text_height, _text_scale);
	_box_width =70;
	_box_height=250;

	active = false;
	active_col   = Vecc4(0.9,0.1,0.1, 0.7);
	inactive_col = Vecc4(0.1,0.5,0.1, 0.7);

	start_col = Vecc3(0.0,0.0,1.0);
	end_col   = Vecc3(1.0,0.0,0.0);
}

void ColorMap::Draw()
{
float t;
Vec3 curColor;

	if (active)
		glColor4fv(&active_col.X);
	else
		glColor4fv(&inactive_col.X);

	glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, window_w, 0, window_h, -400, 400);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		glFontBegin (&font);
		glLoadIdentity();
			glFontTextOut(_text.c_str(), posx, window_h - posy, 4, _text_scale);
		glFontEnd();

		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINES);
			for (t=window_h - posy - _text_sep - _text_height - _box_height; t<window_h - posy - _text_sep; t++)
			{
				curColor = VecMix(start_col, end_col,(t - (window_h - posy - _text_sep - _text_height - _box_height))/(_text_height + _box_height));
				curColor = VecClampOne(curColor);
				if (active)
					glColor4f(curColor.X, curColor.Y, curColor.Z, active_col.W);
				else
					glColor4f(curColor.X, curColor.Y, curColor.Z, inactive_col.W);
				glVertex3f(posx,             t, 4);
				glVertex3f(posx+ _box_width, t, 4);
			}
		glEnd();
		glColor3f(0,0,0);
		glLineWidth(2);
		glBegin(GL_LINE_LOOP);
			glVertex3f(posx,             window_h - posy - _text_sep,                  4);
			glVertex3f(posx + _box_width,window_h - posy - _text_sep,                  4);
			glVertex3f(posx + _box_width,window_h - posy - _text_sep - _text_height - _box_height, 4);
			glVertex3f(posx,             window_h - posy - _text_sep - _text_height - _box_height, 4);
		glEnd();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

}
bool ColorMap::Clicked(int button, int state, int x, int y)
{
	if ((state==GLUT_DOWN) && (x<posx + _box_width + 1) && (x>posx - 1) && (y<posy+_text_height) && (y>posy))
		return true;
	return false;
}

void ColorMap::Hover(int x, int y)
{
//	if ((x<posx + _box_width) && (x>posx) && (y<posy+_text_height) && (y>posy))
//	{	active=true; glutSetCursor(GLUT_CURSOR_INHERIT);}
//	else
//		active=false;
}

void ColorMap::SetBoxWidth(int _b_w)
{		_box_width=_b_w;}

void ColorMap::SetBoxHeight(int _b_h)
{		_box_height=_b_h;	}
