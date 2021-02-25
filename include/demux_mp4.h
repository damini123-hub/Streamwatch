/* ----------------------------------------------------------------------------
 * File: demux_mp4.h
 *
 * Desc: MP4 Demuxer API
 *
 * Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.
 *
 * MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 * This software is protected by copyright law and international treaties.  Unauthorized
 * reproduction or distribution of any portion is prohibited by law.
 * ----------------------------------------------------------------------------
 */

/*! @file /demux_mp4.h
 */
#if !defined (__DEMUX_MP4_API_INCLUDED__)
#define __DEMUX_MP4_API_INCLUDED__

#include "mctypes.h"
#include "mcdefs.h"
#include "bufstrm.h"
#include "mcapiext.h"
#include "mcmediatypes.h"


/*! called to seek in the source file in splitter mode
/*!
* position - file position to seek to
* return 0 if OK, 1 if fault
*/
typedef int32_t (*mp4_splitter_seek_callback)(void* split_app_ptr, int64_t position);

/*! called to read data in splitter file mode
/*!
* read bufferSize bytes from file into buffer
* return number of bytes read, 0 if fault
*/
typedef int32_t (*mp4_splitter_read_callback)(void* split_app_ptr, uint8_t *buffer, int32_t bufferSize);


/*! external i/o function prototypes
/*!
* @note these are depreciated, the \ref mp4dmux_settings.p_external_io should be used instead
* @note if these are used, the \ref mpgdmux_settings.file_length field MUST be filled in
*       with the correct file size by the app
* called to open a file
* and the return value will be passed back to the seek, read and close functions
* ext_app_ptr - the ext_app_ptr field in the mpgdmux_settings structure is passed back here
* fileName - if a filename was passed to mpgdmux, it is returned here
* return the file handle as a long or -1 if an error occurs
*/
typedef long (*mp4_external_open_callback)(void *ext_app_ptr, char *fileName);
#if !defined(__APPLE__) && !defined(__linux__) && !defined(__vxworks)
typedef long (*mp4_external_open_callbackW)(void *ext_app_ptr, wchar_t *fileName);
#else
typedef long (*mp4_external_open_callbackW)(void *ext_app_ptr, uint16_t *fileName);
#endif

/*! called to seek in a file
/*!
* ext_app_ptr - the ext_app_ptr field in the mpgdmux_settings structure is passed back here
* fileHandle - a file handle returned by the fn_external_open_callback function
* position - the position to seek to
* return 0 if Ok, 1 if an error occurs
*/
typedef int (*mp4_external_seek_callback)(void *ext_app_ptr, long fileHandle, int64_t position);

/*! called to read from a file
/*!
* appData - the ext_app_ptr field in the mpgdmux_settings structure is passed back here
* fileHandle - a file handle returned by the fn_external_open_callback function
* buffer - a buffer for the data
* bufferSize - the number of bytes to read
* return the number of bytes actually read or 0 if an error occurs
*/
typedef int (*mp4_external_read_callback)(void *ext_app_ptr, long fileHandle, uint8_t *buffer, int bufferSize);

/*! called to close a file
/*!
* appData - the ext_app_ptr field in the mpgdmux_settings structure is passed back here
* fileHandle - a file handle returned by the fn_external_open_callback function
*/
typedef void (*mp4_external_close_callback)(void *ext_app_ptr, long fileHandle);


//! @name MP4 Demuxer object
typedef struct mp4_demuxer mp4dmux_tt;	//!< demuxer object


#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif

//! @brief Used to define the \ref mp4dmux_tt object
//! @details The demultiplexer uses the mp4dmux_settings structure to hold its settings
struct mp4dmux_settings
{
//! @{
//! @brief Splitter mode parameters.
    int32_t splitter_mode;									//!< @brief Indicates that the demuxer is being used in splitter mode.
    mp4_splitter_seek_callback splitter_seek_callback;		//!< @brief Seek callback for splitter mode.
    mp4_splitter_read_callback splitter_read_callback;		//!< @brief Read callback for splitter mode.
    void *split_app_ptr;									//!< @brief App pointer for splitter mode.
//!@}

//! @{
//! @brief External IO parameters.
    int32_t use_external_io;								//!< @brief Indicates that the app will provide the I/O for the demuxer.
    mp4_external_open_callback external_open_callback;		//!< @brief External open callback pointer.
    mp4_external_open_callbackW external_open_callbackW;	//!< @brief UNICODE external open callback pointer.
    mp4_external_seek_callback external_seek_callback;		//!< @brief External seek callback pointer.
    mp4_external_read_callback external_read_callback;		//!< @brief External read callback pointer.
    mp4_external_close_callback external_close_callback;	//!< @brief External close callback pointer.
    mc_external_io_t *p_external_io;						//!< @brief If non NULL and use_external_io = 1, this will be used instead of the above
    void *ext_app_ptr;										//!< @brief App pointer for external I/O mode, it is passed to the external callbacks.
//!@}

    int64_t file_length;						//!< @brief Length of the input file, used for splitter and external I/O modes only.

    uint8_t stop_parsing_after_mdat;			//!< @brief This field tells the parser to stop parsing after it reaches the MDAT atom.
												/*!< @details This can be useful with external I/O to simulate streaming decoding.
												* If this is enabled, all of the other atoms needed to demux the stream must be present before the MDAT atom.
												* If this is not the case, the file will be rejected as an unknown file.
												*/

    int32_t initial_buffer_size;				//!< @brief Size of buffer for file detection
												/*!< @details This is equivalent to the mp4dmux_parser_settings.requested_buffer_size setting.
												* When a file is opened, the demuxer creates an internal parser to do the initial read/detection.
												* This field specifies the file buffer size the demuxer uses to do this initial read/detection.
												*/

    uint8_t reserved[55];						//!< Reserved
};

//! @brief Used to define a parser object
struct mp4dmux_parser_settings
{
    int32_t requested_buffer_size;			//!< @brief Size in bytes the app wished the parser to use for bitstream buffers.

	uint8_t pull_mode;						//!< @brief Set to 1 if the app will call auxinfo to get data, otherwise the app will call mp4DemuxPush

    uint8_t add_adts_headers;				//!< @brief If this value equal 1, demultiplexer forming special headers before every AAC sample (ADTS header).

    uint8_t annexb_output;					//!< @brief If this value equal 1, demultiplexer replace sizes of H.264/AVC slices on special start code 0x0001.

    uint8_t use_field_rate;					//!< @brief If this value equal 1, demultiplexer used field rate for H.264/AVC time scale calculation.

    uint8_t detect_frames_types;			//!< @brief If this value equal 1, demultiplexer detect frame types (I,P,B etc.) for every video frame while parse file.

    uint8_t add_amr_header;					//!< @brief If this value equal 1, demultiplexer forming special headers before every AMR sample.

	uint8_t format_pcm_output_flag;			//!< @brief If this value is 1, the demuxer will output any PCM type streams as standard PCM format for the platform.

    uint8_t reserved[62];					//!< Reserved
};

//! @brief Used to define the stream info
typedef struct mp4dmux_stream_format_s
{
    uint32_t ID;                            //!< @brief This value equal for internal track ID. Unique for every track.

    mc_stream_format_t format;              //!< @brief This field describes the stream in a standard mc_stream_format_t structure defined in mcmediatypes.h

    int32_t ES_info_length;                 //!< @brief Equal elementary stream decoder configuration size in bytes. This information is necessary for decoders.

    uint8_t *descriptors;					//!< @brief Array of bytes which contain decoder configuration.

    uint64_t sample_count;                  //!< @brief Number of video frames for a video stream

    uint32_t samples_per_frame;             //!< @brief Number of audio samples in 1 sample_count above

    double duration;                        //!< @brief The duration of the stream in seconds.

    int32_t raw_decoder_config_len;         //!< @brief The size in bytes of the raw_decoder_config field.

    uint8_t *raw_decoder_config;            //!< @brief The decoder config (if any) as present in the container for this stream.

    uint32_t track_width;                   //!< @brief Width indicated in the track header

	uint32_t track_height;                  //!< @brief Height indicated in the track header

    int64_t first_timestamp;                //!< @brief First timestamp in track

    uint8_t reserved[40];                   //!< Reserved

} mp4dmux_stream_format_tt;

//! @brief Used to define the file info
struct mp4dmux_file_info
{
    int32_t stream_count;                   //!< @brief This value equal of total elementary streams count in file.

    double duration;                        //!< @brief Total file durations (in seconds). Equal duration of longest track.

    int32_t file_type;						//!< @brief This value equal ID for input stream type (MP4, 3GPP, QuickTime etc.).
											//!< @note At present moment, only one value available - STREAM_MP4.

    int64_t file_size;						//!< @brief Total input file size in bytes.

    int32_t file_info_length;               //!< @brief Reserved for future use.

    uint8_t *descriptors;					//!< @brief Reserved for future use.

    uint8_t reserved[64];					//!< Reserved
};

//! @brief Used to hold the stream settings
struct mp4dmux_stream_settings
{
    int32_t stream_num;			//!< @brief Internal stream number which. Used in MC API.
    bufstream_tt *bs;			//!< @brief Pointer to a bufstream_tt instance for the streams output

    uint8_t reserved[64];		//!< Reserved
};

//! @brief Used to define the seek info
struct mp4dmux_seek_info
{
    int32_t parser_num;			//!< @brief Parser number for the seek.
    int32_t stream_num;			//!< @brief Stream number for the seek.
    int64_t seek_sample;		//!< @brief Sample number to get.

    uint32_t ref_do;			//!< @brief Demuxing will start at this point after a seek if return_exact_sample = 0.
								/*!< @details For video streams this will be the reference frame display order where decoding must start to decode
								* the target frame. For audio streams, it will be the starting sample number of the compressed audio frame or uncompressed chunk.
								*/

    uint32_t ref_co;			//!< @brief Demuxing will start at this point after a seek if return_exact_sample = 0.
								/*!< For video streams this will be the reference frame coding order where decoding must start to decode the target frame.
								* For audio streams, this will be the starting sample number of the compressed audio frame or uncompressed chunk.
								*/

    int64_t ref_PTS;			//!< @brief The reference frame timestamp (27MHz units).

    uint32_t seek_do;			//!< @brief For video streams this will be the desired frame display order. For audio streams, this will be the desired sample number.
    uint32_t seek_co;			//!< @brief For video streams this will be the desired frame coding order. For audio streams, this will be the desired sample number.
    int64_t seek_PTS;			//!< @brief The desired samples timestamp (27MHz units).

    uint8_t return_exact_sample;	//!< @brief Set this to have the demuxer position the seek point to the desired sample instead of the reference frame needed to decode the desired sample.

	//!< @brief These do not change the file position, the merely return the appropriate time:
    uint8_t seek_next_key_frame;                //!< @brief return the time of the next key frame based on seek_sample
    uint8_t seek_prev_key_frame;                //!< @brief return the time of the previous key frame based on seek_sample

    double seek_start_time;						//!< @brief Seek start time in seconds
    double seek_stop_time;						//!< @brief Seek stop time in seconds

    uint8_t reserved[45];						//!< Reserved
};

#pragma pack(pop)


#ifdef __cplusplus
extern "C" {
#endif


//! call to create and initialize an demuxer instance
//!
//! @param[in]    get_rc_ex		pointer to a get_rc_ex resource function
//! @param[in]    rc_app_ptr	application supplied pointer that will be passed back with any get_rc calls
//! @param[in]    set			pointer to an mp4dmux_settings structure
//!
//! @return a pointer to a demuxer instance if successful, else NULL

mp4dmux_tt * MC_EXPORT_API mp4DemuxNew(void *(MC_EXPORT_API *get_rc_ex)(void *rc_app_ptr, const char* name), void *rc_app_ptr, struct mp4dmux_settings *set);


//! call to free a demuxer instance
//!
//! @param[in]    demuxer		pointer to a demuxer instance
//!
//! @return    none

void MC_EXPORT_API mp4DemuxFree(mp4dmux_tt *demuxer);


//! call to open a file for demuxing
//!
//! @param[in]    demuxer		pointer to a demuxer instance
//! @param[in]    reserved		reserved
//! @param[in]    filename		pointer to a filename to open
//!
//! @return    0 if successful, else non-zero

int32_t MC_EXPORT_API mp4DemuxOpen(mp4dmux_tt *demuxer, void *reserved, char *filename);

//! unicode version
#if !defined(__APPLE__) && !defined(__linux__) && !defined(__vxworks)

int32_t MC_EXPORT_API mp4DemuxOpenW(mp4dmux_tt *demuxer, void *reserved, wchar_t *filename);

#else

// this version will currently return an error!
int32_t MC_EXPORT_API mp4DemuxOpenW(mp4dmux_tt *demuxer, void *reserved, uint16_t *filename);

#endif


//! call to close file
//!
//! @param[in]    demuxer		pointer to a demuxer instance
//!
//! @return    none

void MC_EXPORT_API mp4DemuxClose(mp4dmux_tt *demuxer);


//! call to get information about the input file
//!
//! @param[in]    demuxer		pointer to a demuxer instance
//! @param[in]    file_info		pointer to a file_info_struct
//!
//! @return    0 if successful, else non-zero

int32_t MC_EXPORT_API mp4DemuxGetFileInfo(mp4dmux_tt *demuxer, struct mp4dmux_file_info *file_info);


//! call to get a description of a stream
//!
//! @param[in]    demuxer		pointer to a demuxer instance
//! @param[in]    stream_info	pointer to a mp4dmux_stream_format_tt
//! @param[in]    stream_num	zero-based stream number
//!
//! @return    0 if successful, else non-zero

int32_t MC_EXPORT_API mp4DemuxGetStreamFormat(mp4dmux_tt *demuxer, mp4dmux_stream_format_tt *stream_format, int32_t stream_num);


//! call to create and initialize a parser instance
//!
//! @param[in]    demuxer	pointer to a demuxer instance
//! @param[in]    set		pointer to a mp4dmux_parser_settings structure
//!
//! @return    a non-zero parser number if successful, else 0

int32_t MC_EXPORT_API mp4DemuxNewParser(mp4dmux_tt *demuxer,struct  mp4dmux_parser_settings *set);


//! call to free a parser instance
//!
//! @param[in]    demuxer		pointer to a demuxer instance
//! @param[in]    parser_num	parser number
//!
//! @return   none

void MC_EXPORT_API mp4DemuxFreeParser(mp4dmux_tt *demuxer, int32_t parser_num);


//! call to add a stream to a parser instance
//!
//! @param[in]    demuxer		pointer to a demuxer instance
//! @param[in]    parser_num	parser number
//! @param[in]    set			pointer to an mp4dmux_stream_settings structure
//!
//! @return    0 if successful, else non-zero

int32_t MC_EXPORT_API mp4DemuxAddStream(mp4dmux_tt *demuxer, int32_t parser_num, struct mp4dmux_stream_settings *set);


//! call to remove an output bufstream
//!
//! @param[in]    demuxer		pointer to a demuxer instance
//! @param[in]    parser_num	parser number
//! @param[in]    set			pointer to an mp4dmux_stream_settings structure
//!
//! @return    0 if successful, else non-zero

int32_t MC_EXPORT_API mp4DemuxRemoveStream(mp4dmux_tt *demuxer, int32_t parser_num, struct mp4dmux_stream_settings *set);


//! call to seek to the specified time
//!
//! @param[in]    demuxer		pointer to a demuxer instance
//! @param[in]    info			pointer to a mp4dmux_seek_info structure
//!
//! @return    0 if successful, 2 if EOF, 1 if an error occurs

int32_t MC_EXPORT_API mp4DemuxSeek(mp4dmux_tt *demuxer,struct  mp4dmux_seek_info *info);


//! call to push a packet through the parser
//!
//! @param[in]    demuxer		pointer to a demuxer instance
//! @param[in]    parser_num	parser number
//!
//! @return   \-1 if an error occurs, 0 if EOF else the number of bytes consumed from the source

int32_t MC_EXPORT_API mp4DemuxPush(mp4dmux_tt *demuxer, int32_t parser_num);


//! call to re-open a parser
//!
//! @param[in]    demuxer		pointer to a demuxer instance
//! @param[in]    parser_num	parser number
//!
//! @return   0 if successful, else non-zero

int32_t MC_EXPORT_API mp4DemuxWakeParser(mp4dmux_tt *demuxer, int32_t parser_num);


//! call to close the actual file of a parser, the parser is left intact and can be re-opened with the wake function
//!
//! @param[in]    demuxer		pointer to a demuxer instance
//! @param[in]    parser_num	parser number
//!
//! @return   0 if successful, else non-zero

int32_t MC_EXPORT_API mp4DemuxSleepParser(mp4dmux_tt *demuxer, int32_t parser_num);


//! call to have the file size updated if it is getting bigger
//!
//! @param[in]    demuxer		pointer to a demuxer instance
//! @param[in]    parser_num	parser number
//! @param[in]    file_size		the new file size if splitter or external i/o mode, else unused
//!
//! @return   0 if successful, else non-zero

int32_t MC_EXPORT_API mp4DemuxUpdateFilesize(mp4dmux_tt *demuxer, int32_t parser_num, int64_t file_size);


//! call to have the parser perform a seek before the next read and optionally update the internal app_data ptr
//!
//! @param[in]    demuxer		pointer to a demuxer instance
//! @param[in]    parser_num	parser number
//! @param[in]    split_app_ptr	can be used to updated the split_app_ptr from the mp4dmux_settings structure
//!
//! @return   0 if successful, else non-zero

int32_t MC_EXPORT_API mp4DemuxSetFirstSplitRead(mp4dmux_tt *demuxer, int32_t parser_num, void *split_app_ptr);


//! call to get extend API functions
//!
//! @param[in]   func	ID for function a pointer to get
//!
//! @return  function pointer or NULL

APIEXTFUNC MC_EXPORT_API mp4DemuxGetAPIExt(uint32_t func);

//! call to retrieve UV metadata
//!
//! @param[in]   demuxer	pointer to a demuxer instance
//! @param[in]   pBuff	buffer to the metadata; at the first call of the function, when API contributor doesn't know exact required data size this field should be NULL; at second call this pointer should be valid and allocated with the required size
//! @param[in]   iBuffSize	allocated buffer size; at the first call this argument should be 0; at the second call should be equal to the buffer allocation size
//! @param[in]   piRequiredBuffSize	this argument contains metadata size in bytes
//!
//! @return   0 if successful, else non-zero

int32_t MC_EXPORT_API mp4DemuxGetUVMetaData(mp4dmux_tt *demuxer, int8_t *pBuff, uint32_t iBuffSize, uint32_t *piRequiredBuffSize);


#ifdef __cplusplus
}
#endif




//! @name depreciated items
//! @brief use the enums in mcmediatypes.h instead
//! @{
#define MP4DMUX_MPEG4_VIDEO              0x01    //!< MPEG-4 video
#define MP4DMUX_H263_VIDEO               0x02    //!< h.263 video
#define MP4DMUX_H264_VIDEO               0x03    //!< h.264 video
#define MP4DMUX_H264_ANNEXB_VIDEO        0x04    //!< h.264 Annex B video
#define MP4DMUX_DV100_VIDEO              0x05    //!< DV100 video
#define MP4DMUX_JP2C_VIDEO               0x06    //!< JP2C video
#define MP4DMUX_MPEG2_VIDEO              0x07    //!< 13818-2 video
#define MP4DMUX_DVH6_VIDEO               0x08    //!< DVCPRO-HD 1080/60i
#define MP4DMUX_DVH5_VIDEO               0x09    //!< DVCPRO-HD 1080/50i
#define MP4DMUX_DVC_VIDEO                0x0A    //!< DVC
#define MP4DMUX_DVCPAL_VIDEO             0x0B    //!< DVC PAL
#define MP4DMUX_DVC50_VIDEO              0x0C    //!< DVCPRO-50
#define MP4DMUX_VC3_VIDEO                0x0D    //!< DNxHD
#define MP4DMUX_DVC50PAL_VIDEO           0x0E    //!< DVCPRO-50 PAL
#define MP4DMUX_DVH3_VIDEO               0x0F    //!< DVCPRO-HD 1080/24p/30p
#define MP4DMUX_DVHP_VIDEO               0x10    //!< DVCPRO-HD 720/24p/30p
#define MP4DMUX_DVHQ_VIDEO               0x11    //!< DVCPRO-HD 720/25p/50p
#define MP4DMUX_DVPP_VIDEO               0x12    //!< DVCPRO PAL
#define MP4DMUX_MPEG1_VIDEO              0x07    //!< 11172-2 video

#define MP4DMUX_AAC_AUDIO                0x21    //!< AAC audio
#define MP4DMUX_AMR_AUDIO                0x22    //!< AMR audio
#define MP4DMUX_MP3_AUDIO                0x23    //!< MP3 audio
#define MP4DMUX_TWOS_AUDIO               0x24    //!< TWOS LPCM audio
#define MP4DMUX_QT_PCM_AUDIO             0x25    //!< QuickTime PCM audio
#define MP4DMUX_AC3_AUDIO                0x26    //!< AC3 audio
#define MP4DMUX_EC3_AUDIO                0x27    //!< AC3+ audio
#define MP4DMUX_DTS_AUDIO                0x28    //!< DTS audio
#define MP4DMUX_ALAW_AUDIO               0x29    //!< A-Law audio
#define MP4DMUX_ULAW_AUDIO               0x30    //!< U-Law audio
#define MP4DMUX_IADPCM_AUDIO             0x31    //!< Intel ADPCM audio
#define MP4DMUX_MADPCM_AUDIO             0x32    //!< MS ADPCM audio

#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif

//! @brief  use mp4dmux_stream_format_tt instead
struct mp4dmux_stream_info
{
    int32_t sdk_stream_type;
    uint32_t ID;
    int32_t ES_info_length;
    uint8_t *descriptors;
    uint8_t twos_sample_size;
    uint32_t twos_sample_rate;
    uint8_t twos_channels;
    uint32_t avg_time_per_frame;
    uint64_t sample_count;
    double duration;
    uint8_t sample_size;
    uint32_t sample_rate;
    uint8_t channels;
    uint32_t avg_bytes_per_sec;
    uint32_t samples_per_frame;
    uint32_t width;
    uint32_t height;
    uint32_t avg_bitrate;
    uint32_t decoder_config_length;
    uint8_t *decoder_config;
    uint8_t reserved[7];
};

//! @name MP4 Demuxer object
typedef struct mp4_demuxer_push mp4dmx_push_tt;	//!< demuxer object

//! @brief Used to define the \ref mp4dmx_push_tt object
//! @details The demultiplexer uses the mp4dmux_push_settings structure to hold its settings
struct mp4dmux_push_settings
{
    uint8_t add_adts_headers;				//!< @brief If this value equal 1, demultiplexer forming special headers before every AAC sample (ADTS header).
    uint8_t annexb_output;					//!< @brief If this value equal 1, demultiplexer replace sizes of H.264/AVC slices on special start code 0x0001.
    uint8_t use_field_rate;					//!< @brief If this value equal 1, demultiplexer used field rate for H.264/AVC time scale calculation.
    uint8_t detect_frames_types;			//!< @brief If this value equal 1, demultiplexer detect frame types (I,P,B etc.) for every video frame while parse file.
    uint8_t add_amr_header;					//!< @brief If this value equal 1, demultiplexer forming special headers before every AMR sample.
    uint8_t format_pcm_output_flag;			//!< @brief If this value is 1, the demuxer will output any PCM type streams as standard PCM format for the platform.
};

#pragma pack(pop)


#ifdef __cplusplus
extern "C" {
#endif

//! @brief use mp4DemuxGetStreamFormat instead
int32_t MC_EXPORT_API mp4DemuxGetStreamInfo(mp4dmux_tt *demuxer, struct mp4dmux_stream_info *stream_info, int32_t stream_num);


//! call to create and initialize an push demuxer instance
//!
//! @param[in]    get_rc_ex     pointer to a get_rc_ex resource function
//! @param[in]    rc_app_ptr    application supplied pointer that will be passed back with any get_rc calls
//! @param[in]    set           pointer to an mp4dmux_settings structure
//!
//! @return a pointer to a demuxer instance if successful, else NULL

mp4dmx_push_tt * MC_EXPORT_API mp4DemuxPushNew(void *(MC_EXPORT_API *get_rc_ex)(void *rc_app_ptr, const char* name), void *rc_app_ptr, struct mp4dmux_push_settings *set);


//! call to free a push demuxer instance
//!
//! @param[in]    demuxer       pointer to a demuxer instance
//!
//! @return    none

void MC_EXPORT_API mp4DemuxPushFree(mp4dmx_push_tt *demuxer);

//! call to add a stream to a parser instance
//!
//! @param[in]    demuxer		pointer to a demuxer instance
//! @param[in]    parser_num	parser number
//! @param[in]    set			pointer to an mp4dmux_stream_settings structure
//!
//! @return    0 if successful, else non-zero

int32_t MC_EXPORT_API mp4DemuxPushAddStream(mp4dmx_push_tt *demuxer, struct mp4dmux_stream_settings *set);


//! call to remove a output bufstream
//!
//! @param[in]    demuxer		pointer to a demuxer instance
//! @param[in]    parser_num	parser number
//! @param[in]    set			pointer to an mp4dmux_stream_settings structure
//!
//! @return    0 if successful, else non-zero

int32_t MC_EXPORT_API mp4DemuxPushRemoveStream(mp4dmx_push_tt *demuxer, struct mp4dmux_stream_settings *set);


//! call to get information about the input file
//!
//! @param[in]    demuxer		pointer to a demuxer instance
//! @param[in]    file_info		pointer to a file_info_struct
//!
//! @return    0 if successful, else non-zero

int32_t MC_EXPORT_API mp4DemuxPushGetFileInfo(mp4dmx_push_tt *demuxer, struct mp4dmux_file_info *file_info);

//! call to get a description of a stream
//!
//! @param[in]    demuxer		pointer to a demuxer instance
//! @param[in]    stream_info	pointer to a mp4dmux_stream_format_tt
//! @param[in]    stream_num	zero-based stream number
//!
//! @return    0 if successful, else non-zero

int32_t MC_EXPORT_API mp4DemuxPushGetStreamFormat(mp4dmx_push_tt *demuxer, mp4dmux_stream_format_tt *stream_format, int32_t stream_num);

//! call to add a stream to a demuxer instance
//!
//! @param[in]    demuxer       pointer to a demuxer instance
//! @param[in]    bufstream_tt  pointer to bufstream
//!
//! @return   \-1 if an error occurs, 0 if EOF else the number of bytes consumed from the source

int32_t MC_EXPORT_API mp4DemuxPushInitStream(mp4dmx_push_tt *demuxer, bufstream_tt *bs);

#ifdef __cplusplus
}
#endif

//!@}

#endif // #if !defined (__DEMUX_MP4_API_INCLUDED__)

