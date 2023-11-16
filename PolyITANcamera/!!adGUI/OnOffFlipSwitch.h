#ifndef ONOFFFLIPSWITCH_H
#define ONOFFFLIPSWITCH_H

#include "glfont.h"
#include "gui_element.h"
#include "../!!adGlobals/vector_math.h"
#include "../!!adGUI/SubWindow.h"
#include <string>

extern GLFONT font;

class OnOffFlipSwitch : public GUI_Element
{
public:
   float _text_width;
   float _text_height;

   bool bEnabled;
   bool bON;
   bool bPushButton;

   std::string _text;
   
   Vec4 vColor_focused;
   Vec4 vColor_defocused;

   OnOffFlipSwitch(std::string strCaption, int px, int py, float size);
   ~OnOffFlipSwitch();

   void Draw();
   virtual void Hover(int x, int y);
   virtual bool Clicked(int button, int state, int x, int y);

   OpenGLSubWindow* OnPreClickThis;
   bool(OpenGLSubWindow::*OnPreClick)(bool bON_Request);

   void SetOnOff(bool _bOn, bool bCallCallback);

protected:
	float _size;
	int m_iBox_width;
	int m_iBox_sep;

	bool bFocused;
	int iGUIpushed;
};

#endif
