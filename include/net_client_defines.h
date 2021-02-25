/**
@file  net_client_defines.h
@brief Unified network streaming client API common defines

@verbatim
File: net_client_defines.h
Desc: Unified network streaming client API common defines

Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
This software is protected by copyright law and international treaties.  Unauthorized
reproduction or distribution of any portion is prohibited by law.
@endverbatim
**/

#if !defined (__MC_NET_CLIENT_DEFINES_H__)
#define __MC_NET_CLIENT_DEFINES_H__

#include "mctypes.h"
#include "net_common.h"
#include "parser_sdp_defines.h"

/**
* @brief Default size of memory block for client response
* @hideinitializer
**/
#define RESP_BUFF_SIZE 0x10000 // 65536b

#include "mcapiext.h"


#if !defined(DEFAULT_RTSP_PORT)
/**
* @def DEFAULT_RTSP_PORT
* @brief RTSP standart port
* @hideinitializer
**/
    #define DEFAULT_RTSP_PORT 554
#endif

#if !defined(DEFAULT_HTTP_PORT)
/**
* @def DEFAULT_HTTP_PORT
* @brief HTTP stansart port
* @hideinitializer
**/
    #define DEFAULT_HTTP_PORT 80
#endif

#if !defined(DEFAULT_HTTPS_PORT)
/**
* @def DEFAULT_HTTPS_PORT
* @brief HTTPS stansart port
* @hideinitializer
**/
    #define DEFAULT_HTTPS_PORT 443
#endif

/**
* namespace MainConcept
* @brief Global SDK namespace
**/
namespace MainConcept {

    /**
    * namespace NetworkStreaming
    * @brief Network streaming specific namespace
    **/
    namespace NetworkStreaming {

        /**
            @brief Describes RTSP method to use with Keep-alive request
        */
        typedef enum _tagRtspKeepAliveMode {

            RtspKeepAliveNone,                                      /**< Without keep-alive request */
            RtspKeepAliveOptions,                                   /**< Use RTSP OPTIONS command as keep-alive request, default value */
            RtspKeepAliveSetParameter,                              /**< Use RTSP SET_PARAMETER command as keep-alive request */
            RtspKeepAliveGetParameter                               /**< Use RTSP GET_PARAMETER command as keep-alive request */
        } RtspKeepAliveMode;

        /**
        * @brief Known RTSP server's type
        **/
        typedef enum  _tagServerTypeByName {
            ElecardRTSPServer,                                      /**< Elecard RTSP Server */
            WMServer,                                               /**< Windows Media Server */
            DarwinServer,                                           /**< Darwin Streaming Server */
            KasennaServer,                                          /**< Kasenna Server */
            SeaChange,                                              /**< SeaChange */
            RealMedia,                                              /**< RealMedia Server */
            Mango,                                                  /**< Mango */
			Wowza,													/**< Wowza */
            RTSP_RFC2326      = 0xffff                              /**< Server is compatible with RFC2326(RTSP) */
        } RTSPSERVERNAME;

        /**
        * @brief Stores authorization credentials
        **/
        typedef struct _tagAuthorizationField {

            int8_t* pUserName;                                      /**< zero-terminated string user name */
            int8_t* pPassword;                                      /**< zero-terminated string password */
        } AUTHORIZATIONFIELD;

        /**
        * @brief Predefined Protocol/Profile/LowerTransport connectivity mode sets
        **/
        typedef enum _tagClientProtocolType {
            Mp2tAvpUdpUnicastMode,                                  /**< MP2T/AVP/UDP Unicast */
            Mp2tAvpUdpMulticastMode,                                /**< MP2T/AVP/UDP Multicast */
            Mp2tAvpTcpUnicastMode,                                  /**< MP2T/AVP/TCP Unicast */
            UnicastRtpAvpUdpMode,                                   /**< RTP/AVP/UDP Unicast */
            MulticastRtpAvpMode,                                    /**< RTP/AVP/UDP Multicast */
            MulticastRawMode,                                       /**< RAW/RAW/UDP Multicast*/
            InterleavingMode,                                       /**< interleaving */
            UnicastRawUdpMode,                                      /**< RAW/RAW/UDP Unicast */
            UnicastRawTcpMode,                                      /**< RAW/RAW/TCP Unicast */
            UnknownMode             = 0xffff                        /**< Unknown mode */
        } CLIENTPROTOCOLTYPE;

        class IMediaConsumer;

        /**
        * @brief Describes media sample.
        *
        * Recommended buffer size is @ref RESP_BUFF_SIZE or greater, for example RESP_BUFF_SIZE * 2,
        * count of buffers: min = 1, max = infinity (depending on actual RAM size).
        * void *pContext - this pointer should be used for storing pointers on an
        * objects with information about one sample, for example IMediaSample object.
        **/
        typedef struct _tagConsumerInfo {

            bool       bIsValidSample;                              /**< indicates that buffer is recognized as a/v sample */
            bool       bIsMarker;                                   /**< marker of sample's end */

            uint32_t   ulCurrentPacketNumber;                       /**< current packet number */
            uint32_t   ulCyclesCount;                               /**< wrap count of packet number  */
            uint32_t   dwPrefixLen;                                 /**< offset of data start */
            uint8_t*   pData;                                       /**< pointer to media data */
            uint8_t*   pInitPointer;                                /**< pointer to begin of buffer */
            uint32_t   dwBufferLen;                                 /**< buffer size */
            uint32_t   dwActualLen;                                 /**< actual data size */

            int64_t    tStart;                                      /**< start sample timestamp */
            int64_t    tStop;                                       /**< stop sample timestamp */

            STREAMTYPE iMediaType;                                  /**< stream type */
            void*      pContext;                                    /**< application can use this field
                                                                    to store additional information on sample */

            uint32_t   dwDecoderConfigSize;                         /**< indicates count of bytes if buffer starts
                                                                    with decoder config */
            int64_t    tPacketReceiveTime;                          /**< time in milliseconds when packet has arrived to reciever. */

            bool       bDontSend;                                   /**< if this is set, then current OnData call is a
                                                                    notify that data was received and dwActualLen
                                                                    is updated. Application must not modify or delete
                                                                    buffer at this time. */
            bool       bDiscontinuity;                              /**< when true - indicates discontinuity in stream */
            bool       bNewSegment;                                 /**< when true - indicates that timing should be reset */
			bool	   bOutOfBand;									/**< when true - indicates that sample should be sent immediately without buffering (used for decoder config) */
			bool       bSyncPoint;									/**< when true - indicates that this sample's time could be used for clock adjustment */
        } CONSUMERINFO;

        /**                                                          
        * @brief Segment info structure                             
        **/                                                          
        typedef struct _tagNetSourceSegmentInfo {

            uint64_t _SizeInBytes;
            uint64_t _DurationInMilliSeconds;
        } NETSOURCESEGMENTINFO;

        /**                                                          
        * @brief Network source engine notifies                             
        **/                                                          
        typedef enum _tagNetSourceNotifyType {                       

            EJitterValue,                                           /**< Jitter value, argument: int32_t RTP jitter value */
            EGapInData,                                             /**< Gap in data, argument: uint32_t dropped packet count */
            ESyncSourceChanged,                                     /**< SSRC changed, argument: uint32_t new SSRC value */
            EDisconnect,                                            /**< Disconnect, argument: none */
            EKeepAliveValue,                                        /**< KeepAlive timeout, argument: uint32_t session timeout value */
            ETimeOut,                                               /**< Network timeout, argument: none */
            EEndOfStream,                                           /**< End of Stream reached, argument: none */
            EBufferingDone,                                         /**< Buffering is finished, argument: none */
			ESegmentDone,											/**< Finished loading stream segment, argument: @ref NETSOURCESEGMENTINFO */
			ENetworkBytesReceived,									/**< Received bytes from network, argument: int32_t count of bytes */
            ERtcpPacket,                                            /**< RAW RTCP Packet Data */
            ENotifiesMax       = 0x7FFFFFFF                         /**< Max notify value */
        } NETSOURCENOTIFYTYPE;
    };
};

#endif //__MC_NET_CLIENT_DEFINES_H__
