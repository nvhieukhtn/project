#include "stdafx.h"
#include   <windows.h>		
#include   <CommCtrl.h>
#include <commdlg.h>
#include   "resource.h"
#include "Configure.h"
#include <cstdio>
#include <stdio.h>
DWORD g_BytesTransferred = 0;

VOID CALLBACK FileIOCompletionRoutine(
	__in  DWORD dwErrorCode,
	__in  DWORD dwNumberOfBytesTransfered,
	__in  LPOVERLAPPED lpOverlapped
);

VOID CALLBACK FileIOCompletionRoutine(
	__in  DWORD dwErrorCode,
	__in  DWORD dwNumberOfBytesTransfered,
	__in  LPOVERLAPPED lpOverlapped)
{
	g_BytesTransferred = dwNumberOfBytesTransfered;
}

TCHAR* onOpenFileDialog(HWND hWnd)
{
	OPENFILENAME ofn;       // common dialog box structure
	TCHAR szFile[260];       // buffer for file name
	HANDLE hf;              // file handle

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = TEXT("MyPaint\0*.MPT\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn) == TRUE)
		return szFile;
	return TEXT("cancel");
}

TCHAR* onSaveFileDialog(HWND hWnd)
{
	OPENFILENAME ofn;       // common dialog box structure
	TCHAR szFile[260];       // buffer for file name
	HANDLE hf;              // file handle

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = TEXT("MyPaint\0*.MPT\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetSaveFileName(&ofn) == TRUE)
		return szFile;
	return TEXT("cancel");
}

void doCreateFile(HANDLE &hFile,TCHAR* szFile)
{
	hFile = CreateFile(szFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}
bool doWriteFile(HANDLE hFile, char* buffer)
{
	BOOL bError = FALSE;
	DWORD dwBytesWritten = 0;
	bError = WriteFile(hFile, buffer, strlen(buffer), &dwBytesWritten, NULL);
	if (bError == FALSE || dwBytesWritten != strlen(buffer))
		return false;
	return true;
}
void doCloseFile(HANDLE &hFile)
{
	CloseHandle(hFile);
}

void doOpenFile(HANDLE &hFile, TCHAR* szFile)
{
	hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
}

void doReadFile(HANDLE hFile, char* buffer, int size)
{
	OVERLAPPED ol = { 0 };
	if (FALSE == ReadFileEx(hFile, buffer, size - 1, &ol, FileIOCompletionRoutine))
		CloseHandle(hFile);
	SleepEx(5000, TRUE);
	if(g_BytesTransferred != 0)
		buffer[g_BytesTransferred] = '\0';
}

void toData(SHAPE* shapes, int index,char* info)
{
	char* line = new char[20];
	line = strtok(NULL, "\n");
	if (line != NULL)
		toData(shapes, index + 1, line);
	char* value = new char[20];
	value = strtok(info, " ");
	shapes[index].x1 = atoi(value);
	value = strtok(NULL, " ");
	shapes[index].y1 = atoi(value);
	value = strtok(NULL, " ");
	shapes[index].x2 = atoi(value);
	value = strtok(NULL, " ");
	shapes[index].y2 = atoi(value);
	value = strtok(NULL, " ");
	shapes[index].shape = atoi(value);
	value = strtok(NULL, " ");
	shapes[index].color = atoi(value);
}

void doLoadFile(TCHAR* szFile, SHAPE* shapes, int &count)
{
	HANDLE hFile;
	doOpenFile(hFile, szFile);
	char buffer[100000];
	doReadFile(hFile, buffer, 100000);
	char* line = new char[50];
	line = strtok(buffer, "\n");
	count = atoi(line);
	line = strtok(NULL, "\n");
	if(line != NULL)
		toData(shapes, 0,line);
	doCloseFile(hFile);
}

void doSaveFile(TCHAR* szFile, SHAPE* shapes,int count)
{
	HANDLE hFile;
	doCreateFile(hFile, szFile);
	char countStr[5];
	sprintf(countStr, "%d\n", count);
	doWriteFile(hFile, countStr);
	for (int i = 0; i < count; i++)
	{
		char shapeInfo[100];
		sprintf(shapeInfo, "%d %d %d %d %d %d\n", shapes[i].x1, shapes[i].y1, shapes[i].x2, shapes[i].y2, shapes[i].shape, shapes[i].color);
		doWriteFile(hFile, shapeInfo);
	}
	doCloseFile(hFile);
}