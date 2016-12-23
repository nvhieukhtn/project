#pragma once
#include "afxsock.h"
#include "stdafx.h"
#include "afxwin.h"

// StartApplication dialog

class StartApplication : public CDialogEx
{
	DECLARE_DYNAMIC(StartApplication)

public:
	StartApplication(CWnd* pParent = NULL);   // standard constructor
	virtual ~StartApplication();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_START_APP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit AppName;
	afx_msg void OnBnClickedStartApp();
	afx_msg void OnBnClickedStopApp();
};
