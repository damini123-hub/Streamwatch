// Stream_Watch_Tab1.cpp : implementation file
//

#include "stdafx.h"
#include "StreamWatch.h"
#include "Stream_Watch_Tab1.h"

// CStream_Watch_Tab1 dialog

extern CStream StreamWatch[MAX_STREAM];

//#define IDC_COMBO_SOURCE_PRIM_ IDC_COMBO_SOURCE_PRIM_

IMPLEMENT_DYNAMIC(CStream_Watch_Tab1, CDialog)

CStream_Watch_Tab1::CStream_Watch_Tab1(CWnd* pParent /*=NULL*/, CStream_WatchDlg* parent_Dlg)
	: CDialog(CStream_Watch_Tab1::IDD, pParent)
    ,m_parent_dlg(parent_Dlg)
{
	for(int k = 0; k < MAX_STREAM/3; k++)
	{
		for(int i = 0; i < MAX_RECEIVER; i++)
		{
			General_Tab1.m_strNic_Source[k][i] = _T("");
			General_Tab1.m_strIp_Source[k][i] = _T("");
			//General_Tab1.m_nPort_Source[k][i] = 8001 + i + (2 * k) ;
			General_Tab1.m_nPort_Source[k][i] = 0;
		}
		General_Tab1.m_strNic_Dest[k] = _T("");
		General_Tab1.m_strIp_Dest[k] = _T("");
		General_Tab1.m_nPort_Dest[k] = 0;
		General_Tab1.m_m_nDetection_Time[k] = 0;
		General_Tab1.m_nPresentDetection_Time[k] = 0;
		General_Tab1.m_strChannel_Name[k] = _T("");
	}
}

CStream_Watch_Tab1::~CStream_Watch_Tab1()
{
	if(m_pToolTip != NULL)
	{
		SAFE_DELETE(m_pToolTip);
	}
}

void CStream_Watch_Tab1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);	
	for(int k = 0; k < MAX_STREAM/3; k++)
	{
		int m = 200 + (20 * k) + 1;

		DDX_Control(pDX, m++, General_Tab1.m_cmbSourceAdapter[k][0]);
		DDX_Text(pDX, m++, General_Tab1.m_strIp_Source[k][0]);
		DDX_Text(pDX, m++, General_Tab1.m_nPort_Source[k][0]);

		DDX_Control(pDX, m++, General_Tab1.m_cmbSourceAdapter[k][1]);
		DDX_Text(pDX, m++, General_Tab1.m_strIp_Source[k][1]);
		DDX_Text(pDX, m++, General_Tab1.m_nPort_Source[k][1]);

		DDX_Control(pDX, m++, General_Tab1.m_cmbDestAdapter[k]);
		DDX_Text(pDX, m++, General_Tab1.m_strIp_Dest[k]);
		DDX_Text(pDX, m++, General_Tab1.m_nPort_Dest[k]);

		DDX_Text(pDX, m, General_Tab1.m_m_nDetection_Time[k]);
		m = m + 4;
		DDX_Control(pDX, m++, General_Tab1.m_PriStatus[k]);
		DDX_Control(pDX, m++, General_Tab1.m_SecondaryStatus[k]);
		DDX_Control(pDX, m++, General_Tab1.m_OtputStatus[k]);
		m++;
		DDX_Text(pDX, m++, General_Tab1.m_strChannel_Name[k]);
		DDV_MaxChars(pDX, General_Tab1.m_strChannel_Name[k], 20);
		DDX_Text(pDX, m, General_Tab1.m_nPresentDetection_Time[k]);

		m = 1171 + k;
		DDX_Control(pDX, m, General_Tab1.m_cmbPrgNo[k]);
	}

}

BEGIN_MESSAGE_MAP(CStream_Watch_Tab1, CDialog)
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(IDC_BUTTON_START_1, &CStream_Watch_Tab1::OnBnClickedButtonStart1)
	ON_BN_CLICKED(IDC_BUTTON_START_2, &CStream_Watch_Tab1::OnBnClickedButtonStart2)
	ON_BN_CLICKED(IDC_BUTTON_START_3, &CStream_Watch_Tab1::OnBnClickedButtonStart3)
	ON_BN_CLICKED(IDC_BUTTON_START_4, &CStream_Watch_Tab1::OnBnClickedButtonStart4)
	ON_BN_CLICKED(IDC_BUTTON_START_5, &CStream_Watch_Tab1::OnBnClickedButtonStart5)
	ON_BN_CLICKED(IDC_BUTTON_START_6, &CStream_Watch_Tab1::OnBnClickedButtonStart6)
	ON_BN_CLICKED(IDC_BUTTON_START_7, &CStream_Watch_Tab1::OnBnClickedButtonStart7)
	ON_BN_CLICKED(IDC_BUTTON_START_8, &CStream_Watch_Tab1::OnBnClickedButtonStart8)
	ON_BN_CLICKED(IDC_BUTTON_START_9, &CStream_Watch_Tab1::OnBnClickedButtonStart9)
	ON_BN_CLICKED(IDC_BUTTON_START_10, &CStream_Watch_Tab1::OnBnClickedButtonStart10)
	ON_BN_CLICKED(IDC_BUTTON_START_11, &CStream_Watch_Tab1::OnBnClickedButtonStart11)
	ON_BN_CLICKED(IDC_BUTTON_START_12, &CStream_Watch_Tab1::OnBnClickedButtonStart12)
	ON_BN_CLICKED(IDC_BUTTON_START_13, &CStream_Watch_Tab1::OnBnClickedButtonStart13)
	ON_BN_CLICKED(IDC_BUTTON_START_14, &CStream_Watch_Tab1::OnBnClickedButtonStart14)
	ON_BN_CLICKED(IDC_BUTTON_START_15, &CStream_Watch_Tab1::OnBnClickedButtonStart15)
	ON_BN_CLICKED(IDC_BUTTON_START_16, &CStream_Watch_Tab1::OnBnClickedButtonStart16)
	
	ON_BN_CLICKED(IDC_BUTTON_STOP_1, &CStream_Watch_Tab1::OnBnClickedButtonStop1)
	ON_BN_CLICKED(IDC_BUTTON_STOP_2, &CStream_Watch_Tab1::OnBnClickedButtonStop2)
	ON_BN_CLICKED(IDC_BUTTON_STOP_3, &CStream_Watch_Tab1::OnBnClickedButtonStop3)
	ON_BN_CLICKED(IDC_BUTTON_STOP_4, &CStream_Watch_Tab1::OnBnClickedButtonStop4)
	ON_BN_CLICKED(IDC_BUTTON_STOP_5, &CStream_Watch_Tab1::OnBnClickedButtonStop5)
	ON_BN_CLICKED(IDC_BUTTON_STOP_6, &CStream_Watch_Tab1::OnBnClickedButtonStop6)
	ON_BN_CLICKED(IDC_BUTTON_STOP_9, &CStream_Watch_Tab1::OnBnClickedButtonStop9)
	ON_BN_CLICKED(IDC_BUTTON_STOP_8, &CStream_Watch_Tab1::OnBnClickedButtonStop8)
	ON_BN_CLICKED(IDC_BUTTON_STOP_7, &CStream_Watch_Tab1::OnBnClickedButtonStop7)
	ON_BN_CLICKED(IDC_BUTTON_STOP_10, &CStream_Watch_Tab1::OnBnClickedButtonStop10)
	ON_BN_CLICKED(IDC_BUTTON_STOP_11, &CStream_Watch_Tab1::OnBnClickedButtonStop11)
	ON_BN_CLICKED(IDC_BUTTON_STOP_12, &CStream_Watch_Tab1::OnBnClickedButtonStop12)
	ON_BN_CLICKED(IDC_BUTTON_STOP_13, &CStream_Watch_Tab1::OnBnClickedButtonStop13)
	ON_BN_CLICKED(IDC_BUTTON_STOP_14, &CStream_Watch_Tab1::OnBnClickedButtonStop14)
	ON_BN_CLICKED(IDC_BUTTON_STOP_15, &CStream_Watch_Tab1::OnBnClickedButtonStop15)
	ON_BN_CLICKED(IDC_BUTTON_STOP_16, &CStream_Watch_Tab1::OnBnClickedButtonStop16)
	
	ON_BN_CLICKED(IDC_CHECK_1, &CStream_Watch_Tab1::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK_2, &CStream_Watch_Tab1::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK_3, &CStream_Watch_Tab1::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK_4, &CStream_Watch_Tab1::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK_5, &CStream_Watch_Tab1::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK_6, &CStream_Watch_Tab1::OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_CHECK_8, &CStream_Watch_Tab1::OnBnClickedCheck8)
	ON_BN_CLICKED(IDC_CHECK_7, &CStream_Watch_Tab1::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_CHECK_9, &CStream_Watch_Tab1::OnBnClickedCheck9)
	ON_BN_CLICKED(IDC_CHECK_10, &CStream_Watch_Tab1::OnBnClickedCheck10)
	ON_BN_CLICKED(IDC_CHECK_11, &CStream_Watch_Tab1::OnBnClickedCheck11)
	ON_BN_CLICKED(IDC_CHECK_12, &CStream_Watch_Tab1::OnBnClickedCheck12)
	ON_BN_CLICKED(IDC_CHECK_13, &CStream_Watch_Tab1::OnBnClickedCheck13)
	ON_BN_CLICKED(IDC_CHECK_14, &CStream_Watch_Tab1::OnBnClickedCheck14)
	ON_BN_CLICKED(IDC_CHECK_15, &CStream_Watch_Tab1::OnBnClickedCheck15)
	ON_BN_CLICKED(IDC_CHECK_16, &CStream_Watch_Tab1::OnBnClickedCheck16)

	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME1, &CStream_Watch_Tab1::OnEnKillfocusEditChannelName1)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME2, &CStream_Watch_Tab1::OnEnKillfocusEditChannelName2)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME3, &CStream_Watch_Tab1::OnEnKillfocusEditChannelName3)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME4, &CStream_Watch_Tab1::OnEnKillfocusEditChannelName4)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME5, &CStream_Watch_Tab1::OnEnKillfocusEditChannelName5)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME6, &CStream_Watch_Tab1::OnEnKillfocusEditChannelName6)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME7, &CStream_Watch_Tab1::OnEnKillfocusEditChannelName7)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME8, &CStream_Watch_Tab1::OnEnKillfocusEditChannelName8)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME9, &CStream_Watch_Tab1::OnEnKillfocusEditChannelName9)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME10, &CStream_Watch_Tab1::OnEnKillfocusEditChannelName10)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME11, &CStream_Watch_Tab1::OnEnKillfocusEditChannelName11)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME12, &CStream_Watch_Tab1::OnEnKillfocusEditChannelName12)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME13, &CStream_Watch_Tab1::OnEnKillfocusEditChannelName13)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME14, &CStream_Watch_Tab1::OnEnKillfocusEditChannelName14)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME15, &CStream_Watch_Tab1::OnEnKillfocusEditChannelName15)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME16, &CStream_Watch_Tab1::OnEnKillfocusEditChannelName16)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_1, &CStream_Watch_Tab1::OnBnClickedManualCheck1)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_2, &CStream_Watch_Tab1::OnBnClickedManualCheck2)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_3, &CStream_Watch_Tab1::OnBnClickedManualCheck3)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_4, &CStream_Watch_Tab1::OnBnClickedManualCheck4)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_5, &CStream_Watch_Tab1::OnBnClickedManualCheck5)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_6, &CStream_Watch_Tab1::OnBnClickedManualCheck6)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_7, &CStream_Watch_Tab1::OnBnClickedManualCheck7)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_8, &CStream_Watch_Tab1::OnBnClickedManualCheck8)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_9, &CStream_Watch_Tab1::OnBnClickedManualCheck9)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_10, &CStream_Watch_Tab1::OnBnClickedManualCheck10)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_11, &CStream_Watch_Tab1::OnBnClickedManualCheck11)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_12, &CStream_Watch_Tab1::OnBnClickedManualCheck12)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_13, &CStream_Watch_Tab1::OnBnClickedManualCheck13)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_14, &CStream_Watch_Tab1::OnBnClickedManualCheck14)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_15, &CStream_Watch_Tab1::OnBnClickedManualCheck15)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_16, &CStream_Watch_Tab1::OnBnClickedManualCheck16)
	ON_BN_CLICKED(IDC_CHECK_EME1, &CStream_Watch_Tab1::OnBnClickedCheckEme1)
	ON_BN_CLICKED(IDC_CHECK_EME2, &CStream_Watch_Tab1::OnBnClickedCheckEme2)
	ON_BN_CLICKED(IDC_CHECK_EME3, &CStream_Watch_Tab1::OnBnClickedCheckEme3)
	ON_BN_CLICKED(IDC_CHECK_EME4, &CStream_Watch_Tab1::OnBnClickedCheckEme4)
	ON_BN_CLICKED(IDC_CHECK_EME5, &CStream_Watch_Tab1::OnBnClickedCheckEme5)
	ON_BN_CLICKED(IDC_CHECK_EME6, &CStream_Watch_Tab1::OnBnClickedCheckEme6)
	ON_BN_CLICKED(IDC_CHECK_EME7, &CStream_Watch_Tab1::OnBnClickedCheckEme7)
	ON_BN_CLICKED(IDC_CHECK_EME8, &CStream_Watch_Tab1::OnBnClickedCheckEme8)
	ON_BN_CLICKED(IDC_CHECK_EME9, &CStream_Watch_Tab1::OnBnClickedCheckEme9)
	ON_BN_CLICKED(IDC_CHECK_EME10, &CStream_Watch_Tab1::OnBnClickedCheckEme10)
	ON_BN_CLICKED(IDC_CHECK_EME11, &CStream_Watch_Tab1::OnBnClickedCheckEme11)
	ON_BN_CLICKED(IDC_CHECK_EME12, &CStream_Watch_Tab1::OnBnClickedCheckEme12)
	ON_BN_CLICKED(IDC_CHECK_EME13, &CStream_Watch_Tab1::OnBnClickedCheckEme13)
	ON_BN_CLICKED(IDC_CHECK_EME14, &CStream_Watch_Tab1::OnBnClickedCheckEme14)
	ON_BN_CLICKED(IDC_CHECK_EME15, &CStream_Watch_Tab1::OnBnClickedCheckEme15)
	ON_BN_CLICKED(IDC_CHECK_EME16, &CStream_Watch_Tab1::OnBnClickedCheckEme16)
	ON_EN_CHANGE(IDC_WAITING_TIME_1, &CStream_Watch_Tab1::OnEnChangeWaitingTime1)
	END_MESSAGE_MAP()

BOOL CStream_Watch_Tab1::OnInitDialog()
{
	CDialog::OnInitDialog();
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	// Set small icon

	//ShowWindow(SW_MINIMIZE);

	// TODO: Add extra initialization here
	for(int i = 0; i < MAX_STREAM/3; i++)
	{
		General_Tab1.m_PriStatus[i].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
		General_Tab1.m_SecondaryStatus[i].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
		General_Tab1.m_OtputStatus[i].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
	}

	char ac[80];
	struct in_addr addr;
	struct hostent *HostName;
	size_t Temp;
	memset(&addr, 0, sizeof(in_addr));

	if(gethostname(ac, sizeof(ac)) != SOCKET_ERROR)
	{
		 HostName = gethostbyname(ac);
		for(int i = 0; HostName->h_addr_list[i] != NULL; i++)
		{
			memcpy(&addr, HostName->h_addr_list[i], sizeof(struct in_addr));
			#ifdef UNICODE
			char* strAddr = inet_ntoa(addr);
			wchar_t strWAddr[16];
			mbstowcs_s(&Temp, strWAddr, (size_t)16, strAddr, (size_t)16);
			for(int k = 0; k < MAX_STREAM/3 ; k++)
			{
				for(int i = 0; i < MAX_RECEIVER; i++)
				{
					General_Tab1.m_cmbSourceAdapter[k][i].AddString(strWAddr);
					General_Tab1.m_cmbSourceAdapter[k][i].SetCurSel(0);	
					General_Tab1.m_cmbSourceAdapter[k][i].EnableToolTips(1);
				#else	
					m_cmbSourceAdapter[i].AddString(inet_ntoa(addr));
				#endif
				}
				General_Tab1.m_cmbDestAdapter[k].AddString(strWAddr);
				General_Tab1.m_cmbDestAdapter[k].SetCurSel(0);
				General_Tab1.m_cmbDestAdapter[k].EnableToolTips(1);
			}
		}
	}
	
	for(int i = 0; i < MAX_STREAM/3; i++)
	{
		GetDlgItem(211 + (20 * i))->EnableWindow(false);
		GetDlgItem(212 + (20 * i))->EnableWindow(false);
	}

	m_pToolTip = new CToolTipCtrl;
	m_pToolTip->Create(this);

	for(int k = 0; k < MAX_STREAM/3; k++)
	{
		int m = 200 + (20 * k) + 1;
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Primary Source Adaptor Address"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Primary Source IP Address"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Primary Source Port Number"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Secondary Source Adaptor Address"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Secondary Source IP Address"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Secondary Source Port Number"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Destination Adaptor Address"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Destination IP Address"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Destination Port Number"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Enter Absent Detection Time Between 0 to 180000"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Click start Streaming"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Click stop Streaming"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Click to enable start"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Primary Source Status"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Secondary Source Status"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Output Status"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Click to Select Manual Secondary Input"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Enter Channel Name"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Enter Present Detection Time Between 0 to 10000"));
	}
	m_pToolTip->Activate(TRUE);

	UpdateData(false);

	return TRUE;  // return TRUE  unless you set the focus to a control
}
BOOL CStream_Watch_Tab1::PreTranslateMessage(MSG* pMsg)
{
	if (NULL != m_pToolTip)
		m_pToolTip->RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}
// CStream_Watch_Tab1 message handlers

#define ON_CLICK_START(XX)	void CStream_Watch_Tab1::OnBnClickedButtonStart##XX()	\
							{													\
								int nVal = XX;									\
								m_parent_dlg->StartEvent(nVal - 1);							\
							}
ON_CLICK_START(1);
ON_CLICK_START(2);
ON_CLICK_START(3);
ON_CLICK_START(4);
ON_CLICK_START(5);
ON_CLICK_START(6);
ON_CLICK_START(7);
ON_CLICK_START(8);
ON_CLICK_START(9);
ON_CLICK_START(10);
ON_CLICK_START(11);
ON_CLICK_START(12);
ON_CLICK_START(13);
ON_CLICK_START(14);
ON_CLICK_START(15);
ON_CLICK_START(16);

#define ON_CLICK_STOP(XX)	void CStream_Watch_Tab1::OnBnClickedButtonStop##XX()	\
							{													\
								int nVal = XX;									\
								m_parent_dlg->StopEvent(nVal - 1);							\
							}
ON_CLICK_STOP(1);
ON_CLICK_STOP(2);
ON_CLICK_STOP(3);
ON_CLICK_STOP(4);
ON_CLICK_STOP(5);
ON_CLICK_STOP(6);
ON_CLICK_STOP(7);
ON_CLICK_STOP(8);
ON_CLICK_STOP(9);
ON_CLICK_STOP(10);
ON_CLICK_STOP(11);
ON_CLICK_STOP(12);
ON_CLICK_STOP(13);
ON_CLICK_STOP(14);
ON_CLICK_STOP(15);
ON_CLICK_STOP(16);

#define ON_CLICK_CHECK(XX)	void CStream_Watch_Tab1::OnBnClickedCheck##XX()	\
							{											\
								int nVal = XX;							\
								m_parent_dlg->CheckedBoxEffect(nVal - 1,0);				\
							}
ON_CLICK_CHECK(1);
ON_CLICK_CHECK(2);
ON_CLICK_CHECK(3);
ON_CLICK_CHECK(4);
ON_CLICK_CHECK(5);
ON_CLICK_CHECK(6);
ON_CLICK_CHECK(7);
ON_CLICK_CHECK(8);
ON_CLICK_CHECK(9);
ON_CLICK_CHECK(10);
ON_CLICK_CHECK(11);
ON_CLICK_CHECK(12);
ON_CLICK_CHECK(13);
ON_CLICK_CHECK(14);
ON_CLICK_CHECK(15);
ON_CLICK_CHECK(16);

#define ON_KILLFOCUS_CHANNELNAME(XX)	void CStream_Watch_Tab1::OnEnKillfocusEditChannelName##XX()	\
										{											\
											int nVal = XX;							\
											m_parent_dlg->KillFocusEffect(nVal - 1);				\
										}
ON_KILLFOCUS_CHANNELNAME(1);
ON_KILLFOCUS_CHANNELNAME(2);
ON_KILLFOCUS_CHANNELNAME(3);
ON_KILLFOCUS_CHANNELNAME(4);
ON_KILLFOCUS_CHANNELNAME(5);
ON_KILLFOCUS_CHANNELNAME(6);
ON_KILLFOCUS_CHANNELNAME(7);
ON_KILLFOCUS_CHANNELNAME(8);
ON_KILLFOCUS_CHANNELNAME(9);
ON_KILLFOCUS_CHANNELNAME(10);
ON_KILLFOCUS_CHANNELNAME(11);
ON_KILLFOCUS_CHANNELNAME(12);
ON_KILLFOCUS_CHANNELNAME(13);
ON_KILLFOCUS_CHANNELNAME(14);
ON_KILLFOCUS_CHANNELNAME(15);
ON_KILLFOCUS_CHANNELNAME(16);

#define ON_CLICK_MANUAL_CHECK(XX)	void CStream_Watch_Tab1::OnBnClickedManualCheck##XX()	\
									{											\
										int nVal = XX;							\
										m_parent_dlg->ManualCheckBoxEffect(nVal - 1,2);		\
									}
ON_CLICK_MANUAL_CHECK(1);
ON_CLICK_MANUAL_CHECK(2);
ON_CLICK_MANUAL_CHECK(3);
ON_CLICK_MANUAL_CHECK(4);
ON_CLICK_MANUAL_CHECK(5);
ON_CLICK_MANUAL_CHECK(6);
ON_CLICK_MANUAL_CHECK(7);
ON_CLICK_MANUAL_CHECK(8);
ON_CLICK_MANUAL_CHECK(9);
ON_CLICK_MANUAL_CHECK(10);
ON_CLICK_MANUAL_CHECK(11);
ON_CLICK_MANUAL_CHECK(12);
ON_CLICK_MANUAL_CHECK(13);
ON_CLICK_MANUAL_CHECK(14);
ON_CLICK_MANUAL_CHECK(15);
ON_CLICK_MANUAL_CHECK(16);

#define ON_CLICK_EME_CHECK(XX)	void CStream_Watch_Tab1::OnBnClickedCheckEme##XX()	\
							{											\
								int nVal = XX;							\
								m_parent_dlg->EmeCheckBoxEffect(nVal - 1,2);		\
							}

ON_CLICK_EME_CHECK(1);
ON_CLICK_EME_CHECK(2);
ON_CLICK_EME_CHECK(3);
ON_CLICK_EME_CHECK(4);
ON_CLICK_EME_CHECK(5);
ON_CLICK_EME_CHECK(6);
ON_CLICK_EME_CHECK(7);
ON_CLICK_EME_CHECK(8);
ON_CLICK_EME_CHECK(9);
ON_CLICK_EME_CHECK(10);
ON_CLICK_EME_CHECK(11);
ON_CLICK_EME_CHECK(12);
ON_CLICK_EME_CHECK(13);
ON_CLICK_EME_CHECK(14);
ON_CLICK_EME_CHECK(15);
ON_CLICK_EME_CHECK(16);

void CStream_Watch_Tab1::OnEnChangeWaitingTime1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
             
