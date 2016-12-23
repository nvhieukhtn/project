#pragma once
TCHAR* onOpenFileDialog(HWND hWnd);
TCHAR* onSaveFileDialog(HWND hWnd);
void doCreateFile(HANDLE &hFile, TCHAR* szFile);
bool doWriteFile(HANDLE hFile, char* buffer);
void doCloseFile(HANDLE &hFile);
void doOpenFile(HANDLE &hFile, TCHAR* szFile);
void doReadFile(HANDLE hFile, char* buffer, int size);
void doSaveFile(TCHAR* szFile, SHAPE* shapes, int count);
void doLoadFile(TCHAR* szFile, SHAPE* shapes, int &count);