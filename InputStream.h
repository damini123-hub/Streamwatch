#pragma once

#include "stdafx.h"

extern CStream StreamWatch[MAX_STREAM];

class CInput
{
public:
	CInput();

	CInput *pPrimary;

	bool m_bAutoStopPrimary;

	int m_nCurRead;
	bool m_bReset;
	bool m_bNULLPid;

	bool m_NullPacketStream;
	bool m_EmptyPacketStream;
	bool m_VideoPacketMissing;
	bool m_PCR_PIDPacketMissing;

	bool m_bPrimaryInput;
	int m_bEmergencyActive;
	unsigned char m_VideoType;
	unsigned char m_AudioType[5];
	int m_nSwitchIndicator;

	unsigned char m_SourceBuffer[MAX_PACKET_SIZE][1316]; //hard
	unsigned char m_AnalysPacket_Buf[188];
	unsigned char m_ReplaceBuffer[188];
	unsigned char m_PCRSender_Buf[188];

	unsigned int m_nIndex_Scan;
	bool m_bScanStartDiff;

	unsigned int m_nIndex_Write;
	int m_nCurrent_Time;

	int m_nDetect_Time;
	int m_nChannelNumber;

	bool m_bMPTS_Stream; 
	bool m_bPCR_PIDNotFound;

	bool m_bScannar_Terminate;

	TCHAR strNic_Source[30]; // adaptor add
	TCHAR strIp_Source[30]; // ip add
	int nPort_Source; // port No
	bool bStatus_Stream; // If stream is avilable or not

	int m_nPcrPid;
	int m_nVideoPid;
	int m_nAudioPid[5];
	int m_nTotalAudio;
	int m_nFound_PMT_PID;
	int m_nFound_PCR_PID; 
	int m_nProgramNo_Buff[50]; 
	int m_nPMT_PIDBuff[50];
	int m_nTotalProgNo;
	int m_nProgram_Num;
	bool m_bPAT_Found;
	bool m_bPMT_Found; 
	bool m_bPCRPID_Found;
	bool m_bPCRValue_Found;
	bool m_bVideoPidFound;
	bool m_bnOVedioPid;

	int m_nVideoGap; // Max_Video Packet Gap valid file
	int m_nValidVideoCount; // Round for checking invalid Gap file
	int m_nInvalidCount; // Round for checking valid Gap file
	int m_nTotalPackets; // Total Packet Count
	int m_nVideoPackets; // Total Video Packet
	int m_nVideoPacketGap; //Video packet gap variable
	int m_nLastVidPacketIndex; // last video packet on packet index
	int m_nErrorRound; //Invalide Round Counter
	int m_nErrorFreeRound;//Valid Round Counter
	bool m_bVideoHealth; // Input Heath falg
	int m_nRound; // Total packet count round
	int m_nRoundVid; // Total Video packet count round
	DWORD m_dwLastVed_PidArrived;
	DWORD m_dwLastPCR_PIDArrived;

	bool m_bAlertStatus;

	DWORD m_dwFisrtUpdateTime;
	DWORD m_dwLastUpdateTime;

	int m_nPCR_ValueIndex[20];
	__int64 m_nPcr_TempValue;
	int m_nPCR_Index;
	int m_nPCR_ReadIndex;
	bool m_bstartSending;
	__int64 m_nPCR_ValueBuf[20];
	
	// 
	DWORD ScannarThreadID;
	HANDLE ScannarHandle;
	//function
	void Indentify_Scan_Input();
	void Scan_Replacement_Input();
	int Find_PID(unsigned char b1,unsigned char b2);
	int Parse_Packet(unsigned char inBuf_Pack[]);
	int Find_PCR_PID(unsigned char inBuf_Pack[],int nPMT_PId);
	int Parse_PMT(unsigned char inBuf_Pack[]);
	int Find_Length(unsigned char b1,unsigned char b2);
	bool Scan_Packet(unsigned char inBuf_Pack[]);
	bool Scan_VedPid(unsigned char inBuf_Pack[], int nVed_Pid, int *nAud_Pid, int nTotalAudio);
	unsigned __int64 Find_PCR_Value(unsigned char inBuf_Pack[], int nPCR_PId);
	void Change_StreamInfo(unsigned char inBuf_Pack[]);
	void Replace_Pid(int Pid, int oneByte, int SecondByte);
	void Replace_VideoPacket();
	void Replace_AudioPacket(int i);
	void Replace_PATPacket();
	void Replace_PMTPacket();
	void Replace_PCRPacket();
	void Replace_WithNullPacket();
	uint32_t CalculateCRC( BYTE *p_begin, int i_count );
	CString Switch_Reason();
};