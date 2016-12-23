// Audio.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <mmsystem.h>

#define EXPORT __declspec(dllexport)

#include   "stdafx.h"
#include   <windows.h>
#include   <winbase.h>
#include   <commdlg.h>
#include   <mmsystem.h>
#include   <stdio.h>

//  ---------------------------------------------------------------------
//    Cac bien toan cuc
//  ---------------------------------------------------------------------

MCIDEVICEID		mciWaveDevID = NULL;

//  ---------------------------------------------------------------------
//	  Prototype cua cac ham cuc bo	
//  ---------------------------------------------------------------------
BOOL	isWave(wchar_t *szFileName);
BOOL	isMIDI(wchar_t *szFileName);

//  ---------------------------------------------------------------------
//  ---------------------------------------------------------------------
EXPORT void doMCICommandStringPlay(HWND hFrameWnd, wchar_t *szFileName)
{
	wchar_t			szDevType[60];
	wchar_t			szMCIString[128];
	MCIERROR		dwReturn;
	if (wcscmp(szFileName, L"") == 0)
		return;

	if (isWave(szFileName))	wcscpy_s(szDevType, L"waveaudio");
	else if (isMIDI(szFileName))  wcscpy_s(szDevType, L"sequencer");
	else {
		MessageBox(hFrameWnd, L"File type not supported !", L"Error", MB_OK);
		return;
	}

	wsprintf(szMCIString, L"open %s type %s alias MyWAVMIDI ", szFileName, szDevType);

	if (dwReturn = mciSendString(szMCIString, NULL, 0, NULL))
	{
		wchar_t szError[128];

		mciGetErrorString(dwReturn, szError, 128);

		MessageBox(hFrameWnd, szError, L"Error", MB_OK);
		return;
	}

	// notify: se goi message MM_MCINOTIFY cho cua so parent de bao hieu da play xong
	mciSendString(L"play MyWAVMIDI notify", NULL, 0, hFrameWnd);

}

//  ---------------------------------------------------------------------
//  ---------------------------------------------------------------------
EXPORT void doMCICommandStringStop(HWND hFrameWnd)
{
	mciSendCommand(1, MCI_CLOSE, 0, 0L);
}

//  ---------------------------------------------------------------------
//  ---------------------------------------------------------------------
EXPORT void doMCICommandMessagePlay(HWND hFrameWnd, wchar_t *szFileName)
{
	// Mo wave/midi
	MCI_OPEN_PARMS	mciOpenParams;
	MCIERROR		dwReturn;

	if (wcscmp(szFileName, L"") == 0)
		return;

	// mciOpenParams.lpstrDeviceType phai dung voi flag MCI_OPEN_TYPE
	if (isWave(szFileName))	mciOpenParams.lpstrDeviceType = L"waveaudio";
	else if (isMIDI(szFileName))  mciOpenParams.lpstrDeviceType = L"sequencer";
	else {
		MessageBox(hFrameWnd, L"File type not supported !", L"Error", MB_OK);
		return;
	}

	mciOpenParams.lpstrElementName = szFileName;	// phai dung voi flag MCI_OPEN_ELEMENT

	if (dwReturn = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
		(DWORD)(LPVOID)&mciOpenParams))
	{
		wchar_t szError[128];

		mciGetErrorString(dwReturn, szError, 128);

		MessageBox(hFrameWnd, szError, L"Error", MB_OK);
		return;
	}

	// Lay Device ID
	mciWaveDevID = mciOpenParams.wDeviceID;

	// Play file
	MCI_PLAY_PARMS mciPlayParams;
	mciPlayParams.dwCallback = (unsigned long)hFrameWnd;			// cua so nhan message MM_MCINOTIFY

	if (dwReturn = mciSendCommand(mciWaveDevID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlayParams))
	{
		wchar_t szError[128];

		mciSendCommand(mciWaveDevID, MCI_CLOSE, 0, 0L);

		mciGetErrorString(dwReturn, szError, 128);

		MessageBox(hFrameWnd, szError, L"Error", MB_OK);
		return;
	}
}

//  ---------------------------------------------------------------------
//  ---------------------------------------------------------------------
void doMCICommandMessageStop(HWND hFrameWnd)
{
	if (mciWaveDevID)
		mciSendCommand(mciWaveDevID, MCI_STOP, 0, 0);
}

//  ---------------------------------------------------------------------
//	   Kiem tra xem file szFileName co phai la 1 file WAVE khong ?
//  ---------------------------------------------------------------------
BOOL isWave(wchar_t *szFileName)
{
	HMMIO hmmio;

	// Mo file 
	if (!(hmmio = mmioOpen(szFileName, 0, MMIO_READ | MMIO_ALLOCBUF)))
		return FALSE;

	MMCKINFO mmckinfoParent;   // Group Header (special chunk)

							   // Dinh vi nhom wave (group header) 
	mmckinfoParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	// Neu khong co nhom wave thi day la khong phai thuoc dinh dang wave
	if (mmioDescend(hmmio, (LPMMCKINFO)&mmckinfoParent, 0, MMIO_FINDRIFF))
		return FALSE;

	// Nguoc lai dung -> dung
	return TRUE;
}

//  ---------------------------------------------------------------------
//	   Kiem tra xem file szFileName co phai la 1 file MIDI khong ?
//  ---------------------------------------------------------------------
BOOL isMIDI(wchar_t *szFileName)
{
	HMMIO hmmio;

	// Mo file 
	if (!(hmmio = mmioOpen(szFileName, 0, MMIO_READ | MMIO_ALLOCBUF)))
		return FALSE;

	MMCKINFO mmckinfoSubchunk;

	memset(&mmckinfoSubchunk, 0, sizeof(mmckinfoSubchunk));

	// Tim nhom MThd 
	mmckinfoSubchunk.ckid = mmioFOURCC('M', 'T', 'h', 'd');

	// Neu khong tim thay nhom MThd -> khong phai file Midi
	if (mmioDescend(hmmio, &mmckinfoSubchunk, 0, MMIO_FINDCHUNK))
		return FALSE;

	// Nguoc lai -> dung
	return TRUE;
}

//  ---------------------------------------------------------------------
//	   Kiem tra xem file szFileName co phai la 1 file AVI khong ?
//  ---------------------------------------------------------------------
BOOL isAVI(wchar_t *szFileName)
{
	HMMIO hmmio;

	// Mo file 
	if (!(hmmio = mmioOpen(szFileName, 0, MMIO_READ | MMIO_ALLOCBUF)))
		return FALSE;

	MMCKINFO mmckinfoSubchunk;

	memset(&mmckinfoSubchunk, 0, sizeof(mmckinfoSubchunk));

	// Tim nhom RIFF 
	mmckinfoSubchunk.ckid = mmioFOURCC('R', 'I', 'F', 'F');

	// Neu khong tim thay nhom RIFF -> khong phai file Midi
	if (mmioDescend(hmmio, &mmckinfoSubchunk, 0, MMIO_FINDCHUNK))
		return FALSE;

	// Nguoc lai -> dung
	return TRUE;
}

EXPORT void PlayAudio(wchar_t *szFileName)
{
	if (wcscmp(szFileName, L"") == 0)
		return;
	PlaySound(szFileName, NULL, SND_ASYNC | SND_FILENAME);
}

EXPORT void StopAudio()
{
	PlaySound(0, 0, 0);
}
