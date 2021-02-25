/**
@file  media_detector_api.h
@brief Media Detector API

@verbatim
File: media_detector_api.h
Desc: Media Detector API

Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
This software is protected by copyright law and international treaties.  Unauthorized
reproduction or distribution of any portion is prohibited by law.
@endverbatim
**/

#if !defined (__MC_MEDIA_DETECTOR_API_H__)
#define __MC_MEDIA_DETECTOR_API_H__

#include "net_common.h"
#include "upnp_common.h"
#include "metadata_extractor_api.h"

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
            * @brief Declares Media Detector interface
            **/
		    class IMediaDetector : public INetRefCounted
		    {
		    public:
			    virtual ~IMediaDetector() {}

			    /**
                * @brief Try's to detect given content
                *
                * @param[in] pDataSource valid pointer to @ref IDataSource
                * @return    true on successful detection, false otherwise
                **/
			    virtual bool Detect( IDataSource *pDataSource ) = 0;

			    /**
                * @brief Retrieves container (stream) mime type
                *
                * @return valid pointer to null terminated string which represents stream type on success, NULL otherwise
                **/
			    virtual const int8_t * GetMimetype( ) const = 0;

                /**
                * @brief Retrieves container (stream) mime type
                *
                * @return valid pointer to null terminated string which represents stream type on success, NULL otherwise
                **/
			    virtual MEDIATYPE GetMediatype( ) const = 0;

                /**
                * @brief Retrieves list of mime types of current detector
                *
                * @param[out] iCount count of mimetypes
                *
                * @return valid pointer to array of null terminated strings which are represent mime types of current detector;
                **/
			    virtual const int8_t ** const GetSupportedTypes( int32_t* iCount ) const = 0;

			    /**
                * @brief Retrieves media file duration in milli seconds
                *
                * @return duration on success, 0 otherwise
                **/
			    virtual int64_t GetDuration( ) const = 0;

			    /**
                * @brief Retrieves DLNA profile name for the given multimedia file
                *
                * @return valid pointer to null terminated string which represents dlna profile name on success, NULL otherwise
                **/
			    virtual const int8_t * GetDlnaProfileName() = 0;

                /**
			    * @brief Retrieves metadata property value by name
			    *
			    * @param[in] pPropertyName property name
			    * @return                  property value or NULL if not found
			    **/
			    virtual const int8_t * GetPropertyValue( const int8_t *pPropertyName ) const = 0;

			    /**
			    * @brief Enumerates all Metadata fields from file
			    *
			    * @param[in] uiIndex index number of field
			    * @return            pointer to metadata field or NULL if index is out of range
			    **/
			    virtual const UpnpValuePair * EnumMetaDataEntries( uint32_t uiIndex ) const = 0;

                /**
			    * @brief Returns the count of the Metadata fields available for the given file
			    *
			    * @return            the count of metadata entries
			    **/
                virtual int32_t MetadataEntryCount( ) const = 0;

                /**
			    * @brief This method allows to provide the metadata extractor object
			    *
			    * @param[in] pMetaDataExtractor a valid pointer to the metadata extractor object @ref IMetaDataExtractor
			    * @return            true - when extractor was added to the detector, false - otherwise
			    **/
			    virtual bool SetMetaDataExtractor( IMetaDataExtractor *pMetaDataExtractor ) = 0;
		    };
        };
	};
};

#endif //__MC_MEDIA_DETECTOR_API_H__
