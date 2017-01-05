// P3_1412166.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "P3_1412166.h"
#include "Commctrl.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH_LINE 80
#define BOX_SIZE 70

#define MAX_LOADSTRING 100

#define IDT_TIMER_DRAW	WM_USER + 1
#define IDT_TIMER_MOVING	WM_USER + 2
HWND hWndMain;


// Global Variables:
int numberOfPackages = 5;
int numberOfRemainPackages = 0;
int movingSpeed = 10;
Package *listPackages;
int currentPackage;
bool isRunning = false;
bool isChecking = false;


HANDLE hChecker;

HANDLE hWareHouseThread[3];
HANDLE hObserver;

WareHouse *wareHouse[3];
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name




// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Settings(HWND, UINT, WPARAM, LPARAM);

DWORD WINAPI WareHouseFirstProc(LPVOID lpParameter);
DWORD WINAPI WareHouseSecondProc(LPVOID lpParameter);
DWORD WINAPI WareHouseThirdProc(LPVOID lpParameter);
DWORD WINAPI ObserverProc(LPVOID lpParameter);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_P3_1412166, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_P3_1412166));

    MSG msg;

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



ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_P3_1412166));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_P3_1412166);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
	{
		srand(time(NULL));
		for (int i = 0; i < 3; i++)
		{
			wareHouse[i] = new WareHouse();
			wareHouse[i]->type = i;
			wareHouse[i]->count = 0;
		}
		wareHouse[0]->color = RGB(255, 0, 0);
		wareHouse[1]->color = RGB(0, 0, 255);
		wareHouse[2]->color = RGB(0, 255, 0);
		hWndMain = hWnd;
		hChecker = CreateMutex(NULL, FALSE, NULL);
	}
	break;
	case WM_SIZE:
	{
		resizeAllPackages();
	}
	break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
			case IDM_SETTING:
			{
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_SETTINGS), hWnd, Settings);
			}
			break;
			case IDM_START:
			{	
				HMENU hMenu = GetMenu(hWnd);
				if (!isRunning)
				{
					isRunning = true;
					ModifyMenu(hMenu, IDM_START, MF_BYCOMMAND | MF_STRING, IDM_START, L"Stop");
					DrawMenuBar(hWnd);
					numberOfRemainPackages = numberOfPackages;
					InvalidateRect(hWnd, NULL, TRUE);
					listPackages = new Package[numberOfPackages];
					RECT rectWindows = getWindowsSize();
					for (int i = 0; i < numberOfPackages; i++)
					{
						listPackages[i].type = generateType();
						listPackages[i].pt.y = (rectWindows.bottom - WIDTH_LINE) / 2 + 5;
						listPackages[i].pt.x = 5 - i * (BOX_SIZE + 10);
					}

					listPackages[0].visible = true;
					for (int i = 0; i < 3; i++)
						wareHouse[i]->count = 0;
					ReleaseMutex(hChecker);
					currentPackage = 0;
					SetTimer(hWnd, IDT_TIMER_DRAW, 300, NULL);
					SetTimer(hWnd, IDT_TIMER_MOVING, movingSpeed, NULL);
					hWareHouseThread[0] = CreateThread(NULL, 0, WareHouseFirstProc, wareHouse[0], 0, 0);
					hWareHouseThread[1] = CreateThread(NULL, 0, WareHouseSecondProc, wareHouse[1], 0, 0);
					hWareHouseThread[2] = CreateThread(NULL, 0, WareHouseThirdProc, wareHouse[2], 0, 0);
					hObserver = CreateThread(NULL, 0, ObserverProc, NULL, 0, 0);
				}
				else
					release();
			}
			break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_TIMER:
	{
		switch (wParam)
		{
		case IDT_TIMER_DRAW:
		{
			InvalidateRect(hWnd, NULL, TRUE);
			drawAllPackages();
		}
		break;
		case IDT_TIMER_MOVING:
		{
			moveAllPackages();
			updateAllPackages();
		}
		break;
		}
	}
	break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			drawPath(hdc);
			drawAllPackages();
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
	{
		for (int i = 0; i < 3; i++)
		{
			delete[]wareHouse[i];
			wareHouse[i] = NULL;
		}
		CloseHandle(hChecker);
		PostQuitMessage(0);
	}
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Settings(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		HWND hItem;
		hItem = GetDlgItem(hDlg, IDC_EDIT_NUM_PACKET);
		TCHAR szTemp[10];
		wsprintf(szTemp, TEXT("%d"), numberOfPackages);
		SetWindowText(hItem, szTemp);

		hItem = GetDlgItem(hDlg, IDC_SLIDER_SPEED);
		SendMessage(hItem, TBM_SETPOS, TRUE, 100 - movingSpeed);
		return (INT_PTR)TRUE;
	}
	case WM_HSCROLL:
	{
		HWND hItem;
		hItem = GetDlgItem(hDlg, IDC_SLIDER_SPEED);
		movingSpeed = 100 - SendMessage(hItem, TBM_GETPOS, 0, 0);
		SetTimer(hWndMain, IDT_TIMER_MOVING, movingSpeed, NULL);
	}
	break;
	case WM_COMMAND:
	{
		int id = LOWORD(wParam);
		switch (id)
		{
		case IDOK:
		{
			HWND hItem;
			hItem = GetDlgItem(hDlg, IDC_EDIT_NUM_PACKET);
			TCHAR szTemp[10];
			GetWindowText(hItem, szTemp, sizeof(szTemp));
			numberOfPackages = _ttoi(szTemp);
			
			hItem = GetDlgItem(hDlg, IDC_SLIDER_SPEED);
			movingSpeed = SendMessage(hItem, TBM_GETPOS, 0, 0) / 10;

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
		case IDCANCEL:
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
		default:
			break;
		}
	}
	break;

	}
	return (INT_PTR)FALSE;
}


DWORD WINAPI WareHouseFirstProc(LPVOID lpParameter)
{
	WareHouse* localWareHouse = (WareHouse*)lpParameter;

	RECT rect = getWindowsSize();
	int center = (rect.right - WIDTH_LINE)/2 + 5;
	while (listPackages && numberOfRemainPackages > 0)
	{
		
		WaitForSingleObject(hChecker, INFINITE);
		if (listPackages && listPackages[currentPackage].pt.x > center && !listPackages[currentPackage].arranged)
		{
			isChecking = true;
			listPackages[currentPackage].color = localWareHouse->color;
			Sleep(1000);
			if (listPackages[currentPackage].type == localWareHouse->type)
			{
				localWareHouse->count++;
				
				listPackages[currentPackage].arranged = true;
				currentPackage++;
				numberOfRemainPackages--;
				isChecking = false;
			}
		}
		ReleaseMutex(hChecker);
	}
	ExitThread(0);
	return true;
}


DWORD WINAPI WareHouseSecondProc(LPVOID lpParameter)
{
	WareHouse* localWareHouse = (WareHouse*)lpParameter;
	RECT rect = getWindowsSize();
	int center = (rect.right - WIDTH_LINE)/2 + 5;
	while (listPackages && numberOfRemainPackages > 0)
	{

		WaitForSingleObject(hChecker, INFINITE);
		if (listPackages && listPackages[currentPackage].pt.x > center && !listPackages[currentPackage].arranged)
		{
			isChecking = true;
			listPackages[currentPackage].color = localWareHouse->color;
			Sleep(1000);
			if (listPackages[currentPackage].type == localWareHouse->type)
			{
				localWareHouse->count++;
				
				listPackages[currentPackage].arranged = true;
				currentPackage++;
				numberOfRemainPackages--;
				isChecking = false;
			}
		}
		ReleaseMutex(hChecker);
	}
	ExitThread(0);
	return true;
}

DWORD WINAPI WareHouseThirdProc(LPVOID lpParameter)
{
	WareHouse* localWareHouse = (WareHouse*)lpParameter;
	RECT rect = getWindowsSize();
	int center = (rect.right - WIDTH_LINE)/2 + 5;
	while (listPackages && numberOfRemainPackages > 0)
	{
		WaitForSingleObject(hChecker, INFINITE);
		if (listPackages && listPackages[currentPackage].pt.x > center && !listPackages[currentPackage].arranged)
		{
			isChecking = true;
			listPackages[currentPackage].color = localWareHouse->color;
			Sleep(1000);
			if (listPackages[currentPackage].type == localWareHouse->type)
			{
				localWareHouse->count++;
				listPackages[currentPackage].arranged = true;
				currentPackage++;
				numberOfRemainPackages--;
				isChecking = false;
			}
		}
		ReleaseMutex(hChecker);
	}
	ExitThread(0);
	return true;
}

void notify()
{
	TCHAR content[512];
	wsprintf(content, L"Warehouse 1 : %d\nWarehouse 2 : %d\nWarehouse 3 : %d", wareHouse[0]->count, wareHouse[1]->count, wareHouse[2]->count);
	MessageBox(NULL, content, L"Notification", MB_OK);
}

void release()
{
	TerminateThread(hWareHouseThread[0], 0);
	TerminateThread(hWareHouseThread[1], 0);
	TerminateThread(hWareHouseThread[2], 0);
	TerminateThread(hObserver, 0);
	isRunning = false;
	HMENU hMenu = GetMenu(hWndMain);
	ModifyMenu(hMenu, IDM_START, MF_BYCOMMAND | MF_STRING, IDM_START, L"Start");
	DrawMenuBar(hWndMain);
	delete[] listPackages;
	listPackages = NULL;
	KillTimer(hWndMain, IDT_TIMER_DRAW);
	KillTimer(hWndMain, IDT_TIMER_DRAW);
	//InvalidateRect(hWndMain, NULL, TRUE);
}

DWORD WINAPI ObserverProc(LPVOID lpParameter)
{
	WaitForMultipleObjects(3, hWareHouseThread, true, INFINITE);
	ExitThread(0);
	return 0;
}


RECT getWindowsSize()
{
	RECT rect;
	GetClientRect(hWndMain, &rect);
	return rect;
}

void drawHostInfomation(HDC hdc)
{
	RECT windowsRect = getWindowsSize();

	RECT localRect;
	localRect.top = (windowsRect.bottom - WIDTH_LINE) / 2 - 20;
	localRect.bottom = (windowsRect.bottom - WIDTH_LINE) / 2 - 5;
	localRect.left = 0;
	localRect.right = (windowsRect.right - WIDTH_LINE) / 2 - 5;

	TCHAR remain[512];
	wsprintf(remain, TEXT("Remaining=%d"), numberOfRemainPackages);
	
	DrawText(hdc, remain, wcslen(remain), &localRect, DT_SINGLELINE | DT_BOTTOM);
	localRect.top = localRect.top - 15;
	localRect.bottom = localRect.bottom - 15;
	TCHAR total[512];
	wsprintf(total, TEXT("Total=%d"), numberOfPackages);
	DrawText(hdc, total, wcslen(total), &localRect, DT_SINGLELINE | DT_BOTTOM);
}

void drawWareHouseFirstInfomation(HDC hdc)
{
	SetTextColor(hdc, wareHouse[0]->color);
	RECT windowsRect = getWindowsSize();

	RECT localRect;
	localRect.top = 0;
	localRect.bottom = (windowsRect.bottom - WIDTH_LINE) / 2 - 5;
	localRect.left = (windowsRect.right + WIDTH_LINE) / 2 + 5;
	localRect.right = windowsRect.right;

	TCHAR count[512];
	wsprintf(count, TEXT("(Count=%d)"), wareHouse[0]->count);
	DrawText(hdc, count, wcslen(count), &localRect, DT_SINGLELINE | DT_TOP);
	localRect.top = localRect.top + 15;
	localRect.bottom = localRect.bottom + 15;
	TCHAR lable[512] = TEXT("LINE 1");
	DrawText(hdc, lable, wcslen(lable), &localRect, DT_SINGLELINE | DT_TOP);
}

void drawWareHouseSecondInfomation(HDC hdc)
{
	SetTextColor(hdc, wareHouse[1]->color);
	RECT windowsRect = getWindowsSize();

	RECT localRect;
	localRect.top = (windowsRect.bottom + WIDTH_LINE) / 2 + 5;
	localRect.bottom = (windowsRect.bottom + WIDTH_LINE) / 2 + 20;
	localRect.left = windowsRect.right - 80;
	localRect.right = windowsRect.right;

	TCHAR count[512];
	wsprintf(count, TEXT("(Count=%d)"), wareHouse[1]->count);
	DrawText(hdc, count, wcslen(count), &localRect, DT_SINGLELINE | DT_TOP);
	localRect.top = localRect.top + 15;
	localRect.bottom = localRect.bottom + 15;
	TCHAR lable[512] = L"LINE 2";
	DrawText(hdc, lable, wcslen(lable), &localRect, DT_SINGLELINE | DT_TOP);
}

void drawWareHouseThirdInfomation(HDC hdc)
{
	SetTextColor(hdc, wareHouse[2]->color);
	RECT windowsRect = getWindowsSize();

	RECT localRect;
	localRect.top = windowsRect.bottom - 30;
	localRect.bottom = windowsRect.bottom - 15;
	localRect.left = (windowsRect.right + WIDTH_LINE) / 2 + 5;
	localRect.right = windowsRect.right;

	TCHAR count[512];
	wsprintf(count,L"(Count=%d)",wareHouse[2]->count);
	DrawText(hdc, count, wcslen(count), &localRect, DT_SINGLELINE | DT_TOP);
	localRect.top = localRect.top + 15;
	localRect.bottom = localRect.bottom + 15;
	TCHAR lable[512] = L"LINE 3";
	DrawText(hdc, lable, wcslen(lable), &localRect, DT_SINGLELINE | DT_TOP);
}

void drawPackage(Package package)
{
	TCHAR typeStr[512];
	wsprintf(typeStr, L"%d", package.type + 1);
	HDC hdc = GetDC(hWndMain);
	HPEN hPen = CreatePen(PS_SOLID, 2, package.color);
	SelectObject(hdc, hPen);

	RECT rect;
	rect.left = package.pt.x;
	rect.top = package.pt.y;
	rect.right = package.pt.x + BOX_SIZE;
	rect.bottom = package.pt.y + BOX_SIZE; 

	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	SetTextColor(hdc, package.color);
	DrawText(hdc, typeStr, wcslen(typeStr), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}


void eraseOldPackage(Package package)
{
	RECT RefreshRect;
	RefreshRect.left = package.pt.x - 5;
	RefreshRect.top = package.pt.y;
	RefreshRect.right = RefreshRect.left + BOX_SIZE + 10;
	RefreshRect.bottom = RefreshRect.top + BOX_SIZE;
}
void drawPath(HDC hdc)
{
	HPEN hPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
	SelectObject(hdc, hPen);
	RECT rect = getWindowsSize();
	//left
	MoveToEx(hdc, 0, (rect.bottom - WIDTH_LINE) / 2, NULL);
	LineTo(hdc, (rect.right - WIDTH_LINE) / 2, (rect.bottom - WIDTH_LINE) / 2);

	MoveToEx(hdc, 0, (rect.bottom + WIDTH_LINE) / 2, NULL);
	LineTo(hdc, (rect.right - WIDTH_LINE) / 2, (rect.bottom + WIDTH_LINE) / 2);
	//right
	MoveToEx(hdc, rect.right, (rect.bottom - WIDTH_LINE) / 2, NULL);
	LineTo(hdc, (rect.right + WIDTH_LINE) / 2, (rect.bottom - WIDTH_LINE) / 2);

	MoveToEx(hdc, rect.right, (rect.bottom + WIDTH_LINE) / 2, NULL);
	LineTo(hdc, (rect.right + WIDTH_LINE) / 2, (rect.bottom + WIDTH_LINE) / 2);
	//top
	MoveToEx(hdc, (rect.right - WIDTH_LINE) / 2, 0, NULL);
	LineTo(hdc, (rect.right - WIDTH_LINE) / 2, (rect.bottom - WIDTH_LINE) / 2);

	MoveToEx(hdc, (rect.right + WIDTH_LINE) / 2, 0, NULL);
	LineTo(hdc, (rect.right + WIDTH_LINE) / 2, (rect.bottom - WIDTH_LINE) / 2);
	//bottom
	MoveToEx(hdc, (rect.right - WIDTH_LINE) / 2, rect.bottom, NULL);
	LineTo(hdc, (rect.right - WIDTH_LINE) / 2, (rect.bottom + WIDTH_LINE) / 2);

	MoveToEx(hdc, (rect.right + WIDTH_LINE) / 2, rect.bottom, NULL);
	LineTo(hdc, (rect.right + WIDTH_LINE) / 2, (rect.bottom + WIDTH_LINE) / 2);

	drawHostInfomation(hdc);
	drawWareHouseFirstInfomation(hdc);
	drawWareHouseSecondInfomation(hdc);
	drawWareHouseThirdInfomation(hdc);
}

int generateType()
{
	int type = rand() % 3;
	return type;
}

void drawAllPackages() 
{
	RECT rect = getWindowsSize();
	for (int i = 0; i < numberOfPackages; i++)
		if (listPackages != NULL && listPackages[i].visible)
		{
			drawPackage(listPackages[i]);
		}
}

void resizeAllPackages()
{
	RECT rectWindows = getWindowsSize();
	for (int i = 0; i < numberOfPackages; i++)
		if(listPackages != NULL)
		{
			listPackages[i].pt.y = (rectWindows.bottom - WIDTH_LINE) / 2 + 5;
			listPackages[i].pt.x = 5;
		}
}

void updateAllPackages()
{
	if (!listPackages)
		return;
	RECT rectWindows = getWindowsSize();
	isRunning = false;
	for (int i = 0; i < numberOfPackages; i++)
		if (listPackages[i].pt.x >= 0 && listPackages[i].pt.x <= rectWindows.right + BOX_SIZE &&
			listPackages[i].pt.y >= -10 - BOX_SIZE && listPackages[i].pt.y <= rectWindows.bottom + BOX_SIZE)
		{
			listPackages[i].visible = true;
			isRunning = true;
		}
	if (!isRunning)
	{
		release();
		notify();
	}
}

void moveAllPackages()
{
	if (!listPackages)
		return;
	for (int i = 0; i < numberOfPackages; i++)
	{
		if (isChecking && i >= currentPackage)
			continue;
		if (!listPackages[i].arranged || listPackages[i].type == 1)
			listPackages[i].pt.x++;
		else if(listPackages[i].type == 0)
		{
			listPackages[i].pt.y--;
		}
		else if (listPackages[i].type == 2)
		{
			listPackages[i].pt.y++;
		}
	}
}

void redrawPathArea()
{
	RECT windowRect = getWindowsSize();
	RECT rectCenter;

	rectCenter.left = 0;
	rectCenter.right = windowRect.right;
	rectCenter.top = (windowRect.bottom - WIDTH_LINE) / 2 + 2;
	rectCenter.bottom = rectCenter.top + BOX_SIZE + 6;

	InvalidateRect(hWndMain, &rectCenter, TRUE);
}