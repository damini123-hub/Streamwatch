#include "Stream.h"
#include "Shlwapi.h"

CStream::CStream()
{
	m_bUpdateGapFile = false;
	m_FileCreateTime = 0;
	m_ModifiedFileTime = 0; 

	m_nCheck_EngineStatus = false;
	
	m_nChannel_Index = 0;
	m_nCheckBox_Status = 0;	
	m_bValidation_Status = false;
	m_dwStartClickTime = 0;
	m_bValidation_Error = false;
	m_bManualCheckStatus = 0;

	m_dwFirstAvalableInput = 0;
	m_dwGoodPrimeDetector = 0;
	m_nGoodPrimeCompair = 0;
	m_bStartBackSwitch= 0;
	m_nCurrDuration = 100;
	m_nDetection_Time = 0;
	m_nPreDetection_Time = 0;
	m_nSwitch_Input = 0;
	
	SenderThreadID = 0;
	SenderHandle = INVALID_HANDLE_VALUE;
	m_bPrimUDPDetect = false;
	m_bSecUDPDetect = false;

	m_ScheduleManualSwitching= 3;
	m_LastModifiedTime = 0;
	m_LastReadedTime = 0;

	m_bStart = false;
	m_nSelected_PrgNo = 0;
	m_strSelected_PrgNo = _T("");
	m_bSelected_PrgNo = false;
	m_nTotal_PrgNo = 0;
	for(int i = 0 ; i < 50; i++)
		m_strPrgNo_List[i] = _T("");

	m_bCCVideo = false;
	for(int i = 0; i < 5; i++)
	{
		m_bCCAudio[i] = false;
		m_lastCCAudio[i] = 0;
	}
	m_lastCCVideo = 0;
	m_CCError = 0;
	strCCError = _T("");
	m_lastCCError = 0;
	m_ModifiedCCError = 0;
	m_bCCError = false;
	m_CCErrorResetFile = 1;
}

void CStream::CleanUp_Engine(int i)
{
	if ( pEngine[i] )
	{ 				
		delete pEngine[i];
		pEngine[i] = NULL;
	}
}

void CStream::CLEANUP_ALL()
{
	for( int i = 0; i < MAX_RECEIVER; i++)
	{
		CleanUp_Engine(i);
	}
}

int CStream::InitEngine(int i)
{
	size_t tempId;
	TCHAR strBufWide[MAX_SIZE];
	char strBuf[MAX_SIZE];
	char strNIC[MAX_SIZE];

	_stprintf_s(strBufWide, MAX_SIZE, _T("udp://%s:%d"), m_Input[i].strIp_Source, m_Input[i].nPort_Source);

#ifdef UNICODE
	wcstombs_s(&tempId,strBuf, (size_t)MAX_SIZE, strBufWide, (size_t)MAX_SIZE);
	wcstombs_s(&tempId,strNIC,(size_t)MAX_SIZE, m_Input[i].strNic_Source, (size_t)MAX_SIZE);
#else
	_stprintf_s(strBuf, MAX_SIZE, _T("%s"), strBufWide);
	_stprintf_s(strNIC, MAX_SIZE, _T("%s"), m_Input[i].strNic_Source);
#endif

	m_Input[0].m_nIndex_Write = 0;
	m_Input[1].m_nIndex_Write = 0;

	pEngine[i] = new CNetLiveClientEngine(i , this);
	int nInit_Check = pEngine[i]->Init((const int8_t*)strBuf, (const int8_t*)strNIC, 0);
	if(nInit_Check == 0)
	{
		m_nCheck_EngineStatus = true;
	}
	if( nInit_Check < 0)
	{
		return 0;
	}
	return 1;
}
void CStream::SetIndex(int index)
{
	m_nChannel_Index = index;
	m_Input[0].m_nChannelNumber = m_nChannel_Index;
	m_Input[1].m_nChannelNumber = m_nChannel_Index;
}

int CStream::InitSession()
{
	CLEANUP_ALL();

	for(int i = 0; i < MAX_RECEIVER; i++)
	{
		if(!InitEngine(i))
		{
			CleanUp_Engine(i);
			return 0;
		}
	}
	return 1;
}

int CStream::PlaybackLoop()
{
	for( int i = 0; i < MAX_RECEIVER; i++)
	{
		if ( pEngine[i]->GetState() == engineStopped ) 
		{
			if(!InitEngine(i))
			{
				CleanUp_Engine(i);
				return 0;
			}
		}
		pEngine[i]->Run();
	}
	return 1;
}
void CStream::Start()
{
	for(int i = 0; i < 2; i++)
	{
		if(bStartVideoAnalysis)
			m_Input[i].m_bVideoHealth = false;

		m_Input[i].m_bnOVedioPid = false;
		m_Input[i].m_bScannar_Terminate = false;
		m_Input[i].m_bReset = false;
		m_Input[i].m_nPCR_Index = 0;
		m_Input[i].m_nPCR_ReadIndex = 0;
		memset(m_Input[i].m_nPCR_ValueIndex, 0, sizeof(m_Input[i].m_nPCR_ValueIndex));
	}
	m_Output.m_bThread_Terminate = false;
	int nSession_Check = InitSession();
	if( nSession_Check <= 0)
	{
		CLEANUP_ALL();
		m_bValidation_Error = true;
		LogErrorInfo(_T("Error in initializing receiver engines"),m_nChannel_Index+1);
		//MessageBox(NULL, _T("Error in initializing receiver engines"), _T("ERROR"), MB_OK);
	}
	if(m_bValidation_Error == false)
	{
		SenderHandle = CreateThread(0, 0, sender , this , 0, &SenderThreadID);//strat Sending
		for(int i = 0; i < 2; i++)
		{
			if(i == 0)
			{
				m_Input[i].m_bPrimaryInput = true;
				m_Input[i].pPrimary = NULL;
			}
			else
			{
				m_Input[i].pPrimary = &m_Input[i - 1]; 
			}
			m_Input[i].ScannarHandle = CreateThread(0, 0, scannar , &m_Input[i] , 0, &m_Input[i].ScannarThreadID);
		}
		PlaybackLoop();//Start Receiving Data
	}
}

bool CStream::Stop()
{
	const int BUF_SIZE = 100;
	TCHAR strMsg[BUF_SIZE];
	m_Output.m_bThread_Terminate = true;
	m_Input[0].m_bScannar_Terminate = true;
	m_Input[1].m_bScannar_Terminate = true;
	m_Input[0].m_bReset = true;
	m_Input[1].m_bReset = true;
	if(m_nCheck_EngineStatus == true)
	{
		_stprintf_s(strMsg, BUF_SIZE, _T("Do you want to stop streaming?"));
		if(IDNO == MessageBox(NULL, strMsg, _T("StreamWatch"), MB_ICONWARNING | MB_YESNO | MB_TOPMOST))
			return false;
		else
		{
			CLEANUP_ALL();	
			m_nCheck_EngineStatus = false;
			return true;
		}
	}
	else
	{
		_stprintf_s(strMsg, BUF_SIZE, _T("Do you want to exit application?"));
		if(IDNO == MessageBox(NULL, strMsg, _T("StreamWatch"), MB_ICONWARNING | MB_YESNO | MB_TOPMOST))
			return false;
		else
		{
			return true;
		}
	}
}

void CStream::readVideoAnalysisInfo(int ChannelIndex)
{
	    m_bUpdateGapFile = false;
		CFile FilePointer;
		CFileStatus Status;
		CFileException excFile;
		TCHAR strData[300];
		memset(strData, 0, sizeof(strData));
		if(FilePointer.Open(m_strFPathVideopara, CFile::modeRead | CFile::typeBinary, &excFile))
		{
			FilePointer.GetStatus(Status);
			m_FileCreateTime = Status.m_mtime;
			int m = FilePointer.GetLength();
			FilePointer.Seek(2, CFile::begin);
			FilePointer.Read(strData, m * sizeof(TCHAR));
			FilePointer.Close();
		}

		TCHAR *strToken = NULL;

         if(_tcscmp(strData, _T("")) != 0)
        {
            strToken = _tcstok((wchar_t*)strData, _T("~"));
			m_Input[0].m_nVideoGap = _ttoi(strToken);
          
			strToken = _tcstok(NULL, _T("~"));
            m_Input[0].m_nInvalidCount = _ttoi(strToken);
        
            strToken = _tcstok(NULL, _T("~"));
			m_Input[0].m_nValidVideoCount = _ttoi(strToken);

			strToken = _tcstok(NULL, _T("~"));
			m_Input[1].m_nVideoGap = _ttoi(strToken);

			strToken = _tcstok(NULL, _T("~"));
            m_Input[1].m_nInvalidCount = _ttoi(strToken);

			strToken = _tcstok(NULL, _T("~"));
			m_Input[1].m_nValidVideoCount = _ttoi(strToken);
        }
		 else
		{
			m_Input[0].m_nVideoGap = 1500;
            m_Input[0].m_nInvalidCount = 5;
			m_Input[0].m_nValidVideoCount = 1500;

			m_Input[1].m_nVideoGap = 1500;
            m_Input[1].m_nInvalidCount = 5;
			m_Input[1].m_nValidVideoCount = 1500;

			TCHAR strTemp[200];
			int m = _stprintf(strTemp, _T("%d~%d~%d~%d~%d~%d"),m_Input[0].m_nVideoGap, m_Input[0].m_nInvalidCount, m_Input[0].m_nValidVideoCount, m_Input[1].m_nVideoGap, m_Input[1].m_nInvalidCount, m_Input[1].m_nValidVideoCount);
			if(FilePointer.Open(m_strFPathVideopara, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary, &excFile))
			{
				FilePointer.Write(strTemp, m * sizeof(TCHAR));
				FilePointer.Close();
			}
		}
		TCHAR help[200];
		_stprintf(help, _T("Hello read complete %d %d %d"), m_Input[0].m_nVideoGap,   m_Input[0].m_nInvalidCount, m_Input[0].m_nValidVideoCount);
		//OutputDebugString(help);
}

CString CStream::GetAppPath(HMODULE hModule = NULL)
{
	TCHAR buf[MAX_PATH] = {'\0'};
	CString strDir, strTemp;

	::GetModuleFileName( hModule, buf, MAX_PATH);
	strTemp = buf;
	strDir = strTemp.Left( strTemp.ReverseFind('\\') + 1 );

	return strDir;
}
void CStream::ScheduleSwitchingCheck()
{
	CFile FilePointer;
	CFileStatus Status;
	CFileException excFile;

	TCHAR strFilePath[150];
	memset(strFilePath, 0, 150);
	_stprintf(strFilePath, _T("%s\\ScheduleManualSwitch_%d.txt"), GetAppPath(), m_nChannel_Index + 1);
	const int STARTSTOPBUF_SIZE = 800;
	TCHAR strStartStopBuf[STARTSTOPBUF_SIZE];
	memset(strStartStopBuf, 0, STARTSTOPBUF_SIZE*sizeof(TCHAR));

	char strData[10];
	memset(strData, 0, sizeof(strData));
	if(FilePointer.Open(strFilePath, CFile::modeRead | CFile::typeBinary, &excFile))
	{
		int m = FilePointer.GetLength();
		//FilePointer.Seek(2, CFile::begin);
		FilePointer.Read(strData, m * sizeof(char));
		FilePointer.Close();

			int Check = atoi(strData);
			if(Check == 1)
		{
			if(m_ScheduleManualSwitching != 1)
				{
					m_ScheduleManualSwitching = 1;
					LogStartStopEvents(_T("\r\n"), m_nChannel_Index);
					_stprintf_s(strStartStopBuf, STARTSTOPBUF_SIZE, _T("Read Value of ScheduleManualSwitch_%d : %d. Switch to Secondary stream."), m_nChannel_Index, m_ScheduleManualSwitching);
					LogStartStopEvents(strStartStopBuf, m_nChannel_Index);
				}
			}
			else if(Check == 0)
			{
			if(m_ScheduleManualSwitching != 0)
			{
					m_ScheduleManualSwitching = 0;
					LogStartStopEvents(_T("\r\n"), m_nChannel_Index);
					_stprintf_s(strStartStopBuf, STARTSTOPBUF_SIZE, _T("Read Value of ScheduleManualSwitch_%d : %d. Switch to Primary stream."), m_nChannel_Index, m_ScheduleManualSwitching);
					LogStartStopEvents(strStartStopBuf, m_nChannel_Index);
				}
			}
			else
			{
			if(m_ScheduleManualSwitching != 2)
			{
				m_ScheduleManualSwitching = 2;
				LogStartStopEvents(_T("\r\n"), m_nChannel_Index);
				_stprintf_s(strStartStopBuf, STARTSTOPBUF_SIZE, _T("Read Value of ScheduleManualSwitch_%d : %d. Switch to Primary stream. Schedule Manual Switching is OFF."), m_nChannel_Index, m_ScheduleManualSwitching);
				LogStartStopEvents(strStartStopBuf, m_nChannel_Index);
			}
		}
	}
}

void CStream::DecisionMaking()
{
	const int STARTSTOPBUF_SIZE = 800;
	TCHAR strStartStopBuf[STARTSTOPBUF_SIZE];
	memset(strStartStopBuf, 0, STARTSTOPBUF_SIZE*sizeof(TCHAR));

	ScheduleSwitchingCheck();

	if(m_bManualCheckStatus == 1)
	{
		if(m_nSwitch_Input == 0)
		{
			m_Input[1].m_nSwitchIndicator = 1; 
			m_nSwitch_Input = 1;
			LogStartStopEvents(_T("\r\n"), m_nChannel_Index);
			_stprintf_s(strStartStopBuf, STARTSTOPBUF_SIZE, _T("SWITCHING------Wait Period Time = %d is over-------Switch to Secondary Stream: Reason Manual switch Activated---Schedule switching %d"), m_nCurrDuration, m_ScheduleManualSwitching);
			LogStartStopEvents(strStartStopBuf, m_nChannel_Index);

			m_Output.m_nIndex_Read = m_Input[m_nSwitch_Input].m_nIndex_Write - MAX_SIZE;
			if(m_Output.m_nIndex_Read < 0)
			{
				m_Output.m_nIndex_Read += MAX_PACKET_SIZE;
			}

			m_Output.m_nSend_NextPoint = m_Output.m_nIndex_Read + 1;
			m_Output.m_nPrevPCR_value = 0;
			m_Output.m_nCurrPCR_value = 0;
			m_Output.m_nTimeDiff = 40;
			m_Input[1].m_nPCR_Index = 0;
			m_Input[1].m_nPCR_ReadIndex = 0;
			memset(m_Input[1].m_nPCR_ValueIndex, 0, sizeof(m_Input[1].m_nPCR_ValueIndex));
			TCHAR strInfo[200];
			_stprintf_s(strInfo,200, _T("\n Manual----------- Switching to stream %d "), m_nSwitch_Input);
			//OutputDebugString(strInfo);
		}
					
	}
	else
	{
		// switching logic start [[
		m_nCurrDuration = GetTickCount() - m_Input[0].m_dwLastUpdateTime;
		if(m_nSwitch_Input == 1 && m_nCurrDuration < m_nDetection_Time)
		{
			if(m_bStartBackSwitch == false)
			{
				m_bStartBackSwitch = true;
				m_dwGoodPrimeDetector = m_Input[0].m_dwLastUpdateTime;
				m_dwFirstAvalableInput = GetTickCount();
			}
			else
			{
				m_nGoodPrimeCompair = m_Input[0].m_dwLastUpdateTime - m_dwGoodPrimeDetector;
			}
		}
		else
		{
			m_dwGoodPrimeDetector = 0;
			m_bStartBackSwitch = false;
		}
		if((m_nSwitch_Input == 0 && m_nCurrDuration > m_nDetection_Time) || (m_nSwitch_Input == 1 && m_nGoodPrimeCompair > m_nPreDetection_Time && m_bStartBackSwitch == true))
		{
			if(m_nSwitch_Input == 0)
			{
			    m_Input[1].m_nSwitchIndicator = 1;
				CString strReason = m_Input[0].Switch_Reason();
				m_nSwitch_Input = 1;
				m_Input[m_nSwitch_Input].m_nPCR_Index = 0;
				m_Input[m_nSwitch_Input].m_nPCR_ReadIndex = 0;
				memset(m_Input[m_nSwitch_Input].m_nPCR_ValueIndex, 0, sizeof(m_Input[m_nSwitch_Input].m_nPCR_ValueIndex));
				LogStartStopEvents(_T("\r\n"), m_nChannel_Index);
				_stprintf_s(strStartStopBuf, STARTSTOPBUF_SIZE, _T("SWITCHING------Wait Period Time = %d is over-------Switch to Secondary Stream. Reason %s"), m_nCurrDuration, strReason);
				LogStartStopEvents(strStartStopBuf, m_nChannel_Index);
			}
			else
			{
				m_Input[1].m_nSwitchIndicator = 0;
				m_nSwitch_Input = 0;
				m_Input[m_nSwitch_Input].m_nPCR_Index = 0;
				m_Input[m_nSwitch_Input].m_nPCR_ReadIndex = 0;
				memset(m_Input[m_nSwitch_Input].m_nPCR_ValueIndex, 0, sizeof(m_Input[m_nSwitch_Input].m_nPCR_ValueIndex));
				LogStartStopEvents(_T("\r\n"), m_nChannel_Index);
				_stprintf_s(strStartStopBuf, STARTSTOPBUF_SIZE, _T("SWITCHING------Wait Period Time = %d is over-------Switch to Primary Stream."), m_nGoodPrimeCompair);
				LogStartStopEvents(strStartStopBuf, m_nChannel_Index);
			}
			m_Output.m_nIndex_Read = m_Input[m_nSwitch_Input].m_nIndex_Write - MAX_SIZE;
			if(m_Output.m_nIndex_Read < 0)
			{
				m_Output.m_nIndex_Read += MAX_PACKET_SIZE;
			}

			TCHAR strInfo[200];
			if(m_nSwitch_Input == 0)
				_stprintf_s(strInfo, 200, _T("\n Wait Period Time = %d is over --- Switching to stream %d and Switch check bool %d "), m_nGoodPrimeCompair, m_nSwitch_Input, m_bStartBackSwitch);
			else
				_stprintf_s(strInfo, 200, _T("\n Wait Period Time = %d is over --- Switching to stream %d "), m_nCurrDuration, m_nSwitch_Input);
			//OutputDebugString(strInfo);

			//If we want to analyze Stream agian after switch Input
			m_Input[m_nSwitch_Input].m_bVideoPidFound = false;
			m_Input[m_nSwitch_Input].m_bPAT_Found = false;
			m_Input[m_nSwitch_Input].m_bPMT_Found = false; 
			m_Input[m_nSwitch_Input].m_bPCRPID_Found = false;
			m_Input[m_nSwitch_Input].m_bPCRValue_Found = false;

			m_Output.m_nSend_NextPoint = m_Output.m_nIndex_Read + 1;
			m_Output.m_nPrevPCR_value = 0;
			m_Output.m_nCurrPCR_value = 0;
			m_Output.m_nTimeDiff = 40;
			m_dwGoodPrimeDetector = 0;
		}
	}

	DWORD CurrentTime = GetTickCount();
	int nTempPrime = CurrentTime - m_Input[0].m_dwLastUpdateTime;
	int nTemSecond = CurrentTime - m_Input[1].m_dwLastUpdateTime;

	//Sento Status Checking
	if(m_Input[m_nSwitch_Input].m_bPCRPID_Found == false)
	{
		if(m_bManualCheckStatus == 1)
		{
			if(nTemSecond > m_nDetection_Time)
			{
				m_Output.m_bSendTo_Status = false;
			}
		}
		else
		{
			if(nTempPrime > m_nDetection_Time && nTemSecond > m_nDetection_Time)
			{
				m_Output.m_bSendTo_Status = false;
			}
		}
	}
	else
	{
		if((m_Input[m_nSwitch_Input].m_bVideoHealth == false && bStartVideoAnalysis == true) || bStartVideoAnalysis == false)
		{
			if(m_bManualCheckStatus == 1)
			{
				if(nTemSecond > m_nDetection_Time)
				{
					if(m_Output.m_nIndex_Read >= m_Output.m_nSend_NextPoint)
					{
						if((m_Output.m_nIndex_Read - m_Output.m_nSend_NextPoint) < 100)
						{
							m_Output.m_bSendTo_Status = false;
						}
					}
					else if(m_Output.m_nIndex_Read < m_Output.m_nSend_NextPoint)
					{
						if(((m_Output.m_nIndex_Read - m_Output.m_nSend_NextPoint) + MAX_PACKET_SIZE) < 100)
						{
							m_Output.m_bSendTo_Status = false;
						}
					}
				}
			}
			else
			{
				if(nTempPrime > m_nDetection_Time && nTemSecond > m_nDetection_Time)
				{
					if(m_Output.m_nIndex_Read >= m_Output.m_nSend_NextPoint)
					{
						if((m_Output.m_nIndex_Read - m_Output.m_nSend_NextPoint) < 100)
						{
							m_Output.m_bSendTo_Status = false;
						}
					}
					else if(m_Output.m_nIndex_Read < m_Output.m_nSend_NextPoint)
					{
						if(((m_Output.m_nIndex_Read - m_Output.m_nSend_NextPoint) + MAX_PACKET_SIZE) < 100)
						{
							m_Output.m_bSendTo_Status = false;
						}
					}
				}
			}
		}
		else
		{
			if(bStartVideoAnalysis)
			{
				m_Output.m_bSendTo_Status = false;
				Sleep(40);
			}
		}
	}
}

void CStream::IntialDecisionMaking()
{
	const int STARTSTOPBUF_SIZE = 800;
	TCHAR strStartStopBuf[STARTSTOPBUF_SIZE];
	memset(strStartStopBuf, 0, STARTSTOPBUF_SIZE*sizeof(TCHAR));

	if(m_Input[0].m_nIndex_Write == 0)
	{
		int nTempCalculationTime = (GetTickCount() - m_dwStartClickTime);
		if(nTempCalculationTime > m_nDetection_Time)
		{
			m_Input[1].m_nSwitchIndicator = 1;
			m_nSwitch_Input = 1;
			m_Input[m_nSwitch_Input].m_nPCR_Index = 0;
		    m_Input[m_nSwitch_Input].m_nPCR_ReadIndex = 0;
		    memset(m_Input[m_nSwitch_Input].m_nPCR_ValueIndex, 0, sizeof(m_Input[m_nSwitch_Input].m_nPCR_ValueIndex));
		}
		if(m_Input[1].m_nIndex_Write == 0)
		{
			m_Input[1].m_nSwitchIndicator = 0;
			m_nSwitch_Input = 0;
			m_Input[m_nSwitch_Input].m_nPCR_Index = 0;
			m_Input[m_nSwitch_Input].m_nPCR_ReadIndex = 0;
			memset(m_Input[m_nSwitch_Input].m_nPCR_ValueIndex, 0, sizeof(m_Input[m_nSwitch_Input].m_nPCR_ValueIndex));
		}
	}
		
	if(m_Output.m_bIndex_Diff == false && m_Input[m_nSwitch_Input].m_nIndex_Write > MAX_SIZE)
	{
		m_Output.m_bIndex_Diff = true;

		//TCHAR strTemp[50];
		_stprintf(strStartStopBuf, _T("Index Diff over %d of channal %d"), m_nSwitch_Input, m_nChannel_Index);
		//OutputDebugString(strTemp);
		LogStartStopEvents(strStartStopBuf, m_nChannel_Index);
	}
	else
	{
		Sleep(100);
	}			
}

void CStream::ResetCCError(int nChannelNumber)
{
	try
	{
		bool bFileTooBig = false;
		bool bFileOpen = false;

		SYSTEMTIME lt;
		GetLocalTime(&lt);

		CFile CCErrorResetFile;
		CFileException excCCErrorResetFile;

		CString strFilePath;
		TCHAR folderpath[MAX_PATH];
		_stprintf_s(folderpath, MAX_PATH, _T("%s\\CCErrorResetLog"), GetFolderPath());

		int retval;
		retval = PathFileExists(folderpath);
		if(retval == 0)
		{
			CreateDirectory(folderpath ,NULL);
		}

		strFilePath.Format( _T("%s\\CCErrorReset.log"), folderpath);

		TCHAR strError[1000];
		_stprintf_s(strError, 1000, _T("[%04d-%02d-%02d %02d:%02d:%02d:%03d] - %d_%s_CCError is reset\r\n"), lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds, nChannelNumber + 1, StreamWatch[nChannelNumber].m_strCstreamChannel_Name);
		if(CCErrorResetFile.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeBinary, &excCCErrorResetFile))
		{	
			if(CCErrorResetFile.GetLength() > 10 * 1024 * 1024)//More Than 10 MB
			{
				bFileTooBig = true;
				bFileOpen = false;
				CCErrorResetFile.Close();
			}
			else
			{
				bFileOpen = true;
				bFileTooBig = false;
			}
		}

		//Rename Current File
		if(bFileTooBig)
		{
			bFileTooBig = false;
			CString strNewFilePath;
			strNewFilePath.Format( _T("%s\\CCErrorReset_Part_%d.log"), folderpath, m_CCErrorResetFile++);

			try
			{
				CFile::Rename(strFilePath, strNewFilePath);
			}
			catch(...)
			{
			}
			if(CCErrorResetFile.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeBinary, &excCCErrorResetFile))
				bFileOpen = true;
		}

		if(!bFileTooBig && bFileOpen)
		{
			CCErrorResetFile.SeekToEnd();
			CCErrorResetFile.Write(strError, _tcslen(strError)*sizeof(TCHAR));
			CCErrorResetFile.Close();
		}
	}
	catch(...)
	{
	}
}