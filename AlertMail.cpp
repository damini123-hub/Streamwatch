// AlertMail.cpp : implementation file
//

#include "stdafx.h"
#include "StreamWatch.h"
#include "AlertMail.h"
#include "./CXml/Xml.h"
#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>
#include "Shlwapi.h"
#include "StreamWatchDlg.h"

// CAlertMail dialog
using namespace Generic; //for xml classes
MAIL Mail;
int MailCount = 0;
IMPLEMENT_DYNAMIC(CAlertMail, CDialog)
char Status[2][5] = {"Up", "Down"
};

CAlertMail::CAlertMail(CWnd* pParent /*=NULL*/)
	: CDialog(CAlertMail::IDD, pParent)
	, m_strSMTP(_T(""))
{
	m_strUserName = _T("");
	m_strPasswort = _T("");
	m_strReceiverEmailId = _T("");
	m_strSMTP = _T("");

	for(int i = 0; i < 50; i++)
	{
		nStopChannelPrime[i] = 0;
		nStopChannelSecond[i] = 0;
		m_nChangedStatusPrime[i][0] = 0;
		m_nChangedStatusPrime[i][1] = 0;
		m_nChangedStatusSecond[i][0] = 0;
		m_nChangedStatusSecond[i][1] = 0;
		m_nChannelCCError[i][0] = 0;
		m_nChannelCCError[i][1] = 0;
	}
	
	m_nAlertDurationTime = 0;
	for(int i = 0; i < 2; i++)
	{
		nIndex[i] = 0;
		m_nTotalChangedChannels[i] = 0;
		m_nAlertStatusChanged[i] = 0;
		m_bStopChannel[i] = false;
	}
	memset(SystemName, 0, sizeof(TCHAR)* 100);
	m_nTotalChannelCCError = 0;
	m_bPrimUDP = false;
	m_bSecUDP = false;
}

CAlertMail::~CAlertMail()
{
}
BOOL CAlertMail::OnInitDialog()
{
	CDC* pDC = GetDlgItem(IDC_LIST_SELECTED_EMAIL)->GetDC();
	CSize sz;
    int dx = 0;
	CDialog::OnInitDialog();
	if(bXmlFileStatus == true)
	{
		m_strUserName = AlertData.m_strAlertUserName;
		m_strPasswort = AlertData.m_strAlertPassword;
		m_strSMTP = AlertData.m_strAlertSMTP;
		m_nAlertDurationTime = AlertData.m_nAlertTime;
		for(int i = 0; i < m_nTotalEmailId; i++)
		{
			m_List_Final_Email.InsertString(i, AlertData.m_strAlertReceiverEmailId[i]);
			sz = pDC->GetTextExtent(AlertData.m_strAlertReceiverEmailId[i]);

			if(sz.cx > dx)
				dx = sz.cx;
		}
		 m_List_Final_Email.ReleaseDC(pDC);

		if(m_List_Final_Email.GetHorizontalExtent() < dx )
		{
			m_List_Final_Email.SetHorizontalExtent(dx);
			ASSERT(m_List_Final_Email.GetHorizontalExtent() == dx);
		}

		// scroll bar for email list
		pDC = GetDlgItem(IDC_LIST_EMAIL)->GetDC();
		for(int i = 0; i < m_nTotalEmailIdList; i++)
		{
			m_ListBox_Email.InsertString(i, AlertData.m_strAlertEmailList[i]);
			sz = pDC->GetTextExtent(AlertData.m_strAlertEmailList[i]);

			if(sz.cx > dx)
				dx = sz.cx;
		}
		 m_ListBox_Email.ReleaseDC(pDC);

		if(m_ListBox_Email.GetHorizontalExtent() < dx )
		{
			m_ListBox_Email.SetHorizontalExtent(dx);
			ASSERT(m_ListBox_Email.GetHorizontalExtent() == dx);
		}
		m_strReceiverEmailId = _T("");
		
		m_pToolTip = new CToolTipCtrl;
		m_pToolTip->Create(this);
		m_pToolTip->AddTool(GetDlgItem(IDC_EDIT_ALERT_TIME_DURATION),_T("Enter Email Interval in minute."));
		m_pToolTip->AddTool(GetDlgItem(IDC_EDIT_SERVER),_T("Enter sender mail server."));
		m_pToolTip->AddTool(GetDlgItem(IDC_EDIT_USERNAME_ALERT),_T("Enter Sender UserName."));
		m_pToolTip->AddTool(GetDlgItem(IDC_EDIT_PASSWORD_ALERT),_T("Enter Sender Password."));
		m_pToolTip->AddTool(GetDlgItem(IDC_LIST_SELECTED_EMAIL),_T("Selected Email Id to Send Email"));
		m_pToolTip->AddTool(GetDlgItem(IDC_LIST_EMAIL),_T("All Email id. select to add or delete Email Id."));
		m_pToolTip->AddTool(GetDlgItem(IDC_BUTTON_LIST_ADD),_T("Add Selected Email Id."));
		m_pToolTip->AddTool(GetDlgItem(IDC_BUTTON_LIST_ALLADD),_T("Add all Email Id."));
		m_pToolTip->AddTool(GetDlgItem(IDC_BUTTON_LIST_REMOVE),_T("Remove selected Email id from active list."));
		m_pToolTip->AddTool(GetDlgItem(IDC_BUTTON_LIST_REMOVEALL),_T("Remove all Email id from active list."));
		m_pToolTip->AddTool(GetDlgItem(IDC_EDIT_EMAIL_ALERT),_T("Enter Email id to add in a list."));
		m_pToolTip->AddTool(GetDlgItem(IDC_BUTTON_ADD),_T("Click to add Email id"));
		m_pToolTip->AddTool(GetDlgItem(IDC_BUTTON_DELETE),_T("Click to delete selected Email id from list."));
		

		m_pToolTip->Activate(TRUE);
		UpdateData(false);
	}
	return true;
}

BOOL CAlertMail::PreTranslateMessage(MSG* pMsg)
{
	if (NULL != m_pToolTip)
		m_pToolTip->RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}

void CAlertMail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME_ALERT, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD_ALERT, m_strPasswort);
	DDX_Text(pDX, IDC_EDIT_EMAIL_ALERT, m_strReceiverEmailId);
	DDX_Control(pDX, IDC_LIST_EMAIL, m_ListBox_Email);
	DDX_Control(pDX, IDC_LIST_SELECTED_EMAIL, m_List_Final_Email);
	DDX_Text(pDX, IDC_EDIT_ALERT_TIME_DURATION, m_nAlertDurationTime);
	DDX_Text(pDX, IDC_EDIT_SERVER, m_strSMTP);
}

BEGIN_MESSAGE_MAP(CAlertMail, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CAlertMail::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CAlertMail::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_LIST_ADD, &CAlertMail::OnBnClickedButtonListAdd)
	ON_BN_CLICKED(IDC_BUTTON_LIST_ALLADD, &CAlertMail::OnBnClickedButtonListAlladd)
	ON_BN_CLICKED(IDC_BUTTON_LIST_REMOVE, &CAlertMail::OnBnClickedButtonListRemove)
	ON_BN_CLICKED(IDC_BUTTON_LIST_REMOVEALL, &CAlertMail::OnBnClickedButtonListRemoveall)
	ON_EN_KILLFOCUS(IDC_EDIT_ALERT_TIME_DURATION, &CAlertMail::OnEnKillfocusEditAlertTimeDuration)

	ON_BN_CLICKED(IDCANCEL, &CAlertMail::OnClose)
	ON_BN_CLICKED(IDOK, &CAlertMail::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_EDIT_USERNAME_ALERT, &CAlertMail::OnEnKillfocusEditUsernameAlert)
END_MESSAGE_MAP()

//bool IsProcessRunning(const wchar_t *processName)
//{
//    bool exists = false;
//    PROCESSENTRY32 entry;
//    entry.dwSize = sizeof(PROCESSENTRY32);

//    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

//    if (Process32First(snapshot, &entry))
//        while (Process32Next(snapshot, &entry))
//            if (!wcsicmp(entry.szExeFile, processName))
//                exists = true;

//    CloseHandle(snapshot);
//    return exists;
//}

//DWORD WINAPI MonitorExe(LPVOID)
//{
//	while(1)
//	{
//		Sleep(10000);
//		bool exist = IsProcessRunning(L"mail.exe");
//		if(exist)
//		{
//			system("taskkill /f /im mail.exe");
//			exist = false;
//		}
//	}
//	return 1;
//}
 //CAlertMail message handlers
 DWORD WINAPI WatchTXT(LPVOID lpparam)
 {
	int MailCount = *((int *)lpparam);
		Sleep(10000);
	TCHAR Buffer[MAX_PATH];
	TCHAR strFile[MAX_PATH];
    DWORD dwRet;
	dwRet = GetCurrentDirectory(MAX_PATH, Buffer);
	_stprintf_s(strFile, MAX_PATH, _T("%s\\MailDir\\email_%d.txt"), Buffer,MailCount);
	int retval;
	retval = PathFileExists(strFile);
	if(retval == 1)
	{
		bool exist = IsProcessRunning(L"SendMail.exe");
		if(!exist)
 //CAlertMail message handlers
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

	}
	return 0;	
}
void CAlertMail::SendMail()
{
	TCHAR Buffer[MAX_PATH];
	TCHAR folderpath[MAX_PATH];
    DWORD dwRet;
	dwRet = GetCurrentDirectory(MAX_PATH, Buffer);
	_stprintf_s(folderpath, MAX_PATH, _T("%s\\MailDir"), Buffer);
	int retval;
    retval = PathFileExists(folderpath);
	if(retval == 0)
	{
		CreateDirectory(folderpath ,NULL);
	}
    DWORD len = GetCurrentDirectory(0,NULL);
	TCHAR strFile[MAX_PATH];
	MailCount++;
	if(MailCount > 50000)
	{
		MailCount = 0;
	}
	_stprintf_s(strFile, MAX_PATH, _T("%s\\MailDir\\email_%d.txt"), Buffer,MailCount);
    CFile outputfile;
    CFileException excOutputfile;

	int BUF_LENGTH = TotalCheckedBox * 100 + 200;
    char* strPrintMsg = new char[BUF_LENGTH];
	
    if(outputfile.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeBinary, &excOutputfile))
	{
		

		sprintf(strPrintMsg,  "%s \r\n",Mail.SMTP);
        outputfile.Write(strPrintMsg, strlen(strPrintMsg)*sizeof(char));
		sprintf(strPrintMsg,  "%s \r\n",Mail.From);
		outputfile.Write(strPrintMsg, strlen(strPrintMsg)*sizeof(char));
		sprintf(strPrintMsg,  "PASSWORD: \r\n");
        outputfile.Write(strPrintMsg, strlen(strPrintMsg)*sizeof(char));
		sprintf(strPrintMsg,  "%s \r\n",Mail.Password);
        outputfile.Write(strPrintMsg, strlen(strPrintMsg)*sizeof(char));
		sprintf(strPrintMsg,  "TO: \r\n");
        outputfile.Write(strPrintMsg, strlen(strPrintMsg)*sizeof(char));
		for(int listno = 0; listno < m_nTotalEmailId ; listno++)
		{
			sprintf(strPrintMsg,  "%s \r\n",Mail.To[listno]);
			outputfile.Write(strPrintMsg, strlen(strPrintMsg)*sizeof(char));
		}
		sprintf(strPrintMsg, "SUBJECT: \r\n");
        outputfile.Write(strPrintMsg, strlen(strPrintMsg)*sizeof(char));
		sprintf(strPrintMsg,  "%s \r\n",Mail.Subject);
        outputfile.Write(strPrintMsg, strlen(strPrintMsg)*sizeof(char));
		sprintf(strPrintMsg,  "BODY: \r\n");
        outputfile.Write(strPrintMsg, strlen(strPrintMsg)*sizeof(char));
		sprintf(strPrintMsg, "%s \r\n",Mail.Text);
        outputfile.Write(strPrintMsg, strlen(strPrintMsg)*sizeof(char));
		outputfile.Close();
	}
	TCHAR strEmail[2000];
	_stprintf(strEmail, _T("email_%d.txt Written \n"),MailCount);
	LogAlertMail(strEmail);
	//LogStartStopEvents(strEmail, 0);
	OutputDebugString(_T("\n email.txt Written \n"));
	CreateThread(0,0,WatchTXT,&MailCount,0,0);
	delete[] strPrintMsg;
	strPrintMsg = NULL;
}
//void CAlertMail::SendData1(int TotalCount[], int ChnnelNumPrime[], int ChnnelNumSecond[])
//{
//	memset(&Mail,0,sizeof(MAIL));
//
//	int Length = 0;
//	bool bMsgFormat = false;
//	_swprintf(Mail.From, L"%s", m_strUserName);
//	for(int i = 0; i < m_nTotalEmailId; i++)	_swprintf(Mail.To[i],L"%s", AlertData.m_strAlertReceiverEmailId[i]);
//	_swprintf(Mail.SMTP,L"smtp.googlemail.com");
//	_swprintf(Mail.UserName,L"%s",m_strUserName);
//	_swprintf(Mail.Password,L"%s",m_strPasswort);
//	_swprintf(Mail.Subject,L"StreamWatch Input Stream Alert");
//	_swprintf(Mail.Text,L"This email has been auto-generated by StreamWatch.\r\n");
//	
//	for(int k = 0; k < 2; k++)
//	{
//		for(int i = 0; i < TotalCount[k]; i++)
//		{ 
//			if (i == 0)
//			{
//				if(k == 0)
//				{
//					bMsgFormat = true;
//					if(TotalCount[k] < 2)
//						Length += _swprintf(Mail.Text+ Length,L"StreamWatch Primary input is currently not available on \n");
//					else
//						Length += _swprintf(Mail.Text+ Length,L"StreamWatch Primary input are currently not available on \n");
//				}
//				else
//				{
//					if(bMsgFormat == false)
//					{
//						if(TotalCount[k] < 2)
//							Length += _swprintf(Mail.Text + Length,L"StreamWatch Secondary input is currently not available on \n ");
//						else
//							Length += _swprintf(Mail.Text + Length,L"StreamWatch Secondary input are currently not available on \n ");
//					}
//					else
//					{
//						if(TotalCount[k] < 2)
//							Length += _swprintf(Mail.Text + Length,L"And Secondary input is currently not available on \n");
//						else
//							Length += _swprintf(Mail.Text + Length,L"And Secondary input are currently not available on \n");
//					}
//				}
//			}
//			if(k == 0)
//			{
//				Length += _swprintf(Mail.Text + Length,L" of Channel Number %d \n", StreamWatch[ChnnelNumPrime[i]].m_strCstreamChannel_Name, ChnnelNumPrime[i]);
//			}
//			else if(k == 1)
//			{
//				Length += _swprintf(Mail.Text + Length,L"Channel Name: %s of Channel Number %d \n", StreamWatch[ChnnelNumSecond[i]].m_strCstreamChannel_Name, ChnnelNumSecond[i]);
//			}
//		}
//	}
//	
//	_swprintf(Mail.HeaderText,L"Wi5seDV inc.");
//
//	SendMail();
//}
void CAlertMail::SendData(int TotalCount[],int ChnnelNumPrime[],int ChnnelNumSecond[])
{
	memset(&Mail,0,sizeof(MAIL));

	SYSTEMTIME lt;
	GetLocalTime(&lt);
	int Length = 0;
	int txtLength = 0;
	bool bMsgFormat = false;

	if(m_nTotalEmailId > 0)
	{
		for(int i = 0; i < m_nTotalEmailId; i++)
		{
			//sprintf(Mail.To[i],"%s", AlertData.m_strAlertReceiverEmailId[i]);
			wcstombs(Mail.To[i],  AlertData.m_strAlertReceiverEmailId[i], _tcslen( AlertData.m_strAlertReceiverEmailId[i]) + 1);
		}
		//sprintf(Mail.From,"%s",AlertData.m_strAlertUserName);
		wcstombs(Mail.From,  AlertData.m_strAlertUserName, _tcslen( AlertData.m_strAlertUserName) + 1);
		//_swprintf(Mail.SMTP,L"smtp.googlemail.com");
		//sprintf(Mail.UserName,"%s",AlertData.m_strAlertUserName);
		//sprintf(Mail.Password,"%s",AlertData.m_strAlertPassword);
		wcstombs(Mail.UserName,  AlertData.m_strAlertUserName, _tcslen( AlertData.m_strAlertUserName) + 1);
		wcstombs(Mail.Password,  AlertData.m_strAlertPassword, _tcslen( AlertData.m_strAlertPassword) + 1);
		wcstombs(Mail.SMTP,  AlertData.m_strAlertSMTP, _tcslen( AlertData.m_strAlertSMTP) + 1);
		//sprintf(Mail.SMTP,"%s",AlertData.m_strAlertSMTP);	//pw
		//_swprintf(Mail.SMTP,L"smtp.wisedv.com");
		char tempSystemName[100];
		wcstombs(tempSystemName,  SystemName, _tcslen(SystemName) + 1);
		if(m_bPrimUDP == false && m_bSecUDP == false)
		{
			if(TotalCount[0] == 0 && TotalCount[1] == 0)
				Length = sprintf(Mail.Subject," %s is OK : ", tempSystemName);
			else
				Length = sprintf(Mail.Subject," %s : ", tempSystemName);

			txtLength = sprintf(Mail.Text,"[%04d-%02d-%02d %02d:%02d:%02d:%03d] - This email has been auto-generated by StreamWatch.\r\n", lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);

			if(m_nTotalChangedChannels[0] > 0 || m_nTotalChangedChannels[1] > 0)
			{
				for(int s = 0; s < 2; s++)
				{
					for(int k = 0; k < m_nTotalChangedChannels[s]; k++)
					{
						if(s == 1 && k == 0)
						{
							if(bMsgFormat == true)
								Length += sprintf(Mail.Subject + Length,"& Secondary Inputs_");
							else
								Length += sprintf(Mail.Subject + Length,"Secondary Inputs_");
						}

						if(k == 0)
						{
							bMsgFormat = true;
							if(m_nTotalChangedChannels[s] < 2)
								Length += sprintf(Mail.Subject + Length,"%d Channel - %s : ", m_nTotalChangedChannels[s], Status[m_nAlertStatusChanged[s]]);
							else
								Length += sprintf(Mail.Subject + Length,"%d Channels - %s : ", m_nTotalChangedChannels[s], Status[m_nAlertStatusChanged[s]]);
						}

						if(m_nTotalChangedChannels[s] != m_nLiecensedStream)
						{
							if(s == 0 && k == 0)
							{
								if(m_nAlertStatusChanged[s] == 1)
								{
									if(m_nTotalChangedChannels[s] < 2)
										txtLength += sprintf(Mail.Text + txtLength,"StreamWatch Primary input is not available on \n");
									else
										txtLength += sprintf(Mail.Text + txtLength,"StreamWatch Primary input are not available on \n");
								}
								else
									txtLength += sprintf(Mail.Text + txtLength,"StreamWatch Primary input is just available on \n");
							}
							else if(s == 1 && k == 0)
							{
								if(m_nAlertStatusChanged[s] == 1)
								{
									if(m_nTotalChangedChannels[s] < 2)
										txtLength += sprintf(Mail.Text + txtLength,"StreamWatch Secondary input is not available on \n");
									else
										txtLength += sprintf(Mail.Text + txtLength,"StreamWatch Secondary input are not available on \n");
								}
								else
									txtLength += sprintf(Mail.Text + txtLength,"StreamWatch Secondary input is just available on \n");
							}
						}

						char strTempName[20];/*, strTempChannel[20];*/
						if(s == 0)
						{
							wcstombs(strTempName,  StreamWatch[m_nChangedStatusPrime[k][0]].m_strCstreamChannel_Name, _tcslen(StreamWatch[m_nChangedStatusPrime[k][0]].m_strCstreamChannel_Name) + 1);
							/*char *pch = strchr(strTempName, _T('-'));
							if(pch != NULL)
							{
							unsigned int loc = (unsigned int)(pch - strTempName);
							strcpy(strTempChannel, strTempName + loc + 1);
							Length += sprintf(Mail.Subject + Length," %s,", strTempChannel);
							}
							else*/
							Length += sprintf(Mail.Subject + Length," %s,", strTempName);

							if(m_nTotalChangedChannels[s] != m_nLiecensedStream)
								txtLength += sprintf(Mail.Text + txtLength,"Channel Name: %s -- %s \r\n", strTempName, Status[m_nChangedStatusPrime[k][1]]);
							//Length += sprintf(Mail.Subject + Length," %s : %s,",strTempName , Status[m_nChangedStatusPrime[k][1]]);
						}
						else
						{
							wcstombs(strTempName,  StreamWatch[m_nChangedStatusSecond[k][0]].m_strCstreamChannel_Name, _tcslen(StreamWatch[m_nChangedStatusSecond[k][0]].m_strCstreamChannel_Name) + 1);
							/*char *pch = strchr(strTempName, _T('-'));
							if(pch != NULL)
							{
							unsigned int loc = (unsigned int)(pch - strTempName);
							strcpy(strTempChannel, strTempName + loc + 1);
							Length += sprintf(Mail.Subject + Length," %s,", strTempChannel);
							}
							else*/
							Length += sprintf(Mail.Subject + Length," %s,", strTempName);

							if(m_nTotalChangedChannels[s] != m_nLiecensedStream)
								txtLength += sprintf(Mail.Text + txtLength,"Channel Name: %s -- %s \r\n", strTempName, Status[m_nChangedStatusSecond[k][1]]);
							//Length += sprintf(Mail.Subject + Length," %s : %s,",strTempName , Status[m_nChangedStatusSecond[k][1]]);
						}
					}
					sprintf(Mail.Subject + Length - 1," ");
				}
			}
			else
			{
				sprintf(Mail.Subject,"StreamWatch e-Mail Alert");
			}
			sprintf(Mail.HeaderText,"StreamWatch");

			/*TCHAR * str1 = _T("ABC");
			TCHAR * str2 = _T("ABC123");
			TCHAR strMsg[300];
			_stprintf(strMsg, _T("%s"), str1);
			_stprintf(strMsg, _T("%s%s"), strMsg, str2);
			_tcscat(strMsg, str2);*/
			//Length = 0;
			//Length = sprintf(Mail.Text,"[%04d-%02d-%02d %02d:%02d:%02d:%03d] - This email has been auto-generated by StreamWatch.\r\n", lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);
			// _swprintf(Mail.Text,L"This email has been auto-generated by StreamWatch.\r\n");
			nIndex[0] = 0;
			nIndex[1] = 0;
			for(int i = 0; i < MAX_STREAM &&  i < m_nLiecensedStream; i++)
			{
				if(ChnnelNumPrime[i] == 1)
				{
					for(int s = 0; s < m_nTotalChangedChannels[0]; s++)
					{
						if(i == m_nChangedStatusPrime[s][0])
							m_bStopChannel[0] = true;
					}
					if(!m_bStopChannel[0])
					{
						nStopChannelPrime[nIndex[0]] = i;
						nIndex[0]++;
					}
					else
						m_bStopChannel[0] = false;
				}
				if(ChnnelNumSecond[i] == 1)
				{
					for(int s = 0; s < m_nTotalChangedChannels[1]; s++)
					{
						if(i == m_nChangedStatusSecond[s][0])
							m_bStopChannel[1] = true;
					}
					if(!m_bStopChannel[1])
					{
						nStopChannelSecond[nIndex[1]] = i;
						nIndex[1]++;
					}
					else
						m_bStopChannel[1] = false;
				}
			}

			int limit = 0;
			if(bStopUDP)
				limit = 1;
			else
				limit = 2;

			for(int s = 0; s < limit; s++)
			{
				if(m_nTotalChangedChannels[s] != m_nLiecensedStream)
				{
					if(m_nAlertStatusChanged[s] == 1)
					{					
						int nLastStopChannel = TotalCount[s] - m_nTotalChangedChannels[s];
						for(int i = 0; i < nLastStopChannel; i++)
						{
							if(s == 0 && i == 0)
							{
								if(nLastStopChannel < 2)
									txtLength += sprintf(Mail.Text + txtLength,"StreamWatch Primary input is still not available on \n");
								else
									txtLength += sprintf(Mail.Text+ txtLength,"StreamWatch Primary input are still not available on \n");
							}
							else if(s == 1 && i == 0)
							{
								if(nLastStopChannel < 2)
									txtLength += sprintf(Mail.Text + txtLength,"StreamWatch Secondary input is still not available on \n");
								else
									txtLength += sprintf(Mail.Text+ txtLength,"StreamWatch Secondary input are still not available on \n");
							}

							char strTempStopChannel[20];
							if(s == 0)
							{
								wcstombs(strTempStopChannel,  StreamWatch[nStopChannelPrime[i]].m_strCstreamChannel_Name, _tcslen(StreamWatch[nStopChannelPrime[i]].m_strCstreamChannel_Name) + 1);
								txtLength += sprintf(Mail.Text + txtLength,"Channel Name: %s -- %s \r\n", strTempStopChannel, Status[ChnnelNumPrime[nStopChannelPrime[i]]]);
							}
							else
							{
								wcstombs(strTempStopChannel,  StreamWatch[nStopChannelSecond[i]].m_strCstreamChannel_Name, _tcslen(StreamWatch[nStopChannelSecond[i]].m_strCstreamChannel_Name) + 1);
								txtLength += sprintf(Mail.Text + txtLength,"Channel Name: %s -- %s \r\n", strTempStopChannel, Status[ChnnelNumSecond[nStopChannelSecond[i]]]);
							}
						}
					}
					else
					{
						for(int i = 0; i < TotalCount[s]; i++)
						{
							if(s == 0 && i == 0)
							{
								if(TotalCount[s] < 2)
									txtLength += sprintf(Mail.Text + txtLength,"StreamWatch Primary input is still not available on \n");
								else
									txtLength += sprintf(Mail.Text+ txtLength,"StreamWatch Primary input are still not available on \n");
							}
							else if(s == 1 && i == 0)
							{
								if(TotalCount[s] < 2)
									txtLength += sprintf(Mail.Text + txtLength,"StreamWatch Secondary input is still not available on \n");
								else
									txtLength += sprintf(Mail.Text+ txtLength,"StreamWatch Secondary input are still not available on \n");
							}

							char strTempStopChannel[20];
							if(s == 0)
							{
								wcstombs(strTempStopChannel,  StreamWatch[nStopChannelPrime[i]].m_strCstreamChannel_Name, _tcslen(StreamWatch[nStopChannelPrime[i]].m_strCstreamChannel_Name) + 1);
								txtLength += sprintf(Mail.Text + txtLength,"Channel Name: %s -- %s \r\n", strTempStopChannel, Status[ChnnelNumPrime[nStopChannelPrime[i]]]);
							}
							else
							{
								wcstombs(strTempStopChannel,  StreamWatch[nStopChannelSecond[i]].m_strCstreamChannel_Name, _tcslen(StreamWatch[nStopChannelSecond[i]].m_strCstreamChannel_Name) + 1);
								txtLength += sprintf(Mail.Text + txtLength,"Channel Name: %s -- %s \r\n", strTempStopChannel, Status[ChnnelNumSecond[nStopChannelSecond[i]]]);
							}
						}
					}
				}
				else
				{
					if(s == 0)
					{
						if(m_nAlertStatusChanged[s] == 1)
							txtLength += sprintf(Mail.Text + txtLength," All Inputs are down for primary.\n");
					}
					else
					{
						if(m_nAlertStatusChanged[s] == 1)
							txtLength += sprintf(Mail.Text + txtLength," All Inputs are down for secondary.\n");
					}
				}
			}
			//for(int k = 0; k < 2; k++)
			//{
			//	for(int i = 0; i < TotalCount[k]; i++)
			//	{ 
			//		if (i == 0)
			//		{
			//			if(k == 0)
			//			{
			//				bMsgFormat = true;
			//				if(TotalCount[k] < 2)
			//					Length += sprintf(Mail.Text + Length,"StreamWatch Primary input is currently not available on \n");
			//				else
			//					Length += sprintf(Mail.Text+ Length,"StreamWatch Primary input are currently not available on \n");
			//			}
			//			/*else
			//			{
			//				if(bMsgFormat == false)
			//				{
			//					if(TotalCount[k] < 2)
			//						Length += sprintf(Mail.Text + Length,"StreamWatch Secondary input is currently not available on \n ");
			//					else
			//						Length += sprintf(Mail.Text + Length,"StreamWatch Secondary input are currently not available on \n ");
			//				}
			//				else
			//				{
			//					if(TotalCount[k] < 2)
			//						Length += sprintf(Mail.Text + Length,"And Secondary input is currently not available on \n");
			//					else
			//						Length += sprintf(Mail.Text + Length,"And Secondary input are currently not available on \n");
			//				}
			//			}*/
			//		}
			//		if(k == 0)
			//		{
			//			char strTempPrimary[20];
			//			wcstombs(strTempPrimary,  StreamWatch[nStopChannelPrime[i]].m_strCstreamChannel_Name, _tcslen(StreamWatch[nStopChannelPrime[i]].m_strCstreamChannel_Name) + 1);
			//			Length += sprintf(Mail.Text + Length,"Channel Name: %s -- Channel Number %d \r\n", strTempPrimary, nStopChannelPrime[i] + 1);
			//		}
			//		/*if(k == 1)
			//		{
			//			char strTempSecondary[20];
			//			wcstombs(strTempSecondary,   StreamWatch[nStopChannelSecond[i]].m_strCstreamChannel_Name, _tcslen( StreamWatch[nStopChannelSecond[i]].m_strCstreamChannel_Name) + 1);
			//			Length += sprintf(Mail.Text + Length,"Channel Name: %s -- Channel Number %d \r\n", strTempSecondary, nStopChannelSecond[i] + 1);
			//		}*/
			//	}
			//}

			if(TotalCount[0] == 0 && TotalCount[1] == 0)
			{
				txtLength += sprintf(Mail.Text + txtLength," All Inputs are available.\n");
			}
		}
		else
		{
			char strSource[20];
			if(m_bPrimUDP == true && m_bSecUDP == true)
				strcpy(strSource, "primary & secondary");
			else if(m_bPrimUDP == true)
				strcpy(strSource, "primary");
			else if(m_bSecUDP == true)
				strcpy(strSource, "secondary");
			sprintf(Mail.Subject," %s : Network issue in %s", tempSystemName, strSource);
			txtLength = sprintf(Mail.Text,"[%04d-%02d-%02d %02d:%02d:%02d:%03d] - This email has been auto-generated by StreamWatch.\r\n", lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);
			sprintf(Mail.Text + txtLength,"Channels are down due to unexpected network port disconnection detected for %s in streamwatch. \n", strSource);
		}
		sprintf(Mail.HeaderText,"Streamwatch.");

		SendMail();
	}
}

void CAlertMail::SendCCErrorData()
{
	memset(&Mail,0,sizeof(MAIL));
	
	SYSTEMTIME lt;
	GetLocalTime(&lt);
	int Length = 0;

	if(m_nTotalEmailId > 0)
	{
		for(int i = 0; i < m_nTotalEmailId; i++)
		{
			wcstombs(Mail.To[i],  AlertData.m_strAlertReceiverEmailId[i], _tcslen( AlertData.m_strAlertReceiverEmailId[i]) + 1);
		}
		wcstombs(Mail.From,  AlertData.m_strAlertUserName, _tcslen( AlertData.m_strAlertUserName) + 1);
		wcstombs(Mail.UserName,  AlertData.m_strAlertUserName, _tcslen( AlertData.m_strAlertUserName) + 1);
		wcstombs(Mail.Password,  AlertData.m_strAlertPassword, _tcslen( AlertData.m_strAlertPassword) + 1);
		wcstombs(Mail.SMTP,  AlertData.m_strAlertSMTP, _tcslen( AlertData.m_strAlertSMTP) + 1);

		char tempSystemName[100];
		wcstombs(tempSystemName,  SystemName, _tcslen(SystemName) + 1);

		Length = sprintf(Mail.Subject," %s : CCError -", tempSystemName);

		char strTempName[20];/*, strTempChannel[20]*/
		for(int i = 0; i < m_nTotalChannelCCError; i++)
		{
			wcstombs(strTempName,  StreamWatch[m_nChannelCCError[i][0]].m_strCstreamChannel_Name, _tcslen(StreamWatch[m_nChannelCCError[i][0]].m_strCstreamChannel_Name) + 1);
			/*char *pch = strchr(strTempName, _T('-'));
			if(pch != NULL)
			{
			unsigned int loc = (unsigned int)(pch - strTempName);
			strcpy(strTempChannel, strTempName + loc + 1);
			Length += sprintf(Mail.Subject + Length," %s,", strTempChannel);
			}
			else*/
			Length += sprintf(Mail.Subject + Length," %s,", strTempName);
		}
		sprintf(Mail.Subject + Length - 1," ");
		
		Length = 0;

		Length = sprintf(Mail.Text,"[%04d-%02d-%02d %02d:%02d:%02d:%03d] - This email has been auto-generated by StreamWatch.\r\n", lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);
		char strTempChannelName[20];
		for(int i = 0; i < m_nTotalChannelCCError; i++)
		{
			wcstombs(strTempChannelName,  StreamWatch[m_nChannelCCError[i][0]].m_strCstreamChannel_Name, _tcslen(StreamWatch[m_nChannelCCError[i][0]].m_strCstreamChannel_Name) + 1);
			Length += sprintf(Mail.Text + Length," %s_CCError : %d \r\n", strTempChannelName, m_nChannelCCError[i][1]);
		}

		SendMail();
	}
}

CString CAlertMail::GetAppPath(HMODULE hModule = NULL)
{
	TCHAR buf[MAX_PATH] = {'\0'};
	CString strDir, strTemp;

	::GetModuleFileName( hModule, buf, MAX_PATH);
	strTemp = buf;
	strDir = strTemp.Left( strTemp.ReverseFind('\\') + 1 );

	return strDir;
}

bool CAlertMail::findEmail(int StartIndex, CString strEmail)
{
	int Length = strEmail.GetLength();
	int nCountDot = 0;
	TCHAR Emailbuf[40];
	_stprintf(Emailbuf, _T("%s"), strEmail);
	while (StartIndex <= Length)
	{
		if(Emailbuf[StartIndex] == '.')
		{
			nCountDot++;
			StartIndex++;
			if(Emailbuf[StartIndex] == '.')
			{
				return false;
			}
		}
		if(Emailbuf[StartIndex] > 31 && Emailbuf[StartIndex] < 46 )
		{
			//( ( CWnd* ) GetDlgItem( IDC_STATIC_EMAIL ) )->SetWindowText( _T("Email Address in invalid format.") );
			return false;
		}
		if(Emailbuf[StartIndex] > 57 && Emailbuf[StartIndex] < 65 )
		{
			//( ( CWnd* ) GetDlgItem( IDC_STATIC_EMAIL ) )->SetWindowText( _T("Email Address in invalid format.") );
			return false;
		}
		if(Emailbuf[StartIndex] > 90 && Emailbuf[StartIndex] < 96 )
		{
			//( ( CWnd* ) GetDlgItem( IDC_STATIC_EMAIL ) )->SetWindowText( _T("Email Address in invalid format.") );
			return false;
		}
		if(Emailbuf[StartIndex] > 122 && Emailbuf[StartIndex] < 127 )
		{
			//( ( CWnd* ) GetDlgItem( IDC_STATIC_EMAIL ) )->SetWindowText( _T("Email Address in invalid format.") );
			return false;
		}
		if(Emailbuf[StartIndex] == 47 )
		{
			//( ( CWnd* ) GetDlgItem( IDC_STATIC_EMAIL ) )->SetWindowText( _T("Email Address in invalid format.") );
			return false;
		}
		else
		{
			StartIndex++;
		}
	}
	if(StartIndex >= Length)
	{
		return true;
	}
	if(nCountDot < 1)
	{
		( ( CWnd* ) GetDlgItem( IDC_STATIC_EMAIL ) )->SetWindowText( _T("Email Address in invalid format.") );
		return false;
	}
}

bool CAlertMail::isEmail(CString inputEmail)
{
	int Length = inputEmail.GetLength();
	TCHAR Emailbuf[40];
	_stprintf(Emailbuf, _T("%s"), inputEmail);
	bool nEmailProper = false;
	int i = 0;
	int nCountat = 0;
	bool bFirstCharacter = false;
	while(i < Length)
	{
		if(Emailbuf[0] >= 65 && Emailbuf[0] <= 90)
		{
			bFirstCharacter = true;
		}
		if(Emailbuf[0] >= 97 && Emailbuf[0] <= 122)
		{
			bFirstCharacter = true;
		}
		if(bFirstCharacter == false)
		{
			//( ( CWnd* ) GetDlgItem( IDC_STATIC_EMAIL ) )->SetWindowText( _T("Email Address in invalid format.") );
			return false;
		}
		if(Emailbuf[i] == '@' && i >= 1)
		{
			i++;
			nCountat++;
			nEmailProper = findEmail(i, inputEmail);
			if(nEmailProper == true)
			{
				return true;
			}
			else
			{
				//( ( CWnd* ) GetDlgItem( IDC_STATIC_EMAIL ) )->SetWindowText( _T("Email Address in invalid format.") );
				return false;
			}
		}
		if(Emailbuf[i] > 31 && Emailbuf[i] < 46 )
		{
			//( ( CWnd* ) GetDlgItem( IDC_STATIC_EMAIL ) )->SetWindowText( _T("Email Address in invalid format.") );
			return false;
		}
		if(Emailbuf[i] > 57 && Emailbuf[i] < 64 )
		{
			//( ( CWnd* ) GetDlgItem( IDC_STATIC_EMAIL ) )->SetWindowText( _T("Email Address in invalid format.") );
			return false;
		}
		if(Emailbuf[i] > 90 && Emailbuf[i] < 95 )
		{
			//( ( CWnd* ) GetDlgItem( IDC_STATIC_EMAIL ) )->SetWindowText( _T("Email Address in invalid format.") );
			return false;
		}
		if(Emailbuf[i] > 122 && Emailbuf[i] < 127 )
		{
			//( ( CWnd* ) GetDlgItem( IDC_STATIC_EMAIL ) )->SetWindowText( _T("Email Address in invalid format.") );
			return false;
		}
		if(Emailbuf[i] == 47 && Emailbuf[i] == 96 )
		{
			//( ( CWnd* ) GetDlgItem( IDC_STATIC_EMAIL ) )->SetWindowText( _T("Email Address in invalid format.") );
			return false;
		}
		if(Emailbuf[i] == '.' || Emailbuf[i] == '_')
		{
			i++;
			if(Emailbuf[i] == '.' || Emailbuf[i] == '_')
			{
				//( ( CWnd* ) GetDlgItem( IDC_STATIC_EMAIL ) )->SetWindowText( _T("Email Address in invalid format.") );
				return false;
			}
		}
		else
		{
			i++;
		}
	}
	if(nCountat > 1 || nCountat < 1)
	{
		//( ( CWnd* ) GetDlgItem( IDC_STATIC_EMAIL ) )->SetWindowText( _T("Email Address in invalid format.") );
		return false;
	}
 return true;
}

bool CAlertMail::isMailServer(CString mailServer)
{
	int Length = mailServer.GetLength();
	int count = 0;
	TCHAR MailServerbuf[40];
	_stprintf(MailServerbuf, _T("%s"), mailServer);
	for(int i = 0; i < Length; i++)
		if(MailServerbuf[i] == '.')
			count++;
	if(count == 2)
		return true;
	return false;
}

void CAlertMail::xmlDataStore()
{
	UpdateData(true);
	CString strTemp;
	TCHAR strTempTchar[30];
	
	_stprintf(AlertData.m_strAlertUserName,_T("%s"), m_strUserName);
	_stprintf(AlertData.m_strAlertPassword,_T("%s"), m_strPasswort);
	_stprintf(AlertData.m_strAlertSMTP, _T("%s"), m_strSMTP);
	
	m_nTotalEmailId = m_List_Final_Email.GetCount();
	
	for(int i = 0; i < m_nTotalEmailId; i++)
	{
		m_List_Final_Email.GetText(i , AlertData.m_strAlertReceiverEmailId[i]);
	}
	
	m_nTotalEmailIdList = m_ListBox_Email.GetCount();
	
	for(int i = 0; i < m_nTotalEmailIdList; i++)
	{
		m_ListBox_Email.GetText(i , AlertData.m_strAlertEmailList[i]);
	}

	//XML 
	CXml xml;
	CString strFilePath;
	CString strMsg;

	strFilePath.Format( _T("%s\\StreamWatchConfigFile.xml"), GetAppPath());	

	if( xml.Open(strFilePath) )
	{
		xml.GetRoot()->GetChild(_T("Alert"))->Detach(); //Delete old data
		
		xml.GetRoot()->GetChild(_T("Alert"), TRUE)->GetChild(_T("Alert_Duration_Time"), TRUE)->SetValue(AlertData.m_nAlertTime);
		xml.GetRoot()->GetChild(_T("Alert"),TRUE)->GetChild(_T("TotalMailId"), TRUE)->SetValue(m_nTotalEmailId);
		xml.GetRoot()->GetChild(_T("Alert"),TRUE)->GetChild(_T("TotalListMailId"), TRUE)->SetValue(m_nTotalEmailIdList);
		xml.GetRoot()->GetChild(_T("Alert"), TRUE)->GetChild(_T("UserName"), TRUE)->SetValue(AlertData.m_strAlertUserName);
		xml.GetRoot()->GetChild(_T("Alert"), TRUE)->GetChild(_T("Password"), TRUE)->SetValue(AlertData.m_strAlertPassword);
		xml.GetRoot()->GetChild(_T("Alert"), TRUE)->GetChild(_T("SmtpServer"), TRUE)->SetValue(AlertData.m_strAlertSMTP);

		CString strReceiver_Email = L"";
		CString strEmail_List = L"";
		for(int i = 0; i < m_nTotalEmailId; i++)
		{
			strReceiver_Email.Format(_T("Receiver_Email_%02d"),i);
			xml.GetRoot()->GetChild(_T("Alert"), TRUE)->GetChild(strReceiver_Email, TRUE)->SetValue(AlertData.m_strAlertReceiverEmailId[i]);
		}
		for(int i = 0; i < m_nTotalEmailIdList; i++)
		{
			strEmail_List.Format(_T("strEmail_List%02d"),i);
			xml.GetRoot()->GetChild(_T("Alert"), TRUE)->GetChild(strEmail_List, TRUE)->SetValue(AlertData.m_strAlertEmailList[i]);
		}
		xml.Save(strFilePath);
		xml.Close();
	}
}
void CAlertMail::OnBnClickedButtonAdd()
{
	CDC* pDC = GetDlgItem(IDC_LIST_EMAIL)->GetDC();
	CSize sz;
    int dx = 0;
	CString strEmailId;
	bool bEmailEdual = false;
 	GetDlgItemText(IDC_EDIT_EMAIL_ALERT, strEmailId);
	( ( CWnd* ) GetDlgItem( IDC_STATIC_EMAIL ) )->SetWindowText( _T(" ") );
	if(isEmail(strEmailId) == true)
	{
		SetDlgItemText(IDC_EDIT_EMAIL_ALERT, _T(""));
		int length = m_ListBox_Email.GetCount();

		if(length != 0)
		{
			for(int i = 0; i < length; i++)
			{
				CString strTempCompare;
				m_ListBox_Email.GetText( i , strTempCompare);
				if(strEmailId == strTempCompare)
				{
					bEmailEdual = true;
					( ( CWnd* ) GetDlgItem( IDC_STATIC_EMAIL ) )->SetWindowText( _T("Email Address and Receiver User Name is already added.") );
				}
			}
			if(bEmailEdual == false)	
			{
				m_ListBox_Email.AddString(strEmailId);
				sz = pDC->GetTextExtent(strEmailId);

				if(sz.cx > dx)
					dx = sz.cx;
				m_ListBox_Email.SetCurSel(0);
			}
		}
		else
		{
			m_ListBox_Email.AddString(strEmailId);
			sz = pDC->GetTextExtent(strEmailId);

			if(sz.cx > dx)
				dx = sz.cx;
			m_ListBox_Email.SetCurSel(0);
		}
		 m_List_Final_Email.ReleaseDC(pDC);

		if(m_ListBox_Email.GetHorizontalExtent() < dx )
		{
			m_ListBox_Email.SetHorizontalExtent(dx);
			ASSERT(m_ListBox_Email.GetHorizontalExtent() == dx);
		}
		if(bEmailEdual == false)
		{
			xmlDataStore();
			( ( CWnd* ) GetDlgItem( IDC_STATIC_EMAIL ) )->SetWindowText( _T("Successfully Submitted.") );
		}
	}
	else
	{
		( ( CWnd* ) GetDlgItem( IDC_STATIC_EMAIL ) )->SetWindowText( _T("Email Address in invalid format.") );
	}
}

void CAlertMail::OnBnClickedButtonDelete()
{
	( ( CWnd* ) GetDlgItem( IDC_STATIC_EMAIL ) )->SetWindowText( _T(".") );
	int count = m_ListBox_Email.GetSelCount();
    int *lp = new int[1025];
           
    int count2 = m_ListBox_Email.GetSelItems(1024, lp);
 
    for(int i = count2; i > 0; i--)
	{
        CString strEmailId;
        m_ListBox_Email.GetText(lp[i-1], strEmailId);
		int nCountFinal = m_List_Final_Email.GetCount();
		for(int j = 0; j < nCountFinal; j++)
		{
			CString strActiveEmail;
			m_List_Final_Email.GetText(j, strActiveEmail);
			if(strActiveEmail == strEmailId)
			{
				m_List_Final_Email.DeleteString(j);
				j = nCountFinal;
			}
		}
		m_ListBox_Email.DeleteString(lp[i-1]);
	}
	xmlDataStore();
}

void CAlertMail::OnBnClickedButtonListAdd()
{
	CSize sz;
    int dx = 0;
	bool bEqual = false;
	int *lp = new int[1025];

	( ( CWnd* ) GetDlgItem( IDC_STATIC_EMAIL ) )->SetWindowText( _T(".") );
	CDC* pDC = GetDlgItem(IDC_LIST_SELECTED_EMAIL)->GetDC();
	int count = m_ListBox_Email.GetSelCount();
	
    int count2 = m_ListBox_Email.GetSelItems(1024, lp);
	for(int i = 0; i < count2; i++)
	{
	  bEqual = false; 

	  CString strActiveEmail;
	  m_ListBox_Email.GetText(lp[i], strActiveEmail);
	  int nCountFinal = m_List_Final_Email.GetCount();
	  for(int j = 0; j < nCountFinal; j++)
	  {
		 CString strCompairEmailId;
		 m_List_Final_Email.GetText(j, strCompairEmailId);
		 if(strCompairEmailId == strActiveEmail)
		 {
			bEqual = true;
		 }
	  }
	  if(bEqual == false)
	  {
		  m_List_Final_Email.AddString(strActiveEmail);
		  sz = pDC->GetTextExtent(strActiveEmail);

		  if(sz.cx > dx)
				dx = sz.cx;
		  xmlDataStore();
	  }
	}
	m_List_Final_Email.ReleaseDC(pDC);

	if(m_List_Final_Email.GetHorizontalExtent() < dx )
	{
		m_List_Final_Email.SetHorizontalExtent(dx);
		ASSERT(m_List_Final_Email.GetHorizontalExtent() == dx);
	}

}

void CAlertMail::OnBnClickedButtonListAlladd()
{
	( ( CWnd* ) GetDlgItem( IDC_STATIC_EMAIL ) )->SetWindowText( _T(".") );

	CSize sz;
    int dx = 0;
	CString strActiveEmail;

	CDC* pDC = GetDlgItem(IDC_LIST_SELECTED_EMAIL)->GetDC();
	int TotalId = m_ListBox_Email.GetCount();

	for(int i = 0; i < TotalId; i++)
	{
		bool bEqual = false;
		m_ListBox_Email.GetText(i, strActiveEmail);
		int TotalFinalId = m_List_Final_Email.GetCount();
		for(int j = 0; j < TotalFinalId; j++)
		{
			CString strCompairEmailId;
			m_List_Final_Email.GetText(j, strCompairEmailId);
			if(strCompairEmailId == strActiveEmail)
			{
				bEqual = true;
			}
		}
		if(bEqual == false)
		{
			m_List_Final_Email.AddString(strActiveEmail);

			sz = pDC->GetTextExtent(strActiveEmail);
			if(sz.cx > dx)
				dx = sz.cx;

			xmlDataStore();
		}
	}

	m_List_Final_Email.ReleaseDC(pDC);
	if(m_List_Final_Email.GetHorizontalExtent() < dx )
	{
		m_List_Final_Email.SetHorizontalExtent(dx);
		ASSERT(m_List_Final_Email.GetHorizontalExtent() == dx);
	}
}

void CAlertMail::OnBnClickedButtonListRemove()
{
	int CountEMailList = m_List_Final_Email.GetSelCount();
    int *lp1 = new int[1025];
           
    int CountSelectedEMailList = m_List_Final_Email.GetSelItems(1024, lp1);
 
    for(int i = CountSelectedEMailList; i > 0; i--)
    {
        // CString strEmailId;
        // m_List_Final_Email.GetText(lp1[i-1], strActiveEmail);
		m_List_Final_Email.DeleteString(lp1[i-1]);
    }
	xmlDataStore();
}

void CAlertMail::OnBnClickedButtonListRemoveall()
{
	int nTotalCount = m_List_Final_Email.GetCount();
	for(int i = nTotalCount; i > 0; i--)
	{
		m_List_Final_Email.DeleteString(i-1);
	}
	xmlDataStore();
}

void CAlertMail::OnEnKillfocusEditAlertTimeDuration()
{
	UpdateData(true);
	int m_nDurationAlert = m_nAlertDurationTime;
	if(m_nDurationAlert < 0 || m_nDurationAlert > 30)
	{
		MessageBox(_T("Alert Duration time should between 0 to 30 minute"),_T("ERROR"));
		GetDlgItem(IDC_EDIT_ALERT_TIME_DURATION)->SetWindowTextW(_T(""));
	}
	else
	{
		AlertData.m_nAlertTime = m_nDurationAlert;
		xmlDataStore();
	}
}

void CAlertMail::OnClose()
{
	OnBnClickedOk();
}

void CAlertMail::OnBnClickedOk()
{
	UpdateData(true);
	CString strTempUserId;
	GetDlgItemText(IDC_EDIT_USERNAME_ALERT, strTempUserId);
	if(m_strUserName.IsEmpty() == false)
	{
		if(isEmail(strTempUserId) == false)
		{
			MessageBox(_T("Invalid Username"),_T("ERROR"));
		}
		else
		{
			if((m_strSMTP.IsEmpty() == true) || (m_strPasswort.IsEmpty() == true))
			{
				MessageBox(_T("Outgoing Mail server and password must not be empty"),_T("ERROR"));
			}
			else if(isMailServer(m_strSMTP) == false)
			{
				MessageBox(_T("Invalid Outgoing Mail Server"),_T("ERROR"));
			}
			else
			{
				xmlDataStore();
				OnOK();
			}
		}
	}
	else
	{
		MessageBox(_T("Username must not be empty"),_T("ERROR"));
		xmlDataStore();
		OnOK();
	}
}

void CAlertMail::OnEnKillfocusEditUsernameAlert()
{
	UpdateData(true);
	CString strTempUserId;
	GetDlgItemText(IDC_EDIT_USERNAME_ALERT, strTempUserId);
	if(m_strUserName.IsEmpty() == false)
	{
		if(isEmail(strTempUserId) == false)
		{
			MessageBox(_T("Invalid Username"),_T("ERROR"));
		}
	}
	else
	{
		MessageBox(_T("Username must not be empty"),_T("ERROR"));
	}
}
