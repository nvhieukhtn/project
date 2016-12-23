#pragma once
void onChangeClientColor(HWND &hWnd);
void onDraw(HWND &hWnd, int x1, int y1, int x2, int y2, int stylePen, CurrentConfig &config);
void onDrawLine(HDC &hdc, int x1, int y1, int x2, int y2);
void onDrawRectange(HDC &hdc, int x1, int y1, int x2, int y2);
void onDrawEllipse(HDC &hdc, int x1, int y1, int x2, int y2);