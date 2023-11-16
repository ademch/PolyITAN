#include "../../!!adGlobals/vector_math.h"
#include "smx-20Sxx.h"


class Camera
{
public:

	double		  fGainA;
	double 		  fExposureMs;
	double		  fFrameRate;
	double 		  fExpMinMs, fExpMaxMs;
	unsigned long ulExpMin, ulExpMax;
	unsigned long ulExposure;
	Vec3 		  vGainRGB;
	bool 		  bAutoExpo;

	Camera();
	~Camera()
	{
		free(data);
		data = NULL;
	}

	static float db2ms(float val);
	static float ms2db(float val);

	bool Connect();
	bool Disconnect();
	
	bool StartStream();
	bool StopStream();

	bool GetCameraInfo();
	bool GetFrequency();
	bool GetFrameParams();
	
	bool SetFrameRate(double fRate);
	bool GetFrameRate();
	bool GetFrameRateMinMax();

	//bool SetExposureMs(double fExpoMs);
	//bool GetExposureMinMaxMs();
	//bool GetExposureMs();
	bool SetExposureRows(unsigned long ulExpo);
	bool GetExposureRows();
	bool GetExposureRowsMinMax();

	bool GetGain();
	bool SetGain();
	
	void Render();

	void AllocFrameMemory();
	void UpdateTextureFromCamera();
	void _TextureUpdate_Test();

	bool m_connected;
	bool m_streaming;
	unsigned int m_texture;

	bool bSaveFramePending;

protected:
	HANDLE smx;
	
	const unsigned int widthCONST       = 4096;
	const unsigned int heightCONST      = 3000;
	const unsigned int nrChannelsCONST  = 1;


private:
	unsigned char*     data;
	WORD               wFrameNumberLoaded;

};
