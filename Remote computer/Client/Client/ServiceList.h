#pragma once
#include <string>

// ServiceList dialog

class ServiceList : public CDialogEx
{
	DECLARE_DYNAMIC(ServiceList)

public:
	ServiceList(std::string content, CWnd* pParent = NULL);   // standard constructor
	virtual ~ServiceList();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
