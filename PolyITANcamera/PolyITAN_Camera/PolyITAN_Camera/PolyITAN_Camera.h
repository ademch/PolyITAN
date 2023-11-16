#include <GL/freeglut.h>
#include "../../!!adGUI/glfont.h"
#include "../../!!adGUI/SubWindow.h"
#include <vector>
#include "GLSL_Pipeline.h"
#include <cstddef>


GLFONT font;

int iAppWndWidth = 1300;
int iAppWndHeight = 480;

bool bExitDealocPerformed = false;


void ReshapeFunc(GLsizei w, GLsizei h);
void globaldraw();
void keyboard(unsigned char, int, int);
void keyboardSpecial(int key, int x, int y);
void MouseFunc(int, int, int, int);
void MouseWheelFunc(int, int, int, int);
void MotionFunc(int x, int y);
void PassiveMotionFunc(int x, int y);
void WMClose();


std::vector<OpenGLSubWindow*> liWindows;

GLSL_Pipeline glsl_pipeline;
