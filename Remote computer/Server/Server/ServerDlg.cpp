
// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"
#include "StartApplication.h"
#include "KeyLogger.h"
#include "Service.h"


#define PORT 1828

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSocket CServerDlg::_client;
CSocket CServerDlg::_server;


int CServerDlg::_flag = 0;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CServerDlg dialog



CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TXT_PORT, portNumber);
	DDX_Control(pDX, IDC_BTN_TURN, turnOn);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET, SockMsg)
	ON_BN_CLICKED(IDC_BTN_TURN, &CServerDlg::OnBnClickedBtnTurn)
	ON_BN_CLICKED(IDC_BTN_SHUTDOWN, &CServerDlg::OnBnClickedBtnShutdown)
	ON_BN_CLICKED(IDC_BTN_SERVICE, &CServerDlg::OnBnClickedBtnService)
	ON_BN_CLICKED(IDC_BTN_KEYLOG, &CServerDlg::OnBnClickedBtnKeylog)
	ON_BN_CLICKED(IDC_BTN_APP, &CServerDlg::OnBnClickedBtnApp)
END_MESSAGE_MAP()


// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
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
	portNumber.SetWindowText(L"1828");

	portNumber.EnableWindow(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerDlg::OnPaint()
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
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CServerDlg::SockMsg(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam))
	{
		// Display the error and close the socket
		_client.Close();
		_server.Close();
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
		case FD_ACCEPT:
		{
			_server.Accept(_client);
			GetDlgItem(IDC_BTN_SERVICE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_SHUTDOWN)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_APP)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_KEYLOG)->EnableWindow(TRUE);
			break;
		}
		case FD_READ:
		{
			switch (_flag)
			{
				case 1: //Shutdown
				case 2: //Danh sách dịch vụ
				{
					Service::_mess = new char[5000];
					int len = _client.Receive(Service::_mess, 5000, 0);
					Service::_mess[len] = '\0';
					Service::getInstance()->Display();
					break;
				}
				case 3: //Đóng dịch vụ
				case 4: //Mở dịch vụ
				case 5: //Đóng ứng dụng
				case 6: //Mở ứng dụng
				case 7: //Hook
					break;
			}
			break;
		}

		case FD_CLOSE:
		{
			GetDlgItem(IDC_BTN_TURN)->SetWindowText(L"Turn on");
			GetDlgItem(IDC_BTN_SERVICE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_SHUTDOWN)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_APP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_KEYLOG)->EnableWindow(FALSE);
			_client.Close();
		
			break;
		}

	}
	return 0;
}

void CServerDlg::OnBnClickedBtnTurn()
{
	CString cstr;
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BTN_TURN)->GetWindowText(cstr);
	if (cstr == L"Turn on")
	{
		GetDlgItem(IDC_BTN_TURN)->SetWindowText(L"Turn off");
		UpdateData(TRUE);
		AfxSocketInit(NULL);
		_server.Create(PORT);

		_server.Listen();
		int err = WSAAsyncSelect(_server, m_hWnd, WM_SOCKET, FD_READ | FD_ACCEPT | FD_CLOSE);
		if (err)
			MessageBox((LPCTSTR)"Cant call WSAAsyncSelect");
	}
	else if (cstr == L"Turn off")
	{
		GetDlgItem(IDC_BTN_TURN)->SetWindowText(L"Turn on");
		GetDlgItem(IDC_BTN_SERVICE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SHUTDOWN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_APP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_KEYLOG)->EnableWindow(FALSE);
		_client.Close();
		_server.Close();
	}
}


void CServerDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
}


void CServerDlg::OnBnClickedBtnShutdown()
{
	char *mess = new char[10];
	// TODO: Add your control notification handler code here
	_client.Send("Shutdownxxxxxxxx", 16, 0);
	delete[]mess;
}


void CServerDlg::OnBnClickedBtnService()
{
	Service *s = Service::getInstance();
	s->DoModal();
}


void CServerDlg::OnBnClickedBtnKeylog()
{
	// TODO: Add your control notification handler code here
	KeyLogger keylogger;
	keylogger.DoModal();
}


void CServerDlg::OnBnClickedBtnApp()
{
	// TODO: Add your control notification handler code here
	StartApplication app;
	app.DoModal();
}
