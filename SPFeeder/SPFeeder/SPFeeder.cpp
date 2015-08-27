// SPFeeder.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <Psapi.h>
#include "resource.h"
#include "tray.h"
#include "usb.h"
#include "SPFeeder.h"


#define	DEBUG_CONSOLE


void RegisterMainWndClass(HINSTANCE hInstance);


BOOL g_bModalState = FALSE;


// ------------------------------------------------------------------------------------------------
// Main entry point
//
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef DEBUG_CONSOLE
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	printf("Starting...\n");
#endif

	HMENU	hSysMenu = NULL;
	HWND	hWnd = NULL;
	HWND	hPrev = NULL;
	MSG		msg;

	// create invisible window to receive messages
	RegisterMainWndClass(hInstance);
	hWnd = CreateWindow(THIS_CLASSNAME, THIS_TITLE, 0, 0, 0, 100, 100, NULL, NULL, hInstance, NULL);
	if (!hWnd)
	{
		MessageBox(NULL, _T("Unable to create main window!"), THIS_TITLE, MB_ICONERROR | MB_OK | MB_TOPMOST);
		return(1);
	}


	// start USB handler thread
	DWORD	id;
	HANDLE hconin = CreateThread(NULL, 0, USB_task, hWnd, 0, &id);
	if (hconin == INVALID_HANDLE_VALUE)
	{
		printf(__FUNCTION__ ": CreateThread() failed");
		return -1;
	}
	CloseHandle(hconin);	// no longer required


	// message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnregisterClass(THIS_CLASSNAME, hInstance);

	return 0;
}

// ------------------------------------------------------------------------------------------------
// Message handler
//
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		AddTrayIcon(hWnd, ID_TRAYICON, APPWM_TRAYICON, IDI_MEMORY, _T("Looking for SUPERPLAY..."));
		//MonitorProcess(proc_name, limit_mb, hWnd, ID_TRAYICON);
		return(0);

	case APPWM_NOP:			// see OnTrayIconRBtnUp()
		return(0);

	case APPWM_TRAYICON:	// mouse events involving our tray icon
		SetForegroundWindow(hWnd);

		switch (lParam)
		{
		case WM_RBUTTONUP:	// see OnTrayIconRBtnUp()
			OnTrayIconRBtnUp(hWnd);
			return(0);

		}
		return(0);

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_ABOUT:
			g_bModalState = TRUE;
			MessageBox(hWnd, HELP_ABOUT, THIS_TITLE, MB_ICONINFORMATION | MB_OK);
			g_bModalState = FALSE;
			break;
		case ID_EXIT:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		}
		return(0);

	case WM_CLOSE:
		OnClose(hWnd);
		// fall through
	default:
		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
	}
}

// ------------------------------------------------------------------------------------------------
void RegisterMainWndClass(HINSTANCE hInstance)
{
	WNDCLASSEX	wc;
	memset(&wc, 0, sizeof(wc));

	wc.cbSize = sizeof(wc);
	wc.style = 0;
	wc.lpfnWndProc = &WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);	// special magic
	wc.lpszMenuName = NULL;
	wc.lpszClassName = THIS_CLASSNAME;

	RegisterClassEx(&wc);
}
