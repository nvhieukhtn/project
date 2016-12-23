#pragma once

#include "afxsock.h"
#include "afxwin.h"
#include "ServerDlg.h"
// Service dialog

class Service : public CDialogEx
{
	DECLARE_DYNAMIC(Service)

public:
	Service(CWnd* pParent = NULL);   // standard constructor
	virtual ~Service();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVICE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	static Service *_instance;
public:
	afx_msg void Display();
	static char *_mess;
	afx_msg static Service* getInstance();
	afx_msg void OnBnClickedDisplay();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedClose();
	CListBox listBox;
};
