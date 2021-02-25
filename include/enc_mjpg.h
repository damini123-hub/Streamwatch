/**
@file: enc_mjpg.h
@brief MJPG Encoder API

@verbatim
File: enc_mjpg.h
Desc: MJPG Encoder API

Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
This software is protected by copyright law and international treaties.  Unauthorized
reproduction or distribution of any portion is prohibited by law.
@endverbatim
 **/

#if !defined (__ENC_MJPG_API_INCLUDED__)
#define __ENC_MJPG_API_INCLUDED__

#include "mctypes.h"
#include "mcfourcc.h"
#include "mcapiext.h"


////////// available fourcc's
/*
#define FOURCC_BGR3 0x33524742
#define FOURCC_BGRA 0x41524742
#define FOURCC_BGR4 0x34524742
#define FOURCC_UYVY 0x59565955
#define FOURCC_R555 0x35353552
#define FOURCC_R565 0x35363552
#define FOURCC_YUY2 0x32595559
#define FOURCC_YUYV 0x56595559
#define FOURCC_VYUY 0x59555956
*/

#ifndef FOURCC_VYUY
#define FOURCC_VYUY MAKEFOURCC('V', 'Y', 'U', 'Y')
#endif
#ifndef FOURCC_YuYv
#define FOURCC_YuYv MAKEFOURCC('Y', 'u', 'Y', 'v') //No clamping to ITU601 range, (decode only for now)
#endif
#ifndef FOURCC_UyVy
#define FOURCC_UyVy MAKEFOURCC('U', 'y', 'V', 'y') //No clamping to ITU601 range  (decode only for now)
#endif
#ifndef FOURCC_bgra
#define FOURCC_bgra MAKEFOURCC('b', 'g', 'r', 'a') //No clamping to ITU601 range  (decode only for now)
#endif
#ifndef FOURCC_bgr4
#define FOURCC_bgr4 MAKEFOURCC('b', 'g', 'r', '4') //No clamping to ITU601 range  (decode only for now)
#endif
#ifndef FOURCC_bgr3
#define FOURCC_bgr3 MAKEFOURCC('b', 'g', 'r', '3') //No clamping to ITU601 range  (decode only for now)
#endif
//////////


////////// compression flags
#define COMP_ONE_FIELD_ONLY    0x00000002  /**<@brief use one field only @hideinitializer*/
#define COMP_INV_FIELD_ORDER   0x00000004  /**<@brief invert field order, upper and lower fields are switched @hideinitializer*/
#define COMP_INV_AVI1_ORDER    0x00000020  /**<@brief change the order of AVI1 markers, usable only to produce the stream like produces on some kind of hardware.@hideinitializer*/
#define COMP_BITRATE_STRONG    0x00000080  /**<@brief allow to do the recoding if size of compressed frame is too big or too small @hideinitializer*/
#define COMP_BITRATE_USE       0x00010000  /**<@brief try to keep the bitrate, the “quality“ parameters will be used as required size of encoded frame @hideinitializer*/
#define COMP_INTERLACE_USE     0x00020000  /**<@brief use interlaced encoding, must be used for ITU601R source @hideinitializer*/

#define COMP_WRITE_DHT         0x00000001  /**<@brief put Huffman table into stream @hideinitializer*/
#define COMP_APP0_JFIF2        0x00700000  /**<@brief generate JFIF v1.2 app0 marker @hideinitializer*/


#ifdef __cplusplus
extern "C" {
#endif

/**
\brief With CompressBuffer_MJPG a MJPEG frame is encoded from the specified input color format:
\return Returns the size of the compressed data or 0 on error (for example unsupported colorspace)
\param[in] pb_src pointer to source buffer
\param[in] stride	size of one line in byte (stride) in source buffer, could be negative
\param[in] pb_dst	pointer to destination buffer
\param[in] dst_buf_len	length of destination buffer (must be big)
\param[in] width	width of input image in pixel
\param[in] height	height of input image in pixel
\param[in] flags	combination of the COMP_xxx flags (see below)
\param[in] fourcc	destination buffer color space, one of the FOURCC_xxx
\param[in] opt_flags	reserved, set to 0
\param[in] ext_info	reserved, set to NULL
*/
uint32_t MC_EXPORT_API CompressBuffer_MJPG(unsigned char  *pb_src,
                                           int32_t         stride,
                                           unsigned char  *pb_dst,
                                           uint32_t        dst_buf_len,
                                           uint32_t        width,
                                           uint32_t        height,
                                           uint32_t        quality,
                                           uint32_t        flags,
                                           uint32_t        fourcc,
                                           uint32_t        opt_flags,
                                           void           *ext_info);

/**
\brief Provides access to extended module API.
\return  pointer to requested function or NULL
\param[in] func identifier of module extended function
 */
APIEXTFUNC MC_EXPORT_API MJPG_Compress_GetAPIExt(uint32_t func);


#ifdef __cplusplus
}
#endif

#endif // #if !defined (__ENC_MJPG_API_INCLUDED__)

