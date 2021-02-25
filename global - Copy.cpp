#include "global.h"
#include "Stream.h"
#include "sample_net_common.h"
#include "net_client_engine.h"
#include <tlhelp32.h>
#include "Shlwapi.h"

#define ResourceBuild(x,y) x##y

CStream StreamWatch[MAX_STREAM];
int m_nLiecensedStream; 
int TotalCheckedBox = 0;
bool bClickeStartAll = false;
bool bXmlFileStatus = false;
bool bStopUDP = false;
bool bStartVideoAnalysis = false;
bool bEMERGENCY = false;


DWORD WINAPI sender(LPVOID lpParameter)
{
	//struct sockaddr_in si_other;
	size_t Temp;
	const int BUF_SIZE = 100;
	const int STARTSTOPBUF_SIZE = 800;
	const int ERRORBUF_SIZE = 300;
	const int TIMEBUF_SIZE = 1000;
	CStream *pStream = (CStream*)lpParameter;
	OUTPUT *pOutput = &pStream->m_Output;

	// create socket
	if(!bStopUDP)
	{
		pStream->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (pStream->sock == SOCKET_ERROR)
		{
			const int BUF_SIZE = 300;
			TCHAR strError[BUF_SIZE];
			_stprintf_s(strError, BUF_SIZE, _T("socket creation failed with error code : %d"), WSAGetLastError());
			LogErrorInfo(strError, pStream->m_nChannel_Index);
			goto Error_Label;
		}

	char strNicDest[MAX_SIZE];
	wcstombs_s(&Temp, strNicDest, (size_t)MAX_SIZE, pOutput->strNic_Source, (size_t)MAX_SIZE);
	struct in_addr ip_adaptor;
	ip_adaptor.S_un.S_addr = inet_addr((const char *)strNicDest);
		
	if (setsockopt(pStream->sock, IPPROTO_IP, IP_MULTICAST_IF, (char *)&ip_adaptor, sizeof(ip_adaptor)) < 0)
	{
		TCHAR strError[BUF_SIZE];
		_stprintf_s(strError, BUF_SIZE, _T("setsocket failed with error code : %d"), WSAGetLastError());
		LogErrorInfo(strError, pStream->m_nChannel_Index);
		goto Error_Label;
	}

	// setup adress structure
	char strIpDest[MAX_SIZE];
	wcstombs_s(&Temp, strIpDest,(size_t)MAX_SIZE, pOutput->strIp_Source, (size_t)MAX_SIZE);

		pStream->Sender_addr.sin_family = AF_INET;
		pStream->Sender_addr.sin_port = htons(pOutput->nPort_Source);
		pStream->Sender_addr.sin_addr.s_addr = inet_addr((const char *)strIpDest);
	}

	LARGE_INTEGER qpc;
	QueryPerformanceFrequency(&qpc);

	LARGE_INTEGER lPerformanceCount,lPrevPerformanceCount;
	LARGE_INTEGER lPerformanceFreq;
	lPrevPerformanceCount.QuadPart = lPerformanceCount.QuadPart = 0;
	LARGE_INTEGER lWaitStart;
		
	double WaitAccumulate = 0;
	double diffWait = 0,lwaitadd= 0;
	double nTotalFraction = 0;
	QueryPerformanceFrequency(&lPerformanceFreq);

	pOutput->m_nPcr_Check = 0;
	pOutput->m_nPrevPCR_value = 0;
	pOutput->m_nCurrPCR_value = 0;
	pOutput->m_nTimeDiff = 40;
	pOutput->m_nSend_StartPoint = 0;
	pOutput->m_nSend_NextPoint = 0;
	pOutput->m_nSendOffset = 0;
	pOutput->m_bIndex_Diff = false;
	pOutput->m_nIndex_Read = 0;

	pStream->m_nCurrDuration = 100;
	pStream->m_nSwitch_Input = 0;

	int checkm_nSend_StartPoint = 0;
	int SendToException = 0;
	int Prev_SameIndex_Read = 0;
	int Curr_SameIndex_Read= 0;
	int Prev_SameIndex_Write = 0;
	int Curr_SameIndex_Write = 0;
	int Pcr_value_Diff = 0;
	//Reassignment
	int *CI = &pStream->m_nSwitch_Input;

	TCHAR strStartStopBuf[STARTSTOPBUF_SIZE];
	memset(strStartStopBuf, 0, STARTSTOPBUF_SIZE*sizeof(TCHAR));

	_stprintf_s(strStartStopBuf, STARTSTOPBUF_SIZE, _T("START SENDER THREAD---->Of Stream %d"), pStream->m_nChannel_Index);
	LogStartStopEvents(strStartStopBuf, pStream->m_nChannel_Index);

	while(pOutput->m_bThread_Terminate == false)
	{
		if(pOutput->m_bIndex_Diff == true)
		{
			pStream->DecisionMaking(); // Decision Function

			CInput *pCInput = &pStream->m_Input[*CI];

			if((pCInput->m_bPCRPID_Found == true && bStartVideoAnalysis == false) || (bStartVideoAnalysis == true && pCInput->m_bVideoHealth == false && pCInput->m_bPCRPID_Found == true))
			{
				if(pCInput->m_bScanStartDiff == true && pOutput->m_nIndex_Read != pCInput->m_nIndex_Scan)
				//if(pCInput->m_bScanStartDiff == true)
				{
				int k = 0;
				while(k < 7)
				{
					try
					{
						memcpy(pCInput->m_PCRSender_Buf, pCInput->m_SourceBuffer[pOutput->m_nIndex_Read] + (k * 188), 188);
					}
					catch(...)
					{
						LogException(_T("Problem in creating AnalysBuffer for send the data"),pStream->m_nChannel_Index);
					}
					//Find PCR_VALue
					pCInput->m_nPcr_TempValue = pCInput->Find_PCR_Value(pCInput->m_PCRSender_Buf,pCInput->m_nFound_PCR_PID);

					if(pCInput->m_nPcr_TempValue != 0)
					{
						//pCInput->m_dwLastPCR_PIDArrived = GetTickCount();
						//Read Next PCR_VALUE Index
						pOutput->m_nSend_StartPoint = pOutput->m_nSend_NextPoint;
						pOutput->m_nSend_NextPoint = pOutput->m_nIndex_Read;
						pOutput->m_nSendOffset = pOutput->m_nSend_NextPoint - pOutput->m_nSend_StartPoint;

						TCHAR Offset[100];
						_stprintf(Offset, _T(" \n  Current Offset %d\n"), pOutput->m_nSendOffset);
						//OutputDebugString(Offset);

						if(pOutput->m_nSendOffset < 0)
						{
							pOutput->m_nSendOffset = pOutput->m_nSend_NextPoint - pOutput->m_nSend_StartPoint + MAX_PACKET_SIZE;

							TCHAR Offset[100];
							_stprintf(Offset, _T(" \n AFTER LOOP Current Offset %d\n"), pOutput->m_nSendOffset);
							//OutputDebugString(Offset);
						}

						//PCR_VALUE assignment
						pOutput->m_nPrevPCR_value = pOutput->m_nCurrPCR_value;
						pOutput->m_nCurrPCR_value = pCInput->m_nPcr_TempValue;
						// Send routine
						for(int i = 0; i < pOutput->m_nSendOffset; i++)
						{		
							try
							{
								if(!bStopUDP)
								{
									if(sendto(pStream->sock, (const char*)pCInput->m_SourceBuffer[pOutput->m_nSend_StartPoint], 1316, 0, (sockaddr *)&pStream->Sender_addr, sizeof(pStream->Sender_addr)) < 0)
									{
										pOutput->m_bSendTo_Status = false;

										TCHAR strError[ERRORBUF_SIZE];
										_stprintf_s(strError,ERRORBUF_SIZE, _T("sentto() failed with error code Permenantly : %d") , WSAGetLastError());
										LogErrorInfo(strError, pStream->m_nChannel_Index);
									}
									else
									{
										pOutput->m_bSendTo_Status = true;
									}
								}
								else
								{
									Sleep(100);
								}
							}
							catch(...)
							{
								SendToException++;

								TCHAR strException[ERRORBUF_SIZE];
								_stprintf_s(strException,ERRORBUF_SIZE, _T("Sento Attempt : %d") , SendToException);
								LogException(strException, pStream->m_nChannel_Index);

								if(SendToException < 6)
								{
									i--;
									break;
								}
								else
								{
									LogException(_T("Sento Permenantly fail."),pStream->m_nChannel_Index);
									goto Error_Label;
								}
							}
								
						pOutput->m_nSend_StartPoint++;
						if(pOutput->m_nSend_StartPoint >= MAX_PACKET_SIZE)
							pOutput->m_nSend_StartPoint = 0;
							if(i == (pOutput->m_nSendOffset - 1))
							{
								if(pOutput->m_nPrevPCR_value != 0)
									pOutput->m_nTimeDiff = double(pOutput->m_nCurrPCR_value - pOutput->m_nPrevPCR_value)/27000;
								
								//TCHAR strMsgoffset[1000];
								//_stprintf(strMsgoffset, _T("\n ####### UDP Packet offset = %d ||||| PCR_Value Difference = %.02f"), pOutput->m_nSendOffset, pOutput->m_Output.m_nTimeDiff);
								//OutputDebugString(strMsgoffset);
							if(pOutput->m_nTimeDiff > 1000 || pOutput->m_nTimeDiff < 0)
							{
								//TCHAR strInfo[200];
								//_stprintf(strInfo, _T(" Error:  m_Output.m_Output.m_nTimeDiff = %.4f ---and UDP PAckege %d\n"),pOutput->m_nTimeDiff,pOutput->m_nSendOffset);
								//OutputDebugString(strInfo);
								pOutput->m_nTimeDiff = 40;
							}
							QueryPerformanceCounter(&lPerformanceCount);
							WaitAccumulate = (double)(lPerformanceCount.QuadPart - lPrevPerformanceCount.QuadPart)*1000/lPerformanceFreq.QuadPart;
							if(WaitAccumulate >= (double)(pOutput->m_nTimeDiff + lwaitadd))
							{

							}
							else // wait loop
							{
								double nTotalWait = pOutput->m_nTimeDiff + lwaitadd - WaitAccumulate;
								QueryPerformanceCounter(&lWaitStart);

								if(nTotalWait >= pOutput->m_nTimeDiff - 3)//10)//15)//25)
								{
									if(nTotalWait - 3 > 0)
										::Sleep((int)nTotalWait - 3);
									QueryPerformanceCounter(&lPerformanceCount);
									diffWait = (double)(lPerformanceCount.QuadPart - lWaitStart.QuadPart)*1000/lPerformanceFreq.QuadPart;
								}
								else if(nTotalWait >= 13 && nTotalWait < 25)//10)//15)//25)
								{
									::Sleep((int)nTotalWait - 3);
									QueryPerformanceCounter(&lPerformanceCount);
									diffWait = (double)(lPerformanceCount.QuadPart - lWaitStart.QuadPart)*1000/lPerformanceFreq.QuadPart;
								}
								else
								{
									if(nTotalWait > 5)
										::Sleep((int)nTotalWait - 3);
									TCHAR strMsg[TIMEBUF_SIZE];
									_stprintf_s(strMsg, TIMEBUF_SIZE, _T("@@@@ nTotalWait = %d Time diff = %f WaitAccumulate = %f\n"), nTotalWait, pOutput->m_nTimeDiff, WaitAccumulate);
									//OutputDebugString(strMsg);
								}

								QueryPerformanceCounter(&lPerformanceCount);
								diffWait = (double)(lPerformanceCount.QuadPart - lWaitStart.QuadPart)*1000/lPerformanceFreq.QuadPart;
								//if(pStream->m_nChannel_Index == 0)
								//{
								//	//TCHAR strMsg[TIMEBUF_SIZE];
								//	//_stprintf_s(strMsg, TIMEBUF_SIZE, _T("\n @@@@ nTotalWait = %d Time diff = %f WaitAccumulate = %f - diffWait = %f\n"), nTotalWait, pStream->m_nTimeDiff, WaitAccumulate, diffWait);
								//	//OutputDebugString(strMsg);
								//}

								while(1)
								{
									QueryPerformanceCounter(&lPerformanceCount);
									diffWait = (double)(lPerformanceCount.QuadPart - lWaitStart.QuadPart)*1000/lPerformanceFreq.QuadPart;
									if((diffWait + WaitAccumulate) >= (double)(pOutput->m_nTimeDiff + lwaitadd))
										break;
									if(pOutput->m_bThread_Terminate == true)
										break;
								}
							}

								lPrevPerformanceCount = lPerformanceCount;
							}
						}
					}
					k++;
					}
				}
				//Update ReadIndex of Sender
				int nCurrentIndex_diff = pCInput->m_nIndex_Write - pOutput->m_nIndex_Read;
				if(nCurrentIndex_diff < 0)
					nCurrentIndex_diff += MAX_PACKET_SIZE;
				if(nCurrentIndex_diff > 0)
				{
					//pOutput->m_nIndex_Read = pCInput->m_nCurRead;
					if(++pOutput->m_nIndex_Read >= MAX_PACKET_SIZE)
					{
						pOutput->m_nIndex_Read = 0;
						//pStream->read_Round++;
					}
				}
				else
				{
					Prev_SameIndex_Read = Curr_SameIndex_Read;
					Prev_SameIndex_Write = Curr_SameIndex_Write;
					Curr_SameIndex_Read = pOutput->m_nIndex_Read;
					Curr_SameIndex_Write = pCInput->m_nIndex_Write;
					if(Prev_SameIndex_Read == Curr_SameIndex_Read && Curr_SameIndex_Read == Curr_SameIndex_Write)
					{
						if(pOutput->m_bCountEqualIndex == false)
						{
							pOutput->m_bCountEqualIndex = true;
							pOutput->m_dwFisrtEqualTime = GetTickCount();
						}
						else
						{
							if((GetTickCount() - pOutput->m_dwFisrtEqualTime) >= 100)
							{
								if(pOutput->m_bSendTo_Status)
								{
									LogException(_T("\n Read Write Input Index are same. Send signal Stop\n"), pStream->m_nChannel_Index);
									pOutput->m_bSendTo_Status = false;
								}
							}
						}
						
					}
					else
					{
						pOutput->m_bCountEqualIndex = false;
					}
					Sleep(40);					
				}
			}
			else
			{
				Sleep(40);
			}
		}
		else
		{
			pStream->IntialDecisionMaking();
		}
	}
	Error_Label:
		
	if(pOutput->m_bThread_Terminate == true)
	{
		_stprintf_s(strStartStopBuf, STARTSTOPBUF_SIZE, _T("STOP SENDER THREAD---->Of Stream %d \r\n"), pStream->m_nChannel_Index);
		LogStartStopEvents(strStartStopBuf, pStream->m_nChannel_Index);
		closesocket(pStream->sock);
	}
	return 0;
}
DWORD WINAPI scannar(LPVOID lpParameter)
{
	int nTempRead = 0;
	CInput *pInput = (CInput*)lpParameter;

	pInput->m_nCurRead = 0;
	pInput->m_bPCRPID_Found = false;
	pInput->m_bScanStartDiff = false;
	pInput->m_bstartSending = false;
	pInput->m_nIndex_Scan = 0;
	pInput->m_bPAT_Found =  false;
	pInput->m_bPMT_Found =  false;
	pInput->m_bPCRPID_Found = false; 
	pInput->m_bPCRValue_Found =  false;
	pInput->m_bVideoPidFound = false;
	pInput->m_nProgram_Num = 0;
	pInput->m_nVideoPid = 0;
	pInput->m_nSwitchIndicator = 0;

	while(pInput->m_bScannar_Terminate == false)
	{
		if(pInput->m_bScanStartDiff == true)
		{
			if(pInput->m_nIndex_Scan != pInput->m_nIndex_Write)
			{
				if(pInput->m_bPCRPID_Found == false)
				{
					pInput->Indentify_Scan_Input();
				}
				else
				{
					pInput->Scan_Replacement_Input();
				}
				pInput->m_nIndex_Scan++;
						
				if(pInput->m_nIndex_Scan == MAX_PACKET_SIZE)
					pInput->m_nIndex_Scan = 0;
			}
			else
			{
				Sleep(40);
			}
		}
		else
		{
			if(pInput->m_nIndex_Write > (MAX_SIZE - 50))
			{
				pInput->m_bScanStartDiff = true;
			}
			else
			{
				if(0 == pInput->m_dwLastUpdateTime)
				{
					pInput->m_dwLastUpdateTime = GetTickCount();
					pInput->m_dwFisrtUpdateTime = pInput->m_dwLastUpdateTime;
				}
				Sleep(100);
			}	
		}
	}
	return 0;
}
//void Check_MPTS_Stream(int StreamId)
//{
//	int nMPTSStreamPrime[48];
//	int nMPTSStreamSecond[48];
//	int nTotalMPTSStreamPrime = 0;
//	int nTotalMPTSStreamSecond = 0;
//	int j;
//
//	TCHAR strError[2000], strStartStopBuf[800];
//	if(StreamId == -1)
//	{
//		for(int i = 0; i < MAX_STREAM; i++)
//		{
//			for(int m = 0; m < MAX_RECEIVER; m++)
//			{
//				if(StreamWatch[i].m_Input[m].m_bMPTS_Stream == true)
//				{
//					if(m == 0)
//					{
//						nMPTSStreamPrime[nTotalMPTSStreamPrime] = i+1;
//						nTotalMPTSStreamPrime++;
//						_stprintf_s(strStartStopBuf, 800, _T("STOP STREAM---MPTS Stream in Primary Input of Channel-> %d"), i);
//					}
//					else
//					{
//						nMPTSStreamSecond[nTotalMPTSStreamSecond] = i+1;
//						nTotalMPTSStreamSecond++;
//						_stprintf_s(strStartStopBuf, 800, _T("STOP STREAM---MPTS Stream in Secondary Input of Channel-> %d"), i);
//					}
//					LogStartStopEvents(strStartStopBuf, i);
//				}
//			}
//		}
//
//		for(int k = 0; k < nTotalMPTSStreamPrime; k++)
//		{
//			if(k == 0)
//			{
//				j = _stprintf(strError, _T("MPTS Streams are found in \n Primary Input of channel  %d "), nMPTSStreamPrime[k]);
//			}
//			else
//				j += _stprintf( strError + j, _T(" %d "), nMPTSStreamPrime[k]); // C4996
//			//if(k == (nTotalMPTSStream - 1))
//			//{
//			//	j += _stprintf( strError + j, _T(" \n Only SPTS is Valid "), nMPTSStream[k]); // C4996
//			//}
//		}
//		for(int k = 0; k < nTotalMPTSStreamSecond; k++)
//		{
//			if(k == 0)
//			{
//				j = _stprintf(strError, _T(" %s MPTS Streams are found in \n Secondary Input of channel  %d "),strError ,nMPTSStreamPrime[k]);
//			}
//			else
//				j += _stprintf( strError + j, _T(" %d "), nMPTSStreamPrime[k]); // C4996
//		}
//	}
//	else
//	{
//		_stprintf_s(strError,800, _T("MPTS Streams are found at Channal %d "), StreamId + 1);
//	}
//	AfxMessageBox(strError);
//}
void LogException(TCHAR* strMsg , int streamId)
{
	bool bFileTooBig = false;
	bool bFileOpen = false;

	CFile outputfile;
	CFileException excOutputfile;

	try{
		SYSTEMTIME lt;
		GetLocalTime(&lt);

		TCHAR strFile[MAX_PATH];
		if(streamId == -1)
			_stprintf_s(strFile, MAX_PATH, _T("Exception_Error_%04d-%02d-%02d.log"), lt.wYear, lt.wMonth, lt.wDay);
		else
			_stprintf_s(strFile, MAX_PATH, _T("Stream_%d_Exception_Error_%04d-%02d-%02d.log"), streamId, lt.wYear, lt.wMonth, lt.wDay);
			

		if(outputfile.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeBinary, &excOutputfile))
		{
			if(outputfile.GetLength() > 10 * 1024 * 1024)//More Than 10 MB
			{
				bFileTooBig = true;
				bFileOpen = false;
				outputfile.Close();
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

			TCHAR strNewFileName[MAX_PATH];
			if(streamId == -1)
				_stprintf_s(strNewFileName, MAX_PATH, _T("Exception_Error_%04d-%02d-%02d_Part_%02d-%02d-%02d.log"), lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond);
			else
				_stprintf_s(strNewFileName, MAX_PATH, _T("Stream_%d_Exception_Error_%04d-%02d-%02d_Part_%02d-%02d-%02d.log"), streamId, lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond);
			
			try
			{
				CFile::Rename(strFile, strNewFileName);
			}
			catch(...)
			{
			}
			if(outputfile.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeBinary, &excOutputfile))
				bFileOpen = true;
		}
		if(!bFileTooBig && bFileOpen)
		{
			const int BUF_SIZE = 1500;
			TCHAR strError[BUF_SIZE];
			_stprintf_s(strError, BUF_SIZE, _T("[%04d-%02d-%02d %02d:%02d:%02d:%03d] - %s\r\n"), lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds, strMsg);
			//OutputDebugString(strError);

			outputfile.SeekToEnd();
			outputfile.Write(strError, _tcslen(strError)*sizeof(TCHAR));
			outputfile.Close();
		}
	}
	catch(...)
	{
	}
}

void LogErrorInfo(TCHAR* strMsg , int streamId)
{
	TCHAR strFile[MAX_PATH];

	bool bFileTooBig = false;
	bool bFileOpen = false;

	try{
		SYSTEMTIME lt;
		GetLocalTime(&lt);
		if(streamId == -1)
			_stprintf_s(strFile, MAX_PATH, _T("Stream_XX_Error_Info_%04d-%02d-%02d-%02d.log"), lt.wYear, lt.wMonth, lt.wDay, lt.wHour);
		else
			_stprintf_s(strFile, MAX_PATH, _T("Stream_%d_Error_Info_%04d-%02d-%02d-%02d.log"), streamId, lt.wYear, lt.wMonth, lt.wDay, lt.wHour);		

		CFile outputfile;
		CFileException excOutputfile;
		if(outputfile.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeBinary, &excOutputfile))
		{
			if(outputfile.GetLength() > 10 * 1024 * 1024)//More Than 10 MB
			{
				bFileTooBig = true;
				bFileOpen = false;
				outputfile.Close();
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

			TCHAR strNewFileName[MAX_PATH];
			if(streamId == -1)
				_stprintf_s(strNewFileName, MAX_PATH, _T("Error_Info_%04d-%02d-%02d-%02d_Part_%02d-%02d-%03d.log"), lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);
			else
				_stprintf_s(strNewFileName, MAX_PATH, _T("Stream_%d_Error_Info_%04d-%02d-%02d-%02d_Part_%02d-%02d-%03d.log"), streamId, lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);
			try
			{
				CFile::Rename(strFile, strNewFileName);
			}
			catch(...)
			{
			}

			if(outputfile.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeBinary, &excOutputfile))
				bFileOpen = true;
			else
				bFileOpen = false;
		}
		
		if(!bFileTooBig && bFileOpen)
		{
			const int BUF_SIZE = 1500;
			TCHAR strError[BUF_SIZE];
			_stprintf_s(strError, BUF_SIZE, _T("[%04d-%02d-%02d %02d:%02d:%02d:%03d] - %s\r\n"), lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds, strMsg);
			//OutputDebugString(strError);

			outputfile.SeekToEnd();
			outputfile.Write(strError, _tcslen(strError)*sizeof(TCHAR));
			outputfile.Close();
		}
		
		if(!bFileOpen)
		{
			TCHAR szMsg[1025];
			excOutputfile.GetErrorMessage(szMsg, 1025);
			try{
				if(outputfile.m_hFile != CFile::hFileNull)
					outputfile.Close();
			}catch(...){}

			const int BUF_SIZE = 1200;
			TCHAR strError[BUF_SIZE];
			_stprintf_s(strError, BUF_SIZE, _T("DebugInfo File could not be open because %s"), szMsg);
			LogException(strError, -1);
		}
	}
	catch(...)
	{
		const int BUF_SIZE = 1200;
		TCHAR strError[BUF_SIZE];
		_stprintf_s(strError, BUF_SIZE, _T("DebugInfo File could not be open - %s"), strFile);
		LogException(strError, -1);
	}
}

void LogStartStopEvents(TCHAR* strMsg , int streamId)
{
    try{
        SYSTEMTIME lt;
        GetLocalTime(&lt);

        TCHAR strFile[MAX_PATH];
		_stprintf_s(strFile, MAX_PATH, _T("Stream_%d_StartStop_%04d-%02d-%02d.log"), streamId + 1, lt.wYear, lt.wMonth, lt.wDay);

        CFile outputfile;
        CFileException excOutputfile;
        if(outputfile.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeBinary, &excOutputfile))
        {
			const int BUF_LENGTH = 500;
            TCHAR strPrintMsg[BUF_LENGTH];

			if(wcscmp(strMsg,_T("\r\n")) == 0)
				_stprintf_s(strPrintMsg, BUF_LENGTH, _T("%s"),strMsg);
			else
				_stprintf_s(strPrintMsg, BUF_LENGTH, _T("[%04d-%02d-%02d %02d:%02d:%02d:%03d] - %s\r\n"), lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds, strMsg);

            outputfile.SeekToEnd();
            outputfile.Write(strPrintMsg, _tcslen(strPrintMsg)*sizeof(TCHAR));
            outputfile.Close();
        }
		else
		{
			TCHAR szMsg[1025];
			excOutputfile.GetErrorMessage(szMsg, 1025);
			try
			{
				if(outputfile.m_hFile != CFile::hFileNull)
					outputfile.Close();
			}
			catch(...){}

			const int BUF_SIZE = 1200;
			TCHAR strError[BUF_SIZE];
			_stprintf_s(strError, BUF_SIZE, _T("Internal_DTMF_Signal_ File could not be open because %s"), szMsg);
			LogException(strError, streamId);
		}
    }
    catch(...)
    {
    }
}
bool IsProcessRunning(const wchar_t *processName)
{
    bool exists = false;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (Process32First(snapshot, &entry))
        while (Process32Next(snapshot, &entry))
            if (!wcsicmp(entry.szExeFile, processName))
                exists = true;
    CloseHandle(snapshot);
    return exists;
}