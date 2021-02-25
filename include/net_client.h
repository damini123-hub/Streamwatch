/**
@file  net_client.h
@brief Unified network streaming client API

@verbatim
File: net_client.h
Desc: Unified network streaming client API

Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
This software is protected by copyright law and international treaties.  Unauthorized
reproduction or distribution of any portion is prohibited by law.
@endverbatim
**/

#if !defined (__MC_UNIFIED_NET_CLIENT_API_H__)
#define __MC_UNIFIED_NET_CLIENT_API_H__

#include "net_client_defines.h"
#include "net_playlist.h"
#include "parser_sdp.h"

/**
 * namespace MainConcept
 * @brief Global SDK namespace
 **/
namespace MainConcept {

    /**
     * namespace NetworkStreaming
     * Network streaming specific namespace
     **/
    namespace NetworkStreaming {

		/**
		* @brief Unified client events for asynchronous mode
		**/
		typedef enum _tagNetClientEvents {

			MCNetClientEventPlay	= 0,							/**< Playback was started (associated data: operation result code, type: @ref MCNETRESULT) */
			MCNetClientEventPause	= 1,							/**< Playback was paused (associated data: operation result code, type: @ref MCNETRESULT) */
			MCNetClientEventStop	= 2								/**< Playback was stopped (associated data: operation result code, type: @ref MCNETRESULT) */
		} MCNETCLIENTEVENT;

        /**
         * @brief Type of low level engine
         **/
        typedef enum _tagMCNetClientEngineType {

            MCNetClientEngineRtp       = 0,                         /**< Live client */
            MCNetClientEngineRtsp      = 1,                         /**< Rtsp client */
            MCNetClientEngineRtmp      = 2,                         /**< Rtmp client */
            MCNetClientEngineHttp      = 3,                         /**< Http client */
			MCNetClientEngineAsync      = 4,                         /**< Pull source */
            MCNetClientEngineUndefined = -1                         /**< Undefined/unininitialized */
        } MCNETCLIENTENGINETYPE;

		/**
         * @brief Type of Http stream
         **/
		typedef enum _tagNetClientStreamingType {

			MCNetClientStreamingTypeRegular		= 0,                /**< Regular HTTP file */
			MCNetClientStreamingTypeHLS			= 1,                /**< HTTP Live streaming */
			MCNetClientStreamingTypeSmooth		= 2,                /**< Microsoft Smooth Streaming */
			MCNetClientStreamingTypeYoutube		= 3,                /**< Youtube link */
			MCNetClientStreamingTypePlaylist	= 4,                /**< Playlist file on HTTP server */
			MCNetClientStreamingTypeSDP			= 5,                /**< SDP file on HTTP server */
			MCNetClientStreamingTypeDSAS		= 6,                /**< DivX Adaptive Streaming */
            MCNetClientStreamingTypeDash		= 7,                /**< DASH */
		} MCNETCLIENTSTREAMINGTYPE;


        /**
         * @brief Unified client parameters
         **/
        typedef enum _tagMCNetClientParamType {

            MCNetClientParamDuration                 = 0,           /**< type double: seconds.milliseconds */
            MCNetClientParamSpecificSourceAddr       = 1,           /**< type int8_t*: 0.0.0.0 or NULL for disabling, correct address for enabling SSM */
            MCNetClientParamSSRCChangesTrackingState = 2,           /**< type bool: true - for enabling SSRC changes tracking, false - for disabling */
            MCNetClientParamTimeout                  = 3,           /**< type uint32_t: seconds */
            MCNetClientParamRtspProtocol             = 4,           /**< type CLIENTPROTOCOLTYPE: RTSP media protocol type */
            MCNetClientParamServerTimeStamp          = 5,           /**< type double: seconds.milliseconds */
            MCNetClientParamRtspServerName           = 6,           /**< type RTSPSERVERNAME: RTSP server name/type */
            MCNetClientParamCurrentState             = 7,           /**< type ENGINESTATUS: current engine status */
            MCNetClientParamServerErrCode            = 8,           /**< type int32_t: last server error code */
            MCNetClientParamFirstAvailablePort       = 9,           /**< type uint16_t: first available port for client use (RTSP mode) */
            MCNetClientParamUserAgent                = 10,          /**< type int8_t *: user agent string */
            MCNetClientParamProxyURL                 = 11,          /**< type int8_t *: proxy url */
            MCNetClientParamContentLen               = 12,          /**< type int64_t: content length in bytes (HTTP) */
            MCNetClientParamMimeTypeString           = 13,          /**< type int8_t *: mime type (HTTP) */
            MCNetClientParamServerStartRange         = 14,          /**< type int64_t: server's start offset in bytes (HTTP) */
            MCNetClientParamServerEndRange           = 15,          /**< type int64_t: server's end offset in bytes (HTTP) */
            MCNetClientParamRtmpFLVMode              = 16,          /**< type bool: true - enables FLV muxing mode, false - disables (RTMP) */
            MCNetClientParamRtmpBufferLen            = 17,          /**< type uint32_t: true - buffering time in milliseconds (RTMP) */
            MCNetClientParamRtmpTransition           = 18,          /**< type int8_t *: transition string for Play2 (RTMP) */
            MCNetClientParamRtmpObserver             = 19,          /**< type IRtmpObserver: RTMP observer */
            MCNetClientParamRtspObserver             = 20,          /**< type IRtspObserver: RTSP observer */
            MCNetClientParamRtcpState                = 21,          /**< type bool: RTCP state - true enables, false - disables */
            MCNetClientParamRtspMulticastGroup       = 22,          /**< type int8_t *: multicast group address for RTSP multicasting request */
            MCNetClientParamAuthorization            = 23,          /**< type AUTHORIZATIONFIELD: authorization parameters */
            MCNetClientParamEngineType               = 25,          /**< type NetInterfaceType: current engine type */
			MCNetClientParamSeekUnits                = 26,          /**< type MCNETSEEKUNITS: type of units used for stream seeking */
			MCNetClientParamRtpInfo                  = 27,          /**< type const MCNETSOURCERTPINFOARRAY *: RTP-Info from last RTSP command */
			MCNetClientParamPlaylist				 = 28,          /**< type IMCPlaylist *: Playlist instance, must be released by caller */
			MCNetClientParamPlaylistItem			 = 29,          /**< type uint32_t: Currently selected playlist item, engine and consumers are released on item change */
			MCNetClientParamSDPDecoder				 = 30,          /**< type ISdpDecoder *: Current SDP decoder for stream, must be released by caller */
			MCNetClientParamStreamingType			 = 31,			/**< type MCNETCLIENTSTREAMINGTYPE: Current streaming type */
            MCNetClientParamRtmpHandshakeKeys        = 32,          /**< type RtmpHandshakeKeyStorage: handshake key pair */
            MCNetClientParamRtspKeepAliveMethod      = 33,          /**< type RtspKeepAliveMode : method to keep connection alive */
			MCNetClientParamEventObserver			 = 35,			/**< type INetClientEventObserver: unified net client async events observer */
			MCNetClientParamPlaybackRate			 = 36,			/**< type double: Retrieves current playback rate (server respond when available) */
            MCNetClientParamRtspHeadersConformRFC    = 37,          /**< type bool: Determines whether Transport header in RTSP SETUP request will conform RFC (true) or always use "client_port=" field to be compatible with some buggy servers (false) */
			MCNetClientParamSharedSocketInfo		 = 38,			/**< type ISharedSocketInfo *: retrieves a pointer to the object that used to share data receiving sockets with Network Renderer via IPC */
			MCNetClientParamSharedSocketExchange	 = 39,			/**< type bool: tells the engine to start (true) or stop (false) IPC exchange session for socket sharing (see description of MCNetClientParamSharedSocketInfo) */
            MCNetClientParamEncryptionInfo           = 40           /**< type IPlaylistItemEncryptionInfo: retrieves information related to stream encryption (Key URI availability, IV, Key, etc...) */
        } MCNETCLIENTPARAMTYPE;

        /**
            @brief Describes buffering parameters
        */
		typedef enum _tagMcNetClientBufferingParamType {
			MCNetClientBufferingState					= 0,	/**< type bool: Retrieve buffering status. true if buffering is active and packets are kept in queue and false if packets are actively fed to upstream consumer. */
			MCNetClientBufferingMode					= 1,	/**< type MCNETCLIENTBUFFERINGMODE: Buffering mode. Any auto-adjustments made to the buffering time in Auto mode will be reset by this call. */
			MCNetClientBufferingUserDefinedTime			= 2,	/**< type uint32_t: Buffering time limit for user defined buffering mode. */
			MCNetClientBufferingAdjustedTime			= 3,	/**< type uint32_t: Retrieve currently active time buffering limit. */
			MCNetClientBufferingTimeLimit				= 4,	/**< type uint32_t: Maximal buffering time that can be set or auto-adjusted. */
			MCNetClientBufferingSegmentCount			= 5,	/**< type uint32_t: Count of segments to buffer in segment buffering mode */
			MCNetClientBufferingSyncConsumer			= 6,	/**< type IMediaConsumer *: Set buffering consumer that should be used as sync point. Buffering engine will use it's timestamps to decide when to finish buffering. By default first created consumer is used. */
			MCNetClientBufferingTimeObserver			= 7,	/**< type INetTimeObserver *: Set time observer which will receive packet arrival callbacks. */
			MCNetClientBufferingLatePacketThreshold		= 8,	/**< type uint32_t: Minimal count of late packets to receive before audo-adjusting buffering time. */
			MCNetClientBufferingPacketSizeThreshold		= 9,	/**< type uint32_t: Minimal free byte count in buffer after which no data should be received in that buffer and new one should be requested. You'll want this to be at least greater than MTU size. Note that UDP packets can be as big as 64Kb. */
			MCNetClientBufferingLastReceivedTimestamp	= 10,	/**< type int64_t: Retrieve timestamp of last buffered packet after time reset. */
		} MCNETCLIENTBUFFERINGPARAMTYPE;

        /**
            @brief Describes consumer specific parameters
        */
		typedef enum _tagMcNetClientBufferingConsumerParamType {
			MCNetClientBufferingConsumerState				= 0,	/**< type bool: Enable or disable buffering consumer. Unused consumers or consumers with no data should be deactivated to avoid delays in buffering. */
			MCNetClientBufferingConsumerHasData				= 1,	/**< type bool: Check if buffering consumer actually received any data after last buffering reset. */
			MCNetClientBufferingConsumerTimeDelta			= 2,	/**< type int32_t: Set time to add to samples timestamps. Use this if your stream has constant audio and video time shift. */
			MCNetClientBufferingConsumerInfo				= 3,	/**< type MCNETBUFFERINGINFO: This method allows to trace current buffering status. */
			MCNetClientBufferingConsumerPassThru			= 4,	/**< type bool: Switch buffering consumer to pass-thru state, in which it forwards all IMediaConsumer callbacks without modifying them. */
		} MCNETCLIENTBUFFERINGCONSUMERPARAMTYPE;

		/**
         * @brief Unified client consumer parameters
         **/
		typedef enum _tagMCNetClientConsumerParamType {
			MCNetClientConsumerParamState					 = 0,          /**< type bool: Consumer state: true to enable, false to disable */
			MCNetClientConsumerParamQualitySelector			 = 1,          /**< type IMCPlaylistItemQualitySelector  *: Quality selector, must be released by caller */
			MCNetClientConsumerParamQualityItem				 = 2,          /**< type uint32_t: Currently active quality index if quality selector is available */
			MCNetClientConsumerParamSegmentDuration			 = 3,          /**< type uint32_t: Segment duration, if available */
			MCNetClientConsumerParamRestartSegment			 = 4,		   /**< type bool: Set to true to force engine to reload current segment */
            MCNetClientConsumerParamSeek        			 = 5		   /**< type uint64_t: Seek to desired time in milliSeconds, automatically resumes downloading data from the given consumer */
		} MCNETCLIENTCONSUMERPARAMTYPE;

        /**
         * @brief Buffering mode
         **/
        typedef enum _tagMCNetClientBufferingMode {

            MCNetClientBufferingModeLowDelay    = 0,                /**< Minimal delay */
            MCNetClientBufferingModeUserDefined = 1,                /**< Buffer fixed amount of time */
            MCNetClientBufferingModeAuto        = 2,                /**< Automatically adjust buffering time */
			MCNetClientBufferingModeSegment     = 3                 /**< Buffer by segment, buffering finishes when full segment is loaded */
        } MCNETCLIENTBUFFERINGMODE;

		/**
         * @brief Type of seek units
         **/
        typedef enum _tagMCNetClientSeekUnits {

            MCNetClientSeekUnitsSeconds    = 0,                /**< Position is specified in seconds */
            MCNetClientSeekUnitsBytes      = 1,                /**< Position is specified in bytes */
        } MCNETCLIENTSEEKUNITS;

        /**
         * @brief Describes media stream properties
         **/
        typedef struct _tagMCNetStreamMediaInfo {

            STREAMTYPE codec;                                       /**< media stream type */
            uint32_t   width;                                       /**< video width in pixels */
            uint32_t   height;                                      /**< video height in pixels */
            uint32_t   sample_rate;                                 /**< a/v sample rate in Hz */
            uint32_t   bit_per_sample;                              /**< bits per sample */
            uint32_t   bit_rate;                                    /**< media stream bitrate in bits */
            uint32_t   channels_count;                              /**< audio channel count */
            uint8_t*   pConfig;                                     /**< decoder config payload */
            uint32_t   uiConfigLen;                                 /**< decoder config payload size in bytes */
            uint32_t   mpeg_audio_layer;                            /**< mpeg audio layers = 1, 2, 3 */
            uint32_t   interlace;                                   /**< interlace flag */
            float      parX;                                        /**< picture aspect ratio X */
            float      parY;                                        /**< picture aspect ratio Y */
            float      fps;                                         /**< video frame rate */
            MEDIATYPE  mediaType;                                   /**< media type */
            uint8_t*   pLangCode;                                   /**< language when present */
            uint32_t   id;                                          /**< Media ID */
        } MCNETSTREAMMEDIAINFO;

		 /**
         * @brief Describes current buffering information
         **/
        typedef struct _tagMCNetBufferingInfo {

            uint32_t CurrentlyBuffered;                                         /**< buffered time in milli seconds */
			uint32_t RequestedBuffer;                                           /**< requested buffer in milli seconds*/
        } MCNETBUFFERINGINFO;

        /**
         * @brief Interface is used for callbacks from engine and creation of media data consumer
         **/
        class INetStreamCreator
        {
        public:
            virtual ~INetStreamCreator() {}

            /**
            * @brief This method creates data consumer for net stream
            *
            * @param[in] pStreamInfo       structure describes stream properties/media type
			* @param[in] pQualitySelector  provide quality selection functionality for item
            * @param[in] uiQualityIndex    requested quality index
            * @return                      Returns pointer on IMediaConsumer interface; might return NULL in case of unsupported media type
            **/
            virtual IMediaConsumer * OnCreateConsumerForNetStream( const MCNETSTREAMMEDIAINFO *pStreamInfo, IMCPlaylistItemQualitySelector *pQualitySelector, uint32_t uiQualityIndex ) = 0;

            /**
            * @brief This method request authorization properties: username and password
            *
			* @param[in] pUrl contains url for which auth info is required
            * @param[in, out] pAuthParams contains authorization properties
            * @return                     None
            **/
            virtual void OnAuthorizationRequest( const int8_t *pUrl, AUTHORIZATIONFIELD * const pAuthParams ) = 0;

			/**
            * @brief This method notifies that low level engine was successfully (re-)initialized
            *
			* @param[in] aType Type of engine
            * @return                     None
            **/
            virtual void OnEngineInit( MCNETCLIENTENGINETYPE aType ) = 0;
        };

		/**
         * @brief Pull source interface for unified client
         **/
		class INetPullSource : public INetRefCounted
		{
			/**
            * @brief Request data range
            *
			* @param[in,out] pBuffer Buffer to hold received data
			* @param[in] i64Start Data start offset, in bytes
			* @param[in] i64End Data end offset, in bytes
			* @param[out] pi64ReadBytes Count of bytes that were actually placed into buffer
            * @return Returns MCNetResultOk on success and error code otherwise
            **/
			virtual MCNETRESULT ReadRange( int8_t *pBuffer, int64_t i64Start, int64_t i64End, int64_t *pi64ReadBytes ) = 0;
		};

        /**
         * @brief Universal network client control asynchronous interface
         **/
        class IUnifiedNetClientAsync : public INetRefCounted
        {
        public:
            /**
            * @brief This method starts network stream
            *
            * @param[in] dStartTime start time in Sec.mSecs or bytes offset
            * @param[in] dStopTime  stop time in Sec.mSecs or bytes offset
            * @param[in] dRate      specifies playback speed rate
            * @return               Returns MCNetResultOk on success and error code otherwise
            **/
            virtual MCNETRESULT PlayAsync( double dStartTime, double dStopTime, double dRate = 1.0 ) = 0;

            /**
            * @brief This method pauses network stream or stops engine in case of Live streaming
            *
            * @return Returns MCNetResultOk on success and error code otherwise
            **/
            virtual MCNETRESULT PauseAsync() = 0;

            /**
            * @brief This method stops network stream
			*
            * @return Returns MCNetResultOk on success and error code otherwise
            **/
            virtual MCNETRESULT StopAsync() = 0;

            /**
            * @brief This method allows to set engine dependent parameters
            *
            * @param[in] aType      parameter type, see MCNETCLIENTPARAMTYPE::MCNetClientParamEventObserver
            * @param[in] pValue     valid parameter value
            * @param[in] iValueSize actual parameter size
            * @return               Returns MCNetResultOk on success and error code otherwise
            **/
            virtual MCNETRESULT SetParameter( MCNETCLIENTPARAMTYPE aType, void *pValue, int32_t iValueSize ) = 0;
        };

        /**
         * @brief Universal network client
         **/
        class IUnifiedNetClient : public INetRefCounted
        {
        public:
            virtual ~IUnifiedNetClient() {}

            /**
            * @brief This method initializes engine
            *
            * @param[in] ServerUrl complete server URL or null terminated SDP data buffer or UTF8 SDP file name
            * @param[in] aNIC      local network interface to use, might be NULL for system default interface
            * @return              Returns MCNetResultOk on success and error code otherwise
            **/
            virtual MCNETRESULT Init( const int8_t *ServerUrl, const int8_t *aNIC = NULL) = 0;

            /**
            * @brief This method starts network stream
            *
            * @param[in] dStartTime start time in Sec.mSecs or bytes offset
            * @param[in] dStopTime  stop time in Sec.mSecs or bytes offset
            * @param[in] dRate      specifies playback speed rate
            * @return               Returns MCNetResultOk on success and error code otherwise
            **/
            virtual MCNETRESULT Play( double dStartTime, double dStopTime, double dRate = 1.0 ) = 0;

            /**
            * @brief This method pauses network stream or stops engine in case of Live streaming
            *
            * @return Returns MCNetResultOk on success and error code otherwise
            **/
            virtual MCNETRESULT Pause() = 0;

            /**
            * @brief This method stops network stream
            *
			* With bWait=false must only be used from engine callbacks
			*
			* @param[in] bWait      set to true to wait for engine state change to complete or set to false to request asynchronous state change and return immediately
			*
            * @return Returns MCNetResultOk on success and error code otherwise
            **/
            virtual MCNETRESULT Stop( bool bWait = true ) = 0;

            /**
            * @brief This method allows to set engine dependent parameters
            *
            * @param[in] aType      parameter type
            * @param[in] pValue     valid parameter value
            * @param[in] iValueSize actual parameter size
            * @return               Returns MCNetResultOk on success and error code otherwise
            **/
            virtual MCNETRESULT SetParameter( MCNETCLIENTPARAMTYPE aType, void *pValue, int32_t iValueSize ) = 0;

            /**
            * @brief This method allows to get engine dependent parameters
            *
            * @param[in]  aType      parameter type
            * @param[out] pValue     valid parameter value
            * @param[out] pValueSize actual parameter size
            * @return                Returns MCNetResultOk on success and error code otherwise
            **/
            virtual MCNETRESULT GetParameter( MCNETCLIENTPARAMTYPE aType, void *pValue, int32_t *pValueSize ) = 0;

			/**
            * @brief This method allows to set engine dependent parameters
            *
			* @param[in] pConsumer  consumer instance to set parameter on
            * @param[in] aType      parameter type
            * @param[in] pValue     valid parameter value
            * @param[in] iValueSize actual parameter size
            * @return               Returns MCNetResultOk on success and error code otherwise
            **/
            virtual MCNETRESULT SetConsumerParameter( const IMediaConsumer *pConsumer, MCNETCLIENTCONSUMERPARAMTYPE aType, void *pValue, int32_t iValueSize ) = 0;

            /**
            * @brief This method allows to get engine dependent parameters
            *
			* @param[in]  pConsumer  consumer instance to get parameter from
            * @param[in]  aType      parameter type
            * @param[out] pValue     valid parameter value
            * @param[out] pValueSize actual parameter size
            * @return                Returns MCNetResultOk on success and error code otherwise
            **/
            virtual MCNETRESULT GetConsumerParameter( const IMediaConsumer *pConsumer, MCNETCLIENTCONSUMERPARAMTYPE aType, void *pValue, int32_t *pValueSize ) = 0;

            /**
            * @brief This method allows to retrieve protocol specific engine API
            *
            * @param[in]  engineType  engine type
            * @param[out] ppInterface pointer on requested interface, might be NULL when failed
            * @return                 Returns MCNetResultOk on success and error code otherwise
            **/
            virtual MCNETRESULT GetInterface( MCNETCLIENTENGINETYPE engineType, void **ppInterface ) = 0;

			/**
            * @brief This method releases all engines, use it before destroying consumers
            *
            * @return                 Returns MCNetResultOk on success and error code otherwise
            **/
			virtual MCNETRESULT Shutdown() = 0;
        };

		/**
         * @brief Client events notifier
         **/
        class INetClientEventObserver
        {
        public:
			virtual ~INetClientEventObserver() {}

			/**
            * @brief Notifies application about occurred event
            *
            * @param[in] aEvent      event type
            * @param[in] pEventData  pointer to optional data associated with the event (event-specific, see description of @ref MCNETCLIENTEVENT)
            * @return                None
            **/
			virtual void OnNetClientEvent( MCNETCLIENTEVENT aEvent, void *pEventData = NULL ) = 0;
		};

        /**
         * @brief Time events notifier
         **/
        class INetTimeObserver
        {
        public:
            virtual ~INetTimeObserver() {}

            /**
            * @brief Reports arrival of new sample into buffer.
            *
			* Application can use this interface to adjust system clock.
			* Note that this callback is only called when buffering is finished and data is being sent to upstream consumer.
            *
            * @param[in] bSync        Notifies about source of time's changing, true - if timestamp changes on sync consumer, false - otherwise
            * @param[in] iConsumerID  User identifier of consumer specified in GetBufferingConsumer
            * @param[in] i64Timestamp Indicates timestamp of received packet
            * @return                 None
            **/
            virtual void OnTimeUpdate( bool bSync, int32_t iConsumerID, int64_t i64Timestamp ) = 0;
        };

        /**
         * @brief Declares interface of data buffering and synchronization engine
         **/
        class INetBufferingManager : public INetRefCounted
        {
        public:
            virtual ~INetBufferingManager() {}

            /**
            * @brief Creates instance of buffering media consumer.
            *
            * Pass this consumer to network engine and it will buffer data and output it to application's media consumer.
            *
            * @param[in] pUpstreamConsumer Pointer to application's media consumer instance that will receive data after buffering
            * @param[in] iID               User identifier of consumer which will be passed to application in callbacks
            * @return                      Returns pointer to buffering media consumer that can be passed to engine or NULL on error
            **/
            virtual IMediaConsumer *GetBufferingConsumer( IMediaConsumer *pUpstreamConsumer, int32_t iID ) = 0;

            /**
            * @brief Releases instance of buffering media consumer created by GetBufferingConsumer call.
            *
            * @param[in] pBufferingConsumer Pointer to buffering media consumer that should be released
            * @return                       Returns MCNetResultOk on success and error code otherwise
            **/
            virtual MCNETRESULT ReleaseBufferingConsumer( IMediaConsumer *pBufferingConsumer ) = 0;

            /**
            * @brief Notify buffering manager that packets come later or earlier than expected.
            *
			* @param[in] pBufferingConsumer Pointer to buffering media consumer for which information applies
            * @param[in] iMilliseconds Positive value means packets are late, negative value means that packets came too early.
            * @return Returns MCNetResultOk on success and error code otherwise
            **/
            virtual MCNETRESULT AdjustBuffering( IMediaConsumer *pBufferingConsumer, int32_t iMilliseconds ) = 0;

			/**
            * @brief Activate or reactivate buffering.
            *
            * If buffering was finished - it will be restarted so application is responsible for handling
            * any buffering delay that may occur when reactivating buffering in the middle of playback.
            *
            * @return Returns MCNetResultOk on success and error code otherwise
            **/
            virtual MCNETRESULT ResetBuffering() = 0;

			/**
			* @brief Change state of buffering engine.
			*
			* While paused, no samples will be fed to consumers.
			*
			* @param[in] bPause	Set to true to hold samples, false allow feeding samples to consumers
			* @return	Returns MCNetResultOk on success and error code otherwise
			**/
			virtual MCNETRESULT Pause( bool bPause ) = 0;

            /**
            * @brief Clear buffered samples queue.
            *
            * This call also resets buffering.
            *
            * @param[in] bFeedSamples When set to true - buffered samples will be sent to upstream consumer, when set to false - buffered samples will be just released
            * @return                 Returns MCNetResultOk on success and error code otherwise
            **/
            virtual MCNETRESULT Flush( bool bFeedSamples ) = 0;

			/**
            * @brief Clear buffered samples queue for specific consumer.
            *
			* @param[in] pBufferingConsumer Pointer to buffering media consumer which should be flushed
            * @param[in] bFeedSamples When set to true - buffered samples will be sent to upstream consumer, when set to false - buffered samples will be just released
            * @return                 Returns MCNetResultOk on success and error code otherwise
            **/
            virtual MCNETRESULT FlushConsumer( IMediaConsumer *pBufferingConsumer, bool bFeedSamples ) = 0;

			/**
            * @brief This method allows to set buffering parameters
            *
            * @param[in] aType      parameter type
            * @param[in] pValue     valid parameter value
            * @param[in] iValueSize actual parameter size
            * @return               Returns MCNetResultOk on success and error code otherwise
            **/
            virtual MCNETRESULT SetParameter( MCNETCLIENTBUFFERINGPARAMTYPE aType, void *pValue, int32_t iValueSize ) = 0;

            /**
            * @brief This method allows to get buffering parameters
            *
            * @param[in]  aType      parameter type
            * @param[out] pValue     valid parameter value
            * @param[out] pValueSize actual parameter size
            * @return                Returns MCNetResultOk on success and error code otherwise
            **/
            virtual MCNETRESULT GetParameter( MCNETCLIENTBUFFERINGPARAMTYPE aType, void *pValue, int32_t *pValueSize ) = 0;

			/**
            * @brief This method allows to set buffering consumer parameters
            *
			* @param[in] pBufferingConsumer  buffering consumer instance to set parameter on
            * @param[in] aType               parameter type
            * @param[in] pValue              valid parameter value
            * @param[in] iValueSize          actual parameter size
            * @return                        Returns MCNetResultOk on success and error code otherwise
            **/
            virtual MCNETRESULT SetConsumerParameter( IMediaConsumer *pBufferingConsumer, MCNETCLIENTBUFFERINGCONSUMERPARAMTYPE aType, void *pValue, int32_t iValueSize ) = 0;

            /**
            * @brief This method allows to get buffering consumer parameters
            *
			* @param[in]  pBufferingConsumer  buffering consumer instance to get parameter from
            * @param[in]  aType               parameter type
            * @param[out] pValue              valid parameter value
            * @param[out] pValueSize          actual parameter size
            * @return                         Returns MCNetResultOk on success and error code otherwise
            **/
            virtual MCNETRESULT GetConsumerParameter( IMediaConsumer *pBufferingConsumer, MCNETCLIENTBUFFERINGCONSUMERPARAMTYPE aType, void *pValue, int32_t *pValueSize ) = 0;

        };

		/**
         * @brief Quality measurement and switching decision making helper
		 *
		 * Quality monitor is not thread-safe
         **/
		class INetQualityMonitor : public INetRefCounted
		{
		public:
			virtual ~INetQualityMonitor() {};

			/**
			* @brief Reset accumulated statistics
			**/
			virtual void Reset() = 0;

			/**
			* @brief Sets count of packet arrival timestamps to keep for instant bitrate calculation
			*
			* @param[in] uiPacketCount	Count of packets for which to keep arrival timestamps
			**/
			virtual void SetRecentPacketCount( uint32_t uiPacketCount ) = 0;

			/**
			* @brief Sets count of segment bitrate measurements to keep for average segment bitrate calculation
			*
			* @param[in] uiSegmentCount	Count of segments to measure
			**/
			virtual void SetRecentSegmentCount( uint32_t uiSegmentCount ) = 0;

			/**
			* @brief Must be called when new data packet is received
			*
			* @param[in] uiByteCount	Size of received packet in bytes
			**/
			virtual void OnPacketReceived( uint32_t uiByteCount ) = 0;

			/**
			* @brief Should be called when segment loading has finished
			**/
			virtual void OnSegmentFinished() = 0;

			/**
			* @brief Returns average bitrate of last completed segment
			*
			* @return	Last completed segment bitrate in bits per second
			**/
			virtual uint32_t GetLastSegmentBitrate() = 0;

			/**
			* @brief Returns average bitrate, calculated for current segment
			*
			* Since this bitrate is reset on each new segment, it can be inaccurate if called shortly after segment change.
			* If stream has no segments or consists of single segment - return value will be same as with GetAverageBitrate.
			*
			* @return	Segment bitrate in bits per second
			**/
			virtual uint32_t GetCurrentSegmentBitrate() = 0;

			/**
			* @brief Returns average bitrate, calculated for a number of recent segments.
			*
			* Use SetRecentSegmentCount to adjust segment count.
			*
			* @return	Average segment bitrate in bits per second
			**/
			virtual uint32_t GetAverageSegmentBitrate() = 0;

			/**
			* @brief Returns average bitrate, calculated for entire measuring period
			*
			* @return	Average bitrate in bits per second
			**/
			virtual uint32_t GetAverageBitrate() = 0;

			/**
			* @brief Returns instant bitrate, calculated for a number of recent packets.
			*
			* Use SetRecentPacketCount to adjust packet count.
			*
			* @return	Current instant bitrate in bits per second
			**/
			virtual uint32_t GetInstantBitrate() = 0;

			/**
			* @brief Get milliseconds elapsed from start of segment loading
			*
			* @return	Count of milliseconds elapsed from start of current segment
			**/
			virtual int64_t GetCurrentSegmentLoadingTime() = 0;

			/**
			* @brief Get milliseconds elapsed from start of measurement
			*
			* @return	Count of milliseconds elapsed from start of measurement
			**/
			virtual int64_t GetMeasurementTime() = 0;
		};

#if defined (__cplusplus)
        extern "C" {
#endif
            /**
            * @brief Unified network streaming client provides simple API for working with multiple streaming protocols.
            *
            * It handles creation and initialization of low level engines.
            *
            * @param[in] pStreamCreator valid pointer on network stream creator
            * @param[in]  get_rc        Pointer to get_rc memory manager implementation or NULL to use standard allocators
            * @return                   Returns valid pointer on success and NULL when failed
            **/
            IUnifiedNetClient* NET_API_CALL GetUnifiedNetClientInterface( INetStreamCreator *pStreamCreator, void *(MC_EXPORT_API *get_rc)(const char*) );

            /**
            * @brief Buffering manager provides buffering and synchronization on top of low level engines.
            *
            * Use manager to create 'proxy' media data consumers that will do the work and then send data to
            * application's media data consumers.
            *
            * @param[in] get_rc Pointer to get_rc memory manager implementation or NULL to use standard allocators
            * @return           Returns valid pointer on success and NULL when failed
            **/
            INetBufferingManager* NET_API_CALL GetNetBufferingManagerInterface( void *(MC_EXPORT_API *get_rc)(const char*) );

			/**
            * @brief Quality monitor provides statistical information on data flow and can be used to decide when to change stream quality
            *
            * @param[in] get_rc Pointer to get_rc memory manager implementation or NULL to use standard allocators
            * @return           Returns valid pointer on success and NULL when failed
            **/
			INetQualityMonitor* NET_API_CALL GetNetQualityMonitorInterface( void *(MC_EXPORT_API *get_rc)(const char*) );
#if defined (__cplusplus)
        }
#endif
    };
};

#endif //__MC_UNIFIED_NET_CLIENT_API_H__
