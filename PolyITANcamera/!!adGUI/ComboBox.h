#ifndef COMBO_BOX_H
#define COMBO_BOX_H

#include "glfont.h"
#include "gui_element.h"
#include "../!!adGlobals/vector_math.h"
#include "SubWindow.h"
#include <vector>
#include <string>



class ComboBox: public GUI_Element
{
public:
   float m_Height;
   int   m_Width;
   std::string _text;

   bool bFocused;
   bool bEnabled;
   bool bVisible;

   unsigned int iSelected;
   
   Vec4 vColor_focused;
   Vec4 vColor_defocused;

   ComboBox(std::string caption, int px, int py, int width, float size);
   ~ComboBox();

   OpenGLSubWindow*OnClickThis;
   bool(OpenGLSubWindow::*OnClick)();
   bool(OpenGLSubWindow::*OnSelect)(int);

   void Draw();
   virtual void Hover(int x, int y);
   virtual bool Clicked(int button, int state, int x, int y);

   void Reposition(int px, int py)
   {
	   posx = px;
	   posy = py;
   }

   bool SetSelected(std::string);
   std::string GetSelected();

   std::vector<std::string> items;

protected:
	float m_TextSize;

	bool bExpanded;
	int iGUIpushed;

	float m_TextWidth;

private:
	int iListPixelShift;
	unsigned int iHovered;
};

#endif
