// H15_1412166.cpp : Defines the entry polong long for the application.
//

#include "stdafx.h"
#include "H15_1412166.h"
#include "Shlwapi.h";
#include <fstream>
#include <Commctrl.h>
#include <commdlg.h>
#include <ShlObj.h>

#define MAX_LOADSTRING 100

struct CopySetting
{
	HWND hDlg;
	long long progressId;
	long long percentId;
	HANDLE hReader;
	HANDLE hWriter;
	long long start;
	long long end;
};

struct WaitSetting
{
	HWND hDlg;
};
CopySetting *setting[5];
// Global Variables:
HINSTANCE hInst;                                // current instance

TCHAR* getNameOfFile(TCHAR *path);
INT_PTR CALLBACK    ThreadUIProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI CopyProc(LPVOID lpParameter);
void onResetProgress(HWND hDlg);
void onCopy(HWND hDlg);
TCHAR* onLoadFile(HWND hWnd);
TCHAR* onLoadDirectory(HWND hWnd);
HANDLE hThread[5];
HANDLE hFileWrite, hFileRead;
HANDLE hReader, hWriter;
int numberOfThreads = 0;
TCHAR *sourceFile = new TCHAR[MAX_PATH];
TCHAR *destFolder = new TCHAR[MAX_PATH];

void setPercentOfProgress(HWND hDlg, long long progressId, long long percent)
{
	HWND hProgress = GetDlgItem(hDlg, progressId);
	PostMessage(hProgress, PBM_SETPOS, percent, 0);
}


bool isFile(TCHAR *filename)
{
	DWORD dwAttrib = GetFileAttributes(filename);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_ARCHIVE));
}
bool isFolder(TCHAR *filename) 
{
	DWORD dwAttrib = GetFileAttributes(filename);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

long long getNumberOfThreads(HWND hDlg) 
{
	HWND hCb = GetDlgItem(hDlg, IDC_COMBO_THREADS);
	long long index = SendMessage(hCb, (UINT)CB_GETCURSEL,
		(WPARAM)0, (LPARAM)0);
	return index + 1;
}

void initCombobox(HWND hDlg) 
{
	HWND hCb = GetDlgItem(hDlg, IDC_COMBO_THREADS);
	TCHAR NumberOfThreads[9][10] =
	{
		TEXT("1"), TEXT("2"), TEXT("3"), TEXT("4"),
		TEXT("5")
	};

	TCHAR A[16];
	long long  k = 0;

	memset(&A, 0, sizeof(A));
	for (k = 0; k <= 5; k += 1)
	{
		wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)NumberOfThreads[k]);

		// Add string to combobox.
		SendMessage(hCb, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
	}

	// Send the CB_SETCURSEL message to display an initial item 
	//  in the selection field  
	SendMessage(hCb, (UINT)CB_DELETESTRING, (WPARAM)0, (LPARAM)0);
	SendMessage(hCb, CB_SETCURSEL, (WPARAM)2, (LPARAM)0);

	HWND hWndEditControl = GetDlgItem(hDlg, IDC_STATIC_FILE_SIZE);
	SetWindowText(hWndEditControl, L"Unknown");
	hWndEditControl = GetDlgItem(hDlg, IDC_STATIC_STATUS);
	SetWindowText(hWndEditControl, L"Choose file and folder to copy"); 
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Perform application initialization:
	if (!DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_UI), NULL, ThreadUIProc))
	{
		return FALSE;
	}
	hInst = hInstance;
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_H15_1412166));

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
	return (int)msg.wParam;
}

INT_PTR CALLBACK ThreadUIProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		onResetProgress(hDlg);
		wsprintf(sourceFile, L"");
		wsprintf(destFolder, L"");
		initCombobox(hDlg);
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
	{
		int id = LOWORD(wParam);
		switch (id)
		{
		case IDC_BUTTON_GET_SOURCEFILE:
		{
			TCHAR *result = onLoadFile(hDlg);
			if (result)
				wsprintf(sourceFile, L"%s", result);				
			SetDlgItemText(hDlg, IDC_EDIT_SOURCE_FILE, sourceFile);
			HANDLE hFile = CreateFile(sourceFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (wcslen(sourceFile))
			{
				long long fileSize = GetFileSize(hFile, NULL);
				fileSize = fileSize / (1024);
				HWND hWndSizeFile = GetDlgItem(hDlg, IDC_STATIC_FILE_SIZE);
				TCHAR szSizeFile[512];
				wsprintf(szSizeFile, L"%d KB", fileSize);
				SetWindowText(hWndSizeFile, szSizeFile);
			}
			else {
				HWND hWndSizeFile = GetDlgItem(hDlg, IDC_STATIC_FILE_SIZE);
				TCHAR szSizeFile[512];
				wsprintf(szSizeFile, L"Unknown");
				SetWindowText(hWndSizeFile, szSizeFile);
			}
			HWND hWndEditControl = GetDlgItem(hDlg, IDC_STATIC_STATUS);
			if (wcslen(sourceFile) && wcslen(destFolder))
				SetWindowText(hWndEditControl, L"Ready!");
			else
				SetWindowText(hWndEditControl, L"Choose file and folder to copy!");
		}
		break;
		case IDC_BUTTON_GET_DESTFOLDER:
		{
			TCHAR *result = onLoadDirectory(hDlg);
			if (result)
				wsprintf(destFolder, L"%s", result);
			SetDlgItemText(hDlg, IDC_EDIT_DEST_FOLDER, destFolder);
			HWND hWndEditControl = GetDlgItem(hDlg, IDC_STATIC_STATUS);
			if (wcslen(sourceFile) && wcslen(destFolder))
				SetWindowText(hWndEditControl, L"Ready!");
			else
				SetWindowText(hWndEditControl, L"Choose file and folder to copy!");
		}
		break;
		case IDC_BUTTON_COPY:
		{
			onResetProgress(hDlg);
			HWND hWndEditControl = GetDlgItem(hDlg, IDC_STATIC_STATUS);
			SetWindowText(hWndEditControl, L"Copying!");
			onCopy(hDlg);
		}
		break;
		default:
			break;
		}
	}
	break;
	case WM_CLOSE:
	{
		delete[]sourceFile;
		delete[]destFolder;
		for (int i = 0; i < 5; i++)
			if (setting[i] != NULL)
				delete[]setting[i];
		PostQuitMessage(0);
	}
		break;
	}
	return (INT_PTR)FALSE;
}


DWORD WINAPI CopyProc(LPVOID lpParameter)
{
	CopySetting* st = (CopySetting*)lpParameter;
	BYTE   buff[409600];
	int sizeOfBuff = sizeof(buff);
	DWORD  dwBytesRead, dwBytesWritten, dwPos = st->start;
	int nReader = (sizeOfBuff < st->end - dwPos) ? sizeOfBuff : (st->end - dwPos);
	HWND hWndPercent;
	TCHAR percentString[512];
	hWndPercent = GetDlgItem(st->hDlg, st->percentId);
	while (dwPos <= st->end)
	{
		WaitForSingleObject(hReader, INFINITE);
		SetFilePointer(st->hReader, dwPos, NULL, FILE_BEGIN);
		ReadFile(st->hReader, buff, nReader, &dwBytesRead, NULL);
		ReleaseMutex(hReader);
		WaitForSingleObject(hWriter, INFINITE);
		SetFilePointer(st->hWriter, dwPos, NULL, FILE_BEGIN);
		LockFile(st->hWriter, dwPos, 0, dwBytesRead, 0);
		WriteFile(st->hWriter, buff, dwBytesRead, &dwBytesWritten, NULL);
		UnlockFile(st->hWriter, dwPos, 0, dwBytesRead, 0);
		ReleaseMutex(hWriter);
		dwPos += dwBytesRead;
		nReader = (sizeOfBuff < st->end - dwPos) ? sizeOfBuff : (st->end - dwPos);
		if (nReader == 0)
			break;
		int percent = (int)(((dwPos - st->start) * 100) / (st->end - st->start));
		setPercentOfProgress(st->hDlg, st->progressId, percent);

		wsprintf(percentString, L"%d %%", percent);
		SetWindowText(hWndPercent, percentString);
	}
	setPercentOfProgress(st->hDlg, st->progressId, 100);
	wsprintf(percentString, L"100 %%");
	SetWindowText(hWndPercent, percentString);
	ExitThread(0);
	return 0;
}


TCHAR* getNameOfFile(TCHAR *path)
{
	TCHAR filepath[512];
	wcscpy_s(filepath, path);
	TCHAR filename[512];
	TCHAR *sub;
	sub = wcstok(filepath, L"\\");
	while (sub != NULL)
	{
		wcscpy_s(filename, sub);
		sub = wcstok(NULL, L"\\");
	}
	return filename;
}

TCHAR* onLoadFile(HWND hWnd)
{
	TCHAR szFile[512];
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All files\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	GetOpenFileName(&ofn);
	
	return szFile;
}


TCHAR* onLoadDirectory(HWND hWnd)
{
	TCHAR szDir[MAX_PATH];
	BROWSEINFO bInfo;
	bInfo.hwndOwner = NULL;
	bInfo.pidlRoot = NULL;
	bInfo.pszDisplayName = szDir; // Address of a buffer to receive the display name of the folder selected by the user
	bInfo.lpszTitle = L"Please, select a folder"; // Title of the dialog
	bInfo.ulFlags = 0;
	bInfo.lpfn = NULL;
	bInfo.lParam = 0;
	bInfo.iImage = -1;

	LPITEMIDLIST lpItem = SHBrowseForFolder(&bInfo);
	if (lpItem != NULL)
	{
		SHGetPathFromIDList(lpItem, szDir);
		wcscat_s(szDir, L"\\");
		return szDir;
	}
	return L"";
}


DWORD WINAPI WaitProc(LPVOID lpParameter)
{
	WaitSetting *wst = (WaitSetting*)lpParameter;
	WaitForMultipleObjects(numberOfThreads, hThread, true, INFINITE);
	HWND hWndEditControl = GetDlgItem(wst->hDlg, IDC_STATIC_STATUS);
	SetWindowText(hWndEditControl, L"Finished!");
	CloseHandle(hFileWrite);
	CloseHandle(hFileRead);
	ExitThread(0);
	return true;
}


void onCopy(HWND hDlg)
{
	numberOfThreads = getNumberOfThreads(hDlg);

	TCHAR* filename = getNameOfFile(sourceFile);
	TCHAR destFolderCopy[MAX_PATH];
	wsprintf(destFolderCopy, L"%s", destFolder);
	TCHAR* destFile = wcscat(destFolderCopy, filename);

	hFileWrite = CreateFile(destFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	hFileRead = CreateFile(sourceFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	hReader = CreateMutex(NULL, FALSE, NULL);
	hWriter = CreateMutex(NULL, FALSE, NULL);
	long long sizefile = GetFileSize(hFileRead, NULL);
	for (long long i = 0; i < numberOfThreads; i++)
	{
		setting[i] = new CopySetting();
		setting[i]->hDlg = hDlg;
		setting[i]->hReader = hFileRead;
		setting[i]->hWriter = hFileWrite;
		setting[i]->start = sizefile * i / numberOfThreads;
		setting[i]->end = sizefile * (i + 1) / numberOfThreads;
		if (i == numberOfThreads - 1)
			setting[i]->end = sizefile;
		if (i == 0)
		{
			setting[i]->progressId = IDC_PROGRESS1;
			setting[i]->percentId = IDC_STATIC_PERCENT1;
		}
		if (i == 1)
		{
			setting[i]->progressId = IDC_PROGRESS2;
			setting[i]->percentId = IDC_STATIC_PERCENT2;
		}
		if (i == 2)
		{
			setting[i]->progressId = IDC_PROGRESS3;
			setting[i]->percentId = IDC_STATIC_PERCENT3;
		}
		if (i == 3)
		{
			setting[i]->progressId = IDC_PROGRESS4;
			setting[i]->percentId = IDC_STATIC_PERCENT4;
		}
		if (i == 4)
		{
			setting[i]->progressId = IDC_PROGRESS5;
			setting[i]->percentId = IDC_STATIC_PERCENT5;
		}

		hThread[i] = CreateThread(NULL, 0, CopyProc, setting[i], 0, 0);
	}

	WaitSetting *waitSetting = new WaitSetting();
	waitSetting->hDlg = hDlg;
	CreateThread(NULL, 0, WaitProc, waitSetting, 0, 0);
}

void onResetProgress(HWND hDlg)
{
	setPercentOfProgress(hDlg, IDC_PROGRESS1, 0);
	setPercentOfProgress(hDlg, IDC_PROGRESS2, 0);
	setPercentOfProgress(hDlg, IDC_PROGRESS3, 0);
	setPercentOfProgress(hDlg, IDC_PROGRESS4, 0);
	setPercentOfProgress(hDlg, IDC_PROGRESS5, 0);

	TCHAR percent[512] = L"0 %";
	HWND hWndPercent;
	hWndPercent = GetDlgItem(hDlg, IDC_STATIC_PERCENT1);
	SetWindowText(hWndPercent, percent);

	hWndPercent = GetDlgItem(hDlg, IDC_STATIC_PERCENT2);
	SetWindowText(hWndPercent, percent);

	hWndPercent = GetDlgItem(hDlg, IDC_STATIC_PERCENT3);
	SetWindowText(hWndPercent, percent);

	hWndPercent = GetDlgItem(hDlg, IDC_STATIC_PERCENT4);
	SetWindowText(hWndPercent, percent);

	hWndPercent = GetDlgItem(hDlg, IDC_STATIC_PERCENT5);
	SetWindowText(hWndPercent, percent);
}