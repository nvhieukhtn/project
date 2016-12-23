#include "stdafx.h"
#include   <windows.h>		
#include   <CommCtrl.h>
#include   "resource.h"
#include "Configure.h"

void onChangeClientColor(HWND &hWnd)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	SetClassLong(hWnd, GCLP_HBRBACKGROUND, (LONG)hBrush);
	InvalidateRect(hWnd, &rect, TRUE);
}

void onDrawLine(HDC &hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

void onDrawRectange(HDC &hdc, int x1, int y1, int x2, int y2)
{
	Rectangle(hdc, x1, y1, x2, y2);
}

void onDrawEllipse(HDC &hdc, int x1, int y1, int x2, int y2)
{
	Ellipse(hdc, x1, y1, x2, y2);
}

void onDraw(HWND &hWnd, int x1, int y1, int x2, int y2, int stylePen, CurrentConfig &config)
{
	if (config.status)
	{
		HDC hdc = GetDC(hWnd);
		COLORREF color = config.color;
		HPEN hPen = CreatePen(PS_SOLID, 2, color);
		SelectObject(hdc, hPen);
		SetROP2(hdc, stylePen);
		switch (config.shape)
		{
		case 0:
			onDrawLine(hdc, x1, y1, x2, y2);
			break;
		case 1:
			onDrawRectange(hdc, x1, y1, x2, y2);
			break;
		case 2:
			onDrawEllipse(hdc, x1, y1, x2, y2);
			break;
		default:
			break;
		}
	}
}
