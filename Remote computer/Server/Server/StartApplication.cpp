// StartApplication.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "StartApplication.h"
#include "afxdialogex.h"
#include "ServerDlg.h"


// StartApplication dialog

IMPLEMENT_DYNAMIC(StartApplication, CDialogEx)

StartApplication::StartApplication(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_START_APP, pParent)
{

}

StartApplication::~StartApplication()
{
}

void StartApplication::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NAME_APP, AppName);
}


BEGIN_MESSAGE_MAP(StartApplication, CDialogEx)
	ON_BN_CLICKED(IDC_START_APP, &StartApplication::OnBnClickedStartApp)
	ON_BN_CLICKED(IDC_STOP_APP, &StartApplication::OnBnClickedStopApp)
END_MESSAGE_MAP()


// StartApplication message handlers


void StartApplication::OnBnClickedStartApp()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString AppNameStr;
	AppName.GetWindowText(AppNameStr);
	if (AppNameStr.GetLength() == 0)
		return;
	CT2CA AppNameChar(AppNameStr);

	char *name = new char[100];
	strcpy(name, "ApplicationStart");
	strcat(name, AppNameChar);
	CServerDlg::_client.Send(name, strlen(name), 0);
}


void StartApplication::OnBnClickedStopApp()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CServerDlg::_client.Send("ApplicationClose", 16, 0);
	CString AppNameStr;
	AppName.GetWindowText(AppNameStr);
	if (AppNameStr.GetLength() == 0)
		return;
	CT2CA AppNameChar(AppNameStr);
	char *name = new char[100];
	strcpy(name, "ApplicationClose");
	strcat(name, AppNameChar);
	CServerDlg::_client.Send(AppNameChar, AppNameStr.GetLength(), 0);
}
