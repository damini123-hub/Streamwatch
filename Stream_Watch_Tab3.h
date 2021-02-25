#pragma once

#include "stdafx.h"
#include "PictureCtrl.h"
#include "StreamWatchDlg.h"

// CStream_Watch_Tab3 dialog

class CStream_Watch_Tab3 : public CDialog
{
	DECLARE_DYNAMIC(CStream_Watch_Tab3)

public:
	CStream_Watch_Tab3(CWnd* pParent, CStream_WatchDlg* parent_Dlg);   // standard constructor
	virtual ~CStream_Watch_Tab3();
// Dialog Data
	enum { IDD = IDD_STREAM_WATCH_TAB3 };
	CToolTipCtrl* m_pToolTip;

protected:
	CStream_WatchDlg* m_parent_dlg;
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	 CTabGeneral General_Tab3;
	
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	
	afx_msg void OnBnClickedButtonStart33();
	afx_msg void OnBnClickedButtonStart34();
	afx_msg void OnBnClickedButtonStart35();
	afx_msg void OnBnClickedButtonStart36();
	afx_msg void OnBnClickedButtonStart37();
	afx_msg void OnBnClickedButtonStart38();
	afx_msg void OnBnClickedButtonStart39();
	afx_msg void OnBnClickedButtonStart40();
	afx_msg void OnBnClickedButtonStart41();
	afx_msg void OnBnClickedButtonStart42();
	afx_msg void OnBnClickedButtonStart43();
	afx_msg void OnBnClickedButtonStart44();
	afx_msg void OnBnClickedButtonStart45();
	afx_msg void OnBnClickedButtonStart46();
	afx_msg void OnBnClickedButtonStart47();
	afx_msg void OnBnClickedButtonStart48();
	afx_msg void OnBnClickedButtonStop33();
	afx_msg void OnBnClickedButtonStop34();
	afx_msg void OnBnClickedButtonStop35();
	afx_msg void OnBnClickedButtonStop36();
	afx_msg void OnBnClickedButtonStop37();
	afx_msg void OnBnClickedButtonStop38();
	afx_msg void OnBnClickedButtonStop39();
	afx_msg void OnBnClickedButtonStop40();
	afx_msg void OnBnClickedButtonStop41();
	afx_msg void OnBnClickedButtonStop42();
	afx_msg void OnBnClickedButtonStop43();
	afx_msg void OnBnClickedButtonStop44();
	afx_msg void OnBnClickedButtonStop45();
	afx_msg void OnBnClickedButtonStop46();
	afx_msg void OnBnClickedButtonStop47();
	afx_msg void OnBnClickedButtonStop48();
	afx_msg void OnBnClickedCheck33();
	afx_msg void OnBnClickedCheck34();
	afx_msg void OnBnClickedCheck35();
	afx_msg void OnBnClickedCheck36();
	afx_msg void OnBnClickedCheck37();
	afx_msg void OnBnClickedCheck38();
	afx_msg void OnBnClickedCheck39();
	afx_msg void OnBnClickedCheck40();
	afx_msg void OnBnClickedCheck41();
	afx_msg void OnBnClickedCheck42();
	afx_msg void OnBnClickedCheck43();
	afx_msg void OnBnClickedCheck44();
	afx_msg void OnBnClickedCheck45();
	afx_msg void OnBnClickedCheck46();
	afx_msg void OnBnClickedCheck47();
	afx_msg void OnBnClickedCheck48();
	afx_msg void OnEnKillfocusEditChannelName33();
	afx_msg void OnEnKillfocusEditChannelName34();
	afx_msg void OnEnKillfocusEditChannelName35();
	afx_msg void OnEnKillfocusEditChannelName36();
	afx_msg void OnEnKillfocusEditChannelName37();
	afx_msg void OnEnKillfocusEditChannelName38();
	afx_msg void OnEnKillfocusEditChannelName39();
	afx_msg void OnEnKillfocusEditChannelName40();
	afx_msg void OnEnKillfocusEditChannelName41();
	afx_msg void OnEnKillfocusEditChannelName42();
	afx_msg void OnEnKillfocusEditChannelName43();
	afx_msg void OnEnKillfocusEditChannelName44();
	afx_msg void OnEnKillfocusEditChannelName45();
	afx_msg void OnEnKillfocusEditChannelName46();
	afx_msg void OnEnKillfocusEditChannelName47();
	afx_msg void OnEnKillfocusEditChannelName48();
	afx_msg void OnBnClickedManualCheck33();
	afx_msg void OnBnClickedManualCheck34();
	afx_msg void OnBnClickedManualCheck35();
	afx_msg void OnBnClickedManualCheck36();
	afx_msg void OnBnClickedManualCheck37();
	afx_msg void OnBnClickedManualCheck38();
	afx_msg void OnBnClickedManualCheck39();
	afx_msg void OnBnClickedManualCheck40();
	afx_msg void OnBnClickedManualCheck41();
	afx_msg void OnBnClickedManualCheck42();
	afx_msg void OnBnClickedManualCheck43();
	afx_msg void OnBnClickedManualCheck44();
	afx_msg void OnBnClickedManualCheck45();
	afx_msg void OnBnClickedManualCheck46();
	afx_msg void OnBnClickedManualCheck47();
	afx_msg void OnBnClickedManualCheck48();
	afx_msg void OnBnClickedCheckEme33();
	afx_msg void OnBnClickedCheckEme34();
	afx_msg void OnBnClickedCheckEme35();
	afx_msg void OnBnClickedCheckEme36();
	afx_msg void OnBnClickedCheckEme37();
	afx_msg void OnBnClickedCheckEme38();
	afx_msg void OnBnClickedCheckEme39();
	afx_msg void OnBnClickedCheckEme40();
	afx_msg void OnBnClickedCheckEme41();
	afx_msg void OnBnClickedCheckEme42();
	afx_msg void OnBnClickedCheckEme43();
	afx_msg void OnBnClickedCheckEme44();
	afx_msg void OnBnClickedCheckEme45();
	afx_msg void OnBnClickedCheckEme46();
	afx_msg void OnBnClickedCheckEme47();
	afx_msg void OnBnClickedCheckEme48();
};
