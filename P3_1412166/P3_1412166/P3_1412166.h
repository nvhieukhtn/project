#pragma once
#include "resource.h"

struct Package
{
	bool arranged = false;
	bool visible = false;
	int type = 0;
	COLORREF color = RGB(0, 0, 0);
	POINT pt;
};

struct WareHouse
{
	int type = 0;
	int color = 0;
	int count = 0;
};

void drawPath(HDC hdc);
void eraseOldPackage(Package package);
void drawPackage(Package package);
RECT getWindowsSize();
int generateType();
void drawAllPackages();
void resizeAllPackages();
void updateAllPackages();
void moveAllPackages();
void redrawPathArea();
void notify();
void release();