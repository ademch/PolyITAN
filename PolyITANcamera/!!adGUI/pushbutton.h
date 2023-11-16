#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include "button.h"
#include <string>

class PushButton : public Button
{
public:

	PushButton(std::string caption, float size, float r, float g,float b, int px, int py, bool active=false);
	PushButton();
	~PushButton();

	void Hover(int x, int y);
	bool ClickedD(int button, int state, int x, int y);
	bool IsActive();
};

#endif
