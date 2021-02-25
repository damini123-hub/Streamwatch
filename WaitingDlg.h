#pragma once

#include "PictureEx.h"

// WaitingDlg dialog

class WaitingDlg : public CDialog
{
	DECLARE_DYNAMIC(WaitingDlg)

public:
	WaitingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~WaitingDlg();

	BOOL OnInitDialog();
	void OnClose();

// Dialog Data
	enum { IDD = IDD_WAITING_DLG };

	CPictureEx m_Picture;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
