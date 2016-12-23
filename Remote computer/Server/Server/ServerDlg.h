
// ServerDlg.h : header file
//

#pragma once
#include "afxsock.h"
#include "afxwin.h"
#include "Service.h"

#define WM_SOCKET WM_USER+1

// CServerDlg dialog
class CServerDlg : public CDialogEx
{
// Construction
public:
	CServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnTurn();
	afx_msg void OnBnClickedButton1();
private:
	CEdit portNumber;
public:
	afx_msg void OnBnClickedBtnShutdown();
	CButton turnOn;
	afx_msg void OnBnClickedBtnService();
	afx_msg void OnBnClickedBtnKeylog();
	afx_msg void OnBnClickedBtnApp();
	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);
	static CSocket _server;
	static CSocket _client;
	static int _flag;
};
