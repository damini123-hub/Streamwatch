#pragma once
#include "StreamWatchDlg.h"

// CStream_Watch_Tab2 dialog

class CStream_Watch_Tab2 : public CDialog
{
	DECLARE_DYNAMIC(CStream_Watch_Tab2)

public:
	CStream_Watch_Tab2(CWnd* pParent, CStream_WatchDlg* parent_Dlg);   // standard constructor
	virtual ~CStream_Watch_Tab2();

// Dialog Data
	enum { IDD = IDD_STREAM_WATCH_TAB2 };
	CToolTipCtrl* m_pToolTip;

protected:
	CStream_WatchDlg* m_parent_dlg;
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	CTabGeneral General_Tab2;
	BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonStart17();
	afx_msg void OnBnClickedButtonStart18();
	afx_msg void OnBnClickedButtonStart19();
	afx_msg void OnBnClickedButtonStart20();
	afx_msg void OnBnClickedButtonStart21();
	afx_msg void OnBnClickedButtonStart22();
	afx_msg void OnBnClickedButtonStart23();
	afx_msg void OnBnClickedButtonStart24();
	afx_msg void OnBnClickedButtonStart25();
	afx_msg void OnBnClickedButtonStart26();
	afx_msg void OnBnClickedButtonStart27();
	afx_msg void OnBnClickedButtonStart28();
	afx_msg void OnBnClickedButtonStart29();
	afx_msg void OnBnClickedButtonStart30();
	afx_msg void OnBnClickedButtonStart31();
	afx_msg void OnBnClickedButtonStart32();
	afx_msg void OnBnClickedButtonStop17();
	afx_msg void OnBnClickedButtonStop18();
	afx_msg void OnBnClickedButtonStop19();
	afx_msg void OnBnClickedButtonStop20();
	afx_msg void OnBnClickedButtonStop21();
	afx_msg void OnBnClickedButtonStop22();
	afx_msg void OnBnClickedButtonStop23();
	afx_msg void OnBnClickedButtonStop24();
	afx_msg void OnBnClickedButtonStop25();
	afx_msg void OnBnClickedButtonStop26();
	afx_msg void OnBnClickedButtonStop27();
	afx_msg void OnBnClickedButtonStop28();
	afx_msg void OnBnClickedButtonStop29();
	afx_msg void OnBnClickedButtonStop30();
	afx_msg void OnBnClickedButtonStop31();
	afx_msg void OnBnClickedButtonStop32();
	afx_msg void OnBnClickedCheck17();
	afx_msg void OnBnClickedCheck18();
	afx_msg void OnBnClickedCheck19();
	afx_msg void OnBnClickedCheck20();
	afx_msg void OnBnClickedCheck21();
	afx_msg void OnBnClickedCheck22();
	afx_msg void OnBnClickedCheck23();
	afx_msg void OnBnClickedCheck24();
	afx_msg void OnBnClickedCheck25();
	afx_msg void OnBnClickedCheck26();
	afx_msg void OnBnClickedCheck27();
	afx_msg void OnBnClickedCheck28();
	afx_msg void OnBnClickedCheck29();
	afx_msg void OnBnClickedCheck30();
	afx_msg void OnBnClickedCheck31();
	afx_msg void OnBnClickedCheck32();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnEnKillfocusEditChannelName17();
	afx_msg void OnEnKillfocusEditChannelName18();
	afx_msg void OnEnKillfocusEditChannelName19();
	afx_msg void OnEnKillfocusEditChannelName20();
	afx_msg void OnEnKillfocusEditChannelName21();
	afx_msg void OnEnKillfocusEditChannelName22();
	afx_msg void OnEnKillfocusEditChannelName23();
	afx_msg void OnEnKillfocusEditChannelName24();
	afx_msg void OnEnKillfocusEditChannelName25();
	afx_msg void OnEnKillfocusEditChannelName26();
	afx_msg void OnEnKillfocusEditChannelName27();
	afx_msg void OnEnKillfocusEditChannelName28();
	afx_msg void OnEnKillfocusEditChannelName29();
	afx_msg void OnEnKillfocusEditChannelName30();
	afx_msg void OnEnKillfocusEditChannelName31();
	afx_msg void OnEnKillfocusEditChannelName32();
	afx_msg void OnBnClickedManualCheck17();
	afx_msg void OnBnClickedManualCheck18();
	afx_msg void OnBnClickedManualCheck19();
	afx_msg void OnBnClickedManualCheck20();
	afx_msg void OnBnClickedManualCheck21();
	afx_msg void OnBnClickedManualCheck22();
	afx_msg void OnBnClickedManualCheck23();
	afx_msg void OnBnClickedManualCheck24();
	afx_msg void OnBnClickedManualCheck25();
	afx_msg void OnBnClickedManualCheck26();
	afx_msg void OnBnClickedManualCheck27();
	afx_msg void OnBnClickedManualCheck28();
	afx_msg void OnBnClickedManualCheck29();
	afx_msg void OnBnClickedManualCheck30();
	afx_msg void OnBnClickedManualCheck31();
	afx_msg void OnBnClickedManualCheck32();
	afx_msg void OnBnClickedCheckEme17();
	afx_msg void OnBnClickedCheckEme18();
	afx_msg void OnBnClickedCheckEme19();
	afx_msg void OnBnClickedCheckEme20();
	afx_msg void OnBnClickedCheckEme21();
	afx_msg void OnBnClickedCheckEme22();
	afx_msg void OnBnClickedCheckEme23();
	afx_msg void OnBnClickedCheckEme24();
	afx_msg void OnBnClickedCheckEme25();
	afx_msg void OnBnClickedCheckEme26();
	afx_msg void OnBnClickedCheckEme27();
	afx_msg void OnBnClickedCheckEme28();
	afx_msg void OnBnClickedCheckEme29();
	afx_msg void OnBnClickedCheckEme30();
	afx_msg void OnBnClickedCheckEme31();
	afx_msg void OnBnClickedCheckEme32();
};
