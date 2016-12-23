// Application.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Application.h"
#include "Data.h"
#include <shellapi.h>
#include "Commdlg.h"
using namespace std;


#define MAX_LOADSTRING 100
#define PIANO L"piano.data"
#define BLIND L"blind.data"
typedef VOID(*HOOK)(HWND);
typedef VOID(*SETTING)(map<int, wstring>, bool);
NOTIFYICONDATA notifyIconData;
UINT WM_TASKBAR = 0;
void InitNotifyIconData();
TCHAR szTIP[64] = TEXT("Click to open setting");
TCHAR buffBG[1024];

// Global Variables:
HINSTANCE hInst;                                // current instance
HWND hDialog;
HMENU Hmenu;
HINSTANCE hLib = LoadLibrary(L"AKeysHook.dll");
map<int, wstring> setting4Blind;
map<int, wstring> setting4Piano;
map<int, wstring> setting4User;
bool checkB = false;
int N = 10;


// Forward declarations of functions included in this code module:
BOOL CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK CustomDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
ATOM MyRegisterClass(HINSTANCE hInstance);
void InitNotifyIconData();

void InitNotifyIconData()
{
	memset(&notifyIconData, 0, sizeof(NOTIFYICONDATA));
	notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	notifyIconData.hWnd = hDialog;
	notifyIconData.uID = ID_TRAY_APP_ICON;
	notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	notifyIconData.uCallbackMessage = WM_SYSICON; //Set up our invented Windows Message
	notifyIconData.hIcon = (HICON)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_TRAY));
	wcscpy_s(notifyIconData.szTip, szTIP);
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
	
	WM_TASKBAR = RegisterWindowMessageA("TaskbarCreated");
	// TODO: Place code here.
	MSG msg;

	MyRegisterClass(hInstance);

	hInst = hInstance;

	hDialog = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, DialogProc);

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
	InitNotifyIconData();

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!IsDialogMessage(hDialog, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC) DialogProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = (HICON) LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_TRAY));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _T("KeyboardHook");
	wcex.hIconSm = (HICON) LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_TRAY));

	return RegisterClassEx(&wcex);
}

void loadSetting(map<int, wstring> &setting, wchar_t* path)
{
	Init(setting, path);
}

void setSetting(int choice)
{
	if (hLib)
	{
		SETTING set = (SETTING)GetProcAddress(hLib, "_getAKeySetting");
		if (set)
		{
			switch (choice)
			{
			case IDC_PIANO:
				set(setting4Piano, checkB);
				break;
			case IDC_BLIND:
				set(setting4Blind, checkB);
				break;
			case IDC_CUSTOM:
				set(setting4User, checkB);
				break;
			}
		}
	}
}

BOOL CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	static int choice;
	static HBITMAP hBmp = NULL;

	if (message == WM_TASKBAR && !IsWindowVisible(hDialog))
	{
		ShowWindow(hDialog, SW_HIDE);
		return 0;
	}

    switch (message)
    {
	case WM_INITDIALOG:
	{
		choice = IDC_PIANO;
		CheckRadioButton(hWnd, IDC_PIANO, IDC_CUSTOM, choice);
		SetFocus(GetDlgItem(hWnd, choice));
		Hmenu = CreatePopupMenu();
		AppendMenu(Hmenu, MF_STRING, ID_TRAY_EXIT, TEXT("Exit The Program"));

		hBmp = (HBITMAP)LoadBitmapW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BACKGROUND1));
	
		if (hLib)
		{
			HOOK install = (HOOK)GetProcAddress(hLib, "_doInstallAKeyHook");
			if (install)
				install(hWnd);
		}
		loadSetting(setting4Piano, const_cast<wchar_t*>(PIANO));
		loadSetting(setting4Blind, const_cast<wchar_t*>(BLIND));
	}
		return (INT_PTR)TRUE; 
    case WM_COMMAND:
        {
			wmId = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case IDC_PIANO:
			case IDC_BLIND:
			case IDC_CUSTOM:
				choice = LOWORD(wParam);
				CheckRadioButton(hWnd, IDC_PIANO, IDC_CUSTOM, choice);
				break;
			case IDC_CUSTOMIZE:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, CustomDlg);
				break;
			case IDC_HIDE:
				ShowWindow(hDialog, SW_HIDE);
				Shell_NotifyIcon(NIM_ADD, &notifyIconData);
				break;
			case IDOK:
				setSetting(choice);
				break;
            case IDCANCEL:
                DestroyWindow(hWnd);
                break;
            }
        }
        break;
	case WM_PAINT:
	{
		HDC PaintDC;
		PAINTSTRUCT PS;
		PaintDC = BeginPaint(hWnd, &PS);
		HBITMAP hOldBmp;
		BITMAP bm;
		HDC memDC;
		SetBkMode(PaintDC, TRANSPARENT);
		GetObject(hBmp, sizeof(BITMAP), &bm);
		memDC = CreateCompatibleDC(PaintDC);
		hOldBmp = (HBITMAP)SelectObject(memDC, hBmp);
		BitBlt(PaintDC, 0, 0, bm.bmWidth, bm.bmHeight, memDC, 0, 0, SRCCOPY);
		SelectObject(memDC, hOldBmp);
		DeleteObject(hOldBmp);
		DeleteDC(memDC);
		EndPaint(hWnd, &PS);
	}
		break;
	case WM_SYSICON:
	{
		switch (wParam)
		{
		case ID_TRAY_APP_ICON:
			SetForegroundWindow(hDialog);
			break;
		}
		if (lParam == WM_LBUTTONUP)
			ShowWindow(hDialog, SW_SHOW);
		else if (lParam == WM_RBUTTONDOWN)
		{
			POINT curPoint;
			GetCursorPos(&curPoint);
			SetForegroundWindow(hWnd);
			UINT clicked = TrackPopupMenu(Hmenu, TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x, curPoint.y, 0, hWnd, NULL);
			SendMessage(hWnd, WM_NULL, 0, 0); // send benign message to window to make sure the menu goes away.
			if (clicked == ID_TRAY_EXIT)
			{
				Shell_NotifyIcon(NIM_DELETE, &notifyIconData);
				DestroyWindow(hWnd);
			}
		}
	}
	break;
	case WM_NCHITTEST:
	{
		UINT uHitTest = DefWindowProc(hWnd, WM_NCHITTEST, wParam, lParam);
		if (uHitTest == HTCLIENT)
			return HTCAPTION;
		else
			return uHitTest;
	}
	break;
    case WM_DESTROY:
	{
		if (hLib)
		{
			HOOK remove = (HOOK)GetProcAddress(hLib, "_doRemoveAKeyHook");
			if (remove)
				remove(hWnd);
		}
		FreeLibrary(hLib);
	}
        PostQuitMessage(0);
        break;
    }
	return (INT_PTR)FALSE;
}

int CheckSelected(HWND hDlg)
{
	int itemIndex = SendMessage(GetDlgItem(hDlg, IDC_LISTBUTTON), LB_GETCURSEL, 0, 0);
	int itemIndex1 = SendMessage(GetDlgItem(hDlg, IDC_LISTPATH), LB_GETCURSEL, 0, 0);

	if (itemIndex != LB_ERR)
		return itemIndex;
	if (itemIndex1 != LB_ERR)
		return itemIndex1;
	return -1;
}

INT_PTR CALLBACK CustomDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	HWND item = GetDlgItem(hDlg, IDC_COMBO);
	HWND hBTN = GetDlgItem(hDlg, IDC_LISTBUTTON);
	HWND hPTH = GetDlgItem(hDlg, IDC_LISTPATH);
	HWND background = GetDlgItem(hDlg, IDC_BACKGROUND);
	static HBITMAP hBmp1 = NULL;
	switch (message)
	{
	case WM_INITDIALOG:
	{
		hBmp1 = (HBITMAP)LoadBitmapW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BACKGROUND2));
		if (checkB)
		{
			SendMessage(background, BM_SETCHECK, BST_CHECKED, 0);
			SetDlgItemText(hDlg, IDC_EDITB, setting4User[VK_INSERT].c_str());
		}
		
		for (int i = 0; i < N; i++)
		{
			SendMessage(item, CB_ADDSTRING, 0, (LPARAM)to_wstring(i).c_str());
			SendMessage(item, CB_SETITEMDATA, i, (LPARAM) 48 + i);
		}
		SendMessage(item, CB_SETCURSEL, 0, 0);
	}
		return (INT_PTR)TRUE;

	case WM_PAINT:
	{
		HDC PaintDC;
		PAINTSTRUCT PS;
		PaintDC = BeginPaint(hDlg, &PS);
		HBITMAP hOldBmp;
		BITMAP bm;
		HDC memDC;
		SetBkMode(PaintDC, TRANSPARENT);
		GetObject(hBmp1, sizeof(BITMAP), &bm);
		memDC = CreateCompatibleDC(PaintDC);
		hOldBmp = (HBITMAP)SelectObject(memDC, hBmp1);
		BitBlt(PaintDC, 0, 0, bm.bmWidth, bm.bmHeight, memDC, 0, 0, SRCCOPY);
		SelectObject(memDC, hOldBmp);
		DeleteObject(hOldBmp);
		DeleteDC(memDC);
		EndPaint(hDlg, &PS);
	}
	break;
	case WM_COMMAND:
	{
		int itemIndex = CheckSelected(hDlg);
		TCHAR buff[1024], buff1[1024], buff2[1024];
		
		switch (LOWORD(wParam))
		{
			case IDOK:
			{
				GetDlgItemText(hDlg, IDC_EDITB, buff2, 1024);
				if (SendMessage(background, BM_GETCHECK, 0, 0) == BST_CHECKED && wcscmp(L"", buff2) == 0)
				{
					MessageBox(hDlg, L"Missing path !", L"Warning", MB_OK | MB_ICONWARNING);
					break;
				}

				setting4User.clear();
				int num = SendMessage(hBTN, LB_GETCOUNT, 0, (LPARAM)0);

				if (SendMessage(background, BM_GETCHECK, 0, 0) == BST_CHECKED)
				{
					checkB = true;
					setting4User.insert(pair<int, wstring>(VK_INSERT, buff2));
					setting4Blind.insert(pair<int, wstring>(VK_INSERT, buff2));
					setting4Piano.insert(pair<int, wstring>(VK_INSERT, buff2));
				}
				else
					checkB = false;
				
				for (int i = 0; i < num; i++)
				{
					SendMessage(hBTN, LB_GETTEXT, i, (LPARAM)buff);
					SendMessage(hPTH, LB_GETTEXT, i, (LPARAM)buff1);
					int temp = (int)SendMessage(item, CB_GETITEMDATA, _ttoi(buff), (LPARAM)0);
					setting4User.insert(pair<int, wstring>(temp, buff1));
				}				
			}
				break;
			case IDC_ADD:
				GetDlgItemText(hDlg, IDC_COMBO, buff, 1024);
				GetDlgItemText(hDlg, IDC_EDIT, buff1, 1024);
				if (wcscmp(L"", buff1) != 0)
				{
					int check = SendMessage(hBTN, LB_FINDSTRINGEXACT, 0, (LPARAM)buff);
					int check1 = SendMessage(hPTH, LB_FINDSTRINGEXACT, 0, (LPARAM)buff1);
					if (check == LB_ERR && check1 == LB_ERR)
					{
						SendMessage(hBTN, LB_INSERTSTRING, 0, (LPARAM)buff);
						SendMessage(hPTH, LB_INSERTSTRING, 0, (LPARAM)buff1);
					}
					else
						MessageBox(hDlg, L"Duplicate item !", L"Warning", MB_OK | MB_ICONWARNING);
					SetDlgItemText(hDlg, IDC_EDIT, L"");
				}
				else
					MessageBox(hDlg, L"Missing path !", L"Warning", MB_OK | MB_ICONWARNING);
				break;
			case IDC_ERASE:
				SendMessage(hBTN, LB_DELETESTRING, itemIndex, 0);
				SendMessage(hPTH, LB_DELETESTRING, itemIndex, 0);
				break;
			case IDC_OPEN1:
			{// dat doan CT xu ly mo file vao day
				OPENFILENAME ofn;		// data structure used for dialog open
				TCHAR szFile[256];
				TCHAR szFilter[] = TEXT("Only wav files (*.wav)\0*.wav\0");
				szFile[0] = '\0';
				ZeroMemory(&ofn, sizeof(OPENFILENAME));
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = hDlg;		// handle of parent window
				ofn.lpstrFilter = szFilter;
				ofn.nFilterIndex = 4;
				ofn.lpstrFile = szFile;		// string to store the returned filename
				ofn.nMaxFile = sizeof(szFile);
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
				if (GetOpenFileName(&ofn))
					SetDlgItemText(hDlg, IDC_EDIT, ofn.lpstrFile);
			}
				break;
			case IDC_OPEN2:
			{// dat doan CT xu ly mo file vao day
				OPENFILENAME ofn;		// data structure used for dialog open
				TCHAR szFile[256];
				TCHAR szFilter[] = TEXT("Only wav files (*.wav)\0*.wav\0");
				szFile[0] = '\0';
				ZeroMemory(&ofn, sizeof(OPENFILENAME));
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = hDlg;		// handle of parent window
				ofn.lpstrFilter = szFilter;
				ofn.nFilterIndex = 4;
				ofn.lpstrFile = szFile;		// string to store the returned filename
				ofn.nMaxFile = sizeof(szFile);
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
				if (GetOpenFileName(&ofn))
					SetDlgItemText(hDlg, IDC_EDITB, ofn.lpstrFile);
			}
				break;
			case IDCANCEL:
				EndDialog(hDlg, LOWORD(wParam));
				break;
		}
	}
	break;
	}
	return (INT_PTR)FALSE;
}