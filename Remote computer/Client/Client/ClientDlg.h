
// ClientDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxsock.h"
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include "afxcmn.h"

#define WM_SOCKET WM_USER+2


// CClientDlg dialog
class CClientDlg : public CDialogEx
{
// Construction
public:
	CClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LONG_PTR OnHookLowKeyboard(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	static int _flag;
	char *_name;
	CEdit ID_ADDR;
	CButton CONNECT;
	CSocket _client;
	afx_msg void Shutdown();
	afx_msg std::string exec(const char* cmd); // Hàm lấy dữ liệu Output từ hàm System()
	afx_msg void SetHookMessage();
	afx_msg void UnsetHookMessage();
	afx_msg void Service_Display();
	afx_msg void Service_Start();
	afx_msg void Service_Close();
	afx_msg void Application_Start();
	afx_msg void Application_Close();
public:
	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnConnect();
	
	CString _hookText;
	
};
