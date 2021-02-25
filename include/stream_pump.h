/* ----------------------------------------------------------------------------
 * File: stream_pump.h
 *
 * Desc: Stream Pump API
 *
 * Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.
 *
 * MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 * This software is protected by copyright law and international treaties.  Unauthorized
 * reproduction or distribution of any portion is prohibited by law.
 * ----------------------------------------------------------------------------
 */

#if !defined (__MC_STREAM_PUMP_ENGINE_API_H__)
#define __MC_STREAM_PUMP_ENGINE_API_H__

#include "mctypes.h"
#include "mcapiext.h"
#include "net_common.h"

/**
* @def STREAMPUMP_API
* @brief Defines calling convention
* @hideinitializer
**/
#if defined(WIN32)
    #define STREAMPUMP_API __cdecl
#else
    #define STREAMPUMP_API
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
         * @brief Describes file container
         **/
        typedef enum _tagContainerType {

            containerMPEGTS,                             /**< MPEG2 Transport stream*/
            containerMPEGPS,                             /**< MPEG2 Program Stream*/
            containerMPEGSS,                             /**< MPEG1 System Stream */
            containerASF,                                /**< Advanced Systems Format by Microsoft: WMA, WMV, ASF containers*/
            containerMpegAudio,                          /**< MPEG Audio stream*/
            containerFLV,                                /**< Flash Video Container*/
            containerUndefined = 0x7ffffff               /**< Stub Value*/
        } CONTAINERTYPE;

        /**
        * @brief Describes seeking mode
        **/
        typedef enum _tagPositionType {

            unitsTime,                                   /**< Seek by time value in 100-nanosec units*/
            unitsBytes                                   /**< Seek by bytes offset*/
        } POSITIONTYPE;

        /**
         * @brief Describes single chunk of data(sample)
         **/
        typedef struct _tagSPSampleInfo {

            int8_t*  pData;                              /**< Pointer to sample data*/
            uint32_t uiActualSize;                       /**< Size of sample data*/
            uint32_t uiMaxBufferSize;                    /**< Maximum available buffer size*/
            int64_t  tStart;                             /**< Start timestamp*/
            int64_t  tStop;                              /**< Stop timestamp*/
            uint32_t uiFlags;                            /**< Extra flags*/
        } SPSAMPLEINFO;

         /**
         * @brief Describes overall stream properties
         **/
        typedef struct _tagStreamInfo {

            int64_t       _duration;                     /**< Stream duration(100-nanosec unis) */
            int64_t       _fileSize;                     /**< File size(bytes)*/
            CONTAINERTYPE _containerType;                /**< Container type*/
            bool          _hasIndex;                     /**< Indicates index file existing*/
            uint32_t      _bitrate;                      /**< Stream bitrate*/
            uint32_t      _maxPacketSize;                /**< Max Packet size inside of file*/
            uint8_t*      _pHeader;                      /**< File header, generally ASF applicated*/
            uint32_t      _HeaderSize;                   /**< Header size*/
            uint32_t      _width;                        /**< Video Frame width*/
            uint32_t      _height;                       /**< Video Frame height*/
            uint32_t      _uiSampleRate;                 /**< Sample rate*/
            uint32_t      _uiChannelCount;               /**< Audio channel count*/
            uint32_t      _Flags;                        /**< Extra flags for mpeg audio: 1 - layer3, 2 - layer2, 3 - layer1 */
            uint32_t      _MpegVer;                      /**< MPEG audio version: 1 - mpeg1; 2 - mpeg2; 3 - mpeg2.5 (3) */
        } STREAMINFO;

         /**
         * @brief Provides API to use index file for fast seeking
         **/
        class IIndexReader : public INetRefCounted
        {
        public:
            virtual ~IIndexReader() {}

            /**
            * @brief Opens index file
            *
            * @param[in] pFileName  index file path
            * @return               Returns true on success, otherwise false
            **/
            virtual bool Open( const wchar_t *pFileName ) = 0;

            /**
            * @brief Retrieves the bytes offset by timestamp
            *
            * @param[in]  aTimeOffset  timestamp for searching
            * @return                  Returns file bytes offset
            **/
            virtual int64_t GetPos( int64_t aTimeOffset ) = 0;

            /**
            * @brief Retrieves the timestamp by bytes offset
            *
            * @param[in]  aBytesOffset  byte offset for searching
            * @return                   Returns 100-nanoseconds timestamp value
            **/
            virtual int64_t GetTime( int64_t aBytesOffset ) = 0;

            /**
            * @brief Retrieves the whole stream duration
            *
            * @return Returns stream duration
            **/
            virtual int64_t GetDuration() = 0;

            /**
            * @brief Fill the list of supported containers.
            *
            * First call with pContainers == NULL return count of values in pCount
            *
            * @param[in]     pContainers  pointer to allocated array of @ref CONTAINERTYPE
            * @param[in,out] pCount       amount of value or available buffer space
            * @return                     None
            **/
            virtual void GetSupportedContainers( CONTAINERTYPE **pContainers, int32_t *pCount ) = 0;

            /**
            * @brief Closes index reader
            *
            * @return None
            **/
            virtual void Close() = 0;
        };

         /**
         * @brief Provides API container recognizer
         *
         * Read internal container structure and retrieve info
         **/
        class IBaseContainerParser : public INetRefCounted
        {
        public:
            virtual ~IBaseContainerParser() {}

            /**
            * @brief Extends by external Index reader
            *
            * @param[in]  pIndexReader  valid pointer to index reader implementation
            * @return                   Returns 100-nanoseconds timestamp value
            **/
            virtual void SetIndexReader( IIndexReader *pIndexReader ) = 0;

            /**
            * @brief Reads and parses data from IDataSource implementation
            *
            * @param[in]  pDataSource  valid pointer to IDataSource implementation
            * @return                  Returns true on success parsing, otherwise false
            **/
            virtual bool Detect( IDataSource *pDataSource ) = 0;

            /**
            * @brief Retrieves the container type
            *
            * @return @ref CONTAINERTYPE value
            **/
            virtual CONTAINERTYPE GetContainerType( ) const = 0;

            /**
            * @brief Indicates having file index
            *
            * @return Returns true on existing index, otherwise false
            **/
            virtual bool HasIndex() = 0;

            /**
            * @brief Retrieves the current position in file
            *
            * @param[in]   i64Pos  current stream position
            * @param[out]  aType   preferable unit format
            * @return              Returns true on success, otherwise false
            **/
            virtual bool GetPosition( int64_t *i64Pos, POSITIONTYPE aType ) = 0;

            /**
            * @brief Seek to new position
            *
            * @param[in]  i64Pos   new reading position
            * @param[in]  aType    position units format
            * @param[in]  bPrecise true to specified value or false for nearest packet start
            * @return              Returns true on success, otherwise false
            **/
            virtual bool SetPosition( int64_t i64Pos, POSITIONTYPE aType, bool bPrecise = true ) = 0;

            /**
            * @brief Retrieves the stream duration
            *
            * @param[in]  i64Duration  whole stream duration
            * @param[in]  aType        value format type
            * @return                  Returns true on success, otherwise false
            **/
            virtual bool GetDuration( int64_t *i64Duration, POSITIONTYPE aType ) = 0;

            /**
            * @brief Retrieves the data sample
            *
            * @param[in,out]  pSample  descriptor of media data sample, buffer for media data must be allocated
            * @return                  Returns true on success, otherwise false
            **/
            virtual bool GetSample( SPSAMPLEINFO *pSample ) = 0;

            /**
            * @brief Clear internal state
            *
            * @return  None
            **/
            virtual void Reset() = 0;

            /**
            * @brief Retrieves the stream properties
            *
            * @param[out]  pInfo  @ref STREAMINFO value of describing stream
            * @return             None
            **/
            virtual void GetSpecificConfig( STREAMINFO *pInfo ) = 0;
        };

         /**
         * @brief Provide API to Stream Pump component
         **/
        class IStreamPump : public INetRefCounted
        {
            public:
                virtual ~IStreamPump() {}

                 /**
                * @brief Initializes component
                *
                * @param[in] pDataSource valid pointer to source of file data,
                * @param[in] pIndexReader pointer to index reader implementation or NULL in case of index reader is not using
                * @return                 Returns true on success, otherwise false
                **/
                virtual bool Init( IDataSource *pDataSource, IIndexReader *pIndexReader = NULL ) = 0;

                /**
                * @brief Retrieves the stream properties
                *
                * @param[out] pStreamInfo stream properties
                * @return                 Returns true on success, otherwise false
                **/
                virtual bool GetStreamInfo( STREAMINFO *pStreamInfo ) = 0;

                /**
                * @brief Retrieves the current stream position
                *
                * @param[in]  aValue       current reading position
                * @param[in]  aType        value format type
                * @return                  Returns true on success, otherwise false
                **/
                virtual bool GetPosition( int64_t *aValue, POSITIONTYPE aType ) = 0;

                /**
                * @brief Seek to new position
                *
                * @param[in]  aValue   new reading position
                * @param[in]  aType    position units format
                * @param[in]  aPrecise true to specified value or false for nearest packet start, used without index
                * @return              Returns true on success, otherwise false
                **/
                virtual bool SetPosition( int64_t aValue, POSITIONTYPE aType, bool aPrecise ) = 0;

                /**
                * @brief Retrieves the stream duration
                *
                * @param[out] pDuration    whole stream duration in 100-nanoseconds units
                * @return                  Returns true on success, otherwise false
                **/
                virtual bool GetDuration( int64_t *pDuration ) = 0;

                /**
                * @brief Retrieves the data sample
                *
                * @param[in,out]  pSampleInfo  descriptor of media data sample, buffer for media data must be allocated
                * @return                  Returns true on success, otherwise false
                **/
                virtual bool GetSample( SPSAMPLEINFO *pSampleInfo ) = 0;

                /**
                * @brief Extends stream pump with additional format support
                *
                * @param[in] pRecognizer valid pointer to IBaseContainerParser implementation
                * @return                None
                **/
                virtual void AdviseRecognizer( IBaseContainerParser *pRecognizer ) = 0;

                /**
                * @brief Clear internal state
                *
                * @return  None
                **/
                virtual void Reset() = 0;
        };

#if defined (__cplusplus)
        extern "C" {
#endif
            /**
            * @brief Creates new instance of component
            *
            * @param[in] get_rc Pointer to get_rc memory manager implementation or NULL to use standard allocators
            * @return            valid pointer to @ref IStreamPump or NULL
            **/
            IStreamPump * STREAMPUMP_API CreateStreamPump( void *(MC_EXPORT_API *get_rc)(const char*) );

            /**
            * @brief Creates new instance of index reader implementation
            *
            * @param[in] get_rc Pointer to get_rc memory manager implementation or NULL to use standard allocators
            * @return            valid pointer to @ref IIndexReader or NULL
            **/
            IIndexReader * STREAMPUMP_API CreateIndexReader( void *(MC_EXPORT_API *get_rc)(const char*) );

            /**
            * @brief Offers extended API to module
            *
            * @param[in] func identifier of requested function
            * @return         valid pointer to function or NULL
            **/
            APIEXTFUNC STREAMPUMP_API StreamPumpGetAPIExt(uint32_t func);
#if defined (__cplusplus)
        }
#endif
    };
};

#endif //__MC_STREAM_PUMP_ENGINE_API_H__

