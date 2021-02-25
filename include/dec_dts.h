/** 
 @file  dec_dts.h
 @brief DTS Audio Decoder API
 
 @verbatim
 File: dec_dts.h 
 Desc: DTS Audio Decoder API 
 
 Copyright (c) 2009, MainConcept GmbH. All rights reserved.

 This software is the confidential and proprietary information of
 MainConcept GmbH and may be used only in accordance with the terms of
 your license from MainConcept GmbH.
 @endverbatim
 **/
 
#if !defined (__DEC_DTS_API_INCLUDED__)
#define __DEC_DTS_API_INCLUDED__

#include "bufstrm.h"
#include "mcapiext.h"


/////////////////////////////////////////////////////////////////////////////
// API structures & defines
/////////////////////////////////////////////////////////////////////////////

/**                                                          
 * @brief maximum number of channels                                      
 **/ 
#define DTS_DEC_MAX_CHAN        31

/**                                                          
 * @brief DTS decoder audio frame information                                        
 **/ 
typedef struct
{

  int sampling_rate;				/**< @brief sampling rate of output audio data */
  int num_channels;					/**< @brief number of output channels */

  int samples_per_channel;			/**< @brief number of audio samples decoded for every channel */
  int decoded_frame_size;			/**< @brief size in bytes of decoded frame */

  int reserved[11];

} dts_decoded_frame_info;

/**                                                          
 * @brief DTS decoder configuration                                       
 **/ 
typedef struct
{
  // input stream info
  int bit_width;				/**< @brief number of output bits per sample
									@brief Available value: 8, 16 or 24 */

  int reserved0[20];

} dts_decoder_config;


/////////////////////////////////////////////////////////////////////////////
// API functions
/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

	/** 
    * @brief Create the DTS Audio Decoder
    *
    * @return  point to a bufstream interface
    **/
bufstream_tt* MC_EXPORT_API open_DTSin_Audio_stream(void);

	/** 
    * @brief Create the DTS Audio Decoder
    *
    * @return  point to a bufstream interface
    **/
bufstream_tt* MC_EXPORT_API open_DTSin_Audio_stream_ex(void *(MC_EXPORT_API *get_rc)(const char* name), long reserved1, long reserved2);

#ifdef __cplusplus
}
#endif

#endif // #if !defined (__DEC_DTS_API_INCLUDED__)

