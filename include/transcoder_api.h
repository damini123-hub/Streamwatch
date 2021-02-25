/**
@file  transcoder_api.h
@brief UPnP Transcoder API

@verbatim
File: transcoder_api.h
Desc: UPnP Transcoder API

Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
This software is protected by copyright law and international treaties.  Unauthorized
reproduction or distribution of any portion is prohibited by law.
@endverbatim
**/

#if !defined (__MC_UPNP_TRANSCODER_API_H__)
#define __MC_UPNP_TRANSCODER_API_H__

/**
* @def MC_UPNP_API
* @brief Defines calling convention
* @hideinitializer
**/
#if defined(WIN32)
#define MC_UPNP_API __cdecl
#else
#define MC_UPNP_API
#endif

#include "mcapiext.h"
#include "net_common.h"
#include "upnp_device_api.h"

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
        * namespace UpnpStack
        * @brief Contains DLNA interfaces
        **/
        namespace UpnpStack {

			/**
            * @brief Describes transcoder job events
            **/
			typedef enum _tagTanscoderNotifyType {

				ETranscoderJobCompleted = 0x00,			/**< Transcoding has been finished; possible arguments: type = int32_t; value equal to zero in case of success, other values indicate failed result */
                ETranscoderJobDemuxError,				/**< Error occured while parsing source file */
                ETranscoderJobDecoderError,				/**< Error occured while decoding elementary stream */
                ETranscoderJobEncoderError,				/**< Error occured while encoding stream */
                ETranscoderJobMuxerError,				/**< Error occured while multiplexing output streams */
			} TranscoderNotifyType;

			/**
            * @brief Describes transcoder target format
            **/
			typedef struct _tagTranscoderTargetDescription {

				uint32_t uiTargetID;					/**< Target format identifier, unique for given transcoder instance */
				UpnpValuePair *pAttributes;				/**< Attribute array that can be used to fill ContentDirectory item resource */
				uint32_t uiAttributeCount;				/**< Count of attributes in array */
			} TranscoderTargetDescription;

			class ITranscoderJob;

			/**
            * @brief Declares transcoder observer interface; every transcoder job instance must have single implementation of the given API
            **/
			class ITranscoderObserver
			{
			public:
				virtual ~ITranscoderObserver() {}

				/**
                * @brief Delivers notify from single job to transcoder
                *
                * @param[in] pChild represents valid pointer of a single transcoder job (notify source)
				* @param[in] aNotify represents notify type
				* @param[in] pData represents notify specific argument (might be NULL)
				* @param[in] iDataLen represents notify specific argument size (might be 0)
                * @return                    None
                **/
				virtual void TranscoderNotify( const ITranscoderJob *pChild, TranscoderNotifyType aNotify, void *pData, int32_t iDataLen ) = 0;
			};

			/**
            * @brief Declares single transcoder's job; for example: single job may represents convertation from MP4 (AVC/AAC) to MPEG2 PS (MPEG2 A/V)
            **/
			class ITranscoderJob : public IDataSource
			{
			public:
				virtual ~ITranscoderJob() {}

				/**
                * @brief This method has basic player Play/Run meaning
                *
                * @param[in] iStart mandatory parameter; represents start time for transcoding in milliseconds
				* @param[in] iStop mandatory parameter; represents stop time for transcoding in milliseconds
                * @return Return @ref MCNETRESULT
                **/
				virtual MCNETRESULT Start( int64_t iStart, int64_t iStop ) = 0;

				/**
                * @brief This method has basic player Stop meaning
                *
                * @return Return @ref MCNETRESULT
                **/
				virtual MCNETRESULT Stop() = 0;

				/**
                * @brief This method has basic player Pause meaning
                *
                * @return Return @ref MCNETRESULT
                **/
				virtual MCNETRESULT Pause() = 0;

				/**
                * @brief This method retrieves transcoded file mime type
                *
                * @return		valid string with transcoded file mime type
                **/
				virtual const int8_t * GetOutMimeType( void ) = 0;

				/**
                * @brief This method retrieves transcoded file DLNA profile string
                *
                * @return		valid string with transcoded file DLNA profile string
                **/
				virtual const int8_t * GetOutProfile( void ) = 0;
			};

			/**
            * @brief Declares transcoder API; transcoder might have multiple jobs for media
            **/
			class IUpnpTranscoder : public INetRefCounted
			{
			public:
				virtual ~IUpnpTranscoder() {}

				/**
                * @brief This method retrieves array of transcoding output formats available for specified file
                *
                * @param[in]  pClient        represents an device that requested item
				* @param[in]  pItem          represents an item of content directory as media file
				* @param[out] pTargets       array of transcoder target descriptions; when no longer needed - it must be cleared by calling IUpnpTranscoder::FreeEnumFormats()
				* @param[out] pTargetCount   count of transcoding target descriptions in result array
                * @return                    Return @ref MCNETRESULT
                **/
				virtual MCNETRESULT EnumOutFormats( IUpnpClientDescriptor* pClient, IUpnpContentDirectoryItem *pItem, TranscoderTargetDescription **pTargets, uint32_t *pTargetCount ) = 0;

				/**
                * @brief Frees array which was returned by EnumOutFormats()
                *
				* @param[in] pTargets pointer to array of MIME types returned by IUpnpTranscoder::EnumOutFormats()
                **/
				virtual void FreeEnumFormats( TranscoderTargetDescription *pTargets, uint32_t uiTargetCount ) = 0;

				/**
                * @brief This method creates a single instance of transcoding job
                *
				* @param[in] pInputFileName complete path to source media file
				* @param[in] pInputMimeType optional parameter that specifies detected input file media type; transcoder may use it to avoid extra file parsing
				* @param[in] uiTargetID specifies what transcoding target should be used
                * @param[in] pObserver valid pointer to instance of ITranscoderObserver to receiving notifications
				* @param[out] ppJob represents allocated pointer of single transcoding job
                * @return Return @ref MCNETRESULT
                **/
				virtual MCNETRESULT RequestJob( const wchar_t *pInputFileName, const int8_t *pInputMimeType, uint32_t uiTargetID, ITranscoderObserver *pObserver, ITranscoderJob **ppJob ) = 0;
			};
		};
	};
};

#endif //__MC_UPNP_TRANSCODER_API_H__
