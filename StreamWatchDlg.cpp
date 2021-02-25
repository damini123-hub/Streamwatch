// New_Version_Stream_WatchDlg.cpp : implementation file
//
#include "stdafx.h"
#include "StreamWatch.h"
#include "StreamWatchDlg.h"
#include "WaitingDlg.h"
#include "./CXml/Xml.h"
#include "Stream_Watch_Tab1.h"
#include "Stream_Watch_Tab2.h"
#include "Stream_Watch_Tab3.h"
#include "Stream_Watch_Tab4.h"
#include <tlhelp32.h>
#include "Shlwapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDT_TIMER_STREAM_STATUS           (WM_USER + 221) // web UI evnt timer
#define IDT_TIMER_ALERT           (WM_USER + 231) // web UI evnt timer
#define IDT_TIMER_CCERROR			(WM_USER + 232)
// CAboutDlg dialog used for App About

using namespace Generic; //for xml classes
CAlertMail AlertDlg;

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CStream_WatchDlg dialog
void MonitorExe()
{
	bool exist = IsProcessRunning(L"SendMail.exe");
	if(exist)
	{
		system("taskkill /f /im SendMail.exe");
		exist = false;
	}
}

CStream_WatchDlg::CStream_WatchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStream_WatchDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nTotalStartStream = 0;
	m_nTotalStartedChannel = 0;
	m_bTimerSet = false;
	for(int i = 0; i < 2; i++)
	{
		index[i] = 0;
		lastCheckedIndex[i] = 0;
	}
	for(int i = 0; i < 50; i++)
	{
		m_nAlertBufMessagePrime[i] = 0;
		m_nAlertCompairPrime[i] = 0;
		m_nAlertBufMessageSecond[i] = 0;
		m_nAlertCompairSecond[i] = 0;
		m_bCheckAlertStatusList[i] = false;
	}
	m_bCheckAlertStatus = true;
	m_bIntervalAlertTime = false;
	m_nLastAlertTime = 0;
	m_bCCErrorMail = true;
	m_bAlertNoMail = false;
}

void CStream_WatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STREAM_TAB, m_TabStreamTabCtrl);

	int n = 2003;
	for(int k = 0; k < 4; k++)
	{
		DDX_Control(pDX, n++, m_pic_Notify[k]);
	}
}

BEGIN_MESSAGE_MAP(CStream_WatchDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(IDCANCEL, &CStream_WatchDlg::OnClose)
	ON_BN_CLICKED(IDOK, &CStream_WatchDlg::OnBnClickedOk)

	ON_BN_CLICKED(IDC_BUTTON_START_ALL, &CStream_WatchDlg::OnBnClickedButtonStartAll)
	ON_BN_CLICKED(IDC_BUTTON_STOP_ALL, &CStream_WatchDlg::OnBnClickedButtonStopAll)
	ON_BN_CLICKED(IDC_CHECKED_ALL, &CStream_WatchDlg::OnBnClickedCheckedAll)
	ON_BN_CLICKED(IDC_BUTTON_ALERT_CONFIG, &CStream_WatchDlg::OnBnClickedAlertConfig)
END_MESSAGE_MAP()

CStream_WatchDlg::~CStream_WatchDlg()
{
	if(m_pToolTip != NULL)
	{
		SAFE_DELETE(m_pToolTip);
	}
}
// CStream_WatchDlg message handlers

void CStream_WatchDlg::DisableSecondarySource()
{
	//int nTab_Id = index / 16;
	//int nResource_Id = 200 + (20 * index) + 1;
	//int nStatusResource_Id = 2301 + index;
	//
	//m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
	//m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
	//m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
	//m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
	//m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
	//m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
	//m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
	//m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
	//m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
	//m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
	//m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
	//m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(true);
	//m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
	//nResource_Id += 4;
	//m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);   // for channel Name 
	//m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);

	//m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(nStatusResource_Id)->EnableWindow(false);
	//nStatusResource_Id += 50;
	//m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(nStatusResource_Id)->EnableWindow(true);
	for(int i = 0; i < MAX_STREAM; i++)
	{
		int nResourceId = 204 + (20 * i);
		int nTab_Id = i / 16;
		int nStatusResource_Id = 2501 + i;


		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResourceId++)->EnableWindow(false);
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResourceId++)->EnableWindow(false);
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResourceId++)->EnableWindow(false);
		nResourceId += 10;
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResourceId++)->EnableWindow(false);
		m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(nStatusResource_Id)->EnableWindow(false);
	}
}
void CStream_WatchDlg::DiableDestination()
{
	for(int i = 0; i < MAX_STREAM; i++)
	{
		int nResourceId = 207 + (20 * i);
		int nTab_Id = i / 16;

		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResourceId++)->EnableWindow(false);
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResourceId++)->EnableWindow(false);
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResourceId++)->EnableWindow(false);
	}
}
void CStream_WatchDlg::readEmailSystemName(TCHAR* AppPath)
{
	CFile FilePointer;
	CFileStatus Status;
	CFileException excFile;
	TCHAR strData[300];
	memset(strData, 0, sizeof(strData));
	if(FilePointer.Open(AppPath, CFile::modeRead | CFile::typeBinary, &excFile))
	{
		FilePointer.GetStatus(Status);
		//m_FileCreateTime = Status.m_mtime;
		int m = FilePointer.GetLength();
		FilePointer.Seek(2, CFile::begin);
		FilePointer.Read(strData, m * sizeof(TCHAR));
		_stprintf(AlertDlg.SystemName, _T("%s"), strData);
		FilePointer.Close();
	}
}
BOOL CStream_WatchDlg::OnInitDialog()
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
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);		// Set small icon


	// TODO: Add extra initialization here

	m_nLiecensedStream = ((CStream_WatchApp*)AfxGetApp())->m_LicenseValidator.m_nMax_LicensedStream;
	//m_nLiecensedStream = 12;
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP_ALERT);
	CButton* pButton = (CButton* )GetDlgItem(IDC_BUTTON_ALERT_CONFIG);
	pButton->ModifyStyle(0,BS_BITMAP);
	pButton->SetBitmap(bmp);
	
	m_TabStreamTabCtrl.m_tabPages[0] = new CStream_Watch_Tab1(NULL,this);
	m_TabStreamTabCtrl.m_tabPages[1] = new CStream_Watch_Tab2(NULL,this);
	m_TabStreamTabCtrl.m_tabPages[2] = new CStream_Watch_Tab3(NULL,this);
	m_TabStreamTabCtrl.m_tabPages[3] = new CStream_Watch_Tab4(NULL,this);
	
	//tab Intialization
	m_TabStreamTabCtrl.InsertItem(0, _T("Stream 1 - 16"));
	m_TabStreamTabCtrl.InsertItem(1, _T("Stream 17 - 32"));
	m_TabStreamTabCtrl.InsertItem(2, _T("Stream 33 - 48"));
	m_TabStreamTabCtrl.InsertItem(3, _T("    Status    "));

	m_TabStreamTabCtrl.Init();

	ReadXmlOnce();
	
	m_pic_Notify[0].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_GREEN), RT_BITMAP);
	m_pic_Notify[1].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_RED), RT_BITMAP);
	m_pic_Notify[2].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_LIGHTGREEN), RT_BITMAP);
	m_pic_Notify[3].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_ORANGE), RT_BITMAP);

	m_pToolTip = new CToolTipCtrl;
	m_pToolTip->Create(this);
	m_pToolTip->AddTool(GetDlgItem(IDC_BUTTON_ALERT_CONFIG),_T("Email Alert Configration."));
	m_pToolTip->AddTool(GetDlgItem(IDC_CHECKED_ALL),_T("Click to select all Channels."));
	m_pToolTip->AddTool(GetDlgItem(IDC_BUTTON_STOP_ALL),_T("Click to stop all running Channels."));
	m_pToolTip->AddTool(GetDlgItem(IDC_BUTTON_START_ALL),_T("Click to start all selected Channels."));
	m_pToolTip->AddTool(GetDlgItem(IDOK),_T("Click to close Application."));
	m_pToolTip->Activate(TRUE);

	FILE *fIndex = NULL;
	TCHAR Index_Stream[10];
	memset(Index_Stream, 0, sizeof(Index_Stream));
	TCHAR tempPath[MAX_PATH];
	memset(tempPath, 0, MAX_PATH);
	::GetModuleFileName(0, tempPath, MAX_PATH);
	unsigned int loc = (unsigned int)(_tcsrchr(tempPath, _T('\\')) - tempPath);
	tempPath[loc] = NULL;

	TCHAR temppathSendstop[MAX_PATH]; 
	_stprintf_s(temppathSendstop,MAX_PATH, _T("%s\\SendStop.txt"), tempPath);
	 _tfopen_s(&fIndex,temppathSendstop, _T("r"));

	if(fIndex != NULL)
	{
		fclose(fIndex);
		bStopUDP = true;
	}
	else
		bStopUDP = false;

	//TCHAR temppathSendstop[MAX_PATH]; 
	_stprintf_s(temppathSendstop,MAX_PATH, _T("%s\\VideoAnalysis.txt"), tempPath);
	 _tfopen_s(&fIndex,temppathSendstop, _T("r"));

	if(fIndex != NULL)
	{
		fclose(fIndex);
		bStartVideoAnalysis = true;
	}
	else
		bStartVideoAnalysis = false;

	//TCHAR temppathSendstop[MAX_PATH]; 
	_stprintf_s(temppathSendstop,MAX_PATH, _T("%s\\Emergency.txt"), tempPath);
	 _tfopen_s(&fIndex,temppathSendstop, _T("r"));

	if(fIndex != NULL)
	{
		fclose(fIndex);
		bEMERGENCY = true;
	}
	else
		bEMERGENCY = false;

	if(bStopUDP)
	{
		DisableSecondarySource();
		DiableDestination();
	}

	TCHAR strTempPath[500];
	_stprintf(strTempPath, _T("%s\\SystemName.txt"),tempPath);
	readEmailSystemName(strTempPath);
	if(m_nLiecensedStream > 0)
	{		
		int nTab_Id = 0;
		for(int i = m_nLiecensedStream; i < MAX_STREAM; i++)
		{
			int nResource_Id = 201 + (i * 20);
			nTab_Id = i / 16;
			((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(213 + (i * 20)))->SetCheck(0);
			for(int p = 0; p < 20; p++)
			{
				((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++))->EnableWindow(false);	
			}
			((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2401 + i))->SetCheck(0);
			((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2401 + i))->EnableWindow(false);
			((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2501 + i))->SetCheck(0);
			((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2501 + i))->EnableWindow(false);
			((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2301 + i))->EnableWindow(false);
			((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2551 + i))->SetCheck(0);
			((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2551 + i))->EnableWindow(false);
		}
	}

	if(!bEMERGENCY)
	{
		int nTemp_Id, nTab_Id;
		((CButton*)m_TabStreamTabCtrl.m_tabPages[0]->GetDlgItem(Emergency1))->ShowWindow(false);
		((CButton*)m_TabStreamTabCtrl.m_tabPages[0]->GetDlgItem(EMEBorder1))->ShowWindow(false);
		((CButton*)m_TabStreamTabCtrl.m_tabPages[1]->GetDlgItem(Emergency2))->ShowWindow(false);
		((CButton*)m_TabStreamTabCtrl.m_tabPages[1]->GetDlgItem(EMEBorder2))->ShowWindow(false);
		((CButton*)m_TabStreamTabCtrl.m_tabPages[2]->GetDlgItem(Emergency3))->ShowWindow(false);
		((CButton*)m_TabStreamTabCtrl.m_tabPages[2]->GetDlgItem(EMEBorder3))->ShowWindow(false);
		
		((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(EmergencyS1))->ShowWindow(false);
		((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(EmergencyS2))->ShowWindow(false);
		((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(EmergencyS3))->ShowWindow(false);
		((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(EMEBorderS1))->ShowWindow(false);
		((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(EMEBorderS1))->ShowWindow(false);
		((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(EMEBorderS1))->ShowWindow(false);

		for(int k = 0 ; k < MAX_STREAM ; k++)
		{
			int nResource_Id = 220 + (k * 20);
			nTab_Id = k / 16;

			((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(220 + (k * 20)))->ShowWindow(false);
			((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2551 + k))->ShowWindow(false);
		}
	}

	//((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(220 + (k * 20)))->SetCheck(StreamWatch[k].m_Input[0].m_bEmergencyActive);
	//((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2551 + k))->SetCheck(StreamWatch[k].m_Input[0].m_bEmergencyActive);
	
	bmp.LoadBitmap(IDB_BITMAP_RESET);
	for(int i = 0; i < MAX_STREAM; i++)
	{
		pButton = (CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2651 + i);
		pButton->ModifyStyle(0,BS_BITMAP);
		pButton->SetBitmap(bmp);

		int nTab_Id = i / 16;
		((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(1171 + i))->EnableWindow(false);
	}

	/*if(!bStopUDP)
	{
		for(int i = 0; i < MAX_STREAM; i++)
		{
			m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2601 + i)->EnableWindow(false);
			m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2651 + i)->EnableWindow(false);
		}
	}*/
	UpdateData(false);

	if(m_nTotalStartedChannel > 0)
	{
		//m_nTotalStartStream = m_nTotalStartedChannel;
		OnBnClickedButtonStartAll();
	}
	
     SetWindowPos(NULL, 10, 10, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	bool exist = IsProcessRunning(L"SendMail.exe");
	if(!exist)
	{
		TCHAR Buffer[MAX_PATH];
		TCHAR folderpath[MAX_PATH];
		DWORD dwRet;
		dwRet = GetCurrentDirectory(MAX_PATH, Buffer);
		_stprintf_s(folderpath, MAX_PATH, _T("%s\\SendMail.exe"), Buffer);
		SHELLEXECUTEINFO InterServerExecution = {0};
		InterServerExecution.cbSize = sizeof(SHELLEXECUTEINFO);
		InterServerExecution.fMask = SEE_MASK_NOCLOSEPROCESS;
		InterServerExecution.hwnd = NULL;
		InterServerExecution.lpVerb = NULL;
		InterServerExecution.lpFile = folderpath;		
		InterServerExecution.lpParameters = _T("");
		InterServerExecution.lpDirectory = NULL;
		InterServerExecution.nShow = SW_HIDE;
		InterServerExecution.hInstApp = NULL;	
		ShellExecuteEx(&InterServerExecution);
		WaitForSingleObject(InterServerExecution.hProcess,100);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}
BOOL CStream_WatchDlg::PreTranslateMessage(MSG* pMsg)
{
	if (NULL != m_pToolTip)
		m_pToolTip->RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}
void CStream_WatchDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CStream_WatchDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CStream_WatchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CStream_WatchDlg::OnBnClickedOk()
{
	OnClose();
}

void CStream_WatchDlg::OnClose()
{
	const int BUF_SIZE = 100;
	const int STARTSTOPBUF_SIZE = 800;
	int nTab_Id = 0; 
	int nTemp_Id = 0;
	int nCheck_StreamStatus = 0;

	CTabGeneral *pTabGolbalPtr = NULL;
	
	TCHAR strStartStopBuf[STARTSTOPBUF_SIZE];
	memset(strStartStopBuf, 0, STARTSTOPBUF_SIZE*sizeof(TCHAR));

	TCHAR strMsg[BUF_SIZE];
	memset(strMsg, 0, BUF_SIZE*sizeof(TCHAR));
	StoreCheckboxData();
	
	for(int k = 0; k < MAX_STREAM && k < m_nLiecensedStream; k++)
	{
		if(StreamWatch[k].m_nCheck_EngineStatus == true)
		{
			nCheck_StreamStatus = 1;
			break;
		}
	}
	if(nCheck_StreamStatus == 1)
	{
		_stprintf_s(strMsg, BUF_SIZE, _T("Do you want to stop streaming and exit application?"));
		
		if(IDNO == MessageBox(strMsg, _T("StreamWatch"), MB_ICONWARNING | MB_YESNO | MB_TOPMOST))
			return;
		else
		{
			StorePrevStreamStatus();
			StoreSecondaryPrgNo();   //Replacement as per selected prg no
			for(int k = 0; k < MAX_STREAM && k < m_nLiecensedStream; k++)
			{
				if(k >= 0 && k < 16)
				{
					pTabGolbalPtr = &(((CStream_Watch_Tab1*)m_TabStreamTabCtrl.m_tabPages[0])->General_Tab1);
				}
				if(k >= 16 && k < 32)
				{
					pTabGolbalPtr = &(((CStream_Watch_Tab2*)m_TabStreamTabCtrl.m_tabPages[1])->General_Tab2);
				}
				if(k >= 32 && k < 48)
				{
		
					pTabGolbalPtr = &(((CStream_Watch_Tab3*)m_TabStreamTabCtrl.m_tabPages[2])->General_Tab3);
				}
				if(StreamWatch[k].m_nCheck_EngineStatus == true)
				{
					StreamWatch[k].m_Output.m_bThread_Terminate = true;
					StreamWatch[k].m_Input[0].m_bScannar_Terminate = true;
					StreamWatch[k].m_Input[1].m_bScannar_Terminate = true;
					StreamWatch[k].m_nCheck_EngineStatus = false;
				
					StreamWatch[k].CLEANUP_ALL();
					MonitorExe();
					OnOK();	
					
					_stprintf_s(strStartStopBuf, STARTSTOPBUF_SIZE, _T("STOP STREAM ---->PrimAdopter: %s PrimIPAddr: %s PrimPort: %d SecondAdopter: %s SecondIPAddr: %s SecondPort: %d DestAdopter: %s DestIPAddr: %s DestPort: %d"), pTabGolbalPtr->m_strNic_Source[nTemp_Id][0], pTabGolbalPtr->m_strIp_Source[nTemp_Id][0], pTabGolbalPtr->m_nPort_Source[nTemp_Id][0], pTabGolbalPtr->m_strNic_Source[nTemp_Id][1], pTabGolbalPtr->m_strIp_Source[nTemp_Id][1], pTabGolbalPtr->m_nPort_Source[nTemp_Id][1], pTabGolbalPtr->m_strNic_Dest[nTemp_Id], pTabGolbalPtr->m_strIp_Dest[nTemp_Id], pTabGolbalPtr->m_nPort_Dest[nTemp_Id]);
					LogStartStopEvents(strStartStopBuf, k);
				}
			}
		}
	}
	else
	{
		_stprintf_s(strMsg, BUF_SIZE, _T("Do you want to exit application?"));
		if(IDNO == MessageBox(strMsg, _T("StreamWatch"), MB_ICONWARNING | MB_YESNO | MB_TOPMOST))
			return;
		else
		{
			StorePrevStreamStatus();
			StoreSecondaryPrgNo();   //Replacement as per selected prg no
			MonitorExe();
			OnOK();	
		}
	}
}

void CStream_WatchDlg::OnBnClickedButtonStartAll()
{
	const int BUF_SIZE = 100;
	const int STARTSTOPBUF_SIZE = 800;
	int nTab_Id = 0; 
	int	nTemp_Id = 0;
	int nCheckBoxStatus = 0;
	bClickeStartAll = true;

	CTabGeneral *pTabGolbalPtr = NULL;
	
	TCHAR strStartStopBuf[STARTSTOPBUF_SIZE];
	memset(strStartStopBuf, 0, STARTSTOPBUF_SIZE*sizeof(TCHAR));
	DWORD StartTickCount = GetTickCount();

	for(int i = 0 ; i < MAX_STREAM && i < m_nLiecensedStream; i++)
	{
		nTemp_Id = i % 16;
		nTab_Id = i / 16;
		if(i >= 0 && i < 16)
		{
			pTabGolbalPtr = &(((CStream_Watch_Tab1*)m_TabStreamTabCtrl.m_tabPages[0])->General_Tab1);
		}
		if(i >= 16 && i < 32)
		{
			pTabGolbalPtr = &(((CStream_Watch_Tab2*)m_TabStreamTabCtrl.m_tabPages[1])->General_Tab2);
		}
		if(i >= 32 && i < 48)
		{
			pTabGolbalPtr = &(((CStream_Watch_Tab3*)m_TabStreamTabCtrl.m_tabPages[2])->General_Tab3);
		}
		nCheckBoxStatus = ((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem((213 + (20 * i))))->GetCheck();
		if(nCheckBoxStatus == 1)
		{
			if(StreamWatch[i].m_nCheck_EngineStatus == false)
			{
				StreamWatch[i].m_Output.m_bSendTo_Status = false;
				StreamWatch[i].m_Input[0].m_dwLastUpdateTime = StartTickCount; //0;
				StreamWatch[i].m_Input[1].m_dwLastUpdateTime = 0;
				StreamWatch[i].m_dwStartClickTime = GetTickCount();
				
				int check = StreamStart(i);
				if(check == 1)
				{
					m_nTotalStartStream++;
					if(m_nTotalStartStream == 1)
					{
							SetTimer(IDT_TIMER_STREAM_STATUS, 1000, NULL); 
							SetTimer(IDT_TIMER_ALERT, 1000, NULL);
							//SetTimer(IDT_TIMER_CCERROR, 300000, NULL);
							SetTimer(IDT_TIMER_CCERROR, 180000, NULL);
					}
					
					//Sleep(3000);

					DisableControl(i);

					_stprintf_s(strStartStopBuf, STARTSTOPBUF_SIZE, _T("START STREAM---->PrimAdopter: %s PrimIPAddr: %s PrimPort: %d SecondAdopter: %s SecondIPAddr: %s SecondPort: %d DestAdopter: %s DestIPAddr: %s DestPort: %d"), pTabGolbalPtr->m_strNic_Source[nTemp_Id][0], pTabGolbalPtr->m_strIp_Source[nTemp_Id][0], pTabGolbalPtr->m_nPort_Source[nTemp_Id][0], pTabGolbalPtr->m_strNic_Source[nTemp_Id][1], pTabGolbalPtr->m_strIp_Source[nTemp_Id][1], pTabGolbalPtr->m_nPort_Source[nTemp_Id][1], pTabGolbalPtr->m_strNic_Dest[nTemp_Id], pTabGolbalPtr->m_strIp_Dest[nTemp_Id], pTabGolbalPtr->m_nPort_Dest[nTemp_Id]);
					LogStartStopEvents(strStartStopBuf, i);
				}
			}
		}
	}
	if(TotalCheckedBox == 0)
	{
		MessageBox(_T("Choose atleast One CheckBox "), _T("ERROR"), MB_OK);
	}

	if(TotalCheckedBox > 0 && m_nTotalStartStream > 0)
	{
		GetDlgItem(IDC_BUTTON_START_ALL)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_STOP_ALL)->EnableWindow(true);
	}
	
	//if(m_nTotalStartStream > 0)
		//SetTimer(IDT_TIMER_CCERROR, 300000, NULL);

	if(m_nTotalStartStream == m_nLiecensedStream)
	{
		GetDlgItem(IDC_CHECKED_ALL)->EnableWindow(false);
	}
	m_nTotalStartedChannel = m_nTotalStartStream;
	CreateXmlFile();
}

WaitingDlg* CurrentWaitingDialog;

DWORD WINAPI InitWaiting(LPVOID lpParameter)
{
	CurrentWaitingDialog = new WaitingDlg(NULL);
	
	if(CurrentWaitingDialog != NULL)
	{
		CurrentWaitingDialog->DoModal();
	}
	return 0;
}

DWORD WINAPI StopWaiting(LPVOID lpParameter)
{
	if(CurrentWaitingDialog != NULL)
	{
		CurrentWaitingDialog->OnClose();
		//CurrentWaitingDialog->DestroyWindow();
	}
	if(CurrentWaitingDialog)
	{
		SAFE_DELETE(CurrentWaitingDialog);
	}

	return 0;
}

void CStream_WatchDlg::OnBnClickedButtonStopAll()
{
	const int STARTSTOPBUF_SIZE = 800;
	int nTemp_Id = 0, nTab_Id = 0;
	bClickeStartAll = false;

	CTabGeneral *pTabGolbalPtr = NULL;
	CStatusMembers *pStatusPtr = NULL;
	
	TCHAR strStartStopBuf[STARTSTOPBUF_SIZE];
	memset(strStartStopBuf, 0, STARTSTOPBUF_SIZE * sizeof(TCHAR));
	
	if(m_nTotalStartStream > 0)
	{
		if(IDNO == MessageBox(_T("Do you want to stop all streaming?"), _T("StreamWatch"), MB_ICONWARNING | MB_YESNO | MB_TOPMOST))
			return;
		else
		{
			pStatusPtr = &(((CStream_Watch_Tab4*)m_TabStreamTabCtrl.m_tabPages[3])->StatusTab);

			DWORD WaitingDLGThreadID = 0;
			HANDLE WaitingDLGHandle = INVALID_HANDLE_VALUE;

			WaitingDLGHandle = CreateThread(0, 0, InitWaiting , 0 , 0, &WaitingDLGThreadID);

			for(int k = 0; k < MAX_STREAM && k < m_nLiecensedStream; k++)
			{
				nTemp_Id = k % 16;
				nTab_Id = k / 16;

				if(k >= 0 && k < 16)
				{
					pTabGolbalPtr = &(((CStream_Watch_Tab1*)m_TabStreamTabCtrl.m_tabPages[0])->General_Tab1);
				}
				if(k >= 16 && k < 32)
				{
					pTabGolbalPtr = &(((CStream_Watch_Tab2*)m_TabStreamTabCtrl.m_tabPages[1])->General_Tab2);
				}
				if(k >= 32 && k < 48)
				{
					pTabGolbalPtr = &(((CStream_Watch_Tab3*)m_TabStreamTabCtrl.m_tabPages[2])->General_Tab3);
				}

				if(!bStopUDP)
				{
					for(int j = 0; j < MAX_RECEIVER; j++)
					{
						if(StreamWatch[k].m_Input[j].m_nTotalProgNo > 1/*&& bStop[k] == false*/)
						{
							((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(1171 + k))->EnableWindow(true);
							//CString strPrgNo;
							pTabGolbalPtr->m_cmbPrgNo[k].ResetContent();
							for(int i = 0; i < StreamWatch[k].m_Input[j].m_nTotalProgNo; i++)
							{
								StreamWatch[k].m_strPrgNo_List[i].Format(_T("%d"),StreamWatch[k].m_Input[j].m_nProgramNo_Buff[i]);
								if(StreamWatch[k].m_strPrgNo_List[i] != _T("0"))
									pTabGolbalPtr->m_cmbPrgNo[k].AddString(StreamWatch[k].m_strPrgNo_List[i]);
							}
							pTabGolbalPtr->m_cmbPrgNo[k].SetCurSel(0);
							//bStop[k] = true;
							StreamWatch[k].m_bStart = true;
						}
					}
					StreamWatch[k].m_bSelected_PrgNo = false;
				}

				//Engine Stop Process
				if(StreamWatch[k].m_nCheck_EngineStatus == true)
				{
					StreamWatch[k].m_nCheck_EngineStatus = false;
					StreamWatch[k].m_Output.m_bThread_Terminate = true;
					StreamWatch[k].m_Input[0].m_bScannar_Terminate = true;
					StreamWatch[k].m_Input[1].m_bScannar_Terminate = true;
					StreamWatch[k].m_Input[0].m_bReset = true;
					StreamWatch[k].m_Input[1].m_bReset = true;
					
					--m_nTotalStartStream;

					EnableControl(k);
					StreamWatch[k].CLEANUP_ALL();	
					
					pTabGolbalPtr->m_PriStatus[nTemp_Id].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
		            pTabGolbalPtr->m_SecondaryStatus[nTemp_Id].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
					pTabGolbalPtr->m_OtputStatus[nTemp_Id].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);

					pStatusPtr->m_pic_Prime_Stream[k].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
		            pStatusPtr->m_pic_Second_Stream[k].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
					pStatusPtr->m_pic_Output_Stream[k].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);

					_stprintf_s(strStartStopBuf, STARTSTOPBUF_SIZE, _T("STOP STREAM---->PrimAdopter: %s PrimIPAddr: %s PrimPort: %d SecondAdopter: %s SecondIPAddr: %s SecondPort: %d DestAdopter: %s DestIPAddr: %s DestPort: %d"), pTabGolbalPtr->m_strNic_Source[nTemp_Id][0], pTabGolbalPtr->m_strIp_Source[nTemp_Id][0], pTabGolbalPtr->m_nPort_Source[nTemp_Id][0], pTabGolbalPtr->m_strNic_Source[nTemp_Id][1], pTabGolbalPtr->m_strIp_Source[nTemp_Id][1], pTabGolbalPtr->m_nPort_Source[nTemp_Id][1], pTabGolbalPtr->m_strNic_Dest[nTemp_Id], pTabGolbalPtr->m_strIp_Dest[nTemp_Id], pTabGolbalPtr->m_nPort_Dest[nTemp_Id]);
					LogStartStopEvents(strStartStopBuf, k);
				}
				m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem((213 + (20 * k)))->EnableWindow(true);
			}
				
			if(WaitingDLGHandle)
				CloseHandle(WaitingDLGHandle);

			WaitingDLGHandle = CreateThread(0, 0, StopWaiting ,0 , 0, &WaitingDLGThreadID);

			GetDlgItem(IDC_CHECKED_ALL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STOP_ALL)->EnableWindow(FALSE);

			if(TotalCheckedBox >= 2)
			{
				GetDlgItem(IDC_BUTTON_START_ALL)->EnableWindow(true);
			}
			if(m_nTotalStartStream == 0)
			{
				KillTimer(IDT_TIMER_STREAM_STATUS);
				KillTimer(IDT_TIMER_ALERT);
				KillTimer(IDT_TIMER_CCERROR);
			}
		}
	}
	else
	{
		for(int k = 0; k < MAX_STREAM && k < m_nLiecensedStream; k++)
		{
			nTab_Id = k / 16;
			nTemp_Id = k % 16;
			pStatusPtr = &(((CStream_Watch_Tab4*)m_TabStreamTabCtrl.m_tabPages[3])->StatusTab);
			m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem((213 + (20 * k)))->EnableWindow(true);
			if(k >= 0 && k < 16)
			{
				pTabGolbalPtr = &(((CStream_Watch_Tab1*)m_TabStreamTabCtrl.m_tabPages[0])->General_Tab1);
			}
			if(k >= 16 && k < 32)
			{
				pTabGolbalPtr = &(((CStream_Watch_Tab2*)m_TabStreamTabCtrl.m_tabPages[1])->General_Tab2);
			}
			if(k >= 32 && k < 48)
			{
				pTabGolbalPtr = &(((CStream_Watch_Tab3*)m_TabStreamTabCtrl.m_tabPages[2])->General_Tab3);
			}
			pTabGolbalPtr->m_PriStatus[nTemp_Id].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
            pTabGolbalPtr->m_SecondaryStatus[nTemp_Id].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
			pTabGolbalPtr->m_OtputStatus[nTemp_Id].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);

			pStatusPtr->m_pic_Prime_Stream[k].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
            pStatusPtr->m_pic_Second_Stream[k].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
			pStatusPtr->m_pic_Output_Stream[k].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
		}
		GetDlgItem(IDC_BUTTON_STOP_ALL)->EnableWindow(FALSE);
		if(TotalCheckedBox >= 2)
		{
			GetDlgItem(IDC_BUTTON_START_ALL)->EnableWindow(true);
		}
		KillTimer(IDT_TIMER_STREAM_STATUS);
		KillTimer(IDT_TIMER_ALERT);
		KillTimer(IDT_TIMER_CCERROR);
	}
	m_nTotalStartedChannel = m_nTotalStartStream;
	//CreateXmlFile();
	LoadXML_Started_channels();
}

CString CStream_WatchDlg::GetAppPath(HMODULE hModule = NULL)
{
	TCHAR buf[MAX_PATH] = {'\0'};
	CString strDir, strTemp;

	::GetModuleFileName( hModule, buf, MAX_PATH);
	strTemp = buf;
	strDir = strTemp.Left( strTemp.ReverseFind('\\') + 1 );

	return strDir;
}

void CStream_WatchDlg::CreateXmlFile()
{
	CString strXml;
	strXml = _T("<?xml version=\"1.0\"?>")
			 _T("<StreamWatch>")
			 _T("\n")
			 _T("</StreamWatch>");

	CXml xml;
	CString strFilePath;
	if( xml.LoadXml(strXml) )
	{
		// if the xml string loaded, then save to the file
		strFilePath.Format( _T("%s\\StreamWatchConfigFile.xml"), GetAppPath());
		::DeleteFile(strFilePath);

		xml.Save(strFilePath);
		xml.Close();
	}

	bXmlFileStatus = true;
	LoadXml();
}

CString CStream_WatchDlg::LoadXml()
{
	CXml xml;
	CString strFilePath;

	strFilePath.Format( _T("%s\\StreamWatchConfigFile.xml"), GetAppPath());
	CString strMsg;

	if( xml.Open(strFilePath) )
	{
		CString strRoot = L"";
		for(int i = 0 ; i < MAX_STREAM && i < m_nLiecensedStream ; i++)
		{
			strRoot.Format(_T("Stream_%02d"),i+1);

			if(bEMERGENCY)
				xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("PID_Replacement_status"), TRUE)->SetValue(StreamWatch[i].m_Input[0].m_bEmergencyActive);

			xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("Manual_Selection_status"), TRUE)->SetValue(StreamWatch[i].m_bManualCheckStatus);
			xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("Selected_For_Detection"), TRUE)->SetValue(StreamWatch[i].m_nCheckBox_Status);
			xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("ChannelName"), TRUE)->SetValue(StreamWatch[i].m_strCstreamChannel_Name);
			xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("Prime_Input_Absent_Duration"), TRUE)->SetValue(StreamWatch[i].m_nDetection_Time);
			xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("Prime_Input_Present_Duration"), TRUE)->SetValue(StreamWatch[i].m_nPreDetection_Time);

			xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("PrimaryInput"), TRUE)->GetChild(_T("Network_Adaptor"), TRUE)->SetValue(StreamWatch[i].m_Input[0].strNic_Source);
			xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("PrimaryInput"), TRUE)->GetChild(_T("IP_Address"), TRUE)->SetValue(StreamWatch[i].m_Input[0].strIp_Source);
			xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("PrimaryInput"), TRUE)->GetChild(_T("Port_Number"), TRUE)->SetValue(StreamWatch[i].m_Input[0].nPort_Source); 
			xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("PrimaryInput"), TRUE)->GetChild(_T("Stream_Prev_Status"), TRUE)->SetValue(m_nAlertCompairPrime[i]);

			xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("SecondaryInput"), TRUE)->GetChild(_T("Network_Adaptor"), TRUE)->SetValue(StreamWatch[i].m_Input[1].strNic_Source);
			xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("SecondaryInput"), TRUE)->GetChild(_T("IP_Address"), TRUE)->SetValue(StreamWatch[i].m_Input[1].strIp_Source);
			xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("SecondaryInput"), TRUE)->GetChild(_T("Port_Number"), TRUE)->SetValue(StreamWatch[i].m_Input[1].nPort_Source); 
			xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("SecondaryInput"), TRUE)->GetChild(_T("Stream_Prev_Status"), TRUE)->SetValue(m_nAlertCompairSecond[i]);

			xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("Output"), TRUE)->GetChild(_T("Network_Adaptor"), TRUE)->SetValue(StreamWatch[i].m_Output.strNic_Source);
			xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("Output"), TRUE)->GetChild(_T("IP_Address"), TRUE)->SetValue(StreamWatch[i].m_Output.strIp_Source);
			xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("Output"), TRUE)->GetChild(_T("Port_Number"), TRUE)->SetValue(StreamWatch[i].m_Output.nPort_Source);
			
		}
		xml.GetRoot()->GetChild(_T("Alert"), TRUE)->GetChild(_T("Alert_Duration_Time"), TRUE)->SetValue(AlertDlg.AlertData.m_nAlertTime);
		xml.GetRoot()->GetChild(_T("Alert"), TRUE)->GetChild(_T("TotalListMailId"), TRUE)->SetValue(AlertDlg.m_nTotalEmailIdList);
		xml.GetRoot()->GetChild(_T("Alert"), TRUE)->GetChild(_T("TotalMailId"), TRUE)->SetValue(AlertDlg.m_nTotalEmailId);
		xml.GetRoot()->GetChild(_T("Alert"), TRUE)->GetChild(_T("UserName"), TRUE)->SetValue(AlertDlg.AlertData.m_strAlertUserName);
		xml.GetRoot()->GetChild(_T("Alert"), TRUE)->GetChild(_T("Password"), TRUE)->SetValue(AlertDlg.AlertData.m_strAlertPassword);
		xml.GetRoot()->GetChild(_T("Alert"), TRUE)->GetChild(_T("SmtpServer"), TRUE)->SetValue(AlertDlg.AlertData.m_strAlertSMTP);

		CString strReceiver_Email = L"";
		CString strEmail_List = L"";
		for(int i = 0; i < AlertDlg.m_nTotalEmailIdList; i++)
		{
			strEmail_List.Format(_T("strEmail_List%02d"),i);
			xml.GetRoot()->GetChild(_T("Alert"), TRUE)->GetChild(strEmail_List, TRUE)->SetValue(AlertDlg.AlertData.m_strAlertEmailList[i]);
		}
		for(int i = 0; i < AlertDlg.m_nTotalEmailId; i++)
		{
			strReceiver_Email.Format(_T("Receiver_Email_%02d"),i);
			xml.GetRoot()->GetChild(_T("Alert"), TRUE)->GetChild(strReceiver_Email, TRUE)->SetValue(AlertDlg.AlertData.m_strAlertReceiverEmailId[i]);
		}
		xml.GetRoot()->GetChild(_T("Streamwatch_channel_info"), TRUE)->GetChild(_T("Total_started_channels"), TRUE)->SetValue(m_nTotalStartedChannel);
		
		xml.Save(strFilePath);
		xml.Close();
	}
	return strFilePath;
}

void CStream_WatchDlg::ReadXmlOnce()
{
	int nTemp_Id = 0; 
	int nTab_Id = 0;
	int n_TotalNic = 0;
	int n_lengthNic = 0;
	CString strCompareNic;
	
	CTabGeneral *pTabGolbalPtr = NULL;
	CStatusMembers *pStatusPtr = NULL;
	
	pStatusPtr = &(((CStream_Watch_Tab4*)m_TabStreamTabCtrl.m_tabPages[3])->StatusTab);

	if(ReadXml())
	{
		bXmlFileStatus = true;
		for(int k = 0 ; k < MAX_STREAM && k < m_nLiecensedStream; k++)
		{
			nTemp_Id = k % 16;

			if(k >= 0 && k < 16)
			{
				pTabGolbalPtr = &(((CStream_Watch_Tab1*)m_TabStreamTabCtrl.m_tabPages[0])->General_Tab1);
				nTab_Id = 0;
			}
			if(k >= 16 && k < 32)
			{
				if(k == 16)
				{
					m_TabStreamTabCtrl.m_tabPages[nTab_Id]->UpdateData(false);
				}
				pTabGolbalPtr = &(((CStream_Watch_Tab2*)m_TabStreamTabCtrl.m_tabPages[1])->General_Tab2);
				nTab_Id = 1;
			}
			if(k >= 32 && k < 48)
			{
				if(k == 32)
				{
					m_TabStreamTabCtrl.m_tabPages[nTab_Id]->UpdateData(false);
				}
				pTabGolbalPtr = &(((CStream_Watch_Tab3*)m_TabStreamTabCtrl.m_tabPages[2])->General_Tab3);
				nTab_Id = 2;
			}

			//assigned memebers varible data from xml read File 
			StreamWatch[k].SetIndex(k);
			pTabGolbalPtr->m_strChannel_Name[nTemp_Id] = StreamWatch[k].m_strCstreamChannel_Name;
			pStatusPtr->m_strStatusChannel_Name[k] = pTabGolbalPtr->m_strChannel_Name[nTemp_Id];

			for(int i = 0; i < MAX_RECEIVER; i++)
			{
				pTabGolbalPtr->m_strNic_Source[nTemp_Id][i] = StreamWatch[k].m_Input[i].strNic_Source;
				//Count the droplist Item and check for pertcular value ans set its focus
				n_TotalNic = pTabGolbalPtr->m_cmbSourceAdapter[nTemp_Id][i].GetCount();
				for(int j = 0; j < n_TotalNic; j++)
				{
					 n_lengthNic = (pTabGolbalPtr->m_cmbSourceAdapter[nTemp_Id][i]).GetLBTextLen(j);
					 (pTabGolbalPtr->m_cmbSourceAdapter[nTemp_Id][i]).GetLBText(j, strCompareNic.GetBuffer(n_lengthNic));
					 if(pTabGolbalPtr->m_strNic_Source[nTemp_Id][i] == strCompareNic)
					 {
						 pTabGolbalPtr->m_cmbSourceAdapter[nTemp_Id][i].SetCurSel(j);
					 }
				}
				pTabGolbalPtr->m_strIp_Source[nTemp_Id][i] = StreamWatch[k].m_Input[i].strIp_Source;
				pTabGolbalPtr->m_nPort_Source[nTemp_Id][i] = StreamWatch[k].m_Input[i].nPort_Source;
			}

			pTabGolbalPtr->m_strNic_Dest[nTemp_Id] = StreamWatch[k].m_Output.strNic_Source;
			n_TotalNic = pTabGolbalPtr->m_cmbDestAdapter[nTemp_Id].GetCount();
			for(int j = 0; j < n_TotalNic; j++)
			{
				 n_lengthNic = (pTabGolbalPtr->m_cmbDestAdapter[nTemp_Id]).GetLBTextLen(j);
				 (pTabGolbalPtr->m_cmbDestAdapter[nTemp_Id]).GetLBText(j, strCompareNic.GetBuffer(n_lengthNic));
				 if(pTabGolbalPtr->m_strNic_Dest[nTemp_Id] == strCompareNic)
				 {
					 pTabGolbalPtr->m_cmbDestAdapter[nTemp_Id].SetCurSel(j);
				 }
			}
			pTabGolbalPtr->m_strIp_Dest[nTemp_Id] = StreamWatch[k].m_Output.strIp_Source;
			pTabGolbalPtr->m_nPort_Dest[nTemp_Id] = StreamWatch[k].m_Output.nPort_Source;
			pTabGolbalPtr->m_m_nDetection_Time[nTemp_Id] = StreamWatch[k].m_nDetection_Time;
			StreamWatch[k].m_Input[0].m_nDetect_Time = StreamWatch[k].m_nDetection_Time;
			StreamWatch[k].m_Input[1].m_nDetect_Time = StreamWatch[k].m_nDetection_Time;
			pTabGolbalPtr->m_nPresentDetection_Time[nTemp_Id] = StreamWatch[k].m_nPreDetection_Time;

			if(StreamWatch[k].m_nCheckBox_Status == 1)
			{
				((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(213 + (k * 20)))->SetCheck(1);
				((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2401 + k))->SetCheck(1);

				TotalCheckedBox++;

				m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(211 + (20 * k))->EnableWindow(true);
				m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2301 + k)->EnableWindow(true);

				if(TotalCheckedBox == m_nLiecensedStream)
				{
					((CButton*)GetDlgItem(IDC_CHECKED_ALL))->SetCheck(1);
				}
			}
			else
			{
				m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(211 + (20 * k))->EnableWindow(false);
				m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2301 + k)->EnableWindow(false);
			}
			((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(217 + (k * 20)))->SetCheck(StreamWatch[k].m_bManualCheckStatus);
			((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2501 + k))->SetCheck(StreamWatch[k].m_bManualCheckStatus);
			if(bEMERGENCY)
			{
				((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(220 + (k * 20)))->SetCheck(StreamWatch[k].m_Input[0].m_bEmergencyActive);
				((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2551 + k))->SetCheck(StreamWatch[k].m_Input[0].m_bEmergencyActive);
			}
			if(TotalCheckedBox >= 2)
			{
				GetDlgItem(IDC_BUTTON_START_ALL)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_STOP_ALL)->EnableWindow(false);
			}
			else
			{
				GetDlgItem(IDC_BUTTON_START_ALL)->EnableWindow(false);
				GetDlgItem(IDC_BUTTON_STOP_ALL)->EnableWindow(false);
			}

			//Replacement as per selected prg no
			for(int i = 0; i < StreamWatch[k].m_nTotal_PrgNo; i++)
			{
				pTabGolbalPtr->m_cmbPrgNo[k].AddString(StreamWatch[k].m_strPrgNo_List[i]);
				pTabGolbalPtr->m_cmbPrgNo[k].SetCurSel(0);	
				/*if(nSelected_PrgNo[k] == _ttoi(strPrgNo_List[k][i]))
					pTabGolbalPtr->m_cmbPrgNo[k].SetCurSel(Index);	*/
			}
			
		}
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->UpdateData(false);
		m_TabStreamTabCtrl.m_tabPages[3]->UpdateData(false);
		UpdateData(false);
	}
}

bool CStream_WatchDlg::ReadXml()
{
	bool bResult = false;

	CString strFilePath;
	CString strMsg;

	strFilePath.Format( _T("%s\\StreamWatchConfigFile.xml"), GetAppPath());
	
	CXml xml;
	if( xml.Open(strFilePath) )
	{

		CString strRoot = L"";
		for(int i = 0 ; i < MAX_STREAM && i < m_nLiecensedStream ; i++)
		{
			strRoot.Format(_T("Stream_%02d"),i+1);
		
			if(bEMERGENCY)
				StreamWatch[i].m_Input[0].m_bEmergencyActive = xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("PID_Replacement_status"))->GetValue(0);

			StreamWatch[i].m_bManualCheckStatus = xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("Manual_Selection_status"))->GetValue(0);
			StreamWatch[i].m_nCheckBox_Status = xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("Selected_For_Detection"), TRUE)->GetValue(0);
			_stprintf_s(StreamWatch[i].m_strCstreamChannel_Name, 20, _T("%s"), xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("ChannelName"), TRUE)->GetValue());
			StreamWatch[i].m_nDetection_Time = xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("Prime_Input_Absent_Duration"), TRUE)->GetValue(0);
			StreamWatch[i].m_nPreDetection_Time = xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("Prime_Input_Present_Duration"), TRUE)->GetValue(0);
			
			_stprintf_s(StreamWatch[i].m_Input[0].strNic_Source, 30, _T("%s"),xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("PrimaryInput"), TRUE)->GetChild(_T("Network_Adaptor"), TRUE)->GetValue());
			_stprintf_s(StreamWatch[i].m_Input[0].strIp_Source, 30,_T("%s"), xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("PrimaryInput"), TRUE)->GetChild(_T("IP_Address"), TRUE)->GetValue());
			StreamWatch[i].m_Input[0].nPort_Source = xml.GetRoot()->GetChild(strRoot,TRUE)->GetChild(_T("PrimaryInput"), TRUE)->GetChild(_T("Port_Number"), TRUE)->GetValue(0); 
			m_nAlertCompairPrime[i] = xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("PrimaryInput"), TRUE)->GetChild(_T("Stream_Prev_Status"), TRUE)->GetValue(0);

			_stprintf_s(StreamWatch[i].m_Input[1].strNic_Source, 30,_T("%s"), xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("SecondaryInput"), TRUE)->GetChild(_T("Network_Adaptor"), TRUE)->GetValue());
			_stprintf_s(StreamWatch[i].m_Input[1].strIp_Source, 30,_T("%s"), xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("SecondaryInput"), TRUE)->GetChild(_T("IP_Address"), TRUE)->GetValue());
			StreamWatch[i].m_Input[1].nPort_Source = xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("SecondaryInput"),TRUE)->GetChild(_T("Port_Number"), TRUE)->GetValue(0); 
			m_nAlertCompairSecond[i] = xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("SecondaryInput"), TRUE)->GetChild(_T("Stream_Prev_Status"), TRUE)->GetValue(0);

			_stprintf_s(StreamWatch[i].m_Output.strNic_Source, 30, _T("%s"), xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("Output"), TRUE)->GetChild(_T("Network_Adaptor"), TRUE)->GetValue());
			_stprintf_s(StreamWatch[i].m_Output.strIp_Source, 30, _T("%s"),xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("Output"), TRUE)->GetChild(_T("IP_Address"), TRUE)->GetValue());
			StreamWatch[i].m_Output.nPort_Source = xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("Output"), TRUE)->GetChild(_T("Port_Number"), TRUE)->GetValue(0);

			//Replacement as per selected prg no
			StreamWatch[i].m_nTotal_PrgNo = xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("SecondaryInput"), TRUE)->GetChild(_T("TotalPrgNo"), TRUE)->GetValue(0);
			_stprintf_s(StreamWatch[i].m_strPrgNoList, 1000, _T("%s"), xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("SecondaryInput"), TRUE)->GetChild(_T("Program_Numbers"), TRUE)->GetValue());
			StreamWatch[i].m_strPrgNo_List[0] = _tcstok(StreamWatch[i].m_strPrgNoList, _T(","));
			for(int k = 1; k < StreamWatch[i].m_nTotal_PrgNo; k++)
			{
				StreamWatch[i].m_strPrgNo_List[k] = _tcstok(NULL, _T(", "));
			}
			StreamWatch[i].m_nSelected_PrgNo = xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("SecondaryInput"), TRUE)->GetChild(_T("Selected_PrgNo"), TRUE)->GetValue(0);

			StreamWatch[i].m_Input[0].m_nDetect_Time = StreamWatch[i].m_nDetection_Time;
			StreamWatch[i].m_Input[1].m_nDetect_Time = StreamWatch[i].m_nDetection_Time;
		
		}
		AlertDlg.AlertData.m_nAlertTime = xml.GetRoot()->GetChild(_T("Alert"), TRUE)->GetChild(_T("Alert_Duration_Time"), TRUE)->GetValue(0);
		AlertDlg.m_nTotalEmailIdList = xml.GetRoot()->GetChild(_T("Alert"),TRUE)->GetChild(_T("TotalListMailId"), TRUE)->GetValue(0);
		AlertDlg.m_nTotalEmailId = xml.GetRoot()->GetChild(_T("Alert"), TRUE)->GetChild(_T("TotalMailId"), TRUE)->GetValue(0);
		m_nTotalStartedChannel = xml.GetRoot()->GetChild(_T("Streamwatch_channel_info"), TRUE)->GetChild(_T("Total_started_channels"))->GetValue(0);
		_stprintf_s(AlertDlg.AlertData.m_strAlertUserName, 100, _T("%s"), xml.GetRoot()->GetChild(_T("Alert"), TRUE)->GetChild(_T("UserName"), TRUE)->GetValue());
		_stprintf_s(AlertDlg.AlertData.m_strAlertPassword, 100, _T("%s"), xml.GetRoot()->GetChild(_T("Alert"), TRUE)->GetChild(_T("Password"), TRUE)->GetValue());
		_stprintf_s(AlertDlg.AlertData.m_strAlertSMTP, 100, _T("%s"), xml.GetRoot()->GetChild(_T("Alert"), TRUE)->GetChild(_T("SmtpServer"), TRUE)->GetValue());
		CString strReceiver_Email = L"";
		CString strEmail_List = L"";
		for(int i = 0; i < AlertDlg.m_nTotalEmailIdList; i++)
		{
			strEmail_List.Format(_T("strEmail_List%02d"),i);
			_stprintf_s(AlertDlg.AlertData.m_strAlertEmailList[i], 100, _T("%s"), xml.GetRoot()->GetChild(_T("Alert"), TRUE)->GetChild(strEmail_List, TRUE)->GetValue());
		}
		for(int i = 0; i < AlertDlg.m_nTotalEmailId; i++)
		{
			strReceiver_Email.Format(_T("Receiver_Email_%02d"),i);
			_stprintf_s(AlertDlg.AlertData.m_strAlertReceiverEmailId[i], 100, _T("%s"), xml.GetRoot()->GetChild(_T("Alert"), TRUE)->GetChild(strReceiver_Email, TRUE)->GetValue());
		}
	
		xml.Close();
		bResult = true;		
	}
	return bResult;
}
void CStream_WatchDlg::DisableControl(int index)
{
	int nTab_Id = index / 16;
	int nResource_Id = 200 + (20 * index) + 1;
	int nStatusResource_Id = 2301 + index;
	
	m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
	m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
	m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);

	if(!bStopUDP)
	{
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);

		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(1171 + index)->EnableWindow(false);
	}
	else
	{
		nResource_Id += 6;
	}

	m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
	m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
	m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(true);
	m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
	nResource_Id += 4;
	m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);   // for channel Name 
	m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);

	m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(nStatusResource_Id)->EnableWindow(false);
	nStatusResource_Id += 50;
	m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(nStatusResource_Id)->EnableWindow(true);
}

void CStream_WatchDlg::EnableControl(int index)
{
	int nTab_Id = index / 16;
	int nResource_Id = 200 + (20 * index) + 1;
	int nStatusResource_Id = 2301 + index;
	
	m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(true);
	m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(true);
	m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(true);

	if(!bStopUDP)
	{
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(true);
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(true);
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(true);


		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(true);
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(true);
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(true);
	}
	else
	{
		nResource_Id += 6;
	}
	m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(true);
	m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(true);
	m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(false);
	m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(true);
	nResource_Id += 4;
	m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(true); // channel name
	m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(nResource_Id++)->EnableWindow(true);

	m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(nStatusResource_Id)->EnableWindow(true);
	nStatusResource_Id += 50;
	m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(nStatusResource_Id)->EnableWindow(false);
}

bool CStream_WatchDlg::StreamValidation(int StreamId)
{
	int nTab_Id = StreamId / 16;
	int nMaxPort_range = 65535;
	int nMinPort_range = 1000;
	int temp_Time = 0;
	int temp_PreTime = 0;
	int temp_Port[3] = {0,0,0};
	int Control_Id = 201 + (20 * StreamId);
	const int bufSize = 100;
	int nTempChecking = 0;

	TCHAR temp_Nic_add[3][20];
	TCHAR temp_Ip_add[3][20];

	TCHAR port[20];
	memset(port, 0, 20*sizeof(TCHAR));

	StreamWatch[StreamId].m_bValidation_Error = false;

	TCHAR strBufErrorMsg[bufSize];
	_stprintf_s(strBufErrorMsg, bufSize, _T("Souce Port Number should be between %d to %d ."), nMinPort_range, nMaxPort_range);

	//take values from dailog
	if(!bStopUDP)
		nTempChecking = 3;
	else
		nTempChecking = 1;

	for(int i = 0; i < nTempChecking; i++)
	{
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItemText(Control_Id++, (LPTSTR)temp_Nic_add[i], 20);
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItemText(Control_Id++, (LPTSTR)temp_Ip_add[i], 120);
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItemText(Control_Id++, (LPTSTR)port, 20);
		temp_Port[i] = _ttoi(port);
	}

	if(!bStopUDP)
	{
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItemText(Control_Id++,(LPTSTR)port,20);
		temp_Time = _ttoi(port);
		Control_Id += 8;
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItemText(Control_Id++,(LPTSTR)port,20);
		temp_PreTime = _ttoi(port);
	}
	else
	{
		Control_Id += 3;
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItemText(Control_Id++,(LPTSTR)port,20);
		temp_Time = _ttoi(port);
		Control_Id += 8;
		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItemText(Control_Id++,(LPTSTR)port,20);
		temp_PreTime = _ttoi(port);
	}
	
	// Validation

	if(!bStopUDP)
	{
		if(temp_Port[0] == 0 || temp_Port[1] == 0 || temp_Port[2] == 0)
			{
			MessageBox(_T("port Number should not be empty."), _T("ERROR"), MB_OK);
			StreamWatch[StreamId].m_bValidation_Error = true;
			return false;
		}
	}
	else
	{
		if(temp_Port[0] == 0)
		{
			MessageBox(_T("port Number should not be empty."), _T("ERROR"), MB_OK);
			StreamWatch[StreamId].m_bValidation_Error = true;
			return false;
		}
	}

	if(!bStopUDP)
	{
		if ((_tcscmp(temp_Nic_add[0], _T("")) == 0) ||(_tcscmp(temp_Nic_add[1], _T("")) == 0) || (_tcscmp(temp_Nic_add[2], _T("")) == 0) ||(_tcscmp(temp_Ip_add[0], _T("0.0.0.0")) == 0) || (_tcscmp(temp_Ip_add[1], _T("0.0.0.0")) == 0) || (_tcscmp(temp_Ip_add[2], _T("0.0.0.0")) == 0))
		{
			MessageBox(_T("Value should not be empty."), _T("ERROR"), MB_OK);
			StreamWatch[StreamId].m_bValidation_Error = true;
			return false;
		}
	}
	else
	{
		if ((_tcscmp(temp_Nic_add[0], _T("")) == 0) || (_tcscmp(temp_Ip_add[0], _T("0.0.0.0")) == 0))
		{
			MessageBox(_T("Value should not be empty."), _T("ERROR"), MB_OK);
			StreamWatch[StreamId].m_bValidation_Error = true;
			return false;
		}
	}

	if(!bStopUDP)
	{
		for( int i=0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				if(i != j)
				{
					if (temp_Port[i] == temp_Port[j] &&  temp_Ip_add[i] == temp_Ip_add[j] && temp_Nic_add[i] == temp_Nic_add[j])
					{
						MessageBox(_T("Port number,Ip address and Adaptor combination should different."), _T("ERROR"), MB_OK);
						StreamWatch[StreamId].m_bValidation_Error = true;
						return false;
					}
				}
			}
		}
	}

	/*if(!bStopUDP)
		nTempChecking = 3;
	else
		nTempChecking = 1;

	for(int i = 0; i < nTempChecking; i++)
	{
		if ( temp_Port[i] > nMaxPort_range ||  temp_Port[i] < nMinPort_range)
		{
			MessageBox(strBufErrorMsg, _T("ERROR"), MB_OK); 
			StreamWatch[StreamId].m_bValidation_Error = true;
			return false;
		}
	}*/

	if (temp_Time < 0 || temp_Time > 180000)
	{
		MessageBox(_T("Waiting Time should between 0 to 180000 millisecond."), _T("ERROR"), MB_OK);
		StreamWatch[StreamId].m_bValidation_Error = true;
		return false;
	}

	if(temp_PreTime < 0 || temp_PreTime > 10000)
	{
		MessageBox(_T("Waiting Time should between 0 to 10000 millisecond."), _T("ERROR"), MB_OK);
		StreamWatch[StreamId].m_bValidation_Error = true;
		return false;
	}
	else
		return true;
}

int CStream_WatchDlg::StreamStart(int StreamId)
{
	CTabGeneral *pTabGolbalPtr = NULL;
	CString strVideoGapAnalysis;
	strVideoGapAnalysis.Format(_T("%s\\GapAnalysis"), GetAppPath());

	int retval;
	retval = PathFileExists(strVideoGapAnalysis);
	if(retval == 0)
	{
		CreateDirectory(strVideoGapAnalysis ,NULL);
	}

	_stprintf(StreamWatch[StreamId].m_strFPathVideopara, _T("%s\\GapAnalysis_%d.txt"), strVideoGapAnalysis, StreamId + 1);
	if(bStartVideoAnalysis)
		StreamWatch[StreamId].readVideoAnalysisInfo(StreamId);
	int nTab_Id = StreamId / 16;
	int nTemp_Id = StreamId % 16;

	if(StreamId >= 0 && StreamId < 16)
	{
		pTabGolbalPtr = &(((CStream_Watch_Tab1*)m_TabStreamTabCtrl.m_tabPages[0])->General_Tab1);
	}
	if(StreamId >= 16 && StreamId < 32)
	{
		pTabGolbalPtr = &(((CStream_Watch_Tab2*)m_TabStreamTabCtrl.m_tabPages[1])->General_Tab2);
	}
	if(StreamId >= 32 && StreamId < 48)
	{
		pTabGolbalPtr = &(((CStream_Watch_Tab3*)m_TabStreamTabCtrl.m_tabPages[2])->General_Tab3);
	}

	//Replacement as per selected prg no
	if(StreamWatch[StreamId].m_bStart == true)
	{
		short nIndex;
		nIndex = pTabGolbalPtr->m_cmbPrgNo[StreamId].GetCurSel();
		pTabGolbalPtr->m_cmbPrgNo[StreamId].GetLBText(nIndex, StreamWatch[StreamId].m_strSelected_PrgNo);
		StreamWatch[StreamId].m_nSelected_PrgNo = _ttoi(StreamWatch[StreamId].m_strSelected_PrgNo);
		StreamWatch[StreamId].m_bSelected_PrgNo = true;
		StreamWatch[StreamId].m_bStart = false;
	}

	StreamWatch[StreamId].m_Output.m_bThread_Terminate = false;
	StreamWatch[StreamId].m_Input[0].m_bScannar_Terminate = false;
	StreamWatch[StreamId].m_Input[1].m_bScannar_Terminate = false;
	StreamWatch[StreamId].m_bValidation_Status = StreamValidation(StreamId);

	if(StreamWatch[StreamId].m_bValidation_Status == true)
	{
		for(int i = 0; i < MAX_RECEIVER; i++)
		{
			short cIndex;
			cIndex = pTabGolbalPtr->m_cmbSourceAdapter[nTemp_Id][i].GetCurSel();
			pTabGolbalPtr->m_cmbSourceAdapter[nTemp_Id][i].GetLBText(cIndex, pTabGolbalPtr->m_strNic_Source[nTemp_Id][i]);  
		}
		short cIndex;
		cIndex = pTabGolbalPtr->m_cmbDestAdapter[nTemp_Id].GetCurSel();
		pTabGolbalPtr->m_cmbDestAdapter[nTemp_Id].GetLBText(cIndex, pTabGolbalPtr->m_strNic_Dest[nTemp_Id]);

   		m_TabStreamTabCtrl.m_tabPages[nTab_Id]->UpdateData(true);

		for( int i=0; i < MAX_RECEIVER; i++)
		{
			StreamWatch[StreamId].m_Input[i].nPort_Source = pTabGolbalPtr->m_nPort_Source[nTemp_Id][i];
			_stprintf_s(StreamWatch[StreamId].m_Input[i].strNic_Source , 30, _T("%s"), pTabGolbalPtr->m_strNic_Source[nTemp_Id][i]);
			_stprintf_s(StreamWatch[StreamId].m_Input[i].strIp_Source, 30, _T("%s"), pTabGolbalPtr->m_strIp_Source[nTemp_Id][i]);
		}

		_stprintf_s(StreamWatch[StreamId].m_Output.strNic_Source , 30, _T("%s"), pTabGolbalPtr->m_strNic_Dest[nTemp_Id]);
		_stprintf_s(StreamWatch[StreamId].m_Output.strIp_Source , 30, _T("%s"), pTabGolbalPtr->m_strIp_Dest[nTemp_Id]);
		StreamWatch[StreamId].m_Output.nPort_Source = pTabGolbalPtr->m_nPort_Dest[nTemp_Id];

		StreamWatch[StreamId].m_nDetection_Time = pTabGolbalPtr->m_m_nDetection_Time[nTemp_Id];
		StreamWatch[StreamId].m_nPreDetection_Time = pTabGolbalPtr->m_nPresentDetection_Time[nTemp_Id];

		_stprintf_s(StreamWatch[StreamId].m_strCstreamChannel_Name , 20, _T("%s"), pTabGolbalPtr->m_strChannel_Name[nTemp_Id]);

		StreamWatch[StreamId].m_nCheckBox_Status = ((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem((213 + (20 * StreamId))))->GetCheck();
		StreamWatch[StreamId].m_bManualCheckStatus = ((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem((217 + (20 * StreamId))))->GetCheck();
		if(bEMERGENCY)
			StreamWatch[StreamId].m_Input[0].m_bEmergencyActive = ((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem((220 + (20 * StreamId))))->GetCheck();

		StreamWatch[StreamId].m_Input[0].m_nDetect_Time = StreamWatch[StreamId].m_nDetection_Time;
		StreamWatch[StreamId].m_Input[1].m_nDetect_Time = StreamWatch[StreamId].m_nDetection_Time;

		StreamWatch[StreamId].m_Input[0].m_nChannelNumber = StreamId;
		StreamWatch[StreamId].m_Input[1].m_nChannelNumber = StreamId;

		StreamWatch[StreamId].Start();
		//lastCCError[StreamId] = CCError[StreamId];
		//SetTimer(IDT_TIMER_CCERROR, 300000, NULL);

		return 1;
	}
	else
		return 0;
}

void CStream_WatchDlg::StartEvent(int startButton_Id)
{
	const int STARTSTOPBUF_SIZE = 800;
	int nTemp_Id = startButton_Id % 16;

	CTabGeneral *pTabGolbalPtr = NULL;
	
	if(startButton_Id >= 0 && startButton_Id < 16)
	{
		pTabGolbalPtr = &(((CStream_Watch_Tab1*)m_TabStreamTabCtrl.m_tabPages[0])->General_Tab1);
	}
	if(startButton_Id >= 16 && startButton_Id < 32)
	{
		pTabGolbalPtr = &(((CStream_Watch_Tab2*)m_TabStreamTabCtrl.m_tabPages[1])->General_Tab2);
	}
	if(startButton_Id >= 32 && startButton_Id < 48)
	{
		pTabGolbalPtr = &(((CStream_Watch_Tab3*)m_TabStreamTabCtrl.m_tabPages[2])->General_Tab3);
	}

	//Replacement as per selected prg no
	/*if(bStart[startButton_Id] == true)
	{
		short nIndex;
		nIndex = pTabGolbalPtr->m_cmbPrgNo[0].GetCurSel();
		pTabGolbalPtr->m_cmbPrgNo[0].GetLBText(nIndex, strSelected_PrgNo);
		nSelected_PrgNo = _ttoi(strSelected_PrgNo);
		bSelected_PrgNo = true;
		//b = false;
	}*/

	TCHAR strStartStopBuf[STARTSTOPBUF_SIZE];
	memset(strStartStopBuf, 0, STARTSTOPBUF_SIZE*sizeof(TCHAR));
	
	StreamWatch[startButton_Id].m_Output.m_bSendTo_Status = false;
	StreamWatch[startButton_Id].m_Input[0].m_dwLastUpdateTime = GetTickCount();//0;
	StreamWatch[startButton_Id].m_Input[1].m_dwLastUpdateTime = 0;
	StreamWatch[startButton_Id].m_dwStartClickTime = GetTickCount();

	int check = StreamStart(startButton_Id);
	if(check == 1)
	{
		DisableControl(startButton_Id);
		
		m_nTotalStartStream++;
		if(m_nTotalStartStream <= 1)
		{
			SetTimer(IDT_TIMER_STREAM_STATUS, 1000, NULL);
			SetTimer(IDT_TIMER_ALERT, 1000, NULL);
			//SetTimer(IDT_TIMER_CCERROR, 300000, NULL);
			SetTimer(IDT_TIMER_CCERROR, 180000, NULL);
		}
		if(m_nTotalStartStream == m_nLiecensedStream || m_nTotalStartStream == TotalCheckedBox)
		{
			GetDlgItem(IDC_BUTTON_START_ALL)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON_STOP_ALL)->EnableWindow(true);
		}
		m_nTotalStartedChannel = m_nTotalStartStream;		
		CreateXmlFile();//saving in xml file
		
		_stprintf_s(strStartStopBuf, STARTSTOPBUF_SIZE, _T("START STREAM---->PrimAdopter: %s PrimIPAddr: %s PrimPort: %d SecondAdopter: %s SecondIPAddr: %s SecondPort: %d DestAdopter: %s DestIPAddr: %s DestPort: %d"), pTabGolbalPtr->m_strNic_Source[nTemp_Id][0], pTabGolbalPtr->m_strIp_Source[nTemp_Id][0], pTabGolbalPtr->m_nPort_Source[nTemp_Id][0], pTabGolbalPtr->m_strNic_Source[nTemp_Id][1], pTabGolbalPtr->m_strIp_Source[nTemp_Id][1], pTabGolbalPtr->m_nPort_Source[nTemp_Id][1], pTabGolbalPtr->m_strNic_Dest[nTemp_Id], pTabGolbalPtr->m_strIp_Dest[nTemp_Id], pTabGolbalPtr->m_nPort_Dest[nTemp_Id]);
		LogStartStopEvents(strStartStopBuf, startButton_Id);
		
	}
}

void CStream_WatchDlg::StopEvent(int stopButton_Id)
{
	const int STARTSTOPBUF_SIZE = 800;
	int nTemp_Id = stopButton_Id % 16;;
	int nTab_Id =  stopButton_Id / 16;
	
	TCHAR strStartStopBuf[STARTSTOPBUF_SIZE];
	memset(strStartStopBuf, 0, STARTSTOPBUF_SIZE*sizeof(TCHAR));

	CTabGeneral *pTabGolbalPtr = NULL;
	CStatusMembers *pStatusPtr = NULL;

	pStatusPtr = &(((CStream_Watch_Tab4*)m_TabStreamTabCtrl.m_tabPages[3])->StatusTab);

	if(stopButton_Id >= 0 && stopButton_Id < 16)
	{
		pTabGolbalPtr = &(((CStream_Watch_Tab1*)m_TabStreamTabCtrl.m_tabPages[0])->General_Tab1);
	}
	if(stopButton_Id >= 16 && stopButton_Id < 32)
	{
		pTabGolbalPtr = &(((CStream_Watch_Tab2*)m_TabStreamTabCtrl.m_tabPages[1])->General_Tab2);
	}
	if(stopButton_Id >= 32 && stopButton_Id < 48)
	{
		pTabGolbalPtr = &(((CStream_Watch_Tab3*)m_TabStreamTabCtrl.m_tabPages[2])->General_Tab3);
	}
	
	bool bCheck = StreamWatch[stopButton_Id].Stop();
	if(bCheck == true)
	{
		if(--m_nTotalStartStream == 0)
		{
			OnBnClickedButtonStopAll();
		}
		if(m_nTotalStartStream != TotalCheckedBox)
		{
			GetDlgItem(IDC_BUTTON_START_ALL)->EnableWindow(true);
			GetDlgItem(IDC_BUTTON_STOP_ALL)->EnableWindow(false);
		}
		
		EnableControl(stopButton_Id);
		
		_stprintf_s(strStartStopBuf, STARTSTOPBUF_SIZE, _T("STOP STREAM---->PrimAdopter: %s PrimIPAddr: %s PrimPort: %d SecondAdopter: %s SecondIPAddr: %s SecondPort: %d DestAdopter: %s DestIPAddr: %s DestPort: %d"), pTabGolbalPtr->m_strNic_Source[nTemp_Id][0], pTabGolbalPtr->m_strIp_Source[nTemp_Id][0], pTabGolbalPtr->m_nPort_Source[nTemp_Id][0], pTabGolbalPtr->m_strNic_Source[nTemp_Id][1], pTabGolbalPtr->m_strIp_Source[nTemp_Id][1], pTabGolbalPtr->m_nPort_Source[nTemp_Id][1], pTabGolbalPtr->m_strNic_Dest[nTemp_Id], pTabGolbalPtr->m_strIp_Dest[nTemp_Id], pTabGolbalPtr->m_nPort_Dest[nTemp_Id]);
		LogStartStopEvents(strStartStopBuf, stopButton_Id);
		
		pTabGolbalPtr->m_PriStatus[nTemp_Id].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
		pTabGolbalPtr->m_SecondaryStatus[nTemp_Id].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
		pTabGolbalPtr->m_OtputStatus[nTemp_Id].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
		
		pStatusPtr->m_pic_Prime_Stream[stopButton_Id].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
		pStatusPtr->m_pic_Second_Stream[stopButton_Id].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
		pStatusPtr->m_pic_Output_Stream[stopButton_Id].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);

		if(!bStopUDP && bEMERGENCY)
		{
			//Replacement as per selected prg no
			for(int j = 0; j < MAX_RECEIVER; j++)
			{
				if(StreamWatch[stopButton_Id].m_Input[j].m_nTotalProgNo > 1/*&& bStop[stopButton_Id] == false*/)
				{
					((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(1171 + stopButton_Id))->EnableWindow(true);
					pTabGolbalPtr->m_cmbPrgNo[stopButton_Id].ResetContent();
					for(int i = 0; i < StreamWatch[stopButton_Id].m_Input[j].m_nTotalProgNo; i++)
					{
						StreamWatch[stopButton_Id].m_strPrgNo_List[i].Format(_T("%d"),StreamWatch[stopButton_Id].m_Input[j].m_nProgramNo_Buff[i]);
						if(StreamWatch[stopButton_Id].m_strPrgNo_List[i] != _T("0"))
							pTabGolbalPtr->m_cmbPrgNo[stopButton_Id].AddString(StreamWatch[stopButton_Id].m_strPrgNo_List[i]);
					}
					pTabGolbalPtr->m_cmbPrgNo[stopButton_Id].SetCurSel(0);
					//bStop[stopButton_Id] = true;
					StreamWatch[stopButton_Id].m_bStart = true;
				}
			}
			StreamWatch[stopButton_Id].m_bSelected_PrgNo = false;
		}
	}
	m_nTotalStartedChannel = m_nTotalStartStream;
	//CreateXmlFile();
	LoadXML_Started_channels();
	//StoreCheckboxData(stopButton_Id);
}
void CStream_WatchDlg::ManualCheckBoxEffect(int checkBoxId,int clicked_tabId)
{
	int nTab_Id = checkBoxId / 16;
	int nTempManualCheck_status = 0;
	
	if(clicked_tabId == 3)
	{
		nTempManualCheck_status = ((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2501 + checkBoxId))->GetCheck();  
		((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(217 + (checkBoxId * 20)))->SetCheck(nTempManualCheck_status);				
	}
	else
	{
		nTempManualCheck_status = ((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(217 + (checkBoxId * 20)))->GetCheck();		
		((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2501 + checkBoxId))->SetCheck(nTempManualCheck_status); 
	}
}
void CStream_WatchDlg::EmeCheckBoxEffect(int checkBoxId,int clicked_tabId)
{
	int nTab_Id = checkBoxId / 16;
	int nTempManualCheck_status = 0;
	
	if(clicked_tabId == 3)
	{
		nTempManualCheck_status = ((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2551 + checkBoxId))->GetCheck();  
		((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(220 + (checkBoxId * 20)))->SetCheck(nTempManualCheck_status);				
	}
	else
	{
		nTempManualCheck_status = ((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(220 + (checkBoxId * 20)))->GetCheck();		
		((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2551 + checkBoxId))->SetCheck(nTempManualCheck_status); 
	}

}

void CStream_WatchDlg::CheckedBoxEffect(int checkBoxId,int pTab_Id)
{
	int nTab_Id =  checkBoxId / 16;
	int nTemp_CheckedStatus = 0;
	int nTemp_CheckBoxId = 0;
	int nTemp_TabId = 0;
	
	int nIsChecked = ((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem((213 + (20 * checkBoxId))))->GetCheck();
	int nIsCheckedStatusTab = ((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2401 + checkBoxId))->GetCheck();

	if(pTab_Id != 3)
	{
		nTemp_TabId = 3; 
		nTemp_CheckBoxId = 2401 + checkBoxId;
		nTemp_CheckedStatus = nIsChecked;
		
		if((nIsChecked == 1))
		{
			TotalCheckedBox++;
		}
		else if((nIsChecked == 0))
		{
			TotalCheckedBox--;

			if(TotalCheckedBox < m_nLiecensedStream)
			{
				((CButton*)GetDlgItem(IDC_CHECKED_ALL))->SetCheck(0);
			}
		}
	}
	else //for status page
	{
		nTemp_TabId = nTab_Id;
		nTemp_CheckBoxId = 213 + (20 * checkBoxId);
		nTemp_CheckedStatus = nIsCheckedStatusTab;

		if((nIsCheckedStatusTab == 1))
		{
			TotalCheckedBox++;
		}
		else if((nIsCheckedStatusTab == 0))
		{
			TotalCheckedBox--;

			if(TotalCheckedBox < m_nLiecensedStream)
			{
				((CButton*)GetDlgItem(IDC_CHECKED_ALL))->SetCheck(0);
			}
		}
	}
	
	((CButton*)m_TabStreamTabCtrl.m_tabPages[nTemp_TabId]->GetDlgItem(nTemp_CheckBoxId))->SetCheck(nTemp_CheckedStatus);

	m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(211 + (checkBoxId * 20))->EnableWindow(nTemp_CheckedStatus);
	m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2301 + checkBoxId)->EnableWindow(nTemp_CheckedStatus);

	StreamWatch[checkBoxId].m_nCheckBox_Status = nTemp_CheckedStatus;
	
	if(TotalCheckedBox == m_nLiecensedStream)
	{
		((CButton*)GetDlgItem(IDC_CHECKED_ALL))->SetCheck(1);
	}
	if(m_nTotalStartStream == TotalCheckedBox && m_nTotalStartStream != 0)
	{
		GetDlgItem(IDC_BUTTON_START_ALL)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_STOP_ALL)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_START_ALL)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_STOP_ALL)->EnableWindow(false);
	}
	/*if(TotalCheckedBox >= 2)
	{
		GetDlgItem(IDC_BUTTON_START_ALL)->EnableWindow(true);
	}
	else*/ if(TotalCheckedBox < 2)
	{
		GetDlgItem(IDC_BUTTON_START_ALL)->EnableWindow(false);
	}
}

void CStream_WatchDlg::KillFocusEffect(int ChannelId)
{
	int nTab_Id = ChannelId / 16; 
	int	tempReadId = 0, tempWriteId = 0;
	tempReadId = 218 + (ChannelId * 20);
	tempWriteId = 2251 + ChannelId;

	CString strTempChannelName;
	m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(tempReadId)->GetWindowTextW(strTempChannelName);
	m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(tempWriteId)->SetWindowTextW(strTempChannelName);
	_stprintf_s(StreamWatch[ChannelId].m_strCstreamChannel_Name, 20, _T("%s"), strTempChannelName);

	//store data in xml
	CXml xml;
	CString strFilePath;
	CString strMsg;

	strFilePath.Format( _T("%s\\StreamWatchConfigFile.xml"), GetAppPath());	

	if( xml.Open(strFilePath) )
	{
		CString strRoot = _T("");
		strRoot.Format(_T("Stream_%02d"), ChannelId + 1);
		xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("ChannelName"), TRUE)->SetValue(StreamWatch[ChannelId].m_strCstreamChannel_Name);
		xml.Save(strFilePath);
		xml.Close();
	}
}

void CStream_WatchDlg::OnBnClickedCheckedAll()
{
	int nTab_Id = 0;
	int nTempValue = 1;
	bool bCheckAllStatus = false;

	for(int i = 0; i < MAX_STREAM && i < m_nLiecensedStream; i++)
	{
		nTab_Id = i/16;

		if(((CButton*)GetDlgItem(IDC_CHECKED_ALL))->GetCheck() == 1)
		{
			bCheckAllStatus = true;
			if(((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(213 + (i * 20)))->GetCheck() == 0)
			{
				nTempValue = 1;
			}
		}
		else
		{
			if(StreamWatch[i].m_nCheck_EngineStatus == false)
			{
				nTempValue = 0;
			}
		}

		((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(213 + (i * 20)))->SetCheck(nTempValue);
		((CButton*)m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2401 + i))->SetCheck(nTempValue);

		if(StreamWatch[i].m_nCheck_EngineStatus == false)
		{
			m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(211 + (i * 20))->EnableWindow(nTempValue);
		}
		m_TabStreamTabCtrl.m_tabPages[3]->GetDlgItem(2301 + i)->EnableWindow(nTempValue);

		StreamWatch[i].m_nCheckBox_Status = nTempValue;
	}
	GetDlgItem(IDC_BUTTON_STOP_ALL)->EnableWindow(false);
	if(bCheckAllStatus == true)
	{
		TotalCheckedBox = m_nLiecensedStream;
		GetDlgItem(IDC_BUTTON_START_ALL)->EnableWindow(true);
	}
	else
	{
		TotalCheckedBox = m_nTotalStartStream;
		GetDlgItem(IDC_BUTTON_START_ALL)->EnableWindow(false);
	}

	if(m_nTotalStartStream == TotalCheckedBox && m_nTotalStartStream != 0)
	{
		GetDlgItem(IDC_BUTTON_START_ALL)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_STOP_ALL)->EnableWindow(true);
	}
}

void CStream_WatchDlg::OnTimer(UINT_PTR nIDEvent)
{
	int nTab_Id = 0;
	int nTemp_Id = 0;
	int nManualCheck_status = 0;
	int nTempColorId = 0;
	int nTempStatusTabColorId = 0;

	CTabGeneral *pTabGolbalPtr = NULL;
	CStatusMembers *pStatusPtr = NULL;

	pStatusPtr = &(((CStream_Watch_Tab4*)m_TabStreamTabCtrl.m_tabPages[3])->StatusTab);

	CFile FilePointer;
	CFileStatus Status;
	CFileException excFile;

	TCHAR m_strFPathVideopara[200];
	_stprintf(m_strFPathVideopara, _T("%s\\ScheduleManualSwitch.txt"), GetAppPath());

	try
	{
		if(IDT_TIMER_STREAM_STATUS == nIDEvent)
		{
			for(int i = 0; i < MAX_STREAM && i < m_nLiecensedStream; i++)
			{
				//StreamWatch[i].m_Input[0].bStatus_Stream = false;
				//StreamWatch[i].m_Input[1].bStatus_Stream = false;
				nTab_Id = i / 16;
				nTemp_Id = i % 16;
				if(i >= 0 && i < 16)
				{
					if(i == 0)
					{
						index[0] = 0;
						index[1] = 0;
					}
					pTabGolbalPtr = &(((CStream_Watch_Tab1*)m_TabStreamTabCtrl.m_tabPages[0])->General_Tab1);
				}
				if(i >= 16 && i < 32)
				{
					pTabGolbalPtr = &(((CStream_Watch_Tab2*)m_TabStreamTabCtrl.m_tabPages[1])->General_Tab2);
				}
				if(i >= 32 && i < 48)
				{
					pTabGolbalPtr = &(((CStream_Watch_Tab3*)m_TabStreamTabCtrl.m_tabPages[2])->General_Tab3);
				}

				if(StreamWatch[i].m_bValidation_Status == true)
				{
					if(StreamWatch[i].m_Output.m_bThread_Terminate == false)
					{
						if(StreamWatch[i].m_Input[0].m_bScannar_Terminate == false)
						StreamWatch[i].m_Input[0].m_nCurrent_Time = GetTickCount() - StreamWatch[i].m_Input[0].m_dwLastUpdateTime;
						if(StreamWatch[i].m_Input[1].m_bScannar_Terminate == false)
						StreamWatch[i].m_Input[1].m_nCurrent_Time = GetTickCount() - StreamWatch[i].m_Input[1].m_dwLastUpdateTime;
						for(int k = 0; k < MAX_RECEIVER; k++)
						{
							if (StreamWatch[i].m_Input[k].m_nCurrent_Time > StreamWatch[i].m_nDetection_Time)
							{
								StreamWatch[i].m_Input[k].bStatus_Stream = true;
							}
							else
							{
								if(StreamWatch[i].m_nGoodPrimeCompair != 0)
								{
									if(k == 0  && StreamWatch[i].m_nGoodPrimeCompair > StreamWatch[i].m_nPreDetection_Time)
									{
										if(( GetTickCount() - StreamWatch[i].m_dwFirstAvalableInput) > StreamWatch[i].m_nPreDetection_Time)
										StreamWatch[i].m_Input[k].bStatus_Stream = false;
									}
								}
								else
								{
									StreamWatch[i].m_Input[k].bStatus_Stream = false;
								}
								if(k == 1)
								StreamWatch[i].m_Input[k].bStatus_Stream = false;			
							}
						}

						if(StreamWatch[i].m_ScheduleManualSwitching == 1)
						{
							((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(217 + (i * 20)))->SetCheck(1);
						}
						if(StreamWatch[i].m_ScheduleManualSwitching == 0)
						{
							((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(217 + (i * 20)))->SetCheck(0);
						}
								
						nManualCheck_status = ((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(217 + (i * 20)))->GetCheck();
						if(nManualCheck_status == 0)
							StreamWatch[i].m_bManualCheckStatus = 0;
						else
							StreamWatch[i].m_bManualCheckStatus = 1;

						if(bEMERGENCY)
						{
							int TempCheck = ((CButton*)m_TabStreamTabCtrl.m_tabPages[nTab_Id]->GetDlgItem(220 + (i * 20)))->GetCheck();
							if(TempCheck == 0)
								StreamWatch[i].m_Input[0].m_bEmergencyActive = 0;
							else
								StreamWatch[i].m_Input[0].m_bEmergencyActive = 1;
						}

						//Give effect to the status 
						if(StreamWatch[i].m_Output.m_bSendTo_Status == true)
						{
							pTabGolbalPtr->m_OtputStatus[nTemp_Id].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_GREEN), RT_BITMAP);
							pStatusPtr->m_pic_Output_Stream[i].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_GREEN), RT_BITMAP);
						}
						else
						{
							pTabGolbalPtr->m_OtputStatus[nTemp_Id].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_RED), RT_BITMAP);
							pStatusPtr->m_pic_Output_Stream[i].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_RED), RT_BITMAP);
						}	

						if(StreamWatch[i].m_Input[1].bStatus_Stream == true && StreamWatch[i].m_Input[0].bStatus_Stream == true)
						{
							pTabGolbalPtr->m_PriStatus[nTemp_Id].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_RED), RT_BITMAP);
							pTabGolbalPtr->m_SecondaryStatus[nTemp_Id].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_RED), RT_BITMAP);

							pStatusPtr->m_pic_Prime_Stream[i].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_RED), RT_BITMAP);
							pStatusPtr->m_pic_Second_Stream[i].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_RED), RT_BITMAP);
							//StreamWatch[i].m_dwLastUpdateTime_Prim = 0;
						}
						else
						{
							if(StreamWatch[i].m_bManualCheckStatus == 0)
							{
								if(StreamWatch[i].m_Input[0].bStatus_Stream == false && StreamWatch[i].m_Input[1].bStatus_Stream == true)
								{
									nTempColorId = IDB_BITMAP_GREEN;
									nTempStatusTabColorId = IDB_BITMAP_RED;
								}
								if(StreamWatch[i].m_Input[0].bStatus_Stream == false && StreamWatch[i].m_Input[1].bStatus_Stream == false)
								{
									nTempColorId = IDB_BITMAP_GREEN;
									nTempStatusTabColorId = IDB_BITMAP_LIGHTGREEN;
								}
								if(StreamWatch[i].m_Input[0].bStatus_Stream == true && StreamWatch[i].m_Input[1].bStatus_Stream == false)
								{
									nTempColorId = IDB_BITMAP_RED;
									nTempStatusTabColorId = IDB_BITMAP_GREEN;
								}
							}
							else
							{
								if(StreamWatch[i].m_Input[0].bStatus_Stream == false && StreamWatch[i].m_Input[1].bStatus_Stream == false )
								{
									nTempColorId = IDB_BITMAP_LIGHTGREEN;
									nTempStatusTabColorId = IDB_BITMAP_ORANGE;
								}
								if(StreamWatch[i].m_Input[0].bStatus_Stream == true && StreamWatch[i].m_Input[1].bStatus_Stream == false)
								{
									nTempColorId = IDB_BITMAP_RED;
									nTempStatusTabColorId = IDB_BITMAP_ORANGE;
								}
								if(StreamWatch[i].m_Input[0].bStatus_Stream == false && StreamWatch[i].m_Input[1].bStatus_Stream == true)
								{
									nTempColorId = IDB_BITMAP_LIGHTGREEN;
									nTempStatusTabColorId = IDB_BITMAP_RED;
								}
							}
							pTabGolbalPtr->m_PriStatus[nTemp_Id].LoadFromResource(NULL, MAKEINTRESOURCE(nTempColorId), RT_BITMAP);
							pTabGolbalPtr->m_SecondaryStatus[nTemp_Id].LoadFromResource(NULL, MAKEINTRESOURCE(nTempStatusTabColorId), RT_BITMAP);

							pStatusPtr->m_pic_Prime_Stream[i].LoadFromResource(NULL, MAKEINTRESOURCE(nTempColorId), RT_BITMAP);
							pStatusPtr->m_pic_Second_Stream[i].LoadFromResource(NULL, MAKEINTRESOURCE(nTempStatusTabColorId), RT_BITMAP);	

						}
					}
				}
				else
				{
					pTabGolbalPtr->m_PriStatus[nTemp_Id].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
					pTabGolbalPtr->m_SecondaryStatus[nTemp_Id].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
					pTabGolbalPtr->m_OtputStatus[nTemp_Id].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);

					pStatusPtr->m_pic_Prime_Stream[i].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
					pStatusPtr->m_pic_Second_Stream[i].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
					pStatusPtr->m_pic_Output_Stream[i].LoadFromResource(NULL, MAKEINTRESOURCE(IDB_BITMAP_NULL), RT_BITMAP);
				}
				/*if(StreamWatch[i].m_Output.m_bThread_Terminate == true && StreamWatch[i].m_Input[0]->m_bPCR_PIDNotFound == true)
				{
					if(--m_nTotalStartStream == 0)
					{
						OnBnClickedButtonStopAll();
					}
					EnableControl(i);
				}*/
				//Check if Gap Anlaysis file is modified 
				if(bStartVideoAnalysis)
				{
					CFile FilePointer;
					CFileStatus Status;
					CFileException excFile;
					if(StreamWatch[i].m_bUpdateGapFile == true)
					{
						int diff = GetTickCount() - StreamWatch[i].m_ModifiedFileTime;
						if(diff > 3000)
						 StreamWatch[i].readVideoAnalysisInfo(StreamWatch[i].m_nChannel_Index);
					}
					if(FilePointer.Open(StreamWatch[i].m_strFPathVideopara, CFile::modeRead | CFile::typeBinary, &excFile))
					{
						FilePointer.GetStatus(Status);
						if(StreamWatch[i].m_FileCreateTime != Status.m_mtime && StreamWatch[i].m_FileCreateTime < Status.m_mtime)
						{
							StreamWatch[i].m_ModifiedFileTime = GetTickCount();
							StreamWatch[i].m_FileCreateTime = Status.m_mtime;
							StreamWatch[i].m_bUpdateGapFile = true;
						}
						FilePointer.Close();
					}
				}
			}
		}
		if(IDT_TIMER_ALERT == nIDEvent)
		{
			memset(m_nAlertBufMessagePrime, 0, 50 * sizeof(int));
			memset(m_nAlertBufMessageSecond, 0, 50 * sizeof(int));
			memset(index, 0, 2 * sizeof(int));
			for(int h = 0; h < 2; h++)
			{
				memset(AlertDlg.m_nChangedStatusPrime[h], 0 , 50 * sizeof(int));
				memset(AlertDlg.m_nChangedStatusSecond[h], 0 , 50 * sizeof(int));
				AlertDlg.m_nTotalChangedChannels[h] = 0;
			}
			DWORD nTimeDiffAlert = GetTickCount() - m_nLastAlertTime;
			if(nTimeDiffAlert > (AlertDlg.m_nAlertDurationTime * 60000))
			{
				m_bIntervalAlertTime = false;
				m_nLastAlertTime = GetTickCount();
			}
			if(m_bIntervalAlertTime == false)
			{
				m_bIntervalAlertTime = true;
				for(int i = 0; i < MAX_STREAM &&  i < m_nLiecensedStream; i++)
				{
					if(StreamWatch[i].m_bValidation_Status == true)
					{
						if(StreamWatch[i].m_Output.m_bThread_Terminate == false)
						{
							if((GetTickCount() - StreamWatch[i].m_dwStartClickTime) >= StreamWatch[i].m_nDetection_Time)
							{
								m_bCheckAlertStatusList[i] = true;
							}
							StreamWatch[i].m_Input[0].m_nCurrent_Time = GetTickCount() - StreamWatch[i].m_Input[0].m_dwLastUpdateTime;
							StreamWatch[i].m_Input[1].m_nCurrent_Time = GetTickCount() - StreamWatch[i].m_Input[1].m_dwLastUpdateTime;
							//for(int k = 0; k < MAX_RECEIVER; k++)
							int limit = 0;
							if(bStopUDP)
							{
								limit = 1;
							}
							else
							{
								limit = 2;
							}
							for(int k = 0; k < limit; k++) 
							{
								TCHAR strEmail[2000];
								_stprintf(strEmail, _T("\n %d : StreamWatch[i].m_Input[0].m_nCurrent_Time %d StreamWatch[i].m_Input[0].m_dwLastUpdateTime %d StreamWatch[i].m_nDetection_Time %d\n"),i,StreamWatch[i].m_Input[0].m_nCurrent_Time ,StreamWatch[i].m_Input[0].m_dwLastUpdateTime,StreamWatch[i].m_nDetection_Time);
								if (StreamWatch[i].m_Input[k].m_nCurrent_Time > StreamWatch[i].m_nDetection_Time)
								{
									StreamWatch[i].m_Input[k].m_bAlertStatus = true;
									if(k == 0)
									{
										m_nAlertBufMessagePrime[i] = 1;
										index[0]++;
									}
									if(k == 1)
									{
										m_nAlertBufMessageSecond[i] = 1;
										index[1]++;
									}
								}
								else
								{
									if(StreamWatch[i].m_Input[k].m_bAlertStatus == true)
									{
										StreamWatch[i].m_Input[k].m_bAlertStatus = false;
									}
								}
							}
							if(m_bCheckAlertStatusList[i] == true)
							{	
								if(m_nAlertCompairPrime[i] != m_nAlertBufMessagePrime[i])
								{
									if(!bStopUDP)
									{
										if((m_nAlertBufMessagePrime[i] == 1) && (m_nAlertBufMessageSecond[i] == 1))
										{
											if(StreamWatch[i].m_bPrimUDPDetect == true && StreamWatch[i].m_bSecUDPDetect == true)
											{
												m_bAlertNoMail = false;
												AlertDlg.m_bPrimUDP = true;
												AlertDlg.m_bSecUDP = true;
											}
											else if(StreamWatch[i].m_bSecUDPDetect == true)
											{
												m_bAlertNoMail = false;
												AlertDlg.m_bSecUDP = true;
											}
											else
												m_bAlertNoMail = true;
										}
										else
										{
											m_bAlertNoMail = false;
											if(m_nAlertBufMessagePrime[i] == 1)
											{
												AlertDlg.m_nAlertStatusChanged[0] = 1;
												if(StreamWatch[i].m_bPrimUDPDetect == true)
													AlertDlg.m_bPrimUDP = true;
											}
											else
												AlertDlg.m_nAlertStatusChanged[0] = 0;
										}
									}
									else
									{
										if(m_nAlertBufMessagePrime[i] == 1)
										{
											AlertDlg.m_nAlertStatusChanged[0] = 1;
											if(StreamWatch[i].m_bPrimUDPDetect == true)
												AlertDlg.m_bPrimUDP = true;
										}
										else
											AlertDlg.m_nAlertStatusChanged[0] = 0;
									}
									if(!m_bAlertNoMail)
									{
										AlertDlg.m_nChangedStatusPrime[AlertDlg.m_nTotalChangedChannels[0]][0] = i;
										AlertDlg.m_nChangedStatusPrime[AlertDlg.m_nTotalChangedChannels[0]][1] = m_nAlertBufMessagePrime[i];
										AlertDlg.m_nTotalChangedChannels[0]++;
										m_bCheckAlertStatus = false;
									}
								}

								if(!m_bAlertNoMail)
								{
									if(m_nAlertCompairSecond[i] != m_nAlertBufMessageSecond[i])
									{
										if(m_nAlertBufMessageSecond[i] == 1)
											AlertDlg.m_nAlertStatusChanged[1] = 1;
										else
											AlertDlg.m_nAlertStatusChanged[1] = 0;

										AlertDlg.m_nChangedStatusSecond[AlertDlg.m_nTotalChangedChannels[1]][0] = i;
										AlertDlg.m_nChangedStatusSecond[AlertDlg.m_nTotalChangedChannels[1]][1] = m_nAlertBufMessageSecond[i];
										AlertDlg.m_nTotalChangedChannels[1]++;
										m_bCheckAlertStatus = false;
									}
								}
								m_bCheckAlertStatusList[i] = false;
							}
						}
					}

					if(i == (m_nLiecensedStream - 1))
					{
						if(m_bCheckAlertStatus == false)
						{
							m_bCheckAlertStatus = true;
							//if(index[0] != 0 || index[1] != 0)
							{
								TCHAR strEmail[2000];
								_stprintf(strEmail, _T("Copying All m_nAlertBufMessagePrime and m_nAlertBufMessageSecond to m_nAlertCompairPrime and m_nAlertCompairSecond respectively. \n"));
								LogAlertMail(strEmail);
								//LogStartStopEvents(strEmail, 0);
								for(int c = 0; c < MAX_STREAM &&  c < m_nLiecensedStream; c++)
								{
									m_nAlertCompairPrime[c] = m_nAlertBufMessagePrime[c];
									m_nAlertCompairSecond[c] = m_nAlertBufMessageSecond[c];
								}
								lastCheckedIndex[0] = index[0];
								lastCheckedIndex[1] = index[1];
								AlertDlg.SendData(index, m_nAlertBufMessagePrime, m_nAlertBufMessageSecond);
								AlertDlg.m_bPrimUDP = false;
								AlertDlg.m_bSecUDP = false;
								//AlertDlg.SendData1(index, m_nAlertBufMessagePrime, m_nAlertBufMessageSecond);
								index[0] = 0;
								index[1] = 0;
							}
						}

						if(m_bAlertNoMail == true)
						{
							m_bAlertNoMail = false;
							for(int c = 0; c < MAX_STREAM &&  c < m_nLiecensedStream; c++)
							{
								m_nAlertCompairPrime[c] = m_nAlertBufMessagePrime[c];
								m_nAlertCompairSecond[c] = m_nAlertBufMessageSecond[c];
							}
						}
					}
				}
			}
		}

		if(IDT_TIMER_CCERROR == nIDEvent)
		{
			for(int h = 0; h < 2; h++)
				memset(AlertDlg.m_nChannelCCError[h], 0 , 50 * sizeof(int));

			AlertDlg.m_nTotalChannelCCError = 0;
			
			SYSTEMTIME lt;
			GetLocalTime(&lt);

			for(int i = 0; i < MAX_STREAM && i < m_nLiecensedStream; i++)
			{
				if(StreamWatch[i].m_lastCCError != StreamWatch[i].m_CCError)
				{
					TCHAR strError[1000];
					_stprintf_s(strError, 1000, _T("[%04d-%02d-%02d %02d:%02d:%02d:%03d] - CCError - %d\r\n"), lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds, StreamWatch[i].m_CCError);
					
					LogCCError(strError, i);

					TCHAR strCCErrorSummary[1000];
					_stprintf_s(strCCErrorSummary, 1000, _T("[%04d-%02d-%02d %02d:%02d:%02d:%03d] - %d_%s_CCError - %d\r\n"), lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds, i + 1, StreamWatch[i].m_strCstreamChannel_Name, StreamWatch[i].m_CCError);

					LogCCErrorSummary(strCCErrorSummary);

					StreamWatch[i].m_lastCCError = StreamWatch[i].m_CCError;

					if(!StreamWatch[i].m_bCCError && StreamWatch[i].m_CCError >= 15)
					{
						StreamWatch[i].m_bCCError = true;
						AlertDlg.m_nChannelCCError[AlertDlg.m_nTotalChannelCCError][0] = i;
						AlertDlg.m_nChannelCCError[AlertDlg.m_nTotalChannelCCError][1] = StreamWatch[i].m_CCError;
						AlertDlg.m_nTotalChannelCCError++;
						m_bCCErrorMail = false;
					}
				}
				/*else if(StreamWatch[i].m_CCError != 0)
				{
					TCHAR strError[1000];
					_stprintf_s(strError, 1000, _T("[%04d-%02d-%02d %02d:%02d:%02d:%03d] - CCError - %d -> no update in file\r\n"), lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds, StreamWatch[i].m_CCError);

					LogCCError(strError, i);
				}*/

				if(i == (m_nLiecensedStream - 1))
				{
					if(!m_bCCErrorMail)
					{
						m_bCCErrorMail = true;
						AlertDlg.SendCCErrorData();
					}
				}
			}
		}
	}
	catch(...)
	{
	}
}

void CStream_WatchDlg::StorePrevStreamStatus()
{
	CXml xml;
	CString strFilePath;
	CString strMsg;
	int limit = 0;
	if(bStopUDP)
	{
		limit = 1;
	}
	else
	{
		limit = 2;
	}
	strFilePath.Format( _T("%s\\StreamWatchConfigFile.xml"), GetAppPath());	

	if( xml.Open(strFilePath) )
	{
		CString strRoot = L"";
		for(int stream = 0; stream < MAX_STREAM && stream < m_nLiecensedStream; stream++)
		{
			for(int k = 0 ; k < limit ; k++)
			{
			 	strRoot.Format(_T("Stream_%02d"),stream+1);
			    xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("PrimaryInput"), TRUE)->GetChild(_T("Stream_Prev_Status"), TRUE)->SetValue(m_nAlertCompairPrime[stream]);
				xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("SecondaryInput"), TRUE)->GetChild(_T("Stream_Prev_Status"), TRUE)->SetValue(m_nAlertCompairSecond[stream]);
			}
		}
		xml.Save(strFilePath);
		xml.Close();
	}
}

void CStream_WatchDlg::OnBnClickedAlertConfig()
{
	AlertDlg.DoModal();
}

void CStream_WatchDlg::StoreCheckboxData()
{
	//XML 
	CXml xml;
	CString strFilePath;
	CString strMsg;

	strFilePath.Format( _T("%s\\StreamWatchConfigFile.xml"), GetAppPath());	

	if( xml.Open(strFilePath) )
	{
		CString strRoot = L"";
		for(int k = 0; k < MAX_STREAM && k < m_nLiecensedStream; k++)
		{
			strRoot.Format(_T("Stream_%02d"),k+1);
			if(bEMERGENCY)
				xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("PID_Replacement_status"), TRUE)->SetValue(StreamWatch[k].m_Input[0].m_bEmergencyActive);
			xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("Manual_Selection_status"), TRUE)->SetValue(StreamWatch[k].m_bManualCheckStatus);
		}

		xml.Save(strFilePath);
		xml.Close();
	}
}
void CStream_WatchDlg::LoadXML_Started_channels()
{
	CXml xml;
	CString strFilePath;
	CString strMsg;

	strFilePath.Format( _T("%s\\StreamWatchConfigFile.xml"), GetAppPath());	

	if( xml.Open(strFilePath) )
	{
		xml.GetRoot()->GetChild(_T("Streamwatch_channel_info"), TRUE)->GetChild(_T("Total_started_channels"), TRUE)->SetValue(m_nTotalStartedChannel);
		xml.Save(strFilePath);
		xml.Close();
	}

}

//Replacement as per selected prg no
void CStream_WatchDlg::StoreSecondaryPrgNo()
{
	CXml xml;
	CString strFilePath;
	CString strMsg;

	if(!bStopUDP && bEMERGENCY)
	{
		strFilePath.Format( _T("%s\\StreamWatchConfigFile.xml"), GetAppPath());	
		if( xml.Open(strFilePath) )
		{
			CString strRoot = L"";
			for(int k = 0; k < MAX_STREAM && k < m_nLiecensedStream; k++)
			{
				strRoot.Format(_T("Stream_%02d"),k+1);
				for(int j = 0; j < MAX_RECEIVER; j++)
				{
					if(StreamWatch[k].m_Input[j].m_nTotalProgNo > 1)
					{
						//TCHAR strPrgNo[1000];
						_stprintf(StreamWatch[k].m_strPrgNoList, _T("%d"), StreamWatch[k].m_Input[j].m_nProgramNo_Buff[0]);
						xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("SecondaryInput"), TRUE)->GetChild(_T("TotalPrgNo"), TRUE)->SetValue(StreamWatch[k].m_Input[j].m_nTotalProgNo);
						for(int i = 1; i < StreamWatch[k].m_Input[j].m_nTotalProgNo; i++)
							_stprintf(StreamWatch[k].m_strPrgNoList, _T("%s, %d"), StreamWatch[k].m_strPrgNoList, StreamWatch[k].m_Input[j].m_nProgramNo_Buff[i]);
						//strPrgNo.Format(_T("%s, %d"), strPrgNo, StreamWatch[k].m_Input[1].m_nProgramNo_Buff[i]);
						xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("SecondaryInput"), TRUE)->GetChild(_T("Program_Numbers"), TRUE)->SetValue(StreamWatch[k].m_strPrgNoList);
						xml.GetRoot()->GetChild(strRoot, TRUE)->GetChild(_T("SecondaryInput"), TRUE)->GetChild(_T("Selected_PrgNo"), TRUE)->SetValue(StreamWatch[k].m_nSelected_PrgNo);
					}
				}
			}

			xml.Save(strFilePath);
			xml.Close();

		}
	}
}