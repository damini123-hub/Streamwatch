/* ----------------------------------------------------------------------------
 * File: data_consumer.cpp
 *
 * Desc: Data Consumer Implementation
 *
 * Copyright (c) 2007-2009, MainConcept GmbH. All rights reserved.
 *
 * This software is the confidential and proprietary information of
 * MainConcept GmbH and may be used only in accordance with the terms of
 * your license from MainConcept GmbH.
 * ----------------------------------------------------------------------------
 */
#include "stdafx.h"
#include "sample_net_common.h"
#include "data_consumer.h"
#include "net_client_engine.h"
#include "Winbase.h"

using namespace MainConcept;
using namespace MainConcept::NetworkStreaming;

#define MIN_BUFFER_SIZE 20*1316
#define MAX_BUFFER_LEN  2 * MIN_BUFFER_SIZE

CDataConsumer::~CDataConsumer()
{
	if ( m_Buffer._pSrcPos && m_Buffer._uiCurrBuffLen > 0 ) 
	{
	/*	if ( m_pOutFile )
		{
			fwrite(m_Buffer._pSrcPos, 1, m_Buffer._uiCurrBuffLen, m_pOutFile);
		}*/
		m_Buffer._pCurrPos = m_Buffer._pSrcPos;
		m_Buffer._uiCurrBuffLen = 0;
/*	}
	if ( m_pOutFile ) {
        fclose(m_pOutFile);*/
    }
    if ( !m_Buffer._bIsExternalBuffer ) {
        SAFE_DELETE_ARRAY(m_Buffer._pSrcPos);
    }
}

CDataConsumer::CDataConsumer( const int8_t *pName, const int8_t *pStreamMime, CNetLiveClientEngine *pEngine  )
:   m_pOutFile(NULL)
,   m_bIsDecodeAVC(false)
,	m_pEngine(pEngine)
{
	/*memset(m_pFileName, 0, 1024);
	sprintf((char*)m_pFileName, "D:\\%s.%s", pName, pStreamMime);
    m_pOutFile = fopen((char*)m_pFileName, "wb");*/

    memset(&m_Buffer, 0, sizeof(BUFFERINFO));
}

void CDataConsumer::GetBuffer( CONSUMERINFO *aInfo )
{
	aInfo->pData = m_Buffer._pCurrPos;
	InitConsumerInfo( aInfo, m_Buffer._pSrcPos, m_Buffer._uiMaxBuffLen - m_Buffer._uiCurrBuffLen );
}

void CDataConsumer::OnData( CONSUMERINFO* aInfo )
{
    if ( !aInfo || !aInfo->pData ) 
	{
        return;
    }

	if ( aInfo->bIsValidSample  ) 
	{
		m_Buffer._pCurrPos += aInfo->dwActualLen;
		m_Buffer._uiCurrBuffLen += aInfo->dwActualLen;
	}

	if ( m_Buffer._pSrcPos && m_Buffer._uiMaxBuffLen - m_Buffer._uiCurrBuffLen < MIN_BUFFER_SIZE ) 
	{
		/*if ( m_pOutFile ) 		
		{*/
			for(unsigned int j = 0;j < (m_Buffer._uiCurrBuffLen/1316); j++)
			{
				int npEngine_wrt = m_pEngine->m_pEngineId;
				/*if(npEngine_wrt == 0)
				{
					fwrite(m_Buffer._pSrcPos + (j * 1316), 1, 1316, m_pOutFile);
				}*/
				try
				{
					memcpy((void *)m_pEngine->m_pStream->m_Input[npEngine_wrt].m_SourceBuffer[m_pEngine->m_pStream->m_Input[npEngine_wrt].m_nIndex_Write], m_Buffer._pSrcPos + (j * 1316), 1316);
	
				}
				catch(...)
				{
					LogException(_T("problem in data receiver"),m_pEngine->m_pStream->m_nChannel_Index);
				}
				
				m_pEngine->m_pStream->m_Input[npEngine_wrt].m_nIndex_Write++;
				if(m_pEngine->m_pStream->m_Input[npEngine_wrt].m_nIndex_Write >= 10000)
				{
					m_pEngine->m_pStream->m_Input[npEngine_wrt].m_nIndex_Write = 0;
					//m_pEngine->m_pStream->write_Round[npEngine_wrt]++;
				}
			}
			m_Buffer._pCurrPos = m_Buffer._pSrcPos;
			m_Buffer._uiCurrBuffLen = 0;
		}
	//}
}

void CDataConsumer::OnNotify( NETSOURCENOTIFYTYPE aNotifyType, void *pNotifyData /*= NULL*/ )
{
	if ( aNotifyType == EDisconnect ) 
	{
		if ( m_Buffer._pSrcPos && m_Buffer._uiCurrBuffLen > 0 )
		{
			m_Buffer._pCurrPos = m_Buffer._pSrcPos;
			m_Buffer._uiCurrBuffLen = 0;
		}
		m_pEngine->OnDisconnect(this);
	}
	else if ( aNotifyType != ENetworkBytesReceived ) 
	{
		printf("\nOn notify [%d]\n", aNotifyType);
	}
}

bool CDataConsumer::IsBufferAvailable()
{
	return !!m_Buffer._pSrcPos && m_Buffer._uiMaxBuffLen - m_Buffer._uiCurrBuffLen >= MIN_BUFFER_SIZE;
}

int CDataConsumer::Init( uint32_t uiMaxBufferLen /*= 0*/, uint8_t *pExternalBuffer/* = NULL*/ )
{
    m_Buffer._bIsExternalBuffer = false;
    if ( uiMaxBufferLen > 0 && pExternalBuffer ) 
	{
        m_Buffer._pCurrPos = m_Buffer._pSrcPos = pExternalBuffer;
        m_Buffer._uiMaxBuffLen = uiMaxBufferLen;
        m_Buffer._bIsExternalBuffer = true;
        return 0;
    }
    m_Buffer._pSrcPos = new uint8_t[MAX_BUFFER_LEN];
    if ( !m_Buffer._pSrcPos ) 
	{
        return -1;
    }
    memset(m_Buffer._pSrcPos, 0, MAX_BUFFER_LEN);
    m_Buffer._pCurrPos = m_Buffer._pSrcPos;
    m_Buffer._uiMaxBuffLen = MAX_BUFFER_LEN;
    return 0;
}

int32_t CDataConsumer::InitAVCDecoder( int8_t* pOutName, int8_t* pConfigName /*= NULL*/, bool bIsSilent /*= false*/ )
{
    if (!pOutName)
	{
        return -1;    
    }
   return 0;
}
