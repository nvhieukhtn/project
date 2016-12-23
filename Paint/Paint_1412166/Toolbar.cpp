#include "stdafx.h"
#include   <windows.h>		
#include   <CommCtrl.h>
#include   "resource.h"

void doCreateToolbar(HWND hWnd,HWND &hWndToolbar)
{
	// loading Common Control DLL
	InitCommonControls();

	TBBUTTON tbButtons[] =
	{
		// Zero-based Bitmap image, ID of command, Button state, Button style, 
		// ...App data, Zero-based string (Button's label)
		{ STD_FILENEW,	IDM_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
		{ STD_FILEOPEN,	IDM_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
		{ STD_FILESAVE,IDM_SAVE,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0 }
	};

	// create a toolbar
	hWndToolbar = CreateToolbarEx(hWnd,
		WS_CHILD | WS_VISIBLE | CCS_ADJUSTABLE | TBSTYLE_TOOLTIPS,
		ID_TOOLBAR,
		sizeof(tbButtons) / sizeof(TBBUTTON),
		HINST_COMMCTRL,
		0,
		tbButtons,
		sizeof(tbButtons) / sizeof(TBBUTTON),
		50,
		50,
		50,
		50,
		sizeof(TBBUTTON));
}

//  ---------------------------------------------------------------------
//    doToolBar_NotifyHandle Function
//		+ handling the Notify message of the Toolbar: displaying ToolTip text
//  ---------------------------------------------------------------------
void doToolBar_NotifyHandle(LPARAM	lParam, HINSTANCE hInst)
{
	LPTOOLTIPTEXT   lpToolTipText;
	TCHAR			szToolTipText[50]; 	// ToolTipText, loaded from Stringtable resource

										// lParam: address of TOOLTIPTEXT struct
	lpToolTipText = (LPTOOLTIPTEXT)lParam;

	if (lpToolTipText->hdr.code == TTN_NEEDTEXT)
	{
		// hdr.iFrom: ID cua ToolBar button -> ID cua ToolTipText string
		LoadString(hInst, lpToolTipText->hdr.idFrom, szToolTipText, 50);

		lpToolTipText->lpszText = szToolTipText;
	}
}

//  ---------------------------------------------------------------------
//    doToolBar_Style Function
//		+ changing style of ToolBar
//  ---------------------------------------------------------------------
void doToolBar_Style(int tbStyle, HWND &hWndToolbar)
{
	ShowWindow(hWndToolbar, SW_HIDE);
	SendMessage(hWndToolbar, TB_SETSTYLE, (WPARAM)0, (LPARAM)(DWORD)tbStyle | CCS_TOP);
	ShowWindow(hWndToolbar, SW_SHOW);
}

void doToolbarShape(HINSTANCE hInst, HWND &hWndToolbar)
{
	TBBUTTON tbButtons[] =
	{
		{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP,0,0 },
		{ 0, IDM_LINE, TBSTATE_ENABLED,TBSTYLE_BUTTON, 0 ,0 },
		{ 1, IDM_RECTANGE, TBSTATE_ENABLED,TBSTYLE_BUTTON, 0 ,0 },
		{ 2, IDM_ELLIPSE, TBSTATE_ENABLED,TBSTYLE_BUTTON, 0 ,0 }
	};
	TBADDBITMAP tbab;
	tbab.hInst = hInst;
	tbab.nID = IDB_BITMAP_LINE;
	int iLine = SendMessage(hWndToolbar, TB_ADDBITMAP, 0, (LPARAM)&tbab);
	tbButtons[1].iBitmap += iLine;
	tbab.nID = IDB_BITMAP_RECTANGE;
	int iEllipse = SendMessage(hWndToolbar, TB_ADDBITMAP, 0, (LPARAM)&tbab);
	tbButtons[2].iBitmap += iEllipse;
	tbab.nID = IDB_BITMAP_ELLIPSE;
	int iRectange = SendMessage(hWndToolbar, TB_ADDBITMAP, 0, (LPARAM)&tbab);
	tbButtons[3].iBitmap += iRectange;
	SendMessage(hWndToolbar, TB_ADDBUTTONS, sizeof(tbButtons) / sizeof(TBBUTTON), (LPARAM)&tbButtons);
}
void onDisableToolbarButton(HWND &hWndToolbar)
{
	SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_SAVE, FALSE);
	SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_LINE, FALSE);
	SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_RECTANGE, FALSE);
	SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_ELLIPSE, FALSE);
}

void onEnableToolbarButton(HWND &hWndToolbar)
{
	SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_SAVE, TRUE);
	SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_LINE, TRUE);
	SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_RECTANGE, TRUE);
	SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_ELLIPSE, TRUE);
}