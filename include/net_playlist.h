/**
@file net_playlist.h
@brief Playlist parsers

@verbatim
File: net_playlist.h
Desc: Playlist parsers

Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
This software is protected by copyright law and international treaties.  Unauthorized
reproduction or distribution of any portion is prohibited by law.
@endverbatim
**/

#if !defined (__MC_NET_PLAYLIST__)
#define __MC_NET_PLAYLIST__

#include "mctypes.h"
#include "net_common.h"

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

        typedef enum _tagPlaylistItemKeyType {

            MCPlaylistKeyNone = 0,
            MCPlaylistKeyAES128CBC = 1,
            MCPlaylistKeyAES128CTR = 2,
            MCPlaylistKeySampleAES = 3
        } MCPLAYLISTKEYTYPE;

		/**
         * @brief Type of playlist
         **/
		typedef enum _tagPlaylistType {
			MCPlaylistSimple = 0,						/**< Simple playlist */
			MCPlaylistVariants = 1,						/**< Extended HLS Playlist with quality variants */
			MCPlaylistSegments = 2,						/**< Extended Playlist with HLS segments */
			MCPlaylistSMIL = 3,							/**< SMIL Playlist */
            MCPlaylistSmooth = 4,						/**< Smooth Manifest file*/
            MCPlaylistDash = 5						/**< DASH MPD file*/
		} MCPLAYLISTTYPE;

		/**
         * @brief Type of playlist media item
         **/
		typedef enum _tagPlaylistItemMediaType {
			MCPlaylistMediaUnknown = 0,					/**< Undefined item type */
			MCPlaylistMediaVideo = 1,					/**< Video item */
			MCPlaylistMediaAudio = 2,					/**< Audio item */
			MCPlaylistMediaSubtitles = 3				/**< Subtitles item */
		} MCPLAYLISTMEDIATYPE;

		// forward declaration
		class IMCPlaylistItem;

		/**
         * @brief Interface for media stream enumeration
         **/
		class IMCPlaylistItemMediaSelector : public INetRefCounted
		{
		public:
			virtual ~IMCPlaylistItemMediaSelector() {};

			/**
			* @brief Get count of media streams
			*
			* @return	Count of available media streams
			**/
			virtual uint32_t GetMediaCount() = 0;

			/**
			* @brief Get media streams by index
			*
			* Caller must release returned item.
			*
			* @param[in] uiIndex	Index of media streams
			* @return	Valid pointer to IMCPlaylistItem or NULL if index is out of bounds.
			**/
			virtual IMCPlaylistItem *GetMediaItem( uint32_t uiIndex ) = 0;
		};

        /**
        @brief Presents media segment descriptor
        */
        class IMCSegmentInfo
        {
        public:
            virtual ~IMCSegmentInfo(){};

             /**
			* @brief Get segment start time
            *
            * @return Segment start time.
			**/
            virtual const uint64_t GetTime() const = 0 ;

            /**
			* @brief Segment duration
            *
            * @return Segment start time.
			**/

            virtual const uint64_t GetDuration() const  = 0;
            /**
			* @brief Segment timescale for other time related values.
            *
            * @return Segment timescale.
			**/
            virtual const uint64_t GetTimescale() const = 0;
        };

        /**
        @brief Presents media segment
        */
        class IMCPlaylistSegmentItem : public IMCAttributeEnumerator
        {
        public:
            virtual ~IMCPlaylistSegmentItem(){};
            /**
			* @brief Get URI for stream
            *
            * @return Segment location.
			**/
			virtual const IMCResourceLocation* GetLocation( ) const = 0;

            /**
			* @brief Get Location for init segment
            *
			* @return Location for initialization part for the segment. This part is needed for correct segment playback
			**/
            virtual const IMCResourceLocation* GetInitLocation( ) const = 0;

            /**
			* @brief Get segment descriptor
            *
			* @return	Valid IMCSegmentInfo* related to the segment
			**/
            virtual const IMCSegmentInfo* GetInfo() const = 0;
        };

        /**
        @brief Uses for enumeration available segments
        */
        class IMCPlaylistSegmentSelector : public INetRefCounted
        {
        public:
            virtual ~IMCPlaylistSegmentSelector() {}

            /**
			* @brief Get segment by its start time relative to presentation begin.

              The segment will have time less or equal requested time.

              @param[in] uiSegmentStartTime time offset of requested fragment
              @param[in] uiTimescale time scale for uiSegmentStartTime. 4/1 is the same as 360/90.
              @param[in] ui64CurrentDownloadRate current download rate in bits
			*
			* @return Valid pointer to IMCPlaylistSegmentItem or NULL if uiSegmentStartTime is out of bounds or segment is not yet available
			**/
            virtual IMCPlaylistSegmentItem* GetByTime( uint64_t uiSegmentStartTime, uint64_t uiTimescale, uint64_t ui64CurrentDownloadRate ) = 0;

            /**
			* @brief Get media segment follow by current
			*
			* Caller must release returned item.
			*
			* @param[in] pItem	current   item, could be NULL ptr, first item will be returned
            * @param[in] ui64CurrentDownloadRate current download rate in bits
			* @return	Valid pointer to IMCPlaylistSegmentItem or NULL if unavailable.
			**/
            virtual IMCPlaylistSegmentItem *GetNext( IMCPlaylistSegmentItem* pItem, uint64_t ui64CurrentDownloadRate ) = 0;
        };

		/**
         * @brief Quality variant information
         **/
		class IMCPlaylistQualityItem : public IMCAttributeEnumerator
		{
		public:
			virtual ~IMCPlaylistQualityItem() {};

			/**
			* @brief Get URI for stream
            *
            * @param[in] uiSegmentPosition time offset of requested fragment, meaningful only for Smooth Streaming, @deprecated will be replaced to IMCPlaylistSegmentSelector using
			* @return	zero-terminated string with URI or NULL if not available
			**/
			virtual const int8_t *GetQualityURI( uint64_t uiSegmentPosition = 0 ) = 0;

			/**
			* @brief Get bandwidth in bits per second for this quality item
			*
			* @return	Bandwidth value in bits per second or 0 if not available
			**/
			virtual uint32_t GetQualityBandwidth() = 0;

            /**
			* @brief Get segment enumerator
			*
			* @return	Valid IMCPlaylistSegmentSelector pointer when segment selector is available
			**/
            virtual IMCPlaylistSegmentSelector* GetSegmentSelector() = 0;

            /**
			* @brief Retrieves presentationTimeOffset
			* 
            * @param[out] pScale scale for the presentationTimeOffset
			* @return	presentationTimeOffset in units accordingly to the defined scale
			**/
			virtual const uint64_t GetPresentationTimeOffset( uint64_t *pScale ) const = 0;
		};

		/**
         * @brief Interface for quality variants enumeration
         **/
		class IMCPlaylistItemQualitySelector : public INetRefCounted
		{
		public:
			virtual ~IMCPlaylistItemQualitySelector() {};

			/**
			* @brief Get count of quality variants
			*
			* @return	Count of available  quality variants
			**/
			virtual uint32_t GetQualityCount() = 0;

			/**
			* @brief Get quality item by index
			*
			* Quality items are always sorted by bitrate with lowest bitrate first
			* Caller must release returned quality item.
			*
			* @param[in] uiIndex	Index of quality item
			* @return	Valid pointer to IMCPlaylistQualityItem or NULL if index is out of bounds.
			**/
			virtual IMCPlaylistQualityItem *GetQualityItem( uint32_t uiIndex ) = 0;

			/**
			* @brief Get quality item with highest bitrate
			*
			* Caller must release returned quality item.
			*
			* @param[out] uiIndex	Pointer to integer variable that will receive index of returned item
			* @return	Valid pointer to IMCPlaylistQualityItem
			**/
			virtual IMCPlaylistQualityItem *GetHighestQuality( uint32_t *uiIndex ) = 0;

			/**
			* @brief Get quality item with lowest bitrate
			*
			* Caller must release returned quality item.
			*
			* @param[out] uiIndex	Pointer to integer variable that will receive index of returned item
			* @return	Valid pointer to IMCPlaylistQualityItem
			**/
			virtual IMCPlaylistQualityItem *GetLowestQuality( uint32_t *uiIndex ) = 0;

			/**
			* @brief Get quality item closest to the specified bitrate
			*
			* Order of item matching:
			* 1. Search for strict bitrate match
			* 2. Search for closest item with bitrate lower than requested
			* 3. Search for closest item with any bitrate
			*
			* @param[in] uiBandwidth	Requested bandwidth in bits per second
			* @param[in] bStrictMatch	Set to true to search for strict bitrate match, false to search for both strict match and closest items
			* @param[out] uiIndex		Pointer to integer variable that will receive index of returned item
			* @return	Valid pointer to IMCPlaylistQualityItem or NULL if no matching items found
			**/
			virtual IMCPlaylistQualityItem *GetQualityByBandwidth( uint32_t uiBandwidth, bool bStrictMatch, uint32_t *uiIndex ) = 0;
		};

		/**
         * @brief Interface for encryption properties retrieving
         **/
        class IPlaylistItemEncryptionInfo : public INetRefCounted
        {
        public:
            virtual ~IPlaylistItemEncryptionInfo() {}

            /**
			* @brief Get encryption type
			*
			* @return	Encryption type @ref MCPLAYLISTKEYTYPE
			**/
            virtual MCPLAYLISTKEYTYPE GetEncryptionType() = 0;

            /**
			* @brief Retrieves initialization vector information
			*
			* @param[out] uiIVLen	initialization vector's length in bytes
			* @return	Valid pointer on the data which represents initialization vector, NULL when not available
			**/
            virtual const uint8_t * GetIV( uint32_t &uiIVLen ) = 0;

            /**
			* @brief Retrieves URI by which decryption key is available
			*
			* @return	Valid pointer on null terminated string, which represents URI (absolute or relative)
			**/
            virtual const int8_t * GetKeyURI() = 0;

            /**
			* @brief Retrieves key information
			*
			* 
			* @param[out] uiKeyLen	Key length in bytes
			* @return	Valid pointer on the data which represents a key for decryption, or NULL when key is not available
			**/
            virtual const uint8_t * GetKey( uint32_t &uiKeyLen ) = 0;

            /**
			* @brief Updates internally stored key and key length
			*
			* @param[in] pKey	    Valid pointer on the data which represents a key for decryption
            * @param[in] uiKeyLen	Key length in bytes
			**/
            virtual void UpdateKey( const uint8_t *pKey, uint32_t uiKeyLen ) = 0;
        };

		/**
         * @brief Interface for playlist item
         **/
		class IMCPlaylistItem : public IMCAttributeEnumerator
		{
		public:
			virtual ~IMCPlaylistItem() {};

			/**
			* @brief Get item title
			*
			* @return	String with item title or NULL if not available
			**/
			virtual const int8_t *GetTitle() = 0;

			/**
			* @brief Get duration of item
			*
			* @return	Item duration in seconds or 0 if not available
			**/
			virtual uint32_t GetDuration() = 0;

			/**
			* @brief Get item URI
			*
			* @return	Item URI string or NULL if not available
			**/
            virtual const int8_t *GetURI( ) = 0;

			/**
			* @brief Get item media type
			*
			* @return	Media type of item
			**/
			virtual MCPLAYLISTMEDIATYPE GetMediaType() = 0;

			/**
			* @brief Get quality selector for item
			*
			* @return	Valid pointer to IMCPlaylistItemQualitySelector or NULL if quality selection is not available for the item
			**/
			virtual IMCPlaylistItemQualitySelector *GetQualitySelector() = 0;

			/**
			* @brief Get additional media selector for item
			*
			* @return	Valid pointer to IMCPlaylistItemMediaSelector or NULL if item does not contain additional media streams
			**/
			virtual IMCPlaylistItemMediaSelector *GetMediaSelector() = 0;
		};

        /**
         * @brief Interface for dynamic playlist
         **/
		class IMCPlaylistInfo
		{
		public:
			virtual ~IMCPlaylistInfo() {};

			/**
			* @brief Get update period for dynamic playlist
			*
			* @return	Update period in milliseconds
			**/
			virtual const uint64_t GetUpdatePeriod() const = 0;

            /**
			* @brief Check if playlist is finalized on server
			*
			* @return	true if playlist is finalized and no further changes are allowed, false if playlist can be changed
			**/
			virtual const bool IsFinalized() const = 0;
		};

		/**
         * @brief Interface for playlist
         **/
		class IMCPlaylist : public IMCAttributeEnumerator
		{
		public:
			virtual ~IMCPlaylist() {};

			/**
			* @brief Get playlist type

			* @deprecated Will be changed to IMCPlaylistInfo using

			* @return	Playlist type
			**/
			virtual MCPLAYLISTTYPE GetType() = 0;

			/**
			* @brief Get playlist URI
			*
			* @return	String with playlist URI or NULL if not available
			**/
			virtual const int8_t *GetContentBase() = 0;

			/**
			* @brief Get count of items in playlist
			*
			* @return	Count of items in playlist
			**/
			virtual uint32_t GetItemCount() = 0;

			/**
			* @brief Get segment duration
			* @deprecated Will be changed to IMCPlaylistInfo using

			* @return	Segment duration in seconds or 0 if not available
			**/
			virtual uint32_t GetSegmentDuration() = 0;

			/**
			* @brief Get duration of playlist. Usually this is summary duration of all items in playlist
			* @deprecated Will be changed to IMCPlaylistInfo using
            *
			* @return	Playlist duration in seconds or 0 if not available
			**/
			virtual uint32_t GetDuration() = 0;

			/**
			* @brief Get playlist item by index
			*
			* Caller must release returned item.
			*
			* @param[in] uiIndex	Index of playlist item
			* @return	Valid pointer to IMCPlaylistItem or NULL if index is out of bounds.
			**/
			virtual IMCPlaylistItem *GetItem( uint32_t uiIndex ) = 0;

			/**
			* @brief Get sequence number of first item in playlist
			*
			* @return	Sequence number of first item in playlist
			**/
			virtual int64_t GetPlaylistSequenceNumber() = 0;

			/**
			* @brief Check if playlist is finalized on server
            *
            * This method is obsolete for Dash playlist. You should use IMCPlaylistInfo::IsFinalized() instead of.
			*
			* @return	true if playlist is finalized and no further changes are allowed, false if playlist can be changed
			**/
			virtual bool IsFinalized() = 0;

            /**
			* @brief Retrieve playlist information
			*
            * Provide easy way to get some additional information about playlist, is available not for each playlist type
            *
			* @return	Valid pointer to IMCPlaylistInfo or NULL if information is absent.
			**/
            virtual const IMCPlaylistInfo* GetInfo() const = 0;

            /**
			* @brief Retrieves encryption information
			* 
			* @return	Valid pointer to IPlaylistItemEncryptionInfo @ref IPlaylistItemEncryptionInfo, or NULL if information is absent.
			**/
            virtual IPlaylistItemEncryptionInfo * GetEncryptionInfo() = 0;
		};

        /**
            @brief Is Used for generating playlist instance
        */
		class IMCPlaylistParser : public INetRefCounted
		{
		public:
			virtual ~IMCPlaylistParser() {};

			/**
			* @brief Create empty playlist instance
			*
			* @param[in] pURI	URI of the playlist which can be used to build absolute URLs for playlsit items
			* @return	Pointer to new empty playlist instance or NULL if not enough memory
			**/
			virtual IMCPlaylist *CreatePlaylist( const int8_t *pURI ) = 0;

			/**
			* @brief Parse chunk of data
			*
			* A call to this method with iLen=0 and bEof=true is valid and can be used to signal end of data. This method is used for updating current playlists(i.e. DASH dynamic playlist).
             Feed new data with existing playlist instance as usual.
			*
			* @param[in] pBuffer	Pointer to buffer with playlist data
			* @param[in] iLen		Length of data in buffer
			* @param[in] bEof		Set to true if this is last chunk of data, set to false if there's more data to come
			* @param[in] pPlaylist	Pointer to playlist which was created by CreatePlaylist call
			*
			* @return	Count of bytes processed, 0 if more data is needed and -1 on parsing error
			**/
			virtual int32_t ParseChunk( const int8_t *pBuffer, int32_t iLen, bool bEof, IMCPlaylist *pPlaylist ) = 0;

			/**
			* @brief Reset internal parser state
			**/
			virtual void Reset() = 0;
		};

        /**
            @brief Is used for requesting additinal data for parsing playlist
        */
        class IMCReader : public INetRefCounted
        {
        public:
            virtual ~IMCReader(){}

            /**
                @brief Read to existing buffer size bytes beginning from uiOffset

                Semantic is similar to fread POSIX call. size might be zero.

                @param[in] pBuffer	Pointer to buffer for data
                @param[in] uiOffset	offset in data source
                @param[in] size   	amount of bytes for reading
                @return	Count of bytes read
            */
            virtual uint64_t Read( int8_t* pBuffer, uint64_t uiOffset, uint64_t size) = 0;
        };

        /**
            @brief Is used for getting time
        */
        class IMCClock : public INetRefCounted
        {
        public:
            typedef struct tm Date;

        public:
            virtual ~IMCClock(){};

            /**
                @brief Returns local time in UTC values

                @return Local time in UTC values
            */
            virtual Date GetCurrentUTCTime() const = 0;
        };

        /**
            @brief Builds primitives needed for parsing
        */
        class IMCParserFactory : public INetRefCounted
        {
        public:
            virtual ~IMCParserFactory(){};

            /**
                @brief Creates data source for presented URI

                @param[in] pUri   	data source location in RFC1738 form

                @return Valid pointer IMCReader* or NULL when data source couldn't be created
            */
            virtual IMCReader* Create(const int8_t* pUri) = 0;

            /**
                @brief Returns local time in UTC values

                @return Local time in UTC values
            */
            virtual IMCClock* Create() = 0;
        };

#if defined (__cplusplus)
        extern "C" {
#endif
            /**
            * @brief M3U Playlist parser provides functions to parse M3U playlists, supports HTTP Live Streaming and M3U8
            *
            * @param[in]  get_rc        Pointer to get_rc memory manager implementation or NULL to use standard allocators
            * @return                   Returns valid pointer on success and NULL when failed
            **/
            IMCPlaylistParser* NET_API_CALL GetM3UPlaylistParserInterface( void *(MC_EXPORT_API *get_rc)(const char*) );

			/**
            * @brief Youtube Playlist parser provides functions to parse Youtube video pages
            *
            * @param[in]  get_rc        Pointer to get_rc memory manager implementation or NULL to use standard allocators
            * @return                   Returns valid pointer on success and NULL when failed
            **/
			IMCPlaylistParser* NET_API_CALL GetYoutubePlaylistParserInterface( void *(MC_EXPORT_API *get_rc)(const char*) );

			/**
            * @brief SMIL Playlist parser, supporting DivX DSAS
            *
            * @param[in]  get_rc        Pointer to get_rc memory manager implementation or NULL to use standard allocators
            * @return                   Returns valid pointer on success and NULL when failed
            **/
			IMCPlaylistParser* NET_API_CALL GetSmilPlaylistParserInterface( void *(MC_EXPORT_API *get_rc)(const char*) );

            /**
            * @brief Smooth Streaming Playlist parser
            *
            * @param[in]  get_rc        Pointer to get_rc memory manager implementation or NULL to use standard allocators
            * @return                   Returns valid pointer on success and NULL when failed
            **/
			IMCPlaylistParser* NET_API_CALL GetSmoothPlaylistParserInterface( void *(MC_EXPORT_API *get_rc)(const char*) );

            /**
            * @brief DASH MPD parser
            *
            * @param[in]  get_rc        Pointer to get_rc memory manager implementation or NULL to use standard allocators
              @param[in]  pFactory      Application side factory implementation for providing primitives to parser
            * @return                   Returns valid pointer on success and NULL when failed
            **/
			IMCPlaylistParser* NET_API_CALL GetDashPlaylistParser( void *(MC_EXPORT_API *get_rc)(const char*), IMCParserFactory* pFactory);

#if defined (__cplusplus)
        }
#endif
    };
};

#endif //__MC_NET_PLAYLIST__
