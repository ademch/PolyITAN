#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include "button.h"
#include "../!!adGlobals/vector_math.h"

class ButtonGroup
{
public:
	Button* Add(std::string caption, float size, float r, float g,float b, int px, int py);
	bool ClickedG(Button* B,int button, int state, int x, int y);
	bool ClickedGAll(int button, int state, int x, int y, bool bFalseIfAlreadyActive=false);
	void DrawAll();
	Button* GetActive();	
	void SetActive(Button*);
	void SetActive(std::string str);
	bool IsActive(Button*);
	bool IsActive(std::string str);
	Button* GetButton(std::string str);
	Button* HoverAll(int x, int y);
	int ActiveIndex();

	int Count;
	Button* Active;

   ButtonGroup();
   ~ButtonGroup();

private:
	Button* ArButton[1000];

	void BoundingBox2D(Vec2& vMin, Vec2& vMax);
};

#endif
