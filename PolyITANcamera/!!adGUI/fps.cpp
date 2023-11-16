#include "fps.h"
#include <GL/gl.h>
#include "glfont.h"
#include <stdio.h>
#include <time.h>

extern GLFONT font;


long difftimespec_ms(const struct timespec after, const struct timespec before)
{
    // need to sum into a single number in ms to perform subtract
    return (after.tv_sec*1000  + after.tv_nsec/1000000) -
           (before.tv_sec*1000 + before.tv_nsec/1000000);
}


FPS::FPS(int px, int py, float size): _size(size)
{
	posx = px;
	posy = py;
	vColor_focused = Vecc4(0.1f, 0.8f, 0.1f, 0.7f);

	clock_gettime(CLOCK_REALTIME, &T0);

	fFPS_avrg = 1;
	frames = 1;
}

FPS::~FPS()
{
}

void FPS::Draw()
{
	GUI_Element::Draw();

	struct timespec T1;
	clock_gettime(CLOCK_REALTIME, &T1);
	float elapsed_sec = float(difftimespec_ms(T1, T0)) / 1000.0f;


	frames++;
	if (elapsed_sec > 1.0)
	{
		fFPS_avrg = (fFPS_avrg + float(frames) / elapsed_sec) / 2;
		frames = 0;

		T0 = T1;
	}

	char strBuffer[80];
	sprintf(strBuffer, "Frame rate: %5.1f", fFPS_avrg);

	glColor4fv(&vColor_focused.X);
	glFontBegin(&font);
		glFontTextOut(strBuffer, float(posx), float(posy), 0, 6);
	glFontEnd();
	glDisable(GL_TEXTURE_2D);

}
