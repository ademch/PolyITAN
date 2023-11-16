#include "Camera.h"
#include <GL/gl.h>
#include <assert.h>
#include "../../!!adGlobals/bmpHandler.h"
#include "../../!!adGlobals/wdir.h"
#include <string.h>
#include <stdio.h>

Camera::Camera()
{
	data = NULL;
	m_texture   = 0;
	
	m_connected = false;
	m_streaming = false;
	smx 		= INVALID_HANDLE_VALUE;

	bAutoExpo   = false;

	fGainA      = 0.0;
	fExposureMs = 0.0;
	fFrameRate  = 0.0;
	fExpMinMs   = 0.0;
	fExpMaxMs   = 0.0;
	ulExposure  = 0;
	ulExpMin    = 0;
	ulExpMax    = 0;
	vGainRGB    = Vecc3();
	
	wFrameNumberLoaded = 0;

	bSaveFramePending = false;
}

float Camera::db2ms(float val)
{
	return pow(10.0f, val / 20.0f);	// 30->(0.2ms)
}

float Camera::ms2db(float val)
{
	return 20.0f*log10f(val);	// 30->(0.2ms)
}


bool Camera::Connect()
{
	printf("Camera OpenDevice...\n");

	smx = smx20s12X_OpenDevice(0, 2);
	if (smx == INVALID_HANDLE_VALUE)
	{
		printf("fail\n");
		return false;
	}
	printf("success\n");

	m_connected = true;

	return true;
}

bool Camera::Disconnect()
{
	if (m_streaming)
	{
		printf("Camera SetStreamMode STOP...\n");

		smx20s12X_SetStreamMode(smx, Camera_Stopped);
		m_streaming = false;
	}
	printf("success\n");

	printf("Camera CloseDevice...\n");

	if (!smx20s12X_CloseDevice(smx))
	{
		printf("fail\n");
		return false;
	}
	printf("success\n");

	m_connected = false;
	smx = INVALID_HANDLE_VALUE;

	return true;
}

bool Camera::GetCameraInfo()
{
	if (!m_connected) return false;	// not connected

	printf("GetCameraInfo...\n");

	TCameraInfo CameraInfo;
	if (!smx20s12X_GetCameraInfo(smx, &CameraInfo))
	{
		printf("fail\n");
		return 0;
	}
	printf("success\n");
	printf("DeviceName:%s, SensorType:%i\n", CameraInfo.DeviceName, CameraInfo.SensorType);
	printf("MaxHeight:%i, MaxWidth:%i\n",  CameraInfo.MaxHeight, CameraInfo.MaxWidth);

	return true;
}

bool Camera::GetFrequency()
{
	if (!m_connected) return false;	// not connected

	printf("GetFrequency...\n");

	ULONG freq = 0;
	if (!smx20s12X_GetFrequency(smx, &freq)) {
		printf("fail\n");
		return 0;
	}
	printf("success\n");
	printf("freq:%liMHz\n", freq);

	return true;
}

bool Camera::GetFrameParams()
{
	if (!m_connected) return false;	// not connected

	printf("GetFrameParams...\n");

	TFrameParams FrameParams;
	if (!smx20s12X_GetFrameParams(smx, &FrameParams))
	{
		printf(" fail\n");
		return 0;
	}
	printf("success\n");
	printf("StartX:%lu, StartY:%lu\n",        FrameParams.StartX, FrameParams.StartY);
	printf("Width:%lu, Height:%lu\n",         FrameParams.Width, FrameParams.Height);
	printf("Decimation:%li, ColorDeep:%lu\n", FrameParams.Decimation, FrameParams.ColorDeep);
	printf("MirrorV:%s, MirrorH:%s\n",        FrameParams.MirrorV ? "true" : "false", FrameParams.MirrorH ? "true" : "false");

	return true;
}

bool Camera::GetFrameRate()
{
	if (!m_connected) return false;	// not connected

	printf("GetFrameRate...\n");

	double fRate;
	if (!smx20s12X_GetFrameRate(smx, &fRate)) {
		printf("fail\n");
		return 0;
	}
	printf("success\n");
	printf("FrameRate:%f\n", fRate);

	fFrameRate = fRate;

	return true;
}

bool Camera::GetFrameRateMinMax()
{
	if (!m_connected) return false;	// not connected

	printf("GetFrameRateMinMax...\n");

	double frameRateMin, frameRateMax;
	if (!smx20s12X_GetFrameRateMinMax(smx, &frameRateMin, &frameRateMax)) {
		printf("fail");
		return 0;
	}
	printf("success\n");
	printf("FrameRateMin:%f, FrameRateMax:%f\n", frameRateMin, frameRateMax);

	return true;
}

bool Camera::GetExposureRowsMinMax()
{
	if (!m_connected) return false;	// not connected

	printf("GetExposureMinMax...\n");

	if (!smx20s12X_GetExposureMinMax(smx, &ulExpMin, &ulExpMax)) {
		printf(" fail\n");
		return 0;
	}
	printf("success\n");
	printf("ExpMin:%lu(rows), ExpMax:%lu(rows)\n", ulExpMin, ulExpMax);

	fExpMinMs = ulExpMin*RowMs;
	fExpMaxMs = ulExpMax*RowMs;

	return true;
}

//bool Camera::GetExposureMinMaxMs()
//{
//	if (!m_connected) return false;	// not connected
//
//	printf("GetExposureMinMaxMs...\n");
//
//	if (!smx20s12X_GetExposureMinMaxMs(SMX, &fExpMinMs, &fExpMaxMs)) {
//		printf(" fail\n");
//		return 0;
//	}
//	printf("success\n");
//	printf("fExpMin:%f(ms), fExpMax:%f(ms)\n", fExpMinMs, fExpMaxMs);
//
//	return true;
//}

//bool Camera::GetExposureMs()
//{
//	if (!m_connected) return false;	// not connected
//
//	printf("GetExposureMs...\n");
//
//	if (!smx20s12X_GetExposureMs(SMX, &fExposureMs)) {
//		printf(" fail\n");
//		return 0;
//	}
//	printf("success\n");
//	printf("Exposure:%f(ms)\n", fExposureMs);
//
//	return true;
//}

bool Camera::GetExposureRows()
{
	if (!m_connected) return false;	// not connected

	printf("GetExposure...\n");

	if (!smx20s12X_GetExposure(smx, &ulExposure)) {
		printf("fail\n");
		return 0;
	}
	printf("success\n");
	printf("Exposure:%lu(rows)\n", ulExposure);

	fExposureMs = ulExposure*RowMs;

	return true;
}

bool Camera::GetGain()
{
	if (!m_connected) return false;	// not connected

	printf("GetGain...\n");

	if (!smx20s12X_GetGain(smx, &fGainA)) {
		printf("fail\n");
		return 0;
	}
	printf("success\n");
	printf("fGain:%.2f\n", fGainA);

	return true;
}

bool Camera::SetGain()
{
	if (!m_connected) return false;	// not connected

	printf("SetGain %.2f...\n", fGainA);

	if (!smx20s12X_SetGain(smx, fGainA)) {
		printf("fail\n");
		return 0;
	}
	printf("success\n");

	return true;
}

bool Camera::SetFrameRate(double fRate)
{
	if (!m_connected) return false;	// not connected

	printf("SetFrameRate %f...\n", fRate);

	double frameRateSet;
	if (!smx20s12X_SetFrameRate(smx, fRate, &frameRateSet)) {
		printf(" fail\n");
		return 0;
	}
	printf("success\n");
	printf("FrameRateSet:%f\n", frameRateSet);

	fFrameRate = frameRateSet;

	return true;
}


//bool Camera::SetExposureMs(double fExpoMs)
//{
//	if (!m_connected) return false;	// not connected
//
//	printf("SetExposure %f...\n", fExpoMs);
//
//	double ExposureSet;
//	if (!smx20s12X_SetExposureMs(smx, fExpoMs, &ExposureSet)) {
//		printf("fail\n");
//		return 0;
//	}
//	printf("success\n");
//	printf("Exposure:%f(ms)\n", ExposureSet);
//
//	fExposureMs = ExposureSet;
//
//	return true;
//}

bool Camera::SetExposureRows(ULONG _ulExpo)
{
	if (!m_connected) return false;	// not connected

	printf("SetExposure %lu...\n", _ulExpo);

	if (!smx20s12X_SetExposure(smx, _ulExpo)) {
		printf("fail\n");
		return 0;
	}
	printf("success\n");

	ulExposure  = _ulExpo;
	fExposureMs = _ulExpo*RowMs;

	return true;
}

bool Camera::StartStream()
{
	if (!m_connected) return false;	// not connected
	if ( m_streaming) return false;	// already streaming
	
	wFrameNumberLoaded = 0x7777;	// some random number to make the fresh frame to be returned by smx 

	printf("Camera SetStreamMode START...\n");

	if (!smx20s12X_SetStreamMode(smx, Camera_Started)) {
		printf("fail\n");
		return false;
	}
	printf("success\n");

	m_streaming = true;

	return true;
}

bool Camera::StopStream()
{
	if (!m_connected) return false; // not connected
	if (!m_streaming) return false;	// stream has not been started nothing to stop

	printf("Camera SetStreamMode STOP...\n");

		if (!smx20s12X_SetStreamMode(smx, Camera_Stopped))
		{
			printf("fail\n");
			return false;
		}
		
	printf("success\n");

	m_streaming = false;

	return true;
}

void Camera::AllocFrameMemory()
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// free previously allocated data if any
	free(data);

	// load and generate the texture
	// NPOT Texture!!! (supported since GL v2.0)
	data = (unsigned char *) malloc( widthCONST*heightCONST*nrChannelsCONST );
	//ZeroMemory(data, width*height*nrChannels);
	memset(data, 120, widthCONST*heightCONST*nrChannelsCONST);

	//           targ         mml  int frmt                       brdr inc frmt      inc data type     inc data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthCONST, heightCONST, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);

}


void Camera::_TextureUpdate_Test()
{
	assert(data != NULL);

	//ZeroMemory(data, width*height*nrChannels);
	memset(data, 80, widthCONST*heightCONST*nrChannelsCONST);

	for (unsigned int j = 500; j < heightCONST-500; j++)
	{
		unsigned int odd = j % 2;
		for (unsigned int i = odd; i < widthCONST; i+=2)
		{
			data[j*widthCONST + i] = 120;
		}
	}

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, widthCONST,heightCONST, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
}


void Camera::UpdateTextureFromCamera()
{
	if (!m_streaming) return;

	assert(m_connected);
	assert(smx!=INVALID_HANDLE_VALUE);

	// disabled beacuse of local_data
	//~ assert(data!=NULL);
	//~ memset(data, 0, widthCONST*heightCONST*nrChannelsCONST);
	
	void* local_data = NULL;

	BOOL res = smx20s12X_GetFrameEx8(smx, &local_data, widthCONST*heightCONST * nrChannelsCONST, &wFrameNumberLoaded);
	if (!res)
		return;	// either no frame is ready or we already have the most recent one

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, widthCONST, heightCONST, GL_LUMINANCE, GL_UNSIGNED_BYTE, local_data);//data);
	//glFinish();

	if (bSaveFramePending)
	{
		saveGreyscaleBMP(FullPathToFile("Frames\\0.bmp"), data/*not local ?*/, widthCONST, heightCONST, 8);
		bSaveFramePending = false;
	}
}

