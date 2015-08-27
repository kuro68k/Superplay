#pragma once

#include "resource.h"

enum {
	ID_TRAYICON = 1,
	APPWM_TRAYICON = WM_APP,
	APPWM_NOP = WM_APP + 1,

	ID_ABOUT = 2000,
	ID_EXIT,

	IDT_CHECKPROC = 1,
};


#define THIS_CLASSNAME      _T("SPFeeder hidden window")
#define THIS_TITLE          _T("SPFeeder")

#define	HELP_ABOUT			THIS_TITLE _T(" V1.0\n(C) 2015 Paul Qureshi")


extern BOOL g_bModalState;
