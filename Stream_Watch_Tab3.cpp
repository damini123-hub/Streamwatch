// Stream_Watch_Tab3.cpp : implementation file
//
#include "stdafx.h"
#include "StreamWatch.h"
#include "Stream_Watch_Tab3.h"

// CStream_Watch_Tab3 dialog

IMPLEMENT_DYNAMIC(CStream_Watch_Tab3, CDialog)

CStream_Watch_Tab3::CStream_Watch_Tab3(CWnd* pParent /*=NULL*/, CStream_WatchDlg* parent_Dlg)
	: CDialog(CStream_Watch_Tab3::IDD, pParent)
	,m_parent_dlg(parent_Dlg)
{
	for(int k = 0; k < MAX_STREAM/3; k++)
	{
		for(int i = 0; i < MAX_RECEIVER; i++)
		{
			General_Tab3.m_strNic_Source[k][i] = _T("");
			General_Tab3.m_strIp_Source[k][i] = _T("");
			//General_Tab3.m_nPort_Source[k][i] = 8001 + i + (2 * k) ;
			General_Tab3.m_nPort_Source[k][i] = 0 ;
		}
		General_Tab3.m_strNic_Dest[k] = _T("");
		General_Tab3.m_strIp_Dest[k] = _T("");
		General_Tab3.m_nPort_Dest[k] = 0;
		General_Tab3.m_m_nDetection_Time[k] = 0;
		General_Tab3.m_nPresentDetection_Time[k] = 0;
		General_Tab3.m_strChannel_Name[k] = _T("");
	}
}

CStream_Watch_Tab3::~CStream_Watch_Tab3()
{
	if(m_pToolTip != NULL)
	{
		SAFE_DELETE(m_pToolTip);
	}
}

void CStream_Watch_Tab3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	for(int k = 0; k < MAX_STREAM/3; k++)
	{
		int m = 840 + (20 * k) + 1;

		DDX_Control(pDX, m++, General_Tab3.m_cmbSourceAdapter[k][0]);
		DDX_Text(pDX, m++, General_Tab3.m_strIp_Source[k][0]);
		DDX_Text(pDX, m++, General_Tab3.m_nPort_Source[k][0]);

		DDX_Control(pDX, m++, General_Tab3.m_cmbSourceAdapter[k][1]);
		DDX_Text(pDX, m++, General_Tab3.m_strIp_Source[k][1]);
		DDX_Text(pDX, m++, General_Tab3.m_nPort_Source[k][1]);

		DDX_Control(pDX, m++, General_Tab3.m_cmbDestAdapter[k]);
		DDX_Text(pDX, m++, General_Tab3.m_strIp_Dest[k]);
		DDX_Text(pDX, m++, General_Tab3.m_nPort_Dest[k]);

		DDX_Text(pDX, m, General_Tab3.m_m_nDetection_Time[k]);
		m = m + 4;
		DDX_Control(pDX, m++, General_Tab3.m_PriStatus[k]);
		DDX_Control(pDX, m++, General_Tab3.m_SecondaryStatus[k]);
		DDX_Control(pDX, m++, General_Tab3.m_OtputStatus[k]);
		m++;
		DDX_Text(pDX, m++,General_Tab3.m_strChannel_Name[k]);
		DDV_MaxChars(pDX, General_Tab3.m_strChannel_Name[k], 20);
		DDX_Text(pDX, m, General_Tab3.m_nPresentDetection_Time[k]);

		m = 1203 + k;
		DDX_Control(pDX, m, General_Tab3.m_cmbPrgNo[k]);
	}
}


BEGIN_MESSAGE_MAP(CStream_Watch_Tab3, CDialog)

	ON_BN_CLICKED(IDC_BUTTON_START_33, &CStream_Watch_Tab3::OnBnClickedButtonStart33)
	ON_BN_CLICKED(IDC_BUTTON_START_34, &CStream_Watch_Tab3::OnBnClickedButtonStart34)
	ON_BN_CLICKED(IDC_BUTTON_START_35, &CStream_Watch_Tab3::OnBnClickedButtonStart35)
	ON_BN_CLICKED(IDC_BUTTON_START_36, &CStream_Watch_Tab3::OnBnClickedButtonStart36)
	ON_BN_CLICKED(IDC_BUTTON_START_37, &CStream_Watch_Tab3::OnBnClickedButtonStart37)
	ON_BN_CLICKED(IDC_BUTTON_START_38, &CStream_Watch_Tab3::OnBnClickedButtonStart38)
	ON_BN_CLICKED(IDC_BUTTON_START_39, &CStream_Watch_Tab3::OnBnClickedButtonStart39)
	ON_BN_CLICKED(IDC_BUTTON_START_40, &CStream_Watch_Tab3::OnBnClickedButtonStart40)
	ON_BN_CLICKED(IDC_BUTTON_START_41, &CStream_Watch_Tab3::OnBnClickedButtonStart41)
	ON_BN_CLICKED(IDC_BUTTON_START_42, &CStream_Watch_Tab3::OnBnClickedButtonStart42)
	ON_BN_CLICKED(IDC_BUTTON_START_43, &CStream_Watch_Tab3::OnBnClickedButtonStart43)
	ON_BN_CLICKED(IDC_BUTTON_START_44, &CStream_Watch_Tab3::OnBnClickedButtonStart44)
	ON_BN_CLICKED(IDC_BUTTON_START_45, &CStream_Watch_Tab3::OnBnClickedButtonStart45)
	ON_BN_CLICKED(IDC_BUTTON_START_46, &CStream_Watch_Tab3::OnBnClickedButtonStart46)
	ON_BN_CLICKED(IDC_BUTTON_START_47, &CStream_Watch_Tab3::OnBnClickedButtonStart47)
	ON_BN_CLICKED(IDC_BUTTON_START_48, &CStream_Watch_Tab3::OnBnClickedButtonStart48)
	ON_BN_CLICKED(IDC_BUTTON_STOP_33, &CStream_Watch_Tab3::OnBnClickedButtonStop33)
	ON_BN_CLICKED(IDC_BUTTON_STOP_34, &CStream_Watch_Tab3::OnBnClickedButtonStop34)
	ON_BN_CLICKED(IDC_BUTTON_STOP_35, &CStream_Watch_Tab3::OnBnClickedButtonStop35)
	ON_BN_CLICKED(IDC_BUTTON_STOP_36, &CStream_Watch_Tab3::OnBnClickedButtonStop36)
	ON_BN_CLICKED(IDC_BUTTON_STOP_37, &CStream_Watch_Tab3::OnBnClickedButtonStop37)
	ON_BN_CLICKED(IDC_BUTTON_STOP_38, &CStream_Watch_Tab3::OnBnClickedButtonStop38)
	ON_BN_CLICKED(IDC_BUTTON_STOP_39, &CStream_Watch_Tab3::OnBnClickedButtonStop39)
	ON_BN_CLICKED(IDC_BUTTON_STOP_40, &CStream_Watch_Tab3::OnBnClickedButtonStop40)
	ON_BN_CLICKED(IDC_BUTTON_STOP_41, &CStream_Watch_Tab3::OnBnClickedButtonStop41)
	ON_BN_CLICKED(IDC_BUTTON_STOP_42, &CStream_Watch_Tab3::OnBnClickedButtonStop42)
	ON_BN_CLICKED(IDC_BUTTON_STOP_43, &CStream_Watch_Tab3::OnBnClickedButtonStop43)
	ON_BN_CLICKED(IDC_BUTTON_STOP_44, &CStream_Watch_Tab3::OnBnClickedButtonStop44)
	ON_BN_CLICKED(IDC_BUTTON_STOP_45, &CStream_Watch_Tab3::OnBnClickedButtonStop45)
	ON_BN_CLICKED(IDC_BUTTON_STOP_46, &CStream_Watch_Tab3::OnBnClickedButtonStop46)
	ON_BN_CLICKED(IDC_BUTTON_STOP_47, &CStream_Watch_Tab3::OnBnClickedButtonStop47)
	ON_BN_CLICKED(IDC_BUTTON_STOP_48, &CStream_Watch_Tab3::OnBnClickedButtonStop48)
	ON_BN_CLICKED(IDC_CHECK_33, &CStream_Watch_Tab3::OnBnClickedCheck33)
	ON_BN_CLICKED(IDC_CHECK_34, &CStream_Watch_Tab3::OnBnClickedCheck34)
	ON_BN_CLICKED(IDC_CHECK_35, &CStream_Watch_Tab3::OnBnClickedCheck35)
	ON_BN_CLICKED(IDC_CHECK_36, &CStream_Watch_Tab3::OnBnClickedCheck36)
	ON_BN_CLICKED(IDC_CHECK_37, &CStream_Watch_Tab3::OnBnClickedCheck37)
	ON_BN_CLICKED(IDC_CHECK_38, &CStream_Watch_Tab3::OnBnClickedCheck38)
	ON_BN_CLICKED(IDC_CHECK_39, &CStream_Watch_Tab3::OnBnClickedCheck39)
	ON_BN_CLICKED(IDC_CHECK_40, &CStream_Watch_Tab3::OnBnClickedCheck40)
	ON_BN_CLICKED(IDC_CHECK_41, &CStream_Watch_Tab3::OnBnClickedCheck41)
	ON_BN_CLICKED(IDC_CHECK_42, &CStream_Watch_Tab3::OnBnClickedCheck42)
	ON_BN_CLICKED(IDC_CHECK_43, &CStream_Watch_Tab3::OnBnClickedCheck43)
	ON_BN_CLICKED(IDC_CHECK_44, &CStream_Watch_Tab3::OnBnClickedCheck44)
	ON_BN_CLICKED(IDC_CHECK_45, &CStream_Watch_Tab3::OnBnClickedCheck45)
	ON_BN_CLICKED(IDC_CHECK_46, &CStream_Watch_Tab3::OnBnClickedCheck46)
	ON_BN_CLICKED(IDC_CHECK_47, &CStream_Watch_Tab3::OnBnClickedCheck47)
	ON_BN_CLICKED(IDC_CHECK_48, &CStream_Watch_Tab3::OnBnClickedCheck48)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME33, &CStream_Watch_Tab3::OnEnKillfocusEditChannelName33)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME34, &CStream_Watch_Tab3::OnEnKillfocusEditChannelName34)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME35, &CStream_Watch_Tab3::OnEnKillfocusEditChannelName35)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME36, &CStream_Watch_Tab3::OnEnKillfocusEditChannelName36)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME37, &CStream_Watch_Tab3::OnEnKillfocusEditChannelName37)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME38, &CStream_Watch_Tab3::OnEnKillfocusEditChannelName38)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME39, &CStream_Watch_Tab3::OnEnKillfocusEditChannelName39)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME40, &CStream_Watch_Tab3::OnEnKillfocusEditChannelName40)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME41, &CStream_Watch_Tab3::OnEnKillfocusEditChannelName41)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME42, &CStream_Watch_Tab3::OnEnKillfocusEditChannelName42)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME43, &CStream_Watch_Tab3::OnEnKillfocusEditChannelName43)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME44, &CStream_Watch_Tab3::OnEnKillfocusEditChannelName44)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME45, &CStream_Watch_Tab3::OnEnKillfocusEditChannelName45)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME46, &CStream_Watch_Tab3::OnEnKillfocusEditChannelName46)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME47, &CStream_Watch_Tab3::OnEnKillfocusEditChannelName47)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME48, &CStream_Watch_Tab3::OnEnKillfocusEditChannelName48)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_33, &CStream_Watch_Tab3::OnBnClickedManualCheck33)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_34, &CStream_Watch_Tab3::OnBnClickedManualCheck34)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_35, &CStream_Watch_Tab3::OnBnClickedManualCheck35)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_36, &CStream_Watch_Tab3::OnBnClickedManualCheck36)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_37, &CStream_Watch_Tab3::OnBnClickedManualCheck37)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_38, &CStream_Watch_Tab3::OnBnClickedManualCheck38)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_39, &CStream_Watch_Tab3::OnBnClickedManualCheck39)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_40, &CStream_Watch_Tab3::OnBnClickedManualCheck40)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_41, &CStream_Watch_Tab3::OnBnClickedManualCheck41)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_42, &CStream_Watch_Tab3::OnBnClickedManualCheck42)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_43, &CStream_Watch_Tab3::OnBnClickedManualCheck43)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_44, &CStream_Watch_Tab3::OnBnClickedManualCheck44)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_45, &CStream_Watch_Tab3::OnBnClickedManualCheck45)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_46, &CStream_Watch_Tab3::OnBnClickedManualCheck46)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_47, &CStream_Watch_Tab3::OnBnClickedManualCheck47)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_48, &CStream_Watch_Tab3::OnBnClickedManualCheck48)
	ON_BN_CLICKED(IDC_CHECK_EME33, &CStream_Watch_Tab3::OnBnClickedCheckEme33)
	ON_BN_CLICKED(IDC_CHECK_EME34, &CStream_Watch_Tab3::OnBnClickedCheckEme34)
	ON_BN_CLICKED(IDC_CHECK_EME35, &CStream_Watch_Tab3::OnBnClickedCheckEme35)
	ON_BN_CLICKED(IDC_CHECK_EME36, &CStream_Watch_Tab3::OnBnClickedCheckEme36)
	ON_BN_CLICKED(IDC_CHECK_EME37, &CStream_Watch_Tab3::OnBnClickedCheckEme37)
	ON_BN_CLICKED(IDC_CHECK_EME38, &CStream_Watch_Tab3::OnBnClickedCheckEme38)
	ON_BN_CLICKED(IDC_CHECK_EME39, &CStream_Watch_Tab3::OnBnClickedCheckEme39)
	ON_BN_CLICKED(IDC_CHECK_EME40, &CStream_Watch_Tab3::OnBnClickedCheckEme40)
	ON_BN_CLICKED(IDC_CHECK_EME41, &CStream_Watch_Tab3::OnBnClickedCheckEme41)
	ON_BN_CLICKED(IDC_CHECK_EME42, &CStream_Watch_Tab3::OnBnClickedCheckEme42)
	ON_BN_CLICKED(IDC_CHECK_EME43, &CStream_Watch_Tab3::OnBnClickedCheckEme43)
	ON_BN_CLICKED(IDC_CHECK_EME44, &CStream_Watch_Tab3::OnBnClickedCheckEme44)
	ON_BN_CLICKED(IDC_CHECK_EME45, &CStream_Watch_Tab3::OnBnClickedCheckEme45)
	ON_BN_CLICKED(IDC_CHECK_EME46, &CStream_Watch_Tab3::OnBnClickedCheckEme46)
	ON_BN_CLICKED(IDC_CHECK_EME47, &CStream_Watch_Tab3::OnBnClickedCheckEme47)
	ON_BN_CLICKED(IDC_CHECK_EME48, &CStream_Watch_Tab3::OnBnClickedCheckEme48)
END_MESSAGE_MAP()


BOOL CStream_Watch_Tab3::OnInitDialog()
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

	//ShowWindow(SW_MINIMIZE);
	// TODO: Add extra initialization here

	for(int i = 0; i < MAX_STREAM/3; i++)
	{
		General_Tab3.m_PriStatus[i].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
		General_Tab3.m_SecondaryStatus[i].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
		General_Tab3.m_OtputStatus[i].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
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
					General_Tab3.m_cmbSourceAdapter[k][i].AddString(strWAddr);
					General_Tab3.m_cmbSourceAdapter[k][i].SetCurSel(0);	
				#else	
					m_cmbSourceAdapter[i].AddString(inet_ntoa(addr));
				#endif
				}
				General_Tab3.m_cmbDestAdapter[k].AddString(strWAddr);
				General_Tab3.m_cmbDestAdapter[k].SetCurSel(0);	
			}
		}
	}
	for(int i = 0; i < MAX_STREAM/3; i++)
	{
		GetDlgItem(851 + (20 * i))->EnableWindow(false);
		GetDlgItem(852 + (20 * i))->EnableWindow(false);
	}

	m_pToolTip = new CToolTipCtrl;
	m_pToolTip->Create(this);

	for(int k = 0; k < MAX_STREAM/3; k++)
	{
		int m = 840 + (20 * k) + 1;

		m_pToolTip->AddTool(GetDlgItem(m++),_T("Primary Source Adaptor Address"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Primary Source IP Address"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Primary Source Port Number"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Secondary Source Adaptor Address"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Secondary Source IP Address"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Secondary Source Port Number"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Destination Adaptor Address"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Destination IP Address"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Destination Port Number"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Enter Absent Detection Time Between 16 to 180000"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Click start Streaming"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Click stop Streaming"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Click to enable start"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Primary Source Status"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Secondary Source Status"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Output Status"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Click to Select Manual Secondary Input"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Enter Channel Name"));
		m_pToolTip->AddTool(GetDlgItem(m++),_T("Enter Present Detection Time Between 500 to 10000"));
	}
    m_pToolTip->Activate(TRUE);

	UpdateData(false);

	return TRUE;  // return TRUE  unless you set the focus to a control
}
BOOL CStream_Watch_Tab3::PreTranslateMessage(MSG* pMsg)
{
	if (NULL != m_pToolTip)
		m_pToolTip->RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}

#define ON_CLICK_START(XX)	void CStream_Watch_Tab3::OnBnClickedButtonStart##XX()	\
							{													\
								int nVal = XX;									\
								m_parent_dlg->StartEvent(nVal - 1);							\
							}
ON_CLICK_START(33);
ON_CLICK_START(34);
ON_CLICK_START(35);
ON_CLICK_START(36);
ON_CLICK_START(37);
ON_CLICK_START(38);
ON_CLICK_START(39);
ON_CLICK_START(40);
ON_CLICK_START(41);
ON_CLICK_START(42);
ON_CLICK_START(43);
ON_CLICK_START(44);
ON_CLICK_START(45);
ON_CLICK_START(46);
ON_CLICK_START(47);
ON_CLICK_START(48);

#define ON_CLICK_STOP(XX)	void CStream_Watch_Tab3::OnBnClickedButtonStop##XX()	\
							{													\
								int nVal = XX;									\
								m_parent_dlg->StopEvent(nVal - 1);							\
							}
ON_CLICK_STOP(33);
ON_CLICK_STOP(34);
ON_CLICK_STOP(35);
ON_CLICK_STOP(36);
ON_CLICK_STOP(37);
ON_CLICK_STOP(38);
ON_CLICK_STOP(39);
ON_CLICK_STOP(40);
ON_CLICK_STOP(41);
ON_CLICK_STOP(42);
ON_CLICK_STOP(43);
ON_CLICK_STOP(44);
ON_CLICK_STOP(45);
ON_CLICK_STOP(46);
ON_CLICK_STOP(47);
ON_CLICK_STOP(48);

#define ON_CLICK_CHECK(XX)	void CStream_Watch_Tab3::OnBnClickedCheck##XX()	\
							{											\
								int nVal = XX;							\
								m_parent_dlg->CheckedBoxEffect(nVal - 1,2);				\
							}
ON_CLICK_CHECK(33);
ON_CLICK_CHECK(34);
ON_CLICK_CHECK(35);
ON_CLICK_CHECK(36);
ON_CLICK_CHECK(37);
ON_CLICK_CHECK(38);
ON_CLICK_CHECK(39);
ON_CLICK_CHECK(40);
ON_CLICK_CHECK(41);
ON_CLICK_CHECK(42);
ON_CLICK_CHECK(43);
ON_CLICK_CHECK(44);
ON_CLICK_CHECK(45);
ON_CLICK_CHECK(46);
ON_CLICK_CHECK(47);
ON_CLICK_CHECK(48);

#define ON_KILLFOCUS_CHANNELNAME(XX)	void CStream_Watch_Tab3::OnEnKillfocusEditChannelName##XX()	\
										{											\
											int nVal = XX;							\
											m_parent_dlg->KillFocusEffect(nVal - 1);				\
										}

ON_KILLFOCUS_CHANNELNAME(33);
ON_KILLFOCUS_CHANNELNAME(34);
ON_KILLFOCUS_CHANNELNAME(35);
ON_KILLFOCUS_CHANNELNAME(36);
ON_KILLFOCUS_CHANNELNAME(37);
ON_KILLFOCUS_CHANNELNAME(38);
ON_KILLFOCUS_CHANNELNAME(39);
ON_KILLFOCUS_CHANNELNAME(40);
ON_KILLFOCUS_CHANNELNAME(41);
ON_KILLFOCUS_CHANNELNAME(42);
ON_KILLFOCUS_CHANNELNAME(43);
ON_KILLFOCUS_CHANNELNAME(44);
ON_KILLFOCUS_CHANNELNAME(45);
ON_KILLFOCUS_CHANNELNAME(46);
ON_KILLFOCUS_CHANNELNAME(47);
ON_KILLFOCUS_CHANNELNAME(48);

#define ON_CLICK_MANUAL_CHECK(XX)	void CStream_Watch_Tab3::OnBnClickedManualCheck##XX()	\
							{											\
								int nVal = XX;							\
								m_parent_dlg->ManualCheckBoxEffect(nVal - 1,2);		\
							}

ON_CLICK_MANUAL_CHECK(33);
ON_CLICK_MANUAL_CHECK(34);
ON_CLICK_MANUAL_CHECK(35);
ON_CLICK_MANUAL_CHECK(36);
ON_CLICK_MANUAL_CHECK(37);
ON_CLICK_MANUAL_CHECK(38);
ON_CLICK_MANUAL_CHECK(39);
ON_CLICK_MANUAL_CHECK(40);
ON_CLICK_MANUAL_CHECK(41);
ON_CLICK_MANUAL_CHECK(42);
ON_CLICK_MANUAL_CHECK(43);
ON_CLICK_MANUAL_CHECK(44);
ON_CLICK_MANUAL_CHECK(45);
ON_CLICK_MANUAL_CHECK(46);
ON_CLICK_MANUAL_CHECK(47);
ON_CLICK_MANUAL_CHECK(48);

#define ON_CLICK_EME_CHECK(XX)	void CStream_Watch_Tab3::OnBnClickedCheckEme##XX()	\
							{											\
								int nVal = XX;							\
								m_parent_dlg->EmeCheckBoxEffect(nVal - 1,2);		\
							}

ON_CLICK_EME_CHECK(33);
ON_CLICK_EME_CHECK(34);
ON_CLICK_EME_CHECK(35);
ON_CLICK_EME_CHECK(36);
ON_CLICK_EME_CHECK(37);
ON_CLICK_EME_CHECK(38);
ON_CLICK_EME_CHECK(39);
ON_CLICK_EME_CHECK(40);
ON_CLICK_EME_CHECK(41);
ON_CLICK_EME_CHECK(42);
ON_CLICK_EME_CHECK(43);
ON_CLICK_EME_CHECK(44);
ON_CLICK_EME_CHECK(45);
ON_CLICK_EME_CHECK(46);
ON_CLICK_EME_CHECK(47);
ON_CLICK_EME_CHECK(48);

