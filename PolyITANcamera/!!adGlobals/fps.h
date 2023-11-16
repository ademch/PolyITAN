#ifndef FPS_H
#define FPS_H

class FPS
{
	public:
		FPS();
		void MainLoopBegin();
		void glFinishBegin();
		void glFinishEnd();
		void RenderFrameRate();
		float fps;

	private:
		float main_loop_time;
		float cpu_wait_time;
		float handlers_time;
		LARGE_INTEGER  main_loop_begin_tick;
		LARGE_INTEGER  main_loop_begin_tick_old;
		LARGE_INTEGER  gl_finish_begin_tick;
		LARGE_INTEGER  gl_finish_end_tick;

		LARGE_INTEGER T0;
		LARGE_INTEGER ticksPerSecond;

		int frames;
		char strBuffer[80];
};

#endif
