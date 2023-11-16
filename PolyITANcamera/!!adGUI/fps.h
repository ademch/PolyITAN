#ifndef FPS_H
#define FPS_H

#include "gui_element.h"
#include "../!!adGlobals/vector_math.h"

class FPS : public GUI_Element
{
public:
	Vec4 vColor_focused;

	FPS(int px, int py, float size);
	~FPS();

	void Draw();

protected:
	float _size;

	float fFPS_avrg;
	int frames;

	struct timespec T0;
};

#endif
