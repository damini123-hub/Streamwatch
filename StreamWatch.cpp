// New_Version_Stream_Watch.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "StreamWatch.h"
#include "StreamWatchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <Mmsystem.h>
#include <time.h>

extern "C" NTSYSAPI LONG NTAPI NtQueryTimerResolution(PULONG MinimumResolution, PULONG MaximumResolution, PULONG CurrentResolution);
extern "C" NTSYSAPI LONG NTAPI NtSetTimerResolution(ULONG DesiredResolution, BOOLEAN SetResolution, PULONG CurrentResolution);

// CStream_WatchApp

BEGIN_MESSAGE_MAP(CStream_WatchApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// CStream_WatchApp construction

CStream_WatchApp::CStream_WatchApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CStream_WatchApp object

CStream_WatchApp theApp;

// CStream_WatchApp initialization

BOOL CStream_WatchApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

//RETRY_LICENSE_VALIDATION:
	int ValidStatus = 2;
	m_LicenseValidator.m_bAppStartLicenseCheck = true;
	ValidStatus = m_LicenseValidator.IsLicenseValid();

	if(ValidStatus == 3)
	{	
		return FALSE;
	}
	else if(ValidStatus == 2)
	{	
		AfxMessageBox(_T("Essential Files are missing in system. \nPlease re-install Ad Serter to solve this problem"), MB_ICONSTOP);
		return FALSE;
	}
	else if(ValidStatus == 1)
	{
		AfxMessageBox(_T("Invalid License Key Is Found."), MB_ICONSTOP);
		return FALSE;
	}
	FILE *fIndex = NULL;
	TCHAR Index_Stream[10];
	memset(Index_Stream, 0, sizeof(Index_Stream));
	TCHAR tempPath[MAX_PATH];
	memset(tempPath, 0, MAX_PATH);
	::GetModuleFileName(0, tempPath, MAX_PATH);
	unsigned int loc = (unsigned int)(_tcsrchr(tempPath, _T('\\')) - tempPath);
	tempPath[loc] = NULL;

	_stprintf_s(tempPath,MAX_PATH, _T("%s\\Index.txt"), tempPath);

	 _tfopen_s(&fIndex,tempPath, _T("r"));
	if(fIndex != NULL)
	{
		fread(Index_Stream, sizeof(Index_Stream), 1, fIndex);
		fclose(fIndex);
	}
	const int BUF_SIZE = 50;
	TCHAR strMutex[BUF_SIZE];
	_stprintf_s(strMutex, BUF_SIZE, _T("StreamWatch%s"), Index_Stream);
	//_stprintf_s(strMutex, BUF_SIZE, _T("StreamWatch"));
	m_hAppMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, strMutex);
	if (!m_hAppMutex)
			m_hAppMutex = CreateMutex(NULL, FALSE, strMutex); //Create Mutex for First Application
	else
	{			
		AfxMessageBox(_T("The current instance is already running !!"), MB_ICONSTOP);
		return FALSE; 
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	TIMECAPS ptc;
	bool bTimeCapUsed = false;
	ULONG DesiredResolution = 5000;// The requested resolution in 100 ns units: - it will set 0.5 ms
	ULONG CurrentResolution = 0;
	ULONG MinimumResolution = 0;
	ULONG MaximumResolution = 0;

	if( 0 == NtQueryTimerResolution(&MinimumResolution, &MaximumResolution, &CurrentResolution))
	{
		DesiredResolution = MaximumResolution;
	}

	if (NtSetTimerResolution(DesiredResolution, TRUE, &CurrentResolution) != 0)
	{
		UINT cbtc = sizeof(TIMECAPS);
		if(MMSYSERR_NOERROR != timeGetDevCaps(&ptc, cbtc))
		{
			TCHAR strError[BUF_SIZE];
			_stprintf_s(strError, BUF_SIZE, _T("Error for timeGetDevCaps()\n"));
			LogErrorInfo(strError, -1);
		}

		if(MMSYSERR_NOERROR != timeBeginPeriod(ptc.wPeriodMin))
		{
			TCHAR strError[BUF_SIZE];
			_stprintf_s(strError, BUF_SIZE, _T("Error for timeBeginPeriod()\n"));
			LogErrorInfo(strError, -1);
		}	
	}
	
	CStream_WatchDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	timeEndPeriod(ptc.wPeriodMin);

	if(bTimeCapUsed)
		timeEndPeriod(ptc.wPeriodMin);
	else
	{
		NtSetTimerResolution(DesiredResolution, FALSE, &CurrentResolution);
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
