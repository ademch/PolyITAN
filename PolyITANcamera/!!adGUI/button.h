#ifndef BUTTON_H
#define BUTTON_H

#include "glfont.h"
#include "gui_element.h"
#include "../!!adGlobals/vector_math.h"
#include "SubWindow.h"
#include <string>


class Button : public GUI_Element
{
public:
   float m_TextWidth;
   float m_Height;
   int m_Width;

   bool bFocused;
   bool bEnabled;
   std::string _text;
   
   Vec4 vColor_focused;
   Vec4 vColor_defocused;

   Button(std::string caption, int px, int py, int width, float size);
   ~Button();

   OpenGLSubWindow* OnClickThis;
   bool(OpenGLSubWindow::*OnClick)();

   void Draw();
   virtual void Hover(int x, int y);
   virtual bool Clicked(int button, int state, int x, int y);

   void Reposition(int px, int py)
   {
	   posx = px;
	   posy = py;
   }

protected:
	float m_TextSize;

	int iGUIpushed;

};

#endif
