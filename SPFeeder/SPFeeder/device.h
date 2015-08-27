//
// Define below GUIDs
//
#include <initguid.h>

//
// Device Interface GUID.
// Used by all WinUsb devices that this application talks to.
// Must match "DeviceInterfaceGUIDs" registry value specified in the INF file.
// 39fe611f-8055-420e-9393-e3fa316d335b
//
DEFINE_GUID(GUID_DEVINTERFACE_WinUSB_Test,
	0x88BAE032, 0x5A81, 0x49f0, 0xBC, 0x3D, 0xA4, 0xFF, 0x13, 0x82, 0x16, 0xD6);

typedef struct _DEVICE_DATA {

    BOOL                    HandlesOpen;
    WINUSB_INTERFACE_HANDLE WinusbHandle;
    HANDLE                  DeviceHandle;
    TCHAR                   DevicePath[MAX_PATH];

} DEVICE_DATA, *PDEVICE_DATA;

HRESULT OpenDevice(_Out_ PDEVICE_DATA DeviceData, _Out_opt_ PBOOL FailureDeviceNotFound);
VOID CloseDevice(_Inout_ PDEVICE_DATA DeviceData);
