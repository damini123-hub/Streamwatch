/**
@file  net_source.h
@brief RTSP/Live streaming engines API

@verbatim
File: net_source.h
Desc: RTSP/Live streaming engines API

Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
This software is protected by copyright law and international treaties.  Unauthorized
reproduction or distribution of any portion is prohibited by law.
@endverbatim
**/

#if !defined (__NET_CLIENT_API_H__)
#define __NET_CLIENT_API_H__

#include "net_client.h"


class CMCSockets;

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
        * @brief MPEG4 decoder config
        **/
        typedef struct _tagMPEG4Params {
            uint8_t* pConfig;                                       /**< mpeg4 specific config */
            uint32_t ulLen;                                         /**< config length */
        } MPEG4PARAMS;

        /**
        * @brief AAC decoder config
        **/
        typedef struct _tagAACParams {

            uint32_t _uiSizeLength;                                 /**< number of bits on which the AU-size field is encoded in the AU-header */
            uint32_t _uiIndexLength;                                /**< number of bits on which the AU-index field is encoded in the AU-header */
            uint32_t _uiIndexDeltaLength;                           /**< number of bits on which the AU-index-delta field is encoded in the non-first AU-header */
            uint32_t _uiCTSDeltaLength;                             /**< number of bits on which the CTS-delta field is encoded in the AU-header */
            uint32_t _uiDTSDeltaLength;                             /**< number of bits on which the DTS-delta field is encoded in the AU-header */
            uint32_t _uiRAPIndication;                              /**< decimal value of zero or one, indicating whether the RAP-flag is present in the AU-header */
            uint32_t _uiStreamState;                                /**< number of bits on which the Stream-state field is encoded in the AU-header */
        } AACPARAMS;

        /**
        * @brief LATM decoder config
        **/
        typedef struct _tagLatmParams {

            bool     bConfigPresetInPayload;                        /**< indicates location of config, true - config present in stream, false - config appended externally */
            uint8_t* pMuxConfigData;                                /**< specific config */
            int32_t  iMuxConfigDataLen;                             /**< specific config size*/
        } LATMPARAMS;

        /**
        * @brief H264/AVC decoder config
        **/
        typedef struct _tagH264Params {

            uint8_t* ucsSPSandPPS;                                  /**< specifig config */
            uint32_t ulSPSandPPSlen;                                /**< config length */
        } H264PARAMS;

        /**
         * RTCP external SDP information
         **/
        typedef struct _tagRtcpExternalSdpInfo {

            uint8_t* ucsRtcpSrcAddress;                             /**< RTCP source address */
            uint32_t ulRtcpPort;                                    /**< RTCP port */
        } RTCPEXTERNALSDPINFO;

        /**
         * @brief Live RTP streaming parameters
         **/
        typedef struct _tagLiveStreamingParams {

            uint32_t        uiSampleRate;                           /**< a/v sample rate in Hz */
            IMediaConsumer* pMediaConsumer;                         /**< media data consumer */
            uint32_t        ulClientPort;                           /**< client port */
            uint8_t*        ucsNIC;                                 /**< local network interface */
            uint32_t        ulServerPort;                           /**< server port @deprecated not being use anymore*/
            int8_t*         csServerName;                           /**< server address */
            STREAMTYPE      eStreamType;                            /**< stream type */
            PROTOCOL        eProto;                                 /**< kind of transport protocol */
            H264PARAMS      pH264params;                            /**< H264/AVC parameters */
            MPEG4PARAMS     pMPEG4params;                           /**< MPEG4 parameter */
            AACPARAMS       pAACparams;                             /**< AAC parameters */
            LATMPARAMS      pLatmParams;                            /**< LATM parameters */
            uint32_t        uiPayloadType;                          /**< RTP payload type, use 0xff to ignore RTP payload type check */
            RTCPEXTERNALSDPINFO rtcpExternalSdpInfo;                /**< RTCP external SDP information, might be set when SDP contains RTCP media attribute */
        } LIVESTREAMINGPARAMS;

		/**
        * @brief RTSP RTP-Info
        **/
        typedef struct _tagMCNetSourceRtpInfo {

            int64_t i64Timestamp;                                   /**< RTP packet timestamp in 100ns units */
            uint16_t uiSeqNum;                                      /**< RTP Sequence number */
            int8_t* pUri;                                           /**< Absolut control URI for media stream */
        } MCNETSOURCERTPINFO;

		/**
        * @brief RTSP RTP-Info container
        **/
		typedef struct _tagMCNetSourceRtpInfoArray {
			MCNETSOURCERTPINFO *pInfo;								/**< Array of RTP-Info containers */
			uint32_t uiCount;										/**< Count of containers in pInfo array */
		} MCNETSOURCERTPINFOARRAY;

        typedef struct _tagMCNetRtcpPacket {

            RTCPPACKETTYPE _packetType;
            const int8_t *_pRawData;
            uint32_t _uiRawDataSize;
        } MCNETRTCPPACKET;

        /**
        * @brief Receives SDP documents from SAP Receiver API
        **/
        class ISapConsumer
        {
        public:
            virtual ~ISapConsumer() {}

            /**
            * @brief This method is used for notifying SAP API consumer
            *
            * @param[in] pSdp      SDP description in SAP packet
            * @param[in] bDeletion deletion marker on SAP packet
            * @return              None
            **/
            virtual void OnSap( const ISdpDecoder *pSdp, bool bDeletion ) = 0;
        };

        /**
        * @brief SAP announces listener (RFC 2974)
        **/
        class ISapReceiver : public INetRefCounted
        {
        public:
            virtual ~ISapReceiver() {}

            /**
            * @brief Subscribes consumer on SAP notifies
            *
            * @param[in] pbConsumer points to implementation of @ref ISapConsumer
            * @return               true on success otherwise false
            **/
            virtual bool Subscribe( ISapConsumer *pbConsumer ) = 0;

            /**
            * @brief With that method application may unsubscribe from SAP changes notify
            *
            * @param[in] pbConsumer pointer to removing consumer
            * @return               true on success else false
            **/
            virtual bool UnSubscribe( ISapConsumer *pbConsumer ) = 0;

            /**
            * @brief Sets local network interface for incoming SAP network traffic
            *
            * @param[in] pNIC used network interface
            * @return         true on success else false
            **/
            virtual bool SetNic( const int8_t *pNIC ) = 0;

            /**
            * @brief Sets SAP Network Addresses and ports to listen for
            *
            * @param[in] plstSapAddress list of SAP addresses for listening
            * @param[in] uiAddressCount count of addresses
            * @param[in] uiSapPort      port for listening
            * @return                   true on success else false
            **/
            virtual bool SetSapSettings( const int8_t **plstSapAddress, uint32_t uiAddressCount, uint32_t uiSapPort ) = 0;

            /**
            * @brief Starts listening SAP announces
            *
            * @return  true on success else false
            **/
            virtual bool Start() = 0;

            /**
            * @brief Stops listening SAP announces
            *
            * @return  true on success else false
            **/
            virtual bool Stop() = 0;
        };

        /**
        * @brief Application level media data consumer.
        *
        * Via that interface application will
        * receive multimedia data free from additional network protocol headers.
        * Via that interface application also will receive notifies from the LL engine.
        * With this interface application will provide memory buffers for LL engine.
        **/
        class IMediaConsumer
        {
        public:
            virtual ~IMediaConsumer() {}

        public:
            /**
            * @brief Requests buffer.
            *
            * Through that method engine requests memory block for media data
            *
            * @param[in,out] aInfo memory block descriptor
            * @return              None
            **/
            virtual void GetBuffer( CONSUMERINFO *aInfo ) = 0;

            /**
            * @brief With that method application receives multimedia data
            * with description
            *
            * @param[in] aInfo memory block descriptor
            * @return None
            **/
            virtual void OnData( CONSUMERINFO* aInfo ) = 0;

            /**
            * @brief With that method application receives notifies from LL engine
            *
            * @param[in] aNotifyType type of event
            * @param[in] pNotifyData additional data of event(if exists)
            * @return                None
            **/
            virtual void OnNotify( NETSOURCENOTIFYTYPE aNotifyType, void *pNotifyData = NULL ) = 0;

            /**
            * @brief Checks on available buffer
            *
            * @return true if buffer is available, else false
            **/
            virtual bool IsBufferAvailable() = 0;
        };

        /**
        * @brief HTTP client interface - suitable for HTTP Progressive download 1.x
        **/
        class IHttpClient : public INetRefCounted
        {
        public:
            virtual ~IHttpClient() {}

            /**
            * @brief Initializes engine
            *
            * @param[in] ServerUrl  complete server URL
            * @param[in] aNIC local network interface to use, might be NULL for system default interface
			* @param[in] bAutoplay  Start feeding data immediately
            * @return               Returns @ref MCNetResultOK on success and error code otherwise
            **/
            virtual MCNETRESULT Init( const int8_t *ServerUrl, const int8_t *aNIC = NULL, bool bAutoplay = false ) = 0;

            /**
            * @brief Requests data from server(synchronously)
            *
			* @param[in] i64StartRange byte offset start of chunk of data or time in milliseconds when bSeekByTime is true
			* @param[in] i64StopRange  byte offset end of chunk of data or time in milliseconds when bSeekByTime is true
			* @param[in] bSeekByTime   when true - i64StartRange and i64StopRange represent time offsets in milliseconds
            * @return                  Returns @ref MCNetResultOK on success and error code otherwise
            **/
            virtual MCNETRESULT Play( int64_t i64StartRange, int64_t i64StopRange, bool bSeekByTime ) = 0;

            /**
            * @brief Requests data from server(asynchronously)
            *
            * @param[in] i64StartRange byte offset start of chunk of data or time in milliseconds when bSeekByTime is true
            * @param[in] i64StopRange  byte offset end of chunk of data or time in milliseconds when bSeekByTime is true
			* @param[in] bSeekByTime   when true - i64StartRange and i64StopRange represent time offsets in milliseconds
            * @return                  Returns @ref MCNetResultOK on success and error code otherwise
            **/
            virtual MCNETRESULT PlayAsync( int64_t i64StartRange, int64_t i64StopRange, bool bSeekByTime ) = 0;

			/**
            * @brief Suspend data feeding. Operation is asynchronous when bSuspend is false.
			* Note that server might disconnect if engine is suspended for too long.
            *
            * @param[in] bSuspend      true to suspend engine and false to return it to playing state
            * @return                  Returns @ref MCNetResultOK on success and error code otherwise
            **/
            virtual MCNETRESULT Pause( bool bSuspend ) = 0;

            /**
            * @brief Stops engine.
            *
            * Method blocks until engine is stopped.
            *
            * @return Returns @ref MCNetResultOK on success and error code otherwise
            **/
            virtual MCNETRESULT Stop()  = 0;

            /**
            * @brief Stops engine.
            *
            * Method stops engine asynchronously without blocking
            *
            * @return Returns @ref MCNetResultOK on success and error code otherwise
            **/
            virtual MCNETRESULT StopAsync() = 0;

            /**
            * @brief Sets up external multimedia data consumer
            *
            * @param[in] pMediaConsumer pointer to implementation of @ref IMediaConsumer
            * @return                   Returns @ref MCNetResultOK on success and error code otherwise
            **/
            virtual MCNETRESULT SetMediaDataConsumer( IMediaConsumer *pMediaConsumer ) = 0;

            /**
            * @brief Setup authorization credential
            *
            * @param[in] aParameters pointer to @ref AUTHORIZATIONFIELD
            * @return                Returns @ref MCNetResultOK on success and error code otherwise
            **/
            virtual MCNETRESULT SetAuthorization( AUTHORIZATIONFIELD *aParameters ) = 0;

            /**
            * @brief Setup user agent string
            *
            * @param[in] aBuff pointer to zero-terminated user agent string
            * @return          Returns @ref MCNetResultOK on success and error code otherwise
            **/
            virtual MCNETRESULT SetUserAgent( const int8_t *aBuff ) = 0;

            /**
            * @brief Setup proxy URL
            *
            * @param[in] pProxyURL pointer to zero-terminated proxy URL
            * @return              Returns @ref MCNetResultOK on success and error code otherwise
            **/
            virtual MCNETRESULT SetProxyURL( const int8_t *pProxyURL ) = 0;

            /**
            * @brief Retrieves Content-Length value
            *
            * @return Returns size of content in bytes
            **/
            virtual int64_t GetContentLen() const = 0;

			/**
            * @brief Retrieves stream duration in seconds if available
            *
            * @return Returns duration of stream in seconds
            **/
            virtual double GetDuration() const = 0;

            /**
            * @brief Retrieves Stream type
            *
            * @return Returns @ref STREAMTYPE
            **/
            virtual STREAMTYPE GetStreamType() const = 0;

            /**
            * @brief Retrieves engine state
            *
            * @return Returns @ref MCNETENGINESTATUS
            **/
            virtual MCNETENGINESTATUS GetState() const = 0;

            /**
            * @brief Retrieves server HTTP response code
            *
            * @return Returns response code
            **/
            virtual int32_t GetServerErrorCode() const = 0;

            /**
            * @brief Setup network timeout
            *
            * @param aTimeOut timeout value in seconds
            **/
            virtual void SetTimeout( int32_t aTimeOut = 0 ) = 0;

            /**
            * @brief Retrieves media data MIME string
            *
            * @return Returns zero-terminated MIME string;
            **/
            virtual const int8_t * GetMimeTypeStringValue() const = 0;

            /**
            * @brief Retrieves server data range
            *
            * @param[out] pi64RangeStart start range
            * @param[out] pI64RangeEnd   end range
            * @return                    Returns true if server support Range header
            **/
            virtual bool GetServerRange( int64_t *pi64RangeStart = NULL, int64_t *pI64RangeEnd = NULL ) = 0;

			/**
            * @brief Retrieves stream time range
            *
            * @param[out] pfRangeStart    start range in seconds
            * @param[out] pfRangeEnd      end range in seconds
            * @return                    Returns true if server support TimeSeekRange.dlna.org header
            **/
            virtual bool GetServerTimeRange( double *pfRangeStart = NULL, double *pfRangeEnd = NULL ) = 0;

        };

        /**
        * @brief Declares RTSP Observer interface
        **/
        class IRtspObserver
        {
        public:
            virtual ~IRtspObserver() {}

            /**
            * @brief This method is callback for RTSP REDIRECT command from server
            *
            * @param[in] pUrl    redirect address
            * @param[in] iUrlLen length of pUrl
            * @param[in] pRange  value of Range header from REDIRECT request
            * @return None
            **/
            virtual void OnRedirect(const int8_t* pUrl, int32_t iUrlLen, const int8_t* pRange = NULL) = 0;
        };

        /**
        * @brief RTSP client engine interface - suitable for Video on Demand (RFC 2326)
        **/
        class IRtspClient : public INetRefCounted
        {
        public:
            virtual ~IRtspClient() {}

            /**
            * @brief Initializes engine
            *
            * @param[in] aServerName address of RTSP server
            * @param[in] aPort       remote port
            * @param[in] aUrl        full specified requested media URL
            * @param[in] aNIC        local network interface to use
            * @return                Return @ref MCNETRESULT
            **/
            virtual MCNETRESULT Init( const int8_t *aServerName, uint32_t aPort, const int8_t *aUrl, const int8_t *aNIC = NULL ) = 0;

            /**
            * @brief Starts playback (RTSP PLAY)
            *
            * @param[in] aStartTime start position in NPT string format
            * @param[in] aEndTime   end playback position in NPT string format
            * @param[in] aRate      rate of playback
            * @return               Return @ref MCNETRESULT
            **/
            virtual MCNETRESULT Play( const int8_t *aStartTime = NULL, const int8_t *aEndTime = NULL, const int8_t *aRate = NULL ) = 0;

            /**
            * @brief This method is suitable for VCR functionality (RTSP RECORD)
            *
            * @return Return @ref MCNETRESULT
            **/
            virtual MCNETRESULT Record()      = 0;

            /**
            * @brief Pauses playback (RTSP PAUSE)
            *
            * @return Return MCNETRESULT
            **/
            virtual MCNETRESULT Pause()      = 0;

            /**
            * @brief Stops client (Sends RTSP PAUSE request and cleanup all pending requests)
            *
            * @return Return MCNETRESULT
            **/
            virtual MCNETRESULT Stop()     = 0;

            /**
            * @brief Sends RTSP OPTIONS request
            *
            * @param[in] pUserData        pointer to custom data, which will be send in request
            * @param[in] uiUserDataLength size of custom user data
            * @return                     Return @ref MCNETRESULT
            **/
            virtual MCNETRESULT Options( const int8_t *pUserData = NULL, uint32_t uiUserDataLength = 0 )  = 0;

            /**
            * @brief Sends RTSP DESCRIBE request
            *
            * It is used for getting media and connection description from server
            *
            * @param[in] aURI requested media URI
            * @return         Return @ref MCNETRESULT
            **/
            virtual MCNETRESULT Describe( const int8_t *aURI ) = 0;

            /**
            * @brief Sends RTSP ANNOUNCE command to server
            *
            * @param[in] aURI       specified media URI
            * @param[in] aSDPpacket description of media presentation
            * @return               Return @ref MCNETRESULT
            **/
            virtual MCNETRESULT Announce( const int8_t *aURI, uint8_t *aSDPpacket ) = 0;

            /**
            * @brief Sends RTSP SET_PARAMETER request
            *
            * @param[in] pParam         name of parameter
            * @param[in] iParamLen      length of parameter name
            * @param[in] pParamValue    pParam value of parameter
            * @param[in] iParamValueLen length of value
            * @param[in] pContentType   content MIME type
            * @return                   Return @ref MCNETRESULT
            **/
            virtual MCNETRESULT SetParameter( const int8_t *pParam, int32_t iParamLen, const int8_t *pParamValue, int32_t iParamValueLen, const int8_t *pContentType = NULL ) = 0;

            /**
            * @brief Sends RTSP GET_PARAMETER request
            *
            * @param[in]  pParam         name of requested parameter
            * @param[in]  iParamLen      length of parameter name
            * @param[out] pParamValue    value of parameter
            * @param[out] iParamValueLen length of value
            * @return                    Return @ref MCNETRESULT
            **/
            virtual MCNETRESULT GetParameter( const int8_t *pParam, int32_t iParamLen, const int8_t **pParamValue, int32_t* iParamValueLen ) = 0;

            /**
            * @brief Sets media data consumer to engine
            *
            * @param[in] pMediaConsumer           pointer to implementation of IMediaConsumer
            * @param[in] nMediaID                 specifies media stream
            * @param[in] csServerMulticastAddress address for multicast connection to media stream
            * @param[in] bRtcpShouldPresent       enables/disables RTCP notifies
            * @return                             Return @ref MCNETRESULT
            **/
            virtual MCNETRESULT SetMediaDataConsumer( IMediaConsumer *pMediaConsumer, uint32_t nMediaID, const int8_t *csServerMulticastAddress = NULL, bool bRtcpShouldPresent = false ) = 0;

            /**
            * @brief Sets media data consumer to engine
            *
            * @param[in] pMediaConsumer           pointer to implementation of IMediaConsumer
            * @param[in] pMediaInfo               describe media stream
            * @param[in] csServerMulticastAddress address for multicast connection to media stream
            * @param[in] bRtcpShouldPresent       enables/disables RTCP notifies
            * @return                             Return @ref MCNETRESULT
            **/
            virtual MCNETRESULT SetMediaDataConsumer( IMediaConsumer *pMediaConsumer, const ISdpMediaInfo *pMediaInfo, const int8_t *csServerMulticastAddress = NULL, bool bRtcpShouldPresent = false ) = 0;

            /**
            * @brief Sets authorization data
            *
            * @param[in] aParameters pointer to @ref AUTHORIZATIONFIELD struct
            * @return                Return @ref MCNETRESULT
            **/
            virtual MCNETRESULT SetAuthorization( AUTHORIZATIONFIELD *aParameters ) = 0;

            /**
            * @brief Removes consumer from engine
            *
            * @param[in] pConsumer specified consumer for removing
            * @return              Return @ref MCNETRESULT
            **/
            virtual MCNETRESULT DeleteConsumer( IMediaConsumer *pConsumer ) = 0;

            /**
            * @brief Sets network timeout
            *
            * @param[in] aTimeOut timeout in milliseconds or 0 for infinity waiting
            * @return             None
            **/
            virtual void SetTimeout( int32_t aTimeOut = 0 ) = 0;

            /**
            * @brief Closes connection (RTSP TEARDOWN)
            *
            * @return None
            **/
            virtual void CloseConnection() = 0;

            /**
            * @brief Retrieves media type
            *
            * @param[in] index specifies media stream
            * @return          Returns @ref STREAMTYPE value
            **/
            virtual STREAMTYPE GetStreamType( uint32_t index ) const = 0;

            /**
            * @brief Retrieves media stream duration
            *
            * @return duration at seconds
            **/
            virtual double GetDuration() const = 0;

            /**
            * @brief Retrieves last RTP packet timestamp
            *
            * @return last packet timestamp at 100-nanosecond units
            **/
            virtual int64_t GetLastServerTime() const = 0;

            /**
            * @brief Sets preferable transport protocol
            *
            * @param[in] aMode specifies transport protocol options
            * @return          None
            **/
            virtual void SetProtocolType( CLIENTPROTOCOLTYPE aMode = UnknownMode ) = 0;

            /**
            * @brief Receives current transport protocol options
            *
            * @return Returns @ref CLIENTPROTOCOLTYPE
            **/
            virtual CLIENTPROTOCOLTYPE GetProtocolType() const = 0;

            /**
            * @brief Sets first available port for media data receiver
            *
            * @param[in] aPort specifies first available port
            * @return          None
            **/
            virtual void SetFirstAvailablePort( uint32_t aPort = 10200 ) = 0;

            /**
            * @brief Retrieves server name
            *
            * @return Returns @ref RTSPSERVERNAME
            **/
            virtual RTSPSERVERNAME GetServerName() const = 0;

            /**
            * @brief Receives engine state
            *
            * @return Returns @ref MCNETENGINESTATUS
            **/
            virtual MCNETENGINESTATUS GetState() const = 0;

            /**
            * @brief Checks on SDP presence
            *
            * @return true if SDP is exist and available
            **/
            virtual bool IsConfiguratedBySdp() const = 0;

            /**
            * @brief Receives specific server config (SDP)
            *
            * @param[out] pDatalen length of server config
            * @return              pointer to server specific config
            **/
            virtual const int8_t * GetSpecificServerConfig( int32_t *pDatalen ) const = 0;

            /**
            * @brief Provides SDP Document received from Server on DESCRIBE
            *
            * @return pointer to @ref ISdpDecoder with sdp media description
            **/
            virtual const ISdpDecoder * GetSdpDocument() const = 0;

            /**
            * @brief Retrieves last server RTSP response code
            *
            * @return last server response code
            **/
            virtual int32_t GetServerErrorCode() const = 0;

            /**
            * @brief Sets user-agent string
            *
            * @param[in] aBuff custom user-agent string
            * @return          Return @ref MCNETRESULT
            **/
            virtual MCNETRESULT SetUserAgent( const int8_t *aBuff ) = 0;

            /**
            * @brief Sets multicast source specific address.
            *
			* Only packets with specified source address will be received.
			*
            * @param[in] aBuff specific source address
            * @return          None
            **/
            virtual void SetSpecificSourceAddress( const int8_t *aBuff ) = 0;

            /**
            * @brief Sets external observer
            *
            * @param[in] pObserver pointer to implementation of @ref IRtspObserver
            * @return              None
            **/
            virtual void SetRtspObserver(const IRtspObserver* pObserver) = 0;

            /**
            * @brief Enables/disables SSRC changes tracking
            *
			* Setting this to false will disable ESyncSourceChanged notifies on data loss,
			* EGapInData will be send instead.
			*
            * @param[in] bState true - enables tracking, false - disables
            * @return           None
            **/
            virtual void SetSSRCChangesTracking( bool bState ) = 0;

			/**
            * @brief Returns array of RTP-Info structures for streams
            *
            * @return           Returns @ref MCNETSOURCERTPINFOARRAY
            **/
			virtual const MCNETSOURCERTPINFOARRAY *GetRtpInfo() = 0;

			/**
            * @brief Enables/disables UDP packet reordering
            *
			* UDP packets may be received out of order, UDP packet reordering is used to compensate this.
			* This option allows application to enable or disable internal reordering algorithm.
			* Enabled by default for UDP/RTP.
			*
            * @param[in] bEnable true - enables UDP packet reordering, false - disables
            * @return           None
            **/
            virtual void EnableReorderingHandling( bool bEnable ) = 0;

            /**
            * @brief Sets KeepAlive message format
			*
            * @param[in] aMode specifies RTSP message to keep connection alive
            * @return          None
            **/
            virtual void SetKeepAliveMode( RtspKeepAliveMode aMode ) = 0;

            /**
            * @brief Determines should client use RFC notation for "Transport" header
            *
            * Normally (conforming RFC 2326) invoking SETUP request client should use
            * "port=" to specify its destination port in multicast receiving mode and
            * "client_port=" if unicast mode is used. But some servers accept only
            * "client_port=" notation disregarding RFC. Disable this option to provide
            * compatibility with such servers.
			*
            * @param[in] bEnable true - always conform to RFC 2326. This is the default value.
            * @return          None
            **/
            virtual void UseHeadersRFCConforming( bool bEnable ) = 0;

			 /**
            * @brief Retrieves server's rate for the media playback
			*
            * @return          Server side rate
            **/
            virtual double GetRate() = 0;
        };

        /**
        * @brief Live Streaming (RTP) client interface
        **/
        class INetworkSource : public INetRefCounted
        {
        public:
            virtual ~INetworkSource() {}

            /**
            * @brief Starts receiving data
            *
            * @return Return @ref MCNETRESULT
            **/
            virtual MCNETRESULT StartLiveStreamingSession() = 0;

            /**
            * @brief Stops receiving data
            *
            * @param[in] bIsHardStop stop working with reset all internal parameters
            * @return                Return @ref MCNETRESULT
            **/
            virtual MCNETRESULT StopLiveStreamingSession( bool bIsHardStop = false ) = 0;

            /**
            * @brief Adds media data consumer to engine
            *
            * @param[in] aParams            settings of consumer
            * @param[in] bRtcpShouldPresent enables RTCP if true
            * @return                       Return @ref MCNETRESULT
            **/
            virtual MCNETRESULT SetMediaDataConsumer( LIVESTREAMINGPARAMS *aParams, bool bRtcpShouldPresent = false ) = 0;

            /**
            * @brief Removes data consumer from engine
            *
            * @param[in] pConsumer consumer for removing
            * @return              Return @ref MCNETRESULT
            **/
            virtual MCNETRESULT DeleteConsumer( IMediaConsumer *pConsumer ) = 0;

            /**
            * @brief Sets network timeout
            *
            * @param[in] aTimeOut timeout in seconds or 0 for infinity waiting
            **/
            virtual void SetTimeout( int32_t aTimeOut = 0 ) = 0;

            /**
            * @brief Receives stream type
            *
            * @param[in] aMediaIndex index of media stream
            * @return                Returns @ref STREAMTYPE
            **/
            virtual STREAMTYPE GetStreamType( uint32_t aMediaIndex ) const = 0;

            /**
            * @brief Receives stream duration
            *
            * @return duration in seconds
            **/
            virtual double GetDuration() const = 0;

            /**
            * @brief Sets multicast source specific address.
            *
			* Only packets with specified source address will be received.
			*
            * @param[in] aBuff specific source address
            * @return          None
            **/
            virtual void SetSpecificSourceAddress( const int8_t *aBuff ) = 0;

            /**
            * @brief Enables/disables SSRC changes tracking
            *
			* Setting this to false will disable ESyncSourceChanged notifies on data loss,
			* EGapInData will be send instead.
			*
            * @param[in] bState true - enables tracking, false - disables
            * @return           None
            **/
            virtual void SetSSRCChangesTracking( bool bState ) = 0;

			/**
            * @brief Enables/disables UDP packet reordering
            *
			* UDP packets may be received out of order, UDP packet reordering is used to compensate this.
			* This option allows application to enable or disable internal reordering algorithm.
			* Enabled by default for UDP/RTP.
			*
            * @param[in] bEnable true - enables UDP packet reordering, false - disables
            * @return           None
            **/
            virtual void EnableReorderingHandling( bool bEnable ) = 0;

            /**
            * @brief Performs replacing receive socket on shared socket from another application
            *
			* UDP packets may be received on socket which is used by NetRenderer for sending them.
			* It is necessary to get Shared Socket instance from NetRenderer(@see ISharedSocketInfo) and replace
			* inside current INetworkSource for each consumer.
			*
            * @param[in] pConsumer     - consumer instance which will have shared socket instead of own socket
            * @param[in] pSharedSocket - shared socket instance getted from NetRenderer
            * @return           MCNETRESULT
            **/
            virtual MCNETRESULT ReplaceSocket( IMediaConsumer *pConsumer, CMCSockets* pSharedSocket ) = 0;
        };

#if defined (__cplusplus)
        extern "C" {
#endif

            /**
            * @brief Initializes @ref CONSUMERINFO struct
            *
            * @param[in,out] pConsumer   struct @ref CONSUMERINFO
            * @param[in] aContext    pointer to object contains info about one media sample
            * @param[in] aMaxBuffLen max buffer length
            * @return                None
            **/
            void NET_API_CALL InitConsumerInfo( CONSUMERINFO *pConsumer, void *aContext, uint32_t aMaxBuffLen );

            /**
            * @brief Clears @ref CONSUMERINFO struct
            *
            * @param[in,out] pConsumer struct @ref CONSUMERINFO
            * @return                  None
            **/
            void NET_API_CALL ResetConsumerInfo( CONSUMERINFO *pConsumer );

            /**
            * @brief This method creates an instance of RTSP client engine
            *
            * @param[in] get_rc Pointer to get_rc memory manager implementation or NULL to use standard allocators
            * @return           Returns valid pointer to @ref IRtspClient on success and NULL when failed
            **/
            IRtspClient*    NET_API_CALL GetRTSPClientInterface( void *(MC_EXPORT_API *get_rc)(const char*) );

            /**
            * @brief Creates an instance of RTP(live) client engine
            *
            * @param[in] get_rc Pointer to get_rc memory manager implementation or NULL to use standard allocators
            * @return           Returns valid pointer to @ref INetworkSource on success and NULL when failed
            **/
            INetworkSource* NET_API_CALL GetRTPClientInterface( void *(MC_EXPORT_API *get_rc)(const char*) );

            /**
            * @brief Creates an instance of HTTP client engine
            *
            * @param[in] get_rc Pointer to get_rc memory manager implementation or NULL to use standard allocators
            *
            * @return           Returns valid pointer to @ref IHttpClient on success and NULL when failed
            **/
            IHttpClient * NET_API_CALL GetHttpClientInterface( void *(MC_EXPORT_API *get_rc)(const char*) );

            /**
            * @brief Creates an instance of SAP receiver
            *
            * @param[in] get_rc Pointer to get_rc memory manager implementation or NULL to use standard allocators
            * @return           Returns valid pointer to @ref ISapReceiver on success and NULL when failed
            **/
            ISapReceiver * NET_API_CALL GetSapReceiverInterface( void *(MC_EXPORT_API *get_rc)(const char*) );

            /**
            * @brief Receives extended API of NetSource module
            *
            * @param[in] func id of requested function
            * @return         Returns valid pointer to function else NULL
            **/
            APIEXTFUNC NET_API_CALL NetSourceGetAPIExt(uint32_t func);
#if defined (__cplusplus)
        }
#endif
    };
};

#endif // __NET_CLIENT_API_H__
