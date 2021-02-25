
#include "stdafx.h"
#include "PictureCtrl.h"
#include "StreamWatchDlg.h"
#pragma once

// CStream_Watch_Tab1 dialog

class CStream_Watch_Tab1 : public CDialog
{
	DECLARE_DYNAMIC(CStream_Watch_Tab1)

public:
	CStream_Watch_Tab1(CWnd* pParent, CStream_WatchDlg* parent_Dlg);   // standard constructor
	virtual ~CStream_Watch_Tab1();
// Dialog Data
	enum { IDD = IDD_STREAM_WATCH_TAB1 };
    CToolTipCtrl* m_pToolTip;
	
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HICON  m_hIcon;
	CStream_WatchDlg* m_parent_dlg;

	DECLARE_MESSAGE_MAP()

public:
	CTabGeneral General_Tab1;
	BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonStart1();
	afx_msg void OnBnClickedButtonStart2();
	afx_msg void OnBnClickedButtonStart3();
	afx_msg void OnBnClickedButtonStart4();
	afx_msg void OnBnClickedButtonStart5();
	
	afx_msg void OnBnClickedButtonStart6();
	afx_msg void OnBnClickedButtonStart7();
	afx_msg void OnBnClickedButtonStart8();
	afx_msg void OnBnClickedButtonStart9();
	afx_msg void OnBnClickedButtonStart10();
	afx_msg void OnBnClickedButtonStart11();
	afx_msg void OnBnClickedButtonStart12();
	afx_msg void OnBnClickedButtonStart13();
	afx_msg void OnBnClickedButtonStart14();
	afx_msg void OnBnClickedButtonStart15();
	afx_msg void OnBnClickedButtonStart16();

	afx_msg void OnBnClickedButtonStop1();
	afx_msg void OnBnClickedButtonStop2();
	afx_msg void OnBnClickedButtonStop3();
	afx_msg void OnBnClickedButtonStop4();
	afx_msg void OnBnClickedButtonStop5();
	afx_msg void OnBnClickedButtonStop6();
	afx_msg void OnBnClickedButtonStop7();
	afx_msg void OnBnClickedButtonStop8();
	afx_msg void OnBnClickedButtonStop9();
	afx_msg void OnBnClickedButtonStop10();
	afx_msg void OnBnClickedButtonStop11();
	afx_msg void OnBnClickedButtonStop12();
	afx_msg void OnBnClickedButtonStop13();
	afx_msg void OnBnClickedButtonStop14();
	afx_msg void OnBnClickedButtonStop15();
	afx_msg void OnBnClickedButtonStop16();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedCheck6();
	afx_msg void OnBnClickedCheck8();
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnBnClickedCheck9();
	afx_msg void OnBnClickedCheck10();
	afx_msg void OnBnClickedCheck11();
	afx_msg void OnBnClickedCheck12();
	afx_msg void OnBnClickedCheck13();
	afx_msg void OnBnClickedCheck14();
	afx_msg void OnBnClickedCheck15();
	afx_msg void OnBnClickedCheck16();
	
	afx_msg void OnEnKillfocusEditChannelName1();
	afx_msg void OnEnKillfocusEditChannelName2();
	afx_msg void OnEnKillfocusEditChannelName3();
	afx_msg void OnEnKillfocusEditChannelName4();
	afx_msg void OnEnKillfocusEditChannelName5();
	afx_msg void OnEnKillfocusEditChannelName6();
	afx_msg void OnEnKillfocusEditChannelName7();
	afx_msg void OnEnKillfocusEditChannelName8();
	afx_msg void OnEnKillfocusEditChannelName9();
	afx_msg void OnEnKillfocusEditChannelName10();
	afx_msg void OnEnKillfocusEditChannelName11();
	afx_msg void OnEnKillfocusEditChannelName12();
	afx_msg void OnEnKillfocusEditChannelName13();
	afx_msg void OnEnKillfocusEditChannelName14();
	afx_msg void OnEnKillfocusEditChannelName15();
	afx_msg void OnEnKillfocusEditChannelName16();
	afx_msg void OnBnClickedManualCheck1();
	afx_msg void OnBnClickedManualCheck2();
	afx_msg void OnBnClickedManualCheck3();
	afx_msg void OnBnClickedManualCheck4();
	afx_msg void OnBnClickedManualCheck5();
	afx_msg void OnBnClickedManualCheck6();
	afx_msg void OnBnClickedManualCheck7();
	afx_msg void OnBnClickedManualCheck8();
	afx_msg void OnBnClickedManualCheck9();
	afx_msg void OnBnClickedManualCheck10();
	afx_msg void OnBnClickedManualCheck11();
	afx_msg void OnBnClickedManualCheck12();
	afx_msg void OnBnClickedManualCheck13();
	afx_msg void OnBnClickedManualCheck14();
	afx_msg void OnBnClickedManualCheck15();
	afx_msg void OnBnClickedManualCheck16();
	afx_msg void OnBnClickedCheckEme1();
	afx_msg void OnBnClickedCheckEme2();
	afx_msg void OnBnClickedCheckEme3();
	afx_msg void OnBnClickedCheckEme4();
	afx_msg void OnBnClickedCheckEme5();
	afx_msg void OnBnClickedCheckEme6();
	afx_msg void OnBnClickedCheckEme7();
	afx_msg void OnBnClickedCheckEme8();
	afx_msg void OnBnClickedCheckEme9();
	afx_msg void OnBnClickedCheckEme10();
	afx_msg void OnBnClickedCheckEme11();
	afx_msg void OnBnClickedCheckEme12();
	afx_msg void OnBnClickedCheckEme13();
	afx_msg void OnBnClickedCheckEme14();
	afx_msg void OnBnClickedCheckEme15();
	afx_msg void OnBnClickedCheckEme16();
	afx_msg void OnEnChangeWaitingTime1();
};
