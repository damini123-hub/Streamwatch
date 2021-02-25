#pragma once
#include "afxwin.h"

// CAlertMail dialog
typedef struct stMAIL
{
	char To[100][MAX_PATH];
	char From[MAX_PATH];
	char SMTP[MAX_PATH];
	char UserName[MAX_PATH];
	char Password[MAX_PATH];
	char Subject[4500];
	char Text[4500];
	char HeaderText[MAX_PATH];
}
MAIL;

typedef struct AlertMember
{
	int m_nAlertTime;
	TCHAR m_strAlertUserName[100];
	TCHAR m_strAlertPassword[100];
	TCHAR m_strAlertSMTP[100];
	TCHAR m_strAlertReceiverEmailId[10][100];
	TCHAR m_strAlertEmailList[10][100];
}AlertGlobal;


extern CStream StreamWatch[MAX_STREAM];
extern int m_nLiecensedStream;

class CAlertMail : public CDialog
{
	DECLARE_DYNAMIC(CAlertMail)

public:
	CAlertMail(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAlertMail();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	CToolTipCtrl* m_pToolTip;
	AlertGlobal AlertData;

	int m_nAlertDurationTime;
	int m_nTotalEmailId;
	int m_nTotalEmailIdList;
	CString m_strUserName;
	CString m_strPasswort;
	CString m_strReceiverEmailId;
	CString m_strSMTP;
	int nStopChannelPrime[50];
	int nIndex[2];
	int nStopChannelSecond[50];

	int m_nChangedStatusPrime[50][2];
	int m_nChangedStatusSecond[50][2];
	int m_nTotalChangedChannels[2];

	bool m_bPrimUDP;
	bool m_bSecUDP;
	int m_nAlertStatusChanged[2];
	bool m_bStopChannel[2];
	TCHAR SystemName[100];
	CListBox m_List_Final_Email;
	CListBox m_ListBox_Email;

	int m_nChannelCCError[50][2];
	int m_nTotalChannelCCError;

	void SendMail();
	void SendData(int TotalCount[], int ChnnelNumPrime[],int ChnnelNumSecond[]);
	void SendData1(int TotalCount[], int ChnnelNumPrime[], int ChnnelNumSecond[]);
	void SendCCErrorData();
	bool isEmail(CString inputEmail);
	bool isMailServer(CString mailServer);
	bool findEmail(int StartIndex, CString strEmail);
	void xmlDataStore();
	CString GetAppPath(HMODULE hModule);
	
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonListAdd();
	afx_msg void OnBnClickedButtonListAlladd();
	afx_msg void OnBnClickedButtonListRemove();
	afx_msg void OnBnClickedButtonListRemoveall();
	afx_msg void OnEnKillfocusEditAlertTimeDuration();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
	afx_msg void OnEnKillfocusEditUsernameAlert();
	void readEmailSystemName(TCHAR* AppPath);
};
