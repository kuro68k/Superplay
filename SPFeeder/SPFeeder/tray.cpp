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
#include "resource.h"
#include "SPFeeder.h"


// ------------------------------------------------------------------------------------------------
// Add system tray icon
//
void AddTrayIcon(HWND hWnd, UINT uID, UINT uCallbackMsg, UINT uIcon, LPWSTR pszToolTip)
{
	NOTIFYICONDATA	nid;

	memset(&nid, 0, sizeof(nid));

	nid.cbSize = sizeof(nid);
	nid.hWnd = hWnd;
	nid.uID = uID;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = uCallbackMsg;
	nid.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(uIcon));

	wcscpy(nid.szTip, pszToolTip);

	Shell_NotifyIcon(NIM_ADD, &nid);
}

// ------------------------------------------------------------------------------------------------
// Update tray icon
//
void ModifyTrayIcon(HWND hWnd, UINT uID, UINT uIcon, LPWSTR pszToolTip)
{
	NOTIFYICONDATA	nid;

	memset(&nid, 0, sizeof(nid));

	nid.cbSize = sizeof(nid);
	nid.hWnd = hWnd;
	nid.uID = uID;

	if (uIcon != NULL)
	{
		nid.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(uIcon));
		nid.uFlags |= NIF_ICON;
	}

	if (pszToolTip)
	{
		wcscpy(nid.szTip, pszToolTip);
		nid.uFlags |= NIF_TIP;
	}

	if ((uIcon != NULL) || (pszToolTip))
		Shell_NotifyIcon(NIM_MODIFY, &nid);
}

// ------------------------------------------------------------------------------------------------
// Remove system tray icon
//
void RemoveTrayIcon(HWND hWnd, UINT uID)
{
	NOTIFYICONDATA	nid;

	memset(&nid, 0, sizeof(nid));

	nid.cbSize = sizeof(nid);
	nid.hWnd = hWnd;
	nid.uID = uID;

	Shell_NotifyIcon(NIM_DELETE, &nid);
}

// ------------------------------------------------------------------------------------------------
// Display a pop-up balloon message
//
void DisplayBalloon(HWND hWnd, UINT uID, LPWSTR pszMessage)
{
	NOTIFYICONDATA	nid;

	memset(&nid, 0, sizeof(nid));

	nid.cbSize = sizeof(nid);
	nid.hWnd = hWnd;
	nid.uID = uID;
	nid.uFlags = NIF_INFO;
	wcscpy(nid.szInfoTitle, THIS_TITLE);
	wcscpy(nid.szInfo, pszMessage);
	nid.uTimeout = 5000;

	Shell_NotifyIcon(NIM_MODIFY, &nid);
}

// ------------------------------------------------------------------------------------------------
void OnClose(HWND hWnd)
{
	RemoveTrayIcon(hWnd, ID_TRAYICON);
	PostQuitMessage(0);
}

// ------------------------------------------------------------------------------------------------
// Create pop-up menu when tray icon right-clicked
//
BOOL ShowPopupMenu(HWND hWnd, POINT *curpos, int wDefaultItem)
{
	HMENU	hPop = NULL;
	int		i = 0;
	WORD	cmd;
	POINT	pt;

	if (g_bModalState)	// already displayed
		return(false);

	hPop = CreatePopupMenu();

	if (!curpos)
	{
		GetCursorPos(&pt);
		curpos = &pt;
	}

	InsertMenu(hPop, i++, MF_BYPOSITION | MF_STRING, ID_ABOUT, _T("About"));
	InsertMenu(hPop, i++, MF_BYPOSITION | MF_STRING, ID_EXIT, _T("Exit"));
	//SetMenuDefaultItem(hPop, ID_ABOUT, FALSE);

	SetFocus(hWnd);
	SendMessage(hWnd, WM_INITMENUPOPUP, (WPARAM)hPop, 0);

	cmd = TrackPopupMenu(hPop, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, curpos->x, curpos->y, 0, hWnd, NULL);
	SendMessage(hWnd, WM_COMMAND, cmd, 0);

	DestroyMenu(hPop);
	return(cmd);
}

// ------------------------------------------------------------------------------------------------
void OnTrayIconRBtnUp(HWND hWnd)
{
	SetForegroundWindow(hWnd);
	ShowPopupMenu(hWnd, NULL, -1);
	PostMessage(hWnd, APPWM_NOP, 0, 0);
}
