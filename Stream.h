#pragma once

#include "stdafx.h"
#include "net_client_engine.h"
#include "InputStream.h"
//#include "Tab_Global.h"

typedef struct Output
{
	TCHAR strNic_Source[30];
	TCHAR strIp_Source[30];
	int nPort_Source;

	unsigned int m_nIndex_Read;
	bool m_bIndex_Diff;
	bool m_bThread_Terminate;
	__int64 m_nPcr_Check;
	__int64 m_nPrevPCR_value;
	__int64 m_nCurrPCR_value;
	int m_nSend_StartPoint;
	int m_nSend_NextPoint;
	int m_nSendOffset;
	bool m_bCountEqualIndex;
	DWORD m_dwFisrtEqualTime;

	
	double m_nTimeDiff;
	bool m_bSendTo_Status;

	Output()
	{
		m_nIndex_Read = 0;

		m_bIndex_Diff = false;
		m_bThread_Terminate = false;

		m_nPcr_Check = 0;
		m_nPrevPCR_value = 0;
		m_nCurrPCR_value = 0;
		m_nSend_StartPoint = 0;
		m_nSend_NextPoint = 0;
		m_nSendOffset = 0;
		
		m_nTimeDiff = 40;

		m_bSendTo_Status = false;
		m_bCountEqualIndex = false;
		m_dwFisrtEqualTime = 0;
	}
}
OUTPUT;

class CStream
{
public:
	CStream();
	CInput m_Input[MAX_RECEIVER];
	OUTPUT m_Output;

	struct sockaddr_in Sender_addr;
	struct sockaddr_in Prim_addr;
	struct sockaddr_in Sec_addr;

	bool m_bUpdateGapFile;
	CTime m_FileCreateTime;
	DWORD m_LastModifiedTime;
	DWORD m_LastReadedTime;
	DWORD m_ModifiedFileTime; 
	TCHAR m_strFPathVideopara[200];
	
	CNetLiveClientEngine *pEngine[MAX_RECEIVER];
	bool m_nCheck_EngineStatus;

	unsigned char m_ScanPacket_Buf[2][188]; //Logic will change
		//For Mpts Stream
	
	//Channel Info
	int m_nChannel_Index;
	int m_nCheckBox_Status;
	int m_bManualCheckStatus;
	int m_ScheduleManualSwitching;
	bool m_bValidation_Status;
	TCHAR m_strCstreamChannel_Name[20];
	DWORD m_dwStartClickTime;
	bool m_bValidation_Error;
		
	//switching logic variable
	DWORD m_dwFirstAvalableInput;
	DWORD m_dwGoodPrimeDetector;
	int m_nGoodPrimeCompair;
	int m_bStartBackSwitch;
	int m_nCurrDuration;
	int m_nDetection_Time;
	int m_nPreDetection_Time;
	int m_nSwitch_Input;
	
	DWORD SenderThreadID;
	HANDLE SenderHandle;
	SOCKET sockDest;
	SOCKET sockPrim;
	SOCKET sockSec;
	bool m_bPrimUDPDetect;
	bool m_bSecUDPDetect;

	bool m_bStart;
	int m_nSelected_PrgNo;
	CString m_strSelected_PrgNo;
	TCHAR m_strPrgNoList[1000];
	bool m_bSelected_PrgNo;
	int m_nTotal_PrgNo;
	CString m_strPrgNo_List[50];

	bool m_bCCVideo;
	bool m_bCCAudio[5];
	int m_lastCCVideo;
	int m_lastCCAudio[5];
	unsigned __int64 m_CCError;
	CString strCCError;
	unsigned __int64 m_lastCCError;
	unsigned __int64 m_ModifiedCCError;
	CEdit m_strCCError;
	bool m_bCCError;
	int m_CCErrorResetFile;

	//functions
	void CleanUp_Engine(int i);
	void CLEANUP_ALL();
	int InitEngine(int i);
	int InitSession();
	int PlaybackLoop();

	void Start();
	bool Stop();
	void SetIndex(int);
	void readVideoAnalysisInfo(int ChannelIndex);
	void DecisionMaking();
	void ScheduleSwitchingCheck();
	CString GetAppPath(HMODULE hModule);
	void IntialDecisionMaking();
	void ResetCCError(int nChannelNumber);
};