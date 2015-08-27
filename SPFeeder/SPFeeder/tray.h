#pragma once

extern void AddTrayIcon(HWND hWnd, UINT uID, UINT uCallbackMsg, UINT uIcon, LPWSTR pszToolTip);
extern void ModifyTrayIcon(HWND hWnd, UINT uID, UINT uIcon, LPWSTR pszToolTip);
extern void RemoveTrayIcon(HWND hWnd, UINT uID);
extern void DisplayBalloon(HWND hWnd, UINT uID, LPWSTR pszMessage);
extern void OnClose(HWND hWnd);
extern BOOL ShowPopupMenu(HWND hWnd, POINT *curpos, int wDefaultItem);
extern void OnTrayIconRBtnUp(HWND hWnd);
