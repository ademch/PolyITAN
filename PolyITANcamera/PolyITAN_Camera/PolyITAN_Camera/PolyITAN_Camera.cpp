// .cpp : Defines the entry point for the console application.
//

#include "PolyITAN_Camera.h"
#include "../../!!adGlobals/wdir.h"
#include "CameraSubWindow.h"
#include "ParamsSubWindow.h"
#include "HistogramSubWindow.h"
#include "Bayer2rgbFBO.h"
#include "SobelFBO.h"
#include "rgb2yuvFBO.h"
#include "bilateralFBO.h"
#include "Camera.h"
#include "../../!!adExtensions/extensions.h"

#include <unistd.h>
#include <libusb-1.0/libusb.h>


Camera cam;
TextureBank texBank;

typedef char TCHAR;



int main(int argc, TCHAR* argv[], TCHAR* envp[])
{
	GLfloat light_position[] = { 100.0, 100.0, 100.0, 0.0 };

	glutInit(&argc, (char**)argv);
	glutInitWindowSize(iAppWndWidth, iAppWndHeight);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("PolyITAN Camera Renderer");

	glutReshapeFunc(ReshapeFunc);
	glutIdleFunc(globaldraw);
	glutDisplayFunc(globaldraw);

	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	glutMouseFunc(MouseFunc);
	glutMotionFunc(MotionFunc);
	glutPassiveMotionFunc(PassiveMotionFunc);
	glutWMCloseFunc(WMClose);
	glutMouseWheelFunc(MouseWheelFunc);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	setbuf(stdout, NULL);

	printf("Renderer: %s\n",     glGetString(GL_RENDERER));
	printf("Version GL: %s\n",   glGetString(GL_VERSION));
	printf("Version GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	if (!CheckExtensions()) return 1;
	linkExtensions();

	printf("Loading font...\n");
	unsigned int iTextureFont;
	glGenTextures(1, &iTextureFont);
	if (!glFontCreate(&font, FullPathToFile("arial.glf"), iTextureFont))
	{
		printf("fail\n");
		return 1;
	}
	printf("done\n");

	printf("Allocating memory for video stream...\n");
		cam.AllocFrameMemory();
		texBank.bank[TEXTURE_CAM_BAYER_OUT] = new TextureDescriptor(cam.m_texture, 4096, 3000);
	printf("done\n");
	
	
	printf("Initializing usb kernel->user usb bridge...\n");
		int ret = libusb_init(NULL);
		if (ret < 0) return 2;

		if (0) libusb_set_option(NULL, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_DEBUG);
	printf("done\n");


	OpenGLSubWindow* window;
	window = new Bayer2rgbFBO(0, 0, 2048, 1500);
	liWindows.push_back(window);

	window = new bilateralFBO(0, 0, 2048, 1500);
	liWindows.push_back(window);

	window = new SobelFBO(0, 0, 2048, 1500);
	liWindows.push_back(window);

	window = new rgb2yuvFBO(0, 0, 2048, 1500);
	liWindows.push_back(window);

	window = new CameraSubWindow(iAppWndWidth*0.01, iAppWndHeight*0.02, iAppWndWidth*0.46, iAppWndHeight*0.96);
	sprintf(window->m_strCaption, "%s", "Sensor");
	window->bSceneRotationAllowed = false;
	window->fZoomFactor = 0.5;
	liWindows.push_back(window);

	window = new ParamsSubWindow(iAppWndWidth*0.48, iAppWndHeight*0.02, iAppWndWidth*0.28, iAppWndHeight*0.96);
	sprintf(window->m_strCaption, "%s", "Params");
	window->bSceneRotationAllowed = false;
	window->bSceneDragAllowed = false;
	window->bSceneZoomAllowed = false;
	liWindows.push_back(window);

	window = new HistogramSubWindow(iAppWndWidth*0.77, iAppWndHeight*0.02, iAppWndWidth*0.22, iAppWndHeight*0.96);
	sprintf(window->m_strCaption, "%s", "Analytics");
	window->bSceneRotationAllowed = false;
	window->bSceneDragAllowed = false;
	window->bSceneZoomAllowed = false;
	liWindows.push_back(window);

	glutMainLoop();

	return 0;
}



void ReshapeFunc(GLsizei w, GLsizei h)
{
	iAppWndWidth = w;
	iAppWndHeight = h;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w/2.0f, w/2.0f, -h/2.0f, h/2.0f, -200, 200);

	//	gluPerspective (30.0,                   // Field-of-view angle
	//		            float(w)/float(h),      // Aspect ratio of viewing volume
	//		            .1,						// Distance to near clipping plane
	//		            200.0);					// Distance to far clipping plane

	glMatrixMode(GL_MODELVIEW);

	//liWindows[0]->Reshape(no calling for fbo);
	//liWindows[1]->Reshape(no calling for fbo);
	//liWindows[2]->Reshape(no calling for fbo);
	//liWindows[3]->Reshape(no calling for fbo);
	liWindows[4]->Reshape(iAppWndWidth*0.01, iAppWndHeight*0.02, iAppWndWidth*0.46, iAppWndHeight*0.96);
	liWindows[5]->Reshape(iAppWndWidth*0.48, iAppWndHeight*0.02, iAppWndWidth*0.28, iAppWndHeight*0.96);
	liWindows[6]->Reshape(iAppWndWidth*0.77, iAppWndHeight*0.02, iAppWndWidth*0.22, iAppWndHeight*0.96);
}

void globaldraw()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	if (!cam.m_streaming) cam._TextureUpdate_Test();
	cam.UpdateTextureFromCamera();

	for (auto iterWindow : liWindows)
		iterWindow->Render();

	if (!cam.m_streaming) 
		usleep(10000);
	else
		usleep(200000);	// pause the process more during streaming to make usb transmition more stable

	glFinish();

	glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y)
{
	bool res = false;
	
	y = iAppWndHeight - y;

	for (auto iterWindow : liWindows)
	{
		res = iterWindow->KeyboardFunc(key, x, y);
		if (res) break;
	}

	if (res) return;

	switch (key)
	{
	case 'o':
	{
		break;
	}
	case 27:
	case 'Q':
		break;
	}
}

// Handle subwindows resize
void keyboardSpecial(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
	{
		break;
	}
	case GLUT_KEY_DOWN:
	{
		break;
	}
	case GLUT_KEY_RIGHT:
	{

		break;
	}
	case GLUT_KEY_LEFT:
	{
		break;
	}
	break;
	}

}

// (in) x,y - window coords from (0,0) to (w,h)
void MotionFunc(int x, int y)
{
	y = iAppWndHeight - y;

	for (auto iterWindow : liWindows)
		iterWindow->MotionFunc(x, y);
}

void PassiveMotionFunc(int x, int y)
{
	y = iAppWndHeight - y;
	
	for (auto iterWindow : liWindows)
		iterWindow->PassiveMotionFunc(x, y);
}

void MouseFunc(int button, int state, int x, int y)
{
	y = iAppWndHeight - y;
	
	if ((button == GLUT_LEFT_BUTTON) || (button == GLUT_MIDDLE_BUTTON) || (button == GLUT_RIGHT_BUTTON))
	{
		for (auto iterWindow : liWindows)
			iterWindow->MouseFunc(button, state, x, y);
	}
	else if ((button == 3) && (state == GLUT_UP))	// magic number for scrolling upwards (up & down events received)
	{
		for (auto iterWindow : liWindows)
			iterWindow->MouseWheelFunc(0, +120, x, y);
		
	}
	else if ((button == 4) && (state == GLUT_UP))	// magic number for scroller downwards (up & down events received)
	{
		for (auto iterWindow : liWindows)
			iterWindow->MouseWheelFunc(0, -120, x, y);
		
	}

}

void MouseWheelFunc(int state, int delta, int x, int y)
{
	// translate from screen to window coordinates
	int posX = glutGet(GLUT_WINDOW_X);
	int posY = glutGet(GLUT_WINDOW_Y);

	x = x - posX;
	y = y - posY;
	
	y = iAppWndHeight - y;
	for (auto iterWindow : liWindows)
		iterWindow->MouseWheelFunc(state, delta, x, y);
}


// could be called several times
void WMClose()
{
	if (bExitDealocPerformed) return;
	bExitDealocPerformed = true;
	
	glDeleteTextures(1, &(cam.m_texture) );
	
	std::vector<OpenGLSubWindow*>::iterator iterWindow;
	for (iterWindow = liWindows.begin(); iterWindow != liWindows.end(); iterWindow++)
	{
		delete (*iterWindow);
		(*iterWindow) = NULL;
	}

	glFontDestroy(&font);
	
	libusb_exit(NULL);

	// Clean up and exit.
	printf("Exiting.\n");

//	exit(0);
}
