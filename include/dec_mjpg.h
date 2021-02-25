/**
\file: dec_mjpg.h
\brief MJPG Decoder API

\verbatim
File: dec_mjpg.h
Desc: MJPG Decoder API

Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
This software is protected by copyright law and international treaties.  Unauthorized
reproduction or distribution of any portion is prohibited by law.
\endverbatim
 **/

#if !defined (__DEC_MJPG_API_INCLUDED__)
#define __DEC_MJPG_API_INCLUDED__

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


////////// decompression flags
#define DCOMP_ONE_FIELD_SMART  0x00000100	/**<\brief use interpolation to show missing (not decoded) field \hideinitializer */
#define DCOMP_ONE_FIELD_ONLY   0x00000200	/**<\brief decode only one field \hideinitializer */
#define DCOMP_INV_FIELD_ORDER  0x00000400	/**<\brief invert field order, upper and lower fields are switched \hideinitializer */
#define DCOMP_USE_AVI1         0x00002000	/**<\brief use content of AVI1 app0 marker to find out the field order \hideinitializer */
#define DCOMP_IGNORE_ITU601    0x00004000   /**<\brief do not clip output YCbCr to ITU601R range for FOURCC_UYVY, FOURCC_YUY2 and FOURCC_YUYV \hideinitializer */



#ifdef __cplusplus
extern "C" {
#endif

/**
\brief With DecompressBuffer_MJPG a MJPEG frame is decoded into to specified output color format:
\return Returns 1 if successful and 0 on error (for example unsupported colorspace).
\param[in] pb_src	pointer to MJPEG source buffer
\param[in] src_buf_len	length of source buffer
\param[in, out] pb_dst	pointer to destination buffer
\param[in] stride	size of one line in bytes in destination buffer, could be negative
\param[in] width	width of output image in pixel,
\param[in] height	height of output image in pixel,
\param[in] flags	combination of the DCOMP_xxx flags (see below)
\param[in] fourcc	destination buffer color space, one of the FOURCC_xxx
\param[in] opt_flags	reserved, set to 0
\param[in] ext_fnfo	reserved, set to NULL
*/
uint32_t MC_EXPORT_API DecompressBuffer_MJPG(unsigned char  *pb_src,
                                             uint32_t        src_buf_len,
                                             unsigned char  *pb_dst,
                                             int32_t         stride,
                                             uint32_t        width,
                                             uint32_t        height,
                                             uint32_t        flags,
                                             uint32_t        fourcc,
                                             uint32_t        opt_flags,
                                             void           *ext_info);

/**
\brief Provides access to extended module API.
\return  pointer to requested function or NULL
\param[in] func identifier of module extended function
 */
APIEXTFUNC MC_EXPORT_API MJPG_Decompress_GetAPIExt(uint32_t func);

#ifdef __cplusplus
}
#endif

#endif // #if !defined (__DEC_MJPG_API_INCLUDED__)

