// Paint_1412166.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Paint_1412166.h"
#include <CommCtrl.h>
#include <stdio.h>
#include <Commdlg.h>
#include <math.h>
#include "Toolbar.h"
#include "StatusBar.h"
#include "Configure.h"
#include "Graphic.h"
#include "File.h"

#define MAX_LOADSTRING 100
#define DEFAULT_COLOR RGB(0,0,0)    //Black
#define DEFAULT_SHAPE 0				//Line
#define DEFAULT_STATUS	false		//Not allow
#define MAX_SHAPE			1000
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hWndToolbar;
HWND hWndStatusbar;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void onCreateHotkey(HWND hWnd);
void onChooseShape(HWND hWnd, int id);
void onChooseColor(HWND hWnd);
void onRadioPressButtonToolbar(int id);

static CurrentConfig config;
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
    LoadStringW(hInstance, IDC_PAINT_1412166, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAINT_1412166));

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



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAINT_1412166));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_APPWORKSPACE+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PAINT_1412166);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
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

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static SHAPE shapes[MAX_SHAPE];
	static int count;
	static int x1, x2, y1, y2;
    switch (message)
    {
	case WM_CREATE:
	{
		config.status = DEFAULT_STATUS;
		config.color = DEFAULT_COLOR;
		config.shape = DEFAULT_SHAPE;
		count = 0;

		onCreateHotkey(hWnd);
		doCreateToolbar(hWnd,hWndToolbar);
		doToolbarShape(hInst,hWndToolbar);
		doCreateStatusBar(hWnd,hWndStatusbar);

		HMENU hMenu = GetMenu(hWnd);
		CheckMenuRadioItem(hMenu, IDM_LINE, IDM_ELLIPSE, IDM_LINE, MF_ENABLED);
		onDisableToolbarButton(hWndToolbar);
	}
	break;
	case WM_HOTKEY:
		if (LOWORD(lParam) == MOD_CONTROL)
		{
			WORD key = HIWORD(lParam);
			switch (key)
			{
			case 0x4F:
				PostMessage(hWnd, WM_COMMAND, IDM_OPEN, 0);
				break;
			case 0x4E:
				PostMessage(hWnd, WM_COMMAND, IDM_NEW, 0);
				break;
			case 0x53:
				PostMessage(hWnd, WM_COMMAND, IDM_SAVE, 0);
				break;
			default:
				break;
			}
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case IDM_NEW:
			{
				count = 0;
				doChangeTextStatus(hWndStatusbar, TEXT("Created new file Noname.mpt!"));
				onChangeClientColor(hWnd);
				config.status = true;
				wsprintf(config.szFile, TEXT("Noname.mpt"));
				SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_SAVE, (LPARAM)TRUE);
				onEnableToolbarButton(hWndToolbar);
				SendMessage(hWnd, WM_COMMAND, IDM_LINE, 0);
			}
			break;
			case IDM_OPEN:
			{
				TCHAR fileName[100];
				wsprintf(fileName,onOpenFileDialog(hWnd));
				if (_tcscmp(fileName, TEXT("cancel")) != 0)
				{
					wsprintf(config.szFile, fileName);
					doChangeTextStatus(hWndStatusbar, TEXT("Opened file!"));
					doLoadFile(config.szFile, shapes, count);
					onChangeClientColor(hWnd);
					onEnableToolbarButton(hWndToolbar);
					onRadioPressButtonToolbar(IDM_LINE);
					onChooseShape(hWnd, IDM_LINE);
					config.color = 0;
					config.status = true;
				}
			}
			break;
			case IDM_SAVE:
			{
				TCHAR fileName[100];
				
				if (_tcscmp(config.szFile, TEXT("Noname.mpt")) == 0)
				{
					wsprintf(fileName, onSaveFileDialog(hWnd));
					if (_tcscmp(fileName, TEXT("cancel")) != 0)
					{
						wsprintf(config.szFile, TEXT("%s.mpt"), fileName);
						doSaveFile(config.szFile, shapes, count);
					}
				}
				else 
					doSaveFile(config.szFile, shapes, count);
			}
			break;
			case IDM_ELLIPSE:
			case IDM_RECTANGE:
			case IDM_LINE:
			{
				onChooseShape(hWnd, wmId);
				onRadioPressButtonToolbar(wmId);
			}
			break;
			case IDM_COLOR:
			{
				onChooseColor(hWnd);
			}
			break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_LBUTTONDOWN:
	{
		if(config.status && count < MAX_SHAPE)
			SetClassLong(hWnd, GCL_HCURSOR, (LONG)LoadCursor(nullptr, IDC_CROSS));
		x2 = x1 = LOWORD(lParam);
		y2 = y1 = HIWORD(lParam);
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (wParam & MK_LBUTTON == MK_LBUTTON && config.status && count < MAX_SHAPE)
		{
			HMENU hMenu = GetMenu(hWnd);
			onDraw(hWnd, x1, y1, x2, y2, R2_NOTXORPEN, config);
			x2 = LOWORD(lParam);
			y2 = HIWORD(lParam);
			onDraw(hWnd, x1, y1, x2, y2, R2_NOTXORPEN, config);
		}
	}
	break;
	case WM_LBUTTONUP:
	{
		if (config.status && count < MAX_SHAPE)
		{
			SetClassLong(hWnd, GCL_HCURSOR, (LONG)LoadCursor(nullptr, IDC_ARROW));
			x2 = LOWORD(lParam);
			y2 = HIWORD(lParam);
			onDraw(hWnd, x1, y1, x2, y2, R2_MASKPEN, config);
			shapes[count].color = config.color;
			shapes[count].shape = config.shape;
			shapes[count].x1 = x1;
			shapes[count].x2 = x2;
			shapes[count].y1 = y1;
			shapes[count].y2 = y2;
			count++;
		}
	}
	break;
	case WM_SIZE:
	{
		SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
		SendMessage(hWndStatusbar, WM_SIZE, 100, 0);
	}
	break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			SetROP2(hdc, R2_MASKPEN);
            // TODO: Add any drawing code that uses hdc here...
			for (int i = 0; i < count; i++)
			{
				HPEN hPen = CreatePen(PS_SOLID, 2, shapes[i].color);
				SelectObject(hdc, hPen);
				switch (shapes[i].shape)
				{
				case 0:
					onDrawLine(hdc, shapes[i].x1, shapes[i].y1, shapes[i].x2, shapes[i].y2);
					break;
				case 1:
					onDrawRectange(hdc, shapes[i].x1, shapes[i].y1, shapes[i].x2, shapes[i].y2);
					break;
				case 2:
					onDrawEllipse(hdc, shapes[i].x1, shapes[i].y1, shapes[i].x2, shapes[i].y2);
					break;
				default:
					break;
				}
			}
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


void onChooseShape(HWND hWnd, int id)
{
	HMENU hMenu = GetMenu(hWnd);
	switch (id)
	{
	case IDM_LINE:
	{
		config.shape = 0;
		CheckMenuRadioItem(hMenu, IDM_LINE, IDM_ELLIPSE, IDM_LINE, MF_ENABLED);
		doChangeTextStatus(hWndStatusbar, TEXT("Draw Line!"));
	}
	break;
	case IDM_RECTANGE:
	{
		config.shape = 1;
		CheckMenuRadioItem(hMenu, IDM_LINE, IDM_ELLIPSE, IDM_RECTANGE, MF_ENABLED);
		doChangeTextStatus(hWndStatusbar, TEXT("Draw Rectange!"));
	}
	break;
	case IDM_ELLIPSE:
	{
		config.shape = 2;
		CheckMenuRadioItem(hMenu, IDM_LINE, IDM_ELLIPSE, IDM_ELLIPSE, MF_ENABLED);
		doChangeTextStatus(hWndStatusbar, TEXT("Draw Ellipse!"));
	}
	break;
	default:
		break;
	}
}

void onCreateHotkey(HWND hWnd)
{
	RegisterHotKey(hWnd, IDM_OPEN, MOD_CONTROL, 0x4F);
	RegisterHotKey(hWnd, IDM_NEW, MOD_CONTROL, 0x4E);
	RegisterHotKey(hWnd, IDM_SAVE, MOD_CONTROL, 0x53);
}

void onChooseColor(HWND hWnd)
{
	CHOOSECOLOR color;
	static COLORREF acrCustClr[16];
	ZeroMemory(&color, sizeof(CHOOSECOLOR));
	color.lStructSize = sizeof(CHOOSECOLOR);
	color.rgbResult = config.color;
	color.hwndOwner = hWnd;
	color.lpCustColors = acrCustClr;
	color.Flags = CC_FULLOPEN | CC_RGBINIT;
	if (ChooseColor(&color) == TRUE)
	{
		config.color = color.rgbResult;
	}
}


void onRadioPressButtonToolbar(int id)
{
	if (id == IDM_LINE)
	{
		SendMessage(hWndToolbar, TB_PRESSBUTTON, IDM_LINE, TRUE);
		SendMessage(hWndToolbar, TB_PRESSBUTTON, IDM_RECTANGE, FALSE);
		SendMessage(hWndToolbar, TB_PRESSBUTTON, IDM_ELLIPSE, FALSE);
	}
	else if(id == IDM_RECTANGE)
	{
		SendMessage(hWndToolbar, TB_PRESSBUTTON, IDM_LINE, FALSE);
		SendMessage(hWndToolbar, TB_PRESSBUTTON, IDM_RECTANGE, TRUE);
		SendMessage(hWndToolbar, TB_PRESSBUTTON, IDM_ELLIPSE, FALSE);
	}
	else
	{
		SendMessage(hWndToolbar, TB_PRESSBUTTON, IDM_LINE, FALSE);
		SendMessage(hWndToolbar, TB_PRESSBUTTON, IDM_RECTANGE, FALSE);
		SendMessage(hWndToolbar, TB_PRESSBUTTON, IDM_ELLIPSE, TRUE);
	}
}