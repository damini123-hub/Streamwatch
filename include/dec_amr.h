/**
 @file  dec_amr.h
 @brief  AMR Decoder API

 @verbatim
 File: dec_amr.h

 Desc: AMR Decoder API

 Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

 MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 This software is protected by copyright law and international treaties.  Unauthorized
 reproduction or distribution of any portion is prohibited by law.
 @endverbatim
 **/
#if !defined (__DEC_AMR_API_INCLUDED__)
#define __DEC_AMR_API_INCLUDED__

#include "mctypes.h"
#include "bufstrm.h"
#include "mcdefs.h"
#include "mcapiext.h"

/////////////////////////////////////////////////////////////////////////////
// API functions
/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @name AMR modes
 * @{
 **/
#define AMR_NB_MODE 0		/**< @brief AMR Narrow Band mode */
#define AMR_WB_MODE 1		/**< @brief AMR Wide Band mode */
/** @} */

/**
 * @name Decoder configuration
 * @{
 **/
typedef struct
{
  int32_t delay_removal;
  int32_t reserved1[15];
} amr_decoder_config;
/** @} */

/**
    * @brief Create the AMR Decoder
    * @brief Call this function to get a @ref bufstream interface for the AMR decoder
	* @param[in] get_rc a pointer to a get_rc function
	* @param[in] amr_mode AMR mode, support values are @ref AMR_NB_MODE or @ref AMR_WB_MODE
	* @param[in] interface_format bitstream interface format mode
	* <table>
	* <tr><th>value</th><th>meaning</th></tr>
	* <tr><td>1</td><td>Interface format 1</td></tr>
	* <tr><td>2</td><td>Interface format 2</td></tr>
	* </table>
    * @return  point to a @ref bufstream interface
    **/
bufstream_tt* MC_EXPORT_API open_AMRin_Audio_stream(void *(get_rc)(const char* name), long amr_mode, long interface_format);
void          MC_EXPORT_API done_amr(bufstream_tt * bs, int abort);
void          MC_EXPORT_API free_amr(bufstream_tt * bs);

APIEXTFUNC    MC_EXPORT_API AMRin_Audio_GetAPIExt(uint32_t func);

#ifdef __cplusplus
}
#endif

#endif // #if !defined (__DEC_AMR_API_INCLUDED__)

