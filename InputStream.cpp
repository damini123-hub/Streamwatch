#include "stdafx.h"
#include "InputStream.h"

//bool bNULLPid = false;
   
CInput::CInput()
{
	m_bAutoStopPrimary = false;

	m_bReset = false;
	m_bNULLPid = false;
	m_nCurRead = 0;
	m_NullPacketStream = false;
	m_EmptyPacketStream = false;
	m_VideoPacketMissing = false;
	m_PCR_PIDPacketMissing =false;

	m_bPrimaryInput = false;
	m_bEmergencyActive = 0;
	m_nSwitchIndicator = 0;

	m_nIndex_Write = 0;
	m_nCurrent_Time = 0;

	m_nIndex_Scan = 0;
	m_bScanStartDiff = false;

	m_nDetect_Time = 0;
	m_nChannelNumber = 0;

	m_bMPTS_Stream = false;
	m_bPCR_PIDNotFound = false;

	m_bScannar_Terminate = false;

	nPort_Source = 0;
	bStatus_Stream = false;

	m_nVideoPid = 0;
	for(int i = 0; i < 5; i++)
	{
		m_nAudioPid[i] = 0;
	}
	m_nTotalAudio = 0;
	m_nFound_PMT_PID = 0;
	m_nFound_PCR_PID = 0;
	m_nTotalProgNo = 0;
	m_nProgram_Num = 0;
	m_bPAT_Found = false;
	m_bPMT_Found = false; 
	m_bPCRPID_Found = false;
	m_bPCRValue_Found = false;
	m_bVideoPidFound = false;
	m_bnOVedioPid = false;

	m_nVideoGap = 0;
	m_nValidVideoCount = 0;
	m_nInvalidCount = 0;
	m_nTotalPackets = 0;
	m_nVideoPackets = 0;
	m_nVideoPacketGap = 0;
	m_nLastVidPacketIndex = 0;
	m_nErrorRound = 0;
	m_nErrorFreeRound = 0;
	m_bVideoHealth = false;
	m_nRound = 0;
	m_nRoundVid = 0;
	m_dwLastVed_PidArrived = 0;
	m_dwLastPCR_PIDArrived = 0;

	m_bAlertStatus = false;
	m_dwFisrtUpdateTime = 0;
	m_dwLastUpdateTime = 0;

	for(int i = 0; i < 20; i++)
	{
		m_nPCR_ValueIndex[i] = 0;
	}
	m_nPcr_TempValue = 0;
	m_nPCR_Index = 0;
	m_nPCR_ReadIndex = 0;
	m_bstartSending = false;
	
	ScannarThreadID = 0;
	ScannarHandle = INVALID_HANDLE_VALUE;
	
}

int CInput::Find_PID(unsigned char b1,unsigned char b2)
{
	int pid_Int = 0;

	unsigned char pid_new = b1 & 0x1F;
	pid_Int = pid_new;
	pid_Int = pid_Int << 8;
	pid_Int = pid_Int | b2;

	return pid_Int;
}
int CInput::Find_Length(unsigned char b1,unsigned char b2)
{
	int nLength_Int = 0;
	
	unsigned char pid_new = b1 & 0x0F;
	nLength_Int = pid_new;
	nLength_Int = nLength_Int << 8;
	nLength_Int = nLength_Int | b2;

	return nLength_Int;
}
int CInput::Parse_PMT(unsigned char inBuf_Pack[])
{
	int nVedioPid = 0;
	int nSectionLen = 0;
	int nPrgInfoLen = 0;
	int nEs_InfoLen = 0;
	int nIndex = 6;
	int nstartPoint = 0;

	nstartPoint = inBuf_Pack[4];
	if(nstartPoint != 0)
		nstartPoint++;

	nSectionLen = Find_Length(inBuf_Pack[nstartPoint + nIndex],inBuf_Pack[nstartPoint + nIndex + 1]);

	nIndex += 9;
 	nPrgInfoLen = Find_Length(inBuf_Pack[nstartPoint + nIndex],inBuf_Pack[nstartPoint + nIndex + 1]);
	nIndex = nIndex + nPrgInfoLen + 2;
	while(nIndex < (nSectionLen + 4))
	{
		if(inBuf_Pack[nstartPoint + nIndex] == 0x02 || inBuf_Pack[nstartPoint + nIndex] == 0x1b || inBuf_Pack[nstartPoint + nIndex] == 0x80 || inBuf_Pack[nstartPoint + nIndex] == 0x24)
		{
			m_VideoType = inBuf_Pack[nstartPoint + nIndex];
			nVedioPid = Find_PID(inBuf_Pack[nstartPoint + nIndex + 1], inBuf_Pack[nstartPoint + nIndex + 2]); 
			m_bVideoPidFound = true;

		}
		if(inBuf_Pack[nstartPoint + nstartPoint + nIndex] == 0x04 || inBuf_Pack[nstartPoint + nstartPoint + nIndex] == 0x81 || inBuf_Pack[nstartPoint + nstartPoint + nIndex] == 0x03 || inBuf_Pack[nstartPoint + nstartPoint + nIndex] == 0x11 || inBuf_Pack[nstartPoint + nstartPoint + nIndex] == 0x00)
		{
			m_AudioType[m_nTotalAudio] = inBuf_Pack[nstartPoint + nIndex];
			m_nAudioPid[m_nTotalAudio] = Find_PID(inBuf_Pack[nstartPoint + nIndex + 1], inBuf_Pack[nstartPoint + nIndex + 2]);
			m_nTotalAudio++;
		}
		nIndex += 3;
		nEs_InfoLen = Find_Length(inBuf_Pack[nstartPoint + nIndex], inBuf_Pack[nstartPoint + nIndex + 1]);
		nIndex = nIndex + nEs_InfoLen + 2;
	}
	
	if(!m_bVideoPidFound)
	{
		TCHAR strTemp[100];
		_stprintf(strTemp, _T("\n No video pid found in input. Primary Input %d "), m_bPrimaryInput);
		LogErrorInfo(strTemp, m_nChannelNumber);
	}
	return nVedioPid;
}

bool CInput::Scan_Packet(unsigned char inBuf_Pack[])
{
	bool m_bDataOk = true;
	int nTemp_Pid = Find_PID(inBuf_Pack[1], inBuf_Pack[2]);
	unsigned char bufCmp[32];
	memset(bufCmp, 0, 32);
	if(nTemp_Pid == 8191)
	{
		m_NullPacketStream = true;
		
		m_bDataOk = false;
	}
	else
	{
		int CmpResult = memcmp(inBuf_Pack + 4, bufCmp, 32);
		if(CmpResult == 0)
		{
			m_EmptyPacketStream = true;
			m_bDataOk = false;
		}
		else
		{
			m_EmptyPacketStream = false;
		}
	}
	if(m_bVideoPidFound == false && m_bnOVedioPid == true)
	{
		m_bDataOk = false;
	}
	return m_bDataOk;
}

int CInput::Parse_Packet(unsigned char inBuf_Pack[])
{
	unsigned char Buf[189];
	int nstartPoint = 0;
	memset(Buf, 0, 188);
	if(inBuf_Pack[0] == 0x47)
	{
		int new_PID = Find_PID(inBuf_Pack[1], inBuf_Pack[2]);
		if(new_PID == 0)
		{
			nstartPoint = inBuf_Pack[4];

			if(nstartPoint != 0)
				nstartPoint++;
            m_bPAT_Found = true;
			int nPack_Section_Length = inBuf_Pack[nstartPoint + 6] &  0x0F;
			(nPack_Section_Length << 8);
			nPack_Section_Length = nPack_Section_Length | inBuf_Pack[nstartPoint + 7];
			if(nPack_Section_Length >= 255)
			{
				TCHAR strTemp[100];
				_stprintf(strTemp, _T("\n PAT packet is invalide. section length %d Primary Input %d"), nPack_Section_Length, m_bPrimaryInput);
				LogErrorInfo(strTemp, m_nChannelNumber);
			}
			m_nTotalProgNo = ((nPack_Section_Length - 13)/4) + 1;
			int pmt_PID = 0;
			int j = 0;
			for (int i = 0; i < m_nTotalProgNo; i++)
			{
				m_nProgramNo_Buff[j] = inBuf_Pack[nstartPoint + 13 + 4 * i];
				m_nProgramNo_Buff[j] = m_nProgramNo_Buff[i] << 8;
				m_nProgramNo_Buff[j] = m_nProgramNo_Buff[i] | inBuf_Pack[ nstartPoint + 14 + 4 * i];

				m_nPMT_PIDBuff[j] = Find_PID(inBuf_Pack[nstartPoint + 15 + 4 * i], inBuf_Pack[nstartPoint + 16 + 4 * i]);
				j++;
				if(j == 50)
				{
					j = 0;
				}
			}
			if(m_nProgramNo_Buff[0] == 0)
			{
				if(m_bPrimaryInput == true)
				{
					int q = 0;
				}
				/*m_nProgram_Num = m_nProgramNo_Buff[1];
				pmt_PID = m_nPMT_PIDBuff[1];*/
				/*TCHAR strTemp1[100];
				_stprintf(strTemp1, _T("\n-----------------Found in Parse pat %d primary %d"), pmt_PID, m_bPrimaryInput);*/
				//OutputDebugString(strTemp1);

				//Replacement as per selected prg no
				if(m_nTotalProgNo > 2 && StreamWatch[m_nChannelNumber].m_bSelected_PrgNo)
				{
					//Check Multi Program for Zero Program Number at first
					//return -1;
					for(int i = 0; i < m_nTotalProgNo; i++)
					{
						if(StreamWatch[m_nChannelNumber].m_nSelected_PrgNo == m_nProgramNo_Buff[i])
						{
							m_nProgram_Num = m_nProgramNo_Buff[i];
							pmt_PID = m_nPMT_PIDBuff[i];
						}
					}
				}
				else
				{
					m_nProgram_Num = m_nProgramNo_Buff[1];
					pmt_PID = m_nPMT_PIDBuff[1];
				}
			}
			else
			{
				if(m_bPrimaryInput == true)
				{
					int q = 0;
				}
				else
				{
					int qqq = pPrimary->m_nProgram_Num;
				}
				/*m_nProgram_Num = m_nProgramNo_Buff[0];
				pmt_PID = m_nPMT_PIDBuff[0];*/
			/*	TCHAR strTemp[100];
				_stprintf(strTemp, _T("\n-----------------Found in Parse pat %d primary %d"), pmt_PID, m_bPrimaryInput);*/
				//OutputDebugString(strTemp);
				memcpy(Buf, inBuf_Pack, 188);

				//Replacement as per selected prg no
				if(m_nTotalProgNo > 1 && StreamWatch[m_nChannelNumber].m_bSelected_PrgNo)
				{
					//Check Multi Program for Program Number at first
					//return -1;
					for(int i = 0; i < m_nTotalProgNo; i++)
					{
						if(StreamWatch[m_nChannelNumber].m_nSelected_PrgNo == m_nProgramNo_Buff[i])
						{
							m_nProgram_Num = m_nProgramNo_Buff[i];
							pmt_PID = m_nPMT_PIDBuff[i];
						}
					}
				}
				else
				{
					m_nProgram_Num = m_nProgramNo_Buff[0];
					pmt_PID = m_nPMT_PIDBuff[0];
				}
			}
			if(pmt_PID > 0)
				m_bPMT_Found = true;

			TCHAR strTemp[100];
			_stprintf(strTemp, _T("\n primary Input %d Program Number: %d Found PMT PID %d "), m_bPrimaryInput ,m_nProgram_Num, pmt_PID);
			LogStartStopEvents(strTemp, m_nChannelNumber);

			return pmt_PID;
		}	
	}
	return 0;
}


int CInput::Find_PCR_PID(unsigned char inBuf_Pack[],int nPMT_PId)
{
	int nstartPoint = 0;

	if(m_bPCRPID_Found == false)
	{
		int check_PID = Find_PID(inBuf_Pack[1], inBuf_Pack[2]);
		if(check_PID == nPMT_PId)
		{
			nstartPoint = inBuf_Pack[4];

			if(nstartPoint != 0)
				nstartPoint++;

			int nPCR_PId = Find_PID(inBuf_Pack[nstartPoint + 13], inBuf_Pack[nstartPoint + 14]);
			m_nTotalAudio = 0;
			m_nVideoPid = Parse_PMT(inBuf_Pack);
			
			if(m_nVideoPid > 0)
			{
				m_bnOVedioPid = false;
				/*_stprintf(strTemp, _T("\n Flag False m_bnoVedio Pid is false fail implementation now %d and INPUT %d"),m_bnOVedioPid, m_nSwitch_Input);
				OutputDebugString(strTemp);*/
				m_bPCRPID_Found = true;
				int m = m_nAudioPid[m_nTotalAudio];
				if(nPCR_PId > 0)
				{
					TCHAR strTemp1[200];
					_stprintf(strTemp1, _T("\n Found PCR_PID %d from PMT_PID %d in Find PCR_PID(). Primary Input %d"), nPCR_PId, nPMT_PId, m_bPrimaryInput);
					LogStartStopEvents(strTemp1, m_nChannelNumber);
					return nPCR_PId;
				}
			}
			else
			{
				m_bnOVedioPid = true;
				m_bPMT_Found = false;
				m_nFound_PMT_PID = 0;
				/*_stprintf(strTemp, _T("\n Flag False m_bnoVedio Pid is tru now %d and INPUT %d"),m_bnOVedioPid, m_nSwitch_Input);
				OutputDebugString(strTemp);*/
			}
		}	
	}
	return 0;
}
bool CInput::Scan_VedPid(unsigned char inBuf_Pack[], int nVed_Pid, int *nAud_Pid, int nTotalAudio)
{
	//Intialize and find Packet PID.
	bool m_bInputOk = true;
	int nPacketPid = Find_PID(inBuf_Pack[1], inBuf_Pack[2]);
	unsigned char bufCmp[32];
	unsigned char bufData[32];
	memset(bufCmp, 0, 32);
	int ContinutyCount;
	int CCDiff;

	// Count total packet Number
	m_nTotalPackets += 1;
	if(m_nTotalPackets >= 10001)
	{
		m_nTotalPackets = 0;
		m_nRound++;
	}
	// Update Info if the packet is video and find gap 
	if(nPacketPid == nVed_Pid)
	{
		m_nVideoPacketGap = m_nTotalPackets - m_nLastVidPacketIndex;
		if(m_nVideoPacketGap < 0)
		{
			m_nVideoPacketGap += 10000;
			m_nRoundVid ++;
		}
		m_nVideoPackets += 1;
		if(m_nVideoPackets >= 10001)
		{
			m_nVideoPackets = 0;
		}
		m_nLastVidPacketIndex = m_nTotalPackets;

		//count CCError for video packet
		if(pPrimary == NULL)
		{
			//if(StreamWatch[nChannelNumber].m_Input[0].m_nVideoPid == nVed_Pid)
			//{
				ContinutyCount = inBuf_Pack[3];
				ContinutyCount %= 16;
				if(StreamWatch[m_nChannelNumber].m_bCCVideo == true)
				{
					CCDiff = ContinutyCount - StreamWatch[m_nChannelNumber].m_lastCCVideo;
					if(CCDiff > 1)
						StreamWatch[m_nChannelNumber].m_CCError++;
				}
				else
					StreamWatch[m_nChannelNumber].m_bCCVideo = true;
				StreamWatch[m_nChannelNumber].m_lastCCVideo = ContinutyCount;

				if(StreamWatch[m_nChannelNumber].m_ModifiedCCError != StreamWatch[m_nChannelNumber].m_CCError)
				{
					StreamWatch[m_nChannelNumber].strCCError.Format(_T("%I64d"), StreamWatch[m_nChannelNumber].m_CCError);
					StreamWatch[m_nChannelNumber].m_strCCError.SetWindowTextW(StreamWatch[m_nChannelNumber].strCCError);
					StreamWatch[m_nChannelNumber].m_ModifiedCCError = StreamWatch[m_nChannelNumber].m_CCError;

					if(StreamWatch[m_nChannelNumber].m_CCError == 0 && StreamWatch[m_nChannelNumber].m_bCCError)
						StreamWatch[m_nChannelNumber].ResetCCError(m_nChannelNumber);
				}
			//}
		}
	}
	
	//count CCError for audio packet
	for(int i = 0; i < nTotalAudio; i++)
	{
		if(nPacketPid == nAud_Pid[i])
		{
			if(pPrimary == NULL)
			{
				//if(StreamWatch[nChannelNumber].m_Input[0].m_nAudioPid[i] == nAud_Pid[i])
				//{
				ContinutyCount = inBuf_Pack[3];
				ContinutyCount %= 16;
				if(StreamWatch[m_nChannelNumber].m_bCCAudio[i] == true)
				{
					CCDiff = ContinutyCount - StreamWatch[m_nChannelNumber].m_lastCCAudio[i];
					if(CCDiff > 1)
						StreamWatch[m_nChannelNumber].m_CCError++;
				}
				else
					StreamWatch[m_nChannelNumber].m_bCCAudio[i] = true;
				StreamWatch[m_nChannelNumber].m_lastCCAudio[i] = ContinutyCount;

				if(StreamWatch[m_nChannelNumber].m_ModifiedCCError != StreamWatch[m_nChannelNumber].m_CCError)
				{
					StreamWatch[m_nChannelNumber].strCCError.Format(_T("%I64d"), StreamWatch[m_nChannelNumber].m_CCError);
					StreamWatch[m_nChannelNumber].m_strCCError.SetWindowTextW(StreamWatch[m_nChannelNumber].strCCError);
					StreamWatch[m_nChannelNumber].m_ModifiedCCError = StreamWatch[m_nChannelNumber].m_CCError;

					if(StreamWatch[m_nChannelNumber].m_CCError == 0 && StreamWatch[m_nChannelNumber].m_bCCError)
						StreamWatch[m_nChannelNumber].ResetCCError(m_nChannelNumber);

				}
				//}
			}
		}
	}

	// Decide Stream health
	if(m_bVideoHealth == false)
	{
		if(m_nVideoPacketGap > m_nVideoGap)
		{
			
			m_nErrorRound += 1;
			if(m_nErrorRound >= m_nInvalidCount)
			{
				m_bVideoHealth = true;
				m_nErrorFreeRound = 0;
				m_bInputOk = false;
				return m_bInputOk;
			}
		}
		else
		{
			m_nErrorRound = 0;
		}
	}
	else
	{
		if(m_nVideoPacketGap < m_nVideoGap)
		{
			int a = m_nVideoPackets;
			m_nErrorFreeRound += 1;
			if(m_nErrorFreeRound >= m_nValidVideoCount)
			{
				m_bVideoHealth = false;
				m_bInputOk = true;
				return m_bInputOk;
			}
			m_nErrorRound = 0;
		}
		else
		{
			m_nErrorFreeRound = 0;
		}
		m_bInputOk = false;
		return m_bInputOk;
	}

	// Check bad and Null Packet
	if(nPacketPid == nVed_Pid)
	{	
		m_NullPacketStream = false;
		memcpy(bufData, inBuf_Pack, 32);
		int CmpResult = memcmp(inBuf_Pack + 4, bufCmp, 32);//Bad Packet
		if(CmpResult == 0)
		{
			if(m_dwLastVed_PidArrived != 0)
			{
				m_EmptyPacketStream = true;
				m_bInputOk = false;
				return m_bInputOk;
			}
		}
		else
		{
			m_EmptyPacketStream = false;
			m_dwLastVed_PidArrived = GetTickCount();
		}
	}
	else if(nPacketPid == 8191) //NULL Packet
	{
		m_NullPacketStream = true;
		m_bInputOk = false;
		return m_bInputOk;
	}
	else
	{
		m_NullPacketStream = false;
	}

	// Check Video packet is avialbility acoording to detection time
	int nDurationInterval = GetTickCount() - m_dwLastVed_PidArrived;
	if(nDurationInterval > m_nDetect_Time)
	{
		m_VideoPacketMissing = true;
		m_bInputOk = false;
		return m_bInputOk;		
	}
	else
	{
		m_VideoPacketMissing = false;
	}

	//Check PCR_PID packet is avialable according to detection time
	int nDurationPCR_PID = GetTickCount() - m_dwLastPCR_PIDArrived;
	if(nDurationPCR_PID > m_nDetect_Time)
	{
		m_PCR_PIDPacketMissing = true;
		m_bInputOk = false;
		return m_bInputOk;
	}
	else
	{
		m_PCR_PIDPacketMissing = false;
	}
		
	return m_bInputOk;
}

uint32_t CInput::CalculateCRC( BYTE *p_begin, int i_count )
{
    uint32_t CRC32[256] =
    {
        0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9,
        0x130476dc, 0x17c56b6b, 0x1a864db2, 0x1e475005,
        0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61,
        0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd,
        0x4c11db70, 0x48d0c6c7, 0x4593e01e, 0x4152fda9,
        0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,
        0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011,
        0x791d4014, 0x7ddc5da3, 0x709f7b7a, 0x745e66cd,
        0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039,
        0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5,
        0xbe2b5b58, 0xbaea46ef, 0xb7a96036, 0xb3687d81,
        0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
        0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49,
        0xc7361b4c, 0xc3f706fb, 0xceb42022, 0xca753d95,
        0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1,
        0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d,
        0x34867077, 0x30476dc0, 0x3d044b19, 0x39c556ae,
        0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
        0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16,
        0x018aeb13, 0x054bf6a4, 0x0808d07d, 0x0cc9cdca,
        0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde,
        0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02,
        0x5e9f46bf, 0x5a5e5b08, 0x571d7dd1, 0x53dc6066,
        0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
        0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e,
        0xbfa1b04b, 0xbb60adfc, 0xb6238b25, 0xb2e29692,
        0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6,
        0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a,
        0xe0b41de7, 0xe4750050, 0xe9362689, 0xedf73b3e,
        0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,
        0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686,
        0xd5b88683, 0xd1799b34, 0xdc3abded, 0xd8fba05a,
        0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637,
        0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb,
        0x4f040d56, 0x4bc510e1, 0x46863638, 0x42472b8f,
        0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
        0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47,
        0x36194d42, 0x32d850f5, 0x3f9b762c, 0x3b5a6b9b,
        0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff,
        0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623,
        0xf12f560e, 0xf5ee4bb9, 0xf8ad6d60, 0xfc6c70d7,
        0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,
        0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f,
        0xc423cd6a, 0xc0e2d0dd, 0xcda1f604, 0xc960ebb3,
        0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7,
        0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b,
        0x9b3660c6, 0x9ff77d71, 0x92b45ba8, 0x9675461f,
        0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,
        0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640,
        0x4e8ee645, 0x4a4ffbf2, 0x470cdd2b, 0x43cdc09c,
        0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8,
        0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24,
        0x119b4be9, 0x155a565e, 0x18197087, 0x1cd86d30,
        0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
        0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088,
        0x2497d08d, 0x2056cd3a, 0x2d15ebe3, 0x29d4f654,
        0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0,
        0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c,
        0xe3a1cbc1, 0xe760d676, 0xea23f0af, 0xeee2ed18,
        0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,
        0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0,
        0x9abc8bd5, 0x9e7d9662, 0x933eb0bb, 0x97ffad0c,
        0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668,
        0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4
    };

    uint32_t i_crc = 0xffffffff;

    /* Calculate the CRC */
    while( i_count > 0 )
    {
		uint32_t i_crc_24 = (i_crc>>24);
		uint32_t data = ((uint32_t)*p_begin);
		uint32_t index = (i_crc>>24) ^ ((uint32_t)*p_begin);
		uint32_t i_crc_8 = (i_crc<<8);
		uint32_t res = i_crc_8 ^ CRC32[index];

        i_crc = (i_crc<<8) ^ CRC32[ (i_crc>>24) ^ ((uint32_t)*p_begin) ];
        p_begin++;
        i_count--;
    }

    return( i_crc );
}



unsigned __int64 CInput::Find_PCR_Value(unsigned char inBuf_Pack[], int nPCR_PId)
{
	unsigned __int64 Current_PCR = 0;
	if(((unsigned int)((inBuf_Pack[1] & 0x1F) << 8) | inBuf_Pack[2]) == nPCR_PId
        && ((unsigned int)((inBuf_Pack[3] & 0x30) >> 4) > 1) // Adaption Feild is present => 00 - reserved, 01 - not present only payload, 10 - present without payload, 11 - present with payload
        && ((unsigned int)(inBuf_Pack[4]) > 0) // Adaption Feild Length
        && ((unsigned int)(inBuf_Pack[5] & 0x10))) //PCR flag value 
    {
        unsigned __int64 PCR_base = ((unsigned __int64)inBuf_Pack[6] << 25) |((unsigned __int64)inBuf_Pack[7] << 17)|((unsigned __int64)inBuf_Pack[8] << 9)|((unsigned __int64)inBuf_Pack[9] << 1) |((unsigned __int64)inBuf_Pack[10] >> 7);
        unsigned __int64 PCR_ext =  ((unsigned __int64)(inBuf_Pack[10]& 0x1) << 8) |((unsigned __int64)inBuf_Pack[11]);
        
		Current_PCR = PCR_base * 300 + PCR_ext;
	}

	return Current_PCR;
}

void CInput::Replace_Pid(int Pid,int oneByte, int SecondByte)
{
	if(Pid == 8191)
	{
		m_ReplaceBuffer[oneByte] = 0x1F ;
	    m_ReplaceBuffer[SecondByte] = 0xFF ;

	}
	else
	{
		unsigned char PidTemp2 = Pid;
		Pid = Pid >> 8;
		unsigned char PidTemp = Pid;
		m_ReplaceBuffer[oneByte] = (PidTemp & 0x1F) | (m_ReplaceBuffer[oneByte] & 0xE0);
		m_ReplaceBuffer[SecondByte] = PidTemp2;
	}
}
void CInput::Replace_VideoPacket()
{
	Replace_Pid(pPrimary->m_nVideoPid, 1 , 2);
}
void CInput::Replace_AudioPacket(int i)
{
	Replace_Pid(pPrimary->m_nAudioPid[i], 1 , 2);
}
void CInput::Replace_PATPacket()
{
	int TotalCRCCount = 0;
	int nPack_Section_Length = m_ReplaceBuffer[6] &  0x0F;
	(nPack_Section_Length << 8);
	nPack_Section_Length = nPack_Section_Length | m_ReplaceBuffer[7];
	if(nPack_Section_Length <= 0)
	{
		int a = nPack_Section_Length;
	}
     //int nTotalProgram = ((nPack_Section_Length - 13)/4) + 1;
	 int nTotalProgram = ((nPack_Section_Length - 9)/4);
	for (int i = 0; i < m_nTotalProgNo; i++)
	{
		//Program Number replacement
		int TempProgramNum = pPrimary->m_nProgram_Num;
		m_ReplaceBuffer[14 + 4 * i] = TempProgramNum;
		TempProgramNum = TempProgramNum >> 8;
		m_ReplaceBuffer[13 + 4 * i] = TempProgramNum;
		//Program Map PID replacement
		int checkPMTPID = Find_PID(m_ReplaceBuffer[15 + 4 * i], m_ReplaceBuffer[16 + 4 * i]);
		if(checkPMTPID == m_nFound_PMT_PID)
			Replace_Pid(pPrimary->m_nFound_PMT_PID, 15 + 4 * i, 16 + 4 * i);
		else
			Replace_Pid(0, 15 + 4 * i, 16 + 4 * i);
		TotalCRCCount = (16 + 4 * i) + 1;
	}
	//find CRC
	uint32_t FoundCRC = CalculateCRC(m_ReplaceBuffer + 5, TotalCRCCount - 5);
	BYTE NCRCbit4 = FoundCRC;
	FoundCRC = FoundCRC >> 8;
	BYTE NCRCbit3 = FoundCRC;
	FoundCRC = FoundCRC >> 8;
	BYTE NCRCbit2 = FoundCRC;
	FoundCRC = FoundCRC >> 8;
	BYTE NCRCbit1 = FoundCRC;
	//Replace CRC value
	m_ReplaceBuffer[TotalCRCCount++] = NCRCbit1;
	m_ReplaceBuffer[TotalCRCCount++] = NCRCbit2;
	m_ReplaceBuffer[TotalCRCCount++] = NCRCbit3;
	m_ReplaceBuffer[TotalCRCCount++] = NCRCbit4;
}
void CInput::Replace_PMTPacket()
{
	//Replace PID
	Replace_Pid(pPrimary->m_nFound_PMT_PID, 1, 2);
	int nVedioPid = 0;
	int nSectionLen = 0;
	int nPrgInfoLen = 0;
	int nEs_InfoLen = 0;
	int nIndex = 6;
	nSectionLen = Find_Length(m_ReplaceBuffer[nIndex],m_ReplaceBuffer[nIndex + 1]);
	//Replace Program Number
	int TempProgramNumber = pPrimary->m_nProgram_Num;
	m_ReplaceBuffer[nIndex + 3] = TempProgramNumber;
	TempProgramNumber = TempProgramNumber >> 8;
	m_ReplaceBuffer[nIndex + 2] = TempProgramNumber;
	//Replace PCR_PID
	if(pPrimary->m_nFound_PCR_PID == pPrimary->m_nVideoPid)
		Replace_Pid(pPrimary->m_nFound_PCR_PID, nIndex + 7, nIndex + 8);
	else
		Replace_Pid(pPrimary->m_nVideoPid, nIndex + 7, nIndex + 8);

	nIndex += 9;
 	nPrgInfoLen = Find_Length(m_ReplaceBuffer[nIndex],m_ReplaceBuffer[nIndex + 1]);
	nIndex = nIndex + nPrgInfoLen + 2;
	int CountNumbetr = 0;
	//Replace Elementry stream type and PID
	while(nIndex < (nSectionLen + 8 - 4))
	{
		if(m_ReplaceBuffer[nIndex] == 0x02 || m_ReplaceBuffer[nIndex] == 0x1b || m_ReplaceBuffer[nIndex] == 0x24)
		{
			m_ReplaceBuffer[nIndex] = m_VideoType;
			Replace_Pid(pPrimary->m_nVideoPid, nIndex + 1, nIndex + 2);
		}
		if(m_ReplaceBuffer[nIndex] == 0x04 || m_ReplaceBuffer[nIndex] == 0x81 || m_ReplaceBuffer[nIndex] == 0x03 || m_ReplaceBuffer[nIndex] == 0x11 || m_ReplaceBuffer[nIndex] == 0x00)
		{
			if(CountNumbetr < pPrimary->m_nTotalAudio)
			{
				m_ReplaceBuffer[nIndex] = m_AudioType[CountNumbetr];
				Replace_Pid(pPrimary->m_nAudioPid[CountNumbetr], nIndex + 1, nIndex + 2);
				CountNumbetr++;
			}
			else
			{
				//Replacement as per selected prg no
				m_ReplaceBuffer[nIndex] = m_AudioType[CountNumbetr];
				Replace_Pid(8191, nIndex + 1, nIndex + 2);
				CountNumbetr++;
			}
		}
		nIndex += 3;
		nEs_InfoLen = Find_Length(m_ReplaceBuffer[nIndex], m_ReplaceBuffer[nIndex + 1]);
		nIndex = nIndex + nEs_InfoLen + 2;
	}

	//find CRC
	uint32_t FoundCRC = CalculateCRC(m_ReplaceBuffer + 5, nIndex - 5);
	BYTE NCRCbit4 = FoundCRC;
	FoundCRC = FoundCRC >> 8;
	BYTE NCRCbit3 = FoundCRC;
	FoundCRC = FoundCRC >> 8;
	BYTE NCRCbit2 = FoundCRC;
	FoundCRC = FoundCRC >> 8;
	BYTE NCRCbit1 = FoundCRC;
	//Replace CRC value
	m_ReplaceBuffer[nIndex++] = NCRCbit1;
	m_ReplaceBuffer[nIndex++] = NCRCbit2;
	m_ReplaceBuffer[nIndex++] = NCRCbit3;
	m_ReplaceBuffer[nIndex++] = NCRCbit4;
}
void CInput::Replace_WithNullPacket()
{
	Replace_Pid(8191, 1, 2);
}
void CInput::Replace_PCRPacket()
{
	Replace_Pid(pPrimary->m_nFound_PCR_PID, 1, 2);
}
void CInput::Change_StreamInfo(unsigned char inBuf_Pack[])
{
	memcpy(m_ReplaceBuffer, m_AnalysPacket_Buf, 188);
	int nTemp_Pid = Find_PID(m_ReplaceBuffer[1], m_ReplaceBuffer[2]);
	if(nTemp_Pid == m_nVideoPid)
	{
		m_bNULLPid = true;
		Replace_VideoPacket();
	}
	else if(nTemp_Pid == 0)
	{
		m_bNULLPid = true;
		Replace_PATPacket();
	}
	else if(nTemp_Pid == m_nFound_PMT_PID)
	{
		m_bNULLPid = true;
		Replace_PMTPacket();
		
	}
	else
	{
		for(int i = 0; i < m_nTotalAudio; i++)
		{
			if(nTemp_Pid == m_nAudioPid[i])
			{
				m_bNULLPid = true;
				if(i >= pPrimary->m_nTotalAudio)
				{
					Replace_WithNullPacket();
				}
				else
				{
					Replace_AudioPacket(i);
				}

			}
		}
	}
	
	if(!m_bNULLPid)
	{
		Replace_WithNullPacket();
	}
	else
	{
		m_bNULLPid = false;
	}
	/*for(int i = 0; i < m_nTotalAudio; i++)
	{
		if(nTemp_Pid == m_nAudioPid[i])
		{
			if(i >= pPrimary->m_nTotalAudio)
			{
				Replace_WithNullPacket();
			}
			else
			{
				Replace_AudioPacket(i);
			}
			
		}
	}*/
	
}
// Before PCR_PID Found Function
void CInput::Indentify_Scan_Input() 
{
	int j = 0;
	while(j < 7)
	{
		bool bScanOutPut = false;
		try
		{
			memset(m_AnalysPacket_Buf, 0 , sizeof(unsigned char)*188);
			memcpy(m_AnalysPacket_Buf, m_SourceBuffer[m_nIndex_Scan] + (j * 188), 188);
		}
		catch(...)
		{
			LogException(_T("Problem in Creating Packet of Analys"), m_nChannelNumber);
		}

		if(bStartVideoAnalysis)
			bScanOutPut = Scan_Packet(m_AnalysPacket_Buf);
		
		if((bStartVideoAnalysis == true && bScanOutPut == true && m_bMPTS_Stream == false) || (bStartVideoAnalysis == false && m_bMPTS_Stream == false))
		{
			/*if(m_bPrimaryInput == true)
			{
				if(m_bAutoStopPrimary == false)
					m_dwLastUpdateTime = GetTickCount();
			}
			else
			{*/
				m_dwLastUpdateTime = GetTickCount();
			//}
		}

		if(m_bPMT_Found == true)					
		{
			int nTempPID_PCR = Find_PCR_PID(m_AnalysPacket_Buf, m_nFound_PMT_PID);
			m_nPcrPid = nTempPID_PCR;
			if(nTempPID_PCR != 0)
			{
				m_nFound_PCR_PID = nTempPID_PCR;
				OutputDebugString(_T("pcr pid found\n"));
				//break;
			}
		}
		else //if(m_bPMT_Found == false)
		{
			int nTempPID_PMT = Parse_Packet(m_AnalysPacket_Buf);

			if(nTempPID_PMT == 0 && m_bPMT_Found)
			{
				m_bMPTS_Stream = false;
				TCHAR strTemp[200];
				_stprintf(strTemp, _T("\n\n PMT_PID = %d  - pStream->m_bPMT_Found = %d\n\n"), nTempPID_PMT, m_bPMT_Found);
				//OutputDebugString(strTemp);
			}

			if (nTempPID_PMT != 0)
			{
				m_bMPTS_Stream = false;
				m_nFound_PMT_PID = nTempPID_PMT;
				TCHAR strTemp[200];
				_stprintf(strTemp, _T("\n Found Sender PMT_PID %d - pStream->m_bPMT_Found = %d"), m_nFound_PMT_PID, m_bPMT_Found);
				//OutputDebugString(strTemp);
				// m_nProgram_Num and m_nPID_PMT both find here
			}
			if(nTempPID_PMT == -1)
			{
				m_bMPTS_Stream = true;
				m_bPCR_PIDNotFound = true;
			}
		}

		j++;
	}
} 
 // after PCR_PID Found Function
void CInput::Scan_Replacement_Input()
{
	int k = 0;
	while(k < 7)
	{
		bool bScanOutPut = false;
		try
		{
			memcpy(m_AnalysPacket_Buf, m_SourceBuffer[m_nIndex_Scan] + (k * 188), 188);
		}
		catch(...)
		{
			LogException(_T("Problem in creating AnalysBuffer for send the data"),m_nChannelNumber);
		}

		if(bStartVideoAnalysis)
		{
			int temp_pcr = Find_PCR_Value(m_AnalysPacket_Buf, m_nFound_PCR_PID);

			if(temp_pcr != 0)
			{
				m_dwLastPCR_PIDArrived = GetTickCount();
			}

			if(m_bVideoPidFound == true)
			{
				bScanOutPut = Scan_VedPid(m_AnalysPacket_Buf, m_nVideoPid, m_nAudioPid, m_nTotalAudio);
			}

		}

		if((bScanOutPut == true && bStartVideoAnalysis == true) || bStartVideoAnalysis == false)
		{

			/*if(pPrimary->m_bAutoStopPrimary == 0)
			{
				m_dwLastUpdateTime = GetTickCount();
			}
			else
			{
				if(m_bPrimaryInput == false)
				{
					m_dwLastUpdateTime = GetTickCount();
				}
			}	*/
		/*	if(m_bPrimaryInput == true)
			{
				if(m_bAutoStopPrimary == false)
					m_dwLastUpdateTime = GetTickCount();
			}
			else
			{*/
				m_dwLastUpdateTime = GetTickCount();
			//}
		}
		if(bEMERGENCY)
		{
			if(m_bPrimaryInput == false && m_nSwitchIndicator == 1)
			{
				if(pPrimary->m_bPCRPID_Found == true && pPrimary->m_bEmergencyActive == 1)
				{
					Change_StreamInfo(m_AnalysPacket_Buf);
					memcpy(m_SourceBuffer[m_nIndex_Scan] + (k * 188), m_ReplaceBuffer, 188);
				}
			}
		}
		k++;
	}

}
CString CInput::Switch_Reason()
{
	CString Reason = _T("");
	
	if(m_bMPTS_Stream == true)
	{
		Reason.AppendFormat(_T("Input Stream is MPTS"));
	}
	else if(m_bVideoHealth == true)
	{
		Reason.AppendFormat(_T("Video packet Gap of Input stream is more than GapAnalyze file"));
	}
	else if(m_NullPacketStream == true)
	{
		m_NullPacketStream = false;
		Reason.AppendFormat(_T("Input Stream contains only Null packet"));
	}
	else if(m_EmptyPacketStream == true)
	{
		Reason.AppendFormat(_T("Input Stream contains only Empty packets"));
	}
	else if(m_VideoPacketMissing == true)
	{
		Reason.AppendFormat(_T("Video Packet Missing in Input stream"));
	}
	else
	{
		Reason.AppendFormat(_T("Input stream not available"));
	}

	return Reason;
}

