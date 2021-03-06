/**
\file   mcfourcc.h
\brief  fourcc defines

\verbatim
File: mcfourcc.h
Desc: fourcc defines

 Copyright (c) 2015 MainConcept GmbH or its affiliates.  All rights reserved.

 MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 This software is protected by copyright law and international treaties.  Unauthorized
 reproduction or distribution of any portion is prohibited by law.
\endverbatim
**/

#if !defined (__MC_FOURCC_INCLUDED__)
#define __MC_FOURCC_INCLUDED__

#include "mctypes.h"

#ifndef MAKEFOURCC
  #define MAKEFOURCC(ch0, ch1, ch2, ch3)  \
    ((uint32_t)(int8_t)(ch0) | ((uint32_t)(int8_t)(ch1) << 8) | \
    ((uint32_t)(int8_t)(ch2) << 16) | ((uint32_t)(int8_t)(ch3) << 24 ))
#endif // MAKEFOURCC

/**
  * @name FourCC defines
  * @{
**/

//#ifndef FOURCC_RGB3
//#define FOURCC_RGB3 MAKEFOURCC('R', 'G', 'B', '3')
//#endif

#ifndef FOURCC_BGR3
/**@brief RGB24, packed RGB, 24 bpp
* @n Reference to the description http://msdn.microsoft.com/en-us/library/dd407253(VS.85).aspx
* @hideinitializer */
#define FOURCC_BGR3 MAKEFOURCC('B', 'G', 'R', '3')
#endif

#ifndef FOURCC_BGRA
/**@brief BGRA32, packed RGB with alpha channel, 32 bpp
* @n Reference to the description http://msdn.microsoft.com/en-us/library/dd407253(VS.85).aspx
* @hideinitializer */
#define FOURCC_BGRA MAKEFOURCC('B', 'G', 'R', 'A')
#endif

#ifndef FOURCC_RGBA
/**@brief RGBA32, packed RGB with alpha channel, 32 bpp
* @hideinitializer */
#define FOURCC_RGBA MAKEFOURCC('R', 'G', 'B', 'A')
#endif

#ifndef FOURCC_BGRA32F
/**@brief BGRA, packed RGB, 32 bits per component (floating point), 128 bpp
* @hideinitializer */
#define FOURCC_BGRA32F MAKEFOURCC('B', '3', '2', 'F')
#endif

#ifndef FOURCC_B32F
/**@brief BGRA, packed RGB, 32 bits per component (floating point), 128 bpp
* @hideinitializer */
#define FOURCC_B32F MAKEFOURCC('B', '3', '2', 'F')
#endif

#ifndef FOURCC_ARGB
/**@brief ARGB32, packed RGB with alpha channel, 32 bpp
* @n ARGB Colorspace as common used on Macintosh. May not be available on other platforms.
* @hideinitializer */
#define FOURCC_ARGB MAKEFOURCC('A', 'R', 'G', 'B')
#endif

#ifndef FOURCC_BGR4
/**@brief RGB32, packed RGB, 32 bpp
* @n Reference to the description http://msdn.microsoft.com/en-us/library/dd407253(VS.85).aspx
* @hideinitializer */
#define FOURCC_BGR4 MAKEFOURCC('B', 'G', 'R', '4')
#endif

#ifndef FOURCC_R555
/**@brief RGB555, packed RGB, 16 bpp
* @n Reference to the description http://msdn.microsoft.com/en-us/library/dd407253(VS.85).aspx
* @hideinitializer */
#define FOURCC_R555 MAKEFOURCC('R', '5', '5', '5')
#endif

#ifndef FOURCC_R565
/**@brief RGB565, packed RGB, 16 bpp
* @n Reference to the description http://msdn.microsoft.com/en-us/library/dd407253(VS.85).aspx
* @hideinitializer */
#define FOURCC_R565 MAKEFOURCC('R', '5', '6', '5')
#endif

#ifndef FOURCC_AYUV
/**@brief AYUV, packed YUV, 4:4:4, 32 bpp
* @n Reference to the description http://msdn.microsoft.com/en-us/library/dd206750(VS.85).aspx#ayuv
* @hideinitializer */
#define FOURCC_AYUV MAKEFOURCC('A', 'Y', 'U', 'V')
#endif

#ifndef FOURCC_VUYA
/**@brief VUYA, packed YUV, 4:4:4, 32 bpp
* @hideinitializer */
#define FOURCC_VUYA MAKEFOURCC('V', 'U', 'Y', 'A')
#endif

#ifndef FOURCC_VUYA32F
/**@brief VUYA, packed YUV, 4:4:4, 32 bits per component (floating point), 128 bpp
* @hideinitializer */
#define FOURCC_VUYA32F MAKEFOURCC('Y', '3', '2', 'F')
#endif

#ifndef FOURCC_Y32F
/**@brief VUYA, packed YUV, 4:4:4, 32 bits per component (floating point), 128 bpp
* @hideinitializer */
#define FOURCC_Y32F MAKEFOURCC('Y', '3', '2', 'F')
#endif

#ifndef FOURCC_UYVY
/**@brief UYVY, packed YUV, 4:2:2, 16 bpp
* @n Reference to the description http://msdn.microsoft.com/en-us/library/dd206750(VS.85).aspx#uyvy
* @hideinitializer */
#define FOURCC_UYVY MAKEFOURCC('U', 'Y', 'V', 'Y')
#endif

#ifndef FOURCC_YUY2
/**@brief YUYV, packed YUV, 4:2:2, 16 bpp
* @n Reference to the description http://msdn.microsoft.com/en-us/library/dd206750(VS.85).aspx#yuy2
* @hideinitializer */
#define FOURCC_YUY2 MAKEFOURCC('Y', 'U', 'Y', '2')
#endif

#ifndef FOURCC_YUYV
/**@brief YUYV, packed YUV, 4:2:2, 16 bpp
* @n Reference to the description http://msdn.microsoft.com/en-us/library/dd206750(VS.85).aspx#yuy2
* @hideinitializer */
#define FOURCC_YUYV MAKEFOURCC('Y', 'U', 'Y', 'V')
#endif

#ifndef FOURCC_YVYU
/**@brief YVYU, packed YUV, 4:2:2, 16 bpp
* @n Reference to the description is the same as @ref FOURCC_YUYV, but the U and V samples are swapped.
* @hideinitializer */
#define FOURCC_YVYU MAKEFOURCC('Y', 'V', 'Y', 'U')
#endif

#ifndef FOURCC_VYUY
/**@brief VYUY, packed YUV, 4:2:2, 16 bpp
* @n Reference to the description http://www.fourcc.org/yuv.php
* @hideinitializer */
#define FOURCC_VYUY MAKEFOURCC('V', 'Y', 'U', 'Y')
#endif

#ifndef FOURCC_EXTP
/**@brief External, 3-plane colorspace-converter (requires a valid ext_cc_struct)
* @hideinitializer */
#define FOURCC_EXTP MAKEFOURCC('E', 'X', 'T', 'P')
#endif

#ifndef FOURCC_I420
/**@brief YV12, planar YUV, 4:2:0, planes order: Y->U->V, 12 bpp
* @hideinitializer */
#define FOURCC_I420 MAKEFOURCC('I', '4', '2', '0')
#endif

#ifndef FOURCC_I422
/**@brief YV16, planar YUV, 4:2:2, planes order: Y->U->V, 16 bpp
* @hideinitializer */
#define FOURCC_I422 MAKEFOURCC('I', '4', '2', '2')
#endif

#ifndef FOURCC_I444
/**@brief YV24, planar YUV, 4:4:4, planes order: Y->U->V, 24 bpp
* @hideinitializer */
#define FOURCC_I444 MAKEFOURCC('I', '4', '4', '4')
#endif

#ifndef FOURCC_YV12
/**@brief YV12, planar YUV, 4:2:0, planes order: Y->V->U, 12 bpp
* @hideinitializer */
#define FOURCC_YV12 MAKEFOURCC('Y', 'V', '1', '2')
#endif

#ifndef FOURCC_IYUV
/**@brief YV12, planar YUV, 4:2:0, planes order: Y->U->V, 12 bpp
* @hideinitializer */
#define FOURCC_IYUV MAKEFOURCC('I', 'Y', 'U', 'V')
#endif

#ifndef FOURCC_YV16
/**@brief YV16, planar YUV, 4:2:2, planes order: Y->V->U, 16 bpp
* @hideinitializer */
#define FOURCC_YV16 MAKEFOURCC('Y', 'V', '1', '6')
#endif

#ifndef FOURCC_YV24
/**@brief YV24, planar YUV, 4:4:4, planes order: Y->V->U, 24 bpp
* @hideinitializer */
#define FOURCC_YV24 MAKEFOURCC('Y', 'V', '2', '4')
#endif

#ifndef FOURCC_Y41P
/**@brief Y41P, packed YUV, 4:1:1, 12 bpp
* @n Reference to the description http://www.fourcc.org/yuv.php#Y41P
* @hideinitializer */
#define FOURCC_Y41P MAKEFOURCC('Y', '4', '1', 'P')
#endif

#ifndef FOURCC_NV12
/**@brief NV12, planar YUV, 4:2:0, planes order Y->UV, UV interleaved, 12 bpp
* @n Reference to the description http://msdn.microsoft.com/en-us/library/dd206750(VS.85).aspx#nv12
* @hideinitializer */
#define FOURCC_NV12 MAKEFOURCC('N', 'V', '1', '2')
#endif

#ifndef FOURCC_NV21
/**@brief NV21, planar YUV, 4:2:0, planes order Y->UV, UV interleaved, 12 bpp
* @n Reference to the description http://www.fourcc.org/yuv.php#NV21
* @hideinitializer */
#define FOURCC_NV21 MAKEFOURCC('N', 'V', '2', '1')
#endif

#ifndef FOURCC_NV24
/**@brief NV24, planar YUV, 4:4:4, planes order Y->UV, UV interleaved, 24 bpp
* @hideinitializer */
#define FOURCC_NV24 MAKEFOURCC('N', 'V', '2', '4')
#endif

#ifndef FOURCC_IMC1
/**@brief YV12, planar YUV, 4:2:0, planes order: Y->V->U, 12 bpp
* @hideinitializer */
#define FOURCC_IMC1 MAKEFOURCC('I', 'M', 'C', '1')
#endif

#ifndef FOURCC_IMC2
/**@brief YV12, planar YUV, 4:2:0, planes order: Y->U->V, 12 bpp
* @hideinitializer */
#define FOURCC_IMC2 MAKEFOURCC('I', 'M', 'C', '2')
#endif

#ifndef FOURCC_IMC3
/**@brief YV12, planar YUV, 4:2:0, planes order: Y->V->U, UV interleaved, 12 bpp
* @hideinitializer */
#define FOURCC_IMC3 MAKEFOURCC('I', 'M', 'C', '3')
#endif

#ifndef FOURCC_IMC4
/**@brief YV12, planar YUV, 4:2:0, planes order: Y->U->V, UV interleaved, 12 bpp
* @hideinitializer */
#define FOURCC_IMC4 MAKEFOURCC('I', 'M', 'C', '4')
#endif

#ifndef FOURCC_YVU9
/**@brief YVU9, planar YUV, planes order Y->V->U, 9 bpp
* @n Reference to the description http://www.fourcc.org/yuv.php#YVU9
* @hideinitializer */
#define FOURCC_YVU9 MAKEFOURCC('Y', 'V', 'U', '9')
#endif

#ifndef FOURCC_YUV9
/**@brief YUV9, planar YUV, planes order Y->U->V, 9 bpp
* @n Reference to the description http://www.fourcc.org/yuv.php#YUV9
* @hideinitializer */
#define FOURCC_YUV9 MAKEFOURCC('Y', 'U', 'V', '9')
#endif

#ifndef FOURCC_Y211
/**@brief Y211, packed YUV, 2:1:1, 8 bits per component, 8 bpp
* @n Reference to the description http://www.fourcc.org/yuv.php#Y211
* @hideinitializer */
#define FOURCC_Y211 MAKEFOURCC('Y', '2', '1', '1')
#endif

#ifndef FOURCC_CLJR
/**@brief CLJR, packed YUV, 4:1:1, 8 bpp
* @n Reference to the description http://fourcc.org/yuv.php#CLJR
* @hideinitializer */
#define FOURCC_CLJR MAKEFOURCC('C', 'L', 'J', 'R')
#endif

#ifndef FOURCC_RAW
/**@brief Uncompressed data
* @hideinitializer */
#define FOURCC_RAW  MAKEFOURCC('R', 'A', 'W', ' ')
#endif

#ifndef FOURCC_R32C
/**@brief RGB32, packed RGB, 32 bpp (16:black, 235-white)
* @hideinitializer */
#define FOURCC_R32C MAKEFOURCC('R', '3', '2', 'C')
#endif

#ifndef FOURCC_argb
/**@brief ARGB32, packed RGB with alpha channel, 32 bpp (16:black, 235-white)
* @hideinitializer */
#define FOURCC_argb MAKEFOURCC('a', 'r', 'g', 'b')
#endif

#ifndef FOURCC_R24C
/**@brief RGB24, packed RGB, 24 bpp (16:black, 235-white)
* @hideinitializer */
#define FOURCC_R24C MAKEFOURCC('R', '2', '4', 'C')
#endif

#ifndef FOURCC_YuYv
/**@brief YUYV, packed YUV, 4:2:2, 16 bpp (No clamping to ITU601 range)
* @hideinitializer */
#define FOURCC_YuYv MAKEFOURCC('Y', 'u', 'Y', 'v')
#endif

#ifndef FOURCC_UyVy
/**@brief UYVY, packed YUV, 4:2:2, 16 bpp (No clamping to ITU601 range)
* @hideinitializer */
#define FOURCC_UyVy MAKEFOURCC('U', 'y', 'V', 'y')
#endif

#ifndef FOURCC_XYZ8
/**@brief D-cinema xyz colorspace, packed XYZ, 8 bits per component, 24 bpp
* @hideinitializer */
#define FOURCC_XYZ8 MAKEFOURCC('X', 'Y', 'Z', 0x08)
#endif

#ifndef FOURCC_XYZ12
/**@brief D-cinema xyz colorspace, packed XYZ, 16 bits per component but only low 12 bits are significant, 48 bpp
* @hideinitializer */
#define FOURCC_XYZ12 MAKEFOURCC('X', 'Y', 'Z', 0x0C)
#endif

#ifndef FOURCC_PLXYZ8
/**@brief D-cinema xyz colorspace, planar XYZ, 8 bits per component, 24 bpp
* @hideinitializer */
#define FOURCC_PLXYZ8 MAKEFOURCC('P', 'X', 'Y', 0x08)
#endif

#ifndef FOURCC_PLXYZ12
/**@brief D-cinema xyz colorspace, planar XYZ, 16 bits per component but only low 12 bits are significant, 48 bpp
* @hideinitializer */
#define FOURCC_PLXYZ12 MAKEFOURCC('P', 'X', 'Y', 0x0C)
#endif

#ifndef FOURCC_DPX10
/**@brief 10bit DPX format SMPTE 268M, packed RGB, 10 bits per component, 32 bpp
* @hideinitializer */
#define FOURCC_DPX10 MAKEFOURCC('D', 'P', 'X', 0x0A)
#endif

#ifndef FOURCC_V210
/**@brief Quicktime v210, packed YUV, 4:2:2, 10 bits per component, 20 bpp
* @n Reference to the description https://developer.apple.com/library/mac/technotes/tn2162/_index.html
* @hideinitializer */
#define FOURCC_V210  MAKEFOURCC('v', '2', '1', '0')
#endif

#ifndef FOURCC_v210
/**@brief Quicktime v210, packed YUV, 4:2:2, 10 bits per component, 20 bpp
* @n Reference to the description https://developer.apple.com/library/mac/technotes/tn2162/_index.html
* @hideinitializer */
#define FOURCC_v210  MAKEFOURCC('v', '2', '1', '0')
#endif

#ifndef FOURCC_W009
/**@brief Planar YUV, 4:2:0, planes order: Y->V->U, 16 bits per component but only low 9 bits are significant, 24 bpp
* @hideinitializer */
#define FOURCC_W009 MAKEFOURCC('W', '0', '0', '9')
#endif

#ifndef FOURCC_W010
/**@brief Planar YUV, 4:2:0, planes order: Y->V->U, 16 bits per component but only low 10 bits are significant, 24 bpp
* @hideinitializer */
#define FOURCC_W010 MAKEFOURCC('W', '0', '1', '0')
#endif

#ifndef FOURCC_W011
/**@brief Planar YUV, 4:2:0, planes order: Y->V->U, 16 bits per component but only low 11 bits are significant, 24 bpp
* @hideinitializer */
#define FOURCC_W011 MAKEFOURCC('W', '0', '1', '1')
#endif

#ifndef FOURCC_W012
/**@brief Planar YUV, 4:2:0, planes order: Y->V->U, 16 bits per component but only low 12 bits are significant, 24 bpp
* @hideinitializer */
#define FOURCC_W012 MAKEFOURCC('W', '0', '1', '2')
#endif

#ifndef FOURCC_W014
/**@brief Planar YUV, 4:2:0, planes order: Y->V->U, 16 bits per component but only low 14 bits are significant, 24 bpp
* @hideinitializer */
#define FOURCC_W014 MAKEFOURCC('W', '0', '1', '4')
#endif

#ifndef FOURCC_W016
/**@brief Planar YUV, 4:2:0, planes order: Y->V->U, 16 bits per component, 24 bpp
* @hideinitializer */
#define FOURCC_W016 MAKEFOURCC('W', '0', '1', '6')
#endif

#ifndef FOURCC_X009
/**@brief Planar YUV, 4:2:0, planes order: Y->U->V, 16 bits per component but only low 9 bits are significant, 24 bpp
* @hideinitializer */
#define FOURCC_X009 MAKEFOURCC('X', '0', '0', '9')
#endif

#ifndef FOURCC_X010
/**@brief Planar YUV, 4:2:0, planes order: Y->U->V, 16 bits per component but only low 10 bits are significant, 24 bpp
* @hideinitializer */
#define FOURCC_X010 MAKEFOURCC('X', '0', '1', '0')
#endif

#ifndef FOURCC_X011
/**@brief Planar YUV, 4:2:0, planes order: Y->U->V, 16 bits per component but only low 11 bits are significant, 24 bpp
* @hideinitializer */
#define FOURCC_X011 MAKEFOURCC('X', '0', '1', '1')
#endif

#ifndef FOURCC_X012
/**@brief Planar YUV, 4:2:0, planes order: Y->U->V, 16 bits per component but only low 12 bits are significant, 24 bpp
* @hideinitializer */
#define FOURCC_X012 MAKEFOURCC('X', '0', '1', '2')
#endif

#ifndef FOURCC_X014
/**@brief Planar YUV, 4:2:0, planes order: Y->U->V, 16 bits per component but only low 14 bits are significant, 24 bpp
* @hideinitializer */
#define FOURCC_X014 MAKEFOURCC('X', '0', '1', '4')
#endif

#ifndef FOURCC_X016
/**@brief Planar YUV, 4:2:0, planes order: Y->U->V, 16 bits per component, 24 bpp
* @hideinitializer */
#define FOURCC_X016 MAKEFOURCC('X', '0', '1', '6')
#endif

#ifndef FOURCC_W209
/**@brief Planar YUV, 4:2:2, planes order: Y->V->U, 16 bits per component but only low 9 bits are significant, 32 bpp
* @hideinitializer */
#define FOURCC_W209 MAKEFOURCC('W', '2', '0', '9')
#endif

#ifndef FOURCC_W210
/**@brief Planar YUV, 4:2:2, planes order: Y->V->U, 16 bits per component but only low 10 bits are significant, 32 bpp
* @hideinitializer */
#define FOURCC_W210 MAKEFOURCC('W', '2', '1', '0')
#endif

#ifndef FOURCC_W211
/**@brief Planar YUV, 4:2:2, planes order: Y->V->U, 16 bits per component but only low 11 bits are significant, 32 bpp
* @hideinitializer */
#define FOURCC_W211 MAKEFOURCC('W', '2', '1', '1')
#endif

#ifndef FOURCC_W212
/**@brief Planar YUV, 4:2:2, planes order: Y->V->U, 16 bits per component but only low 12 bits are significant, 32 bpp
* @hideinitializer */
#define FOURCC_W212 MAKEFOURCC('W', '2', '1', '2')
#endif

#ifndef FOURCC_W214
/**@brief Planar YUV, 4:2:2, planes order: Y->V->U, 16 bits per component but only low 14 bits are significant, 32 bpp
* @hideinitializer */
#define FOURCC_W214 MAKEFOURCC('W', '2', '1', '4')
#endif

#ifndef FOURCC_W216
/**@brief Planar YUV, 4:2:2, planes order: Y->V->U, 16 bits per component, 32 bpp
* @hideinitializer */
#define FOURCC_W216 MAKEFOURCC('W', '2', '1', '6')
#endif

#ifndef FOURCC_X209
/**@brief Planar YUV, 4:2:2, planes order: Y->U->V, 16 bits per component but only low 9 bits are significant, 32 bpp
* @hideinitializer */
#define FOURCC_X209 MAKEFOURCC('X', '2', '0', '9')
#endif

#ifndef FOURCC_X210
/**@brief Planar YUV, 4:2:2, planes order: Y->U->V, 16 bits per component but only low 10 bits are significant, 32 bpp
* @hideinitializer */
#define FOURCC_X210 MAKEFOURCC('X', '2', '1', '0')
#endif

#ifndef FOURCC_X211
/**@brief Planar YUV, 4:2:2, planes order: Y->U->V, 16 bits per component but only low 11 bits are significant, 32 bpp
* @hideinitializer */
#define FOURCC_X211 MAKEFOURCC('X', '2', '1', '1')
#endif

#ifndef FOURCC_X212
/**@brief Planar YUV, 4:2:2, planes order: Y->U->V, 16 bits per component but only low 12 bits are significant, 32 bpp
* @hideinitializer */
#define FOURCC_X212 MAKEFOURCC('X', '2', '1', '2')
#endif

#ifndef FOURCC_X214
/**@brief Planar YUV, 4:2:2, planes order: Y->U->V, 16 bits per component but only low 14 bits are significant, 32 bpp
* @hideinitializer */
#define FOURCC_X214 MAKEFOURCC('X', '2', '1', '4')
#endif

#ifndef FOURCC_X216
/**@brief Planar YUV, 4:2:2, planes order: Y->U->V, 16 bits per component, 32 bpp
* @hideinitializer */
#define FOURCC_X216 MAKEFOURCC('X', '2', '1', '6')
#endif

#ifndef FOURCC_W409
/**@brief Planar YUV, 4:4:4, planes order: Y->V->U, 16 bits per component but only low 9 bits are significant, 48 bpp
* @hideinitializer */
#define FOURCC_W409 MAKEFOURCC('W', '4', '0', '9')
#endif

#ifndef FOURCC_W410
/**@brief Planar YUV, 4:4:4, planes order: Y->V->U, 16 bits per component but only low 10 bits are significant, 48 bpp
* @hideinitializer */
#define FOURCC_W410 MAKEFOURCC('W', '4', '1', '0')
#endif

#ifndef FOURCC_W411
/**@brief Planar YUV, 4:4:4, planes order: Y->V->U, 16 bits per component but only low 11 bits are significant, 48 bpp
* @hideinitializer */
#define FOURCC_W411 MAKEFOURCC('W', '4', '1', '1')
#endif

#ifndef FOURCC_W412
/**@brief Planar YUV, 4:4:4, planes order: Y->V->U, 16 bits per component but only low 12 bits are significant, 48 bpp
* @hideinitializer */
#define FOURCC_W412 MAKEFOURCC('W', '4', '1', '2')
#endif

#ifndef FOURCC_W414
/**@brief Planar YUV, 4:4:4, planes order: Y->V->U, 16 bits per component but only low 14 bits are significant, 48 bpp
* @hideinitializer */
#define FOURCC_W414 MAKEFOURCC('W', '4', '1', '4')
#endif

#ifndef FOURCC_W416
/**@brief Planar YUV, 4:4:4, planes order: Y->V->U, 16 bits per component, 48 bpp
* @hideinitializer */
#define FOURCC_W416 MAKEFOURCC('W', '4', '1', '6')
#endif

#ifndef FOURCC_X409
/**@brief Planar YUV, 4:4:4, planes order: Y->U->V, 16 bits per component but only low 9 bits are significant, 48 bpp
* @hideinitializer */
#define FOURCC_X409 MAKEFOURCC('X', '4', '0', '9')
#endif

#ifndef FOURCC_X410
/**@brief Planar YUV, 4:4:4, planes order: Y->U->V, 16 bits per component but only low 10 bits are significant, 48 bpp
* @hideinitializer */
#define FOURCC_X410 MAKEFOURCC('X', '4', '1', '0')
#endif

#ifndef FOURCC_X411
/**@brief Planar YUV, 4:4:4, planes order: Y->U->V, 16 bits per component but only low 11 bits are significant, 48 bpp
* @hideinitializer */
#define FOURCC_X411 MAKEFOURCC('X', '4', '1', '1')
#endif

#ifndef FOURCC_X412
/**@brief Planar YUV, 4:4:4, planes order: Y->U->V, 16 bits per component but only low 12 bits are significant, 48 bpp
* @hideinitializer */
#define FOURCC_X412 MAKEFOURCC('X', '4', '1', '2')
#endif

#ifndef FOURCC_X414
/**@brief Planar YUV, 4:4:4, planes order: Y->U->V, 16 bits per component but only low 14 bits are significant, 48 bpp
* @hideinitializer */
#define FOURCC_X414 MAKEFOURCC('X', '4', '1', '4')
#endif

#ifndef FOURCC_X416
/**@brief Planar YUV, 4:4:4, planes order: Y->U->V, 16 bits per component, 48 bpp
* @hideinitializer */
#define FOURCC_X416 MAKEFOURCC('X', '4', '1', '6')
#endif

#ifndef FOURCC_P010
/**@brief P010, planar YUV, 4:2:0, 16 bits per component but only high 10 bits are significant, 24 bpp
* @n Reference to the description http://msdn.microsoft.com/en-us/library/bb970578(VS.85).aspx#_420formats
* @hideinitializer */
#define FOURCC_P010 MAKEFOURCC('P', '0', '1', '0')
#endif

#ifndef FOURCC_P016
/**@brief P016, planar YUV, 4:2:0, 16 bits per component, 24 bpp
* @n Reference to the description http://msdn.microsoft.com/en-us/library/bb970578(VS.85).aspx
* @hideinitializer */
#define FOURCC_P016 MAKEFOURCC('P', '0', '1', '6')
#endif

#ifndef FOURCC_P210
/**@brief P210, planar YUV, 4:2:2, 16 bits per component but only high 10 bits are significant, 32 bpp
* @n Reference to the description http://msdn.microsoft.com/en-us/library/bb970578(VS.85).aspx#_422formats
* @hideinitializer */
#define FOURCC_P210 MAKEFOURCC('P', '2', '1', '0')
#endif

#ifndef FOURCC_P216
/**@brief P216, planar YUV, 4:2:2, 16 bits per component, 32 bpp
* @n Reference to the description http://msdn.microsoft.com/en-us/library/bb970578(VS.85).aspx#_422formats
* @hideinitializer */
#define FOURCC_P216 MAKEFOURCC('P', '2', '1', '6')
#endif

#ifndef FOURCC_Y210
/**@brief Y210, packed YUV, 4:2:2, 16 bits per component but only high 10 bits are significant, 32 bpp
* @n Reference to the description http://msdn.microsoft.com/en-us/library/bb970578(VS.85).aspx#_422formats
* @hideinitializer */
#define FOURCC_Y210 MAKEFOURCC('Y', '2', '1', '0')
#endif

#ifndef FOURCC_Y216
/**@brief Y216, packed YUV, 4:2:2, 16 bits per component, 32 bpp
* @n Reference to the description http://msdn.microsoft.com/en-us/library/bb970578(VS.85).aspx#_422formats
* @hideinitializer */
#define FOURCC_Y216 MAKEFOURCC('Y', '2', '1', '6')
#endif

#ifndef FOURCC_Y410
/**@brief Y410, packed YUV, 4:4:4, 10 bits per component, 2 bits of alpha, 48 bpp
* @n Reference to the description http://msdn.microsoft.com/en-us/library/bb970578(VS.85).aspx#_444formats
* @hideinitializer */
#define FOURCC_Y410 MAKEFOURCC('Y', '4', '1', '0')
#endif

#ifndef FOURCC_Y416
/**@brief Y416, packed YUV, 4:4:4, 16 bits per component, 64 bpp
* @n Reference to the description http://msdn.microsoft.com/en-us/library/bb970578(VS.85).aspx#_444formats
* @hideinitializer */
#define FOURCC_Y416 MAKEFOURCC('Y', '4', '1', '6')
#endif

#ifndef FOURCC_H264
/**@brief AVC/H.264 format
* @hideinitializer */
#define FOURCC_H264 MAKEFOURCC('H', '2', '6', '4')
#endif

#ifndef FOURCC_Y16
/**@brief Y only, 16bpp
* @hideinitializer */
#define FOURCC_Y16 MAKEFOURCC(' ', 'Y', '1', '6')
#endif

#ifndef FOURCC_V216
/**@brief Quicktime v216, packed YUV, 4:2:2, 16 bits per component, 32 bpp
* @n Reference to the description https://developer.apple.com/library/mac/technotes/tn2162/_index.html
* @hideinitializer */
#define FOURCC_V216 MAKEFOURCC('V', '2', '1', '6')
#endif

#ifndef FOURCC_AR10
/**@brief Packed RGB, 10 bits per component with 2 bit alpha channel, 32 bpp
* @hideinitializer */
#define FOURCC_AR10 MAKEFOURCC('A', 'R', '1', '0')
#endif

#ifndef FOURCC_AB10
/**@brief Packed BGR, 10 bits per component with 2 bit alpha channel, 32 bpp
* @hideinitializer */
#define FOURCC_AB10 MAKEFOURCC('A', 'B', '1', '0')
#endif

#ifndef FOURCC_R24P
/**@brief Planar RGB, planes order R->G->B, 8 bits per component, 24 bpp
* @hideinitializer */
#define FOURCC_R24P MAKEFOURCC('R', '2', '4', 'P')
#endif

#ifndef FOURCC_R30P
/**@brief Planar RGB, planes order R->G->B, 10 bits per component, 30 bpp
* @hideinitializer */
#define FOURCC_R30P MAKEFOURCC('R', '3', '0', 'P')
#endif

#ifndef FOURCC_R36P
/**@brief Planar RGB, planes order R->G->B, 12 bits per component, 36 bpp
* @hideinitializer */
#define FOURCC_R36P MAKEFOURCC('R', '3', '6', 'P')
#endif

#ifndef FOURCC_R42P
/**@brief Planar RGB, planes order R->G->B, 14 bits per component, 42 bpp
* @hideinitializer */
#define FOURCC_R42P MAKEFOURCC('R', '4', '2', 'P')
#endif

#ifndef FOURCC_R48P
/**@brief Planar RGB, planes order R->G->B, 16 bits per component, 48 bpp
* @hideinitializer */
#define FOURCC_R48P MAKEFOURCC('R', '4', '8', 'P')
#endif

#ifndef FOURCC_b64a
/**@brief Packed BGRA 16-bit, little endian, 64 bpp
* @hideinitializer */
#define FOURCC_b64a MAKEFOURCC('b', '6', '4', 'a')
#endif

/** @} */

#endif // #if !defined (__MC_FOURCC_INCLUDED__)

