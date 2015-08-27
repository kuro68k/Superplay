/*
*	System tray handling
*	Created: 27/08/2015
*	Author: ゼーロ
*/

#include "stdafx.h"
#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <Psapi.h>
#include <winusb.h>
#include <usb.h>
#include "resource.h"
#include "device.h"
#include "SPFeeder.h"
#include "tray.h"

// ------------------------------------------------------------------------------------------------
void PrintLastError(void)
{
	DWORD error = GetLastError();
	if (error == 0)
	{
		printf("No error.\n");
		return;
	}

	LPSTR buffer = 0;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&buffer, 0, NULL);
	if (size != 0)
		printf("%s\n", buffer);
	else
		printf("Failed to get error message.\n");
}

// ------------------------------------------------------------------------------------------------
bool GetWinUSBDevice(DEVICE_DATA *deviceData, UCHAR *inPipe)
{
	HRESULT					hr;
	USB_DEVICE_DESCRIPTOR	deviceDesc;
	BOOL					noDevice;
	BOOL					bResult;
	ULONG					lengthReceived;

	*inPipe = 0;

	// Find a device connected to the system that has WinUSB installed using our
	// INF
	hr = OpenDevice(deviceData, &noDevice);

	if (FAILED(hr)) {
		if (noDevice) {
			printf("Device not connected or driver not installed\n");
		}
		else {
			printf("Failed looking for device, HRESULT 0x%x\n", hr);
		}
		return false;
	}

	// Get device descriptor
	bResult = WinUsb_GetDescriptor(deviceData->WinusbHandle,
		USB_DEVICE_DESCRIPTOR_TYPE,
		0,
		0,
		(PBYTE)&deviceDesc,
		sizeof(deviceDesc),
		&lengthReceived);

	if (FALSE == bResult || lengthReceived != sizeof(deviceDesc)) {

		printf("Error among LastError %d or lengthReceived %d\n",
			FALSE == bResult ? GetLastError() : 0,
			lengthReceived);
		CloseDevice(deviceData);
		return false;
	}

	// set alternative setting
	if (!WinUsb_SetCurrentAlternateSetting(deviceData->WinusbHandle, 1))
	{
		printf("Unable to set alternative setting.\n");
		return false;
	}

	// Print a few parts of the device descriptor
	printf("Device found: VID_%04X&PID_%04X; bcdUsb %04X\n",
		deviceDesc.idVendor,
		deviceDesc.idProduct,
		deviceDesc.bcdUSB);
	printf("Configurations:\t%u\n", deviceDesc.bNumConfigurations);
	printf("\n");

	USB_INTERFACE_DESCRIPTOR intDesc;
	if (!WinUsb_QueryInterfaceSettings(deviceData->WinusbHandle, 1, &intDesc))
	{
		printf("Error calling QueryInterfaceSettings()\n.");
		return false;
	}

	printf("Endoints:\t%u\n", intDesc.bNumEndpoints);
	printf("Class:\t\t%u\n", intDesc.bInterfaceClass);
	printf("Subclass:\t%u\n", intDesc.bInterfaceSubClass);

	for (int index = 0; index < intDesc.bNumEndpoints; index++)
	{
		WINUSB_PIPE_INFORMATION pipe;
		if (WinUsb_QueryPipe(deviceData->WinusbHandle, 1, (UCHAR)index, &pipe))
		{
			printf("\tPipe index:\t%u\n", index);
			printf("\tPipe type:\t%u ", pipe.PipeType);
			if (pipe.PipeType == UsbdPipeTypeInterrupt)
				printf("(interrupt, ");
			else
				printf("(other, ");
			if (USB_ENDPOINT_DIRECTION_IN(pipe.PipeId))
				printf("in)\n");
			else
				printf("out)\n");

			if (pipe.PipeType == UsbdPipeTypeInterrupt)
			{
				if (USB_ENDPOINT_DIRECTION_IN(pipe.PipeId))
					*inPipe = pipe.PipeId;
			}
		}
	}
	printf("\n");

	printf("In pipe:\t%u\n", *inPipe);
	if (*inPipe == 0)
	{
		printf("Unable to find in pipe.\n");
		return false;
	}

	return true;
}

// ------------------------------------------------------------------------------------------------
DWORD WINAPI USB_task(LPVOID lpParam)
{
	HWND hWnd = (HWND)lpParam;

	printf("USB handler starting...\n");

	DEVICE_DATA	deviceData;
	HRESULT		hr;
	BOOL		noDevice;

	for (;;)
	{
		hr = OpenDevice(&deviceData, &noDevice);
		if (!FAILED(hr))
		{
			printf(__FUNCTION__ ": device found\n");

			UCHAR inPipe = 0;
			if (GetWinUSBDevice(&deviceData, &inPipe))
			{
				// keep reading pipe until it fails
			}
			else
				printf(__FUNCTION__ ": GetSinUSBDevice() failed\n");
		}
		else
			printf(__FUNCTION__ ": no device\n");
		Sleep(1000);
	}
}
