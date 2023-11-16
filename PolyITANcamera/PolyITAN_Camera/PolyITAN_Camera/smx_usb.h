
#include <libusb-1.0/libusb.h>


// control requests (bRequest)
#define DATA_FLOW  		  	1
#define I2C_FLASH_READ    	195
#define WRITE_REGISTER    	201
#define READ_REGISTER     	202

// control commands (wIndex)
#define WINDEX_GAIN       	0x3204
#define WINDEX_FRAME_ROW  	0x3010	// WINDEX_EXPOS_COMP < WINDEX_FRAME_ROW < 1048575 -> should it be larger than max(WINDEX_EXPOS_COMP, 3000)?
                                    // Changing frame rate changes exposure in original implementation
                                    // making frame rate a primary value to set
#define WINDEX_EXPOS_COMP   0x308d	// true exposure row is calcluated as WINDEX_FRAME_ROW - WINDEX_EXPOS_ROW inside the camera!
#define WINDEX_SENSOR_ROWS  0x3316	// number of sensor rows
#define WINDEX_GET_FRAMEPM  0x3310	// frame params

// control wValue
#define START_AHEAD  		1		// stream start
#define STOP_DEAD  		    0		// stream stop



int smx20_usb_ControlRead( libusb_device_handle* hUSBdev, uint8_t byteRequest, uint16_t wIndex, unsigned char* data, uint16_t wLength );
int smx20_usb_ControlWrite( libusb_device_handle* hUSBdev, uint8_t byteRequest, uint16_t wIndex, unsigned char* data, uint16_t wLength );

int smx20_usb_ControlWriteSpecial(libusb_device_handle* hUSBdev, uint8_t byteRequest, uint16_t wValue, uint16_t wIndex, unsigned char* data, uint16_t wLength );

