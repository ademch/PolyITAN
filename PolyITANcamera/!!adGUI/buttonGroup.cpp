
#include "buttonGroup.h"
#include <GL/gl.h>
#include <assert.h>


ButtonGroup::ButtonGroup()
{
int i;

	for (i=0;i<100;i++)
		ArButton[i]=0;

	Count=0;
	Active=NULL;
}


ButtonGroup::~ButtonGroup()
{
int i;

	for (i=0;i<Count;i++)
	{
		assert(ArButton[i]!=NULL);
		delete ArButton[i];
	}
}

Button* ButtonGroup::Add(std::string caption, float size, float r, float g,float b, int px, int py)
{
	ArButton[Count++]= new Button(caption, size, r,g,b, px,py);

return ArButton[Count-1];
}

bool ButtonGroup::ClickedG(Button* B,int button, int state, int x, int y)
{

	if (B->ClickedD(button, state, x, y))	
	{
		if (B->flare) return false;
		if (Active!=NULL) Active->flare=false;
		Active=B;
		Active->flare=true;

		return true;
	}

return false;
}

bool ButtonGroup::ClickedGAll(int button, int state, int x, int y, bool bFalseIfAlreadyActive)
{
int i;

	for (i=0;i<Count;i++)
	{
		assert(ArButton[i]!=NULL);
		if (ArButton[i]->ClickedD(button, state, x, y))	
		{
			// if that is the button that was already active return true immediately
			if (ArButton[i]->flare)
			{	if (bFalseIfAlreadyActive)	
					return false;
				else
					return true;
			}

			// else set new active button
			if (Active!=NULL) Active->flare=false;
			Active=ArButton[i];
			Active->flare=true;
			return true;
		}
	}

return false;
}

Button* ButtonGroup::GetButton(std::string str)
{
int i;

	for (i=0;i<Count;i++)
	{
		assert(ArButton[i]!=NULL);
		if (!ArButton[i]->_text.compare(str))
			return ArButton[i];
	}

return NULL;
}

Button* ButtonGroup::HoverAll(int x, int y)
{
int i;

	for (i=0;i<Count;i++)
	{
		assert(ArButton[i]!=NULL);
		ArButton[i]->HoverWOFlare(x, y);
	}

return NULL;
}

void ButtonGroup::SetActive(Button* B)
{
int i;

	for (i=0;i<Count;i++)
	{
		assert(ArButton[i]!=NULL);
		if (ArButton[i]==B)
		{	ArButton[i]->flare=true;
			Active=B;
			break;
		}
	}
}

void ButtonGroup::SetActive(std::string str)
{
int i;
Button* B = GetButton(str);

	for (i=0;i<Count;i++)
	{
		assert(ArButton[i]!=NULL);
		if (ArButton[i]==B)
		{	ArButton[i]->flare=true;
			Active=B;
			
			break;
		}
	}
}

bool ButtonGroup::IsActive(Button* B)
{
	if (B == NULL) return false;
	if (B->flare)
		return true;

	return false;
}

bool ButtonGroup::IsActive(std::string str)
{
Button* B = GetButton(str);

	if (B == NULL) return false;
	if (B->flare)
		return true;

	return false;
}

int ButtonGroup::ActiveIndex()
{
int i;

	for (i=0;i<Count;i++)
	{
		assert(ArButton[i]!=NULL);
		if (ArButton[i]==Active)
			return i;
	}

	return -1;
}

void ButtonGroup::DrawAll()
{
int i;
Vec2 vMin, vMax;
	
	for (i=0;i<Count;i++)
	{
		assert(ArButton[i]!=NULL);
		ArButton[i]->Draw();
	}

	BoundingBox2D(vMin, vMax);

	glLineWidth(1);
	glDisable(GL_TEXTURE_2D);

	glColor3f(0.3f,0.3f,0.3f);
	glBegin(GL_LINE_LOOP);
		glVertex3f(vMin.X-5, vMin.Y-5, -4);
		glVertex3f(vMax.X+5, vMin.Y-5, -4);
		glVertex3f(vMax.X+5, vMax.Y+5, -4);
		glVertex3f(vMin.X-5, vMax.Y+5, -4);
	glEnd();

}


void ButtonGroup::BoundingBox2D(Vec2& vMin, Vec2& vMax)
{
int i;

	vMin = Vecc2(10000,10000);
	vMax = Vecc2(0,0);

	for (i=0;i<Count;i++)
	{
		assert(ArButton[i]!=NULL);

		vMin.X = min(vMin.X, ArButton[i]->posx);
		vMin.Y = min(vMin.Y, ArButton[i]->posy);

		vMax.X = max(vMax.X, ArButton[i]->posx + ArButton[i]->width);
		vMax.Y = max(vMax.Y, ArButton[i]->posy + ArButton[i]->height);
	}
}


Button* ButtonGroup::GetActive()
{
	return Active;
}
