// Start_Service.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "Start_Service.h"
#include "afxdialogex.h"
#include "ServerDlg.h"

// Start_Service dialog

IMPLEMENT_DYNAMIC(Start_Service, CDialogEx)

Start_Service::Start_Service(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_START_SERVICE, pParent)
{

}

Start_Service::~Start_Service()
{
}

void Start_Service::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TXT_NAME_SERVICE, ServiceName);
}


BEGIN_MESSAGE_MAP(Start_Service, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_START, &Start_Service::OnBnClickedBtnStart)
END_MESSAGE_MAP()


// Start_Service message handlers


void Start_Service::OnBnClickedBtnStart()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString serviceName;
	ServiceName.GetWindowText(serviceName);
	CT2CA cstr(serviceName);
	
	char *name = new char[100];
	strcpy(name, "Service_Startxxx");
	strcat(name, cstr);
	CServerDlg::_client.Send(name, strlen(name), 0);
}
