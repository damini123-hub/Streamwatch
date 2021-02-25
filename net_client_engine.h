/* ----------------------------------------------------------------------------
 * File: net_client_engine.cpp
 *
 * Desc: RTP Client Engine API consumer (impl)
 *
 * Copyright (c) 2007-2009, MainConcept GmbH. All rights reserved.
 *
 * This software is the confidential and proprietary information of
 * MainConcept GmbH and may be used only in accordance with the terms of
 * your license from MainConcept GmbH.
 * ----------------------------------------------------------------------------
 */


#if !defined (__SAMPLE_NET_LIVE_CLIENT_H__)
#define __SAMPLE_NET_LIVE_CLIENT_H__

#include "stdafx.h"
#include "net_source.h"
#include "data_consumer.h"
#include "nutil_api.h"
#include "tools.h"
#include <list>

using namespace std;

typedef enum _tagENGINE_STATE 
{
	engineReady,
	engineActive,
	engineStopped,
	engineUnknown
} ENGINE_STATE;

class CStream;
class CNetLiveClientEngine
{
public:
    CNetLiveClientEngine(int a , CStream *Stream1);
    ~CNetLiveClientEngine();

public:
	int8_t m_pEngineId;
    int32_t Init( const int8_t *aUrl, const int8_t *aNIC, int32_t iTimeOut );
    int32_t Init( const uint8_t *pSdpData, int32_t iSdpLength, const int8_t *aNIC, int32_t iTimeOut, int8_t* pOutName, int8_t* pConfigName, bool isSilent, bool isDecodeAVC );

    int Run();
    int Stop();

    int32_t SetMediaDataConsumer( MainConcept::NetworkStreaming::LIVESTREAMINGPARAMS *aParams, bool bRtcpShouldPresent = false );
    int32_t DeleteConsumer( MainConcept::NetworkStreaming::IMediaConsumer *pConsumer );
    int32_t SetTimeout( int32_t aTimeOut = 0 );
    MainConcept::NetworkStreaming::STREAMTYPE GetMediaType( uint32_t aMediaIndex ) const;
    double GetDuration() const;

	void OnDisconnect( MainConcept::NetworkStreaming::IMediaConsumer *pConsumer);
	ENGINE_STATE GetState() { return m_State; }

protected:
    int32_t StartLiveStreamingSession();
    int32_t StopLiveStreamingSession( bool bIsHardStop = false );

private:
    int32_t PreInit();
    const int8_t * StreamTypeToString( MainConcept::NetworkStreaming::STREAMTYPE aType ) const;
    int32_t ParseURL( int8_t *pUrl, MainConcept::NetworkStreaming::PROTOCOL *pProto, int8_t *pServerAddress, uint32_t *uiPort );

protected:
    MainConcept::NetworkStreaming::INetworkSource   *m_pEngine;
    MainConcept::NetworkStreaming::ISdpDecoder      *m_pSdpDecoder;

    int8_t                                      *m_pUrl;

    int8_t                                      *m_pNIC;
    uint16_t                                    m_usTimeOut;

    list<MainConcept::NetworkStreaming::IMediaConsumer *>   m_lstConsumers;
	int32_t m_iActiveConsumerCount;
	ENGINE_STATE m_State;

public:
	CStream *m_pStream;
};

#endif //__SAMPLE_NET_LIVE_CLIENT_H__
