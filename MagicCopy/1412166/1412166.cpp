// 1412166.cpp : Defines the entry point for the application.
//
// Dinh nghia message se duoc send cho Window khi user tac dong len Tray-Icon


#include "stdafx.h"
#include "1412166.h"
#include <windowsx.h>
#include <sstream>
#include <vector>
#include <shellapi.h>

#define MAX_LOADSTRING 100

#define  MY_WM_NOTIFYICON  WM_USER+1
// Dinh nghia ID cua Icon tren Status Area
#define  TRAY_ICON_ID	1

// Global Variables:
HINSTANCE hInst;                                // current instance
HINSTANCE hinstLib;
HHOOK hMouseHook;
HWND hDialog;
HMENU hmenu;
HMENU hmenuTrackPopup;
static POINT positionOfMouseDown;

BOOL MyTaskBarAddIcon(HWND hWnd, UINT uID, HICON hIcon, LPCWSTR lpszTip);
BOOL MyTaskBarDeleteIcon(HWND hWnd, UINT uID);
void  doSystemTray(HWND hWnd);
void doMyNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    UIProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

	hInst = hInstance;

	hDialog = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG_UI), 0, UIProc);

	// Perform application initialization:
	if (!hDialog)
	{
		TCHAR buff[100];
		wsprintf(buff, L"Error x%x", GetLastError());
		MessageBox(0, buff, L"Create Dialog Error", MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}

	ShowWindow(hDialog, nCmdShow);
	UpdateWindow(hDialog);
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1412166));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



INT_PTR CALLBACK UIProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
	{
		hmenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1));
		hmenuTrackPopup = GetSubMenu(hmenu, 0);
		return (INT_PTR)TRUE;
	}
	break;
	case MY_WM_NOTIFYICON:
	{
		doMyNotify(hDlg, wParam, lParam);
	}
	break;
    case WM_COMMAND:
	{
		int id = LOWORD(wParam);
		switch (id)
		{
		case IDC_BUTTON_OK:
		{
			if (SendDlgItemMessage(hDlg, IDC_CHECK_TURNON, BM_GETCHECK, 0, 0))
				doInstallMouseHook();
			else
				doRemoveMouseHook();
			doSystemTray(hDlg);
		}
		break;
		case IDC_BUTTON_QUIT:
		{
			SendMessage(hDlg, WM_CLOSE, 0, 0);
		}
		break;
		case ID_CUT:
		{
			cut();
		}
		break;
		case ID_COPY:
		{
			copy();
		}
		break;
		case ID_PASTE:
		{
			paste();
		}
		break;
		case ID_SKYPE:
		{
			copy();
			HWND hWndSkypeMain = FindWindow(L"tSkMainForm", NULL);
			HWND hWndSkype1 = FindWindowEx(hWndSkypeMain, NULL, L"TConversationForm", NULL);
			HWND hWndSkype2 = FindWindowEx(hWndSkype1, NULL, L"TChatEntryControl", NULL);
			HWND hWndSkype3 = FindWindowEx(hWndSkype2, NULL, L"TChatRichEdit", NULL);
			Sleep(100);
			TCHAR *test = getContentFromClipboard();

			SendMessage(hWndSkype3, WM_SETTEXT, NULL, (LPARAM)test);
			SetForegroundWindow(hWndSkypeMain);
			//FORWARD_WM_KEYDOWN(hWndSkype, VK_RETURN, 0, 0, SendMessage);
		}
		break;
		case ID_SEARCH:
		{
			copy();
			Sleep(1000);
			TCHAR *test = getContentFromClipboard();
			std::wstring link = getLinkSearch(test);
			std::wstring path = TEXT("start ") + link;
			const std::string s(path.begin(), path.end());
			system(s.c_str());
		}
		break;
		}
	}
	break;
	case WM_CLOSE:
	{
		doRemoveMouseHook();
		DestroyMenu(hmenu);
		PostQuitMessage(0);
	}
		break;
    }
    return (INT_PTR)FALSE;
}



void showMenuPopup(POINT p)
{
	TrackPopupMenu(hmenuTrackPopup,
		TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		p.x, p.y, 0, hDialog, NULL);
}


TCHAR* getContentFromClipboard()
{
	TCHAR *szText = NULL;
	if (OpenClipboard(NULL))
	{
		HANDLE hData = GetClipboardData(CF_UNICODETEXT);
		if (hData != NULL)
		{
			TCHAR* pData = (TCHAR*)GlobalLock(hData);
			if (pData != NULL && wcslen(pData) > 0)
			{
				szText = new TCHAR[wcslen(pData) + 1];
				wcscpy(szText, pData);
			}
			GlobalUnlock(hData);
		}
		CloseClipboard();
	}
	return szText;
}

void CancelPopupMenu()
{
	HWND hwnd = GetLastActivePopup(hDialog);
	if (hwnd)
		FORWARD_WM_KEYDOWN(hwnd, VK_ESCAPE, 0, 0, PostMessage);
}

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) // không xử lý message 
		return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
	MOUSEHOOKSTRUCT *mHookData = (MOUSEHOOKSTRUCT *)lParam;
	if (wParam == WM_LBUTTONUP)
	{
		if ((mHookData->pt.x != positionOfMouseDown.x || mHookData->pt.y != positionOfMouseDown.y))
		{
			showMenuPopup(mHookData->pt);
		}
		else
			CancelPopupMenu();
	}

	if (wParam == WM_LBUTTONDOWN)
		positionOfMouseDown = mHookData->pt;

	return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
}

void doInstallMouseHook()
{
	if (hMouseHook != NULL) return;

	hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)MouseHookProc, hinstLib, 0);
}

void doRemoveMouseHook()
{
	if (hMouseHook == NULL) return;
	UnhookWindowsHookEx(hMouseHook);
	hMouseHook = NULL;
}


void cut()
{
	LPINPUT pInput = new INPUT[2]();
	pInput[0].type = INPUT_KEYBOARD;
	pInput[0].ki.wScan = 0;
	pInput[0].ki.time = 0;
	pInput[0].ki.dwExtraInfo = 0;
	pInput[0].ki.wVk = VK_CONTROL;
	pInput[0].ki.dwFlags = 0;

	pInput[1].type = INPUT_KEYBOARD;
	pInput[1].ki.wScan = 0;
	pInput[1].ki.time = 0;
	pInput[1].ki.dwExtraInfo = 0;
	pInput[1].ki.wVk = 0x58;
	pInput[1].ki.dwFlags = 0;

	SendInput(2, pInput, sizeof(INPUT));
	pInput[0].ki.dwFlags = KEYEVENTF_KEYUP;
	pInput[1].ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(2, pInput, sizeof(INPUT));
}
void copy()
{
	LPINPUT pInput = new INPUT[2]();
	pInput[0].type = INPUT_KEYBOARD;
	pInput[0].ki.wScan = 0;
	pInput[0].ki.time = 0;
	pInput[0].ki.dwExtraInfo = 0;
	pInput[0].ki.wVk = VK_CONTROL;
	pInput[0].ki.dwFlags = 0;

	pInput[1].type = INPUT_KEYBOARD;
	pInput[1].ki.wScan = 0;
	pInput[1].ki.time = 0;
	pInput[1].ki.dwExtraInfo = 0;
	pInput[1].ki.wVk = 0x43;
	pInput[1].ki.dwFlags = 0;

	SendInput(2, pInput, sizeof(INPUT));
	pInput[0].ki.dwFlags = KEYEVENTF_KEYUP;
	pInput[1].ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(2, pInput, sizeof(INPUT));
}

void paste()
{
	LPINPUT pInput = new INPUT[2]();
	pInput[0].type = INPUT_KEYBOARD;
	pInput[0].ki.wScan = 0;
	pInput[0].ki.time = 0;
	pInput[0].ki.dwExtraInfo = 0;
	pInput[0].ki.wVk = VK_CONTROL;
	pInput[0].ki.dwFlags = 0;

	pInput[1].type = INPUT_KEYBOARD;
	pInput[1].ki.wScan = 0;
	pInput[1].ki.time = 0;
	pInput[1].ki.dwExtraInfo = 0;
	pInput[1].ki.wVk = 0x56;
	pInput[1].ki.dwFlags = 0;

	SendInput(2, pInput, sizeof(INPUT));
	pInput[0].ki.dwFlags = KEYEVENTF_KEYUP;
	pInput[1].ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &pInput[1], sizeof(INPUT));
	SendInput(1, &pInput[0], sizeof(INPUT));
}


std::wstring getLinkSearch(TCHAR* text)
{
	const std::wstring baseLink = TEXT("https://www.google.com.vn/?gws_rd=ssl#q=");
	std::wstring textString = std::wstring(text);
	std::wstring link = baseLink;
	std::wstring temp;
	std::vector<std::wstring> parts;
	std::wstringstream wss(textString);
	while (std::getline(wss, temp, L';'))
		parts.push_back(temp);
	for (int i = 0; i < parts.size(); i++)
		link += UrlEncode(parts[i]) + L"+";
	link.pop_back();
	return link;
}

const wchar_t * hexenc[] = {
	L"%00", L"%01", L"%02", L"%03", L"%04", L"%05", L"%06", L"%07",
	L"%08", L"%09", L"%0a", L"%0b", L"%0c", L"%0d", L"%0e", L"%0f",
	L"%10", L"%11", L"%12", L"%13", L"%14", L"%15", L"%16", L"%17",
	L"%18", L"%19", L"%1a", L"%1b", L"%1c", L"%1d", L"%1e", L"%1f",
	L"%20", L"%21", L"%22", L"%23", L"%24", L"%25", L"%26", L"%27",
	L"%28", L"%29", L"%2a", L"%2b", L"%2c", L"%2d", L"%2e", L"%2f",
	L"%30", L"%31", L"%32", L"%33", L"%34", L"%35", L"%36", L"%37",
	L"%38", L"%39", L"%3a", L"%3b", L"%3c", L"%3d", L"%3e", L"%3f",
	L"%40", L"%41", L"%42", L"%43", L"%44", L"%45", L"%46", L"%47",
	L"%48", L"%49", L"%4a", L"%4b", L"%4c", L"%4d", L"%4e", L"%4f",
	L"%50", L"%51", L"%52", L"%53", L"%54", L"%55", L"%56", L"%57",
	L"%58", L"%59", L"%5a", L"%5b", L"%5c", L"%5d", L"%5e", L"%5f",
	L"%60", L"%61", L"%62", L"%63", L"%64", L"%65", L"%66", L"%67",
	L"%68", L"%69", L"%6a", L"%6b", L"%6c", L"%6d", L"%6e", L"%6f",
	L"%70", L"%71", L"%72", L"%73", L"%74", L"%75", L"%76", L"%77",
	L"%78", L"%79", L"%7a", L"%7b", L"%7c", L"%7d", L"%7e", L"%7f",
	L"%80", L"%81", L"%82", L"%83", L"%84", L"%85", L"%86", L"%87",
	L"%88", L"%89", L"%8a", L"%8b", L"%8c", L"%8d", L"%8e", L"%8f",
	L"%90", L"%91", L"%92", L"%93", L"%94", L"%95", L"%96", L"%97",
	L"%98", L"%99", L"%9a", L"%9b", L"%9c", L"%9d", L"%9e", L"%9f",
	L"%a0", L"%a1", L"%a2", L"%a3", L"%a4", L"%a5", L"%a6", L"%a7",
	L"%a8", L"%a9", L"%aa", L"%ab", L"%ac", L"%ad", L"%ae", L"%af",
	L"%b0", L"%b1", L"%b2", L"%b3", L"%b4", L"%b5", L"%b6", L"%b7",
	L"%b8", L"%b9", L"%ba", L"%bb", L"%bc", L"%bd", L"%be", L"%bf",
	L"%c0", L"%c1", L"%c2", L"%c3", L"%c4", L"%c5", L"%c6", L"%c7",
	L"%c8", L"%c9", L"%ca", L"%cb", L"%cc", L"%cd", L"%ce", L"%cf",
	L"%d0", L"%d1", L"%d2", L"%d3", L"%d4", L"%d5", L"%d6", L"%d7",
	L"%d8", L"%d9", L"%da", L"%db", L"%dc", L"%dd", L"%de", L"%df",
	L"%e0", L"%e1", L"%e2", L"%e3", L"%e4", L"%e5", L"%e6", L"%e7",
	L"%e8", L"%e9", L"%ea", L"%eb", L"%ec", L"%ed", L"%ee", L"%ef",
	L"%f0", L"%f1", L"%f2", L"%f3", L"%f4", L"%f5", L"%f6", L"%f7",
	L"%f8", L"%f9", L"%fa", L"%fb", L"%fc", L"%fd", L"%fe", L"%ff"
};

std::wstring UrlEncode(std::wstring url)
{
	size_t len = url.length();
	std::wstring encoded = L"";
	for (size_t i = 0; i < len; i++)
	{
		wchar_t wch = url.at(i);
		if ('A' <= wch && wch <= 'Z') {
			encoded += wch;
		}
		else if ('a' <= wch && wch <= 'z') {
			encoded += wch;
		}
		else if ('0' <= wch && wch <= '9') {
			encoded += wch;
		}
		else if (wch == ' ') {
			encoded += hexenc[wch];
		}
		else if (wch == '-' || wch == '_'
			|| wch == '.' || wch == '!'
			|| wch == '~' || wch == '*'
			|| wch == '\'' || wch == '('
			|| wch == ')') {
			encoded += hexenc[wch];
		}
		else if (wch <= 0x007f) {        // other ASCII
			encoded += hexenc[wch];
		}
		else if (wch <= 0x07FF) {        // non-ASCII <= 0x7FF
			encoded += hexenc[0xc0 | (wch >> 6)];
			encoded += hexenc[0x80 | (wch & 0x3F)];
		}
		else {                    // 0x7FF < ch <= 0xFFFF
			encoded += hexenc[0xe0 | (wch >> 12)];
			encoded += hexenc[0x80 | ((wch >> 6) & 0x3F)];
			encoded += hexenc[0x80 | (wch & 0x3F)];
		}
	}
	return encoded.c_str();
}

// -----------------------------------------------------------------
// MyTaskBarAddIcon - Them icon vao Status Area cua Taskbar 
// Ket qua tra ve: TRUE neu thanh cong; FALSE neu khong thanh cong
// hWnd - handle cua Window se nhan thong diep CallBack 
// uID - identifier cua icon se them
// hicon - handle cua icon dung de them 
// lpszTip - ToolTip text (xuat hien tren icon khi user dua mouse vao icon)
// -----------------------------------------------------------------
BOOL MyTaskBarAddIcon(HWND hWnd, UINT uID, HICON hIcon, LPCWSTR lpszTip)
{
	BOOL Kq;
	NOTIFYICONDATA tnid;

	tnid.cbSize = sizeof(NOTIFYICONDATA);
	tnid.hWnd = hWnd;
	tnid.uID = uID;
	tnid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	tnid.uCallbackMessage = MY_WM_NOTIFYICON;
	tnid.hIcon = hIcon;
	if (lpszTip)
		lstrcpyn(tnid.szTip, lpszTip, sizeof(tnid.szTip));
	else
		tnid.szTip[0] = '\0';

	Kq = Shell_NotifyIcon(NIM_ADD, &tnid);

	if (hIcon)
		DestroyIcon(hIcon);

	return Kq;
}

// -----------------------------------------------------------------
// MyTaskBarDeleteIcon - Xoa icon tren Status Area cua Taskbar 
// Ket qua tra ve: TRUE neu thanh cong; FALSE neu khong thanh cong 
// hWnd - handle cua Window da Add icon vao Status Area
// uID - identifier cua Icon se bi xoa 
// -----------------------------------------------------------------
BOOL MyTaskBarDeleteIcon(HWND hWnd, UINT uID)
{
	BOOL Kq;
	NOTIFYICONDATA tnid;

	tnid.cbSize = sizeof(NOTIFYICONDATA);
	tnid.hWnd = hWnd;
	tnid.uID = uID;

	Kq = Shell_NotifyIcon(NIM_DELETE, &tnid);
	return Kq;
}

void  doSystemTray(HWND hWnd)
{
	ShowWindow(hWnd, SW_HIDE);

	HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));

	MyTaskBarAddIcon(hWnd, TRAY_ICON_ID, hIcon, (LPCWSTR)L"Demo App. Click to restore !");
}

void doMyNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (lParam == WM_LBUTTONDOWN) {

		MyTaskBarDeleteIcon(hWnd, TRAY_ICON_ID);

		ShowWindow(hWnd, SW_SHOW);

		ShowWindow(hWnd, SW_RESTORE);
	}
}
