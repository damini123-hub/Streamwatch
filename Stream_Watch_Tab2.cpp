// Stream_Watch_Tab2.cpp : implementation file
//
#include "stdafx.h"
#include "StreamWatch.h"
#include "Stream_Watch_Tab2.h"

// CStream_Watch_Tab2 dialog
extern CStream StreamWatch[MAX_STREAM];

//#define IDC_COMBO_SOURCE_PRIM_ IDC_COMBO_SOURCE_PRIM_
#define ResourceBuild(x,y) x##y

IMPLEMENT_DYNAMIC(CStream_Watch_Tab2, CDialog)

CStream_Watch_Tab2::CStream_Watch_Tab2(CWnd* pParent /*=NULL*/, CStream_WatchDlg* parent_Dlg)
	: CDialog(CStream_Watch_Tab2::IDD, pParent)
	,m_parent_dlg(parent_Dlg)
{
	for(int k = 0; k < MAX_STREAM/3; k++)
	{
		for(int i = 0; i < MAX_RECEIVER; i++)
		{
			General_Tab2.m_strNic_Source[k][i] = _T("");
			General_Tab2.m_strIp_Source[k][i] = _T("");
			//General_Tab2.m_nPort_Source[k][i] = 8001 + i + (2 * k) ;
			General_Tab2.m_nPort_Source[k][i] = 0;
		}
		General_Tab2.m_strNic_Dest[k] = _T("");
		General_Tab2.m_strIp_Dest[k] = _T("");
		General_Tab2.m_nPort_Dest[k] = 0;
		General_Tab2.m_m_nDetection_Time[k] = 0;
		General_Tab2.m_nPresentDetection_Time[k] = 0;
		General_Tab2.m_strChannel_Name[k] = _T("");
	}
}

CStream_Watch_Tab2::~CStream_Watch_Tab2()
{
	if(m_pToolTip != NULL)
	{
		SAFE_DELETE(m_pToolTip);
	}
}

void CStream_Watch_Tab2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
		for(int k = 0; k < MAX_STREAM/3; k++)
	{
		int m = 520 + (20 * k) + 1;

		DDX_Control(pDX, m++, General_Tab2.m_cmbSourceAdapter[k][0]);
		DDX_Text(pDX, m++, General_Tab2.m_strIp_Source[k][0]);
		DDX_Text(pDX, m++, General_Tab2.m_nPort_Source[k][0]);

		DDX_Control(pDX, m++, General_Tab2.m_cmbSourceAdapter[k][1]);
		DDX_Text(pDX, m++, General_Tab2.m_strIp_Source[k][1]);
		DDX_Text(pDX, m++, General_Tab2.m_nPort_Source[k][1]);

		DDX_Control(pDX, m++, General_Tab2.m_cmbDestAdapter[k]);
		DDX_Text(pDX, m++, General_Tab2.m_strIp_Dest[k]);
		DDX_Text(pDX, m++, General_Tab2.m_nPort_Dest[k]);

		DDX_Text(pDX, m, General_Tab2.m_m_nDetection_Time[k]);
		m = m + 4;
		DDX_Control(pDX, m++, General_Tab2.m_PriStatus[k]);
		DDX_Control(pDX, m++, General_Tab2.m_SecondaryStatus[k]);
		DDX_Control(pDX, m++, General_Tab2.m_OtputStatus[k]);
		m++;
		DDX_Text(pDX, m++,General_Tab2.m_strChannel_Name[k]);
		DDV_MaxChars(pDX, General_Tab2.m_strChannel_Name[k], 20);
		DDX_Text(pDX, m, General_Tab2.m_nPresentDetection_Time[k]);

		m = 1187 + k;
		DDX_Control(pDX, m, General_Tab2.m_cmbPrgNo[k]);
	}
}

BEGIN_MESSAGE_MAP(CStream_Watch_Tab2, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_START_17, &CStream_Watch_Tab2::OnBnClickedButtonStart17)
	ON_BN_CLICKED(IDC_BUTTON_START_18, &CStream_Watch_Tab2::OnBnClickedButtonStart18)
	ON_BN_CLICKED(IDC_BUTTON_START_19, &CStream_Watch_Tab2::OnBnClickedButtonStart19)
	ON_BN_CLICKED(IDC_BUTTON_START_20, &CStream_Watch_Tab2::OnBnClickedButtonStart20)
	ON_BN_CLICKED(IDC_BUTTON_START_21, &CStream_Watch_Tab2::OnBnClickedButtonStart21)
	ON_BN_CLICKED(IDC_BUTTON_START_22, &CStream_Watch_Tab2::OnBnClickedButtonStart22)
	ON_BN_CLICKED(IDC_BUTTON_START_23, &CStream_Watch_Tab2::OnBnClickedButtonStart23)
	ON_BN_CLICKED(IDC_BUTTON_START_24, &CStream_Watch_Tab2::OnBnClickedButtonStart24)
	ON_BN_CLICKED(IDC_BUTTON_START_25, &CStream_Watch_Tab2::OnBnClickedButtonStart25)
	ON_BN_CLICKED(IDC_BUTTON_START_26, &CStream_Watch_Tab2::OnBnClickedButtonStart26)
	ON_BN_CLICKED(IDC_BUTTON_START_27, &CStream_Watch_Tab2::OnBnClickedButtonStart27)
	ON_BN_CLICKED(IDC_BUTTON_START_28, &CStream_Watch_Tab2::OnBnClickedButtonStart28)
	ON_BN_CLICKED(IDC_BUTTON_START_29, &CStream_Watch_Tab2::OnBnClickedButtonStart29)
	ON_BN_CLICKED(IDC_BUTTON_START_30, &CStream_Watch_Tab2::OnBnClickedButtonStart30)
	ON_BN_CLICKED(IDC_BUTTON_START_31, &CStream_Watch_Tab2::OnBnClickedButtonStart31)
	ON_BN_CLICKED(IDC_BUTTON_START_32, &CStream_Watch_Tab2::OnBnClickedButtonStart32)
	ON_BN_CLICKED(IDC_BUTTON_STOP_17, &CStream_Watch_Tab2::OnBnClickedButtonStop17)
	ON_BN_CLICKED(IDC_BUTTON_STOP_18, &CStream_Watch_Tab2::OnBnClickedButtonStop18)
	ON_BN_CLICKED(IDC_BUTTON_STOP_19, &CStream_Watch_Tab2::OnBnClickedButtonStop19)
	ON_BN_CLICKED(IDC_BUTTON_STOP_20, &CStream_Watch_Tab2::OnBnClickedButtonStop20)
	ON_BN_CLICKED(IDC_BUTTON_STOP_21, &CStream_Watch_Tab2::OnBnClickedButtonStop21)
	ON_BN_CLICKED(IDC_BUTTON_STOP_22, &CStream_Watch_Tab2::OnBnClickedButtonStop22)
	ON_BN_CLICKED(IDC_BUTTON_STOP_23, &CStream_Watch_Tab2::OnBnClickedButtonStop23)
	ON_BN_CLICKED(IDC_BUTTON_STOP_24, &CStream_Watch_Tab2::OnBnClickedButtonStop24)
	ON_BN_CLICKED(IDC_BUTTON_STOP_25, &CStream_Watch_Tab2::OnBnClickedButtonStop25)
	ON_BN_CLICKED(IDC_BUTTON_STOP_26, &CStream_Watch_Tab2::OnBnClickedButtonStop26)
	ON_BN_CLICKED(IDC_BUTTON_STOP_27, &CStream_Watch_Tab2::OnBnClickedButtonStop27)
	ON_BN_CLICKED(IDC_BUTTON_STOP_28, &CStream_Watch_Tab2::OnBnClickedButtonStop28)
	ON_BN_CLICKED(IDC_BUTTON_STOP_29, &CStream_Watch_Tab2::OnBnClickedButtonStop29)
	ON_BN_CLICKED(IDC_BUTTON_STOP_30, &CStream_Watch_Tab2::OnBnClickedButtonStop30)
	ON_BN_CLICKED(IDC_BUTTON_STOP_31, &CStream_Watch_Tab2::OnBnClickedButtonStop31)
	ON_BN_CLICKED(IDC_BUTTON_STOP_32, &CStream_Watch_Tab2::OnBnClickedButtonStop32)
	ON_BN_CLICKED(IDC_CHECK_17, &CStream_Watch_Tab2::OnBnClickedCheck17)
	ON_BN_CLICKED(IDC_CHECK_18, &CStream_Watch_Tab2::OnBnClickedCheck18)
	ON_BN_CLICKED(IDC_CHECK_19, &CStream_Watch_Tab2::OnBnClickedCheck19)
	ON_BN_CLICKED(IDC_CHECK_20, &CStream_Watch_Tab2::OnBnClickedCheck20)
	ON_BN_CLICKED(IDC_CHECK_21, &CStream_Watch_Tab2::OnBnClickedCheck21)
	ON_BN_CLICKED(IDC_CHECK_22, &CStream_Watch_Tab2::OnBnClickedCheck22)
	ON_BN_CLICKED(IDC_CHECK_23, &CStream_Watch_Tab2::OnBnClickedCheck23)
	ON_BN_CLICKED(IDC_CHECK_24, &CStream_Watch_Tab2::OnBnClickedCheck24)
	ON_BN_CLICKED(IDC_CHECK_25, &CStream_Watch_Tab2::OnBnClickedCheck25)
	ON_BN_CLICKED(IDC_CHECK_26, &CStream_Watch_Tab2::OnBnClickedCheck26)
	ON_BN_CLICKED(IDC_CHECK_27, &CStream_Watch_Tab2::OnBnClickedCheck27)
	ON_BN_CLICKED(IDC_CHECK_28, &CStream_Watch_Tab2::OnBnClickedCheck28)
	ON_BN_CLICKED(IDC_CHECK_29, &CStream_Watch_Tab2::OnBnClickedCheck29)
	ON_BN_CLICKED(IDC_CHECK_30, &CStream_Watch_Tab2::OnBnClickedCheck30)
	ON_BN_CLICKED(IDC_CHECK_31, &CStream_Watch_Tab2::OnBnClickedCheck31)
	ON_BN_CLICKED(IDC_CHECK_32, &CStream_Watch_Tab2::OnBnClickedCheck32)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME17, &CStream_Watch_Tab2::OnEnKillfocusEditChannelName17)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME18, &CStream_Watch_Tab2::OnEnKillfocusEditChannelName18)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME19, &CStream_Watch_Tab2::OnEnKillfocusEditChannelName19)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME20, &CStream_Watch_Tab2::OnEnKillfocusEditChannelName20)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME21, &CStream_Watch_Tab2::OnEnKillfocusEditChannelName21)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME22, &CStream_Watch_Tab2::OnEnKillfocusEditChannelName22)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME23, &CStream_Watch_Tab2::OnEnKillfocusEditChannelName23)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME24, &CStream_Watch_Tab2::OnEnKillfocusEditChannelName24)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME25, &CStream_Watch_Tab2::OnEnKillfocusEditChannelName25)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME26, &CStream_Watch_Tab2::OnEnKillfocusEditChannelName26)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME27, &CStream_Watch_Tab2::OnEnKillfocusEditChannelName27)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME28, &CStream_Watch_Tab2::OnEnKillfocusEditChannelName28)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME29, &CStream_Watch_Tab2::OnEnKillfocusEditChannelName29)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME30, &CStream_Watch_Tab2::OnEnKillfocusEditChannelName30)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME31, &CStream_Watch_Tab2::OnEnKillfocusEditChannelName31)
	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL_NAME32, &CStream_Watch_Tab2::OnEnKillfocusEditChannelName32)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_17, &CStream_Watch_Tab2::OnBnClickedManualCheck17)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_18, &CStream_Watch_Tab2::OnBnClickedManualCheck18)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_19, &CStream_Watch_Tab2::OnBnClickedManualCheck19)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_20, &CStream_Watch_Tab2::OnBnClickedManualCheck20)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_21, &CStream_Watch_Tab2::OnBnClickedManualCheck21)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_22, &CStream_Watch_Tab2::OnBnClickedManualCheck22)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_23, &CStream_Watch_Tab2::OnBnClickedManualCheck23)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_24, &CStream_Watch_Tab2::OnBnClickedManualCheck24)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_25, &CStream_Watch_Tab2::OnBnClickedManualCheck25)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_26, &CStream_Watch_Tab2::OnBnClickedManualCheck26)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_27, &CStream_Watch_Tab2::OnBnClickedManualCheck27)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_28, &CStream_Watch_Tab2::OnBnClickedManualCheck28)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_29, &CStream_Watch_Tab2::OnBnClickedManualCheck29)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_30, &CStream_Watch_Tab2::OnBnClickedManualCheck30)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_31, &CStream_Watch_Tab2::OnBnClickedManualCheck31)
	ON_BN_CLICKED(IDC_MANUAL_CHECK_32, &CStream_Watch_Tab2::OnBnClickedManualCheck32)
	ON_BN_CLICKED(IDC_CHECK_EME17, &CStream_Watch_Tab2::OnBnClickedCheckEme17)
	ON_BN_CLICKED(IDC_CHECK_EME18, &CStream_Watch_Tab2::OnBnClickedCheckEme18)
	ON_BN_CLICKED(IDC_CHECK_EME19, &CStream_Watch_Tab2::OnBnClickedCheckEme19)
	ON_BN_CLICKED(IDC_CHECK_EME20, &CStream_Watch_Tab2::OnBnClickedCheckEme20)
	ON_BN_CLICKED(IDC_CHECK_EME21, &CStream_Watch_Tab2::OnBnClickedCheckEme21)
	ON_BN_CLICKED(IDC_CHECK_EME22, &CStream_Watch_Tab2::OnBnClickedCheckEme22)
	ON_BN_CLICKED(IDC_CHECK_EME23, &CStream_Watch_Tab2::OnBnClickedCheckEme23)
	ON_BN_CLICKED(IDC_CHECK_EME24, &CStream_Watch_Tab2::OnBnClickedCheckEme24)
	ON_BN_CLICKED(IDC_CHECK_EME25, &CStream_Watch_Tab2::OnBnClickedCheckEme25)
	ON_BN_CLICKED(IDC_CHECK_EME26, &CStream_Watch_Tab2::OnBnClickedCheckEme26)
	ON_BN_CLICKED(IDC_CHECK_EME27, &CStream_Watch_Tab2::OnBnClickedCheckEme27)
	ON_BN_CLICKED(IDC_CHECK_EME28, &CStream_Watch_Tab2::OnBnClickedCheckEme28)
	ON_BN_CLICKED(IDC_CHECK_EME29, &CStream_Watch_Tab2::OnBnClickedCheckEme29)
	ON_BN_CLICKED(IDC_CHECK_EME30, &CStream_Watch_Tab2::OnBnClickedCheckEme30)
	ON_BN_CLICKED(IDC_CHECK_EME31, &CStream_Watch_Tab2::OnBnClickedCheckEme31)
	ON_BN_CLICKED(IDC_CHECK_EME32, &CStream_Watch_Tab2::OnBnClickedCheckEme32)
END_MESSAGE_MAP()


// CStream_Watch_Tab2 message handlers
BOOL CStream_Watch_Tab2::OnInitDialog()
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
		General_Tab2.m_PriStatus[i].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
		General_Tab2.m_SecondaryStatus[i].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
		General_Tab2.m_OtputStatus[i].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
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
					General_Tab2.m_cmbSourceAdapter[k][i].AddString(strWAddr);
					General_Tab2.m_cmbSourceAdapter[k][i].SetCurSel(0);	
				#else	
					m_cmbSourceAdapter[i].AddString(inet_ntoa(addr));
				#endif
				}
				General_Tab2.m_cmbDestAdapter[k].AddString(strWAddr);
				General_Tab2.m_cmbDestAdapter[k].SetCurSel(0);	
			}
		}
	}
	for(int i = 0; i < MAX_STREAM/3; i++)
	{
		GetDlgItem(531 + (20 * i))->EnableWindow(false);
		GetDlgItem(532 + (20 * i))->EnableWindow(false);
	}

	m_pToolTip = new CToolTipCtrl;
	m_pToolTip->Create(this);

	for(int k = 0; k < MAX_STREAM/3; k++)
	{
		int m = 520 + (20 * k) + 1;

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

BOOL CStream_Watch_Tab2::PreTranslateMessage(MSG* pMsg)
{
	if (NULL != m_pToolTip)
		m_pToolTip->RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}
#define ON_CLICK_START(XX)	void CStream_Watch_Tab2::OnBnClickedButtonStart##XX()	\
							{													\
								int nVal = XX;									\
								m_parent_dlg->StartEvent(nVal - 1);							\
							}
ON_CLICK_START(17);
ON_CLICK_START(18);
ON_CLICK_START(19);
ON_CLICK_START(20);
ON_CLICK_START(21);
ON_CLICK_START(22);
ON_CLICK_START(23);
ON_CLICK_START(24);
ON_CLICK_START(25);
ON_CLICK_START(26);
ON_CLICK_START(27);
ON_CLICK_START(28);
ON_CLICK_START(29);
ON_CLICK_START(30);
ON_CLICK_START(31);
ON_CLICK_START(32);

#define ON_CLICK_STOP(XX)	void CStream_Watch_Tab2::OnBnClickedButtonStop##XX()	\
							{													\
								int nVal = XX;									\
								m_parent_dlg->StopEvent(nVal - 1);							\
							}
ON_CLICK_STOP(17);
ON_CLICK_STOP(18);
ON_CLICK_STOP(19);
ON_CLICK_STOP(20);
ON_CLICK_STOP(21);
ON_CLICK_STOP(22);
ON_CLICK_STOP(23);
ON_CLICK_STOP(24);
ON_CLICK_STOP(25);
ON_CLICK_STOP(26);
ON_CLICK_STOP(27);
ON_CLICK_STOP(28);
ON_CLICK_STOP(29);
ON_CLICK_STOP(30);
ON_CLICK_STOP(31);
ON_CLICK_STOP(32);

#define ON_CLICK_CHECK(XX)	void CStream_Watch_Tab2::OnBnClickedCheck##XX()	\
							{											\
								int nVal = XX;							\
								m_parent_dlg->CheckedBoxEffect(nVal - 1,1);				\
							}
ON_CLICK_CHECK(17);
ON_CLICK_CHECK(18);
ON_CLICK_CHECK(19);
ON_CLICK_CHECK(20);
ON_CLICK_CHECK(21);
ON_CLICK_CHECK(22);
ON_CLICK_CHECK(23);
ON_CLICK_CHECK(24);
ON_CLICK_CHECK(25);
ON_CLICK_CHECK(26);
ON_CLICK_CHECK(27);
ON_CLICK_CHECK(28);
ON_CLICK_CHECK(29);
ON_CLICK_CHECK(30);
ON_CLICK_CHECK(31);
ON_CLICK_CHECK(32);

#define ON_KILLFOCUS_CHANNELNAME(XX)	void CStream_Watch_Tab2::OnEnKillfocusEditChannelName##XX()	\
										{											\
											int nVal = XX;							\
											m_parent_dlg->KillFocusEffect(nVal - 1);				\
										}

ON_KILLFOCUS_CHANNELNAME(17);
ON_KILLFOCUS_CHANNELNAME(18);
ON_KILLFOCUS_CHANNELNAME(19);
ON_KILLFOCUS_CHANNELNAME(20);
ON_KILLFOCUS_CHANNELNAME(21);
ON_KILLFOCUS_CHANNELNAME(22);
ON_KILLFOCUS_CHANNELNAME(23);
ON_KILLFOCUS_CHANNELNAME(24);
ON_KILLFOCUS_CHANNELNAME(25);
ON_KILLFOCUS_CHANNELNAME(26);
ON_KILLFOCUS_CHANNELNAME(27);
ON_KILLFOCUS_CHANNELNAME(28);
ON_KILLFOCUS_CHANNELNAME(29);
ON_KILLFOCUS_CHANNELNAME(30);
ON_KILLFOCUS_CHANNELNAME(31);
ON_KILLFOCUS_CHANNELNAME(32);

#define ON_CLICK_MANUAL_CHECK(XX)	void CStream_Watch_Tab2::OnBnClickedManualCheck##XX()	\
									{											\
										int nVal = XX;							\
										m_parent_dlg->ManualCheckBoxEffect(nVal - 1,2);		\
									}

ON_CLICK_MANUAL_CHECK(17);
ON_CLICK_MANUAL_CHECK(18);
ON_CLICK_MANUAL_CHECK(19);
ON_CLICK_MANUAL_CHECK(20);
ON_CLICK_MANUAL_CHECK(21);
ON_CLICK_MANUAL_CHECK(22);
ON_CLICK_MANUAL_CHECK(23);
ON_CLICK_MANUAL_CHECK(24);
ON_CLICK_MANUAL_CHECK(25);
ON_CLICK_MANUAL_CHECK(26);
ON_CLICK_MANUAL_CHECK(27);
ON_CLICK_MANUAL_CHECK(28);
ON_CLICK_MANUAL_CHECK(29);
ON_CLICK_MANUAL_CHECK(30);
ON_CLICK_MANUAL_CHECK(31);
ON_CLICK_MANUAL_CHECK(32);

#define ON_CLICK_EME_CHECK(XX)	void CStream_Watch_Tab2::OnBnClickedCheckEme##XX()	\
							{											\
								int nVal = XX;							\
								m_parent_dlg->EmeCheckBoxEffect(nVal - 1,2);		\
							}
ON_CLICK_EME_CHECK(17);
ON_CLICK_EME_CHECK(18);
ON_CLICK_EME_CHECK(19);
ON_CLICK_EME_CHECK(20);
ON_CLICK_EME_CHECK(21);
ON_CLICK_EME_CHECK(22);
ON_CLICK_EME_CHECK(23);
ON_CLICK_EME_CHECK(24);
ON_CLICK_EME_CHECK(25);
ON_CLICK_EME_CHECK(26);
ON_CLICK_EME_CHECK(27);
ON_CLICK_EME_CHECK(28);
ON_CLICK_EME_CHECK(29);
ON_CLICK_EME_CHECK(30);
ON_CLICK_EME_CHECK(31);
ON_CLICK_EME_CHECK(32);


