/* ----------------------------------------------------------------------------
 * File: mcmediatypes.h
 *
 * Desc: mediatype defines used for connection between components
 *       and for specification of input mediatypes by user apps
 *
 * Copyright (c) 2015 MainConcept GmbH or its affiliates.  All rights reserved.
 *
 * MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 * This software is protected by copyright law and international treaties.  Unauthorized
 * reproduction or distribution of any portion is prohibited by law.
 * ----------------------------------------------------------------------------
 */

#if !defined (__MC_MEDIATYPES_INCLUDED__)
#define __MC_MEDIATYPES_INCLUDED__


// describes the major type of a stream or a container type
typedef enum mcmajortypes
{
    mcmjtElementary = 0,

    mcmjtPES = 1,
    mcmjtPVA = 2,
    mcmjtMPEG1System = 3,
    mcmjtMPEG2Program = 4,
    mcmjtMPEG2Transport = 5,

    mcmjtMXF = 6,

    mcmjtMP4 = 7,

    mcmjtAVI = 8,
    mcmjtMKV = 9,

    mcmjtASF = 10

} mcmajortypes_t;


// stream media types
typedef enum mcmediatypes
{
    mctUnknown = 0,
    mctMinVideoType = 1,

    mctMPEG1V = 2,
    mctMPEG2V = 3,
    mctMPEG4V = 4,
    mctH263 = 5,
    mctH264 = 10,
    mctMVCSub = 11,                    // single MVC substream
    mctMVCMux = 12,                    // MVC stream with base + all substreams
    mctHEVC = 13,
    mctVC1 = 15,
    mctDV = 20,
    mctJ2K = 25,
    mctMotionJPEG = 26,
    mctVC3 = 30,

    mctDIV3 = 35,                      // Older DivX 3.11 and 4 codecs,
    mctDIV4 = 36,                      // not completely mpeg-4 compliant

    mctMPEG4V3 = 40,                    // Microsoft MPEG-4 version 3

    mctMaxVideoType = 128,

    mctMinAudioType = 130,

    mctMPEG1A = 131,                   // MPEG-1 audio, layer is indicated in
    mctMPEG2A = 132,                   // MPEG-2 audio, the mc_audio_format_t.profile field
    mctMPEG3A = 132,                   // do not use, this does not mean layer 3!

    mctAES3_337M_MPEG1A = 133,         // MPEG-1 audio in an AES3 SMPTE 337M stream
    mctAES3_337M_MPEG2A = 134,         // MPEG-2 audio in an AES3 SMPTE 337M stream

    mctAAC_Generic = 140,              // use this media type if you don't exactly know which kind of AAC is in the stream
                                       // - RAW, ADTS or LATM, and parsing is needed
    mctAAC_RAW = 141,
    mctAAC_LATM = 142,
    mctAAC_ADTS = 143,

    mctAC3 = 150,
    mctAC3_DDPlus = 151,
    mctAES3_337M_AC3 = 152,            // Dolby Digital audio wrapped in an AES3 SMPTE 337M stream
    mctAES3_337M_DDPlus = 153,         // Dolby Digital Plus audio wrapped in an AES3 SMPTE 337M stream
    mctAES3_337M_DOLBYE = 154,         // Dolby E audio wrapped in an AES3 SMPTE 337M stream

    mctAMR = 160,

    mctWMA = 165,
    mctWMAPro = 166,
    mctWMALossless = 167,

    mctSDDS = 170,

    mctDTS = 172,

    mctMaxAudioType = 199,

    mctMinPCMType = 200,

    mctPCM = 201,
    mctDVD_LPCM = 202,
    mctHDMV_LPCM = 203,

    mctAES3_302M = 210,        // LPCM audio in an AES3 SMPTE 302M stream
    mctAES3_332M = 211,
    mctAES3_382M = 212,
    mctAES3_331M = 213,
	mctBWF = 214,

    mctTWOS_LPCM = 230,
    mctQT_PCM = 231,                   // used for generic PCM in MOV, usually the same as mctPCM
    mctIntel_ADPCM = 232,              // WAVE format tag 17 type
    mctMS_ADPCM = 233,                 // WAVE format tag 2 type

    mctAIFF = 240,

    mctALAW = 241,
	mctULAW = 242,

    mctMaxPCMType = 259,

    mctMinPrivateType = 260,
    mctDVB_Teletext = 269,
    mctPrivateBinary = 270,            // private stream to be encapsulated into TS with stream type = 0x06
    mctDVD_Subtitles = 280,
    mctDXSB_Subtitles = 281,
    mctUTF8_Subtitles = 282,
    mctSSA_Subtitles = 283,
    mctASS_Subtitles = 284,
    mctUSF_Subtitles = 285,
    mctBMP_Subtitles = 286,
    mctVSUB_Subtitles = 287,
    mctDVB_Subtitles = 288,
    mctCFF_ImgSubtitles = 289,
    mctCFF_TxtSubtitles = 290,
    mctDXSA_Subtitles = 291,

    mctVBI_Data = 300,
    mctANC_Data = 301,

    mctAES3_337M_DATA = 350,          // other data in an AES3 SMPTE 337M stream

    mctMaxPrivateType = 512

} mcmediatypes_t;


#ifdef __GNUC__
#pragma pack(push,8)
#else
#pragma pack(push)
#pragma pack(8)
#endif

#define MC_AUDIO_FORMAT_FLAG_PCM_IS_FLOAT       0x00000001    // PCM samples are floating point samples
#define MC_AUDIO_FORMAT_FLAG_PCM_IS_BIG_ENDIAN  0x00000002    // PCM samples are big endian samples
#define MC_AUDIO_FORMAT_FLAG_PCM_IS_SIGNED      0x00000004    // PCM samples are signed integer samples
#define MC_AUDIO_FORMAT_FLAG_IS_DOLBY_HYBRID    0x00000008    // Dolby stream contains both DD and DD+ frames

/** \struct mc_audio_format_t
 *	describes an audio stream
 *  \param profile              The value is specific for each media type.
 *  \param channels             The number of channels.
 *  \param samples_per_sec      The sample rate.
 *  \param avg_bytes_per_sec    The average bytes per second.
 *  \param bits_per_sample      The number of bits per sample.
 *  \param emphasis             0 - off, 1 - on, DVD LPCM audio.
 *  \param mute_flag            0 - off, 1 - on, DVD LPCM audio.
 *  \param dynamic_range_control  The DVD LPCM audio.
 *  \param language             The language code.
 *  \param reserved             reserved for future use
 */
// describes an audio stream
typedef struct mc_audio_format
{
    uint16_t profile;                  // the value is specific for each mediatype
    uint16_t channels;                 // the number of channels
    uint32_t samples_per_sec;          // the sample rate
    uint32_t avg_bytes_per_sec;        // the average bytes per second
    uint32_t bits_per_sample;          // the number of bits per sample
    uint8_t  emphasis;                 // 0 - off, 1 - on, DVD LPCM audio
    uint8_t  mute_flag;                // 0 - off, 1 - on, DVD LPCM audio
    int16_t  dynamic_range_control;    // DVD LPCM audio
    uint16_t language[4];              // language code
    uint32_t flags;                    // any of the MC_AUDIO_FORMAT_FLAG_xxx defines
    int16_t  blockSize;                // blockSize if any, like nBlockAlign in WAVEFORMATEX
    uint16_t country_code[4];          // country code
    uint16_t media_desc[4];            // media description

    uint8_t  reserved[2];

} mc_audio_format_t;


#define PASS_TYPE_UNKNOWN        0
#define PASS_TYPE_ANALYZE        1
#define PASS_TYPE_ENCODE         2

/*! \def MC_VIDEO_FORMAT_FLAG_INTERLACED
    \brief The define interlaced flag

    if known, equivalent to progressive_sequence in mpeg2, frame_mbs_only_flag in AVC, etc.
*/
#define MC_VIDEO_FORMAT_FLAG_INTERLACED    0x00000001    // if known, equivalent to progressive_sequence in mpeg2, frame_mbs_only_flag in AVC, etc
/*! \def MC_VIDEO_FORMAT_FLAG_PROGRESSIVE
    \brief The define progressive flag

    if known, equivalent to progressive_sequence in mpeg2, frame_mbs_only_flag in AVC, etc.
*/
#define MC_VIDEO_FORMAT_FLAG_PROGRESSIVE   0x00000002    // if known, equivalent to progressive_sequence in mpeg2, frame_mbs_only_flag in AVC, etc

/*! \def MC_VIDEO_FORMAT_FLAG_HAS_PULLDOWN
    \brief The define pulldown flag

    pulldown is present in at least some parts of the stream.
*/
#define MC_VIDEO_FORMAT_FLAG_HAS_PULLDOWN  0x00000004    // pulldown is present in at least some parts of the stream

/*! \def MC_VIDEO_FORMAT_FLAG_TFF
    \brief The define top field first flag

    if known, top field first flag.
*/
#define MC_VIDEO_FORMAT_FLAG_TFF           0x00000010    // if known, top field first flag

/*! \def MC_VIDEO_FORMAT_FLAG_BFF
    \brief The define bottom field first flag

    if known, bottom field first flag.
*/
#define MC_VIDEO_FORMAT_FLAG_BFF           0x00000020    // if known, bottom field first flag

/** \struct mc_video_format_t
 *	describes a video stream
 *  \param profile  The value is specific for each media type.
 *  \param level    The field takes the same value as defined in the corresponding specs.
 *  \param avg_bitrate              The average bitrate in bits per second
 *  \param avg_time_per_frame       The average time per frame, in 100-nanosecond units.
 *  \param width                    The width in pixels
 *  \param height                   The height in pixels
 *  \param pictAspRatioX            The X dimension of picture aspect ratio. For example, 16 for a 16x9 display
 *  \param pictAspRatioY            The Y dimension of the same
 *  \param is_vbr                   1 - VBR, 0 - CBR
 *  \param pass_num                 current pass number
 *  \param pass_type                one of PASS_TYPE_... defined above

 *  \param flags;                   any of the MC_VIDEO_FORMAT_FLAG_xxx defines

 *  \param stated_time_per_frame    The stated time per frame in seq type hdr, in 100-nanosecond units.
                                    This may differ from avg_time_per_frame if pulldown is present
 *  \param nalu_len                 The H.264 NALU length field size

 *  \param reserved                 reserved for future use

 */
// describes a video stream
typedef struct mc_video_format
{
    uint16_t profile;                  // the value is specific for each mediatype
    uint16_t level;                    // this field takes the same value as defined in the corresponding specs
    uint32_t avg_bitrate;              // the average bitrate in bits per second
    int64_t  avg_time_per_frame;       // the average time per frame, in 100-nanosecond units.
    uint32_t width;                    // the width in pixels
    uint32_t height;                   // the height in pixels
    uint32_t pictAspRatioX;            // the X dimension of picture aspect ratio. For example, 16 for a 16x9 display
    uint32_t pictAspRatioY;            // the Y dimension of the same
    uint8_t  is_vbr;                   // 1 - VBR, 0 - CBR
    uint8_t  pass_num;                 // current pass number
    uint8_t  pass_type;                // one of PASS_TYPE_... defined above

    uint32_t flags;                    // any of the MC_VIDEO_FORMAT_FLAG_xxx defines

    int64_t  stated_time_per_frame;    // the stated time per frame in seq type hdr, in 100-nanosecond units.
                                       // this may differ from avg_time_per_frame if pulldown is present
    uint8_t  nalu_len;                 // H.264 NALU length field size

    int32_t comp_mode;                 // compression mode

    uint8_t  reserved[44];

} mc_video_format_t;


/** \struct mc_subtitle_format_t
 *	describes a subtitle stream
 *  \param profile              The value is specific for each media type.
 *  \param avg_time_per_frame   The average time per frame, in 100-nanosecond units.
 *  \param width                The width in pixels.
 *  \param height               The height in pixels.
 *  \param language             The language code.
 *  \param bit_depth            The bit depth of a bitmap type subtitle.
 *  \param bitmap_size          The size of a fixed size bitmap subtitle.
 *  \param reserved             reserved for future use
 */

// describes a subtitle stream
typedef struct mc_subtitle_format
{
    uint16_t profile;                  // the value is specific for each mediatype
    int64_t  avg_time_per_frame;       // the average time per frame, in 100-nanosecond units.
    uint32_t width;                    // the width in pixels
    uint32_t height;                   // the height in pixels
    uint16_t language[4];              // language code

    int16_t  bit_depth;                // bitmap bit depth
    uint32_t bitmap_size;              // bitmap size

    uint16_t country_code[4];          // country code

    uint8_t *xml_namespace;            // null-terminated C string in UTF-8
    uint8_t *xml_schema_location;      // null-terminated C string in UTF-8

    uint16_t media_desc[4];            // media description

    uint8_t  reserved[8];

} mc_subtitle_format_t;

/** \struct mc_private_format_t
 *	describes a private stream
 *  \param reserved              up to user how to use the structure
 */
// describes a private stream
typedef struct mc_private_format
{
    uint8_t  reserved[64];             // up to user how to use the structure

} mc_private_format_t;

/** \struct mc_stream_format_t
 *	describes the format of a stream
 *  \param stream_majortype     The major type, usually mcmjtElementary here.
 *  \param stream_mediatype     The media type.
 *  \param pFormat              The pointer to one of the media structures (mc_video_format_t, mc_audio_format_t and etc).
 *  \param reserved             The value reserved for future use.
 *  \param cbExtraBytes         The number of bytes in pbExtraData.
 *  \param pbExtraData          The can hold SPS for video, or any additional info needed.
 */

// describes the format of a stream
typedef struct mc_stream_format
{
    mcmajortypes_t  stream_majortype;  // the major type, usually mcmjtElementary here
    mcmediatypes_t  stream_mediatype;  // the media type

    void           *pFormat;           // pointer to one of the above structures

    uint8_t         reserved[32];

    uint16_t        cbExtraBytes;      // number of bytes in pbExtraData
    uint8_t        *pbExtraData;       // can hold SPS for video, or any additional info needed

} mc_stream_format_t;

#pragma pack(pop)

#endif // #if !defined (__MC_MEDIATYPES_INCLUDED__)

