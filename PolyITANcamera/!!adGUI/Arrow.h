#ifndef ARROW_H
#define ARROW_H

#include "glfont.h"
#include "gui_element.h"
#include "../!!adGlobals/vector_math.h"
#include <string>


class Arrow : public GUI_Element
{
public:
	std::string _text;

	Vec4 vColor;

	Arrow(std::string caption, int px, int py, int length, float size);
	~Arrow() {}

	void Draw();

protected:
	float _size;
	int _length;

	float m_TextWidth;
	float m_Height;

};


#endif
