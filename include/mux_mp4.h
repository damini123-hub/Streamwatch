/* ----------------------------------------------------------------------------
 * File: mux_mp4.h
 *
 * Desc: MP4 Muxer API declaration
 *
 * Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.
 *
 * MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 * This software is protected by copyright law and international treaties.  Unauthorized
 * reproduction or distribution of any portion is prohibited by law.
 * ----------------------------------------------------------------------------
 */

//  \brief Interface for MP4 multiplexer MC API version


#if !defined (__MUX_MP4_API_INCLUDED__)
#define __MUX_MP4_API_INCLUDED__

#ifndef _EXTERN_
#include "bufstrm.h"
#include "mcprofile.h"
#include "mcapiext.h"
#include "mcmediatypes.h"
#endif

#if !defined (DECLARE_RESERVED_FIELD)
/*!
    Declares reserved field in structure.

    Use this macro for padding structure on x86 and x64 platform. Macro calculates count of additional
    bytes for x86 and x64 platform to save structure size is constant. Macro is based on suggestion
    max size of pointer not greater than sizeof(int64_t).
    Args:
     - _absolute_bytes_count_ - How many bytes will be inserted in general cases(32 and 64 bit platform). Please use count for 64 bit platform, keep in mind that pointer has 8 bytes size. Macro enlarges small size, not decrease big size.
     - _pointers_count_ - how many pointers is stored in structure, their size under 32 bit platform would be padded to size under 64 bit platform.

*/
#define DECLARE_RESERVED_FIELD(_absolute_bytes_count_, _pointers_count_) uint8_t __reserved__[(_absolute_bytes_count_) + (_pointers_count_) * (sizeof(int64_t) - sizeof(uintptr_t))];
#endif


#include "drm_encryption.h"

typedef struct mp4_muxer mp4_muxer_tt; //!< muxer object
/*!
    \file mux_mp4.h
*/
/*!
	 \section MUXMP4LL_COMP_TIME_CONS compile time constrains
     @name MP4 Muxer compile time constrains
*/
///@{

/*! Maximum supported size of the title in Sony XDCAM files */
#define MAX_FILE_TITLE_SIZE 1024
/*! Maximum number of streams demuxer currently supports */
#define MAX_STREAMS         32
///@}


/*! \section MP4MUXLL_SUP_FF Supported File Formats

     @name Supported File formats
 */
///@{
/*! \brief mp4 file ISO 14496-12/14 */
#define MP4_FORMAT              0
/*! \brief 3GP file
preset settings:
- KeepAUDelimiter:  off
- KeepPS:           off
- RemoveFiller:     off
- RemoveSEI:        on
- atom_ordering:    MDAT before MOOV

atoms settings:

FTYPE:
- BrandName:        BrandNameType3GPP5
- MinorVersion:     0
- Compatibilities:  BrandNameTypeISOM, BrandNameType3GPP4, BrandNameTypeMP41, BrandNameTypeMP42

Other settings are compatible with 3gpp and 3g2p file format
*/
#define FILE_FORMAT_3GP         1
/*! \brief mp4 file with Motion JPEG2000
preset settings:
- KeepAUDelimiter:  off
- KeepPS:           off
- RemoveFiller:     off
- RemoveSEI:        off
- atom_ordering:    MDAT before MOOV

atoms settings:
- jP(mj2 family signature atom type):

FTYPE:
- BrandName:        BrandNameTypeMJ2
- MinorVersion:     0
- Compatibilities:  BrandNameTypeMJ2, BrandNameTypeMJ2S

MOOV:
- has IODS (object descriptor atom type)

TKHD:
- flags = 0x000003

Other settings are compatible with ISO/IEC15444-12 file format
*/
#define FILE_FORMAT_MJ2         2
/*! \brief UVU file according to Ultraviolet CFF specification */
#define FILE_FORMAT_UVU         3
///@}


/*!
 \section MUXMP4LL_SCO Stream compatibility options
@name Stream compatibility options
@{*/
#define    COMP_STANDARD       0 /*!< \brief simple mp4 file */
#define    COMP_SONY_PSP       1 /*!< \brief  MP4 file for Sony PSP
                                preset settings:
                                - KeepAUDelimiter:  off
                                - KeepPS:           off
                                - RemoveFiller:     on
                                - RemoveSEI:        off
                                - atom_ordering:    MDAT before MOOV

                                atoms settings:

                                FTYPE:
                                - BrandName:           BrandNameTypeSonyPSP
                                - MinorVersion:        19474624
                                - Compatibilities:     BrandNameTypeSonyPSP, BrandNameTypeMP42, BrandNameTypeISOM
                                - uuid: configured accordingly to SONY PSP specifications

                                MOOV:
                                - has IODS (object descriptor atom type)

                                TRAK:
                                - has EDTS atom

                                STSD:
                                - ESDS(elementary stream description):

                                ESID:                 0x0
                                - stream priority:    0x0

                                MPEG 4 video part 10:
                                - Filler data:                    removed

                                Other settings are compatible with SONY PSP file format
                              */
#define    COMP_IPOD           2 /*!< \brief MP4 file for Apple iPod
                                preset settings:
                                - KeepAUDelimiter:  off
                                - KeepPS:           off
                                - RemoveFiller:     on
                                - RemoveSEI:        on
                                - atom_ordering:    MDAT before MOOV

                                atoms settings:

                                FTYPE:
                                - BrandName:        BrandNameTypeiPod
                                - MinorVersion:     1
                                - Compatibilities:  BrandNameTypeiPod, BrandNameTypeMP42, BrandNameTypeISOM

                                MDAT:
                                - nSize:            1

                                TRAK:
                                - has EDTS atom

                                STSD:
                                - has uuid atom, with the following uuid: 0x6B6840F25F244FC5BA39A51BCF0323F3

                                bitstream information:

                                MPEG 4 video part 2:
                                - Visual Object Sequence Start:         removed
                                - Video Object Layer:                   removed
                                - Group of Video Object Plane:          removed
                                - User data:                            removed
                                - Visual Object:                        removed
                                - Visual Object Sequence:               removed

                                MPEG 4 video part 10:
                                - Access unit delimiter:                removed
                                - Filler data:                          removed
                                - Supplemental Enhancement Information: removed
                                - Sequence parameter set:               removed
                                - Picture parameter set:                removed
                                */

#define    COMP_ISMA           3 /*!< \brief ISMA compatible, reserved
                                preset settings:
                                - KeepAUDelimiter:      off
                                - KeepPS:               off
                                - RemoveFiller:         off
                                - RemoveSEI:            off
                                - atom_ordering:        MDAT before MOOV

                                atoms settings:

                                FTYPE:
                                - BrandName:            BrandNameTypeMP42
                                - MinorVersion:         0
                                - Compatibilities:      BrandNameTypeISOM, BrandNameTypeMP42

                                Other settings are compatible with ISMA file format
                              */
#define    COMP_QT             4 /*!< \brief QuickTime Player support
                                preset settings:
                                - KeepAUDelimiter:      off
                                - KeepPS:               off
                                - RemoveFiller:         on
                                - RemoveSEI:            on
                                - atom_ordering:        MDAT before MOOV

                                atoms settings:

                                FTYPE:
                                - BrandName:           BrandNameTypeMP42 and BrandNameTypeQT for time code support
                                - MinorVersion:        0
                                - Compatibilities:     BrandNameTypeMP42, BrandNameTypeMP41

                                MOOV:
                                - has IODS (object descriptor atom type)

                                TRAK:
                                - has EDTS atom

                                TKHD:
                                - flags = 0x00000F for tracks with time code support and flags = 0x000001 for another tracks

                                MINF(media information atom):
                                - has HDLR (handler reference atom type)

                                STBL:
                                - has SDTP (independent and disposable atom type)

                                STSD:
                                - uses version 1 for audio for time codes support
                                bitstream information:

                                MPEG 4 video part 2:
                                - Visual Object Sequence Start:         removed
                                - Video Object Layer:                   removed
                                - Group of Video Object Plane:          removed
                                - User data:                            removed
                                - Visual Object:                        removed
                                - Visual Object Sequence:               removed

                                MPEG 4 video part 10:
                                - Access unit delimiter:                removed
                                - Filler data:                          removed
                                - Supplemental Enhancement Information: removed
                                - Sequence parameter set:               removed
                                - Picture parameter set:                removed
                                - Other settings are compatible with qt file format
                              */
#define COMP_SONY_PMC       5 /*!< \brief Sony Professional Media Card
                              preset settings:
                              - KeepAUDelimiter:    off
                              - KeepPS:             off
                              - RemoveFiller:       off
                              - RemoveSEI:          off
                              - atom_ordering:      MDAT before MOOV

                              atoms settings:

                              FTYPE:
                              - BrandName:          BrandNameTypeMP42
                              - MinorVersion:       0
                              - Compatibilities:    BrandNameTypeMP42, BrandNameTypeISOM, BrandNameTypeBlank

                              FREE:
                              - present free space atom type

                              MOOV:
                              - has IODS (object descriptor atom type)

                              TRAK:
                              - has EDTS atom

                              STBL:
                              - has SDTP (independent and disposable atom type)

                              Other settings are compatible with SONY PMC file format
                              */
#define COMP_FLASH          6 /*!< \brief Adobe Flash
                              preset settings:
                              - KeepAUDelimiter:    off
                              - KeepPS:             off
                              - RemoveFiller:       off
                              - RemoveSEI:          off
                              - atom_ordering:      MDAT before MOOV

                              atoms settings:

                              FTYPE:
                              - BrandName:          BrandNameTypeMP42
                              - MinorVersion:       0
                              - Compatibilities:    BrandNameTypeMP42, BrandNameTypeISOM

                              MOOV:
                              - has IODS (object descriptor atom type)

                              TRAK:
                              - has EDTS atom

                              STBL:
                              - has SDTP (independent and disposable atom type)

                              bitstream information:

                              MPEG 4 video part 10:
                              - Access unit delimiter:                  removed
                              - Filler data:                            removed
                              - Supplemental Enhancement Information:   removed
                              - Sequence parameter set:                 removed
                              - Picture parameter set:                  removed
                              Other settings are compatible with Adobe f4v file format
                              */
#define COMP_IPHONE         7 /*!< \brief MP4 file for Apple iPhone. This preset settings are equal to the COMP_IPOD */
#define COMP_IPAD           8 /*!< \brief MP4 file for Apple iPad
                              preset settings:
                              - KeepAUDelimiter:    off
                              - KeepPS:             off
                              - RemoveFiller:       on
                              - RemoveSEI:          on
                              - atom_ordering:      MDAT before MOOV

                              atoms settings:

                              FTYPE:
                              - BrandName:          BrandNameTypeMP42
                              - MinorVersion:       0
                              - Compatibilities:    BrandNameTypeMP42, BrandNameTypeISOM

                              TRAK:
                              - has EDTS atom

                              bitstream information:

                              MPEG 4 video part 10:
                              - Access unit delimiter:                  removed
                              - Filler data:                            removed
                              - Supplemental Enhancement Information:   removed
                              - Sequence parameter set:                 removed
                              - Picture parameter set:                  removed
                              */
#define COMP_UVU_ENC        9 /*!< \brief Encrypted Ultraviolet */
#define COMP_UVU_NENC       11 /*!< \brief Unencrypted Ultraviolet */
#define COMP_DASH           12 /*!< \brief DASH Main */
#define COMP_DASH_VOD       13 /*!< \brief DASH On-Demand Profile */
#define COMP_DASH_LIVE      14 /*!< \brief DASH Live Profile */
#define COMP_DPS            15 /*!< \brief DPS */
#define COMP_DPS_LIVE       16 /*!< \brief DPS Live */
#define COMP_DASH_VOD_ENC   17 /*!< \brief DASH Encrypted On-Demand Profile */
#define COMP_DASH_LIVE_ENC  18 /*!< \brief DASH Encrypted Live Profile */
#define COMP_MAX            19

/**@}*/


/*!
\section MUXMP4LL_COM_ACC Commands accepted by mp4MuxAuxInfo
@name Commands accepted by mp4MuxAuxInfo
@{*/

/*! \brief Send XML metadata template to the muxer
*/
#define SET_META_DATA_INFO  0x00000001L

/*! \brief Load XML metadata that will be written into output file
*/
#define LOAD_META_DATA_INFO 0x00000002L

/*! \brief Get information about stream by providing file name (in case of file mode) or
    by  bufstream (in streaming mode)
*/
#define GET_STREAM_INFO     0x00000003L

/*! \brief Set Asset Physical ID (APID)
*/
#define SET_APID            0x00000004L

/**@}*/


#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif

/*!
Structure to get specific info for each stream to be multiplexed

This structure is sent to ::mp4MuxAuxInfo to get additional information
for the specified stream.  Stream can be identified by file name in file mode or
by passing pointer to bufstream structure that was used during muxer initialization.

appropriate message ID is GET_STREAM_INFO
*/
typedef struct mp4mux_stream_info
{
    bufstream_tt *bs;     //!< stream identification in stream mode
    uint8_t* file_name;   //!< stream identification in file mode
    uint32_t track_id;    //!< Track ID assigned for stream
    DECLARE_RESERVED_FIELD(50, 2); //!< Reserved for future use

} mp4mux_stream_info_t;




/*! Structure to specify settings for a stream, used with ::mp4MuxAddStream, ::mp4MuxAddFile
    and ::mp4MuxAddFileBS

 */

typedef struct mp4mux_stream_settings
{
    mc_stream_format_t _format;       //!< Use to specify parameters for an input stream
    char* _extended_language;
    DECLARE_RESERVED_FIELD(248, 1); //!< Reserved for future use

} mp4mux_stream_settings_t;



/*! Structure to configure MP4 multiplexer.
    Usually this structure is filled  by ::mp4MuxDefaults than it can be changed by calling
    application to reflect specific needs. Alternatively application can fill this structure
    from scratch. It is highly recommended to call ::mp4MuxChkSettings  to verify  that
    selected options are applicable.

    The size of the structure  is  4096 bytes.
 */
struct mp4_m_settings
{
    int format;                                 //!< one of the Supported File formats defines
    int    stream_compatibility;                //!< output stream compatibility, one of the Stream compatibility options defines
    unsigned int video_sequence_length;         //!< a/v interleave
    char *output_file_title;                    //!< for Sony PSP
    int reserved1[9];                           //!< reserved for future use
    unsigned int start_frame;                   //!< timecode offset in frames for Sony PMC and in time code in SMPTE format for Quick Time (should be not equal to zero)
    unsigned char *material_id;                 //!< pointer to a 32 byte array containing UMID for Sony PMC or 0. If 0 UMID is generated internally
    int atom_ordering;                          //!< specific order 0 - moov after mdat , 1 - moov before mdat
    char *output_path;                          //!< UTF-8 encoded string that represents path where temporary file will be created
    int capture_mode;                           //!< TODO:  make documantation
    double reserved3;                           //!< reserved for future use
    int KeepAUDelimiter;                        //!< keep AU delimiter for h.264
    int KeepPS;                                 //!< keep SPS and PPS for h.264
    int RemoveSEI;                              //!< remove SEI for h.264
    int SmallAtoms;                             //!< use small atoms only
    int reserved4;                              //!< reserved for future use
    int media_time_offset;                      //!< media time offset (used for edts atom)
    int RemoveFiller;                           //!< remove filler nalus for h.264
    int reserved5;                              //!< reserved for future use
    int AAC_backward_compatible_signaling;      //!< use backward compatible signaling for aac
    int Audio_offset_ms;                        //!< audio offset in ms (500ms means no offset, 400ms = -100ms, 600ms = + 100ms etc.)
    double InputVideoTimescale;                 //!< force external video timescale for h.264
    int fragmented;                             //!< fragmented mp4; value more than zero represents fragment duration in 100NanoSecond units
                                                //!< (video fragment duration will be always GOP aligned)
    int sync_mode;                              //!< synchronization mode:
                                                //!< 0 - direct timestamps (requires valid durations
                                                //!< [start and stop time stamps must be valid for each provided sample]),
                                                //!< no any time correction will be applied in this mode;
                                                //!< 1 - auto adjust (start timestamp must present on every sample), allows to eliminate
                                                //!< any time shifts, overlaps, gaps - this mode should be used for live sources muxing;
                                                //!< 2 - reconstruct (timestamps are not present on samples, but must present in the elementary streams),
                                                //!< this method allows to mux ES directly from the files, the ES data must start from the sync position
                                                //!< no any time shifted data can be muxed within this mode
    drm_encryption_provider_tt* encrypt_provider;//!< pointer to instance of encryption provider for protection data
    unsigned int skip_box_size;                  //!< when value is 0 skip box is not created, when value is more than 0 skip box is created
                                                 //!< with size skip_box_size
    DECLARE_RESERVED_FIELD(984*4, 4);           //!< Reserved for future use
    // total 4096 bytes
};

#pragma pack(pop)

/////////////////////////////////////////////////////////////////////////////
// muxing routines
/////////////////////////////////////////////////////////////////////////////


#ifdef __cplusplus
extern "C" {
#endif

/*!
   Call this method to fill an \ref mp4_m_settings structure with defaults values
   based on one of the Supported File preset values

   @param[out] set  pointer to an \ref mp4_m_settings structure t
   @param [in] format_type  one of the Supported File formats constants

   @return  None
*/
char * MC_EXPORT_API mp4MuxDefaults(struct mp4_m_settings *set, int format_type);


/*!
   Call this method to create an mp4 muxer object

   @param[in] get_rc  optional pointer to a get resource function \ref GET_RESOURCE
   @param [in] set    pointer to a filled in mp4_m_settings structure

   @return pointer to the object that represents muxer instance, NULL if muxer creation was failed
*/
mp4_muxer_tt *  MC_EXPORT_API mp4MuxNew(
    void *(MC_EXPORT_API *get_rc)(const char* name), const struct mp4_m_settings *set);


/*!
    Call to initialize the muxer for a streaming mode muxing session,
    the audio/video data comes from audio and video bufstream
    objects passed to ::mp4MuxAddStream.

    Note that the output parameter must be a pointer to a bufstream
    created with bseek_open_fifo_buf to have a correct file.

   @param[in] muxer pointer to an mp4 muxer object created by ::mp4MuxNew
   @param[in] reserved reserved, set to NULL
   @param[in] output pointer to a bufstream_tt object for the output muxed data

   @return  0 if successful
*/
int  MC_EXPORT_API mp4MuxInitStream(mp4_muxer_tt *muxer, void *reserved, bufstream_tt *output);


/*! Call to add a stream to the muxer for a streaming mode muxing session

    @param[in] muxer  pointer to an mp4 muxer object
    @param[in] set    optional pointer to a pre-filled @ref mp4mux_stream_settings_t structure
    @param[in] input  pointer to a bufstream_tt object that provides the input stream data

    @return     0 if successful
*/
int MC_EXPORT_API mp4MuxAddStream(mp4_muxer_tt *muxer, mp4mux_stream_settings_t *set, bufstream_tt *input);


/*! Call to initialize the muxer for a file mode muxing session,
 the audio/video data comes from files added via ::mp4MuxAddFile

  @param[in]   muxer  pointer to an mp4 muxer object
  @param[in]   reserved  reserved, set to NULL
  @param[in]   output  pointer to an output filename

   @return     0 if successful
*/
int MC_EXPORT_API mp4MuxInitFile(mp4_muxer_tt *muxer, void *reserved, char *output);


/*! Call to initialize the muxer for a file bufstream mode muxing session
 where the output is sent to a bufstream_tt object.
 the audio/video data comes from files added via ::mp4MuxAddFileBS

  @param[in]   muxer  pointer to an mp4 muxer object
  @param[in]   reserved  reserved, set to NULL
  @param[in]   output  pointer to  bufstream created with \ref bseek_open_fifo_buf for the output muxed data

   @return     0 if successful
*/int MC_EXPORT_API mp4MuxInitFileBS(mp4_muxer_tt *muxer, void *reserved, bufstream_tt *output);


/*! Call to add a video, audio or subpic stream to the muxer in file mode,

   @param[in]    muxer     pointer to an mp4 muxer object
   @param[in]    set       optional pointer to a pre-filled @ref mp4mux_stream_settings_t structure
   @param[in]    input     pointer to a filename

   @return     0 if successful
*/
int MC_EXPORT_API mp4MuxAddFile(mp4_muxer_tt *muxer, mp4mux_stream_settings_t *set, char *input);


/*! Call to add a video, audio or subpic stream to the muxer in file bufstream mode,

   @param[in]    muxer     pointer to an mp4 muxer object
   @param[in]    set       optional pointer to a pre-filled @ref mp4mux_stream_settings_t structure
   @param[in]    input     pointer to bufstream for the input data

   @return     0 if successful
*/
int MC_EXPORT_API mp4MuxAddFileBS(mp4_muxer_tt *muxer, mp4mux_stream_settings_t *set, bufstream_tt *input);


/*! Call to do the multiplexing in file mode

   @param[in]       muxer  pointer to an mp4 muxer object

   @return     0 if successful
*/
unsigned long MC_EXPORT_API mp4Mux(mp4_muxer_tt *muxer);


/*! Call to finish a muxing session, set abort non-zero if muxing is being
 aborted.

   @param[in]    muxer  pointer to an mp4 muxer object
   @param[in]    abort  set to 0 to finish any leftover muxing and clean up,
            else just clean up

   @return     0 if successful
*/
int MC_EXPORT_API mp4MuxDone(mp4_muxer_tt *muxer, int abort);


/*! call to free an mp4 muxer object

   @param[in]     muxer  muxer object to free

   @return None
*/
void MC_EXPORT_API mp4MuxFree(mp4_muxer_tt *muxer);


/*! @brief Call this to use special commands  that  extends muxer functionality
*/
int MC_EXPORT_API mp4MuxAuxInfo(mp4_muxer_tt *muxer, uint32_t offs, uint32_t info_ID, void *info_ptr, uint32_t info_size);



/*! Call to get the setting errors/warnings in an mp4_m_settings structure
 use with the get_rc callback \see GET_RESOURCE to provide an err_printf callback to get
 error messages that can be localized

   @param[in]    get_rc  pointer to a get_rc function
   @param[in]    set  pointer to an mp4_m_settings structure
   @param[in]    options  reserved, set to 0
   @param[in]    app  reserved

   @return    0 if no errors found,   one of the INV_* error codes if an error is found
*/
int MC_EXPORT_API mp4MuxChkSettings(void *(MC_EXPORT_API *get_rc)(const char* name), const struct mp4_m_settings *set, unsigned int options, void *app);


/*! Call to get extend API functions

  @param[in]    func ID for function a pointer to get

  @return    function pointer or NULL
*/
APIEXTFUNC MC_EXPORT_API mp4MuxGetAPIExt(uint32_t func);


#ifdef __cplusplus
}
#endif



#endif // #if !defined (__MUX_MP4_API_INCLUDED__)

