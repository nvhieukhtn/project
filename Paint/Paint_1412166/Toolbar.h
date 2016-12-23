#pragma once
void doCreateToolbar(HWND hWnd, HWND &hWndToolbar);
void doToolBar_NotifyHandle(LPARAM	lParam, HINSTANCE hInst);
void doToolBar_Style(int tbStyle, HWND &hWndToolbar);
void doToolbarShape(HINSTANCE hInst, HWND &hWndToolbar);
void onDisableToolbarButton(HWND &hWndToolbar);
void onEnableToolbarButton(HWND &hWndToolbar);