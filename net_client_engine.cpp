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

#include "stdafx.h"
#include "sample_net_common.h"
#include "net_client_engine.h"

#define ENGINE_GLOBAL_ERR -1
#define ENGINE_SUCCESS 0
#define CHECK_NET_ENGINE_PTR_RET( __ptr__ ) if ( !__ptr__ ) return ENGINE_GLOBAL_ERR
#define CHECK_NET_ENGINE_ERR_CODE( __err_code__ ) if ( __err_code__ != MCNetResultOK ) return ENGINE_GLOBAL_ERR

using namespace MainConcept::NetworkStreaming;

CNetLiveClientEngine::CNetLiveClientEngine(int id , CStream *Stream1)
:   m_pEngine(NULL)
,   m_pSdpDecoder(NULL)
,   m_pUrl(NULL)
,   m_pNIC(NULL)
,   m_usTimeOut(5)
,	m_iActiveConsumerCount(0)
,	m_State(engineUnknown)
,   m_pEngineId(0)
,   m_pStream(Stream1)
{
 m_pEngineId = id;
}

CNetLiveClientEngine::~CNetLiveClientEngine()
{
	SAFE_RELEASE(m_pEngine);
	SAFE_RELEASE(m_pSdpDecoder);
    SAFE_DELETE_ARRAY(m_pUrl);
    SAFE_DELETE_ARRAY(m_pNIC);
    list<IMediaConsumer *>::iterator iterator_lstConsumers = m_lstConsumers.begin();
    for ( ;iterator_lstConsumers != m_lstConsumers.end(); ++iterator_lstConsumers ) {

        IMediaConsumer *pDataConsumer = (IMediaConsumer *)*iterator_lstConsumers;
        SAFE_DELETE(pDataConsumer);
    };
}

int32_t CNetLiveClientEngine::PreInit()
{
    SAFE_RELEASE(m_pEngine);

    StartSocketService();

    m_pEngine = GetRTPClientInterface( NULL );
    CHECK_NET_ENGINE_PTR_RET(m_pEngine);

    return ENGINE_SUCCESS;
}

int32_t CNetLiveClientEngine::Init( const int8_t *aUrl, const int8_t *aNIC, int32_t iTimeOut )
{
    if ( PreInit() < 0 ) {
        return ENGINE_GLOBAL_ERR;
    }

    if ( !aUrl || !aNIC ) {
        return ENGINE_GLOBAL_ERR;
    }

    m_pUrl = new int8_t[strlen((char*)aUrl) + 1];
    memset(m_pUrl, 0, strlen((char*)aUrl) + 1);
    memcpy(m_pUrl, aUrl, strlen((char*)aUrl));

    m_pNIC = new int8_t[strlen((char*)aNIC) + 1];
    memset(m_pNIC, 0, strlen((char*)aNIC) + 1);
    memcpy(m_pNIC, aNIC, strlen((char*)aNIC));

    PROTOCOL clientProto = protocolRtpAvp;
    int8_t pServerAddress[1024] = {0};
    uint32_t MediaPort = 1024;

    if ( ParseURL( m_pUrl, &clientProto, pServerAddress, &MediaPort ) < 0 ) {
        return ENGINE_GLOBAL_ERR;
    }

    int8_t MediaName[128] = {0};
    sprintf_s((char*)MediaName,128, "%u", 1);
    CDataConsumer *pDataConsumer = new CDataConsumer( MediaName, StreamTypeToString( isMPEG2TransportStream ), this );

    if ( pDataConsumer->Init() < 0 ) {
        return ENGINE_GLOBAL_ERR;
    }

	m_lstConsumers.push_back( pDataConsumer );
    m_iActiveConsumerCount++;

    LIVESTREAMINGPARAMS livestreamingparams;

	memset(&livestreamingparams, 0, sizeof(LIVESTREAMINGPARAMS));

    livestreamingparams.eProto         = clientProto;
    livestreamingparams.pMediaConsumer = pDataConsumer;
    livestreamingparams.ucsNIC = (uint8_t *)m_pNIC;
    livestreamingparams.uiSampleRate = 90000;
    livestreamingparams.csServerName = (int8_t *)&pServerAddress[0];
    livestreamingparams.ulClientPort = MediaPort;
    livestreamingparams.eStreamType  = isMPEG2TransportStream;
	livestreamingparams.uiPayloadType = 0xff;

    if ( m_pEngine->SetMediaDataConsumer( &livestreamingparams ) < 0 ) {
        return ENGINE_GLOBAL_ERR;
    }

	SetTimeout(iTimeOut);

	m_State = engineReady;
    return ENGINE_SUCCESS;
}

int32_t CNetLiveClientEngine::Init( const uint8_t *pSdpData, int32_t iSdpLength, const int8_t *aNIC, int32_t iTimeOut, int8_t* pOutName, int8_t* pConfigName, bool isSilent, bool bIsDecode )
{
    if ( PreInit() < 0 ) {
        return ENGINE_GLOBAL_ERR;
    }

    m_pNIC = new int8_t[strlen((char*)aNIC) + 1];
    memset(m_pNIC, 0, strlen((char*)aNIC) + 1);
    memcpy(m_pNIC, aNIC, strlen((char*)aNIC));

    m_pSdpDecoder = GetSdpDecoderInterface( NULL );
    if ( !m_pSdpDecoder ) {
        return ENGINE_GLOBAL_ERR;
    }

    if ( !m_pSdpDecoder->ParseSdp( pSdpData, iSdpLength ) ) {

        printf("Corrupted SDP Data\n");
        return ENGINE_GLOBAL_ERR;
    }

    uint32_t uiMediaCount = m_pSdpDecoder->GetMediaCount();
    uint32_t uiIndex = 0;
    for ( ; uiIndex < uiMediaCount; uiIndex++ ) {

        const ISdpMediaInfo *pSdpMediaInfo = m_pSdpDecoder->EnumMediaInfoInterfaces( uiIndex );
        const int8_t *pStreamType = StreamTypeToString( pSdpMediaInfo->GetStreamType() );

        int8_t MediaName[128] = {0};
		sprintf_s((char*)MediaName,128, "%u", pSdpMediaInfo->GetID());
        CDataConsumer *pDataConsumer = new CDataConsumer( MediaName, pStreamType, this );

        if ( pDataConsumer->Init() < 0 ) {

            delete pDataConsumer;
            return ENGINE_GLOBAL_ERR;
        }

		m_lstConsumers.push_back( pDataConsumer );
        m_iActiveConsumerCount++;

		LIVESTREAMINGPARAMS livestreamingparams;
		memset(&livestreamingparams, 0, sizeof(LIVESTREAMINGPARAMS));

        const int8_t *pMediaAddress = pSdpMediaInfo->GetAddress();
        livestreamingparams.csServerName = ( pMediaAddress ) ? (int8_t *)pMediaAddress : (int8_t *)m_pSdpDecoder->GetConnectionAddress();
        livestreamingparams.pMediaConsumer = pDataConsumer;
        livestreamingparams.ucsNIC = (uint8_t *)m_pNIC;
        livestreamingparams.ulClientPort = pSdpMediaInfo->GetPort();
        livestreamingparams.eProto = pSdpMediaInfo->GetProtocol();
        livestreamingparams.eStreamType = pSdpMediaInfo->GetStreamType();
		livestreamingparams.uiPayloadType = pSdpMediaInfo->GetPayloadType();

        LATMPARAMS LATMparams;
		memset(&LATMparams, 0, sizeof(LATMPARAMS));

        if ( pSdpMediaInfo->GetMediaType() == mediaVideo || livestreamingparams.eStreamType == isMPEG2ProgramStream || livestreamingparams.eStreamType == isMPEG2TransportStream || livestreamingparams.eStreamType == isMPEG1SystemStream ) {
            livestreamingparams.uiSampleRate = 90000;
        } else {

            const int8_t *pRtpMap = (const int8_t *)pSdpMediaInfo->GetAttrValueByName((int8_t *)"rtpmap");
            if ( !pRtpMap ) {
                livestreamingparams.uiSampleRate = 90000;
            } else {

                const int8_t *pTemp = (const int8_t*)strstr((char*)pRtpMap, "/") + 1;
                switch ( livestreamingparams.eStreamType ) {

                    case isAC3:
                    case isMPEG1_2_Audio:
                    case isMP3:
                        livestreamingparams.uiSampleRate = 44100;
                        if ( pTemp ) {
                            livestreamingparams.uiSampleRate = atol((const char*)pTemp);
                        }
                        break;

                    case isAAC:
                        {
                            const uint8_t* pTempValue = pSdpMediaInfo->GetAttrValueByName((int8_t *)"sizeLength");
                            if (pTempValue) {
                                livestreamingparams.pAACparams._uiSizeLength = atol((const char *)pTempValue);
                            }

                            pTempValue = pSdpMediaInfo->GetAttrValueByName((int8_t *)"indexLength");
                            if (pTempValue) {
                                livestreamingparams.pAACparams._uiIndexLength = atol((const char *)pTempValue); 
                            }

                            pTempValue = pSdpMediaInfo->GetAttrValueByName((int8_t *)"indexDeltaLength");
                            if (pTempValue) {
                                livestreamingparams.pAACparams._uiIndexDeltaLength = atol((const char *)pTempValue);
                            }

                            livestreamingparams.uiSampleRate = 44100;
                            if ( pTemp ) {
                                livestreamingparams.uiSampleRate = atol((const char*)pTemp);
                            }
                        }
                        break;

                    case isLATM:
                        {
                            livestreamingparams.uiSampleRate = 44100;
                            if ( pTemp ) {
                                livestreamingparams.uiSampleRate = atol((const char*)pTemp);
                            }

                            const uint8_t *pSrcConfig = pSdpMediaInfo->GetAttrValueByName((int8_t *)"config");
                            int32_t iSrcCofnigLen = 0;
                            if ( pSrcConfig ) {

                                iSrcCofnigLen = (int32_t)strlen((const char *)pSrcConfig);

                                LATMparams.pMuxConfigData = new uint8_t[iSrcCofnigLen];
                                memset(LATMparams.pMuxConfigData, 0, iSrcCofnigLen);

                                LATMparams.iMuxConfigDataLen = iSrcCofnigLen;
                                LATMparams.iMuxConfigDataLen = ConvertHexStringToValue( (char*)pSrcConfig, iSrcCofnigLen, LATMparams.pMuxConfigData, LATMparams.iMuxConfigDataLen );
                            }

                            livestreamingparams.pLatmParams.iMuxConfigDataLen = LATMparams.iMuxConfigDataLen;
                            livestreamingparams.pLatmParams.pMuxConfigData    = LATMparams.pMuxConfigData;

                            const int8_t *temp = (const int8_t *)pSdpMediaInfo->GetAttrValueByName((int8_t *)"cpresent");
                            if ( temp ) {
                                livestreamingparams.pLatmParams.bConfigPresetInPayload = !!atol((const char *)temp);
                            }
                        }
                        break;

                    case isPCMU:
                    case isPCMA:
                    case isL8:
                    case isL16:

                        livestreamingparams.uiSampleRate = 8000;
                        if ( pTemp ) {
                            livestreamingparams.uiSampleRate = atol((const char*)pTemp);
                        }
                        break;

					default:;
                };
            }
        }

        uint32_t uiSeqHeaderLen = 0;
        const int8_t *pSeqHeader = (const int8_t *)pSdpMediaInfo->GetSequenceHeader( &uiSeqHeaderLen );
        if ( pSeqHeader && uiSeqHeaderLen ) {

            switch ( livestreamingparams.eStreamType ) {

            case isH263_1998:
            case isH263_2000:
            case isH263:
            case isMPEG4:
                livestreamingparams.pMPEG4params.pConfig = (uint8_t *)pSeqHeader;
                livestreamingparams.pMPEG4params.ulLen = uiSeqHeaderLen;
                break;

            case isH264_SVC:
            case isH264:

                livestreamingparams.pH264params.ucsSPSandPPS = (uint8_t *)pSeqHeader;
                livestreamingparams.pH264params.ulSPSandPPSlen = uiSeqHeaderLen;
                
                if (bIsDecode){
                    pDataConsumer->InitAVCDecoder(pOutName, pConfigName, isSilent);
                }
                

                break;

            case isVC1:
                break;

			default:;
            };
        }

        if ( m_pEngine->SetMediaDataConsumer( &livestreamingparams ) < 0 ) {

            SAFE_DELETE_ARRAY(LATMparams.pMuxConfigData);
            return ENGINE_GLOBAL_ERR;
        }

        SAFE_DELETE_ARRAY(LATMparams.pMuxConfigData);
    };

	SetTimeout(iTimeOut);

	m_State = engineReady;
    return ENGINE_SUCCESS;
}

int32_t CNetLiveClientEngine::StartLiveStreamingSession()
{
    CHECK_NET_ENGINE_PTR_RET(m_pEngine);

    CHECK_NET_ENGINE_ERR_CODE( m_pEngine->StartLiveStreamingSession() );

    return ENGINE_SUCCESS;
}

int32_t CNetLiveClientEngine::StopLiveStreamingSession( bool bIsHardStop /*= false*/ )
{
    CHECK_NET_ENGINE_PTR_RET(m_pEngine);

    CHECK_NET_ENGINE_ERR_CODE( m_pEngine->StopLiveStreamingSession() );

    return ENGINE_SUCCESS;
}

int32_t CNetLiveClientEngine::SetMediaDataConsumer( LIVESTREAMINGPARAMS *aParams, bool bRtcpShouldPresent /*= false*/ )
{
    CHECK_NET_ENGINE_PTR_RET(m_pEngine);

    CHECK_NET_ENGINE_ERR_CODE( m_pEngine->SetMediaDataConsumer(aParams, bRtcpShouldPresent) );

    return ENGINE_SUCCESS;
}

int32_t CNetLiveClientEngine::DeleteConsumer( IMediaConsumer *pConsumer )
{
    CHECK_NET_ENGINE_PTR_RET(m_pEngine);

    CHECK_NET_ENGINE_ERR_CODE( m_pEngine->DeleteConsumer(pConsumer) );

    list<IMediaConsumer *>::iterator iterator_lstConsumers = m_lstConsumers.begin();
    for ( ;iterator_lstConsumers != m_lstConsumers.end(); ++iterator_lstConsumers ) {

        IMediaConsumer *pDataConsumer = (IMediaConsumer *)*iterator_lstConsumers;
        if ( pDataConsumer == pConsumer ) {

            SAFE_DELETE(pDataConsumer);
            break;
        }        
    };

    return ENGINE_SUCCESS;
}

int32_t CNetLiveClientEngine::SetTimeout( int32_t aTimeOut /*= 0*/ )
{
    CHECK_NET_ENGINE_PTR_RET(m_pEngine);

    m_pEngine->SetTimeout();

    return ENGINE_SUCCESS;
}

STREAMTYPE CNetLiveClientEngine::GetMediaType( uint32_t aMediaIndex ) const
{
    return ( m_pEngine ) ? m_pEngine->GetStreamType( aMediaIndex ) : isUNKNOWN;
}

double CNetLiveClientEngine::GetDuration() const
{
    return ( m_pEngine ) ? m_pEngine->GetDuration() : 0;
}

int CNetLiveClientEngine::Run()
{
    CHECK_NET_ENGINE_PTR_RET(m_pEngine);

    CHECK_NET_ENGINE_ERR_CODE( m_pEngine->StartLiveStreamingSession() );
	m_State = engineActive;

    return ENGINE_SUCCESS;
}

int CNetLiveClientEngine::Stop()
{
    CHECK_NET_ENGINE_PTR_RET(m_pEngine);

    CHECK_NET_ENGINE_ERR_CODE( m_pEngine->StopLiveStreamingSession() );
	m_State = engineStopped;

    return ENGINE_SUCCESS;
}

const int8_t * CNetLiveClientEngine::StreamTypeToString( STREAMTYPE aType ) const
{
    switch ( aType ) {

        case isLATM:
            return (const int8_t *)"LATM";

        case isPCMU:
            return (const int8_t *)"wav";

        case isGSM:
            return (const int8_t *)"wav";

        case isG723:
            return (const int8_t *)"wav";

        case isDVI4:
            return (const int8_t *)"wav";

        case isLPC:
            return (const int8_t *)"wav";

        case isPCMA:
            return (const int8_t *)"wav";

        case isL8:
            return (const int8_t *)"wav";

        case isL16:
            return (const int8_t *)"wav";

        case isQCELP:
            return (const int8_t *)"wav";

        case isG728:
            return (const int8_t *)"wav";

        case isG722:
            return (const int8_t *)"wav";

        case isAAC:
            return (const int8_t *)"aac";

        case isH264_SVC:
            return (const int8_t *)"svc";

        case isH264:
            return (const int8_t *)"h264";

        case isJ2K:
            return (const int8_t *)"j2k";

		case isJPEG:
			return (const int8_t *)"mjpeg";

        case isMPEG4:
            return (const int8_t *)"mpeg4";

        case isH263_2000:
        case isH263_1998:
        case isH263:
            return (const int8_t *)"h263";

        case isAMR:
            return (const int8_t *)"amr";

        case isAMR_WB:
            return (const int8_t *)"amr";

        case isAC3:
            return (const int8_t *)"ac3";

        case isMPEG2ProgramStream:
            return (const int8_t *)"mp2p";

        case isMPEG1SystemStream:
            return (const int8_t *)"mp1s";

        case isMPEG2TransportStream:
            return (const int8_t *)"mp2t";

        case isMPEG1_2_Video:
            return (const int8_t *)"mp2v";

        case isMPEG1_2_Audio:
            return (const int8_t *)"mpa";

        case isMP3:
            return (const int8_t *)"mpa";

        case isVC1:
            return (const int8_t *)"vc1";

        case isOGG:
            return (const int8_t *)"ogg";

        default:
            return (const int8_t *)"unknown";
    };
}

int32_t CNetLiveClientEngine::ParseURL( int8_t *pUrl, PROTOCOL *pProto, int8_t *pServerAddress, uint32_t *uiPort )
{
    int32_t index = 0;
    if ( _strnicmp((const char *)pUrl, "udp://", 6) == 0) {

        (*pProto) = protocolUdp;
        index = 5;
    }

    if ( _strnicmp((const char *)pUrl, "rtp://", 5) == 0) {

        index = 5;
        (*pProto) = protocolRtpAvp;
    }

    if ( _strnicmp((const char *)pUrl, "tcp://", 5) == 0) {

        index = 5;
        (*pProto) = protocolTcp;
    }

    if ( index == 0 ) {
        return ENGINE_GLOBAL_ERR;
    }

    uint32_t index2 = 0;
    uint32_t index3 = 0;
    int8_t temp[16] = { 0 };

    index++;
    int32_t lenght = (int32_t)strlen((const char *)pUrl);

    if ( lenght <= index ) {
        return ENGINE_GLOBAL_ERR;
    }

    if ( pUrl[index] != '[' ) {

        while ( pUrl[index] != '/' && pUrl[index] != '\\' && pUrl[index] != '?' && pUrl[index] != ':' && index < (int32_t)strlen((const char *)pUrl) ) {
            pServerAddress[index3++] = pUrl[index++];
        };

        if ( pUrl[index] == ':' ) {
            index++;
        }

        while ( isdigit(pUrl[index]) && index < (int32_t)strlen((const char *)pUrl) ) {

            temp[index2++] = pUrl[index];
            index++;
        };

        if ( index2 == 0 ) {

            if ( pUrl[index] != '/' || pUrl[index] != '\\' ) {
                index = -1;
            }
        }
    } else {

        while ( pUrl[index] == '/' ) {	// ignore inet protocol declaration
            index++;
        };

		bool bInterfaceNameFound = 0;

        for ( ; (int32_t)index < (int32_t)strlen((const char *)pUrl); index++ ) {	// get server address string from source URL

            if ( pUrl[index] != '/' && pUrl[index] != '\\' && pUrl[index] != ']') {

                if ( isxdigit(pUrl[index]) || pUrl[index] == ':' || pUrl[index] == '[' || pUrl[index] == '.' || pUrl[index] == '%' ||
					(bInterfaceNameFound && isalnum(pUrl[index])) ) {

					if ( pUrl[index] == '%' ) {
						bInterfaceNameFound = 1;
					}

                    if ( pUrl[index] == '[' ) {
                        index++;
                    }

                    pServerAddress[index3] = pUrl[index];
                    index3++;
                }

            } else if ( pUrl[index] == ']' ) {

                while ( pUrl[index] != ':' && index < (int32_t)strlen((const char *)pUrl) ) { index++; }
                index++;

                while ( isdigit(pUrl[index]) && index < (int32_t)strlen((const char *)pUrl) ) {
                    temp[index2++] = pUrl[index++];
                };

                if ( index2 == 0 ) {

                    if ( pUrl[index] != '/' || pUrl[index] != '\\' ) {
                        index = -1;
                    }
                }

                break;
            }
        }
    }

    (*uiPort) = ( index == -1 ) ? 0 : atol((const char *)temp);
    
    return ( uiPort > 0 ) ? 0 : ENGINE_GLOBAL_ERR;
}

void CNetLiveClientEngine::OnDisconnect( MainConcept::NetworkStreaming::IMediaConsumer *pConsumer ) 
{
	m_iActiveConsumerCount--;
	if ( m_iActiveConsumerCount == 0 ) {

		m_State = engineStopped;
		//Don't call Stop() to avoid deadlock
		//Stop must be called from other thread
		printf("Playback Finished\n");
	}
}
