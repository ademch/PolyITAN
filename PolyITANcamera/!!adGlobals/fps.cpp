
#include "stdafx.h"
#include "fps.h"
#include "gl/glut.h"
#include <stdio.h>
#include "../!!adGUI/glfont.h"

extern GLFONT font;
extern int iAppWindowWidth;
extern int iAppWindowHeight;


void FPS::MainLoopBegin()
{	main_loop_begin_tick_old=main_loop_begin_tick;
	QueryPerformanceCounter(&main_loop_begin_tick); 

    frames++;
}

void FPS::glFinishBegin()
{	QueryPerformanceCounter(&gl_finish_begin_tick); 
}

void FPS::glFinishEnd()
{	QueryPerformanceCounter(&gl_finish_end_tick);
}

FPS::FPS()
{
	QueryPerformanceFrequency(&ticksPerSecond);
	
	QueryPerformanceCounter(&main_loop_begin_tick);
	T0 = main_loop_begin_tick;
	frames=64;
	fps=64;

	main_loop_time=1.;
	cpu_wait_time=1.;  
	handlers_time=1.;  
}


void FPS::RenderFrameRate()
{
float elapsed_sec = float(main_loop_begin_tick.QuadPart - T0.QuadPart)/float(ticksPerSecond.QuadPart);

	if (elapsed_sec >= 0.2)
	{
		fps = (fps + float(frames)/elapsed_sec)/2;

		T0 = main_loop_begin_tick;
		frames = 0;

		main_loop_time = (float(gl_finish_begin_tick.QuadPart - main_loop_begin_tick_old.QuadPart))/float(ticksPerSecond.QuadPart);
		cpu_wait_time  = (float(gl_finish_end_tick.QuadPart   - gl_finish_begin_tick.QuadPart))/float(ticksPerSecond.QuadPart);
		handlers_time  = (float(main_loop_begin_tick.QuadPart - gl_finish_end_tick.QuadPart))/float(ticksPerSecond.QuadPart);
	}

	glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, iAppWindowWidth, 0, iAppWindowHeight, 0, 1);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glFontBegin (&font);
	glLoadIdentity();
	glColor4f(1,.0,.0,0.85);
		        sprintf(strBuffer, "Frame rate: %6.2f", fps);
			glFontTextOut(strBuffer, 10.,iAppWindowHeight-50.,-0.001, 5);

				sprintf(strBuffer, "CPU+GPU Calc: %6.2fms", main_loop_time*1000);
			glFontTextOut(strBuffer, 10.,iAppWindowHeight-65.,-0.001, 5);

		        sprintf(strBuffer, "GPU Draw, CPU wait: %6.2fms", cpu_wait_time*1000);
			glFontTextOut(strBuffer, 10.,iAppWindowHeight-80.,-0.001, 5);

		        sprintf(strBuffer, "Event Handlers: %6.2fms", handlers_time*1000);
			glFontTextOut(strBuffer, 10.,iAppWindowHeight-95.,-0.001, 5);
 	glFontEnd ();
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

}