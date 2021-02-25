/**
 @file  fhg_enc_aac.h
 @brief Fraunhofer AAC Encoder API

 @verbatim
 File: fhg_enc_aac.h

 Desc: Fraunhofer AAC Encoder API

 Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

 MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 This software is protected by copyright law and international treaties.  Unauthorized
 reproduction or distribution of any portion is prohibited by law.
 @endverbatim
 **/

#if !defined (__ENC_FHG_AAC_API_INCLUDED__)
#define __ENC_FHG_AAC_API_INCLUDED__

#include "bufstrm.h"
#include "mcdefs.h"
#include "mcprofile.h"
#include "mcapiext.h"

/////////////////////////////////////////////////////////////////////////////
// API structures
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief encoder instance type
 */
typedef struct fhg_aac_a_encoder fhg_aac_aenc_tt;

#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif

/**
 * @brief These parameters are used to configure the encoder.
 */
struct fhg_aac_a_settings
{
    uint32_t input_channels;            ///<@brief Number of input audio channels
    uint32_t ch_mode;                   ///<@brief Input channel mode (one of the FHG_AAC_CH_* values)
    uint32_t aac_object_type;           ///<@brief AAC audio object type
    uint32_t audio_bitrate_index;       ///<@brief Audio bitrate
    uint32_t header_type;               ///<@brief Type of the header
    uint32_t vbr;                       /**<@brief Specifies if vbr (variable bitrate) mode is used instead of cbr (constant bitrate) mode, and if
                                          * used, the target quality level for variable bitrate */
    uint32_t sbr_signaling;             ///<@brief SBR signaling mode (one of the FHG_AAC_SBR_SIG_* values)
    uint32_t bits_per_sample;           ///<@brief Number of bits per one PCM sample
    uint32_t protect_adts_stream;       ///<@brief Specifies if audio stream contains CRC protection
    int64_t  pts_offset;                ///<@brief Time stamp offset (starting value)
    int32_t  pts_units;                 ///<@brief Units for the time stamp offset
    uint32_t metadata_mode;             ///<@brief Program metadata mode (one of the FHG_AAC_METADATA_* values)
    uint32_t granule_length;            ///<@brief Granule length (one of the FHG_AAC_GRANULE_* values)
    uint32_t quality;                   ///<@brief Encoding quality (one of the FHG_AAC_QUAL_* values)
    int64_t  restart_interval;          ///<@brief Time stamp restart interval (measured in pts)
    int64_t  *chapter_list;             ///<@brief List of restart time stamps
    int32_t  chapter_list_size;         ///<@brief Number of restart time stamps in the list
    uint32_t dash_segmentation_flag;    ///<@brief DASH segmentation flag. Enables/disables DASH compatibility mode. Dash mode is active only when restart interval or restart timestamps are indicated.
    uint32_t audio_bitrate_custom;      ///<@brief Custom audio bitrate
    int32_t  use_sbr_crc;               ///<@brief Value specifying the usage of CRC bits. If use_sbr_crc does not equal zero, additional CRC bits are added to the bit stream. Applied for HE AAC and Parametric Stereo.
    int32_t  sbr_hdr_time_interval;     ///<@brief Value specifying time in milliseconds, used for the calculation of the transmission interval between SBR headers. A value < 0 enables an SBR header only in the first frame. Applied for HE AAC and Parametric Stereo. Default value is 1000 milliseconds (1 second).
    int32_t  latm_smc_time_interval;    ///<@brief Specifies update interval for appearance of stream mux config in case of LOAS/LATM transport stream (in frames).
    /**
      * @name Reserved
      * @{
    **/
    int32_t reserved_int32[176];
    void *reserved_ptr[56];
    /** @} */
};

/**
 * @brief This structure is used to pass metadata to the encoder via @ref fhgAacOutAudioSubmitMetadata function.
 */
typedef struct fhg_aac_a_metadata_s
{
    int32_t drc_profile;              ///<@brief MPEG DRC compression profile (one of the FHG_AAC_DRC_* values)
    int32_t comp_profile;             ///<@brief ETSI heavy compression profile (one of the FHG_AAC_DRC_* values)

    float drc_target_ref_level;       ///<@brief used to define expected level to adjust limiter to avoid overload
    float comp_target_ref_level;      ///<@brief used to define expected level to adjust limiter to avoid overload

    float drc_ext;                    ///<@brief external feed DRC compression value
    float comp_ext;                   ///<@brief external feed heavy compression value

    int32_t prog_ref_level_present;   ///<@brief flag, if prog_ref_level is present
    float prog_ref_level;             ///<@brief Programme Reference Level = Dialogue Level (-31.75dB .. 0 dB ; stepsize: 0.25dB)

    int32_t pce_mixdown_idx_present;  ///<@brief flag, if dmx-idx should be written in programme config element
    int32_t etsi_dmx_level_present;   ///<@brief flag, if dmx-lvl should be written in ETSI-ancData
    int32_t center_mix_level;         ///<@brief center downmix level (one of the FHG_AAC_DMX_GAIN_* values)
    int32_t surround_mix_level;       ///<@brief surround downmix level (one of the FHG_AAC_DMX_GAIN_* values)

    int32_t dolby_surround_mode;      ///<@brief  Indication for Dolby Surround Encoding Mode (one of the FHG_AAC_DSUR_* values)
    int32_t drc_presentation_mode;    ///<@brief DRC presentation mode (ETSI) (one of the FHG_AAC_DRC_PRESENTATION_* values)

    /**
      * @name Preprocessing flags
      * @{
    **/
    int32_t dc_filter;                ///<@brief not supported. Flag specifying if DC filtering is applied to input
    int32_t lfe_lowpass_filter;       ///<@brief not supported. Flag specifying if 120 Hz low-pass filter is applied to LFE channel
    int32_t sur_phase_90;             ///<@brief not supported. Flag specifying if 90 degree phase shift is applied to surround channels
    int32_t sur_att_3dB;              ///<@brief not supported. Flag specifying if 3 dB attenuation is applied to surround channels
    /** @} */

    /**
      * @name Reserved
      * @{
    **/
    int32_t reserved_int32[80];
    void *reserved_ptr[30];
    /** @} */
} fhg_aac_a_metadata;

/**
 * @brief This structure is used to get the audio specific configuration (ASC) from the encoder via @ref fhgAacOutGetAudioSpecificConfig function.
 */
typedef struct fhg_aac_ascbuf_s
{
    int32_t nbits;          ///<@brief The length of the ASC in bits
    uint8_t buffer[64];     ///<@brief An array containing the ASC
} fhg_aac_ascbuf;

#pragma pack(pop)

/////////////////////////////////////////////////////////////////////////////
// API functions
/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Call this function to fill an @ref fhg_aac_a_settings structure with
 * default values based on a profile type.
 *
 * @param[in] set a pointer to a @ref fhg_aac_a_settings structure to be initialized
 * @param[in] profile one of the preset values for profile. This field specifies
 * which default values will be used. It should be one of the following defines:
 * <table>
 * <tr><th>value</th><th>meaning</th></tr>
 * <tr><td>@ref MCPROFILE_DEFAULT</td><td>default settings</td></tr>
 * <tr><td>@ref MCPROFILE_PSP</td><td>settings compatible with PlayStation Portable by Sony</td></tr>
 * <tr><td>@ref MCPROFILE_IPOD</td><td>settings compatible with iPod by Apple</td></tr>
 * <tr><td>@ref MCPROFILE_3GPP</td><td>settings compatible with 3GPP specification</td></tr>
 * <tr><td>@ref MCPROFILE_3GPP2</td><td>settings compatible with 3GPP2 specification</td></tr>
 * <tr><td>@ref MCPROFILE_ISMA</td><td>settings compatible with ISMA specification</td></tr>
 * <tr><td>@ref MCPROFILE_FLASH_LOWRES</td><td>settings for Adobe Flash (Low Resolution WebContent)</td></tr>
 * <tr><td>@ref MCPROFILE_FLASH_HIGHRES</td><td>settings for Adobe Flash (High Resolution Web Content) </td></tr>
 * <tr><td>@ref MCPROFILE_SILVERLIGHT</td><td>Microsoft Silverlight compatible settings</td></tr>
 * <tr><td>@ref MCPROFILE_IIS_SMOOTHSTREAMING</td><td>fMP4 IIS Smooth Streaming compatible settings </td></tr>
 * <tr><td>@ref MCPROFILE_DPS</td><td>DivX Plus Streaming compatible settings </td></tr>
 * <tr><td>@ref MCPROFILE_DPS_A_51C</td><td>DivX Plus Streaming 5.1 compatible settings</td></tr>
 * </table>
 *
 * @param[out] set modified fhg_aac_a_settings structure
 * @return short description if the ID is valid
 * @return NULL if not
 */
char* MC_EXPORT_API fhgAacOutAudioDefaults(struct fhg_aac_a_settings *set, int32_t profile);


/** @brief Call this function to create an @ref fhg_aac_aenc_tt audio encoding object
 * which is used in other AAC audio encoding calls.
 *
 * @param[in] get_rc pointer to a get resource function
 * @param[in] set pointer to a filled in @ref fhg_aac_a_settings structure
 * @param[in] options reserved, set to 0
 * @param[in] CPU set to 0xFFFFFFFF for autodetect (reserved for CPU-flags)
 * @param[in] sampleRate sample rate of the input audio, must be: 8, 11.025, 12, 16, 22.05, 24, 32, 44.1, 48, 64, 88.2, 96 kHz
 * @return pointer to a @ref fhg_aac_aenc_tt object if succesful
 * @return NULL if unsuccesful
 */
fhg_aac_aenc_tt* MC_EXPORT_API fhgAacOutAudioNew( void *(MC_EXPORT_API *get_rc)(const char* name),const struct fhg_aac_a_settings *set,int32_t options,int32_t CPU,int32_t sampleRate);


/** @brief Call this function to free an @ref fhg_aac_aenc_tt object
 *
 * @param[in] instance a pointer to a @ref fhg_aac_aenc_tt object
 */
void MC_EXPORT_API fhgAacOutAudioFree(fhg_aac_aenc_tt* instance);


/** @brief Call this function to initialize the Fraunhofer AAC Encoder for an encoding session.
 *
 * An existing bufstream object must be passed to this function, the Fraunhofer AAC Encoder
 * will output the encoded data to the bufstream object.
 *
 * @param[in] instance a pointer to a @ref fhg_aac_aenc_tt object
 * @param[in] audiobs a pointer to a @ref bufstream_tt object for the compressed
 * data output stream
 * @return @ref aacOutErrNone if successful
 * @return @ref aacOutError if not
 */
int32_t MC_EXPORT_API fhgAacOutAudioInit(fhg_aac_aenc_tt *instance, bufstream_tt *audiobs);


/** @brief Call this function to encode a buffer of audio, which must contain 8-bit,
 * 16-bit and 24-bit PCM audio data.
 *
 * The input_channels and/or ch_mode fields of the @ref fhg_aac_a_settings structure passed to the @ref
 * fhgAacOutAudioNew function must match the type of audio data in the buffer. In addition,
 * the sampling rate passed to the @ref fhgAacOutAudioNew function must match the sampling
 * rate of the audio data in the buffer.
 *
 * @param[in] instance a pointer to a @ref fhg_aac_aenc_tt object
 * @param[in] audioBuffer a pointer to an audio buffer containing 8-bit, 16-bit and
 * 24-bit PCM audio data
 * @param[in] numAudioBytes the number of bytes in the audio buffer
 * @return @ref aacOutErrNone if successful
 * @return @ref aacOutError if not
 */
int32_t MC_EXPORT_API fhgAacOutAudioPutBytes(fhg_aac_aenc_tt *instance, uint8_t *audioBuffer, uint32_t numAudioBytes);


/** @brief Call to finish an AAC audio encoding session, set the abort flag
 * to a non-zero if audio encoding is being aborted.
 *
 * @param[in] instance a pointer to a @ref fhg_aac_aenc_tt object
 * @param[in] abort flag, set to a non-zero value to abort encoding
 * @return @ref aacOutErrNone if successful
 * @return @ref aacOutError if not
 */
int32_t MC_EXPORT_API fhgAacOutAudioDone(fhg_aac_aenc_tt *instance, int32_t abort);


/// add this flag to options arg to signal that settings can be corrected
#define FHG_AAC_ENC_CORRECT_SETTINGS     0x0001000

/** @brief Call this function to check whether the audio settings are valid.
 *
 * If the get_rc parameter is non-NULL, any error messages are sent to the err_printf function.
 * If the options parameter does contain @ref FHG_AAC_ENC_CORRECT_SETTINGS, the function corrects
 * user settings (despite it is passed with const qualifier for compatibility reason) and
 * does not fail unless invalid sample rate or channel configuration are specified. If the
 * options parameter does not contain @ref CHECK_ALL, the function returns when the first non
 * compliant value is found (the remainder of the settings are not checked). If the options
 * parameter does contain @ref CHECK_ALL, all the checks are performed, an error message is
 * sent to the err_printf callback for every error found and the error code of the first error
 * found is returned. To fully check the audio settings, the mpeg type (supports values are
 * @ref MPEG_MPEG1 .. @ref MPEG_LAST_MPEG_TYPE) and audio frequency is needed. If the sample_rate
 * parameter is not a valid MPEG audio frequency, this call will fail.
 *
 * @param[in] get_rc a pointer to a get_rc function
 * @param[in] set a pointer to a filled in fhg_aac_a_settings structure
 * @param[in] reserved
 * @param[in] sample_rate the audio frequency, either 11025, 12000, 16000, 22050, 24000,
 * 32000, 44100, 48000, 64000, 88200 or 96000
 * @param[in] options @ref CHECK_MPEG_ONLY and/or @ref CHECK_ALL
 * @param[in] app reserved, set to NULL
 * @return @ref aacOutErrNone if settings are valid
 * @return one of the INV_AAC_* error codes if an error is found (see @ref mcdefs.h)
 */
int32_t MC_EXPORT_API fhgAacOutAudioChkSettings(void *(MC_EXPORT_API *get_rc)(const char* name), const struct fhg_aac_a_settings *set, int32_t reserved, int32_t sample_rate, uint32_t options, void *app);

/** @brief Call this function to get default metadata parameters.
 *
 * The function can be called in any time between @ref fhgAacOutAudioInit() and @ref fhgAacOutAudioDone() functions.
 * It always returns only default metadata parameters for specified mpeg_type, not a submitted parameters.
 *
 * @param[out] metadata a pointer to a @ref fhg_aac_a_metadata structure
 * @param[in] reserved
 * @return @ref aacOutErrNone if successful
 * @return @ref aacOutError if not
 */
int32_t MC_EXPORT_API fhgAacOutAudioGetDefaultMetadata(fhg_aac_a_metadata *metadata, int32_t reserved);

/** @brief Call this function to check whether the metadata parameters are valid.
 *
 * If the get_rc parameter is non-NULL, any error messages are sent to the err_printf function.
 * If the options parameter does not contain @ref CHECK_ALL, the function returns when the first 
 * non compliant value is found (the remainder of the settings are not checked). If the options
 * parameter does contain @ref CHECK_ALL, all the checks are performed, an error message is
 * sent to the err_printf callback for every error found and the error code of the first error
 * found is returned. If the options parameter does contain @ref FHG_AAC_ENC_CORRECT_SETTINGS, the function corrects
 * metadata parameters (despite it is passed with const qualifier for compatibility reason) and
 * does not fail.
 *
 * @param[in] get_rc a pointer to a get_rc function
 * @param[in] set a pointer to a filled @ref fhg_aac_a_metadata structure
 * @param[in] reserved one of the MPEG_* defines, used for the VCD/SVCD/DVD/DVB/MicroMV/DVHS checks (see @ref mcdefs.h)
 * @param[in] options @ref CHECK_MPEG_ONLY and/or @ref CHECK_ALL
 * @param[in] app reserved, set to NULL
 * @return @ref aacOutErrNone if settings are valid
 * @return one of the INV_AAC_* error codes if an error is found (see @ref mcdefs.h)
 */
int32_t MC_EXPORT_API fhgAacOutAudioChkMetadata(void *(MC_EXPORT_API *get_rc)(const char* name), fhg_aac_a_metadata *metadata, int32_t reserved, uint32_t options, void *app);

/** @brief Call this function to set or update metadata parameters.
 *
 * The function can be called in any time between @ref fhgAacOutAudioInit() and @ref fhgAacOutAudioDone() functions.
 * The first call will set metadata parameters. Subsequent calls will update metadata parameters.
 *
 * @param[in] instance a pointer to a @ref fhg_aac_aenc_tt object
 * @param[in] metadata a pointer to a filled @ref fhg_aac_a_metadata structure
 * @return @ref aacOutErrNone if successful
 * @return @ref aacOutError if not
 */
int32_t MC_EXPORT_API fhgAacOutAudioSubmitMetadata(const fhg_aac_aenc_tt *instance, const fhg_aac_a_metadata *metadata);

/** @brief Call this function to get the audio specific configuration (ASC).
 *
 * The function can be called in any time between @ref fhgAacOutAudioInit() and @ref fhgAacOutAudioDone() functions.
 *
 * @param[in] instance a pointer to a @ref fhg_aac_aenc_tt object
 * @param[out] ascbuf a pointer to a @ref fhg_aac_ascbuf structure
 * @return @ref aacOutErrNone if successful
 * @return @ref aacOutError if not
 */
int32_t MC_EXPORT_API fhgAacOutGetAudioSpecificConfig(const fhg_aac_aenc_tt *instance, fhg_aac_ascbuf *ascbuf);


APIEXTFUNC MC_EXPORT_API fhgAacOutAudioGetAPIExt(uint32_t func);

#ifdef __cplusplus
}
#endif

//--------------------------------------------------------------------------

#endif // #if !defined (__ENC_FHG_AAC_API_INCLUDED__)