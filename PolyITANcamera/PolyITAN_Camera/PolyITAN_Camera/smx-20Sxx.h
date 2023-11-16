
#include <stddef.h>

#define RowMs 0.021737f


typedef void* HANDLE;
const HANDLE INVALID_HANDLE_VALUE = (HANDLE)-1;

typedef unsigned char  BOOLEAN;
typedef int            INT;
typedef char           CHAR;
typedef unsigned short WORD,  *PWORD;
typedef unsigned long  DWORD;
typedef unsigned char  BYTE;
typedef unsigned long  ULONG, *PULONG;
typedef long           LONG;
typedef int            BOOL,  *PBOOL;
typedef unsigned char  UCHAR, *PUCHAR;
typedef unsigned short USHORT,*PUSHORT;
typedef void          *PVOID;

//#include <pshpack1.h>
#pragma pack(push)
#pragma pack(1)

typedef struct _TCameraInfo
{
    INT SensorType;         ///< SensorType: 1 (color) or 0(mono)
    INT MaxWidth;           ///< Maximum Width of frame.
    INT MaxHeight;          ///< Maximum Height of frame.
    CHAR DeviceName[64];    ///< Camera name
} TCameraInfo, *PCameraInfo;


typedef struct _TCameraInfoEx {
    WORD  HWModelID;     ///< hardware model ID
    WORD  HWVersion;     ///< hardware version
    DWORD HWSerial;     ///< serial number of camera
    WORD  FWCVersion;    ///< firmware version #1
    WORD  FWAVersion;    ///< firmware version #2
    BYTE  Reserved;      ///< reserved
} TCameraInfoEx, *PCameraInfoEx;


typedef enum {
	Usb_UnsupportedSpeed,
	Usb_FullSpeed,
	Usb_HighSpeed,
	Usb_SuperSpeed
} Usb_Speed, *PUsb_Speed;


typedef enum _SMX_StreamMode {
	Camera_Stopped=0,
	Camera_Started=1
} SMX_StreamMode, *PSMX_StreamMode;


typedef struct _TFrameParams {
	ULONG		StartX,StartY;	// start position
	ULONG		Width,Height;	// of frame
	LONG		Decimation;		// 1,2 (,4 for color camera)
	ULONG		ColorDeep;		// 8 or 24
	BOOLEAN		MirrorV;		// vertical
	BOOLEAN		MirrorH;		// horizontal
} TFrameParams, *PFrameParams;


typedef struct _TStreamStats {
	ULONG	CameraFrames;
	ULONG	KernelFrames;
	ULONG	UserFrames;
	ULONG	TotalResyncs;
	ULONG	FrameResyncs;
} TStreamStats, *PStreamStats;


typedef struct _TFrameInfo {
	TFrameParams	FrameParams;
	USHORT			Packets;
	TStreamStats	StreamStats;
	UCHAR			Masks[4000];
} TFrameInfo, *PFrameInfo;


typedef struct _TIdentData {
	WORD Serial;
	BYTE ModelID;
	BYTE HWVersion;
	BYTE FWCVersion;
	BYTE FWAVersion;
	BYTE MfDay;
	BYTE MfMonth;
	WORD MfYear;
	CHAR MfFlasher[32];
	BYTE BPDay;
	BYTE BPMonth;
	WORD BPYear;
	CHAR BPFlasher[32];
} TIdentData, *PIdentData;

typedef struct _TFrameShortParams {
	ULONG		StartX,StartY;	// start position
	ULONG		Width,Height;	// of frame
} TFrameShortParams, *PFrameShortParams;


/// get pointer to FRAMEINFOEX for buffer from IOCTL_GetFrameEx
#define SMX_GET_FRAMEINFOEX(FrameInfoPtr)   ( (PFrameInfo)(FrameInfoPtr) )

/// get frame buffer for buffer from IOCTL_GetFrameEx
#define SMX_GET_FRAME_BUFFER(FrameInfoPtr)  ( (BYTE*)(FrameInfoPtr) + FRAMEINFO_SIZEF )

//#include <poppack.h>
#pragma pack(pop)

BOOL CxPerformVendorInit(HANDLE H);
BOOL CxPerformVendorStreamStart(HANDLE H);


HANDLE smx20s12X_OpenDevice(ULONG DeviceID, int iBulkCount);
HANDLE smx20s12X_OpenDeviceEx(ULONG DeviceID, BOOL Synchronous);
BOOL smx20s12X_CloseDevice(HANDLE H);

BOOL smx20s12X_GetCameraInfo(HANDLE H, PCameraInfo CameraInfo);
BOOL smx20s12X_GetCameraInfoEx(HANDLE H, PCameraInfoEx CameraInfoEx);
BOOL smx20s12X_GetFrequency(HANDLE H, PULONG FrequencyMHz);

BOOL CxGetIdentData(HANDLE H, PIdentData pIdentData);

BOOL smx20s12X_GetStreamMode(HANDLE H, SMX_StreamMode* StreamMode);
BOOL smx20s12X_SetStreamMode(HANDLE H, SMX_StreamMode StreamMode);

// ERROR_NO_DATA_DETECTED	1104 (0x450)	- no video data received from camera
// ERROR_GEN_FAILURE		31 (0x1F)		- mostly incorrect stream mode
// ERROR_CRC				23 (0x17)		- missed frame in HDR mode (smx16 only)
// ERROR_SEM_TIMEOUT		121 (0x79)		- request timeout
//~ BOOL smx20s12X_GetFrame16(HANDLE H, PVOID Buffer, size_t length);
//~ BOOL smx20s12X_GetFrameEx16(HANDLE H, PVOID Buffer, size_t length);

BOOL smx20s12X_GetFrame8(HANDLE H, PVOID Buffer, size_t length);
BOOL smx20s12X_GetFrameEx8(HANDLE H, PVOID *Buffer, size_t length, PWORD wFrameNumber);

BOOL smx20s12X_GetFrameParams(HANDLE H, TFrameParams* FrameParams);
BOOL smx20s12X_SetFrameParams(HANDLE H, TFrameParams* FrameParams);

BOOL smx20s12X_GetGain(HANDLE H, double* AGain/*, double* DGain*/);
BOOL smx20s12X_SetGain(HANDLE H, double AGain/*, double DGain*/);

BOOL smx20s12X_GetExposureMinMax(HANDLE H, PULONG ExpMin, PULONG ExpMax);
BOOL smx20s12X_GetExposure(HANDLE H, PULONG Exposure);
BOOL smx20s12X_SetExposure(HANDLE H, ULONG Exposure);
BOOL smx20s12X_GetExposureMinMaxMs(HANDLE H, double* ExpMin, double* ExpMax);
BOOL smx20s12X_GetExposureMs(HANDLE H, double* Exposure);
BOOL smx20s12X_SetExposureMs(HANDLE H, double Exposure, double* ExposureSet=NULL);

BOOL smx20s12X_GetFrameRateMinMax(HANDLE H, double* FrameRateMin, double* FrameRateMax);
BOOL smx20s12X_GetFrameRate( HANDLE H, double* FrameRate);
BOOL smx20s12X_SetFrameRate( HANDLE H, double FrameRate, double* FrameRateSet=NULL);
BOOL smx20s12X_SetMinFrameRate(HANDLE H);
BOOL smx20s12X_SetMaxFrameRate(HANDLE H);




