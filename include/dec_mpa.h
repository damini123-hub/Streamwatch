/**
 @file  dec_mpa.h
 @brief  MPEG Audio Decoder API

 @verbatim
 File: dec_mpa.h

 Desc: MPEG Audio Decoder API

 Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

 MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 This software is protected by copyright law and international treaties.  Unauthorized
 reproduction or distribution of any portion is prohibited by law.
 @endverbatim
 **/
#ifndef MPEGADEC___H
#define MPEGADEC___H

#include "bufstrm.h"
#include "mcapiext.h"
#include "mcdefs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name Statistics
 * @{
 **/
typedef struct
{
  unsigned int successful;
  unsigned int channels;
  unsigned int bytes_per_sample;
  unsigned int sampling_frequency;
  unsigned int frame_samples;
  unsigned int bitrate;

  unsigned int samples_clipped;

  int reserved[256-7];

} mpa_last_frame_info;
/** @} */

/**
 * @name Statistics
 * @{
 **/

 /**
   * @brief MPA data related to overall decoding process */
typedef struct
{
  unsigned long frames_successful;
  unsigned long frames_erroneus;
  unsigned long bytes_consumed;
  unsigned long bytes_skipped;

  int reserved[256-4*2];

} mpa_statistics;
/** @} */

/**
 * @name Output information
 * @{
 **/

 /**
   * @brief MPA structure holding all output information */
typedef struct
{
  mpa_statistics             statistics;		/**< @brief MPA data related to overall decoding process */
  mpa_last_frame_info    last_frame_info;

  int reserved[256];

} mpa_output_info;
/** @} */

/**
 * @name Decoder configuration
 * @{
 **/
typedef struct
{
    mpa_output_info* p_output_info;		/**< @brief MPA structure holding all output information */
    int32_t reserved1[15];
} mpa_decoder_config;
/** @} */

/**
    * @brief Create the MPEG Audio Decoder
    * @brief Call this function to get a @ref bufstream interface for the MPEG Audio decoder
    * @return  point to a @ref bufstream interface
    **/

bufstream_tt* MC_EXPORT_API open_MPEGin_Audio_stream( void );

/**
    * @brief Create the MPEG Audio Decoder
    * @brief Call this function to get a @ref bufstream interface for the MPEG Audio decoder
	* @param[in] get_rc a pointer to a get_rc function
    * @return  point to a @ref bufstream interface
    **/
bufstream_tt* MC_EXPORT_API open_MPEGin_Audio_stream_ex( void *(MC_EXPORT_API *get_rc)(const char* name), long reserved1, long reserved2 );


APIEXTFUNC MC_EXPORT_API MPEGin_Audio_GetAPIExt(uint32_t func);


#ifdef __cplusplus
}
#endif

#endif // MPEGADEC___H
