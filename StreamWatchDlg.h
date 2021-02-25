// New_Version_Stream_WatchDlg.h : header file
//
#pragma once

#include "StreamTabCtrl.h"
#include "PictureCtrl.h"
#include "stdafx.h"
#include "Tab_Global.h"
#include "StatusMembers.h"
#include "AlertMail.h"

extern CStream StreamWatch[MAX_STREAM];
extern int m_nLiecensedStream; 

// CStream_WatchDlg dialog
class CStream_WatchDlg : public CDialog
{
// Construction
protected:
	HICON m_hIcon;

public:
	CStream_WatchDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CStream_WatchDlg();
	CStreamTabCtrl	m_TabStreamTabCtrl;
	CPictureCtrl m_pic_Notify[4];
	int m_nTotalStartStream;
	int m_nTotalStartedChannel;
	bool m_bTimerSet;
	int m_nAlertBufMessagePrime[50];
	int m_nAlertCompairPrime[50];
	int m_nAlertBufMessageSecond[50];
	int m_nAlertCompairSecond[50];
	bool m_bCheckAlertStatus;
	bool m_bCheckAlertStatusList[50];
	int index[2];
	int IndexSecond;
	int OldIndexSecond;
	int lastCheckedIndex[2];
	bool m_bIntervalAlertTime;
	DWORD m_nLastAlertTime;

	bool m_bCCErrorMail;
	bool m_bAlertNoMail;

// Dialog Data
	enum { IDD = IDD_NEW_VERSION_STREAM_WATCH_DIALOG };
	CToolTipCtrl* m_pToolTip;
	
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	
// Implementation
public:
	void ReadXmlOnce();
	bool ReadXml();
	CString LoadXml();
	void CreateXmlFile();
	CString GetAppPath(HMODULE hModule);
	void StartEvent(int startButton_Id);
	void StopEvent(int stopButton_Id);
	int StreamStart(int num_Id);
	void DisableControl(int i);
	void DisableSecondarySource();
	void DiableDestination();
	void EnableControl(int i);
	void CheckedBoxEffect(int i,int pTab_Id);
	void EmeCheckBoxEffect(int checkBoxId,int clicked_tabId);
	bool StreamValidation(int num_Id);
	void KillFocusEffect(int ChannelId);
	void StoreCheckboxData();
	void readEmailSystemName(TCHAR* AppPath);
	//void readVideoAnalysisInfo(int ChannelId);
	void ManualCheckBoxEffect(int checkBoxId,int clicked_tabId);
	void LoadXML_Started_channels();
	void StorePrevStreamStatus();
	void StoreSecondaryPrgNo();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnClose();

protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonStartAll();
	afx_msg void OnBnClickedButtonStopAll();
	afx_msg void OnBnClickedCheckedAll();
	afx_msg void OnBnClickedAlertConfig();
};
