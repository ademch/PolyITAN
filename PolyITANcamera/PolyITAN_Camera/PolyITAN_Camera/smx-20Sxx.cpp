
#include "smx-20Sxx.h"
#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "smx_usb.h"
#include <linux/wait.h>

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

typedef struct _TBulkDescriptor
{
	struct libusb_transfer* transferDescriptor;
	unsigned char* buffer;
	int            bufferLen;
} BulkDescriptor;


#define IFCOUNT 3

typedef struct tagSMX
{
	libusb_device_handle* hUSBdev = NULL;
	
	// libusb thread
	pthread_t      	thread1;					// libusb requires some polling to execute asynchonous events, lazy bitch :)
	bool           	thread_exit;				// set to -1 to ask thread to exit
	
	TIdentData     	identdata;
	TFrameParams    FrameParams;

	SMX_StreamMode 	streamState;
	
	// Bulk transfer
	int 			iBulkCount = 0;				// the number of bulk requests swarm
	BulkDescriptor* arBulkDesc;					// array of bulk descriptors
	
	unsigned char*  arFrames[IFCOUNT];			// an array of pointers to frame data start pos
	WORD            arFramesNumbers[IFCOUNT];	// an array of frame numbers that corresponds to data in arFrames

	unsigned char*  ptrFramePos;				// position within current frame arFrames[frame_idx]
	
	int 			iFrameCurrent;				// index of a frame in arFrames/arFramesNumbers arrays
	int 			iFrameReady;				// index of a frame ready to be handed out, -1 signals no frame available

	// Sequence quality
	WORD            iPacketExpectedNumber;		// packet number is tracked to make sure the sequence is valid
	bool            bBadFrameSequence;			// set to signal the current
	
	int iFrameBytesAcquired;
	bool bFileSaved;
	bool  bFrameDone;
	
} SMX;


// packet header tagged to the end of each packet has the following structure
typedef struct tagSMXPACKETHEADER
{
	WORD wPacketNumber;			// 2bytes- packet number
	WORD wMagicSignature;		// 2bytes- packet magic signature
	WORD wReserved;				// 2bytes- of who knows static what
	WORD wHostingFrameNumber;	// 2bytes- of a frame number the packet is making
	
} SMXPACKETHEADER;


void* usb_thread_func(void* arg)
{
	bool* thread_exit = (bool*)arg;
	while ( !(*thread_exit) )
	{
		libusb_handle_events(NULL);	 // blocking call, returns when some event happens
	}
	
	return NULL;
}

HANDLE smx20s12X_OpenDevice(ULONG DeviceID, int iBulkCount)
{
int ret;
	
	struct libusb_device_handle* hUSBdev = libusb_open_device_with_vid_pid(NULL, 0x2969, 0xA520);
	if (!hUSBdev)
	{
		printf("  Error finding device vid=0x2969 pid=0xA520");
		return INVALID_HANDLE_VALUE;
	}
	printf("  Successfully connected to device %X:%X\n", 0x2969, 0xA520);
	
	// Allocate mem for camera general descriptor
	SMX* smx = (SMX*) malloc(sizeof(SMX));
	smx->hUSBdev = hUSBdev;
	
	smx->streamState = Camera_Stopped;
	
	// allow libusb to automatically detach the kernel driver on an interface
	// when claiming the interface and attach it back when releasing the interface
	ret = libusb_set_auto_detach_kernel_driver(hUSBdev, 1);
	if (ret != LIBUSB_SUCCESS)
	{
		printf("  Error on auto detach kernel driver: %s\n", libusb_error_name(ret));
		libusb_close(hUSBdev);

		return INVALID_HANDLE_VALUE;
	}
	printf("  Auto detaching kernel driver\n");
	
	ret = libusb_claim_interface(hUSBdev, 0);
	if (ret < 0)
	{
		printf("  Error claiming interface: %s\n", libusb_error_name(ret));
		libusb_close(hUSBdev);

		return INVALID_HANDLE_VALUE;
	}
	printf("  USB interface claimed successfully\n");
	
	// allocate frames
	for (int i=0; i<IFCOUNT; i++)
	{
		smx->arFrames[i] = (unsigned char*) malloc(4096*3000*1);
		memset(smx->arFrames[i], 0, 4096*3000*1);
	}
	printf("  Frame memory allocated successfully\n");
	
	// Experimental block----------------------
	sched_param param;
	pthread_attr_t thread_attributes;
	
		pthread_attr_init(&thread_attributes);
		pthread_attr_setschedpolicy(&thread_attributes, SCHED_FIFO);
		pthread_attr_setinheritsched(&thread_attributes, PTHREAD_EXPLICIT_SCHED);
		param.sched_priority = 20;
		pthread_attr_setschedparam(&thread_attributes, &param);

	//-----------------------------------------

	// start separate thread for asynchronous usblib events handling
	smx->thread_exit = false;
	int k = pthread_create( &(smx->thread1), &thread_attributes, usb_thread_func, &(smx->thread_exit));
	if (k != 0)
		printf("  Error %d", k);

	printf("  libusb thread started successfully\n");
	
	// Init sequence from vendor
	CxPerformVendorInit(smx);

	// STEP: allocate bulk tranfer descriptors
	
	smx->iBulkCount = iBulkCount;
	smx->arBulkDesc = (BulkDescriptor*) malloc( sizeof(BulkDescriptor)*iBulkCount * 2 );	// 2 endpoints
	
	// Endpoint 0x82 and 0x84
	for (int i=0; i<iBulkCount * 2; i++)
	{
		smx->arBulkDesc[i].transferDescriptor = libusb_alloc_transfer(0);
		smx->arBulkDesc[i].bufferLen = 1024*16;
		smx->arBulkDesc[i].buffer = libusb_dev_mem_alloc(smx->hUSBdev, 1024*16 ); //(unsigned char*) malloc( 1024*16 );
	}

	return smx;
}


BOOL smx20s12X_CloseDevice(HANDLE H)
{
	SMX* smx = (SMX*)H;
	if ((!smx) || (!smx->hUSBdev)) return false;

	// Cancel transfers to prevent their resubmission in callback
	for (int i=0; i<smx->iBulkCount * 2; i++)
	{
		libusb_cancel_transfer(smx->arBulkDesc[i].transferDescriptor);	// not a problem if transfer is not in progress, complete or cancelled
	}
	printf("  Cancelled %d pending usb transfers\n", smx->iBulkCount);
	
	// STEP: release usb device interface
	int ret = libusb_release_interface(smx->hUSBdev, 1);
	if (!ret)
		printf("  Device interface[%d] released successfully\n", 1);
	
	// STEP: Free bulk descriptors
	for (int i=0; i<smx->iBulkCount * 2; i++)
	{
		libusb_free_transfer(smx->arBulkDesc[i].transferDescriptor);
		
		//~ free(smx->arBulkDesc[i].buffer);
		//~ smx->arBulkDesc[i].bufferLen = 0;
		libusb_dev_mem_free(smx->hUSBdev, smx->arBulkDesc[i].buffer, smx->arBulkDesc[i].bufferLen);
	}
	free(smx->arBulkDesc);
	printf("  Transfer descriptors freed\n");
	
	// STEP: notify libusb thread to exit
	smx->thread_exit = true;		    // notify thread to exit
	libusb_close(smx->hUSBdev);	        // this wakes up libusb_handle_events and it returns control to the thre function
	
	pthread_join(smx->thread1, NULL);	// consider pthread_cancel...
	printf("  usblib handling thread joined\n");
	

	// STEP: deallocate frames
	for (int i=0; i<IFCOUNT; i++)
	{
		free(smx->arFrames[i]);
		smx->arFrames[i] = NULL;
	}
	printf("  %d frames deallocated\n", IFCOUNT);

	// STEP: deallocate camera context descriptor
	free(smx);
	smx = NULL;

	return true;
}




namespace PacketProcessor
{

	#define PACKET_SIGNATURE 0xA53C

	void CheckPacketHeader(struct libusb_transfer* transfDescriptor)
	{
		SMX*                   smx = (SMX*)transfDescriptor->user_data;
		SMXPACKETHEADER* smxPacket = (SMXPACKETHEADER*) &(transfDescriptor->buffer[transfDescriptor->actual_length - 8]);

		if ((transfDescriptor->actual_length < 8) ||
			(smxPacket->wMagicSignature != PACKET_SIGNATURE) ||
			(smxPacket->wPacketNumber != smx->iPacketExpectedNumber) )
		{
			// in all these cases wait for the next 0x0000 packet to resync
			smx->bBadFrameSequence = true;
			smx->iPacketExpectedNumber = 0x000;
			
			smx->ptrFramePos = smx->arFrames[smx->iFrameCurrent];	// reset head
			
			return;
		}
			
		// try to synchronize
		if (smxPacket->wPacketNumber == 0x0000) // && smx->bBadFrameSequence (commented to save time)
			smx->bBadFrameSequence = false;
	}
	

	void PrepareToNextPacket(struct libusb_transfer* transfDescriptor)
	{
		SMX*                   smx = (SMX*)transfDescriptor->user_data;
		SMXPACKETHEADER* smxPacket = (SMXPACKETHEADER*) &(transfDescriptor->buffer[transfDescriptor->actual_length - 8]);

		//                          \/ if any additional packets are received finish and just wait for the first
		if (smxPacket->wPacketNumber >= 0x02EE)	// 751st incl 0x0000
		{
			// The whole frame successfully received
			printf("  Successful frame: %d!\n", smxPacket->wHostingFrameNumber);
			
			// store the frame number (all packets of the same frame has the same frame number)
			smx->arFramesNumbers[smx->iFrameCurrent] = smxPacket->wHostingFrameNumber;

			// slide memory pointers
			smx->iFrameReady = smx->iFrameCurrent;
			
			smx->iFrameCurrent++;
			if (smx->iFrameCurrent >= IFCOUNT)
				smx->iFrameCurrent = 0;
				
			smx->ptrFramePos = smx->arFrames[smx->iFrameCurrent];	// update packet head pointer

			// memset(smx->ptrFramePos, 0, 4096*3000*1);	should we really do it? is it fast?

			smx->iPacketExpectedNumber = 0;
			
			return;
		}
		
		// sequence is alive, continue collecting packets into frame
		smx->iPacketExpectedNumber++;
	}
	
	
	static void LIBUSB_CALL bulk_callback(struct libusb_transfer* transfDescriptor)
	{
		if (transfDescriptor->status == LIBUSB_TRANSFER_CANCELLED)
		{
			printf("  Received cancelled packet. It will not be resubmitted\n" );
			return;
		}

		if (transfDescriptor->status == LIBUSB_TRANSFER_COMPLETED)
		{
			PacketProcessor::CheckPacketHeader(transfDescriptor);
			
			SMX* smx = (SMX*)transfDescriptor->user_data;
			if ( !smx->bBadFrameSequence )
			{
				if ( (smx->ptrFramePos + transfDescriptor->actual_length - 8) <= smx->arFrames[smx->iFrameCurrent] + 3000*4096*1 )
				{
					memcpy(smx->ptrFramePos, &(transfDescriptor->buffer[0]), transfDescriptor->actual_length - 8);
					smx->ptrFramePos += transfDescriptor->actual_length - 8;
				}

				PacketProcessor::PrepareToNextPacket(transfDescriptor);
			}
			// else
			// 		ignore incomming packets until resync on zero packet happens
			
		}
		else
		{
			//printf("  Received bulk packet with the error status: %s", libusb_error_name(transfDescriptor->status) );
			//printf(" size: %d\n", transfDescriptor->actual_length);
		}

		
		//~ else
		//~ {
			//~ if (transfDescriptor->actual_length != 1024*16)
				//~ printf(" size: %d\n", transfDescriptor->actual_length);


			//~ SMX* smx = (SMX*)transfDescriptor->user_data;

			//~ memcpy(smx->ptrFramePos, &(transfDescriptor->buffer[16376]), 16);
			//~ smx->ptrFramePos += 16;
			//~ smx->iFrameBytesAcquired += 16;

			//~ if (smx->iFrameBytesAcquired > 750*16*10 )
				//~ smx->bFrameDone = true;
		//~ }
		
		
		int res = libusb_submit_transfer(transfDescriptor);
		if (res != 0)
			printf("  libusb_submit_transfer error: %s\n", libusb_error_name(res));
	}
	
	
	void ResetPointers(HANDLE H)
	{
		SMX* smx = (SMX*)H;
	
		smx->iFrameCurrent 		   = 0;
		smx->iFrameReady   		   = -1;
		smx->iPacketExpectedNumber = 0x0000;
		smx->bBadFrameSequence     = false;
		smx->ptrFramePos   		   = smx->arFrames[smx->iFrameCurrent];
	}

}	// end namespace PacketProcessor


BOOL usbStartBulkStream(HANDLE H)
{
	SMX* smx = (SMX*)H;
	if ((!smx) || (!smx->hUSBdev)) return false;

	// Endpoint 0x82 and 0x84
	for (int i=0; i<smx->iBulkCount * 2; i++)
	{
		libusb_fill_bulk_transfer(  smx->arBulkDesc[i].transferDescriptor, // transfer descr
									smx->hUSBdev,   			  		   // device handle
									(i < smx->iBulkCount) ? 0x82 : 0x84,   // endpoint address
									smx->arBulkDesc[i].buffer,	  	  	   // databuffer
									smx->arBulkDesc[i].bufferLen, 	  	   // length of data buffer, however device defines how many bytes to send to host
									PacketProcessor::bulk_callback, 	   // callback
									smx, 				  				   // user data
									5   				  				   // timeout
								 );
								 
		int res = libusb_submit_transfer(smx->arBulkDesc[i].transferDescriptor);
		if (res != 0)
			printf("  libusb_submit_transfer error: %s\n", libusb_error_name(res));
	}


	return true;
}


BOOL usbStopBulkStream(HANDLE H)
{
	SMX* smx = (SMX*)H;
	if ((!smx) || (!smx->hUSBdev)) return false;

	// Cancel transfers to prevent their resubmission in callback
	
	// Enpoint 0x82 and 0x84
	for (int i=0; i<smx->iBulkCount * 2; i++)
	{
		libusb_cancel_transfer(smx->arBulkDesc[i].transferDescriptor);	// not a problem if transfer is not in progress, complete or cancelled
	}

	// clear possible stall
	libusb_clear_halt(smx->hUSBdev, 0x82);
	libusb_clear_halt(smx->hUSBdev, 0x84);
	
	return true;
}


BOOL smx20s12X_GetStreamMode(HANDLE H, SMX_StreamMode* StreamMode)
{
	SMX* smx = (SMX*)H;
	if (!smx) return false;
	
	(*StreamMode) = smx->streamState;
	
	return true;
}

BOOL smx20s12X_SetStreamMode(HANDLE H, SMX_StreamMode StreamModeTarget)
{ 
	SMX* smx = (SMX*)H;
	if ((!smx) || (!smx->hUSBdev)) return false;

	if (smx->streamState == StreamModeTarget) return false;	// state has been already achieved
	
	if (StreamModeTarget == Camera_Started)
	{
		usbStartBulkStream(H);
		
		PacketProcessor::ResetPointers(H);
		
		smx->iFrameBytesAcquired = 0;
		smx->bFileSaved = false;
		smx->bFrameDone = false;
		
		CxPerformVendorStreamStart(H);

		// returns -32, control command not supported, but works!
		smx20_usb_ControlWriteSpecial(smx->hUSBdev, DATA_FLOW, START_AHEAD, 0, NULL, 0);
		//if (ret != 0)
		//	return false;
		
		smx->streamState = Camera_Started;
			
		return true;
	}
	else if (StreamModeTarget == Camera_Stopped)
	{
		// returns -32, control command not supported, but works!
		smx20_usb_ControlWriteSpecial(smx->hUSBdev, DATA_FLOW, STOP_DEAD,   0, NULL, 0);
		//~ if (ret != 0)
			//~ return false;
		
		usbStopBulkStream(H);
		
		smx->streamState = Camera_Stopped;

		return true;
	}

	return false;	// unknown StreamModeTarget
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

BOOL smx20s12X_GetCameraInfo(HANDLE H, PCameraInfo CameraInfo) { return false; }
BOOL smx20s12X_GetCameraInfoEx(HANDLE H, PCameraInfoEx CameraInfoEx) { return false; }
BOOL smx20s12X_GetUsbSpeed(HANDLE H, PUsb_Speed UsbSpeed, PULONG RealSpeedMBpS) { return false; }
BOOL CxGetIdentData(HANDLE H, PIdentData pIdentData) { return false; }


// ERROR_NO_DATA_DETECTED	1104 (0x450)	- no video data received from camera
// ERROR_GEN_FAILURE		31 (0x1F)		- mostly incorrect stream mode
// ERROR_CRC				23 (0x17)		- missed frame in HDR mode (smx16 only)
// ERROR_SEM_TIMEOUT		121 (0x79)		- request timeout
//~ BOOL smx20s12X_GetFrame16(HANDLE H, PVOID Buffer, size_t length) { return false; }
//~ BOOL smx20s12X_GetFrameEx16(HANDLE H, PVOID Buffer, size_t length) { return false; }

BOOL smx20s12X_GetFrame8(HANDLE H, PVOID Buffer, size_t length)
{
	//~ Buffer = NULL;
	
	//~ return false;
	
	SMX* smx = (SMX*)H;
	if (!smx) return false;
	
	if (smx->bFileSaved) return true;

	while(!smx->bFrameDone)
		sleep(1);
	
	printf("Saving frame memory\n");
	FILE *fp = fopen("/home/polyitan/projects/PolyITANcamera/PolyITAN_Camera/PolyITAN_Camera/memory23.txt", "wbx");
	if (!fp) {
		return false;
	}
	
	fwrite(smx->arFrames[0], smx->iFrameBytesAcquired, 1, fp);
	
	fclose(fp);
	
	smx->bFileSaved=true;

	return true;
}


BOOL smx20s12X_GetFrameEx8(HANDLE H, PVOID *Buffer, size_t length, PWORD wFrameNumber)
{
	SMX* smx = (SMX*)H;
	if (!smx) return false;

	if (smx->iFrameReady < 0)
		return false;	// no frame is ready
	
	if (smx->arFramesNumbers[smx->iFrameReady] == (*wFrameNumber))
		return false;	// the frame caller has the same frame, nothing new to hand out
	
	(*wFrameNumber) = smx->arFramesNumbers[smx->iFrameReady];
	//memcpy(Buffer, smx->arFrames[smx->iFrameReady], 4096*3000*1);
	*Buffer = smx->arFrames[smx->iFrameReady];
	
	return true;
}

BOOL smx20s12X_GetFrequency(HANDLE H, PULONG FrequencyMHz) { return false; }

BOOL smx20s12X_GetFrameParams(HANDLE H, TFrameParams* FrameParams) { return false; }

BOOL smx20s12X_SetFrameParams(HANDLE H, TFrameParams* FrameParams) { return false; }

BOOL CxPerformVendorInit(HANDLE H)
{
	SMX* smx = (SMX*)H;
	if ((!smx) || (!smx->hUSBdev)) return false;
	
	// Comamnd 0
	unsigned char data0[256] = {0};
	smx20_usb_ControlRead(smx->hUSBdev,  I2C_FLASH_READ, 0x0700, &(data0[0]), 256);
	
	// Comamnd 1
	smx20_usb_ControlWrite(smx->hUSBdev, 128,            0,      NULL,        0);
	
	// Command 2
	unsigned char data2[8] = {0};
	smx20_usb_ControlRead(smx->hUSBdev,  READ_REGISTER,  WINDEX_GET_FRAMEPM,  &(data2[0]), 8);

	// Command 3
	unsigned char data3[3] = {0};
	smx20_usb_ControlRead(smx->hUSBdev,  READ_REGISTER,  WINDEX_FRAME_ROW,    &(data3[0]), 3);
	
	// Comamnd 4
	unsigned char data4[8] = {0x0,0x0,0x0,0x0,0x0,0x10,0xb8,0x0b};
	smx20_usb_ControlWrite(smx->hUSBdev, WRITE_REGISTER, WINDEX_GET_FRAMEPM,  &(data4[0]), 8);

	// Comamnd 5
	unsigned char data5[1] = {0x1};
	smx20_usb_ControlWrite(smx->hUSBdev, WRITE_REGISTER, 0x300a, &(data5[0]), 1);

	// Comamnd 6
	unsigned char data6[1] = {0x1};
	smx20_usb_ControlWrite(smx->hUSBdev, WRITE_REGISTER, 0x3000, &(data6[0]), 1);

	// Comamnd 7
	unsigned char data7[1] = {0};
	smx20_usb_ControlRead(smx->hUSBdev,  186,            0x0005, &(data7[0]), 1);
	
	// Comamnd 8
	unsigned char data8[1] = {0};
	smx20_usb_ControlWrite(smx->hUSBdev, 187,            0x0005, &(data8[0]), 1);

	// Comamnd 9
	unsigned char data9[1] = {1};
	smx20_usb_ControlWrite(smx->hUSBdev, WRITE_REGISTER, 0x3000, &(data9[0]), 1);

	// Comamnd 10
	unsigned char data10[1] = {1};
	smx20_usb_ControlWrite(smx->hUSBdev, WRITE_REGISTER, 0x300a, &(data10[0]),1);

	// Comamnd 11
	unsigned char data11[256] = {0};
	smx20_usb_ControlRead(smx->hUSBdev, I2C_FLASH_READ,  0x0701, &(data11[0]),256);

	// Comamnd 12
	unsigned char data12[21] = {0};
	smx20_usb_ControlRead(smx->hUSBdev, 176,             0x0000, &(data12[0]),21);

	// Comamnd 13
	unsigned char data13[3] = {0};
	smx20_usb_ControlRead(smx->hUSBdev, 186,             0x0002, &(data13[0]),1);

	return true;
}

BOOL CxPerformVendorStreamStart(HANDLE H)
{
	SMX* smx = (SMX*)H;
	if ((!smx) || (!smx->hUSBdev)) return false;
	
	// Comamnd 0
	unsigned char data0[1] = {1};
	smx20_usb_ControlWrite(smx->hUSBdev, WRITE_REGISTER, 0x300a, &(data0[0]),  1);
	
	// Comamnd 1
	unsigned char data1[1] = {1};
	smx20_usb_ControlWrite(smx->hUSBdev, WRITE_REGISTER, 0x3000, &(data1[0]),  1);
	
	// Command 2
	unsigned char data2[1] = {0};
	smx20_usb_ControlWrite(smx->hUSBdev, WRITE_REGISTER, 0x30ae, &(data2[0]),  1);

	// Command 3
	unsigned char data3[1] = {0};
	smx20_usb_ControlWrite(smx->hUSBdev, WRITE_REGISTER, 0x3000, &(data3[0]),  1);
	
	// Comamnd 4
	unsigned char data4[1] = {0};
	smx20_usb_ControlWrite(smx->hUSBdev, WRITE_REGISTER, 0x300a, &(data4[0]),  1);

	// Comamnd 5
	unsigned char data5[1] = {0};
	smx20_usb_ControlWrite(smx->hUSBdev, WRITE_REGISTER, 0x300b, &(data5[0]),  1);

	// Comamnd 6
	unsigned char data6[1] = {0};
	smx20_usb_ControlWrite(smx->hUSBdev, 187,            0x0008, &(data6[0]),  1);
 
	// Comamnd 7
	unsigned char data7[1] = {0};
	smx20_usb_ControlRead(smx->hUSBdev,  WRITE_REGISTER, 0x3000, &(data7[0]),  1);
	
	// Comamnd 8
	unsigned char data8[1] = {0};
	smx20_usb_ControlWrite(smx->hUSBdev, WRITE_REGISTER, 0x300a, &(data8[0]),  1);

	return true;
}

BOOL smx20s12X_GetGain(HANDLE H, double* AGain/*, double* DGain*/)
{
	SMX* smx = (SMX*)H;
	if ((!smx) || (!smx->hUSBdev)) return false;
	
	unsigned char data[2] = {0};

	int ret = smx20_usb_ControlRead(smx->hUSBdev, READ_REGISTER, WINDEX_GAIN, &data[0], 2);
	if (ret != 2)
		return false;
	
	uint16_t uiGain = 0;
	
	uiGain  = data[1] << 8;
	uiGain |= data[0];

	(*AGain) = uiGain;

	return true;
}

BOOL smx20s12X_SetGain(HANDLE H, double AGain/*, double DGain*/)
{
	SMX* smx = (SMX*)H;
	if ((!smx) || (!smx->hUSBdev)) return false;
	
	if (AGain > 480) {
		printf("  Gain tried to be sent is %f, overriding with 480\n", AGain);
		AGain = 480.0;
	}
	if (AGain < 0) {
		printf("  Gain tried to be sent is %f, overriding with 0\n", AGain);
		AGain = 0.0;
	}

	uint16_t uiGain = AGain;
	unsigned char data[2] = {0};

	data[0] = uiGain & 0xFF;
	data[1] = uiGain >> 8;

	int ret = smx20_usb_ControlWrite(smx->hUSBdev, WRITE_REGISTER, WINDEX_GAIN, &data[0], 2);
	if (ret != 2)
		return false;
	
	return true;
}

// Exposure time is bound by 1 row and frame-acquire row index
BOOL smx20s12X_GetExposureMinMax(HANDLE H, PULONG ExpMin, PULONG ExpMax)
{
	SMX* smx = (SMX*)H;
	if ((!smx) || (!smx->hUSBdev)) return false;
	
	unsigned char data[3] = {0};

	// Get frame row
	int ret = smx20_usb_ControlRead(smx->hUSBdev, READ_REGISTER, WINDEX_FRAME_ROW, &data[0], 3);
	if (ret != 3)
		return false;
	
	uint32_t uiFrameRow = 0;
	
	uiFrameRow  = data[2] << 16;
	uiFrameRow |= data[1] << 8;
	uiFrameRow |= data[0];
	
	(*ExpMin) = 1;
	(*ExpMax) = uiFrameRow;
	
	return true;
}

BOOL smx20s12X_GetExposureMinMaxMs(HANDLE H, double* ExpMin, double* ExpMax)
{
	SMX* smx = (SMX*)H;
	if ((!smx) || (!smx->hUSBdev)) return false;
	
	ULONG ulMin, ulMax;
	if (!smx20s12X_GetExposureMinMax(H, &ulMin, &ulMax))
		return false;

	(*ExpMin) = ulMin*RowMs;
	(*ExpMax) = ulMax*RowMs;

	return true;
}

// Returns Exposure in rows
BOOL smx20s12X_GetExposure(HANDLE H, PULONG Exposure)
{
	SMX* smx = (SMX*)H;
	if ((!smx) || (!smx->hUSBdev)) return false;
	
	unsigned char data[3] = {0};

	// STEP 1: Get frame row
	int ret = smx20_usb_ControlRead(smx->hUSBdev, READ_REGISTER, WINDEX_FRAME_ROW, &data[0], 3);
	if (ret != 3)
		return false;
	
	uint32_t uiFrameRow = 0;
	
	uiFrameRow  = data[2] << 16;
	uiFrameRow |= data[1] << 8;
	uiFrameRow |= data[0];
	
	// STEP 2: Get exposure component, real exposure is the difference between frame row and the component
	memset(&data[0], 0, 3);
	ret = smx20_usb_ControlRead(smx->hUSBdev, READ_REGISTER, WINDEX_EXPOS_COMP, &data[0], 3);
	if (ret != 3)
		return false;
	
	uint32_t uiExposureComp = 0;
	
	uiExposureComp  = data[2] << 16;
	uiExposureComp |= data[1] << 8;
	uiExposureComp |= data[0];
	
	// STEP 3: Calc exposure
	ULONG ulExposure = uiFrameRow - uiExposureComp;
	
	(*Exposure) = ulExposure;
	
	return true;
}

BOOL smx20s12X_SetExposure(HANDLE H, ULONG Exposure)
{
	SMX* smx = (SMX*)H;
	if ((!smx) || (!smx->hUSBdev)) return false;
	
	ULONG ExpMin, ExpMax;
	if (!smx20s12X_GetExposureMinMax( H, &ExpMin, &ExpMax))
		return false;
	
	if (Exposure > ExpMax)
		Exposure = ExpMax;
	if (Exposure < ExpMin)
		Exposure = ExpMin;
		
	// ExpMax is nothing more than WINDEX_FRAME_ROW
	// Calculate exposure_component from exposure
	
	ULONG ulExposComp = ExpMax - Exposure;
			
	unsigned char data[3] = {0};
	
	data[0] =  ulExposComp & 0xFF;
	data[1] = (ulExposComp >> 8 ) & 0xFF;
	data[2] = (ulExposComp >> 16) & 0xFF;

	int ret = smx20_usb_ControlWrite(smx->hUSBdev, WRITE_REGISTER, WINDEX_EXPOS_COMP, &data[0], 3);
	if (ret != 3)
		return false;
	
	return true;
}

BOOL smx20s12X_SetExposureMs(HANDLE H, double Exposure, double* ExposureSet)
{
	SMX* smx = (SMX*)H;
	if ((!smx) || (!smx->hUSBdev)) return false;
	
	ULONG uiExposure = Exposure/RowMs;	// ms/ms

	if (!smx20s12X_SetExposure(H, uiExposure))
		return false;

	(*ExposureSet) = uiExposure*RowMs;

	return true;
}

BOOL smx20s12X_GetExposureMs(HANDLE H, double* Exposure)
{
	SMX* smx = (SMX*)H;
	if ((!smx) || (!smx->hUSBdev)) return false;

	ULONG ExposureCurrent;
	if (!smx20s12X_GetExposure(H, &ExposureCurrent))
		return false;
	
	double fExposure = ExposureCurrent*RowMs;
	(*Exposure) = fExposure;
	
	return true;	
}

BOOL smx20s12X_GetFrameRateMinMax(HANDLE H, double* FrameRateMin, double* FrameRateMax)
{
	SMX* smx = (SMX*)H;
	if ((!smx) || (!smx->hUSBdev)) return false;
	
	// Does not work, no clue why
	//~ unsigned char data[2] = {0};

	//~ int ret = smx20_usb_ControlRead(smx->hUSBdev, READ_REGISTER, WINDEX_SENSOR_ROWS, &data[0], 2);
	//~ if (ret != 2)
		//~ return false;
	
	//~ uint16_t uiSensorRows = 0;
	//~ uiSensorRows  = data[1] << 8;
	//~ uiSensorRows |= data[0];
	
	uint16_t uiSensorRows = 3000;
	
	// Frame rate can not be faster than time required to acquire uiSensorRows and !required exposure time!
	ULONG Exposure;
	if (!smx20s12X_GetExposure(H, &Exposure))
		return false;
	
	if (uiSensorRows < Exposure)	// sensor rows time is smaller than the current exposure
	{
		printf("  Max frame rate is limited by the current exposure %ld\n", Exposure);
		uiSensorRows = Exposure;
	}
	
	(*FrameRateMin) = 1000.0/(1048575*RowMs);
	(*FrameRateMax) = 1000.0/(uiSensorRows*RowMs);
	
	return true;
}

BOOL smx20s12X_GetFrameRate( HANDLE H, double* FrameRate)
{ 
	SMX* smx = (SMX*)H;
	if ((!smx) || (!smx->hUSBdev)) return false;
	
	unsigned char data[3] = {0};

	int ret = smx20_usb_ControlRead(smx->hUSBdev, READ_REGISTER, WINDEX_FRAME_ROW, &data[0], 3);
	if (ret != 3)
		return false;
	
	uint32_t uiFrameRow = 0;
	
	uiFrameRow  = data[2] << 16;
	uiFrameRow |= data[1] << 8;
	uiFrameRow |= data[0];
	
	double fFrameRate = 1000.0/(uiFrameRow*RowMs);	// 1000 comes from s->ms
	(*FrameRate) = fFrameRate;
	
	return true;
}

BOOL smx20s12X_SetFrameRate( HANDLE H, double FrameRate, double* FrameRateSet)
{
	SMX* smx = (SMX*)H;
	if ((!smx) || (!smx->hUSBdev)) return false;
	
	double FrameRateMin;
	double FrameRateMax;
	
	if (!smx20s12X_GetFrameRateMinMax(H, &FrameRateMin, &FrameRateMax))
		return false;
		
	ULONG uiRowsMax = (1000.0/FrameRateMin)/RowMs;	// mind that min frame rate is achieved waiting "max-number-row"
	ULONG uiRowsMin = (1000.0/FrameRateMax)/RowMs;	// mind that max frame rate is achieved waiting "min-number-row"

	uint32_t uiRowsAsked = (1000.0/FrameRate)/RowMs;
	if (uiRowsAsked > uiRowsMax)
		uiRowsAsked = uiRowsMax;
	if (uiRowsAsked < uiRowsMin)
		uiRowsAsked = uiRowsMin;
		
	unsigned char data[3] = {0};
	
	data[0] =  uiRowsAsked & 0xFF;
	data[1] = (uiRowsAsked >> 8 ) & 0xFF;
	data[2] = (uiRowsAsked >> 16) & 0xFF;

	int ret = smx20_usb_ControlWrite(smx->hUSBdev, WRITE_REGISTER, WINDEX_FRAME_ROW, &data[0], 3);
	if (ret != 3)
		return false;
		
	(*FrameRateSet) = 1000.0/(uiRowsAsked*RowMs);
	 
	return true;
}



