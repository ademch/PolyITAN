#ifndef LABEL_H
#define LABEL_H

#include "glfont.h"
#include "gui_element.h"
#include "../!!adGlobals/vector_math.h"
#include <string>

class Label: public GUI_Element
{
public:
   std::string _text;
   
   Vec4 vColor;

   Label(std::string caption, int px, int py, float size);
   ~Label();

   void UpdateCaption(std::string caption);

   void Draw();

protected:
   float _size;

};


#endif
