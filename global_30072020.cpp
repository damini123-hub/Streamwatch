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
int m_CCErrorSummaryFile = 1;
//bool bPrimUDPDetection = false;
//bool bSecUDPDetection = false;

DWORD WINAPI sender(LPVOID lpParameter)
{
	/*TCHAR strPath[MAX_PATH];
	_stprintf(strPath, L"D:\\damini\\Stream_Watch_5.0_AS_Primary_MailNotSent\\Release\\Sample.ts");
	FILE* fp = _tfopen(strPath, L"wb");*/
	//struct sockaddr_in si_other;
	size_t Temp;
	const int BUF_SIZE = 100;
	const int STARTSTOPBUF_SIZE = 800;
	const int ERRORBUF_SIZE = 300;
	const int TIMEBUF_SIZE = 1000;
	CStream *pStream = (CStream*)lpParameter;
	OUTPUT *pOutput = &pStream->m_Output;

	double nTotalInterval = 0, nTimeToSend = 0, nTotalTimeToSend = 0, nInterval = 0;
	DWORD Flags = 0;
	DWORD BytesSent = 0;
	int nTotalSendData = 0;
	WSABUF DataBuf;
	WSAOVERLAPPED Overlapped;
	bool SEND_UDP_G = true;
	TCHAR strInfo[1600];
	LARGE_INTEGER lPerSendStart, lPerSendEnd;
	LARGE_INTEGER lPerIntervalStart, lPerIntervalEnd;

	lPerSendStart.QuadPart = lPerSendEnd.QuadPart = 0;

	// create socket
	//if(!bStopUDP)
	//{
		if(SEND_UDP_G)
		{
			SecureZeroMemory((PVOID)&Overlapped, sizeof(WSAOVERLAPPED));

			Overlapped.hEvent = WSACreateEvent();
			if (Overlapped.hEvent == WSA_INVALID_EVENT)
			{
				_stprintf_s(strInfo, 500, (_T("WSACreateEvent failed with error: %d")), WSAGetLastError());
				LogErrorInfo(strInfo, pStream->m_nChannel_Index);
				return 0;
			}
		
			pStream->sockPrim = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
			if(!bStopUDP)
			{
				pStream->sockSec = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
				pStream->sockDest = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
			}
		}
		else
		{
			pStream->sockPrim = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if(!bStopUDP)
			{
				pStream->sockSec = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
				pStream->sockDest = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			}
		}
		
		char strNicDest[MAX_SIZE], strNicPrim[MAX_SIZE], strNicSec[MAX_SIZE];
		struct in_addr ip_adaptor, ipPrimAdaptor, ipSecAdaptor;
		char strIpDest[MAX_SIZE], strIpPrim[MAX_SIZE], strIpSec[MAX_SIZE];

		//primary socket
		if (pStream->sockPrim == SOCKET_ERROR)
		{
			const int BUF_SIZE = 300;
			TCHAR strError[BUF_SIZE];
			_stprintf_s(strError, BUF_SIZE, _T("socket creation failed for primary with error code : %d"), WSAGetLastError());
			LogErrorInfo(strError, pStream->m_nChannel_Index);
			goto Error_Label;
		}

		//primary adaptor
		wcstombs_s(&Temp, strNicPrim, (size_t)MAX_SIZE, pStream->m_Input[0].strNic_Source, (size_t)MAX_SIZE);
		ipPrimAdaptor.S_un.S_addr = inet_addr((const char *)strNicPrim);
		if (setsockopt(pStream->sockPrim, IPPROTO_IP, IP_MULTICAST_IF, (char *)&ipPrimAdaptor, sizeof(ipPrimAdaptor)) < 0)
		{
			TCHAR strError[BUF_SIZE];
			_stprintf_s(strError, BUF_SIZE, _T("primary setsocket failed with error code : %d"), WSAGetLastError());
			LogErrorInfo(strError, pStream->m_nChannel_Index);
			goto Error_Label;
		}

		// setup primary adress structure
		wcstombs_s(&Temp, strIpPrim,(size_t)MAX_SIZE, pStream->m_Input[0].strIp_Source, (size_t)MAX_SIZE);
		pStream->Prim_addr.sin_family = AF_INET;
		pStream->Prim_addr.sin_port = htons(pStream->m_Input[0].nPort_Source);
		pStream->Prim_addr.sin_addr.s_addr = inet_addr((const char *)strIpPrim);

		if(!bStopUDP)
		{
			//secondary socket
			if (pStream->sockSec == SOCKET_ERROR)
			{
				const int BUF_SIZE = 300;
				TCHAR strError[BUF_SIZE];
				_stprintf_s(strError, BUF_SIZE, _T("socket creation failed for secondary with error code : %d"), WSAGetLastError());
				LogErrorInfo(strError, pStream->m_nChannel_Index);
				goto Error_Label;
			}

			//secondary adaptor
			wcstombs_s(&Temp, strNicSec, (size_t)MAX_SIZE, pStream->m_Input[1].strNic_Source, (size_t)MAX_SIZE);
			ipSecAdaptor.S_un.S_addr = inet_addr((const char *)strNicSec);
			if (setsockopt(pStream->sockSec, IPPROTO_IP, IP_MULTICAST_IF, (char *)&ipSecAdaptor, sizeof(ipSecAdaptor)) < 0)
			{
				TCHAR strError[BUF_SIZE];
				_stprintf_s(strError, BUF_SIZE, _T("secondary setsocket failed with error code : %d"), WSAGetLastError());
				LogErrorInfo(strError, pStream->m_nChannel_Index);
				goto Error_Label;
			}

			// setup secondary adress structure
			wcstombs_s(&Temp, strIpSec,(size_t)MAX_SIZE, pStream->m_Input[1].strIp_Source, (size_t)MAX_SIZE);
			pStream->Sec_addr.sin_family = AF_INET;
			pStream->Sec_addr.sin_port = htons(pStream->m_Input[1].nPort_Source);
			pStream->Sec_addr.sin_addr.s_addr = inet_addr((const char *)strIpSec);

			//destination socket
			if (pStream->sockDest == SOCKET_ERROR)
			{
				const int BUF_SIZE = 300;
				TCHAR strError[BUF_SIZE];
				_stprintf_s(strError, BUF_SIZE, _T("socket creation failed for destination with error code : %d"), WSAGetLastError());
				LogErrorInfo(strError, pStream->m_nChannel_Index);
				goto Error_Label;
			}

			//destination adaptor
			wcstombs_s(&Temp, strNicDest, (size_t)MAX_SIZE, pOutput->strNic_Source, (size_t)MAX_SIZE);
			ip_adaptor.S_un.S_addr = inet_addr((const char *)strNicDest);
			if (setsockopt(pStream->sockDest, IPPROTO_IP, IP_MULTICAST_IF, (char *)&ip_adaptor, sizeof(ip_adaptor)) < 0)
			{
				TCHAR strError[BUF_SIZE];
				_stprintf_s(strError, BUF_SIZE, _T("destination setsocket failed with error code : %d"), WSAGetLastError());
				LogErrorInfo(strError, pStream->m_nChannel_Index);
				goto Error_Label;
			}

			// setup destination adress structure
			wcstombs_s(&Temp, strIpDest,(size_t)MAX_SIZE, pOutput->strIp_Source, (size_t)MAX_SIZE);
			pStream->Sender_addr.sin_family = AF_INET;
			pStream->Sender_addr.sin_port = htons(pOutput->nPort_Source);
			pStream->Sender_addr.sin_addr.s_addr = inet_addr((const char *)strIpDest);
		}
	//}

	LARGE_INTEGER qpc;
	QueryPerformanceFrequency(&qpc);

	LARGE_INTEGER lPerformanceCount,lPrevPerformanceCount;
	LARGE_INTEGER lPerformanceFreq;
	lPrevPerformanceCount.QuadPart = lPerformanceCount.QuadPart = 0;
	LARGE_INTEGER lWaitStart;
		
	double WaitAccumulate = 0;
	double diffWait = 0,lwaitadd= 0, prevWait = 0;
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

			//primary udp detection
			setsockopt(pStream->sockPrim, IPPROTO_IP, IP_MULTICAST_IF, (char *)&ipPrimAdaptor, sizeof(ipPrimAdaptor));
			if(connect(pStream->sockPrim, (sockaddr *)&pStream->Prim_addr, sizeof(pStream->Prim_addr)) < 0)
				pStream->m_bPrimUDPDetect = true;
			else
				pStream->m_bPrimUDPDetect = false;

			if(!bStopUDP)
			{
				//secondary udp detection
				setsockopt(pStream->sockSec, IPPROTO_IP, IP_MULTICAST_IF, (char *)&ipSecAdaptor, sizeof(ipSecAdaptor));
				if(connect(pStream->sockSec, (sockaddr *)&pStream->Sec_addr, sizeof(pStream->Sec_addr)) < 0)
					pStream->m_bSecUDPDetect = true;
				else
					pStream->m_bSecUDPDetect = false;
			}
									
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
							_stprintf(Offset, _T(" \nCurrent Offset = %d, W = %d, R = %d, Send_StartPoint = %d\n"), pOutput->m_nSendOffset, pCInput->m_nIndex_Write,  pOutput->m_nIndex_Read, pOutput->m_nSend_StartPoint);
							OutputDebugString(Offset);

							if(pOutput->m_nSendOffset < 0)
							{
								pOutput->m_nSendOffset = pOutput->m_nSend_NextPoint - pOutput->m_nSend_StartPoint + MAX_PACKET_SIZE;

								TCHAR Offset[100];
								_stprintf(Offset, _T("\nAFTER LOOP Current Offset = %d, W = %d, R = %d, Send_StartPoint = %d\n"), pOutput->m_nSendOffset, pCInput->m_nIndex_Write, pOutput->m_nIndex_Read, pOutput->m_nSend_StartPoint);
								OutputDebugString(Offset);
							}

							//PCR_VALUE assignment
							pOutput->m_nPrevPCR_value = pOutput->m_nCurrPCR_value;
							pOutput->m_nCurrPCR_value = pCInput->m_nPcr_TempValue;

							if (SEND_UDP_G)
							{
								if(lPerSendStart.QuadPart != 0)
								{
									QueryPerformanceCounter(&lPerSendEnd);								
									double nPrevSendTime = (double)(lPerSendEnd.QuadPart - lPerSendStart.QuadPart) * 1000 / lPerformanceFreq.QuadPart;

									lwaitadd = prevWait - nPrevSendTime;
								}
								prevWait = pOutput->m_nTimeDiff + lwaitadd;

								nInterval = double(pOutput->m_nTimeDiff + lwaitadd) / pOutput->m_nSendOffset;
								nTotalInterval = 0, nTimeToSend = 0, nTotalTimeToSend = 0;
								nTotalSendData = 0;
								
								QueryPerformanceCounter(&lPerIntervalStart);								

								lPerSendStart = lPerIntervalStart;
							}

							// Send routine
							for(int i = 0; i < pOutput->m_nSendOffset; i++)
							{		
								try
								{
									if(!bStopUDP)
									{
										if (SEND_UDP_G)
										{
											DataBuf.len = 1316;
											DataBuf.buf = (char*)pCInput->m_SourceBuffer[pOutput->m_nSend_StartPoint];//TransmitBytes;
											BytesSent = 0;
												 
											nTotalSendData = WSASendTo(pStream->sockDest, &DataBuf, 1, &BytesSent, Flags, (SOCKADDR *)& pStream->Sender_addr, sizeof(pStream->Sender_addr), &Overlapped, NULL);
											//memcpy(UDP_Data.buf + i * 1316, &TxDataBuf[ReadBuffer_j][ReadBuffer_k], 1316);

											if (nTotalSendData == SOCKET_ERROR)
											{
												DWORD dwResult = WSAGetLastError();
												if (dwResult != WSA_IO_PENDING)
												{
													TCHAR strInfo[800];
													_stprintf_s(strInfo, 800, _T("------- ****** --- Transmission has error ------- Error = %ld"), dwResult);
													LogErrorInfo(strInfo, pStream->m_nChannel_Index);

													pOutput->m_bSendTo_Status = false;
												}
												else
													pOutput->m_bSendTo_Status = true;
											}
											else
											{
												pOutput->m_bSendTo_Status = true;
											}

											//if(mux_settings.VBR_multiplex == 0)
											{
												QueryPerformanceCounter(&lPerIntervalEnd);

												nTimeToSend = (double)(lPerIntervalEnd.QuadPart - lPerIntervalStart.QuadPart) * 1000 / lPerformanceFreq.QuadPart;

												lPerIntervalStart = lPerIntervalEnd;

												nTotalInterval += nInterval;
												nTotalTimeToSend += nTimeToSend;

												double ActualTime = double(nTotalInterval - nTotalTimeToSend);

												if (ActualTime > 1.0)
												{
													QueryPerformanceCounter(&lPerIntervalStart);

													Sleep(1);

													/*while(CanInitHandle)
													{
														QueryPerformanceCounter(&lPerIntervalEnd);
														diffWait = (double)(lPerIntervalEnd.QuadPart - lPerIntervalStart.QuadPart)*1000/lPerformanceFreq.QuadPart;
														if(diffWait >= ActualTime)
															break;
														if(WaitForSingleObject(hStop, 0) != WAIT_TIMEOUT)
															break;
													}*/

													QueryPerformanceCounter(&lPerIntervalEnd);
													diffWait = (double)(lPerIntervalEnd.QuadPart - lPerIntervalStart.QuadPart) * 1000 / lPerformanceFreq.QuadPart;
													nTotalInterval = diffWait - ActualTime;
													nTotalTimeToSend = 0;

													//QueryPerformanceCounter(&lPerIntervalStart);
													lPerIntervalStart = lPerIntervalEnd;
												}
											}
										}
										else
										{
											if(sendto(pStream->sockDest, (const char*)pCInput->m_SourceBuffer[pOutput->m_nSend_StartPoint], 1316, 0, (sockaddr *)&pStream->Sender_addr, sizeof(pStream->Sender_addr)) < 0)
											{
												pOutput->m_bSendTo_Status = false;

												TCHAR strError[ERRORBUF_SIZE];
												_stprintf_s(strError,ERRORBUF_SIZE, _T("sentto() failed with error code Permenantly : %d") , WSAGetLastError());
												LogErrorInfo(strError, pStream->m_nChannel_Index);
											}
											else
											{
												pOutput->m_bSendTo_Status = true;
												//fwrite(pCInput->m_SourceBuffer[pOutput->m_nSend_StartPoint],sizeof(unsigned char),1316,fp);
											}
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
									
									TCHAR strMsgoffset[1000];
									_stprintf(strMsgoffset, _T("\n ####### UDP Packet offset = %d ||||| PCR_Value Difference = %.02f\n"), pOutput->m_nSendOffset, pOutput->m_nTimeDiff);
									OutputDebugString(strMsgoffset);

									if(pOutput->m_nTimeDiff > 40 && pOutput->m_nTimeDiff < 1000)
									{
										_stprintf(strMsgoffset, _T("\n **** Send Packet GAP = %d ||||| PCR_GAP = %.02f\n"), pOutput->m_nSendOffset, pOutput->m_nTimeDiff);
										OutputDebugString(strMsgoffset);
									}

									//if(pOutput->m_nTimeDiff > 1000 || pOutput->m_nTimeDiff < 0)
									if(pOutput->m_nTimeDiff > 40 || pOutput->m_nTimeDiff < 0)
									{
										TCHAR strInfo[200];
										_stprintf(strInfo, _T("\n Error:  m_Output.m_Output.m_nTimeDiff = %.4f ---and UDP PAckege = %d\n"),pOutput->m_nTimeDiff,pOutput->m_nSendOffset);
										OutputDebugString(strInfo);
										pOutput->m_nTimeDiff = 40;
									}

									if(SEND_UDP_G)
									{
										QueryPerformanceCounter(&lPerformanceCount);

										lPerSendEnd = lPerformanceCount;
							
										WaitAccumulate = (double)(lPerformanceCount.QuadPart - lPrevPerformanceCount.QuadPart)*1000/lPerformanceFreq.QuadPart;

										if(WaitAccumulate < (pOutput->m_nTimeDiff + lwaitadd))
										{
											double nTotalWait = pOutput->m_nTimeDiff + lwaitadd - WaitAccumulate;
											if(nTotalWait > 0)
											{
												QueryPerformanceCounter(&lWaitStart);

												if(nTotalWait > 1)
													::Sleep((int)nTotalWait);

												while(!pOutput->m_bThread_Terminate)
												{
													QueryPerformanceCounter(&lPerformanceCount);
													diffWait = (double)(lPerformanceCount.QuadPart - lWaitStart.QuadPart)*1000/lPerformanceFreq.QuadPart;
													if((diffWait + WaitAccumulate) >= (double)(pOutput->m_nTimeDiff + lwaitadd))
														break;										
												}
											}
										}

										QueryPerformanceCounter(&lPrevPerformanceCount);
									}
									else
									{
										QueryPerformanceCounter(&lPerformanceCount);
										WaitAccumulate = (double)(lPerformanceCount.QuadPart - lPrevPerformanceCount.QuadPart)*1000/lPerformanceFreq.QuadPart;
										if(WaitAccumulate >= (double)(pOutput->m_nTimeDiff + lwaitadd))
										{

										}
										else // wait loop
										{
											double nTotalWait = pOutput->m_nTimeDiff + lwaitadd - WaitAccumulate;
											QueryPerformanceCounter(&lWaitStart);

											if(nTotalWait > 1)
												Sleep(nTotalWait);

											/*if(nTotalWait >= pOutput->m_nTimeDiff - 3)//10)//15)//25)
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
											}*/

											//QueryPerformanceCounter(&lPerformanceCount);
											//diffWait = (double)(lPerformanceCount.QuadPart - lWaitStart.QuadPart)*1000/lPerformanceFreq.QuadPart;
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

					TCHAR strInfo[200];
					_stprintf(strInfo, _T("Sender() - Diff = %d -- W = %d -- R = %d\n"), nCurrentIndex_diff, pCInput->m_nIndex_Write, pOutput->m_nIndex_Read);
					//OutputDebugString(strInfo);
				}
				else
				{
					TCHAR strInfo[200];
					_stprintf(strInfo, _T("Sender() - nCurrentIndex_diff < 0 - Diff = %d -- W = %d -- R = %d\n"), nCurrentIndex_diff, pCInput->m_nIndex_Write, pOutput->m_nIndex_Read);
					//OutputDebugString(strInfo);

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
					Sleep(1);					
				}
			}
			else
			{
				if(pCInput->m_bVideoHealth == true || pCInput->m_bPCRPID_Found == false)
				{
					TCHAR strInfo[200];
					_stprintf(strInfo, _T("Sender() - pCInput->m_bVideoHealth = %d -- pCInput->m_bPCRPID_Found = %d\n"), pCInput->m_bVideoHealth, pCInput->m_bPCRPID_Found);
					OutputDebugString(strInfo);
				}	
				Sleep(40);
			}
		}
		else
		{
			pStream->IntialDecisionMaking();
		}
		//Sleep(3000);
	}
	Error_Label:

	if (SEND_UDP_G)
	{
		if (Overlapped.hEvent != WSA_INVALID_EVENT)
			WSACloseEvent(Overlapped.hEvent);
	}

	if(pOutput->m_bThread_Terminate == true)
	{
		_stprintf_s(strStartStopBuf, STARTSTOPBUF_SIZE, _T("STOP SENDER THREAD---->Of Stream %d \r\n"), pStream->m_nChannel_Index);
		LogStartStopEvents(strStartStopBuf, pStream->m_nChannel_Index);

		closesocket(pStream->sockPrim);
		if(!bStopUDP)
		{
			closesocket(pStream->sockSec);
			closesocket(pStream->sockDest);
		}
	}
	return 0;
}
DWORD WINAPI scannar(LPVOID lpParameter)
{
	int nTempRead = 0;
	CInput *pInput = (CInput*)lpParameter;

	if((pInput->m_nTotalProgNo == 1) && (pInput->m_bEmergencyActive == 1))
		pInput->m_bReset = true;
		
	if(!pInput->m_bReset)
	{
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
		for(int i = 0; i < 5; i++)
		{
			pInput->m_nAudioPid[i] = 0;
		}
	}

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
				Sleep(1);
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
		TCHAR folderpath[MAX_PATH];

		if(streamId == -1)
			_stprintf_s(folderpath, MAX_PATH, _T("%s\\ErrorLogs"), GetFolderPath());
		else
			_stprintf_s(folderpath, MAX_PATH, _T("%s\\StreamLogs_%d"), GetFolderPath(_T("StreamLogs")), streamId + 1);

		int retval;
		retval = PathFileExists(folderpath);
		if(retval == 0)
		{
			CreateDirectory(folderpath ,NULL);
		}

		SYSTEMTIME lt;
		GetLocalTime(&lt);

		TCHAR strFile[MAX_PATH];
		if(streamId == -1)
			_stprintf_s(strFile, MAX_PATH, _T("%s\\Exception_Error_%04d-%02d-%02d.log"), folderpath, lt.wYear, lt.wMonth, lt.wDay);
		else
			_stprintf_s(strFile, MAX_PATH, _T("%s\\Stream_%d_Exception_Error_%04d-%02d-%02d.log"), folderpath, streamId + 1, lt.wYear, lt.wMonth, lt.wDay);


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
				_stprintf_s(strNewFileName, MAX_PATH, _T("%s\\Exception_Error_%04d-%02d-%02d_Part_%02d-%02d-%02d.log"), folderpath, lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond);
			else
				_stprintf_s(strNewFileName, MAX_PATH, _T("%s\\Stream_%d_Exception_Error_%04d-%02d-%02d_Part_%02d-%02d-%02d.log"), folderpath, streamId + 1, lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond);

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
		TCHAR folderpath[MAX_PATH];

		if(streamId == -1)
			_stprintf_s(folderpath, MAX_PATH, _T("%s\\ErrorLogs"), GetFolderPath());
		else
			_stprintf_s(folderpath, MAX_PATH, _T("%s\\StreamLogs_%d"), GetFolderPath(_T("StreamLogs")), streamId + 1);

		int retval;
		retval = PathFileExists(folderpath);
		if(retval == 0)
		{
			CreateDirectory(folderpath ,NULL);
		}

		SYSTEMTIME lt;
		GetLocalTime(&lt);
		if(streamId == -1)
			_stprintf_s(strFile, MAX_PATH, _T("%s\\Stream_XX_Error_Info_%04d-%02d-%02d-%02d.log"), folderpath, lt.wYear, lt.wMonth, lt.wDay, lt.wHour);
		else
			_stprintf_s(strFile, MAX_PATH, _T("%s\\Stream_%d_Error_Info_%04d-%02d-%02d-%02d.log"), folderpath, streamId + 1, lt.wYear, lt.wMonth, lt.wDay, lt.wHour);		

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
				_stprintf_s(strNewFileName, MAX_PATH, _T("%s\\Error_Info_%04d-%02d-%02d-%02d_Part_%02d-%02d-%03d.log"), folderpath, lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);
			else
				_stprintf_s(strNewFileName, MAX_PATH, _T("%s\\Stream_%d_Error_Info_%04d-%02d-%02d-%02d_Part_%02d-%02d-%03d.log"), folderpath, streamId + 1, lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);
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
		TCHAR folderpath[MAX_PATH];
		_stprintf_s(folderpath, MAX_PATH, _T("%s\\StreamLogs_%d"), GetFolderPath(_T("StreamLogs")), streamId + 1);
		int retval;
		retval = PathFileExists(folderpath);
		if(retval == 0)
		{
			CreateDirectory(folderpath ,NULL);
		}

		SYSTEMTIME lt;
		GetLocalTime(&lt);

		TCHAR strFile[MAX_PATH];
		_stprintf_s(strFile, MAX_PATH, _T("%s\\Stream_%d_StartStop_%04d-%02d-%02d.log"), folderpath, streamId + 1, lt.wYear, lt.wMonth, lt.wDay);

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

void LogCCError(TCHAR* strMsg, int streamId)
{
	try
	{
		bool bFileTooBig = false;
		bool bFileOpen = false;

		CFile CCErrorLogFile;
		CFileException excCCErrorLogFile;

		SYSTEMTIME lt;
		GetLocalTime(&lt);

		TCHAR folderpath[MAX_PATH];
		CString strFilePath;
		_stprintf_s(folderpath, MAX_PATH, _T("%s\\CCErrorLog_%d_%s"), GetFolderPath(_T("CCErrorLog")), streamId + 1, StreamWatch[streamId].m_strCstreamChannel_Name);
		int retval;
		retval = PathFileExists(folderpath);
		if(retval == 0)
		{
			CreateDirectory(folderpath ,NULL);
		}

		strFilePath.Format( _T("%s\\CCErrorLog_%d_%s_%04d-%02d-%02d.log"), folderpath, streamId + 1, StreamWatch[streamId].m_strCstreamChannel_Name, lt.wYear, lt.wMonth, lt.wDay);

		if(CCErrorLogFile.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeBinary, &excCCErrorLogFile))
		{	
			if(CCErrorLogFile.GetLength() > 10 * 1024 * 1024)//More Than 10 MB
			{
				bFileTooBig = true;
				bFileOpen = false;
				CCErrorLogFile.Close();
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
			strNewFilePath.Format( _T("%s\\CCErrorLog_%d_%s_%04d-%02d-%02d_Part_%02d-%02d-%02d.log"), folderpath, streamId + 1, StreamWatch[streamId].m_strCstreamChannel_Name, lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond);

			try
			{
				CFile::Rename(strFilePath, strNewFilePath);
			}
			catch(...)
			{
			}
			if(CCErrorLogFile.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeBinary, &excCCErrorLogFile))
				bFileOpen = true;
		}

		if(!bFileTooBig && bFileOpen)
		{
			CCErrorLogFile.SeekToEnd();
			CCErrorLogFile.Write(strMsg, _tcslen(strMsg)*sizeof(TCHAR));
			CCErrorLogFile.Close();
		}
	}
	catch(...)
	{
	}
}


void LogCCErrorSummary(TCHAR* strMsg)
{
	try
	{
		bool bFileTooBig = false;
		bool bFileOpen = false;

		SYSTEMTIME lt;
		GetLocalTime(&lt);

		CFile CCErrorSummaryFile;
		CFileException excCCErrorSummaryFile;

		CString strFilePath;
		/*TCHAR folderpath[MAX_PATH];
		_stprintf_s(folderpath, MAX_PATH, _T("%s\\CCErrorResetLog"), GetFolderPath());

		int retval;
		retval = PathFileExists(folderpath);
		if(retval == 0)
		{
			CreateDirectory(folderpath ,NULL);
		}*/

		strFilePath.Format( _T("%s\\CCErrorSummary.log"), GetFolderPath());

		//TCHAR strError[1000];
		//_stprintf_s(strError, 1000, _T("[%04d-%02d-%02d %02d:%02d:%02d:%03d] - Channel%d_CCError is reset\r\n"), lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds, nChannelNumber + 1);
		if(CCErrorSummaryFile.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeBinary, &excCCErrorSummaryFile))
		{	
			if(CCErrorSummaryFile.GetLength() > 10 * 1024 * 1024)//More Than 10 MB
			{
				bFileTooBig = true;
				bFileOpen = false;
				CCErrorSummaryFile.Close();
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
			strNewFilePath.Format( _T("%s\\CCErrorSummary_Part_%d.log"), GetFolderPath(), m_CCErrorSummaryFile++);

			try
			{
				CFile::Rename(strFilePath, strNewFilePath);
			}
			catch(...)
			{
			}
			if(CCErrorSummaryFile.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeBinary, &excCCErrorSummaryFile))
				bFileOpen = true;
		}

		if(!bFileTooBig && bFileOpen)
		{
			CCErrorSummaryFile.SeekToEnd();
			CCErrorSummaryFile.Write(strMsg, _tcslen(strMsg)*sizeof(TCHAR));
			CCErrorSummaryFile.Close();
		}
	}
	catch(...)
	{
	}
}

void LogAlertMail(TCHAR* strMsg)
{
	try
	{
		bool bFileTooBig = false;
		bool bFileOpen = false;

		CFile AlertMailFile;
		CFileException excAlertMailFile;

		SYSTEMTIME lt;
		GetLocalTime(&lt);

		TCHAR folderpath[MAX_PATH];
		CString strFilePath;
		_stprintf_s(folderpath, MAX_PATH, _T("%s\\AlertMail"), GetFolderPath());
		int retval;
		retval = PathFileExists(folderpath);
		if(retval == 0)
		{
			CreateDirectory(folderpath ,NULL);
		}

		strFilePath.Format( _T("%s\\AlertMail_%04d-%02d-%02d.log"), folderpath, lt.wYear, lt.wMonth, lt.wDay);

		if(AlertMailFile.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeBinary, &excAlertMailFile))
		{	
			if(AlertMailFile.GetLength() > 10 * 1024 * 1024)//More Than 10 MB
			{
				bFileTooBig = true;
				bFileOpen = false;
				AlertMailFile.Close();
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
			strNewFilePath.Format( _T("%s\\AlertMail_%04d-%02d-%02d_Part_%02d-%02d-%02d.log"), folderpath, lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond);

			try
			{
				CFile::Rename(strFilePath, strNewFilePath);
			}
			catch(...)
			{
			}
			if(AlertMailFile.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeBinary, &excAlertMailFile))
				bFileOpen = true;
		}

		if(!bFileTooBig && bFileOpen)
		{
			const int BUF_LENGTH = 500;
			TCHAR strPrintMsg[BUF_LENGTH];

			_stprintf_s(strPrintMsg, BUF_LENGTH, _T("[%04d-%02d-%02d %02d:%02d:%02d:%03d] - %s\r\n"), lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds, strMsg);

			AlertMailFile.SeekToEnd();
			AlertMailFile.Write(strPrintMsg, _tcslen(strPrintMsg)*sizeof(TCHAR));
			AlertMailFile.Close();
		}
	}
	catch(...)
	{
	}
}

CString GetFolderPath(TCHAR* strPath)
{
	TCHAR Buffer[MAX_PATH];
	DWORD dwRet;
	dwRet = GetCurrentDirectory(MAX_PATH, Buffer);

	CString folderpath;
	folderpath.Format(_T("%s\\LOGS"), Buffer);

	int retval;
	retval = PathFileExists(folderpath);
	if(retval == 0)
	{
		CreateDirectory(folderpath ,NULL);
	}

	if(_tcscmp(strPath, _T("")) != 0)
	{
		CString subfolderpath;
		subfolderpath.Format(_T("%s\\%s"), folderpath, strPath);
		retval = PathFileExists(subfolderpath);
		if(retval == 0)
		{
			CreateDirectory(subfolderpath ,NULL);
		}
		return subfolderpath;
	}

	return folderpath;
	
}