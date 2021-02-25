/**
 @file trans_audio_converter.h
 @brief  AudioConverter API declaration

 @verbatim
 File: trans_audio_converter.h

 Desc: AudioConverter API declaration

 Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

 MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 This software is protected by copyright law and international treaties.  Unauthorized
 reproduction or distribution of any portion is prohibited by law.
 @endverbatim
 **/


#ifndef __AC_HPP__
#define __AC_HPP__

#include "bufstrm.h"
#include "mcapiext.h"

typedef struct audio_converter audio_converter_tt;  ///< audio converter object

/// Maximum number of channels
const int MAX_CH_NUMBER = 8;

/// Audio format defines
enum data_format_t
{
    DATA_PCM = 1,			///< little-endian PCM
    DATA_DVD_LPCM = 2,		///< DVD LPCM audio
    DATA_HDMV_LPCM = 3, 	///< HDMV LPCM audio
    DATA_AES3 = 4,			///< AES3 audio
    DATA_FLOAT = 5			///< float
};

/// Channel mask defines
enum audio_flag_t
{
    CHANNEL_FLAG_L   =   0x1,		///< left channel
    CHANNEL_FLAG_R   =   0x2,		///< right channel
    CHANNEL_FLAG_C   =   0x4,		///< center channel
    CHANNEL_FLAG_LFE =   0x8,		///< LFE channel
    CHANNEL_FLAG_Ls  =   0x10,		///< left surround channel
    CHANNEL_FLAG_Rs  =   0x20,		///< right surround channel
    CHANNEL_FLAG_Lrs =   0x40,		///< left back surround channel
    CHANNEL_FLAG_Rrs =   0x80		///< left back surround channel
};

#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif

/// Audio converter settings
struct converter_settings
{
    uint8_t     input_channels;                               ///< Number of input channels, correct value range 1-8.
    uint8_t     input_bits_per_sample;                        ///< Input bits per sample.
    uint8_t     input_format;                                 ///< Input audio format.
    uint32_t    input_channel_mask;                           ///< Input channel mask.
    uint32_t    input_samplerate;                             ///< Input sample rate, correct value range 1 - 256000.

    uint8_t     output_channels;                              /**< @brief Number of output channels, correct value range 0-8.
                                                               * 0 - means the same number of channels as input. */
    uint8_t     output_bits_per_sample;                       ///< Output bits per sample.
    uint8_t     output_format;                                ///< Output audio format.
    uint32_t    output_channel_mask;                          ///< Output channel mask.
    uint32_t    output_samplerate;                            /**< @brief Output sample rate, correct value range 1 - 256000.
                                                               * 0 - means the same sample rate as input. */

    uint32_t    out_frame_size;                               ///< Maximal output frame size (bytes), correct value range is 1 - 16777215.

    uint8_t     user_matrix;                                  ///< Specifies if user mixing matrix will be used.
    double      mixing_matrix[MAX_CH_NUMBER][MAX_CH_NUMBER];  ///< User mixing matrix. Used only if user_matrix=1.

    int8_t     use_custom_in_table;                           ///< Specifies if custom input table is used. Otherwise default input order is used.
    int8_t     in_channel_order[MAX_CH_NUMBER];               ///< Order of channels in input stream.

    int8_t     use_custom_out_table;                          ///< Specifies if custom output table is used. Otherwise default output order is used.
    int8_t     out_channel_order[MAX_CH_NUMBER];              ///< Order of channels in output stream.

    int8_t     separate_input_channels_mode;                  /**< @brief Specifies whether @ref AUDIO_CONVERTER_SEPARATE_CHANNELS_MODE "separate channels mode" is used for input channels or not.
                                                               * 0 - the mode is not used.
                                                               * 1 - the mode is used. */

    int8_t     separate_output_channels_mode;                 /**< @brief Specifies whether @ref AUDIO_CONVERTER_SEPARATE_CHANNELS_MODE "separate channels mode" is used for output channels or not.
                                                               * 0 - the mode is not used.
                                                               * 1 - the mode is used. */

    int8_t    reserved[3537];                                 ///< Reserved for later usage.
};

#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Call this function to create an @ref audio_converter_tt audio converter object.
  *
  * Call this function to create an @ref audio_converter_tt audio converter object.
  * @param[in] get_rc pointer to a get resource function
  * @param[in] set pointer to a filled in converter_settings structure
  * @return pointer to a audio_converter_tt object if successful
  * @return NULL if unsuccesful
*/
audio_converter_tt * MC_EXPORT_API audioConverterNew(void *(MC_EXPORT_API *get_rc)(const char* name), struct converter_settings *set);


/** @brief Call this function to free an @ref audio_converter_tt object.
  *
  * Call this function to free an @ref audio_converter_tt object.
  * @param[in] a_conv pointer to a audio_converter_tt object
*/
void       MC_EXPORT_API audioConverterFree(audio_converter_tt *a_conv);


/** @brief Call this function to initialize the Audio Converter.
  *
  * Call this function to initialize the Audio Converter. An existing @ref bufstream object must be passed to this function,
  * the audio converter will output the converted data to the @ref bufstream object.
  * @param[in] a_conv pointer to an @ref audio_converter_tt object
  * @param[in] bs pointer to a @ref bufstream_tt object for the converted data output stream
  * @return 0 if successful
  * @return 1 if unsuccessful
*/
int32_t    MC_EXPORT_API audioConverterInit(audio_converter_tt *a_conv, bufstream_tt *bs);


/** @brief Call this function to initialize an audio channel.
  *
  * Call this function to initialize an audio channel in @ref AUDIO_CONVERTER_SEPARATE_CHANNELS_MODE "separate channels mode". Call this function for all channels you have.
  * An existing @ref bufstream object must be passed to this function, the audio converter will output the converted data
  * to the @ref bufstream object.
  * @param[in] a_conv pointer to an @ref audio_converter_tt object
  * @param[in] bs pointer to a @ref bufstream_tt object for the converted data output stream
  * @param[in] channel_index channel index
  * @return 0 if successful
  * @return 1 if unsuccessful
*/
int32_t    MC_EXPORT_API audioConverterInitChannel(audio_converter_tt *a_conv, bufstream_tt *bs, int channel_index);


/** @brief Call this function to convert a buffer of audio.
  *
  * Call this function to convert a buffer of audio. The @ref input_format, @ref input_bits_per_sample, @ref input_channels,
  * @ref input_channel_mask, @ref input_sample_rate fields of the @ref converter_settings structure passed to the
  * @ref audioConverterNew function must match the type of audio data in the buffer.
  * @param[in] a_conv pointer to an @ref audio_converter_tt object
  * @param[in] audio_buffer pointer to an audio buffer
  * @param[in] data_size the number of bytes in the audio buffer
  * @param[in] options reserved, set to 0
  * @param[in] ext_info reserved, set to NULL
  * @return 0 if successful
  * @return 1 if unsuccessful
*/
int32_t    MC_EXPORT_API audioConverterPutBytes(audio_converter_tt *a_conv, uint8_t *audio_buffer, uint32_t data_size,
                               uint32_t options, void **ext_info);


/** @brief Call this function to convert one channel.
  *
  * Call this function to convert one channel in @ref AUDIO_CONVERTER_SEPARATE_CHANNELS_MODE "separate channels mode". Call this function for all channels to convert data from a multichannel stream. The @ref input_format, @ref input_bits_per_sample, @ref input_channels,
  * @ref input_channel_mask, @ref input_sample_rate fields of the @ref converter_settings structure passed to the
  * @ref audioConverterNew function must match the type of audio data in the buffer.
  * @param[in] a_conv pointer to an @ref audio_converter_tt object
  * @param[in] audio_buffer pointer to an audio buffer
  * @param[in] data_size the number of bytes in the audio buffer
  * @param[in] channel_index channel index
  * @param[in] options reserved, set to 0
  * @param[in] ext_info reserved, set to NULL
  * @return 0 if successful
  * @return 1 if unsuccessful
*/
int32_t    MC_EXPORT_API audioConverterPutBytesOneChannel(audio_converter_tt *a_conv, uint8_t *audio_buffer, uint32_t data_size,
                            uint8_t channel_index, uint32_t options, void **ext_info);


/** @brief Call this function to finish converting data for a channel.
  *
  * Call this function to finish converting data for a channel in @ref AUDIO_CONVERTER_SEPARATE_CHANNELS_MODE "separate channels mode". Call this function for a channel if there is no more data in this channel.
  * After this call the Audio Converter will fill this channel with the silence if there are data in other channels.
  * @param[in] a_conv pointer to an @ref audio_converter_tt object
  * @param[in] channel_index channel index
  * @return 0 if successful
  * @return 1 if unsuccessful
*/
int32_t    MC_EXPORT_API audioConverterNoMoreDataInChannel(audio_converter_tt *a_conv, int channel_index);


/** @brief Call to finish an audio converter session, set the abort flag to a non-zero if audio converting is being aborted.
  *
  * Call to finish an audio converter session, set the abort flag to a non-zero if audio converting is being aborted.
  * @param[in] a_conv pointer to an @ref audio_converter_tt object
  * @param[in] abort flag, set to a non-zero value to abort converting
  * @return 0 if successful
  * @return 1 if unsuccessful
*/
int32_t    MC_EXPORT_API audioConverterDone(audio_converter_tt *a_conv, int32_t abort);


/** @brief Call this function to check whether the audio settings are valid.
  *
  * Call this function to check whether the audio settings are valid.<br>If the get_rc parameter is non-NULL, any error
  * messages are sent to the err_printf function.
  * @param[in] get_rc pointer to a get_rc function
  * @param[in] set pointer to a filled in @ref converter_settings structure
  * @param[in] options reserved, set to 0
  * @param[in] app reserved, set to NULL
  * @return 0 if settings are valid
  * @return 1 if settings are invalid
*/
int32_t    MC_EXPORT_API audioConverterCheckSettings(void *(MC_EXPORT_API *get_rc)(const char* name),
                               struct converter_settings *set, unsigned int options, void *app);


/** @brief Utility function. Call this function calculates size in bytes of n_sample samples of the specified format.
  *
  * Utility function. Call this function calculates size in bytes of n_sample samples of the specified format.
  * @param[in] n_samples the number of samples
  * @param[in] format format of audio
  * @param[in] n_channels the number of channels
  * @param[in] bps the number of bits per channel
  * @return amount of bytes required for n_samples samples of the specified format.
*/
uint32_t   MC_EXPORT_API audioConverterSamplesToBytes(uint32_t n_samples, uint8_t format, uint8_t n_channels, uint8_t bps);


/** @brief Utility function. Call this function calculates amount of samples of the specified format that can be decoded
  * from data_size bytes.
  *
  * Utility function. Call this function calculates amount of samples of the specified format that can be decoded from
  * data_size bytes.
  * @param[in] data_size size of data in bytes
  * @param[in] format format of audio
  * @param[in] n_channels the number of channels
  * @param[in] bps the number of bits per channel
  * @return amount of samples of the specified format that can be decoded from data_size bytes.
*/
uint32_t   MC_EXPORT_API audioConverterBytesToSamples(uint32_t data_size, uint8_t format, uint8_t n_channels, uint8_t bps);

APIEXTFUNC MC_EXPORT_API audioConverterGetAPIExt(uint32_t func);

#ifdef __cplusplus
}
#endif

#endif //__AC_HPP__
