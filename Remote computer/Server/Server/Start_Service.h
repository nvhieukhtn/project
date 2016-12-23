#pragma once
#include "afxwin.h"
#include "afxsock.h"

// Start_Service dialog

class Start_Service : public CDialogEx
{
	DECLARE_DYNAMIC(Start_Service)

public:
	Start_Service(CWnd* pParent = NULL);   // standard constructor
	virtual ~Start_Service();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_START_SERVICE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
private:
	CEdit ServiceName;
};
