#include "stdafx.h"
#include   <windows.h>		
#include   <CommCtrl.h>
#include   "resource.h"

void doCreateStatusBar(HWND hwndParent,HWND &hWndStatusbar)
{
	// Nap thu vien Common Control DLL
	InitCommonControls();

	// Tao 1 status bar
	hWndStatusbar = CreateStatusWindow(WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
		TEXT("Ready!"), hwndParent, 1);
}
void doChangeTextStatus( HWND &hWndStatusbar, TCHAR text[])
{
	SendMessage(hWndStatusbar, SB_SETTEXT, NULL, (LPARAM)text);
}