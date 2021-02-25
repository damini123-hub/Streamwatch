// Stream_Watch_Tab4.cpp : implementation file
//
#include "stdafx.h"
#include "StreamWatch.h"
#include "Stream_Watch_Tab4.h"

// CStream_Watch_Tab4 dialog

IMPLEMENT_DYNAMIC(CStream_Watch_Tab4, CDialog)

CStream_Watch_Tab4::CStream_Watch_Tab4(CWnd* pParent /*=NULL*/, CStream_WatchDlg* parent_Dlg)
	: CDialog(CStream_Watch_Tab4::IDD, pParent)
	,m_parent_dlg(parent_Dlg)
{
	for(int k = 0; k < MAX_STREAM; k++)
	{
		StatusTab.m_strStatusChannel_Name[k] = _T("Channel");
	}
}

CStream_Watch_Tab4::~CStream_Watch_Tab4()
{
	if(m_pToolTip != NULL)
	{
		SAFE_DELETE(m_pToolTip);
	}
}

BOOL CStream_Watch_Tab4::OnInitDialog()
{
	CDialog::OnInitDialog();
	for(int k = 0; k < MAX_STREAM; k++)
	{
		StatusTab.m_pic_Prime_Stream[k].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
		StatusTab.m_pic_Second_Stream[k].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
		StatusTab.m_pic_Output_Stream[k].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
	}

	for(int i = 0; i < MAX_STREAM; i++)
	{
		GetDlgItem(2301 + i)->EnableWindow(false);
		GetDlgItem(2351 + i)->EnableWindow(false);

		StreamWatch[i].strCCError.Format(_T("%I64d"), StreamWatch[i].m_CCError);	
		StreamWatch[i].m_strCCError.SetWindowTextW(StreamWatch[i].strCCError);	
	}

	m_pToolTip = new CToolTipCtrl;
	m_pToolTip->Create(this);

	for(int k = 0; k < MAX_STREAM && k < m_nLiecensedStream; k++)
	{
		//int m = 2401 + k;
		int m = 2101 + k;
		m_pToolTip->AddTool(GetDlgItem(m),_T("Primary Stream status"));
		m += 50;
		m_pToolTip->AddTool(GetDlgItem(m),_T("Secondary Stream status"));
		m += 50;
		m_pToolTip->AddTool(GetDlgItem(m),_T("Output status"));
		m += 50;
		m_pToolTip->AddTool(GetDlgItem(m),_T("channel name"));
		m += 50;
		m_pToolTip->AddTool(GetDlgItem(m),_T("Click start Streaming"));
		m += 50;
		m_pToolTip->AddTool(GetDlgItem(m),_T("Click stop Streaming"));
		m += 50;
		m_pToolTip->AddTool(GetDlgItem(m),_T("Click to enable start"));
		m += 100;
		m_pToolTip->AddTool(GetDlgItem(m),_T("Click to Select Manual Secondary Input"));
	}
	m_pToolTip->Activate(TRUE);

	UpdateColors(false);
	
	return TRUE; 
}
BOOL CStream_Watch_Tab4::PreTranslateMessage(MSG* pMsg)
{
	if (NULL != m_pToolTip)
		m_pToolTip->RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}
void CStream_Watch_Tab4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	for(int k = 0; k < MAX_STREAM; k++)
	{
		int m = 2100 + k + 1;

		DDX_Control(pDX, m, StatusTab.m_pic_Prime_Stream[k]);
		m += 50;
		DDX_Control(pDX, m, StatusTab.m_pic_Second_Stream[k]);
		m += 50;
		DDX_Control(pDX, m, StatusTab.m_pic_Output_Stream[k]);
		m += 50;
		DDX_Text(pDX, m, StatusTab.m_strStatusChannel_Name[k]);
		m += 350;
		DDX_Control(pDX, m, StreamWatch[k].m_strCCError);
	}
	
}

BEGIN_MESSAGE_MAP(CStream_Watch_Tab4, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_1, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart1)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_2, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart2)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_3, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart3)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_4, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart4)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_5, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart5)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_6, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart6)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_7, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart7)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_8, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart8)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_9, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart9)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_10, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart10)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_11, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart11)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_12, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart12)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_13, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart13)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_14, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart14)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_15, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart15)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_16, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart16)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_17, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart17)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_18, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart18)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_19, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart19)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_20, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart20)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_21, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart21)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_22, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart22)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_23, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart23)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_24, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart24)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_25, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart25)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_26, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart26)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_27, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart27)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_28, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart28)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_29, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart29)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_30, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart30)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_31, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart31)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_32, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart32)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_33, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart33)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_34, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart34)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_35, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart35)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_36, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart36)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_37, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart37)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_38, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart38)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_39, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart39)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_40, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart40)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_41, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart41)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_42, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart42)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_43, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart43)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_44, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart44)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_45, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart45)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_46, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart46)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_47, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart47)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_START_48, &CStream_Watch_Tab4::OnBnClickedButtonStatusStart48)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_1, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop1)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_2, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop2)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_3, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop3)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_4, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop4)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_5, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop5)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_6, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop6)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_7, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop7)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_8, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop8)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_9, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop9)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_10, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop10)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_11, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop11)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_12, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop12)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_13, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop13)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_14, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop14)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_15, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop15)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_16, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop16)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_17, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop17)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_18, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop18)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_19, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop19)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_20, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop20)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_21, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop21)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_22, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop22)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_23, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop23)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_24, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop24)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_25, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop25)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_26, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop26)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_27, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop27)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_28, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop28)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_29, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop29)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_30, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop30)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_31, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop31)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_32, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop32)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_33, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop33)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_34, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop34)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_35, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop35)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_36, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop36)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_37, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop37)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_38, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop38)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_39, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop39)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_40, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop40)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_41, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop41)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_42, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop42)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_43, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop43)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_44, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop44)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_45, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop45)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_46, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop46)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_47, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop47)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_STOP_48, &CStream_Watch_Tab4::OnBnClickedButtonStatusStop48)
	ON_BN_CLICKED(IDC_STATUS_CHECK_1, &CStream_Watch_Tab4::OnBnClickedStatusCheck1)
	ON_BN_CLICKED(IDC_STATUS_CHECK_2, &CStream_Watch_Tab4::OnBnClickedStatusCheck2)
	ON_BN_CLICKED(IDC_STATUS_CHECK_3, &CStream_Watch_Tab4::OnBnClickedStatusCheck3)
	ON_BN_CLICKED(IDC_STATUS_CHECK_4, &CStream_Watch_Tab4::OnBnClickedStatusCheck4)
	ON_BN_CLICKED(IDC_STATUS_CHECK_5, &CStream_Watch_Tab4::OnBnClickedStatusCheck5)
	ON_BN_CLICKED(IDC_STATUS_CHECK_6, &CStream_Watch_Tab4::OnBnClickedStatusCheck6)
	ON_BN_CLICKED(IDC_STATUS_CHECK_7, &CStream_Watch_Tab4::OnBnClickedStatusCheck7)
	ON_BN_CLICKED(IDC_STATUS_CHECK_8, &CStream_Watch_Tab4::OnBnClickedStatusCheck8)
	ON_BN_CLICKED(IDC_STATUS_CHECK_9, &CStream_Watch_Tab4::OnBnClickedStatusCheck9)
	ON_BN_CLICKED(IDC_STATUS_CHECK_10, &CStream_Watch_Tab4::OnBnClickedStatusCheck10)
	ON_BN_CLICKED(IDC_STATUS_CHECK_11, &CStream_Watch_Tab4::OnBnClickedStatusCheck11)
	ON_BN_CLICKED(IDC_STATUS_CHECK_12, &CStream_Watch_Tab4::OnBnClickedStatusCheck12)
	ON_BN_CLICKED(IDC_STATUS_CHECK_13, &CStream_Watch_Tab4::OnBnClickedStatusCheck13)
	ON_BN_CLICKED(IDC_STATUS_CHECK_14, &CStream_Watch_Tab4::OnBnClickedStatusCheck14)
	ON_BN_CLICKED(IDC_STATUS_CHECK_15, &CStream_Watch_Tab4::OnBnClickedStatusCheck15)
	ON_BN_CLICKED(IDC_STATUS_CHECK_16, &CStream_Watch_Tab4::OnBnClickedStatusCheck16)
	ON_BN_CLICKED(IDC_STATUS_CHECK_17, &CStream_Watch_Tab4::OnBnClickedStatusCheck17)
	ON_BN_CLICKED(IDC_STATUS_CHECK_18, &CStream_Watch_Tab4::OnBnClickedStatusCheck18)
	ON_BN_CLICKED(IDC_STATUS_CHECK_19, &CStream_Watch_Tab4::OnBnClickedStatusCheck19)
	ON_BN_CLICKED(IDC_STATUS_CHECK_20, &CStream_Watch_Tab4::OnBnClickedStatusCheck20)
	ON_BN_CLICKED(IDC_STATUS_CHECK_21, &CStream_Watch_Tab4::OnBnClickedStatusCheck21)
	ON_BN_CLICKED(IDC_STATUS_CHECK_22, &CStream_Watch_Tab4::OnBnClickedStatusCheck22)
	ON_BN_CLICKED(IDC_STATUS_CHECK_23, &CStream_Watch_Tab4::OnBnClickedStatusCheck23)
	ON_BN_CLICKED(IDC_STATUS_CHECK_24, &CStream_Watch_Tab4::OnBnClickedStatusCheck24)
	ON_BN_CLICKED(IDC_STATUS_CHECK_25, &CStream_Watch_Tab4::OnBnClickedStatusCheck25)
	ON_BN_CLICKED(IDC_STATUS_CHECK_26, &CStream_Watch_Tab4::OnBnClickedStatusCheck26)
	ON_BN_CLICKED(IDC_STATUS_CHECK_27, &CStream_Watch_Tab4::OnBnClickedStatusCheck27)
	ON_BN_CLICKED(IDC_STATUS_CHECK_28, &CStream_Watch_Tab4::OnBnClickedStatusCheck28)
	ON_BN_CLICKED(IDC_STATUS_CHECK_29, &CStream_Watch_Tab4::OnBnClickedStatusCheck29)
	ON_BN_CLICKED(IDC_STATUS_CHECK_30, &CStream_Watch_Tab4::OnBnClickedStatusCheck30)
	ON_BN_CLICKED(IDC_STATUS_CHECK_31, &CStream_Watch_Tab4::OnBnClickedStatusCheck31)
	ON_BN_CLICKED(IDC_STATUS_CHECK_32, &CStream_Watch_Tab4::OnBnClickedStatusCheck32)
	ON_BN_CLICKED(IDC_STATUS_CHECK_33, &CStream_Watch_Tab4::OnBnClickedStatusCheck33)
	ON_BN_CLICKED(IDC_STATUS_CHECK_34, &CStream_Watch_Tab4::OnBnClickedStatusCheck34)
	ON_BN_CLICKED(IDC_STATUS_CHECK_35, &CStream_Watch_Tab4::OnBnClickedStatusCheck35)
	ON_BN_CLICKED(IDC_STATUS_CHECK_36, &CStream_Watch_Tab4::OnBnClickedStatusCheck36)
	ON_BN_CLICKED(IDC_STATUS_CHECK_37, &CStream_Watch_Tab4::OnBnClickedStatusCheck37)
	ON_BN_CLICKED(IDC_STATUS_CHECK_38, &CStream_Watch_Tab4::OnBnClickedStatusCheck38)
	ON_BN_CLICKED(IDC_STATUS_CHECK_39, &CStream_Watch_Tab4::OnBnClickedStatusCheck39)
	ON_BN_CLICKED(IDC_STATUS_CHECK_40, &CStream_Watch_Tab4::OnBnClickedStatusCheck40)
	ON_BN_CLICKED(IDC_STATUS_CHECK_41, &CStream_Watch_Tab4::OnBnClickedStatusCheck41)
	ON_BN_CLICKED(IDC_STATUS_CHECK_42, &CStream_Watch_Tab4::OnBnClickedStatusCheck42)
	ON_BN_CLICKED(IDC_STATUS_CHECK_43, &CStream_Watch_Tab4::OnBnClickedStatusCheck43)
	ON_BN_CLICKED(IDC_STATUS_CHECK_44, &CStream_Watch_Tab4::OnBnClickedStatusCheck44)
	ON_BN_CLICKED(IDC_STATUS_CHECK_45, &CStream_Watch_Tab4::OnBnClickedStatusCheck45)
	ON_BN_CLICKED(IDC_STATUS_CHECK_46, &CStream_Watch_Tab4::OnBnClickedStatusCheck46)
	ON_BN_CLICKED(IDC_STATUS_CHECK_47, &CStream_Watch_Tab4::OnBnClickedStatusCheck47)
	ON_BN_CLICKED(IDC_STATUS_CHECK_48, &CStream_Watch_Tab4::OnBnClickedStatusCheck48)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_1, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck1)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_2, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck2)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_3, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck3)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_4, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck4)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_5, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck5)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_6, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck6)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_7, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck7)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_8, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck8)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_9, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck9)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_10, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck10)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_11, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck11)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_12, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck12)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_13, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck13)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_14, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck14)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_15, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck15)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_16, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck16)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_17, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck17)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_18, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck18)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_19, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck19)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_20, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck20)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_21, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck21)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_22, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck22)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_23, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck23)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_24, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck24)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_25, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck25)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_26, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck26)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_27, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck27)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_28, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck28)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_29, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck29)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_30, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck30)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_31, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck31)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_32, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck32)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_33, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck33)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_34, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck34)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_35, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck35)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_36, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck36)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_37, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck37)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_38, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck38)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_39, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck39)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_40, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck40)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_41, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck41)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_42, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck42)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_43, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck43)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_44, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck44)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_45, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck45)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_46, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck46)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_47, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck47)
	ON_BN_CLICKED(IDC_MANUAL_STATUS_CHECK_48, &CStream_Watch_Tab4::OnBnClickedManualStatusCheck48)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_1, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck1)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_2, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck2)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_3, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck3)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_4, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck4)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_5, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck5)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_6, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck6)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_7, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck7)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_8, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck8)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_9, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck9)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_10, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck10)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_11, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck11)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_12, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck12)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_13, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck13)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_14, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck14)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_15, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck15)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_16, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck16)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_17, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck17)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_18, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck18)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_19, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck19)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_20, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck20)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_21, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck21)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_22, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck22)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_23, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck23)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_24, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck24)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_25, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck25)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_26, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck26)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_27, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck27)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_28, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck28)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_29, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck29)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_30, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck30)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_31, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck31)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_32, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck32)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_33, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck33)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_34, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck34)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_35, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck35)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_36, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck36)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_37, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck37)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_38, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck38)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_39, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck39)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_40, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck40)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_41, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck41)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_42, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck42)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_43, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck43)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_44, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck44)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_45, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck45)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_46, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck46)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_47, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck47)
	ON_BN_CLICKED(IDC_EME_STATUS_CHECK_48, &CStream_Watch_Tab4::OnBnClickedEmeStatusCheck48)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_1, &CStream_Watch_Tab4::OnBnClickedReset1)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_2, &CStream_Watch_Tab4::OnBnClickedReset2)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_3, &CStream_Watch_Tab4::OnBnClickedReset3)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_4, &CStream_Watch_Tab4::OnBnClickedReset4)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_5, &CStream_Watch_Tab4::OnBnClickedReset5)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_6, &CStream_Watch_Tab4::OnBnClickedReset6)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_7, &CStream_Watch_Tab4::OnBnClickedReset7)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_8, &CStream_Watch_Tab4::OnBnClickedReset8)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_9, &CStream_Watch_Tab4::OnBnClickedReset9)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_10, &CStream_Watch_Tab4::OnBnClickedReset10)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_11, &CStream_Watch_Tab4::OnBnClickedReset11)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_12, &CStream_Watch_Tab4::OnBnClickedReset12)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_13, &CStream_Watch_Tab4::OnBnClickedReset13)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_14, &CStream_Watch_Tab4::OnBnClickedReset14)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_15, &CStream_Watch_Tab4::OnBnClickedReset15)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_16, &CStream_Watch_Tab4::OnBnClickedReset16)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_17, &CStream_Watch_Tab4::OnBnClickedReset17)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_18, &CStream_Watch_Tab4::OnBnClickedReset18)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_19, &CStream_Watch_Tab4::OnBnClickedReset19)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_20, &CStream_Watch_Tab4::OnBnClickedReset20)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_21, &CStream_Watch_Tab4::OnBnClickedReset21)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_22, &CStream_Watch_Tab4::OnBnClickedReset22)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_23, &CStream_Watch_Tab4::OnBnClickedReset23)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_24, &CStream_Watch_Tab4::OnBnClickedReset24)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_25, &CStream_Watch_Tab4::OnBnClickedReset25)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_26, &CStream_Watch_Tab4::OnBnClickedReset26)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_27, &CStream_Watch_Tab4::OnBnClickedReset27)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_28, &CStream_Watch_Tab4::OnBnClickedReset28)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_29, &CStream_Watch_Tab4::OnBnClickedReset29)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_30, &CStream_Watch_Tab4::OnBnClickedReset30)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_31, &CStream_Watch_Tab4::OnBnClickedReset31)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_32, &CStream_Watch_Tab4::OnBnClickedReset32)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_33, &CStream_Watch_Tab4::OnBnClickedReset33)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_34, &CStream_Watch_Tab4::OnBnClickedReset34)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_35, &CStream_Watch_Tab4::OnBnClickedReset35)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_36, &CStream_Watch_Tab4::OnBnClickedReset36)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_37, &CStream_Watch_Tab4::OnBnClickedReset37)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_38, &CStream_Watch_Tab4::OnBnClickedReset38)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_39, &CStream_Watch_Tab4::OnBnClickedReset39)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_40, &CStream_Watch_Tab4::OnBnClickedReset40)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_41, &CStream_Watch_Tab4::OnBnClickedReset41)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_42, &CStream_Watch_Tab4::OnBnClickedReset42)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_43, &CStream_Watch_Tab4::OnBnClickedReset43)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_44, &CStream_Watch_Tab4::OnBnClickedReset44)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_45, &CStream_Watch_Tab4::OnBnClickedReset45)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_46, &CStream_Watch_Tab4::OnBnClickedReset46)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_47, &CStream_Watch_Tab4::OnBnClickedReset47)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CCERROR_48, &CStream_Watch_Tab4::OnBnClickedReset48)
	END_MESSAGE_MAP()


#define ON_CLICK_STATUS_START(XX)	void CStream_Watch_Tab4::OnBnClickedButtonStatusStart##XX()	\
									{													\
										int nVal = XX;									\
										m_parent_dlg->StartEvent(nVal - 1);							\
									}

ON_CLICK_STATUS_START(1);
ON_CLICK_STATUS_START(2);
ON_CLICK_STATUS_START(3);
ON_CLICK_STATUS_START(4);
ON_CLICK_STATUS_START(5);
ON_CLICK_STATUS_START(6);
ON_CLICK_STATUS_START(7);
ON_CLICK_STATUS_START(8);
ON_CLICK_STATUS_START(9);
ON_CLICK_STATUS_START(10);
ON_CLICK_STATUS_START(11);
ON_CLICK_STATUS_START(12);
ON_CLICK_STATUS_START(13);
ON_CLICK_STATUS_START(14);
ON_CLICK_STATUS_START(15);
ON_CLICK_STATUS_START(16);
ON_CLICK_STATUS_START(17);
ON_CLICK_STATUS_START(18);
ON_CLICK_STATUS_START(19);
ON_CLICK_STATUS_START(20);
ON_CLICK_STATUS_START(21);
ON_CLICK_STATUS_START(22);
ON_CLICK_STATUS_START(23);
ON_CLICK_STATUS_START(24);
ON_CLICK_STATUS_START(25);
ON_CLICK_STATUS_START(26);
ON_CLICK_STATUS_START(27);
ON_CLICK_STATUS_START(28);
ON_CLICK_STATUS_START(29);
ON_CLICK_STATUS_START(30);
ON_CLICK_STATUS_START(31);
ON_CLICK_STATUS_START(32);		
ON_CLICK_STATUS_START(33);
ON_CLICK_STATUS_START(34);
ON_CLICK_STATUS_START(35);
ON_CLICK_STATUS_START(36);
ON_CLICK_STATUS_START(37);
ON_CLICK_STATUS_START(38);
ON_CLICK_STATUS_START(39);
ON_CLICK_STATUS_START(40);
ON_CLICK_STATUS_START(41);
ON_CLICK_STATUS_START(42);
ON_CLICK_STATUS_START(43);
ON_CLICK_STATUS_START(44);
ON_CLICK_STATUS_START(45);
ON_CLICK_STATUS_START(46);
ON_CLICK_STATUS_START(47);
ON_CLICK_STATUS_START(48);

#define ON_CLICK_STATUS_STOP(XX)	void CStream_Watch_Tab4::OnBnClickedButtonStatusStop##XX()	\
									{													\
										int nVal = XX;									\
										m_parent_dlg->StopEvent(nVal - 1);							\
									}
ON_CLICK_STATUS_STOP(1);
ON_CLICK_STATUS_STOP(2);
ON_CLICK_STATUS_STOP(3);
ON_CLICK_STATUS_STOP(4);
ON_CLICK_STATUS_STOP(5);
ON_CLICK_STATUS_STOP(6);
ON_CLICK_STATUS_STOP(7);
ON_CLICK_STATUS_STOP(8);
ON_CLICK_STATUS_STOP(9);
ON_CLICK_STATUS_STOP(10);
ON_CLICK_STATUS_STOP(11);
ON_CLICK_STATUS_STOP(12);
ON_CLICK_STATUS_STOP(13);
ON_CLICK_STATUS_STOP(14);
ON_CLICK_STATUS_STOP(15);
ON_CLICK_STATUS_STOP(16);
ON_CLICK_STATUS_STOP(17);
ON_CLICK_STATUS_STOP(18);
ON_CLICK_STATUS_STOP(19);
ON_CLICK_STATUS_STOP(20);
ON_CLICK_STATUS_STOP(21);
ON_CLICK_STATUS_STOP(22);
ON_CLICK_STATUS_STOP(23);
ON_CLICK_STATUS_STOP(24);
ON_CLICK_STATUS_STOP(25);
ON_CLICK_STATUS_STOP(26);
ON_CLICK_STATUS_STOP(27);
ON_CLICK_STATUS_STOP(28);
ON_CLICK_STATUS_STOP(29);
ON_CLICK_STATUS_STOP(30);
ON_CLICK_STATUS_STOP(31);
ON_CLICK_STATUS_STOP(32);
ON_CLICK_STATUS_STOP(33);
ON_CLICK_STATUS_STOP(34);
ON_CLICK_STATUS_STOP(35);
ON_CLICK_STATUS_STOP(36);
ON_CLICK_STATUS_STOP(37);
ON_CLICK_STATUS_STOP(38);
ON_CLICK_STATUS_STOP(39);
ON_CLICK_STATUS_STOP(40);
ON_CLICK_STATUS_STOP(41);
ON_CLICK_STATUS_STOP(42);
ON_CLICK_STATUS_STOP(43);
ON_CLICK_STATUS_STOP(44);
ON_CLICK_STATUS_STOP(45);
ON_CLICK_STATUS_STOP(46);
ON_CLICK_STATUS_STOP(47);
ON_CLICK_STATUS_STOP(48);

#define ON_CLICK_STATUS_CHECK(XX)	void CStream_Watch_Tab4::OnBnClickedStatusCheck##XX()	\
							{											\
								int nVal = XX;							\
								m_parent_dlg->CheckedBoxEffect(nVal - 1,3);				\
							}
ON_CLICK_STATUS_CHECK(1);
ON_CLICK_STATUS_CHECK(2);
ON_CLICK_STATUS_CHECK(3);
ON_CLICK_STATUS_CHECK(4);
ON_CLICK_STATUS_CHECK(5);
ON_CLICK_STATUS_CHECK(6);
ON_CLICK_STATUS_CHECK(7);
ON_CLICK_STATUS_CHECK(8);
ON_CLICK_STATUS_CHECK(9);
ON_CLICK_STATUS_CHECK(10);
ON_CLICK_STATUS_CHECK(11);
ON_CLICK_STATUS_CHECK(12);
ON_CLICK_STATUS_CHECK(13);
ON_CLICK_STATUS_CHECK(14);
ON_CLICK_STATUS_CHECK(15);
ON_CLICK_STATUS_CHECK(16);
ON_CLICK_STATUS_CHECK(17);
ON_CLICK_STATUS_CHECK(18);
ON_CLICK_STATUS_CHECK(19);
ON_CLICK_STATUS_CHECK(20);
ON_CLICK_STATUS_CHECK(21);
ON_CLICK_STATUS_CHECK(22);
ON_CLICK_STATUS_CHECK(23);
ON_CLICK_STATUS_CHECK(24);
ON_CLICK_STATUS_CHECK(25);
ON_CLICK_STATUS_CHECK(26);
ON_CLICK_STATUS_CHECK(27);
ON_CLICK_STATUS_CHECK(28);
ON_CLICK_STATUS_CHECK(29);
ON_CLICK_STATUS_CHECK(30);
ON_CLICK_STATUS_CHECK(31);
ON_CLICK_STATUS_CHECK(32);
ON_CLICK_STATUS_CHECK(33);
ON_CLICK_STATUS_CHECK(34);
ON_CLICK_STATUS_CHECK(35);
ON_CLICK_STATUS_CHECK(36);
ON_CLICK_STATUS_CHECK(37);
ON_CLICK_STATUS_CHECK(38);
ON_CLICK_STATUS_CHECK(39);
ON_CLICK_STATUS_CHECK(40);
ON_CLICK_STATUS_CHECK(41);
ON_CLICK_STATUS_CHECK(42);
ON_CLICK_STATUS_CHECK(43);
ON_CLICK_STATUS_CHECK(44);
ON_CLICK_STATUS_CHECK(45);
ON_CLICK_STATUS_CHECK(46);
ON_CLICK_STATUS_CHECK(47);
ON_CLICK_STATUS_CHECK(48);

#define ON_CLICK_MANUAL_STATUS_CHECK(XX)	void CStream_Watch_Tab4::OnBnClickedManualStatusCheck##XX()	\
							{											\
								int nVal = XX;							\
								m_parent_dlg->ManualCheckBoxEffect(nVal - 1,3);					\
							}

ON_CLICK_MANUAL_STATUS_CHECK(1);
ON_CLICK_MANUAL_STATUS_CHECK(2);
ON_CLICK_MANUAL_STATUS_CHECK(3);
ON_CLICK_MANUAL_STATUS_CHECK(4);
ON_CLICK_MANUAL_STATUS_CHECK(5);
ON_CLICK_MANUAL_STATUS_CHECK(6);
ON_CLICK_MANUAL_STATUS_CHECK(7);
ON_CLICK_MANUAL_STATUS_CHECK(8);
ON_CLICK_MANUAL_STATUS_CHECK(9);
ON_CLICK_MANUAL_STATUS_CHECK(10);
ON_CLICK_MANUAL_STATUS_CHECK(11);
ON_CLICK_MANUAL_STATUS_CHECK(12);
ON_CLICK_MANUAL_STATUS_CHECK(13);
ON_CLICK_MANUAL_STATUS_CHECK(14);
ON_CLICK_MANUAL_STATUS_CHECK(15);
ON_CLICK_MANUAL_STATUS_CHECK(16);
ON_CLICK_MANUAL_STATUS_CHECK(17);
ON_CLICK_MANUAL_STATUS_CHECK(18);
ON_CLICK_MANUAL_STATUS_CHECK(19);
ON_CLICK_MANUAL_STATUS_CHECK(20);
ON_CLICK_MANUAL_STATUS_CHECK(21);
ON_CLICK_MANUAL_STATUS_CHECK(22);
ON_CLICK_MANUAL_STATUS_CHECK(23);
ON_CLICK_MANUAL_STATUS_CHECK(24);
ON_CLICK_MANUAL_STATUS_CHECK(25);
ON_CLICK_MANUAL_STATUS_CHECK(26);
ON_CLICK_MANUAL_STATUS_CHECK(27);
ON_CLICK_MANUAL_STATUS_CHECK(28);
ON_CLICK_MANUAL_STATUS_CHECK(29);
ON_CLICK_MANUAL_STATUS_CHECK(30);
ON_CLICK_MANUAL_STATUS_CHECK(31);
ON_CLICK_MANUAL_STATUS_CHECK(32);
ON_CLICK_MANUAL_STATUS_CHECK(33);
ON_CLICK_MANUAL_STATUS_CHECK(34);
ON_CLICK_MANUAL_STATUS_CHECK(35);
ON_CLICK_MANUAL_STATUS_CHECK(36);
ON_CLICK_MANUAL_STATUS_CHECK(37);
ON_CLICK_MANUAL_STATUS_CHECK(38);
ON_CLICK_MANUAL_STATUS_CHECK(39);
ON_CLICK_MANUAL_STATUS_CHECK(40);
ON_CLICK_MANUAL_STATUS_CHECK(41);
ON_CLICK_MANUAL_STATUS_CHECK(42);
ON_CLICK_MANUAL_STATUS_CHECK(43);
ON_CLICK_MANUAL_STATUS_CHECK(44);
ON_CLICK_MANUAL_STATUS_CHECK(45);
ON_CLICK_MANUAL_STATUS_CHECK(46);
ON_CLICK_MANUAL_STATUS_CHECK(47);
ON_CLICK_MANUAL_STATUS_CHECK(48);


#define ON_CLICK_EME_STATUS_CHECK(XX)	void CStream_Watch_Tab4::OnBnClickedEmeStatusCheck##XX()	\
							{											\
								int nVal = XX;							\
								m_parent_dlg->EmeCheckBoxEffect(nVal - 1,3);					\
							}

ON_CLICK_EME_STATUS_CHECK(1);
ON_CLICK_EME_STATUS_CHECK(2);
ON_CLICK_EME_STATUS_CHECK(3);
ON_CLICK_EME_STATUS_CHECK(4);
ON_CLICK_EME_STATUS_CHECK(5);
ON_CLICK_EME_STATUS_CHECK(6);
ON_CLICK_EME_STATUS_CHECK(7);
ON_CLICK_EME_STATUS_CHECK(8);
ON_CLICK_EME_STATUS_CHECK(9);
ON_CLICK_EME_STATUS_CHECK(10);
ON_CLICK_EME_STATUS_CHECK(11);
ON_CLICK_EME_STATUS_CHECK(12);
ON_CLICK_EME_STATUS_CHECK(13);
ON_CLICK_EME_STATUS_CHECK(14);
ON_CLICK_EME_STATUS_CHECK(15);
ON_CLICK_EME_STATUS_CHECK(16);
ON_CLICK_EME_STATUS_CHECK(17);
ON_CLICK_EME_STATUS_CHECK(18);
ON_CLICK_EME_STATUS_CHECK(19);
ON_CLICK_EME_STATUS_CHECK(20);
ON_CLICK_EME_STATUS_CHECK(21);
ON_CLICK_EME_STATUS_CHECK(22);
ON_CLICK_EME_STATUS_CHECK(23);
ON_CLICK_EME_STATUS_CHECK(24);
ON_CLICK_EME_STATUS_CHECK(25);
ON_CLICK_EME_STATUS_CHECK(26);
ON_CLICK_EME_STATUS_CHECK(27);
ON_CLICK_EME_STATUS_CHECK(28);
ON_CLICK_EME_STATUS_CHECK(29);
ON_CLICK_EME_STATUS_CHECK(30);
ON_CLICK_EME_STATUS_CHECK(31);
ON_CLICK_EME_STATUS_CHECK(32);
ON_CLICK_EME_STATUS_CHECK(33);
ON_CLICK_EME_STATUS_CHECK(34);
ON_CLICK_EME_STATUS_CHECK(35);
ON_CLICK_EME_STATUS_CHECK(36);
ON_CLICK_EME_STATUS_CHECK(37);
ON_CLICK_EME_STATUS_CHECK(38);
ON_CLICK_EME_STATUS_CHECK(39);
ON_CLICK_EME_STATUS_CHECK(40);
ON_CLICK_EME_STATUS_CHECK(41);
ON_CLICK_EME_STATUS_CHECK(42);
ON_CLICK_EME_STATUS_CHECK(43);
ON_CLICK_EME_STATUS_CHECK(44);
ON_CLICK_EME_STATUS_CHECK(45);
ON_CLICK_EME_STATUS_CHECK(46);
ON_CLICK_EME_STATUS_CHECK(47);
ON_CLICK_EME_STATUS_CHECK(48);


#define ON_CLICK_RESET(XX)	  void CStream_Watch_Tab4::OnBnClickedReset##XX()	\
				{													\
					int nVal = XX;									\
					StreamWatch[nVal-1].m_CCError = 0;									\
					StreamWatch[nVal-1].strCCError.Format(_T("%I64d"), StreamWatch[nVal-1].m_CCError);			\
					StreamWatch[nVal-1].m_strCCError.SetWindowTextW(StreamWatch[nVal-1].strCCError);	\
					StreamWatch[nVal-1].ResetCCError(nVal-1);         \
					StreamWatch[nVal-1].m_bCCError = false;           \
				}						
					

ON_CLICK_RESET(1);
ON_CLICK_RESET(2);
ON_CLICK_RESET(3);
ON_CLICK_RESET(4);
ON_CLICK_RESET(5);
ON_CLICK_RESET(6);
ON_CLICK_RESET(7);
ON_CLICK_RESET(8);
ON_CLICK_RESET(9);
ON_CLICK_RESET(10);
ON_CLICK_RESET(11);
ON_CLICK_RESET(12);
ON_CLICK_RESET(13);
ON_CLICK_RESET(14);
ON_CLICK_RESET(15);
ON_CLICK_RESET(16);
ON_CLICK_RESET(17);
ON_CLICK_RESET(18);
ON_CLICK_RESET(19);
ON_CLICK_RESET(20);
ON_CLICK_RESET(21);
ON_CLICK_RESET(22);
ON_CLICK_RESET(23);
ON_CLICK_RESET(24);
ON_CLICK_RESET(25);
ON_CLICK_RESET(26);
ON_CLICK_RESET(27);
ON_CLICK_RESET(28);
ON_CLICK_RESET(29);
ON_CLICK_RESET(30);
ON_CLICK_RESET(31);
ON_CLICK_RESET(32);
ON_CLICK_RESET(33);
ON_CLICK_RESET(34);
ON_CLICK_RESET(35);
ON_CLICK_RESET(36);
ON_CLICK_RESET(37);
ON_CLICK_RESET(38);
ON_CLICK_RESET(39);
ON_CLICK_RESET(40);
ON_CLICK_RESET(41);
ON_CLICK_RESET(42);
ON_CLICK_RESET(43);
ON_CLICK_RESET(44);
ON_CLICK_RESET(45);
ON_CLICK_RESET(46);
ON_CLICK_RESET(47);
ON_CLICK_RESET(48);
				