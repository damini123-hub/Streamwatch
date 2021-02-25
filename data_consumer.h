/* ----------------------------------------------------------------------------
 * File: data_consumer.h
 *
 * Desc: Data Consumer Declaration
 *
 * Copyright (c) 2007-2009, MainConcept GmbH. All rights reserved.
 *
 * This software is the confidential and proprietary information of
 * MainConcept GmbH and may be used only in accordance with the terms of
 * your license from MainConcept GmbH.
 * ----------------------------------------------------------------------------
 */


#if !defined (__NET_DATA_CONSUMER_H__)
#define __NET_DATA_CONSUMER_H__

#include "stdafx.h"
#include "net_source.h"
#include "sample_net_ref_counter.h"

class CNetLiveClientEngine;
class CDataConsumer : public MainConcept::NetworkStreaming::IMediaConsumer
{
public:
    virtual ~CDataConsumer();
    CDataConsumer( const int8_t *pName, const int8_t *pStreamMime, CNetLiveClientEngine* pEngine );

    int Init( uint32_t uiMaxBufferLen = 0, uint8_t *pExternalBuffer = NULL );
    // implementation of MainConcept::NetworkStreaming::IMediaConsumer
public:
    virtual void GetBuffer( MainConcept::NetworkStreaming::CONSUMERINFO *aInfo );
    virtual void OnData( MainConcept::NetworkStreaming::CONSUMERINFO* aInfo );
    virtual void OnNotify( MainConcept::NetworkStreaming::NETSOURCENOTIFYTYPE aNotifyType, void *pNotifyData = NULL );
    virtual bool IsBufferAvailable();
    
    int32_t InitAVCDecoder(int8_t* pOutName, int8_t* pConfigName = NULL, bool bIsSilent = false);

protected:
    FILE         *m_pOutFile;
    int8_t        m_pFileName[1024];
    
    typedef struct _tagBufferInfo {

        bool     _bIsExternalBuffer;
        uint8_t  *_pSrcPos;
        uint8_t  *_pCurrPos;
        uint32_t _uiMaxBuffLen;
        uint32_t _uiCurrBuffLen;
    } BUFFERINFO;

    BUFFERINFO  m_Buffer;
    
    bool m_bIsDecodeAVC;
	CNetLiveClientEngine* m_pEngine;
};

#endif // __NET_DATA_CONSUMER_H__
