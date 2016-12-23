#pragma once
struct CurrentConfig
{
	COLORREF color;
	int shape;
	bool status;
	TCHAR szFile[256];
};

struct SHAPE
{
	int shape;
	int x1;
	int y1;
	int x2;
	int y2;
	COLORREF color;
};