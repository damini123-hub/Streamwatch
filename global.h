#pragma once

#define MAX_SIZE 100
#define MAX_PACKET_SIZE 10000
#define MAX_RECEIVER 2
#define MAX_STREAM 48

//#define USE_TS_TRANSMIT

#include "stdafx.h"

#ifdef USE_TS_TRANSMIT
#include "TS_transmit.h"
#endif

extern int TotalCheckedBox;
extern bool bClickeStartAll;
extern bool bXmlFileStatus;
extern int m_nLiecensedStream;
extern bool bStopUDP;
extern bool bStartVideoAnalysis;
extern bool bEMERGENCY;
extern bool bPrimUDPDetection;
extern bool bSecUDPDetection;

// methods
DWORD WINAPI sender(LPVOID lpParameter);
DWORD WINAPI scannar(LPVOID lpParameter);
void LogException(TCHAR* strMsg, int StreamId);
void LogErrorInfo(TCHAR* strMsg, int StreamId);
void LogStartStopEvents(TCHAR* strMsg , int StreamId);
void LogDebugInfo(TCHAR* strMsg , int StreamId);
void Check_MPTS_Stream(int StreamId);
bool IsProcessRunning(const wchar_t *processName);
void LogCCError(TCHAR* strMsg, int streamId);
void LogCCErrorSummary(TCHAR* strMsg);
void LogAlertMail(TCHAR* strMsg);
CString GetFolderPath(TCHAR* strPath = _T(""));