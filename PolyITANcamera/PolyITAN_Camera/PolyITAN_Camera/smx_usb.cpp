#include "smx_usb.h"
#include <stdio.h>

int smx20_usb_ControlRead(libusb_device_handle* hUSBdev, uint8_t byteRequest, uint16_t wIndex, unsigned char* data, uint16_t wLength )
{
	//printf("Reading byteRequest=%d, wIndex=0x%04x, wLength=%d\n", byteRequest, wIndex, wLength);
	return libusb_control_transfer(hUSBdev,
	                               LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_IN /*device-to-host*/ | LIBUSB_RECIPIENT_DEVICE,
                                   byteRequest,
                                   0, // wValue is always zero
                                   wIndex,
                                   data,
                                   wLength,
                                   500 // ms
								 );
}


int smx20_usb_ControlWrite(libusb_device_handle* hUSBdev, uint8_t byteRequest, uint16_t wIndex, unsigned char* data, uint16_t wLength )
{
	//printf("Writing byteRequest=%d, wIndex=0x%04x, wLength=%d\n", byteRequest, wIndex, wLength);
	return libusb_control_transfer(hUSBdev, 
	                               LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_OUT /*host-to-device*/ | LIBUSB_RECIPIENT_DEVICE,
                                   byteRequest,
                                   0, // wValue is always zero
                                   wIndex,
                                   data,
                                   wLength,
                                   500 // ms
								 );
}

int smx20_usb_ControlWriteSpecial(libusb_device_handle* hUSBdev, uint8_t byteRequest, uint16_t wValue, uint16_t wIndex, unsigned char* data, uint16_t wLength )
{
	//printf("Writing special bmRequestType=%x, byteRequest=%x, wIndex=0x%04x, wLength=%x\n", LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_OUT /*host-to-device*/ | LIBUSB_RECIPIENT_DEVICE, byteRequest, wIndex, wLength);
	return libusb_control_transfer(hUSBdev, 
	                               LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_OUT /*host-to-device*/ | LIBUSB_RECIPIENT_DEVICE,
                                   byteRequest,
                                   wValue,
                                   wIndex,
                                   data,
                                   wLength,
                                   500 // ms
								 );
}
