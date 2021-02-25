/*!
 * @file mfimport.h
 * @brief Media Format Importer API
 * File: mfimport.h
 *
 * Desc: Media Format Importer API
 *
 * Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.
 *
 * MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 * This software is protected by copyright law and international treaties.  Unauthorized
 * reproduction or distribution of any portion is prohibited by law.
 */

#if !defined (__MFIMPORT_API_INCLUDED__)
#define __MFIMPORT_API_INCLUDED__

#include "mctypes.h"
#include "mcapiext.h"
#include "mcdefs.h"
#include "bufstrm.h"
#include "mcmediatypes.h"

//-----------------------------------------------------------------------------------------------
//
// new mfimportXXX API, the old mpegInXXX API follows the new
//
//-----------------------------------------------------------------------------------------------

//! @name Error codes
//! @anchor MFIERRORS
//!@{
#define mfiNoError                  0  //!< no error
#define mfiError                    1  //!< common error
#define mfiErrorInvalidParameter    2  //!< invalid parameter
#define mfiErrorEncryptedFile       3  //!< file is encrypted
#define mfiErrorNoMemory            4  //!< not enough memory
#define mfiErrorNoStreams           5  //!< no streams
#define mfiErrorNoIndexAvailable    6  //!< no index available
#define mfiErrorEndOfStream         7  //!< end of stream

//!@}

//! mfimport object
typedef struct mfimport_instance_tt mfimport_tt;

//! mfimport stream object
typedef struct mfimport_stream_instance_tt mfimport_stream_tt;


#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif

//! @name Available values for mfi_open_settings_t.flags
//! @anchor MFI_OPEN
//! @brief See mfi_open_settings_t.flags
//!@{
#define MFI_OPEN_GET_INFO_ONLY              0x00000001    //!< only get the container, title and stream information

                                                          //!< The file is left open for getXXXInfo calls but the
                                                          //!< seek, getNext and other functions will return an error.

#define MFI_OPEN_USE_EXT_IO                 0x00000002    //!< use external I/O settings in the ext_io field
#define MFI_OPEN_MP2_INDEX_MODE             0x00000004    //!< open the file in index mode, mp2 demuxer only
#define MFI_OPEN_MP2_NO_SEEKING             0x00000008    //!< enable if no duration and seeking (except to frame/sample 0) is required

                                                          //!< Use this if you just want to play file from the start, opening files can
                                                          //!< be quicker and problem files may be opened that cannot be opened in normal mode.
                                                          //!< The duration for all streams will be 0 and the seek functions will return
                                                          //!< an error if the seek position is not 0. mp2 demuxer only.

#define MFI_OPEN_MP2_GET_VTS_MENU           0x00000010    //!< enable to open the VTS menu, else the VTS contents are opened

                                                          //!< Used when opening a DVD VMG/VTS IFO fileset.

#define MFI_OPEN_MXF_IGNORE_EXT_STREAMS     0x00000020    //!< enable to ignore external MXF audio streams

                                                          //!< MXF demuxer only

//!@}

/*!
 @brief Container open settings structure, used by mfimportFileOpen and mfimportFileOpenW.
*/
typedef struct mfi_open_settings_s
{
    uint32_t flags;                                       //!< zero or more of the \ref MFI_OPEN "MFI_OPEN_xxx flags"

    mc_external_io_t ext_io;                              //!< external I/O settings, the mc_external_io_t structure is defined in mcdefs.h

    int32_t find_stream_limit;                            //!< number of megabytes of the input container to search for elementary streams

                                                          //!< A value of 0 means use the default value, used by mp2 demuxer only.

    uint8_t index_mode_required_action;                   //!< this flag specifies the action to take if the mp2 demuxer requires index mode in order to open the file and index mode was not enabled

                                                          //!< 0 = use index mode to open the file<br>
                                                          //!< 1 = return an error<br>
                                                          //!< 2 = call the prompt_index_mode_ex get_rc_ex callback

    uint8_t additional_GOP_search;                        //!< additional GOP's to search when determing pulldown, used by mp2 demuxer only

    bufstream_tt *p_idx_bs;                               //!< existing index bufstream to use when opening a file
    bufstream_tt *p_idx_bs_sec;                           //!< existing secondary index bufstream to use when opening a file

    int32_t reserved[62];                                 //!< reserved for future use

} mfi_open_settings_t;


/*!
 @brief Container information structure, filled by mfimportGetContainerInfo.
 */
typedef struct mfi_container_info_s
{
    mcmajortypes_t container_type;                        //!< container type, one of the mcmajortypes_t enums in mcmediatypes.h
    int32_t num_titles;                                   //!< the number of titles in the container

    void *p_demuxer_instance;                             //!< the demuxer instance

    int32_t reserved[64];                                 //!< reserved for future use

} mfi_container_info_t;


/*!
 @brief Title information structure, filled by mfimportGetTitleInfo.
 */
typedef struct mfi_title_info_s
{
    int32_t num_streams;                                  //!< the number of streams in this title
    int32_t title_id;                                     //!< title ID, format specific

    int32_t reserved[64];                                 //!< reserved for future use

} mfi_title_info_t;

//! @name Available values for mfi_stream_info_t.flags
//! @anchor MFI_STREAM_INFO
//! @brief See mfi_stream_info_t.flags
//!@{
#define MFI_STREAM_INFO_IS_DECODABLE        0x00000001    //!< stream can be decoded and decoder is present
#define MFI_STREAM_INFO_HAS_STILLS          0x00000002    //!< stream contains still frames

//!@}

/*!
 @brief Stream information structure, used by mfimportGetStreamInfo
 */
typedef struct mfi_stream_info_s
{
    mc_stream_format_t format;                            //!< stream format information, It is a mc_stream_format_t structure defined in mcmediatypes.h.
                                                          //!< This field combines the best guess info from the stream and container info contained in the
                                                          //!< respective fields below.

    int32_t stream_id;                                    //!< stream ID, format specific

                                                          //!< These fields are an approximate value, the real duration or number of samples in the stream
                                                          //!< can be slightly different. For example in an mp2 container a precise duration may not be
                                                          //!< computable without indexing. The total_samples field is simply calculated from the duration
                                                          //!< and sample rate above and may not be the actual number of samples.

    uint64_t total_samples;                               //!< total number of samples based on duration / stated best guess sample rate. If it is decided to
                                                          //!< use a sample rate other than the best guess sample rate, the user should recalculate the total
                                                          //!< samples as duration / new sample rate.

    uint64_t duration;                                    //!< duration of the stream in 27MHz units

    uint32_t flags;                                       //!< zero or more of the \ref MFI_STREAM_INFO "MFI_STREAM_INFO_xxx flags"

    uint32_t container_width;                             //!< container stated width of video stream, may be 0 if not present
    uint32_t container_height;                            //!< container stated height of video stream, may be 0 if not present

    int32_t view_count;                                   //!< number of views in the stream

    mc_stream_format_t container_format;                  //!< stream format information taken from container if any.

    int64_t first_timestamp;                              //!< first timestamp in the stream (used for streams win non-zero-based timestamps, works only for MP4 container)

    mc_stream_format_t stream_format;                     //!< stream format information detected from stream headers if any.

    int32_t reserved[33];                                 //!< reserved for future use

} mfi_stream_info_t;

//! @name Available values for mfi_stream_settings_t.flags
//! @anchor MFI_STREAM_SET
//! @brief See mfi_stream_settings_t.flags
//!@{
#define MFI_STREAM_SET_DISABLE_REORDER      0x00000001    //!< disable decoder reordering, get frames in coding order
#define MFI_STREAM_SET_CODED_ONLY           0x00000002    //!< return coded data only, no decoding is done
#define MFI_STREAM_SET_ENABLE_SUBVIEWS      0x00000004    //!< decode all subviews in the stream

//!@}

/*!
 @brief Stream instance settings structure, used by mfimportStreamNew
*/
typedef struct mfi_stream_settings_s
{
    int32_t title_idx;                                    //!< a zero based title number that contains the desired stream
    int32_t stream_idx;                                   //!< a zero based stream number

    uint32_t flags;                                       //!< zero or more of the \ref MFI_STREAM_SET "MFI_STREAM_SET_xxx flags"

    uint32_t decoder_parse_options;                       //!< sent to the decoder with the PARSE_OPTIONS command
    uint32_t requested_buffer_size;                       //!< requested parser buffer size in bytes, 0 = default value

    char *p_serial_number;                                //!< a serial number for decoders that require one

    int32_t reserved[64];                                 //!< reserved for future use

} mfi_stream_settings_t;


/*!
 @brief Stream instance information structure, filled by mfimportStreamNew
 */
typedef struct mfi_stream_instance_info_s
{
    bufstream_tt *p_decoder_instance;                     //!< the decoder instance
    uint8_t divx_decoder_in_use;                          //!< 1 if dec_divx decoder is in use
    uint8_t dd_plus_decoder_in_use;                       //!< 1 if dec_ddp decoder is in use

    uint8_t reserved[255];                                //!< reserved for future use

} mfi_stream_instance_info_t;


/*!
 @brief sample settings structure, used by mfimportStreamSeek and mfimportStreamGetNext
 */
typedef struct mfi_sample_settings_s
{
    uint32_t decoder_options;                             //!< options passed to the decoder with the PARSE_FRAMES command
    int32_t num_samples;                                  //!< requested number of samples

    //! @name video or subtitle
    //!@{
    frame_tt *p_frame;                                    //!< frame_tt for the decoded frame or subtitle overlay
    int64_t subtitle_time;                                //!< desired time of subtitle in 27MHz units for overlay

    //!@}

    //! @name audio and other
    //!@{
    uint8_t *p_buffer;                                    //!< buffer to receive the audio samples

    //!@}

    //!@name video sub views
    //!@{
    uint32_t subview_count;                               //!< number of frame_tt items in the next field
    frame_tt *p_subviews;                                 //!< pointer to an array of frame_tt's for the decoded subviews

    //!@}

    int32_t reserved[62];                                 //!< reserved for future use

} mfi_sample_settings_t;

//! @name Available values for mfi_sample_info_t.flags (video sample flags)
//! @anchor MFI_SAMPLE_VIDEO
//! @brief See mfi_sample_info_t.flags
//!@{
#define MFI_SAMPLE_IS_BROKEN                0x00000001    //!< the frame cannot be decoded correctly
#define MFI_SAMPLE_IS_IDR_TYPE              0x00000010    //!< indicates an IDR type frame
#define MFI_SAMPLE_IS_I_TYPE                0x00000020    //!< indicates an I type frame
#define MFI_SAMPLE_IS_P_TYPE                0x00000040    //!< indicates a P type frame
#define MFI_SAMPLE_IS_B_TYPE                0x00000080    //!< indicates a B type frame
#define MFI_SAMPLE_IS_PROGRESSIVE           0x00000100    //!< the frame is progressive, if known
#define MFI_SAMPLE_IS_INTERLACED            0x00000200    //!< the frame is interlaced, if known
#define MFI_SAMPLE_IS_TFF                   0x00000400    //!< top field first is specified, if known
#define MFI_SAMPLE_IS_BFF                   0x00000800    //!< bottom field first is specified, if known
#define MFI_SAMPLE_IS_RFF                   0x00001000    //!< repeat first field is specified
#define MFI_SAMPLE_IS_I_TYPE_FIELD2         0x00002000    //!< indicates the second field is an I type field, if known
#define MFI_SAMPLE_IS_P_TYPE_FIELD2         0x00004000    //!< indicates the second field is a P type field, if known
#define MFI_SAMPLE_IS_B_TYPE_FIELD2         0x00008000    //!< indicates the second field is a B type field, if known
#define MFI_SAMPLE_IS_MBAFF                 0x00010000    //!< the frame is MBAFF, if known
//!@}

//! @name Available values for mfi_sample_info_t.flags (audio sample flags)
//! @anchor MFI_SAMPLE_AUDIO
//! @brief See mfi_sample_info_t.flags
//!@{
#define MFI_SAMPLE_IS_FLOAT                 0x00000001    //!< samples are floating point, else integer
#define MFI_SAMPLE_IS_BIG_ENDIAN            0x00000002    //!< samples are big endian, else little endian
#define MFI_SAMPLE_IS_SIGNED                0x00000004    //!< samples are signed integers, else unsigned integers

//!@}

/*!
 @brief Sample information structure, filled by mfimportStreamSeek and mfimportStreamGetNext.
 */
typedef struct mfi_sample_info_s
{
    int64_t timestamp;                                    //!< timestamp of first sample in 27MHz units

                                                          //!< It is the timestamp of the first sample if multiple samples are returned.
                                                          //!< For audio tracks this value valid only at frame boundary (first audio sample in frame)
                                                          //!< set to -1 otherwise.

    int32_t num_samples_returned;                         //!< actual number of samples returned
    uint32_t flags;                                       //!< zero or more of MFI_SAMPLE_xxx (See \ref MFI_SAMPLE_VIDEO "MFI_SAMPLE_xxx flags for video samples" or \ref MFI_SAMPLE_AUDIO "MFI_SAMPLE_xxx flags for audio samples")

    int32_t coded_buffer_len;                             //!< raw coded data length

                                                          //!< This field contains the length of the data in p_coded_buffer.
                                                          //!< Only used if the stream was created with MFI_STREAM_SET_CODED_ONLY.

    uint8_t *p_coded_buffer;                              //!< a pointer to the raw coded data

                                                          //!< It is only used if the stream was created with MFI_STREAM_SET_CODED_ONLY.

    mcmediatypes_t stream_mediatype;                      //!< media type of this sample
    int32_t ext_info_len;                                 //!< size in bytes of the extended info (p_ext_info field)
    void *p_ext_info;                                     //!< a pointer to extended sample information

                                                          //!< One of the structures defined in mfimport_defs.h depending on stream_mediatype

    uint32_t cc_discon_count;                             //!< number of CC discontinuities the occurred in this sample, mp2 demuxer only

    int64_t duration;                                     //!< duration of samples in 27MHz units, subtitles only

    int32_t reserved[55];                                 //!< reserved for future use

} mfi_sample_info_t;

//! @name Available values for mfi_seek_settings_t.flags
//! @anchor MFI_SEEK
//! @brief See mfi_seek_settings_t.flags
//!@{
#define MFI_SEEK_BY_SAMPLE_NUMBER           0x00000000    //!< seek by sample number, sample_start represents a sample number.
                                                          //!< If neither the MFI_SEEK_CONTAINER_SAMPLE_RATE nor MFI_SEEK_STREAM_SAMPLE_RATE
                                                          //!< flags are present, sample_start is calculated using the sample rate from
                                                          //!< the mfi_stream_info_t.format structure.
#define MFI_SEEK_BY_TIME                    0x00000001    //!< seek by time, sample_start represents a time value in 27Mhz units. This is the
                                                          //!< preferred seek method!
#define MFI_SEEK_NEXT_KEY_FRAME             0x00000002    //!< returns next key frame/time relative to sample_start, does not do an actual seek, video only
#define MFI_SEEK_PREV_KEY_FRAME             0x00000004    //!< returns previous key frame/time relative to sample start, does not do an actual seek, video only
#define MFI_SEEK_CONTAINER_SAMPLE_RATE      0x00000008    //!< used with sample seeking, indicates sample_start is calculated using container sample rate
                                                          //!< from the mfi_stream_info_t.container_format structure.
#define MFI_SEEK_STREAM_SAMPLE_RATE         0x00000010    //!< used with sample seeking, indicates sample_start is calculated using stream sample rate
                                                          //!< from the mfi_stream_info_t.stream_format structure.
//!@}

/*!
 @brief Seek settings structure, used by mfimportStreamSeek.
 */
typedef struct mfi_seek_settings_s
{
    uint64_t sample_start;                                //!< on input - seek sample number or time, on output - actual sample number or time found

                                                          //!< Used for both input and output.

    uint32_t flags;                                       //!< zero or more of the \ref MFI_SEEK "MFI_SEEK_FLAGS_xxx flags"

    uint8_t avc_seek_start_point;                         //!< specify the method of reference frame seeking in AVC video streams

                                                          //!< 0 = seek to the nearest IDR or recovery point frame before the target frame,
                                                          //!< decode from IDR or recovery point to target frame (default)<br>
                                                          //!< <br>
                                                          //!< 1 = seek to the nearest frame with I slices before the target
                                                          //!< frame, decode from this frame to target frame (see below)<br>
                                                          //!< <br>
                                                          //!< other values indicate the number of frames before the target
                                                          //!< frame where decoding is to start. The actual starting frame
                                                          //!< will be the nearest frame with I slices before the value
                                                          //!< specified.
                                                          //!< <br>
                                                          //!< For example, if avc_seek_start_point = 30 and a seek is performed to
                                                          //!< frame 130, mpegin will backup to frame 100 (130 - 30) to start decoding
                                                          //!< until it reaches frame 130. If frame 100 does not contain any I slices
                                                          //!< but frame 98 does, the decoding will actually start at frame 98 instead
                                                          //!< of frame 100

    uint32_t decoder_parse_options;                       //!< sent to the decoder with the PARSE_OPTIONS command

                                                          //!< This will override any decoder_parse_options
                                                          //!< passed via the mfi_stream_settings_t structure.

    uint32_t decoder_frame_options;                       //!< sent to the decoder with the PARSE_FRAMES command

    uint8_t dvd_angle;                                    //!< the DVD angle to use for the seek

                                                          //!< 0 = leave unchanged, else select a new DVD angle.

    int32_t reserved[64];                                 //!< reserved for future use

} mfi_seek_settings_t;

#pragma pack(pop)


#ifdef __cplusplus
extern "C" {
#endif

//! @name new/free functions
//!@{

/*!
 @brief Call to create and initialize a mfimport_tt instance.

 @param [in] pfn_get_rc_ex  a pointer to a get_rc_ex resource function
 @param [in] p_app_ptr      a user defined pointer passed back to the get_rc_ex functions

 @return                    a pointer to a mfimport_tt instance if successful, else NULL
*/
mfimport_tt* MC_EXPORT_API mfimportNew(void *(*pfn_get_rc_ex)(void *p_app_ptr, const char *p_name),
                                       void *p_app_ptr);


/*!
 @brief Call to free a mfimport_tt instance.

 @param [in] p_mfimport  a pointer to a mfimport_tt instance

 @return                 none
*/
void MC_EXPORT_API mfimportFree(mfimport_tt *p_mfimport);

//!@}

//! @name file functions
//!@{

/*!
 @brief Call to open a file. mfimportFileOpenW is the unicode version of this function.

 @param [in] p_mfimport  a pointer to a mfimport_tt instance
 @param [in] p_filename  a pointer to a filename to open, can be NULL if external I/O is enabled
 @param [in] p_open_set  a pointer to a mfi_open_settings_t structure

 @return                 0 if successful, else non-zero
*/
int32_t MC_EXPORT_API mfimportFileOpen(mfimport_tt *p_mfimport,
                                       char *p_filename,
                                       mfi_open_settings_t *p_open_set);

/*!
 @brief Call to open a file, UNICODE version.

 @note This version will currently return an error on non-windows
 platforms if external I/O is not enabled.

 @param [in] p_mfimport  a pointer to a mfimport_tt instance
 @param [in] p_filename  a pointer to a UNICODE filename to open, can be NULL if external I/O is enabled
 @param [in] p_open_set  a pointer to a mfi_open_settings_t structure

 @return                 0 if successful, else non-zero
*/
int32_t MC_EXPORT_API mfimportFileOpenW(mfimport_tt *p_mfimport,
                                        uint16_t *p_filename,
                                        mfi_open_settings_t *p_open_set);


/*!
 @brief Call to close a file

 @param [in] p_mfimport  a pointer to a mfimport_tt instance

 @return                 none
*/
void MC_EXPORT_API mfimportFileClose(mfimport_tt *p_mfimport);

/*!
 @brief Call to sleep all streams, same as calling mfimportStreamSleep
 for all open streams.

 @details Must be followed by a call to mfimportFileWake
 before any of the other functions can be used again.

 @param [in] p_mfimport  a pointer to a mfimport_tt instance

 @return                 none
*/
void MC_EXPORT_API mfimportFileSleep(mfimport_tt *p_mfimport);

/*!
 @brief Call to wake all streams

 @param [in] p_mfimport  a pointer to a mfimport_tt instance

 @return                 0 if successful, else non-zero
*/
int32_t MC_EXPORT_API mfimportFileWake(mfimport_tt *p_mfimport);

/*!
 @brief Call to save an index to the specified bufstream.

 @param [in] p_mfimport  a pointer to a mfimport_tt instance
 @param [in] p_idx_bs    a pointer to a bufstream_tt instance

 @return                 0 if successful, else non-zero
*/
int32_t MC_EXPORT_API mfimportFileSaveIndex(mfimport_tt *p_mfimport, bufstream_tt *p_idx_bs);

//!@}

//! @name get info functions
//!@{

/*!
 @brief Call to get information about the container.

 @param [in] p_mfimport   a pointer to a mfimport_tt instance
 @param [in] p_info       a pointer to a mfi_container_info_t structure

 @return                  0 if successful, else non-zero
*/
int32_t MC_EXPORT_API mfimportGetContainerInfo(mfimport_tt *p_mfimport,
                                               mfi_container_info_t *p_info);

/*!
 @brief Call to get information about a title in the container.

 @param [in] p_mfimport  a pointer to a mfimport_tt instance
 @param [in] title_idx   a zero based title number
 @param [in] p_info      a pointer to a mfi_title_info_t structure

 @return                 0 if successful, else non-zero
*/
int32_t MC_EXPORT_API mfimportGetTitleInfo(mfimport_tt *p_mfimport,
                                           int32_t title_idx,
                                           mfi_title_info_t *p_info);


/*!
 @brief Call to get information about a stream in a title.

 @param [in] p_mfimport  a pointer to a mfimport_tt instance
 @param [in] title_idx   a zero based title number
 @param [in] stream_idx  a zero based stream number
 @param [in] p_info      a pointer to a mfi_stream_info_t structure

 @return                0 if successful, else non-zero
*/
int32_t MC_EXPORT_API mfimportGetStreamInfo(mfimport_tt *p_mfimport,
                                            int32_t title_idx,
                                            int32_t stream_idx,
                                            mfi_stream_info_t *p_info);

//!@}

//! @name stream functions
//!@{

/*!
 @brief Call to create a mfimport_stream_tt stream instance.

 @param [in] p_mfimport     a pointer to a mfimport_tt instance
 @param [in] p_stream_set   a pointer to a mfi_stream_settings_t structure
 @param [in] p_stream_info  a pointer to a mfi_stream_instance_info_t structure, can be NULL

 @return                    a pointer to a mfimport_stream_tt instance if successful, else NULL
*/
mfimport_stream_tt* MC_EXPORT_API mfimportStreamNew(mfimport_tt *p_mfimport,
                                                    mfi_stream_settings_t *p_stream_set,
                                                    mfi_stream_instance_info_t *p_stream_info);


/*!
 @brief Call to free a stream.

 @param [in] p_mfi_stream  a pointer to a mfimport_stream_tt instance

 @return                   none
*/
void MC_EXPORT_API mfimportStreamFree(mfimport_stream_tt *p_mfi_stream);


/*!
 @brief Call to seek to a particular frame or sample in a stream.

 @param [in] p_mfi_stream   a pointer to a mfimport_stream_tt instance
 @param [in] p_seek_info    a pointer to an app filled mfi_seek_settings_t structure
 @param [in] p_sample_set   a pointer to an app filled mfi_sample_settings_t structure, can be NULL
 @param [in] p_sample_info  a pointer to a mfi_sample_info_t structure, can be NULL

 @return                    0 if successful, else non-zero
*/
int32_t MC_EXPORT_API mfimportStreamSeek(mfimport_stream_tt *p_mfi_stream,
                                         mfi_seek_settings_t *p_seek_info,
                                         mfi_sample_settings_t *p_sample_set,
                                         mfi_sample_info_t *p_sample_info);


/*!
 @brief Call to get the next video frame or audio samples.

 @param [in] p_mfi_stream   a pointer to a mfimport_stream_tt instance
 @param [in] p_sample_set   a pointer to a mfi_sample_settings_t structure
 @param [in] p_sample_info  a pointer to a mfi_sample_info_t structure, can be NULL

 @return                    0 if successful, else non-zero
*/
int32_t MC_EXPORT_API mfimportStreamGetNext(mfimport_stream_tt *p_mfi_stream,
                                            mfi_sample_settings_t *p_sample_set,
                                            mfi_sample_info_t *p_sample_info);


/*!
 @brief Call to pass commands directly to the decoder instance of a stream

 @param [in] p_mfi_stream  a pointer to a mfimport_stream_tt instance
 @param [in] offs          same as bufstream auxinfo parameter
 @param [in] info_ID       same as bufstream auxinfo parameter
 @param [in] p_info_ptr    same as bufstream auxinfo parameter
 @param [in] info_size     same as bufstream auxinfo parameter

 @return                   return value from the decoder auxinfo function
*/
int32_t MC_EXPORT_API mfimportStreamAuxinfo(mfimport_stream_tt *p_mfi_stream,
                                            uint32_t offs,
                                            uint32_t info_ID,
                                            void *p_info_ptr,
                                            uint32_t info_size);


/*!
 @brief Call to close the file handle and free most memory for a stream
 @details This must be followed by a call to mfimportStreamWake before any
 of the other functions can be used again with the stream.

 @param [in] p_mfi_stream  a pointer to a mfimport_stream_tt instance

 @return                   none
*/
void MC_EXPORT_API mfimportStreamSleep(mfimport_stream_tt *p_mfi_stream);

/*!
 @brief Call to restore a stream after a call to mfimportStreamSleep

 @param [in] p_mfi_stream  a pointer to a mfimport_stream_tt instance

 @return                   0 if successful, else non-zero
*/
int32_t MC_EXPORT_API mfimportStreamWake(mfimport_stream_tt *p_mfi_stream);

//!@}

//! @name extended API functions
//!@{

/*!
 @brief Call to get extended API function

 @param [in] func  function ID

 @return           function pointer or NULL
*/
APIEXTFUNC MC_EXPORT_API mfimportGetAPIExt(uint32_t func);

//!@}

#ifdef __cplusplus
}
#endif
















//////////////////////////////////////////////////////////////////////////////////////
//
// old mpegInXXX API, this will eventually be depreciated in favor of the above API
//
//////////////////////////////////////////////////////////////////////////////////////

//! @name find stream options
//! @brief See mpegInInfo.find_stream_options
//! @anchor FIND_xxx
//!@{
#define FIND_NONE      0   //!< do the default search (all streams within the MB limit)
#define FIND_VIDEO     1   //!< stop searching after 1 video stream is found
#define FIND_AUDIO     2   //!< stop searching after 1 audio stream is found
#define FIND_VIDAUD    3   //!< stop searching after 1 video and 1 audio are found
#define FIND_SUBPIC    4   //!< stop searching after 1 subpic stream is found
#define FIND_VIDAUDSP  7   //!< stop searching after 1 video, 1 audio and 1 subpic are found
#define FIND_ALL       8   //!< find all streams within the MB search limit

//!@}

//! @name maximum number of streams supported
//! @anchor xxx_STREAM_MAX
//!@{
#define VIDEO_STREAM_MAX    16
#define AUDIO_STREAM_MAX    32        //!< mpeg and aes3 audio
#define AC3_STREAM_MAX      8
#define DTS_STREAM_MAX      16
#define SDDS_STREAM_MAX     8
#define PCM_STREAM_MAX      8
#define SUBPIC_STREAM_MAX   32
#define AMR_STREAM_MAX      8
#define WMA_STREAM_MAX      8

//!@}

//! @name additional time shifting return codes
//!@{
#define mpegInNoOutputInit 10
#define mpegInNoVideoInit  11
#define mpegInNoAudioInit  12
#define mpegInNoMPEG       13

//!@}

//! @name audio_mode
//! @brief See mpegInInfo.audioMode
//! @anchor MODE_xxx
//!@{
#define MODE_STEREO           0  //!< stereo mode
#define MODE_JOINT_STEREO     1  //!< joint stereo mode
#define MODE_DUAL_CHANNEL     2  //!< dual channel mode
#define MODE_MONO             3  //!< single channel (mono) mode

//!@}

//! @name control flags
//! @brief See \ref controlData callback
//! @anchor CONTROL_xxx
//!@{
#define CONTROL_OK            0
#define CONTROL_EOF           1  //!< called when eof is detected by mpegSplit

//!@}

#define PCM_AUDIO_BUFFER_SIZE 192000    //!< recommended PCM buffer size, 48000 * 4, enough for 1 second

//! decoder object, used internally
typedef struct mpeg_decoder mpegdec_tt;

//!@name splitter callbacks
//!@{

/*!
 @brief Called to seek in the source file in splitter mode
 @param [in] position  file position to seek to
 @return               0 if OK, 1 if fault
 */
typedef int32_t (*seekFileData)(void* lpData, int64_t position);

/*!
 @brief Called to read data in splitter file mode
 @details  read bufferSize bytes from file into buffer
 @param buffer      buffer for datat to be read to
 @param bufferSize  size of buffer
 @return            number of bytes read, 0 if fault
 */
typedef int32_t (*readFileData)(void* lpData, uint8_t *buffer, int32_t bufferSize);

/*!
 @brief Called when a new stream/substream is encountered
 @param [in] streamID     stream ID number
 @param [in] subStreamID  sub-stream ID number
 @return                  1 if stream is to be split, 0 if not
*/
typedef int32_t (*initOutputStream)(void* lpData, int32_t streamID, int32_t subStreamID);

/*!
 @brief Called with a chunk of stream data when available
 @param [in] streamID     stream ID number
 @param [in] subStreamID  sub-stream ID number
 @param [in] buffer       data buffer
 @param [in] bufferSize   number of bytes in buffer
 @return                  currently not used
 */
typedef int32_t (*sendStreamData)(void* lpData, int32_t streamID, int32_t subStreamID, uint8_t *buffer, int32_t bufferSize, int64_t timecode);

/*!
 @brief Callback with control information, one of the \ref CONTROL_xxx constants
 @param contorlID  control information
 @return           currently not used
 */
typedef int32_t (*controlData)(void* lpData, int32_t controlID);

/*!
 @brief Progress monitor prototype
 @param txt    human-readable progress string
 @param abort  abort flag
 @return       none
 */
typedef void (*mpegInProg)(char *txt, int32_t *abort);

//!@}

//! @name external i/o function prototypes
//! @note if these are used, the mpegInInfo.file_length field MUST be filled in
//!       with the correct file size by the app
//! @{

/*!
 @brief Called to open a file
 @details And it will be passed back to the seek, read and close functions.
 @param [in] appData   the appData field in the mpegInInfo structure is passed back here
 @param [in] fileName  if a filename was passed to mpegin, it is returned here
 @return               the file handle as a long or -1 if an error occurs
 */
typedef long (*externalOpen)(void *appData, char *fileName);

/*!
 @brief Called to open a file, UNICODE version
 @details And it will be passed back to the seek, read and close functions.
 @param [in] appData   the appData field in the mpegInInfo structure is passed back here
 @param [in] fileName  if a UNICODE filename was passed to mpegin, it is returned here
 @return               the file handle as a long or -1 if an error occurs
 */
#if !defined(__APPLE__) && !defined(__linux__) && !defined(__vxworks)
typedef long (*externalOpenW)(void *appData, wchar_t *fileName);
#else
typedef long (*externalOpenW)(void *appData, uint16_t *fileName);
#endif

/*!
 @brief Called to seek in a file.
 @param [in] appData     the appData field in the mpegInInfo structure is passed back here
 @param [in] fileHandle  a file handle returned by the externalOpen function
 @param [in] position    the position to seek to
 @return                 0 if Ok, 1 if an error occurs
*/
typedef int32_t (*externalSeek)(void *appData, long fileHandle, int64_t position);

/*!
 @brief Called to read from a file.
 @param [in] appData     the appData field in the mpegInInfo structure is passed back here
 @param [in] fileHandle  a file handle returned by the externalOpen function
 @param [in] buffer      a buffer for the data
 @param [in] bufferSize  the number of bytes to read
 @return                 the number of bytes actually read or 0 if an error occurs
 */
typedef int32_t (*externalRead)(void *appData, long fileHandle, uint8_t *buffer, int32_t bufferSize);

/*!
 @brief Called to close a file.
 @param [in] appData     the appData field in the mpegInInfo structure is passed back here
 @param [in] fileHandle  a file handle returned by the externalOpen function
 */
typedef void (*externalClose)(void *appData, long fileHandle);

//!@}

#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif

/*!
 @brief Contrainer for stream data.
*/
struct mpegin_stream_data
{
    uint16_t pid;                //!< program ID,transport stream only
    uint8_t id;                  //!< stream ID
    uint8_t sub_id;              //!< substream ID, DVD only

    uint16_t program_number;     //!< transport stream program number

    mcmediatypes_t stream_type;  //!< stream type from mcmediatypes.h

    uint32_t reserved;           //!< reserved for future use
};

/*!
 @brief hevc information structure
 @details Contains values of the like named fields in the HEVC stream headers
*/
typedef struct mfi_hevc_info_s
{
    // SPS items
    uint8_t general_progressive_source_flag;
    uint8_t general_interlaced_source_flag;
    uint8_t general_frame_only_constraint_flag;
    uint32_t chroma_format_idc;

    uint8_t frame_field_info_present_flag;
    uint8_t video_signal_type_present_flag;
    uint8_t video_format;
    uint8_t vui_timing_info_present_flag;
    uint32_t vui_num_units_in_tick;
    uint32_t vui_time_scale;
    uint8_t cbr_flag;

    // SEI items
    uint8_t pic_struct;

    int32_t reserved[64];

} mfi_hevc_info_t;


/*!
@brief AVC information structure
@details Contains values of the like named fields in the AVC stream headers
*/
typedef struct mfi_avc_ext_info_s
{
    int32_t     log2_max_frame_num;
    int32_t     pic_order_cnt_type;
    int32_t     log2_max_pic_order_cnt_lsb;
    int32_t     max_num_ref_frames;
    int32_t     num_ref_idx_l0_default_active;
    int32_t     num_ref_idx_l1_default_active;
    int32_t     pic_init_qp;
    int32_t     pic_init_qs;
    int32_t     chroma_qp_index_offset;
    int32_t     gaps_in_frame_num_value_allowed_flag;
    int32_t     vui_video_signal_type_flag;
    int32_t     vui_video_format;
    int32_t     vui_video_full_range_flag;
    int32_t     vui_colour_description_flag;
    int32_t     vui_colour_primaries;
    int32_t     vui_transfer_characteristics;
    int32_t     vui_matrix_coefficients;
    int32_t     vui_timing_info_flag;
    int32_t     vui_nal_hrd_parameters_flag;
    int32_t     vui_vcl_hrd_parameters_flag;
    int32_t     vui_bitstream_restriction_flag;
    int32_t     reserved[43]; // total length is 256 bytes(64 x int32_t)
} mfi_avc_ext_info_t;


/*!
 @brief mpegin structure
 @details To be compatible this structure should be 77400 bytes big.
*/
struct mpegInInfo
{
    // input items
    //! @name depreciated input items
    //!@{
    char *inputFilename;               //!< depreciated, pass filename to mpegInOpen function instead
    mpegInProg inProg;                 //!< depreciated, use get_rc prg_printf instead
    int32_t outputTypes;               //!< depreciated, use the use_external_frame_buffer and frame_info fields instead for video and the audio xxx_stream_to_get fields instead for audio
    int32_t currentTypes;              //!< depreciated, use the use_external_frame_buffer and frame_info fields instead for video and the audio xxx_stream_to_get fields instead for audio

    //!@}

    //! @name settings used with mpegInGetInfo or mpegInOpen (input items)
    //!@{
    int32_t FastAccess;                //!< do PTS searching instead of parsing entire file, default is true

                                       //!< This field specifies whether the file is opened in fast access mode or in indexed mode.
                                       //!< In fast access mode, the files are accessed using the container timestamps if present,
                                       //!< GOP timestamps for video streams and audio frame sizes for audio streams.
                                       //!< The parsing may not be frame accurate in this mode if the timestamps are not consistent
                                       //!< throughout the file.
                                       //!< <br><br>
                                       //!< In indexed mode, the file is parsed and an index of all the frames and samples is created
                                       //!< during the mpegInOpen call. This can take a while on large files, so the get_rc prg_printf
                                       //!< callback should be enabled to prevent the appearance of a program lockup.
                                       //!< This mode is frame accurate.
                                       //!< <br><br>
                                       //!< 0 = indexed mode<br>
                                       //!< 1 = fast access mode (default)

    int32_t Quiet_Flag;                //!< suppress verbose output, default is true

                                       //!< This field specifies whether the decoder will suppress informational messages
                                       //!< that may be generated.
                                       //!< <br><br>
                                       //!< 0 = do not suppress informational messages
                                       //!< 1 = suppress informational messages (default)

    int32_t additional_GOP_search;     //!< additional GOP's to search when determining video pulldown mode

                                       //!< Normally only the 1 GOP is searched.

    //!@}

    // output items
    //! @name container and stream info (output items)
    //!@{
    int32_t MPEG_Open;                 //!< whether a file is open or not, default is false

                                       //!< 0 = a file is not open<br>
                                       //!< 1 = a file is open

    int32_t program_stream_flag;       //!< the container stream type of the file, one of the STREAM_xxx defines in the mcdefs.h file

    // video output variables
    int32_t videoAvail;                //!< whether a video stream is available

                                       //!< 0 = a video stream was not found
                                       //!< 1 = a video stream was found

    //!@}

    //! @name selected video stream info (output items)
    //!@{
    int32_t totalFrames;               //!< the total number of video frames in the file

                                       //!< In fast access mode this may be incorrect if the timestamps
                                       //!< are not consistent throughout the file.

    int32_t horizontal_size;           //!< horizontal size of video
    int32_t vertical_size;             //!< vertical size of video
    int32_t MPEG2_video;               //!< whether a video stream is MPEG-1 (0) or MPEG-2 (1)
    double bit_rate;                   //!< the encoded bitrate in bits per second

                                       //!< The following two fields are mfimport's best guess at the actual frame rate (by default).
                                       //!< They contain one of the values from the stream_frame_rate or container_frame_rate fields below.
                                       //!< If the force_frame_rate field is non-zero, this field will contain the selected frame rate.

    double frame_rate;                 //!< the coded frame rate as a floating point value (25.0, 29.97, etc.) 

    double actual_frame_rate;          //!< the actual frame rate as a floating point value

                                       //!< If this field differs from the frame_rate field, this field represents the actual frame rate before pulldown is applied
                                       //!< The frame_rate field is the frame rate after pulldown is applied
    //!@}

    // video output buffers

    //! @name depreciated output items
    //!@{
    uint8_t *RGBBuffer;                //!< depreciated, use the use_external_frame_buffer and frame_info fields instead

                                       //!< RGB video buffer, BGRA bitmap.

    int32_t RGBRowSize;                //!< depreciated, use the use_external_frame_buffer and frame_info fields instead

                                       //!< Number of bytes in 1 row of RGB video data.
                                       //!< @note the YUV buffers are back-to-back in memory
                                       //!< so they can be treated as one large buffer starting
                                       //!< at YUVBuffers[0]

    uint8_t *YUVBuffers[3];            //!< depreciated, use the use_external_frame_buffer and frame_info fields instead

                                       //!< YUV video buffers, 0 = Y, 1 = U, 2 = V

    int32_t YRowSize;                  //!< depreciated, use the use_external_frame_buffer and frame_info fields instead

                                       //!< number of bytes in 1 row of Y video data

    int32_t YHeight;                   //!< depreciated, use the use_external_frame_buffer and frame_info fields instead

                                       //!< number of rows of Y video data

    int32_t UVRowSize;                 //!< depreciated, use the use_external_frame_buffer and frame_info fields instead

                                       //!< number of bytes in 1 row of U or V video data

    int32_t UVHeight;                  //!< depreciated, use the use_external_frame_buffer and frame_info fields instead

                                       //!< number of rows of U and V video data

    int32_t YUVFormat;                 //!< depreciated, use the use_external_frame_buffer and frame_info fields instead

                                       //!< YUV chroma format. one of the CHROMA* consts

    //!@}

    //! @name selected video stream info (output items)
    //!@{
    uint8_t *raw_buffer;               //!< a pointer to a raw video frame when SR output is enabled
    int32_t raw_buffer_size;           //!< the size in bytes of the data pointed to by the raw_buffer field

    //!@}

    // audio output variables
    //! @name container and stream info (output items)
    //!@{
    int32_t audioAvail;                //!< whether an audio stream is available

                                       //!< 0 = an audio stream was not found<br>
                                       //!< 1 an audio stream was found
    //!@}

    //! @name depreciated output items
    //!@{
    uint32_t totalSamples;             //!< depreciated, used the totalSample64 field instead

                                       //!< Total audio samples in file.

    //!@}

    //! @name selected audio stream info (output items)
    //!@{
    uint32_t numAudioBytes;            //!< the number of bytes returned in the output PCM buffer after a call to mpegInSeekSample64 or mpegInGetNextSamples
    uint32_t audioRate;                //!< the audio sampling rate, either 32000, 44100 or 48000
    int32_t stereo;                    //!< whether the audio is mono or more than one channel

                                       //!< 0 = the audio is mono
                                       //!< 1 = the audio has more than one channel

    uint32_t sampleSize;               //!< the size in bits of an output audio sample, will be 16
    int32_t audioLayer;                //!< the audio layer, MPEG/AAC audio only

                                       //!< 1 = layer 1
                                       //!< 2 = layer 2
                                       //!< 3 = layer 3

    int32_t audioMode;                 //!< the audio mode, MPEG audio only, one of the \ref MODE_xxx consts
    int32_t audioBitrate;              //!< MPEG audio bitrate of the file in 1000 byte units (a value of 224 indicates 224000 bps)
    int32_t bytesPerSample;            //!< the number of bytes in an audio sample (all channels)

    //!@}

    //! @name settings used while file is open (input items)
    //!@{
    int32_t get_yuv_frame;             //!< enable to retrieve a decoded video frame in addition to a RAW frame in SR mode
    int32_t get_pcm_samples;           //!< enable to get decoded PCM samples in addition to RAW frames in SR mode

    //!@}

    //! @name container and stream info (output items)
    //!@{
    int32_t no_frame_seek_available;   //!< frame seeking is not available, only used if the user_no_seeking flag is enabled

    //!@}

    //! @name depreciated output items
    //!@{
    uint8_t *raw_aud_buffer;           //!< raw audio buffer, no longer used
    int32_t raw_aud_size;              //!< size of frame in raw audio buffer, no longer used

    //!@}

    //! @name settings used while file is open (input items)
    //!@{
    int32_t flip_vertically;           //!< enable to flip the video frames vertically, only used if use_external_frame_buffer = 0

    //!@}

    //! @name selected video stream info (output items)
    //!@{
    int32_t sequence_header_found;     //!< true if sequence header was found with the current frame
    int32_t gop_header_found;          //!< true if gop_header_found was found with the current frame
    int32_t first_frame_repeat;        //!< true if the first frame in the stream had the repeat flag set

    //!@}

    //! @name settings used when opening a file (input items)
    //!@{
    frame_tt *frame_info;              //!< use to supply a frame_tt structure for the decoded video frames,

                                       //!< Used if use_external_frame_buffer is enabled.

    int32_t use_external_frame_buffer; //!< use the frame_info field below as the destination frame buffer

                                       //!< In this case the RGBBuffer and YUVBuffer fields are not used.
                                       //!< This must be enabled for H264 and VC1 video and is the preferred
                                       //!< method of decoding all video streams.


    //!@}

    //! @name selected audio stream info (output items)
    //!@{
    int64_t totalSamples64;            //!< the total number of samples in the file

                                       //!< In fast mode this may be incorrect if the PTS timestamps are not
                                       //!< consistent throughout the file.
    //!@}

    //! @name settings used with mpegInGetInfo or mpegInOpen (input items)
    //!@{
    int32_t requested_buffer_size;     //!< the requested buffer size in bytes for the internal file buffers, 0 = use default size

    //!@}

    //! @name depreciated input items
    //!@{
    int32_t use_io_threads;            //!< no longer supported, do not use

                                       //!< Do file i/o in threads, ignored in splitter mode.

    int32_t frame_size_flag;           //!< no longer supported, do not use

    //!@}

    //! @name selected video stream info (output items)
    //!@{
    int32_t first_frame_top;           //!< true if the first frame in the stream had the top field first flag set

    //!@}

    //! @name settings used when opening a file (input items)
    //!@{
    int32_t user_no_seeking;           //!< set to true if the duration and seeking (except to frame/sample 0) is required

                                       //!< Use this if you just want to play file from the start, opening files is quicker
                                       //!< and problem files may be opened that cannot be opened in normal mode.
                                       //!< The duration for all streams will be 0 and the seek functions will return
                                       //!< an error if the seek position is not 0

    int32_t fail_no_fast_mode;         //!< alter the fast/accurate behavior of the open functions

                                       //!< This field is only applicable when a file cannot be opened in fast mode
                                       //!< (using the timestamps in the file). If this is the case and this
                                       //!< field is 1, the open function will return an error and the no_fast_mode_flag
                                       //!< will be set to 1. If this function is 2, the open function will call the
                                       //!< prompt_index_mode function (either from get_rc or the mpegInInfo structure).
                                       //!< If the prompt_index_mode function returns 0, the open function will return an
                                       //!< error, if the prompt_index_mode function returns a non-zero value,
                                       //!< the open function will switch to index (Accurate) mode and parse the entire file
                                       //!< to determine the duration.
                                       //!< <br><br>
                                       //!< 0 open in fast or index mode<br>
                                       //!< 1 open in fast mode only<br>
                                       //!< 2 prompt user for index mode
    //!@}

    //! @name container and stream info (output items)
    //!@{
    int32_t no_fast_mode_flag;         //!< if the above field is true and a file cannot be opened in fast mode, the open call will return an error and this flag will be set

                                       //!< The application can decide whether to open in non-fast mode (FastAccess = 0)

    //!@}

    //! @name settings used when opening a file (input items)
    //!@{
    int32_t use_all_frames;            //!< use all frames/samples in the file

                                       //!< Normally video frames before the first I frame are discarded
                                       //!< as they cannot be decoded correctly. Enabling this setting will include those
                                       //!< frames.

    //!@}

    //! @name selected video stream info (output items)
    //!@{
    int32_t broken_frame;              //!< the video frame returned cannot be decoded
    int32_t padding_frame;             //!< the frame returned is a padding frame

    //!@}

    //! @name selected audio stream info (output items)
    //!@{
    int32_t padding_samples;           //!< the samples returned are padding samples
    int32_t broken_aframe;             //!< at least some of the sample returned could not be decoded

    //!@}

    //! @name settings used when opening a file (input items)
    //!@{
    int32_t fixed_frame_size_flag;     //!< inform mpegin that the input video elementary stream is D10 type video

                                       //!< Used if the GOP timecodes in the stream are all zeros.

    int32_t avc_seek_start_point;      //!< specify the method of reference frame seeking in AVC video

                                       //!< 0 = seek to the nearest IDR frame before the target frame,
                                       //!< decode from IDR to target frame (default).
                                       //!< <br><br>
                                       //!< 1 = seek to the nearest frame with I slices before the target
                                       //!< frame, decode from this frame to target frame
                                       //!< <br><br>
                                       //!< Other values (> 1) indicate the number of frames before the target
                                       //!< frame where decoding is to start. The actual starting frame
                                       //!< will be the nearest frame with I slices before the value
                                       //!< specified.
                                       //!< <br><br>
                                       //!< For example, if avc_seek_start_point = 30 and a seek is performed to
                                       //!< frame 130, mpegin will backup to frame 100 (130 - 30) to start decoding
                                       //!< until it reaches frame 130. If frame 100 does not contain any I slices
                                       //!< but frame 98 does, the decoding will actually start at frame 98 instead
                                       //!< of frame 100, i.e. it will always go back to an I-frame before
                                       //!< the specified value.
    //!@}

    //! @name selected video stream info (output items)
    //!@{
    int32_t pulldown;                  //!< if any pulldown was detected in the file
                                       //!< It will be one of the PULLDOWN_xxx constants defined in mcdefs.h

    uint32_t container_width;          //!< container stated width of video stream, may be 0 if not present
    uint32_t container_height;         //!< container stated height of video stream, may be 0 if not present
    uint32_t container_aspect_x;       //!< container stated x aspect ratio of video stream, may be 0 if not present
    uint32_t container_aspect_y;       //!< container stated y aspect ratio of video stream, may be 0 if not present

    //!@}

    //! @name selected video stream frame rate info
    //!@{
    uint8_t force_frame_rate;          //!< use to force mfimport to use one of the frame rates below (input item)
                                       //!< must be set before mpegInOpen is called.
                                       //!< set to 0 (default) to use mfimport's default frame rate
                                       //!< set to 1 to use container_frame_rate
                                       //!< set to 2 to use stream_frame_rate
    double container_frame_rate;       //!< container stated frame rate of video stream, may be 0 if not present (output item)
    double stream_frame_rate;          //!< frame rate from the video stream headers, may be 0 if not present (output item)

    //!@}

    uint8_t reserved1[3];              //!< reserved for future use

    int32_t reserved2[7];              //!< reserved for future use

    //! @name selected video stream info (output items)
    //!@{
    // header information
    //! video stream profile
    int32_t profile, level;            //!< video stream level

    //!@}

    int32_t reserved3[2];              //!< reserved for future use

    //! @name selected video stream info (output items)
    //!@{
    int32_t M;                         //!< the normal distance between I frames
    int32_t N;                         //!< the normal distance between P frames
    int32_t closed_gops;               //!< true if GOP's do not have the leading B frames

    // sequence_header
    int32_t aspect_ratio_information;  //!< values from the last sequence header, MPEG-1/2 video only

                                       //!< Note this field may now return 0 for non MPEG-1/2 video.
                                       //!< For AVC, MPEG-4 and other non MPEG-1/2 video use the
                                       //!< aspect_ratio_info_present_flag field and others below.

    int32_t frame_rate_code;           //!< values from the last sequence header, MPEG video only
    int32_t bit_rate_value;            //!< values from the last sequence header, MPEG video only
    int32_t vbv_buffer_size;           //!< values from the last sequence header, MPEG video only
    int32_t constrained_parameters_flag;  //!< values from the last sequence header, MPEG video only

    // sequence_extension
    int32_t profile_and_level_indication; //!< value from the last sequence_extension header, MPEG-2 video only
    int32_t progressive_sequence;         //!< value from the last sequence_extension header, MPEG-2 video only
    int32_t chroma_format;                //!< value from the last sequence_extension header, MPEG-2 video only
    int32_t low_delay;                    //!< value from the last sequence_extension header, MPEG-2 video only
    int32_t frame_rate_extension_n;       //!< value from the last sequence_extension header, MPEG-2 video only
    int32_t frame_rate_extension_d;       //!< value from the last sequence_extension header, MPEG-2 video only

    // sequence_display_extension
    int32_t video_format;                 //!<value from the last sequence_display_extension, MPEG-2 video only
    int32_t color_description;            //!<value from the last sequence_display_extension, MPEG-2 video only
    int32_t color_primaries;              //!<value from the last sequence_display_extension, MPEG-2 video only
    int32_t transfer_characteristics;     //!<value from the last sequence_display_extension, MPEG-2 video only
    int32_t matrix_coefficients;          //!<value from the last sequence_display_extension, MPEG-2 video only
    int32_t display_horizontal_size;      //!<value from the last sequence_display_extension, MPEG-2 video only
    int32_t display_vertical_size;        //!<value from the last sequence_display_extension, MPEG-2 video only

    // picture_header
    int32_t temporal_reference;        //!< value from the last picture_header, MPEG video only
    int32_t picture_coding_type;       //!< picture type of last picture decoded
    int32_t picture_type;              //!< picture type of current displayed frame
    int32_t vbv_delay;                 //!< value from the last picture_header, MPEG video only
    int32_t full_pel_forward_vector;   //!< value from the last picture_header, MPEG video only
    int32_t forward_f_code;            //!< value from the last picture_header, MPEG video only
    int32_t full_pel_backward_vector;  //!< value from the last picture_header, MPEG video only
    int32_t backward_f_code;           //!< value from the last picture_header, MPEG video only

    // picture_coding_extension header
    int32_t f_code[2][2];              //!< value from the last picture_coding_extension header, MPEG-2 video only
    int32_t intra_dc_precision;        //!< value from the last picture_coding_extension header, MPEG-2 video only
    int32_t picture_structure;         //!< value from the last picture_coding_extension header, MPEG-2 video only
    int32_t top_field_first;           //!< value from the last picture_coding_extension header, MPEG-2 video only
    int32_t frame_pred_frame_dct;      //!< value from the last picture_coding_extension header, MPEG-2 video only
    int32_t concealment_motion_vectors;//!< value from the last picture_coding_extension header, MPEG-2 video only
    int32_t intra_vlc_format;          //!< value from the last picture_coding_extension header, MPEG-2 video only
    int32_t repeat_first_field;        //!< value from the last picture_coding_extension header, MPEG-2 video only
    int32_t chroma_420_type;           //!< value from the last picture_coding_extension header, MPEG-2 video only
    int32_t progressive_frame;         //!< value from the last picture_coding_extension header, MPEG-2 video only
    int32_t composite_display_flag;    //!< value from the last picture_coding_extension header, MPEG-2 video only
    int32_t v_axis;                    //!< value from the last picture_coding_extension header, MPEG-2 video only
    int32_t field_sequence;            //!< value from the last picture_coding_extension header, MPEG-2 video only
    int32_t sub_carrier;               //!< value from the last picture_coding_extension header, MPEG-2 video only
    int32_t burst_amplitude;           //!< value from the last picture_coding_extension header, MPEG-2 video only
    int32_t sub_carrier_phase;         //!< value from the last picture_coding_extension header, MPEG-2 video only

    // picture_display_extension header
    int32_t frame_center_horizontal_offset[3];  //!< value from the last picture_display_extension header, MPEG-2 video only
    int32_t frame_center_vertical_offset[3];    //!< value from the last picture_display_extension header, MPEG-2 video only

    // copyright_extension header
    int32_t copyright_flag;           //!< value from the last copyright_extension header, MPEG-2 video only
    int32_t copyright_identifier;     //!< value from the last copyright_extension header, MPEG-2 video only
    int32_t original_or_copy;         //!< value from the last copyright_extension header, MPEG-2 video only
    int32_t copyright_number_1;       //!< value from the last copyright_extension header, MPEG-2 video only
    int32_t copyright_number_2;       //!< value from the last copyright_extension header, MPEG-2 video only
    int32_t copyright_number_3;       //!< value from the last copyright_extension header, MPEG-2 video only

    // group_of_pictures_header
    int32_t drop_flag;                //!< value from the last GOP type header
    int32_t hour;                     //!< value from the last GOP type header
    int32_t minute;                   //!< value from the last GOP type header
    int32_t sec;                      //!< value from the last GOP type header
    int32_t frame;                    //!< value from the last GOP type header
    int32_t closed_gop;               //!< value from the last GOP type header
    int32_t broken_link;              //!< value from the last GOP type header

    // sequence header and quant_matrix_extension
    int32_t intra_quantizer_matrix[64];             //!< value from the last sequence header or quant_matrix_extension header, MPEG video only
    int32_t non_intra_quantizer_matrix[64];         //!< value from the last sequence header or quant_matrix_extension header, MPEG video only
    int32_t chroma_intra_quantizer_matrix[64];      //!< value from the last sequence header or quant_matrix_extension header, MPEG video only
    int32_t chroma_non_intra_quantizer_matrix[64];  //!< value from the last sequence header or quant_matrix_extension header, MPEG video only

    int32_t load_intra_quantizer_matrix;            //!< value from the last sequence header or quant_matrix_extension header, MPEG video only
    int32_t load_non_intra_quantizer_matrix;        //!< value from the last sequence header or quant_matrix_extension header, MPEG video only
    int32_t load_chroma_intra_quantizer_matrix;     //!< value from the last sequence header or quant_matrix_extension header, MPEG video only
    int32_t load_chroma_non_intra_quantizer_matrix; //!< value from the last sequence header or quant_matrix_extension header, MPEG video only

    // picture coding extension
    int32_t q_scale_type;                           //!< value from the last picture coding extension, MPEG-2 video only
    int32_t alternate_scan;                         //!< value from the last picture coding extension, MPEG-2 video only

    int32_t number_of_frame_center_offsets;         //!< value from the last picture coding extension, MPEG-2 video only
    int32_t sequence_display_ext_present;           //!< indicates if a sequence_display_extension was present in the MPEG-2 video stream
    int32_t picture_display_ext_present;            //!< indicates if a picture_display_extension was present in the MPEG-2 video stream

    //!@}

    //! @name depreciated output items
    //!@{
    char savedInputFilename[512];                   //!< no longer used, may be used in the future for other things!

    //!@}

    //! @name selected video stream info (output items)
    //!@{
    // for formats other than mpeg-1/2, like AVC, mpeg-4...
    uint8_t aspect_ratio_info_present_flag;         //!< if aspect ratio information is present in the video stream, for non MPEG video stream types
    uint8_t aspect_ratio_idc;                       //!< the aspect_ratio_idc information present in the video stream, for non MPEG video stream types
    uint16_t aspect_ratio_width;                    //!< the aspect_ratio_width information present in the video stream, for non MPEG video stream types
    uint16_t aspect_ratio_height;                   //!< the aspect_ratio_height information present in the video stream, for non MPEG video stream types

    uint8_t avc_video_full_range_flag;              //!< the value of the video_full_range_flag in a H264 video stream

    //!@}

    uint8_t reserved4[1];                           //!< reserved for future use

    int32_t reserved5[256];                         //!< reserved for future use

    //! @name selected audio stream info (output items)
    //!@{
    // audio header
    int32_t channels;                               //!< the number of channels in the selected audio stream
    uint8_t aac_object_type;                        //!< the like named value from an AAC audio stream
    uint8_t aac_home;                               //!< the like named value from an AAC audio stream
    uint8_t aac_copyright_start;                    //!< the like named value from an AAC audio stream
    uint8_t aac_raw_data_blocks;                    //!< the like named value from an AAC audio stream
    int32_t aac_bytes_in_frame;                     //!< the like named value from an AAC audio stream
    int32_t bytesInFrame;                           //!< the number of bytes in a coded audio frame
    int32_t samplesInFrame;                         //!< the number of samples in a coded audio frame
    int32_t aac_buffer_fullness_bytes;              //!< the like named value from an AAC audio stream
    int32_t ID;                                     //!< the value of the like named field in MPEG and AAC ADTS audio streams
    int32_t layer;                                  //!< the value of the like named field in MPEG and AAC ADTS audio streams
    int32_t protection_bit;                         //!< the value of the like named field in MPEG audio streams
    int32_t bitrate_index;                          //!< the value of the like named field in MPEG audio streams
    int32_t sampling_frequency;                     //!< the value of the like named field in MPEG audio streams
    int32_t padding_bit;                            //!< the value of the like named field in MPEG audio streams
    int32_t private_bit;                            //!< the value of the like named field in MPEG audio streams
    int32_t mode_extension;                         //!< the value of the like named field in MPEG audio streams
    int32_t copyright;                              //!< the value of the like named field in MPEG audio streams
    int32_t original;                               //!< the value of the like named field in MPEG audio streams
    int32_t emphasis;                               //!< the value of the like named field in MPEG audio streams
    int32_t aac_flag;                               //!< indicates that the audio is an AAC stream

    //!@}

    //! @name container and stream info (output items)
    //!@{
    int32_t MPEG_type;                //!< type of mpeg stream, one of the MPEG_xxx defines

    //!@}

    //! @name selected video stream info (output items)
    //!@{
    int32_t current_MPEG_N;           //!< the size in frames of the current GOP

    //!@}

    //! @name depreciated output items
    //!@{
    uint32_t audio_streams_found;     //!< depreciated, use audio_streams field below
    uint32_t video_streams_found;     //!< depreciated, use video_streams field below

    //!@}

    //! @name container and stream info (output items)
    //!@{
    int32_t audio_stream_count;       //!< number of audio streams found

                                      //!< The number of MPEG/AAC audio streams found in a file,
                                      //!< it specifies how many items are in the audio_streams array

    int32_t video_stream_count;       //!< number of video streams found

                                      //!< the number of video streams found in a file,
                                      //!< it specifies how many items are in the video_streams array

    //!@}

    //! @name settings used when opening a file (input items)
    //!@{
    int32_t audio_stream_to_get;      //!< which audio stream to decode

                                      //!< A value of 1 means the first audio stream of the type found,
                                      //!< 2 means the second audio stream of the type found, etc.
                                      //!< The fields have the range 1 .. stream count, a value of 0 means
                                      //!< do not use a stream of this type. The fields are checked in the order
                                      //!< listed above.
                                      //!< If there are multiple audio stream types (for instance both a mpeg
                                      //!< and an ac3 audio stream are present) and the desired stream is the ac3 stream,
                                      //!< set audio_stream_to_get = 0 and ac3_stream_to_get = 1.
                                      //!< @note The aes3_stream_to_get includes HDMV type streams.
                                      //!< The audio stream must be selected before calling mpegInOpen.

    int32_t video_stream_to_get;      //!< which video stream to decode

                                      //!< A value of 1 means the first video stream found,
                                      //!< 2 the second video stream found, etc.
                                      //!< The video stream must be selected before calling mpegInOpen.

    //!@}

    //! @name depreciated input items
    //!@{
    int32_t fourCC;                   //!< Ddpreciated, use the use_external_frame_buffer and frame_info fields instead

                                      //!< Fourcc type, can be used in place of VIDEO_xxx constants

    //!@}

    //! @name settings used while file is open (input items)
    //!@{
    int32_t seek_if_dup;              //!< do a seek even if the current frame is equal to the seek frame

    //!@}

    int32_t reserved10[4086];         //!< reserved for future use

    //! @name settings used with mpegInGetInfo or mpegInOpen (input items)
    //!@{
    // splitter data
    int64_t file_length;              //!< the length of the input file if external i/o is used

    //!@}

    //! @name depreciated input items
    //!@{
    // splitter callbacks
    seekFileData seek_file_data;         //!< no longer supported, do not use
    readFileData read_file_data;         //!< no longer supported, do not use
    initOutputStream init_output_stream; //!< no longer supported, do not use
    sendStreamData send_stream_data;     //!< no longer supported, do not use
    controlData control_data;            //!< no longer supported, do not use

    //!@}

    //! @name selected video stream info (output items)
    //!@{
    int64_t first_vPTS;                  //!< the value of the first timestamp of the video stream in 27Mhz units
    int64_t last_vPTS;                   //!< the value of the last timestamp of the video stream in 27Mhz units
    int64_t seek_gop_vPTS;               //!< the value of the GOP timestamp of the target video frame after a seek in 27Mhz units
    int64_t ref_gop_vPTS;                //!< the value of the GOP timestamp of the reference video frame after a seek in 27Mhz units

    //!@}

    //! @name selected audio stream info (output items)
    //!@{
    int64_t first_aPTS;                  //!< the value of the first timestamp of the audio stream in 27Mhz units
    int64_t last_aPTS;                   //!< the value of the last timestamp of the audio stream in 27Mhz units

    //!@}

    //! @name selected video stream info (output items)
    //!@{
    // enough room for 140 bytes of seq hdr and 10 bytes of seq ext
    // plus 2 extra to make it a quad boundary
    int32_t seqhdr_length;               //!< how many bytes are in the seqhdr field
    uint8_t seqhdr[152];                 //!< contains the contents of the first sequence header (and sequence extension if MPEG-2) present in the target video stream

    //!@}

    //! @name selected audio stream info (output items)
    //!@{
    // audio header for MPEG, PCM and AC3 streams, 84 bytes
    int32_t audhdr_length;               //!< how many bytes are in the audhdr field
    uint8_t audhdr[84];                  //!< contains the contents of the audio stream header present in the target audio stream (MPEG, AAC, PCM and AC-3 only)

    //!@}

    //! @name other output items
    //!@{
    // private stream 1 header information, 6 bytes needed, 8 to pad to quad word
    // used for PCM, AC3, DTS and SDDS audio
    // includes the substream ID and packet parameters
    int32_t pvt1hdr_length;              //!< how many bytes are in the pvt1hdr field
    uint8_t pvt1hdr[8];                  //!< contains the contents of the private 1 stream header present in the target private 1 stream

    //!@}

    // private 1 audio streams

    // AC3
    //! @name depreciated output items
    //!@{
    uint32_t ac3_streams_found;     //!< depreciated, use ac3_streams field below

    //!@}

    //! @name container and stream info (output items)
    //!@{
    int32_t ac3_stream_count;       //!< number of ac3 audio streams found

                                    //!< The number of AC-3 audio streams found in a file,
                                    //!< it specifies how many items are in the ac3_streams array

    //!@}

    // DTS
    //! @name depreciated output items
    //!@{
    uint32_t dts_streams_found;     //!< depreciated, use dts_streams field below

    //!@}

    //! @name container and stream info (output items)
    //!@{
    int32_t dts_stream_count;       //!< number of dts audio streams found

                                    //!< The number of DTS audio streams found in a file,
                                    //!< it specifies how many items are in the dts_streams array.

    //!@}

    // SDDS
    //! @name depreciated output items
    //!@{
    uint32_t sdds_streams_found;    //!< depreciated, use sdds_streams field below

    //!@}

    //! @name container and stream info (output items)
    //!@{
    int32_t sdds_stream_count;      //!< number of sdds audio streams found

                                    //!< The number of SDDS audio streams found in a file,
                                    //!< it specifies how many items are in the sdds_streams array.

    //!@}

    // PCM
    //! @name depreciated output items
    //!@{
    uint32_t pcm_streams_found;     //!< depreciated, use pcm_streams field below

    //!@}

    //! @name container and stream info (output items)
    //!@{
    int32_t pcm_stream_count;       //!< number of pcm audio streams found

                                    //!< The number of PCM audio streams found in a file,
                                    //!< it specifies how many items are in the pcm_streams array

    //!@}

    //! @name depreciated input items
    //!@{
    int32_t audio_substream_to_get;    //!< depreciated, use respective stream_to_get fields

                                       //!< Audio substream to get from private 1 streams.

    //!@}

    //! @name selected audio stream info (output items)
    //!@{
    int32_t audio_type;                   //!< the audio type of the selected audio stream, one of the xxx_AUDIO defines in mcdefs.h

    // PCM info
    int32_t pcm_emphasis_flag;            //!< the value of the like named field in DVD LPCM audio streams
    int32_t pcm_mute_flag;                //!< the value of the like named field in DVD LPCM audio streams
    int32_t pcm_quantization_word_length; //!< the value of the like named field in DVD LPCM audio streams
    int32_t pcm_sampling_frequency;       //!< the value of the like named field in DVD LPCM audio streams
    int32_t pcm_number_of_channels;       //!< the value of the like named field in DVD LPCM audio streams
    int32_t pcm_dynamic_range_control;    //!< the value of the like named field in DVD LPCM audio streams

    // AC3 header info
    int32_t ac3_crc1;                     //!< the value of the like named field in AC3 audio streams
    int32_t ac3_fscod;                    //!< the value of the like named field in AC3 audio streams
    int32_t ac3_frmsizecod;               //!< the value of the like named field in AC3 audio streams

    int32_t ac3_bsid;                     //!< the value of the like named field in AC3 audio streams
    int32_t ac3_bsmod;                    //!< the value of the like named field in AC3 audio streams
    int32_t ac3_acmod;                    //!< the value of the like named field in AC3 audio streams
    int32_t ac3_cmixlev;                  //!< the value of the like named field in AC3 audio streams
    int32_t ac3_surmixlev;                //!< the value of the like named field in AC3 audio streams
    int32_t ac3_dsurmod;                  //!< the value of the like named field in AC3 audio streams
    int32_t ac3_lfeon;                    //!< the value of the like named field in AC3 audio streams
    int32_t ac3_dialnorm;                 //!< the value of the like named field in AC3 audio streams
    int32_t ac3_compre;                   //!< the value of the like named field in AC3 audio streams
    int32_t ac3_compr;                    //!< the value of the like named field in AC3 audio streams
    int32_t ac3_langcode;                 //!< the value of the like named field in AC3 audio streams
    int32_t ac3_langcod;                  //!< the value of the like named field in AC3 audio streams
    int32_t ac3_audprodie;                //!< the value of the like named field in AC3 audio streams
    int32_t ac3_mixlevel;                 //!< the value of the like named field in AC3 audio streams
    int32_t ac3_roomtyp;                  //!< the value of the like named field in AC3 audio streams
    int32_t ac3_dialnorm2;                //!< the value of the like named field in AC3 audio streams
    int32_t ac3_compr2e;                  //!< the value of the like named field in AC3 audio streams
    int32_t ac3_compr2;                   //!< the value of the like named field in AC3 audio streams
    int32_t ac3_langcod2e;                //!< the value of the like named field in AC3 audio streams
    int32_t ac3_langcod2;                 //!< the value of the like named field in AC3 audio streams
    int32_t ac3_audprodi2e;               //!< the value of the like named field in AC3 audio streams
    int32_t ac3_mixlevel2;                //!< the value of the like named field in AC3 audio streams
    int32_t ac3_roomtyp2;                 //!< the value of the like named field in AC3 audio streams
    int32_t ac3_copyrightb;               //!< the value of the like named field in AC3 audio streams
    int32_t ac3_origbs;                   //!< the value of the like named field in AC3 audio streams
    int32_t ac3_timecod1e;                //!< the value of the like named field in AC3 audio streams
    int32_t ac3_timecod1;                 //!< the value of the like named field in AC3 audio streams
    int32_t ac3_timecod2e;                //!< the value of the like named field in AC3 audio streams
    int32_t ac3_timecod2;                 //!< the value of the like named field in AC3 audio streams
    int32_t ac3_addbsie;                  //!< the value of the like named field in AC3 audio streams
    int32_t ac3_addbsil;                  //!< the value of the like named field in AC3 audio streams
    uint8_t ac3_addbsi[64];               //!< the value of the like named field in AC3 audio streams

    //!@}

    //! @name settings used while file is open (input items)
    //!@{
    // these two variables are set during an open call, they can be changed
    // afterward
    int32_t always_seek;                  //!< always do video seeks, do not do any advance frame calls

                                          //!< Mainly used for debugging and splitter mode.
                                          //!< This field is set during an open call, it can be changed afterward.
                                          //!< Enable to always do video seeks, do not do any advance frame calls.

    int32_t advance_limit;                //!< number of frames for which to do seek advances instead of an actual seek

                                          //!< Defaults to the number of frames per second.
                                          //!< This field is set during an open call, it can be changed afterward.
                                          //!< Set to the number of frames for which to do seek advances instead of an actual seek.
                                          //!< <br>
                                          //!< If the current frame is 100 and a seek is done to frame 105,
                                          //!< and this field is 30, no seek is actually performs, mfimport simply advances
                                          //!< to the target frame.

    //!@}

    //! @name depreciated input items
    //!@{
    struct mpegInInfo *rawInfo;           //!< no longer supported, do not use

                                          //!< Pointer to another mpegInInfo structure if needed.

    //!@}

    //! @name selected audio stream info (output items)
    //!@{
    int32_t ac3_timecode_flag;     //!< if an AC3 elementary stream has 16-byte timecodes before each audio frame

                                   //!< True if the file is an AC3 elementary stream with 16-byte timecodes
                                   //!< before each audio frame.

    //!@}

    //! @name container and stream info (output items)
    //!@{
    // subpicture items
    int32_t subpicAvail;           //!< whether a subpicture stream is available

                                   //!< 0 = a subpicture stream was not found<br>
                                   //!< 1 = a subpicture stream was found

    //!@}

    //! @name depreciated output items
    //!@{
    uint32_t subpic_streams_found; //!< depreciated, do not use

                                   //!< Combination of the \ref SUBPIC_STREAM_xxx constants

    //!@}

    //! @name container and stream info (output items)
    //!@{
    int32_t subpic_stream_count;   //!< number of subpic streams found

                                   //!< The number of subpicture streams found in a file,
                                   //!< it specifies how many items are in the subpic_streams array.

    //!@}

    //! @name settings used when opening a file (input items)
    //!@{
    int32_t subpic_stream_to_get;  //!< subpicture stream to decode

                                   //!< A value of 1 means the lowest subpicture stream found,
                                   //!< 2 the second stream found, etc.
                                   //!< The subpicture stream must be selected before calling
                                   //!< mpegInOpen.

    //!@}

    //! @name selected subtitle stream info (output items)
    //!@{
    int64_t first_sPTS;            //!< the value of the first timestamp of the subpicture stream in 27Mhz units
    int64_t last_sPTS;             //!< the value of the last timestamp of the subpicture stream in 27Mhz units
    // subpic header, includes the substream ID and packet parameters
    int32_t subpichdr_length;      //!< how many bytes are in the subpichdr field
    uint8_t subpichdr[8];          //!< contains any subpicture header data present in the target subpicture stream

                                   //!< Includes the substream ID and packet parameters.

    //!@}

    //! @name depreciated output items
    //!@{
    // transport stream items
    int32_t video_pids[16];        //!< depreciated, use video_streams field below
    int32_t audio_pids[32];        //!< depreciated, use audio_streams field below
    int32_t ac3_pids[8];           //!< depreciated, use ac3_streams field below
    int32_t dts_pids[8];           //!< depreciated, use dts_streams field below
    int32_t sdds_pids[8];          //!< depreciated, use sdds_streams field below
    int32_t pcm_pids[8];           //!< depreciated, use pcm_streams field below
    int32_t subpic_pids[32];       //!< depreciated, use subpic_streams field below

    //!@}

    //! @name settings used while file is open (input items)
    //!@{
    // parsing options that will be passed to the audio and video decoders
    int32_t video_parse_options;    //!< parsing option that will be passed to the video decoder

                                    //!< This field is passed to the video decoders with a PARSE_OPTIONS auxinfo call.
                                    //!< It can be used to pass options, like the video deinterlacing flags to the decoders.
                                    //!< Do not use the video_parse_options field to pass the INTERN_REORDERING_FLAG
                                    //!< to the video decoder as this is not compatible with mfimport.

    int32_t audio_parse_options;    //!< parsing option that will be passed to the audio decoder

                                    //!< This field is passed to the audio decoders with a PARSE_OPTIONS auxinfo call.
                                    //!< It can be used to pass options, like the video deinterlacing flags to the decoders.

    //!@}

    //! @name settings used with mpegInGetInfo or mpegInOpen (input items)
    //!@{
    int32_t find_stream_options;    //!< what streams to look for when searching the input file for stream information, one of \ref FIND_xxx defines

    //!@}

    //! @name settings used while file is open (input items)
    //!@{
    int32_t display_subpics;        //!< whether to display subpictures or not

                                    //!< It can be changed at any time with 1 meaning display the subpictures and
                                    //!< 0 meaning do not display the subpictures.

    //!@}

    //! @name container and stream info (output items)
    //!@{
    int64_t first_filePos;          //!< the file position where a valid video or audio stream starts

                                    //!< In a program stream it points to the pack header.

    //!@}

    //! @name settings used with mpegInGetInfo or mpegInOpen (input items)
    //!@{
    int32_t find_stream_limit;      //!< the number of megabytes of the input file to search for elementary streams

                                    //!< It is passed to the demuxer, a value of 0 means use the default built into the demuxer.

    //!@}

    //! @name settings used while file is open (input items)
    //!@{
    // items to send a palette to the subpic/video decoders
    int32_t set_subpic_palette;     //!< set to have the subpic palette below be the current palette

                                    //!< Set this field to 1 to send the subpicture palette contained
                                    //!< in the \ref subpic_palette field to mfimport.
                                    //!< <br>
                                    //!< The flag is cleared by mfimport once the values have been received.

    uint8_t subpic_palette[16][4];  //!< DVD type subpicture palette

                                    //!< The palette is NOT located in the VOB files,
                                    //!< it is located in one of the .IFO files.
                                    //!< mfimport has a default palette built into it, but it may not
                                    //!< look correct for all DVD’s.
                                    //!< The palette is composed of 16 4-byte entries in the same format that DVD
                                    //!< specifies (16 RYUV colors where R is reserved).
                                    //!< <br>
                                    //!< 16 24bpp YUV colors, 0 = reserved, 1 = Y, 2 = U, 3 = V, the DVD way!

    //!@}

    //! @name container and stream info (output items)
    //!@{
    // returned value, the number of titles in a title set
    int32_t dvd_num_titles;         //!< the number of titles in a VTS IFO fileset
    int32_t dvd_num_angles;         //!< the number of angles in the selected title of a VTS IFO fileset

    //!@}

    //! @name settings used with mpegInGetInfo or mpegInOpen (input items)
    //!@{
    // input controls
    int32_t dvd_language;           //!< which language unit is used when opening a menu in a DVD IFO file
    int32_t dvd_angle;              //!< which angle is used when opening a DVD VTS IFO fileset

    //!@}

    //! @name depreciated input items
    //!@{
    int32_t dvd_parental_level;     //!< currently not used

    //!@}

    //! @name settings used with mpegInGetInfo or mpegInOpen (input items)
    //!@{
    int32_t dvd_title_num;          //!< which title is used when opening a DVD IFO file
    int32_t dvd_get_vts_menu;       //!< whether the VTS menu or a VTS title is opened when a DVD VTS IFO fileset is opened

    //!@}

    //! @name selected video stream info (output items)
    //!@{
    int32_t h264_video_flag;        //!< the selected video stream is H.264 video

    //!@}

    //! @name settings used with mpegInGetInfo or mpegInOpen (input items)
    //!@{
    // external IO stuff
    externalOpen extOpen;            //!< exnernal I/O open callback pointer
    externalSeek extSeek;            //!< exnernal I/O seek callback pointer
    externalRead extRead;            //!< exnernal I/O read callback pointer
    externalClose extClose;          //!< exnernal I/O close callback pointer

    int32_t use_external_io;         //!< if this settings is enabled an application can handle the file i/o itself instead of the demuxers

                                     //!< The above fields and the file_length field have values.
                                     //!< The application can place function pointers in the
                                     //!< above fields and the demuxers will call these when they need to do any file i/o.

    void *appData;                   //!< an application defined pointer that is passed back to the external i/o functions

    //!@}

    //! @name container and stream info (output items)
    //!@{

    int32_t ts_extra_bytes;          //!< the number of extra bytes that precede each 188 bytes transport stream packet

                                     //!< Extra bytes after every transport stream packet that should be discarded.

    //!@}

    //! @name depreciated input items
    //!@{
#if !defined(__APPLE__) && !defined(__linux__) && !defined(__vxworks)
    wchar_t *inputFilenameW;         //!< depreciated, pass a filename to the mpegInOpen function instead

                                     //!< Pointer to a Unicode string input filename.
#else
    int32_t reserved12;              //!< reserved for future use
#endif

    //!@}

    //! @name settings used when opening a file (input items)
    //!@{
    int32_t h264_assume_frame_rate;  //!< enable this field to have mfimport treat the field rate specified in H264 streams as a frame rate instead of a field rate

                                     //!< Some broken H264 video streams put in a frame rate instead of a field rate.

    //!@}

    // AES3

    //! @name depreciated output items
    //!@{
    int32_t aes3_pids[8];            //!< depreciated, use aes3_streams field below
    uint32_t aes3_streams_found;     //!< depreciated, use aes3_streams_count field below

    //!@}

    //! @name container and stream info (output items)
    //!@{
    int32_t aes3_stream_count;       //!< the number of AES3 and HDMV audio streams found in a file

                                     //!< It specifies how many items are in the aes3_streams array.

    //!@}

    //! @name depreciated input items
    //!@{
    int32_t (*prompt_index_mode)();  //!< depreciated, pass a get_rc function pointer to mpegInOpen instead

    //!@}

    //! @name other output items
    //!@{
    bufstream_tt *dmux_chunk_bs;     //!< pointer to a bufstream that will receive demuxer chunk info in SR mode

    //!@}

    //! @name settings used with mpegInGetInfo or mpegInOpen (input items)
    //!@{
    externalOpenW extOpenW;          //!< exnernal I/O open callback pointer, UNICODE version

    //!@}

    //! @name selected video stream info (output items)
    //!@{
    int32_t mpeg4_video_flag;        //!< the selected video stream is MPEG-4 part 2 video

    //!@}

    //! @name selected audio stream info (output items)
    //!@{
    // extended ac3 header
    uint8_t ac3_strmtyp;             //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_substreamid;         //!< the value of the like named field in extended AC3 audio streams
    uint16_t ac3_frmsiz;             //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_fscod2;              //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_numblkscod;          //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_chanmape;            //!< the value of the like named field in extended AC3 audio streams
    uint16_t ac3_chanmap;            //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_mixmdate;            //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_dmixmod;             //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_ltrtcmixlev;         //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_lorocmixlev;         //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_ltrtsurmixlev;       //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_lorosurmixlev;       //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_lfemixlevcode;       //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_lfemixlevcod;        //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_pgmscle;             //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_pgmscl;              //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_pgmscl2e;            //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_pgmscl2;             //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_extpgmscle;          //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_extpgmscl;           //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_mixdef;              //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_mixdeflen;           //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_mixdata[32];         //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_paninfoe;            //!< the value of the like named field in extended AC3 audio streams
    uint16_t ac3_paninfo;            //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_paninfo2e;           //!< the value of the like named field in extended AC3 audio streams
    uint16_t ac3_paninfo2;           //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_frmmixcfginfoe;      //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_blkmixcfginfo[6];    //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_infomdate;           //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_dheadphonmod;        //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_blkmixcfginfoe;      //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_dsurexmod;           //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_adconvtyp;           //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_adconvtyp2;          //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_sourcefscod;         //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_convsync;            //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_blkid;               //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_xbsi1e;              //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_xbsi2e;              //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_xbsi2;               //!< the value of the like named field in extended AC3 audio streams
    uint8_t ac3_encinfo;             //!< the value of the like named field in extended AC3 audio streams

    uint8_t dd_plus_decoder_in_use;  //!< 1 indicates dec_ddp decoder is being used
    uint8_t dd_plus_hybrid_flag;     //!< 1 indicates the stream is a DD+ hybrid stream

    uint8_t reserved11_1[3];         //!< reserved for future use

    //!@}

    int32_t reserved11[5896];        //!< reserved for future use

    //! @name settings used while file is open (input items)
    //!@{
    uint32_t video_getpic_options;   //!< passed to the video encoders with the GET_PIC auxinfo call

                                     //!< It can be used to pass options, like the video deinterlacing
                                     //!< flags to the decoders.
    //!@}

    //! @name depreciated output items
    //!@{
    uint8_t h264_dpb_size;           //!< no longer used

    //!@}

    //! @name selected video stream info (output items)
    //!@{
    uint8_t h264_cbr_flag;           //!< whether a H264 video stream is constant bitrate

    //!@}

    //! @name settings used when opening a file (input items)
    //!@{
    uint8_t audio_pass_through_flag; //!< enable to pass through audio to the audio pass through component

    //!@}

    //! @name selected audio stream info (output items)
    //!@{
    uint8_t audio_is_floating_point;  //!< whether the output PCM audio is floating point (1) or integer (0).

    //!@}

    //! @name settings used when opening a file (input items)
    //! Note on the audio stream_to_get fields, including the
    //! audio_stream_to_get fields above.
    //! <br>
    //! The fields have the range 1 .. stream count, a value of
    //! 0 means do not get a stream of this type.
    //! <br><br>
    //! The fields are checked in this order:
    //! <br>
    //! audio_stream_to_get<br>
    //! pcm_stream_to_get<br>
    //! ac3_stream_to_get<br>
    //! aes3_stream_to_get<br>
    //! amr_stream_to_get<br>
    //! wma_stream_to_get<br>
    //! dts_stream_to_get<br>
    //! <br>
    //! If there are multiple audio stream types (for instance both an mpeg
    //! and an ac3 audio stream are present) and the desired stream is the ac3
    //! stream, set audio_stream_to_get = 0 and ac3_stream_to_get = 1.

    //!@{

    int32_t pcm_stream_to_get;       //!< specify which audio stream to decode

                                     //!< A value of 1 means the first audio stream of the
                                     //!< type found, 2 means the second audio stream of the type found, etc.
                                     //!< The field has the range 1 .. stream count,
                                     //!< a value of 0 means do not use a stream of this type.
                                     //!< The field is checked in the order listed above.
                                     //!< If there are multiple audio stream types
                                     //!< (for instance both a mpeg and an ac3 audio stream are
                                     //!< present) and the desired stream is the ac3 stream,
                                     //!< set audio_stream_to_get = 0 and ac3_stream_to_get = 1.
                                     //!< The audio stream must be selected
                                     //!< before calling mpegInOpen.

    int32_t ac3_stream_to_get;       //!< specify which audio stream to decode

                                     //!< A value of 1 means the first audio stream of the
                                     //!< type found, 2 means the second audio stream of the type found, etc.
                                     //!< The field has the range 1 .. stream count,
                                     //!< a value of 0 means do not use a stream of this type.
                                     //!< The field is checked in the order listed above.
                                     //!< If there are multiple audio stream types
                                     //!< (for instance both a mpeg and an ac3 audio stream are
                                     //!< present) and the desired stream is the ac3 stream,
                                     //!< set audio_stream_to_get = 0 and ac3_stream_to_get = 1.
                                     //!< The audio stream must be selected
                                     //!< before calling mpegInOpen.

    int32_t aes3_stream_to_get;      //!< specify which audio stream to decode

                                     //!< Includes HDMV LPCM!<br>
                                     //!< A value of 1 means the first audio stream of the
                                     //!< type found, 2 means the second audio stream of the type found, etc.
                                     //!< The field has the range 1 .. stream count,
                                     //!< a value of 0 means do not use a stream of this type.
                                     //!< The field is checked in the order listed above.
                                     //!< If there are multiple audio stream types
                                     //!< (for instance both a mpeg and an ac3 audio stream are
                                     //!< present) and the desired stream is the ac3 stream,
                                     //!< set audio_stream_to_get = 0 and ac3_stream_to_get = 1.
                                     //!< The audio stream must be selected
                                     //!< before calling mpegInOpen.
                                     //!< @note The aes3_stream_to_get includes HDMV type streams.

    //!@}

    //! @name container and stream info (output items)
    //!@{
    struct mpegin_stream_data video_streams[VIDEO_STREAM_MAX];    //!< contains information for each stream found in the container
    struct mpegin_stream_data audio_streams[AUDIO_STREAM_MAX];    //!< contains information for each stream found in the container
    struct mpegin_stream_data ac3_streams[AC3_STREAM_MAX];        //!< contains information for each stream found in the container
    struct mpegin_stream_data aes3_streams[AUDIO_STREAM_MAX];     //!< contains information for each stream found in the container
    struct mpegin_stream_data dts_streams[DTS_STREAM_MAX];        //!< contains information for each stream found in the container
    struct mpegin_stream_data sdds_streams[SDDS_STREAM_MAX];      //!< contains information for each stream found in the container
    struct mpegin_stream_data pcm_streams[PCM_STREAM_MAX];        //!< contains information for each stream found in the container
    struct mpegin_stream_data subpic_streams[SUBPIC_STREAM_MAX];  //!< contains information for each stream found in the container
    struct mpegin_stream_data amr_streams[AMR_STREAM_MAX];        //!< contains information for each stream found in the container

    //!@}

    //! @name settings used when opening a file (input items)
    //!@{
    int32_t amr_stream_to_get;       //!< specify which audio stream to decode

                                     //!< A value of 1 means the first audio stream of the
                                     //!< type found, 2 means the second audio stream of the type found, etc.
                                     //!< The field has the range 1 .. stream count,
                                     //!< a value of 0 means do not use a stream of this type.
                                     //!< The field is checked in the order listed above.
                                     //!< If there are multiple audio stream types
                                     //!< (for instance both a mpeg and an ac3 audio stream are
                                     //!< present) and the desired stream is the ac3 stream,
                                     //!< set audio_stream_to_get = 0 and ac3_stream_to_get = 1.
                                     //!< The audio stream must be selected
                                     //!< before calling mpegInOpen.

    //!@}

    //! @name container and stream info (output items)
    //!@{
    int32_t amr_stream_count;        //!< the number of AMR audio streams found in a file

                                     //!< It specifies how many items are in the amr_streams array.

    //!@}

    //! @name selected video stream info (output items)
    //!@{
    int32_t DivX_decoder_in_use;     //!< whether the DivX decoder is in use

    int32_t jpeg2k_video_flag;       //!< the selected video stream is JPEG 2000 video

    //!@}

    //! @name settings used when opening a file (input items)
    //!@{
    int32_t disable_reordering;      //!< enable to get decoded frames in coding order instead of display order

                                     //!< Only used if use_external_frame_buffer = 1.

    //!@}

    //! @name container and stream info (output items)
    //!@{
    struct mpegin_stream_data wma_streams[WMA_STREAM_MAX];  //!< contain information for each stream found in the container

    //!@}

    //! @name settings used when opening a file (input items)
    //!@{
    int32_t wma_stream_to_get;       //!< specify which audio stream to decode

                                     //!< A value of 1 means the first audio stream of the type found,
                                     //!< 2 means the second audio stream of the type found, etc.
                                     //!< The fields have the range 1 .. stream count, a value of 0 means
                                     //!< do not use a stream of this type. The fields are checked in the order
                                     //!< listed above.
                                     //!< If there are multiple audio stream types (for instance both a mpeg
                                     //!< and an ac3 audio stream are present) and the desired stream is the ac3 stream,
                                     //!< set audio_stream_to_get = 0 and ac3_stream_to_get = 1.
                                     //!< @note The aes3_stream_to_get includes HDMV type streams.
                                     //!< The audio stream must be selected before calling mpegInOpen.

    int32_t wma_stream_count;        //!< the number of WMA audio streams found in a file

                                     //!< It specifies how many items are in the wma_streams array.

    //!@}

    //! @name selected video stream info (output items)
    //!@{
    int32_t vc1_video_flag;          //!< the selected video stream is VC1 video

    int32_t dv_video_flag;           //!< the selected video stream is DV video

    // AVC SPS items
    uint8_t frame_mbs_only_flag;            //!< the value of the like named field in the AVC stream headers
    uint8_t mb_adaptive_frame_field_flag;   //!< the value of the like named field in the AVC stream headers
    uint32_t chroma_format_idc;             //!< the value of the like named field in the AVC stream headers
    uint8_t pic_struct_present_flag;        //!< the value of the like named field in the AVC stream headers
    uint8_t timing_info_present_flag;       //!< the value of the like named field in the AVC stream headers
    uint8_t video_signal_type_present_flag; //!< the value of the like named field in the AVC stream headers
    uint8_t avc_video_format;               //!< the value of the like named field in the AVC stream headers
    uint32_t num_units_in_tick;             //!< the value of the like named field in the AVC stream headers
    uint32_t time_scale;                    //!< the value of the like named field in the AVC stream headers
    uint8_t fixed_frame_rate_flag;          //!< the value of the like named field in the AVC stream headers

    // AVC SEI items
    uint8_t pic_struct;                     //!< the value of the like named field in the AVC stream headers

    // AVC slice items
    uint8_t field_pic_flag;                 //!< the value of the like named field in the AVC stream headers
    uint8_t bottom_field_flag;              //!< the value of the like named field in the AVC stream headers

    //!@}

    uint8_t reserved12_1[2];                //!< reserved for future use

    uint8_t avc_sr_last_sps_buffer[2048];   //!< avc sps found on last seek, sr mode only
    uint32_t avc_sr_last_sps_buffer_len;    //!< avc sps buffer length, sr mode only
    uint8_t avc_sr_last_pps_buffer[2048];   //!< avc pps found on last seek, sr mode only
    uint32_t avc_sr_last_pps_buffer_len;    //!< avc pps buffer length, sr mode only

    mfi_avc_ext_info_t avc_ext_info;        //!< extended information for AVC encoder configuration in SR mode

    int32_t reserved13[3199];               //!< reserved for future use

    int32_t dts_stream_to_get;              //!< specify which DTS audio stream to passthrough

                                            //!< NOTE: DTS audio is not decoded, only valid for pass-through mode! Use the new API to decode DTS audio.
                                            //!< A value of 1 means the first audio stream of the
                                            //!< type found, 2 means the second audio stream of the type found, etc.
                                            //!< The field has the range 1 .. stream count,
                                            //!< a value of 0 means do not use a stream of this type.
                                            //!< The field is checked in the order listed above.
                                            //!< If there are multiple audio stream types
                                            //!< (for instance both a mpeg and an DTS audio stream are
                                            //!< present) and the desired stream is the DTS stream,
                                            //!< set audio_stream_to_get = 0 and dts_stream_to_get = 1.
                                            //!< The audio stream must be selected
                                            //!< before calling mpegInOpen.

    //! @name HEVC items
    //!@{
    uint8_t hevc_video_flag;
    mfi_hevc_info_t hevc_info;              //!< the values of the like named fields in the HEVC stream headers
    //!@}
    uint8_t reserved14[2];

    int32_t reserved15[3193];               //!< reserved for future use

    int32_t mjpeg_video_flag;               //!< the selected video stream is Motion JPEG video

    uint32_t audio_pass_through_size;       //!< returned size when requesting pass through audio frames

    int64_t current_video_pts;              //!< returned after a seek or getnext call
    int64_t current_audio_pts;              //!< returned after a seek or getnext call

    //! @name selected video stream info (output items)
    //!@{
    int16_t avcsr_coding_order;             //!< the coding order of the current frame in SR mode
    uint8_t avcsr_idr_flag;                 //!< whether the current frame is an IDR frame in SR mode
    uint8_t avcsr_reserved;                 //!< reserved for future used

    //!@}

    //! @name container and stream info (output items)
    //!@{
    int32_t encrypted_streams_present;      //!< set to 1 if any encrypted streams are detected

    //!@}

    //! @name settings used when opening a file (input items)
    //!@{
    char *ac3_serial_number;                //!< use this field to pass a serial number to a Dolby Digital decoder

                                            //!< This field must be set before calling mpegInOpen,
                                            //!< it must point to a null terminated char string.

    //!@}

    //! @name selected subtitle stream info (output items)
    //!@{
    int32_t dec_size;                       //!< the size of the private data owned by the mfimport instance

    //!@}

    //! @name depreciated input items
    //!@{
    void  (*err_printf)(const char *fmt, ...);                  //!< depreciated, pass a get_rc function pointer to mpegInOpen instead
    void  (*prg_printf)(int32_t percent, const char *fmt, ...); //!< depreciated, pass a get_rc function pointer to mpegInOpen instead
    void  (*wrn_printf)(const char *fmt, ...);                  //!< depreciated, pass a get_rc function pointer to mpegInOpen instead
    void  (*inf_printf)(const char *fmt, ...);                  //!< depreciated, pass a get_rc function pointer to mpegInOpen instead
    void *(*memAlloc)(uint32_t size);                           //!< depreciated, pass a get_rc function pointer to mpegInOpen instead
    void  (*memFree)(void *memblk);                             //!< depreciated, pass a get_rc function pointer to mpegInOpen instead
    void *(MC_EXPORT_API *get_rc)(const char* name);            //!< depreciated, pass a get_rc function pointer to mpegInOpen instead

    //!@}

    mpegdec_tt *dec;                        //!< decoder instance

};
#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif


//! @name normal functions
//!@{

/*!
 @brief Call to create and initialize a mpegInInfo structure.
 @details Then set the input fields as desired before calling mpegInOpen to open a file.
 This function will create an mpeginInfo instance and return
 a pointer to the initialized structure. This pointer is freed when the mpegInFree function
 is called.<br>
 An application can keep a copy of this structure and use it at a later time to reopen a file
 without the overhead of finding the number/type/duration of streams in the file.
 If a file was opened in indexed mode, this method will still work but the files will be rescanned
 to recreate the indexes. Also if the file date/time or size has changed since the file was initially
 opened, the reopen will revert to a normal open. Note that this is only useful during a session,
 the contents of the structure should not be saved as a permanent thing, there is no guarantee
 it can be use with a different version of this SDK.<br>
 To reuse the mpegInInfo structure an application must allocate its own memory and save the structure
 like so:<br>
 @code
 mpegInInfo *inInfo, *saveInInfo;
int32_t mem_size;
inInfo = mpegInNew(get_rc, NULL);
mpegInOpen(inInfo, „test.mpg“);
mem_size = sizeof(mpegInInfo) + inInfo->dec_size;
saveInInfo = malloc(mem_size);
memcpy(saveInInfo, inInfo, mem_size);
// use the file...
mpegInClose(inInfo);
mpegInFree(inInfo); // this free’s inInfo!
// now the file can be reopened like so
if (mpegInNew(get_rc, saveInInfo) == NULL)
error condition
mpegInOpen(saveInInfo, NULL);
// use the file...
mpegInClose(saveInInfo);
mpegInFree(saveInInfo);
free(saveInInfo); // the application must free the memory!
mpegInInfo *mpegInNew(
void *(*get_rc)(const char* name),
mpegInInfo *existingInfo
)
 @endcode

 @param [in] get_rc        a pointer to a get_rc resource function
 @param [in] existingInfo  a pointer to a previously saved mpegInInfo structure

 @return                   a pointer to an mpegInInfo structure if successful, else NULL
*/
struct mpegInInfo * MC_EXPORT_API mpegInNew(void *(MC_EXPORT_API *get_rc)(const char* name),
                                            struct mpegInInfo *existingInfo);

/*!
 @brief Call to open an mpeg file.
 @details Filename can be NULL if inInfo is a pointer to a saved mpegInInfo structure
 from a previously opened mpeg file (see the API documentation for more details).
 mpegInOpenW is the Unicode version of this function.

 @param [in] inInfo    a pointer to an mpegInInfo structure
 @param [in] filename  a pointer to a filename to open, can be NULL if re-opening a file

 @return               mpegInErrNone if successful, else mpegInError
*/
int32_t MC_EXPORT_API mpegInOpen(struct mpegInInfo *inInfo,
                                 char *filename);


/*!
 @brief Call to open an Unicode mpeg file.
 @details Filename can be NULL if inInfo is a pointer to a saved mpegInInfo structure
 from a previously opened mpeg file (see the API documentation for more details)

 @param [in] inInfo    a pointer to an mpegInInfo structure
 @param [in] filename  a pointer to a unicode filename to open

 @return               mpegInErrNone if successful, else mpegInError
*/
#if !defined(__APPLE__) && !defined(__linux__) && !defined(__vxworks)

int32_t MC_EXPORT_API mpegInOpenW(struct mpegInInfo *inInfo,
                                  wchar_t *filename);

#else

// this version will currently return an error!
int32_t MC_EXPORT_API mpegInOpenW(struct mpegInInfo *inInfo,
                                  uint16_t *filename);

#endif

/*!
 @brief Call to close an mpeg file opened with mpegInOpen and de-initialize the decoder.

 @param [in] inInfo  a pointer to an mpegInInfo structure

 @return             none
*/
void MC_EXPORT_API mpegInClose(struct mpegInInfo *inInfo);


/*!
 @brief Call to free the memory allocated during an mpegInNew call.
 @details If inInfo points to a structure that was allocated by the application
 for re-opening a file, the aplication must free the inInfo memory.
 This function does not free the mpegInInfo structure if it was allocated by the application
 It will only free the memory if it was allocated by mpegInNew.
 See the API documentation for more details.

 @param [in] inInfo  a pointer to an mpegInInfo structure

 @return             none
*/
void MC_EXPORT_API mpegInFree(struct mpegInInfo *inInfo);

/*!
 @brief Call to close all open file handles.
 @details This must be followed by a call to mpegInWakeFiles before
 any of the other functions can be used again.
 This call does not release any memory.

 @param [in] inInfo  a pointer to an mpegInInfo structure

 @return             none
*/
void MC_EXPORT_API mpegInSleepFiles(struct mpegInInfo *inInfo);

/*!
 @brief Call to re-open all file handles.

 @param [in] inInfo  a pointer to an mpegInInfo structure

 @return             mpegInErrNone if successful, else mpegInError
*/
int32_t MC_EXPORT_API mpegInWakeFiles(struct mpegInInfo *inInfo);

/*!
 @brief Call to seek to and decode a particular video frame.

 @param [in] inInfo    a pointer to an mpegInInfo structure
 @param [in] frameNum  a zero based frame number, the range is 0 .. totalFrames

 @return               mpegInErrNone if successful, else mpegInError
*/
int32_t MC_EXPORT_API mpegInSeekFrame(struct mpegInInfo *inInfo,
                                      int32_t frameNum);

/*!
 @brief Call to decode the next video frame.

 @param [in] inInfo  a pointer to an mpegInInfo structure

 @return             mpegInErrNone if successful, else mpegInError
*/
int32_t MC_EXPORT_API mpegInGetNextFrame(struct mpegInInfo *inInfo);

/*!
 @brief Call to seek to and decode a particular number of audio samples
 into the passed audio buffer.
 @details To just seek and not decode any  samples,
 call this function with buffer == NULL or numSamples == 0 or both,
 this function returns the number of samples returned,
 zero if only seeking was performed or -1 if an error occurs

 @param [in] inInfo       a pointer to an initialized mpegInInfo structure
 @param [in,out] buffer   a pointer to a buffer to receive the decoded PCM audio data
 @param [in] startSample  a zero based starting sample number
 @param [in] numSamples   the number of samples to decode

 @return                  the number of samples returned, 0 if only seeking was performed or
                          -1 if an error occurs
*/
int32_t MC_EXPORT_API mpegInSeekSample64(struct mpegInInfo *inInfo,
                                         uint8_t *buffer,
                                         int64_t startSample,
                                         int32_t numSamples);

/*!
 @brief Call to decode the next numSamples audio samples into the passed buffer.

 @param [in] inInfo      a pointer to an mpegInInfo structure
 @param [in,out] buffer  a pointer to a buffer to receive the decoded PCM audio data
 @param [in] numSamples  the number of samples to decode

 @return                 the number of samples returned or -1 if an error occurs
*/
int32_t MC_EXPORT_API mpegInGetNextSample(struct mpegInInfo *inInfo,
                                          uint8_t *buffer,
                                          int32_t numSamples);

//!@}

/*!
 @brief Subpicture info.
 */
struct mpegin_subpic_info_struct
{
    uint8_t *buffer;        //!< buffer for subpic chunk

                            //!< If NULL then the buffer_size field is filled with the size of the
                            //!< next chunk and the PTS and duration fields are filled.

    int32_t buffer_size;    //!< if buffer is non-NULL this is the size of buffer in bytes

                            //!< If buffer is NULL, the size of the next chunk is returned here.

    int64_t PTS;            //!< timestamp of chunk

    int64_t duration;       //!< duration of the chunk

    uint8_t reserved[64];   //!< reserved
};

//! @name normal functions
//!@{

/*!
 @brief Call to get the next raw subpic chunk or the size/timestamp of the next chunk.

 @param [in] inInfo           a pointer to an mpegInInfo structure
 @param [in,out] subpic_info  a pointer to a mpegin_subpic_info_struct structure

 @return                      mpegInErrNone if successful, else mpegInError
*/
int32_t MC_EXPORT_API mpegInGetNextSubpicChunk(struct mpegInInfo *inInfo,
                                               struct mpegin_subpic_info_struct *subpic_info);

/*!
 @brief Call this function to get information about a file.
 @details The audioAvail, videoAvail and subpicAvail flags in
 the mpegInInfo structure are set and stream information is provided.
 Use this information to determine which xxx_stream_to_get fields need to be set
 before calling mpegInOpen.
 @note The number of frames and samples is not determined and
 the file is closed before the function returns.

 @param [in,out] inInfo  a pointer to an mpegInInfo structure

 @return                 mpegInErrNone if successful, else mpegInError
*/
int32_t MC_EXPORT_API mpegInGetInfo(struct mpegInInfo *inInfo);

/*!
 @brief Open an mpeg file using existing index files.
 @details This function is like the mpegInOpen function except the file is opened
 in indexed mode (FastAccess=0) and the passed in index file names are used
 as the index data instead of parsing the file.
 The index files can be created with the mpegInSaveIdx function or by the muxer.
 If the load_idx_to_mem parameter is non-zero the index data is read from the file
 into memory and the index files are no longer accessed, if load_idx_to_mem is 0,
 the index data is used directly from the disk files.
 mpegInOpenMPEGIdxW is the Unicode version of this function.

 @note Please use the function mpegInOpenMPEGIdx() only for MPEG2 TS/PS or Elementary Stream.
 They should not be used for MXF or MP4 containers.

 @param [in] inInfo           a pointer to an mpegInInfo structure
 @param [in] audio_idx_file   a pointer to a filename for the audio index data
 @param [in] video_idx_file   a pointer to a filename for the video index data
 @param [in] load_idx_to_mem  a flag indicating whether the index is loaded to memory

 @return                      mpegInErrNone if successful, else mpegInError
*/
int32_t MC_EXPORT_API mpegInOpenMPEGIdx(struct mpegInInfo *inInfo,
                                        char *audio_idx_file,
                                        char *video_idx_file,
                                        int32_t load_idx_to_mem);

/*!
 @brief Open an mpeg file using existing index files, Unicode version.
 @details This function is like the mpegInOpen function except the file is opened
 in indexed mode (FastAccess=0) and the passed in index file names are used
 as the index data instead of parsing the file.
 The index files can be created with the mpegInSaveIdx function or by the muxer.
 If the load_idx_to_mem parameter is non-zero the index data is read from the file
 into memory and the index files are no longer accessed, if load_idx_to_mem is 0,
 the index data is used directly from the disk files.
 mpegInOpenMPEGIdxW is the Unicode version of this function.

 @note Please use the function mpegInOpenMPEGIdx() only for MPEG2 TS/PS or Elementary Stream.
 They should not be used for MXF or MP4 containers.

 @param [in] inInfo           a pointer to an mpegInInfo structure
 @param [in] audio_idx_file   a pointer to a Unicode filename for the audio index data
 @param [in] video_idx_file   a pointer to a Unicode filename for the video index data
 @param [in] load_idx_to_mem  a flag indicating whether the index is loaded to memory

 @return                      mpegInErrNone if successful, else mpegInError
*/
#if !defined(__APPLE__) && !defined(__linux__) && !defined(__vxworks)

int32_t MC_EXPORT_API mpegInOpenMPEGIdxW(struct mpegInInfo *inInfo,
                                         wchar_t *audio_idx_file,
                                         wchar_t *video_idx_file,
                                         int32_t load_idx_to_mem);
#else

// this version will currently return an error!
int32_t MC_EXPORT_API mpegInOpenMPEGIdxW(struct mpegInInfo *inInfo,
                                         uint16_t *audio_idx_file,
                                         uint16_t *video_idx_file,
                                         int32_t load_idx_to_mem);
#endif

/*!
 @brief Call to save the index data to disk files.
 @details The audio and video index data is saved to separate files
 and can be saved independently from each other
 (i.e. just the audio of just the video can be saved).
 If the current file was not opened in indexed mode (FastAccess = 1),
 this function returns mpegInError.
 mpegInSaveIdxW is the Unicode version of this function.

 @note Please use the function mpegInSaveIdx() only for MPEG2 TS/PS or Elementary Stream.
 They should not be used for MXF or MP4 containers.

 @param [in] inInfo          a pointer to an mpegInInfo structure
 @param [in] audio_idx_file  a pointer to a filename for the audio index data
 @param [in] video_idx_file  a pointer to a filename for the video index data

 @return                     mpegInErrNone if successful, else mpegInError
*/
int32_t MC_EXPORT_API mpegInSaveIdx(struct mpegInInfo *inInfo,
                                    char *audio_idx_file,
                                    char *video_idx_file);

/*!
 @brief Call to save the index data to disk files, Unicode version.
 @details The audio and video index data is saved to separate files
 and can be saved independently from each other
 (i.e. just the audio of just the video can be saved).
 If the current file was not opened in indexed mode (FastAccess = 1),
 this function returns mpegInError.
 mpegInSaveIdxW is the Unicode version of this function.

 @note Please use the function mpegInSaveIdx() only for MPEG2 TS/PS or Elementary Stream.
 They should not be used for MXF or MP4 containers.

 @param [in] inInfo          a pointer to an mpegInInfo structure
 @param [in] audio_idx_file  a pointer to a Unicode filename for the audio index data
 @param [in] video_idx_file  a pointer to a Unicode filename for the video index data

 @return                     mpegInErrNone if successful, else mpegInError
*/
#if !defined(__APPLE__) && !defined(__linux__) && !defined(__vxworks) && !defined(__QNX__)

int32_t MC_EXPORT_API mpegInSaveIdxW(struct mpegInInfo *inInfo,
                                     wchar_t *audio_idx_file,
                                     wchar_t *video_idx_file);

#else

// this version will currently return an error!
int32_t MC_EXPORT_API mpegInSaveIdxW(struct mpegInInfo *inInfo,
                                     uint16_t *audio_idx_file,
                                      uint16_t *video_idx_file);

#endif

// direct access to the decoder's auxinfo functions
/*!
 @brief This function provides direct access to the video decoder's auxinfo function.

 @param [in] inInfo    a pointer to an initialized mpegInInfo structure
 @param [in] offs      determined by info_ID
 @param [in] info_ID   auxinfo message value
 @param [in] info_ptr  determined by info_ID
 @param [in] info_size determined by info_ID

 @return               determined by info_ID
*/
int32_t MC_EXPORT_API mpegInVideoAuxinfo(struct mpegInInfo *inInfo,
                                         uint32_t offs,
                                         uint32_t info_ID,
                                         void *info_ptr,
                                         uint32_t info_size);

/*!
 @brief This function provides direct access to the audio decoder's auxinfo function.

 @param [in] inInfo    a pointer to an initialized mpegInInfo structure
 @param [in] offs      determined by info_ID
 @param [in] info_ID   auxinfo message value
 @param [in] info_ptr  determined by info_ID
 @param [in] info_size determined by info_ID

 @return               determined by info_ID
*/
int32_t MC_EXPORT_API mpegInAudioAuxinfo(struct mpegInInfo *inInfo,
                                         uint32_t offs,
                                         uint32_t info_ID,
                                         void *info_ptr,
                                         uint32_t info_size);

/*!
 @brief This function provides direct access to the subpicture decoder's auxinfo function.

 @param [in] inInfo    a pointer to an initialized mpegInInfo structure
 @param [in] offs      determined by info_ID
 @param [in] info_ID   auxinfo message value
 @param [in] info_ptr  determined by info_ID
 @param [in] info_size determined by info_ID

 @return               determined by info_ID
*/
int32_t MC_EXPORT_API mpegInSubpicAuxinfo(struct mpegInInfo *inInfo,
                                          uint32_t offs,
                                          uint32_t info_ID,
                                          void *info_ptr,
                                          uint32_t info_size);


/*!
 @brief This function provides direct access to the audio decoder.

 @param [in] inInfo    a pointer to an initialized mpegInInfo structure
 @return               NULL or a pointer to the audio decoder instance
*/
bufstream_tt * MC_EXPORT_API mpegInGetAudioDecoderInstance(struct mpegInInfo *inInfo);


/*!
 @brief This function provides direct access to the video decoder.

 @param [in] inInfo    a pointer to an initialized mpegInInfo structure
 @return               NULL or a pointer to the video decoder instance
*/
bufstream_tt * MC_EXPORT_API mpegInGetVideoDecoderInstance(struct mpegInInfo *inInfo);


/*!
 @brief Call to get extended API function.

 @param [in] func  function ID

 @return     function pointer or NULL
*/
APIEXTFUNC MC_EXPORT_API mpegInGetAPIExt(uint32_t func);

//!@}

//! @name internally used functions
//!@{
int32_t MC_EXPORT_API mpegInOpenTSFile(struct mpegInInfo *inInfo, char *audio_idx_file, char *video_idx_file, int32_t load_to_mem);
int32_t MC_EXPORT_API mpegInOpenTSStream(struct mpegInInfo *inInfo, bufstream_tt *audio_idx, bufstream_tt *video_idx);
int32_t MC_EXPORT_API mpegInInitTS(struct mpegInInfo *inInfo);
int32_t MC_EXPORT_API mpegInSeekFrameTS(struct mpegInInfo *inInfo, int32_t frameNum);
int32_t MC_EXPORT_API mpegInGetNextFrameTS(struct mpegInInfo *inInfo);
int32_t MC_EXPORT_API mpegInSeekSample64TS(struct mpegInInfo *inInfo, uint8_t *buffer, int64_t startSample, int32_t numSamples, int32_t *numReturned);
int32_t MC_EXPORT_API mpegInGetNextSampleTS(struct mpegInInfo *inInfo, uint8_t *buffer, int32_t numSamples, int32_t *numReturned);
int32_t MC_EXPORT_API mpegSplitOpenFileTSFile(struct mpegInInfo *inInfo, void* lpData, char *audio_idx_name, char *video_idx_name, int32_t load_to_mem);
int32_t MC_EXPORT_API mpegSplitOpenFileTSStream(struct mpegInInfo *inInfo, void* lpData, bufstream_tt *audio_idx, bufstream_tt *video_idx);
int32_t MC_EXPORT_API mpegSplitInitTS(struct mpegInInfo *inInfo);
int32_t MC_EXPORT_API mpegSplitSeekTimeTS(struct mpegInInfo *inInfo, int64_t timecode, int64_t *seek_timecode, int64_t *seek_filepos, int64_t *ref_timecode, int64_t *ref_filepos, void* lpData, int32_t audioSearch);
int32_t MC_EXPORT_API mpegSplitSeekByteTS(struct mpegInInfo *inInfo, int64_t position, int64_t *actual_timecode, void* lpData, int32_t audioSearch);
int32_t MC_EXPORT_API mpegSplitOpenFile(struct mpegInInfo *inInfo, void* lpData);
int32_t MC_EXPORT_API mpegSplitGetInfo(struct mpegInInfo *inInfo, void* lpData);
void MC_EXPORT_API mpegSplitClose(struct mpegInInfo *inInfo);
int32_t MC_EXPORT_API mpegSplitSeekTime(struct mpegInInfo *inInfo, int64_t timecode, int64_t *seek_timecode, int64_t *seek_filepos, int64_t *ref_timecode, int64_t *ref_filepos, void* lpData, int32_t audioSearch);
int32_t MC_EXPORT_API mpegSplitSeekByte(struct mpegInInfo *inInfo, int64_t position, int64_t *actual_timecode, void* lpData, int32_t audioSearch);
int32_t MC_EXPORT_API mpegInOpenPrimary(void *(MC_EXPORT_API *get_rc)(const char* name),struct  mpegInInfo *inInfo);
int32_t MC_EXPORT_API mpegInOpenSecondary(struct mpegInInfo *inInfo,struct mpegInInfo *orgInfo);
void MC_EXPORT_API mpegInCloseSecondary(struct mpegInInfo *inInfo);
int32_t MC_EXPORT_API mpegInGetNextCodedFrame(struct mpegInInfo *inInfo);

#if !defined(__APPLE__) && !defined(__linux__) && !defined(__vxworks) && !defined(__QNX__)

int32_t MC_EXPORT_API mpegInOpenTSFileW(struct mpegInInfo *inInfo, wchar_t *audio_idx_file, wchar_t *video_idx_file, int32_t load_to_mem);
int32_t MC_EXPORT_API mpegSplitOpenFileTSFileW(struct mpegInInfo *inInfo, void* lpData, wchar_t *audio_idx_name, wchar_t *video_idx_name, int32_t load_to_mem);

#else

// this version will currently return an error!
int32_t MC_EXPORT_API mpegInOpenTSFileW(struct mpegInInfo *inInfo, uint16_t *audio_idx_file, uint16_t *video_idx_file, int32_t load_to_mem);
int32_t MC_EXPORT_API mpegSplitOpenFileTSFileW(struct mpegInInfo *inInfo, void* lpData, uint16_t *audio_idx_name, uint16_t *video_idx_name, int32_t load_to_mem);

#endif

//!@}


//! @name functions, being removed: Do not use!
//!@{
void MC_EXPORT_API mpegInInfoInit(struct mpegInInfo *inInfo);
int32_t MC_EXPORT_API mpegInOpenMPEG(struct mpegInInfo *inInfo);
void MC_EXPORT_API mpegInCloseMPEG(struct mpegInInfo *inInfo);
int32_t MC_EXPORT_API mpegInSeekSample(struct mpegInInfo *inInfo, uint8_t *buffer, uint32_t startSample, int32_t numSamples);
char * MC_EXPORT_API mpegInGetLastErrString(void);
struct mpegInInfo * MC_EXPORT_API mpegInOpenExisting(void *(MC_EXPORT_API *get_rc)(const char* name), int32_t filePtr);
void MC_EXPORT_API mpegInUpdateFiles(struct mpegInInfo *inInfo, int32_t filePtr);
void MC_EXPORT_API mpegInCloseExisting(struct mpegInInfo *inInfo);
int32_t MC_EXPORT_API mpegInSeekFrameExisting(struct mpegInInfo *inInfo, int32_t frameNum, int32_t decodeMode);
int32_t MC_EXPORT_API mpegInGetNextFrameExisting(struct mpegInInfo *inInfo, int32_t decodeMode);
int32_t MC_EXPORT_API mpegInSeekSampleExisting(struct mpegInInfo *inInfo, uint8_t *buffer, int64_t startSample, int32_t numSamples);
int32_t MC_EXPORT_API mpegInGetNextSampleExisting(struct mpegInInfo *inInfo, uint8_t *buffer, int32_t numSamples);
int32_t MC_EXPORT_API mpegInSeekSampleTS(struct mpegInInfo *inInfo, uint8_t *buffer, uint32_t startSample, int32_t numSamples, int32_t *numReturned);

//!@}


#ifdef __cplusplus
}
#endif

//! @name depreciated, output types
//!@{
#define VIDEO_YUV      1   //!< depreciated, use fourCC field instead
#define VIDEO_RGB      3   //!< depreciated, use fourCC field instead
#define AUDIO_PCM      4   //!< depreciated, use the audio stream_to_get fields instead
#define VIDEO_BGR3     0xB //!< depreciated, use fourCC field instead
//!@}

//! @name depreciated, use the video_streams field instead
//!@{
#define VIDEO_STREAM_0  0x00000001
#define VIDEO_STREAM_1  0x00000002
#define VIDEO_STREAM_2  0x00000004
#define VIDEO_STREAM_3  0x00000008
#define VIDEO_STREAM_4  0x00000010
#define VIDEO_STREAM_5  0x00000020
#define VIDEO_STREAM_6  0x00000040
#define VIDEO_STREAM_7  0x00000080
#define VIDEO_STREAM_8  0x00000100
#define VIDEO_STREAM_9  0x00000200
#define VIDEO_STREAM_10 0x00000400
#define VIDEO_STREAM_11 0x00000800
#define VIDEO_STREAM_12 0x00001000
#define VIDEO_STREAM_13 0x00002000
#define VIDEO_STREAM_14 0x00004000
#define VIDEO_STREAM_15 0x00008000
//!@}

//! @name depreciated, use the audio_streams field instead
//!@{
#define AUDIO_STREAM_0  0x00000001
#define AUDIO_STREAM_1  0x00000002
#define AUDIO_STREAM_2  0x00000004
#define AUDIO_STREAM_3  0x00000008
#define AUDIO_STREAM_4  0x00000010
#define AUDIO_STREAM_5  0x00000020
#define AUDIO_STREAM_6  0x00000040
#define AUDIO_STREAM_7  0x00000080
#define AUDIO_STREAM_8  0x00000100
#define AUDIO_STREAM_9  0x00000200
#define AUDIO_STREAM_10 0x00000400
#define AUDIO_STREAM_11 0x00000800
#define AUDIO_STREAM_12 0x00001000
#define AUDIO_STREAM_13 0x00002000
#define AUDIO_STREAM_14 0x00004000
#define AUDIO_STREAM_15 0x00008000
#define AUDIO_STREAM_16 0x00010000
#define AUDIO_STREAM_17 0x00020000
#define AUDIO_STREAM_18 0x00040000
#define AUDIO_STREAM_19 0x00080000
#define AUDIO_STREAM_20 0x00100000
#define AUDIO_STREAM_21 0x00200000
#define AUDIO_STREAM_22 0x00400000
#define AUDIO_STREAM_23 0x00800000
#define AUDIO_STREAM_24 0x01000000
#define AUDIO_STREAM_25 0x02000000
#define AUDIO_STREAM_26 0x04000000
#define AUDIO_STREAM_27 0x08000000
#define AUDIO_STREAM_28 0x10000000
#define AUDIO_STREAM_29 0x20000000
#define AUDIO_STREAM_30 0x40000000
#define AUDIO_STREAM_31 0x80000000
//!@}

//! @name depreciated, use the ac3_streams field instead
//!@{
#define AC3_STREAM_0  0x00000001
#define AC3_STREAM_1  0x00000002
#define AC3_STREAM_2  0x00000004
#define AC3_STREAM_3  0x00000008
#define AC3_STREAM_4  0x00000010
#define AC3_STREAM_5  0x00000020
#define AC3_STREAM_6  0x00000040
#define AC3_STREAM_7  0x00000080
//!@}

//! @name depreciated, use the dts_streams field instead
//!@{
#define DTS_STREAM_0  0x00000001
#define DTS_STREAM_1  0x00000002
#define DTS_STREAM_2  0x00000004
#define DTS_STREAM_3  0x00000008
#define DTS_STREAM_4  0x00000010
#define DTS_STREAM_5  0x00000020
#define DTS_STREAM_6  0x00000040
#define DTS_STREAM_7  0x00000080
//!@}

//! @name depreciated, use the sdds_streams field instead
//!@{
#define SDDS_STREAM_0  0x00000001
#define SDDS_STREAM_1  0x00000002
#define SDDS_STREAM_2  0x00000004
#define SDDS_STREAM_3  0x00000008
#define SDDS_STREAM_4  0x00000010
#define SDDS_STREAM_5  0x00000020
#define SDDS_STREAM_6  0x00000040
#define SDDS_STREAM_7  0x00000080
//!@}

//! @name depreciated, use the pcm_streams field instead
//!@{
#define PCM_STREAM_0  0x00000001
#define PCM_STREAM_1  0x00000002
#define PCM_STREAM_2  0x00000004
#define PCM_STREAM_3  0x00000008
#define PCM_STREAM_4  0x00000010
#define PCM_STREAM_5  0x00000020
#define PCM_STREAM_6  0x00000040
#define PCM_STREAM_7  0x00000080
//!@}

//! @name depreciated, use the subpic_streams field instead
//!@{
#define SUBPIC_STREAM_0  0x00000001
#define SUBPIC_STREAM_1  0x00000002
#define SUBPIC_STREAM_2  0x00000004
#define SUBPIC_STREAM_3  0x00000008
#define SUBPIC_STREAM_4  0x00000010
#define SUBPIC_STREAM_5  0x00000020
#define SUBPIC_STREAM_6  0x00000040
#define SUBPIC_STREAM_7  0x00000080
#define SUBPIC_STREAM_8  0x00000100
#define SUBPIC_STREAM_9  0x00000200
#define SUBPIC_STREAM_10 0x00000400
#define SUBPIC_STREAM_11 0x00000800
#define SUBPIC_STREAM_12 0x00001000
#define SUBPIC_STREAM_13 0x00002000
#define SUBPIC_STREAM_14 0x00004000
#define SUBPIC_STREAM_15 0x00008000
#define SUBPIC_STREAM_16 0x00010000
#define SUBPIC_STREAM_17 0x00020000
#define SUBPIC_STREAM_18 0x00040000
#define SUBPIC_STREAM_19 0x00080000
#define SUBPIC_STREAM_20 0x00100000
#define SUBPIC_STREAM_21 0x00200000
#define SUBPIC_STREAM_22 0x00400000
#define SUBPIC_STREAM_23 0x00800000
#define SUBPIC_STREAM_24 0x01000000
#define SUBPIC_STREAM_25 0x02000000
#define SUBPIC_STREAM_26 0x04000000
#define SUBPIC_STREAM_27 0x08000000
#define SUBPIC_STREAM_28 0x10000000
#define SUBPIC_STREAM_29 0x20000000
#define SUBPIC_STREAM_30 0x40000000
#define SUBPIC_STREAM_31 0x80000000
//!@}

#endif // #if !defined (__MFIMPORT_API_INCLUDED__)

