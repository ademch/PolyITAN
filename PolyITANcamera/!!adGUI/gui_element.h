#pragma once

class GUI_Element
{
public:
	GUI_Element() {posx = 0; posy = 0;}
	virtual ~GUI_Element() {}

	virtual void Draw() {}

	virtual bool Clicked(int button, int state, int x, int y) { return false; }
	virtual bool Drag(int x, int y) { return false; }
	virtual void Hover(int x, int y) { }
	virtual void Wheel(int state,int delta,int x,int y) { }

	void Reposition(int px, int py)
    {
	    posx = px;
	    posy = py;
    }

protected:

	int posx;
	int posy;

};
