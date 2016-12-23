// AKeysHook.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <Windowsx.h>
#include <map>
using namespace std;

#define EXPORT  __declspec(dllexport)

HHOOK hHook = NULL;
HINSTANCE hinstLib;

typedef VOID(*MyAudio)(wchar_t *);
typedef VOID(*MyAudio2)(HWND, wchar_t *);
typedef VOID(*MyAudio3)(HWND);
typedef VOID(*MyAudio4)();
LRESULT CALLBACK AKeyHookProc(int nCode, WPARAM wParam, LPARAM lParam);
map<int, wstring> setting;
bool checkB = false;

LRESULT CALLBACK AKeyHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0 || setting.size() == 0) // không xử lý message 
		return CallNextHookEx(hHook, nCode, wParam, lParam);
	HINSTANCE hLib;
	hLib = LoadLibrary(L"Audio.dll");
	if (hLib)
	{
		MyAudio playBackground = (MyAudio)GetProcAddress(hLib, "PlayAudio");
		MyAudio2 playAKeySound = (MyAudio2)GetProcAddress(hLib, "doMCICommandStringPlay");
		MyAudio3 stopAKeySound = (MyAudio3)GetProcAddress(hLib, "doMCICommandStringStop");
		MyAudio4 stopBackground = (MyAudio4)GetProcAddress(hLib, "StopAudio");
		if (playBackground && playAKeySound && stopAKeySound && stopBackground)
		{
			if ((nCode == HC_ACTION) && ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN)))
			{
				KBDLLHOOKSTRUCT hooked_key = *((KBDLLHOOKSTRUCT*)lParam);
				int key = hooked_key.vkCode;
				switch (key)
				{
					//......................ALPHABET KEYS CASES......................//
				case 'A':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['A'].c_str()));
					break;
				case 'B':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['B'].c_str()));
					break;
				case 'C':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['C'].c_str()));
					break;
				case 'D':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['D'].c_str()));
					break;
				case 'E':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['E'].c_str()));
					break;
				case 'F':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['F'].c_str()));
					break;
				case 'G':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['G'].c_str()));
					break;
				case 'H':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['H'].c_str()));
					break;
				case 'I':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['I'].c_str()));
					break;
				case 'J':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['J'].c_str()));
					break;
				case 'K':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['K'].c_str()));
					break;
				case 'L':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['L'].c_str()));
					break;
				case 'M':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['M'].c_str()));
					break;
				case 'N':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['N'].c_str()));
					break;
				case 'O':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['O'].c_str()));
					break;
				case 'P':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['P'].c_str()));
					break;
				case 'Q':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['Q'].c_str()));
					break;
				case 'R':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['R'].c_str()));
					break;
				case 'S':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['S'].c_str()));
					break;
				case 'T':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['T'].c_str()));
					break;
				case 'U':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['U'].c_str()));
					break;
				case 'V':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['V'].c_str()));
					break;
				case 'W':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['W'].c_str()));
					break;
				case 'X':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['X'].c_str()));
					break;
				case 'Y':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['Y'].c_str()));
					break;
				case 'Z':
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting['Z'].c_str()));
					break;
					//......................NUMBER KEYS CASES......................//
				case 0x30:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[0x30].c_str()));
					break;
				case 0x31:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[0x31].c_str()));
					break;
				case 0x32:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[0x32].c_str()));
					break;
				case 0x33:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[0x33].c_str()));
					break;
				case 0x34:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[0x34].c_str()));
					break;
				case 0x35:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[0x35].c_str()));
					break;
				case 0x36:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[0x36].c_str()));
					break;
				case 0x37:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[0x37].c_str()));
					break;
				case 0x38:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[0x38].c_str()));
					break;
				case 0x39:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[0x39].c_str()));
					break;
				case VK_NUMPAD0:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_NUMPAD0].c_str()));
					break;
				case VK_NUMPAD1:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_NUMPAD1].c_str()));
					break;
				case VK_NUMPAD2:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_NUMPAD2].c_str()));
					break;
				case VK_NUMPAD3:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_NUMPAD3].c_str()));
					break;
				case VK_NUMPAD4:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_NUMPAD4].c_str()));
					break;
				case VK_NUMPAD5:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_NUMPAD5].c_str()));
					break;
				case VK_NUMPAD6:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_NUMPAD6].c_str()));
					break;
				case VK_NUMPAD7:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_NUMPAD7].c_str()));
					break;
				case VK_NUMPAD8:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_NUMPAD8].c_str()));
					break;
				case VK_NUMPAD9:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_NUMPAD9].c_str()));
					break;
				case VK_F1:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_F1].c_str()));
					break;
				case VK_F2:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_F2].c_str()));
					break;
				case VK_F3:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_F3].c_str()));
					break;
				case VK_F4:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_F4].c_str()));
					break;
				case VK_F5:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_F5].c_str()));
					break;
				case VK_F6:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_F6].c_str()));
					break;
				case VK_F7:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_F7].c_str()));
					break;
				case VK_F8:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_F8].c_str()));
					break;
				case VK_F9:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_F9].c_str()));
					break;
				case VK_F10:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_F10].c_str()));
					break;
				case VK_F11:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_F11].c_str()));
					break;
				case VK_F12:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_F12].c_str()));
					break;
					//......................OPERATOR KEYS CASES......................//
				case VK_MULTIPLY:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_MULTIPLY].c_str()));
					break;
				case VK_ADD:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_ADD].c_str()));
					break;
				case VK_SEPARATOR:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_SEPARATOR].c_str()));
					break;
				case VK_SUBTRACT:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_SUBTRACT].c_str()));
					break;
				case VK_DECIMAL:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_DECIMAL].c_str()));
					break;
				case VK_DIVIDE:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_DIVIDE].c_str()));
					break;
					//......................SYSTEM KEYS CASES......................//
				case VK_BACK:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_BACK].c_str()));
					break;
				case VK_TAB:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_TAB].c_str()));
					break;
				case VK_RETURN:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_RETURN].c_str()));
					break;
				case VK_CLEAR:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_CLEAR].c_str()));
					break;
				case VK_SHIFT:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_SHIFT].c_str()));
					break;
				case VK_CONTROL:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_CONTROL].c_str()));
					break;
				case VK_MENU:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_MENU].c_str()));
					break;
				case VK_PAUSE:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_PAUSE].c_str()));
					break;
				case VK_CAPITAL:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_CAPITAL].c_str()));
					break;
				case VK_ESCAPE:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_ESCAPE].c_str()));
					break;
				case VK_SPACE:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_SPACE].c_str()));
					break;
				case VK_PRIOR:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_PRIOR].c_str()));
					break;
				case VK_NEXT:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_NEXT].c_str()));
					break;
				case VK_END:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_END].c_str()));
					break;
				case VK_HOME:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_HOME].c_str()));
					break;
				case VK_LEFT:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_LEFT].c_str()));
					break;
				case VK_UP:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_UP].c_str()));
					break;
				case VK_RIGHT:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_RIGHT].c_str()));
					break;
				case VK_DOWN:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_DOWN].c_str()));
					break;
				case VK_SELECT:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_SELECT].c_str()));
					break;
				case VK_PRINT:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_PRINT].c_str()));
					break;
				case VK_EXECUTE:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_EXECUTE].c_str()));
					break;
				case VK_SNAPSHOT:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_SNAPSHOT].c_str()));
					break;
				case VK_INSERT:
					if (checkB)
						playBackground(const_cast<wchar_t*>(setting[VK_INSERT].c_str()));
					else
					{
						//stopAKeySound(NULL);
						//playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_INSERT].c_str()));
					}
					break;
				case VK_DELETE:
					if (checkB)
						stopBackground();
					else
					{
						//stopAKeySound(NULL);
						//playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_DELETE].c_str()));
					}
					break;
				case VK_HELP:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_HELP].c_str()));
					break;
				case VK_SLEEP:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_SLEEP].c_str()));
					break;
				case VK_NUMLOCK:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_NUMLOCK].c_str()));
					break;
				case VK_SCROLL:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_SCROLL].c_str()));
					break;
				case VK_LSHIFT:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_LSHIFT].c_str()));
					break;
				case VK_RSHIFT:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_RSHIFT].c_str()));
					break;
				case VK_LCONTROL:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_LCONTROL].c_str()));
					break;
				case VK_RCONTROL:
					stopAKeySound(NULL);
					playAKeySound(NULL, const_cast<wchar_t*>(setting[VK_RCONTROL].c_str()));
					break;
				}
			}
		}
	}
	FreeLibrary(hLib);
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

EXPORT void _getAKeySetting(map<int, wstring> currentSetting, bool check)
{
	setting.clear();
	setting = currentSetting;
	checkB = check;
}

EXPORT void _doInstallAKeyHook(HWND hWnd)
{
	if (hHook != NULL) return;

	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)AKeyHookProc, hinstLib, 0);
	if (hHook)
		MessageBox(hWnd, L"Setup hook successfully", L"Result", MB_OK);
	else
		MessageBox(hWnd, L"Setup hook fail", L"Result", MB_OK);
}

EXPORT void _doRemoveAKeyHook(HWND hWnd)
{
	if (hHook == NULL) return;
	UnhookWindowsHookEx(hHook);
	hHook = NULL;
	MessageBox(hWnd, L"Remove hook successfully", L"Result", MB_OK);
}

