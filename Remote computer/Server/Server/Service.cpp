// Service.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "Service.h"
#include "afxdialogex.h"
#include "Start_Service.h"

// Service dialog
char* Service::_mess = NULL;

IMPLEMENT_DYNAMIC(Service, CDialogEx)

Service* Service::_instance = NULL;

Service::Service(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SERVICE, pParent)
{

}

Service::~Service()
{

}

Service* Service::getInstance()
{
	if (_instance == NULL)
		_instance = new Service();
	return _instance;
}

void Service::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SERVICE, listBox);
}


BEGIN_MESSAGE_MAP(Service, CDialogEx)
	ON_BN_CLICKED(IDC_Display, &Service::OnBnClickedDisplay)
	ON_BN_CLICKED(IDC_Start, &Service::OnBnClickedStart)
	ON_BN_CLICKED(IDC_Close, &Service::OnBnClickedClose)
END_MESSAGE_MAP()


// Service message handlers


void Service::OnBnClickedDisplay()
{
	// TODO: Add your control notification handler code here
	
	// TODO: Add your control notification handler code here
	CServerDlg::_flag = 2;
	CServerDlg::_client.Send("Service_Displayx",16,0);
}

void Service::Display()
{
	listBox.ResetContent();
	CString s;
	char *p;
	p = strtok(_mess, "\n");
	p = strtok(NULL, "\n");
	while (p != NULL)
	{
		s.Format(_T("%S"), p);
		listBox.AddString(s.operator LPCWSTR());
		p = strtok(NULL, "\n");
	}
	listBox.DeleteString(listBox.GetCount() - 1);
	delete[] _mess;
	_mess = NULL;
}


void Service::OnBnClickedStart()
{
	CServerDlg::_flag = 4;
	// TODO: Add your control notification handler code here
	Start_Service s;
	s.DoModal();
}


void Service::OnBnClickedClose()
{
	CServerDlg::_flag = 3;
	// TODO: Add your control notification handler code here
	int iSel = listBox.GetCurSel();
	if (iSel == LB_ERR)
		iSel = 0;
	CString serviceName;
	listBox.GetText(iSel, serviceName);
	
	CT2CA cstr(serviceName);
	char *name = new char[100];
	strcpy(name, "Service_Closexxx");
	strcat(name, cstr);
	CServerDlg::_client.Send(name, strlen(name), 0);
	listBox.DeleteString(iSel);
}
