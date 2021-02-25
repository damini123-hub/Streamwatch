/* ----------------------------------------------------------------------------
 * File: trans_video_imagescaler.h
 *
 * Desc: Image Scaler API
 *
 * Copyright (c) 2015 MainConcept GmbH or its affiliates.  All rights reserved.
 *
 * MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 * This software is protected by copyright law and international treaties.  Unauthorized
 * reproduction or distribution of any portion is prohibited by law.
 * ----------------------------------------------------------------------------
 */

#if !defined (__IMAGE_SCALER_API_INCLUDED__)
#define __IMAGE_SCALER_API_INCLUDED__

#include "mctypes.h"
#include "mcapiext.h"

//------------------------------------------------------------------------------

#define MAKE_PIXEL_LAYOUT(a,b,c,d)  ( (((uint32_t)(uint8_t)a)<<0)   | \
                                      (((uint32_t)(uint8_t)b)<<8)   | \
                                      (((uint32_t)(uint8_t)c)<<16)  | \
                                      (((uint32_t)(uint8_t)d)<<24)  )

#define PIXEL_LAYOUT_RGB3         MAKE_PIXEL_LAYOUT('R','G','B','3')
#define PIXEL_LAYOUT_BGR3         MAKE_PIXEL_LAYOUT('B','G','R','3')
#define PIXEL_LAYOUT_R565         MAKE_PIXEL_LAYOUT('R','5','6','5')
#define PIXEL_LAYOUT_R555         MAKE_PIXEL_LAYOUT('R','5','5','5')
#define PIXEL_LAYOUT_BGRA         MAKE_PIXEL_LAYOUT('B','G','R','A')
#define PIXEL_LAYOUT_ARGB         MAKE_PIXEL_LAYOUT('A','R','G','B')
#define PIXEL_LAYOUT_LUT8         MAKE_PIXEL_LAYOUT('L','U','T','8')
#define PIXEL_LAYOUT_GRAY         MAKE_PIXEL_LAYOUT('G','R','A','Y')
#define PIXEL_LAYOUT_YUYV         MAKE_PIXEL_LAYOUT('Y','U','Y','V')
#define PIXEL_LAYOUT_UYVY         MAKE_PIXEL_LAYOUT('U','Y','V','Y')
#define PIXEL_LAYOUT_VUYA         MAKE_PIXEL_LAYOUT('V','U','Y','A')
#define PIXEL_LAYOUT_YV12         MAKE_PIXEL_LAYOUT('Y','V','1','2')
#define PIXEL_LAYOUT_I420         MAKE_PIXEL_LAYOUT('I','4','2','0')
#define PIXEL_LAYOUT_EXT1         MAKE_PIXEL_LAYOUT('E','X','T','1')


//------------------------------------------------------------------------------

/* mask for  interpolation modes*/
#define SCALER_MODE_INTERPOLATE   0x0000000F

/* simplest mode, neighbour */
#define SCALER_MODE_NEIGHBOUR     0x00000000
/* easy bilinear only */
#define SCALER_MODE_BILINEAR      0x00000002
/* FIR-8 polyphase filter */
#define SCALER_MODE_POLYPHASE8    0x00000003

/* currently FIR-8 polyphase filter */
#define SCALER_MODE_BEST          0x00000001
/* SCALER_MODE_BEST is used for best implemented scaler-mode */

/* scale fields independent */
#define SCALER_MODE_INTERLACED    0x00000100

/* how to handle ends in src_rect parameter*/
#define SCALER_MODE_HOR_PIN_BEG   0x00001000
#define SCALER_MODE_HOR_PIN_END   0x00002000
#define SCALER_MODE_VERT_PIN_BEG  0x00004000
#define SCALER_MODE_VERT_PIN_END  0x00008000


/* how the input-output  YUV-colorspace is defined*/
#define SCALER_MODE_INP_ITU601    0x01000000
#define SCALER_MODE_INP_ITU709    0x02000000

#define SCALER_MODE_OUT_ITU601    0x10000000
#define SCALER_MODE_OUT_ITU709    0x20000000


/* bicubic interpolation */
#define SCALER_MODE_BICUBIC       0x00100000
#define SCALER_MODE_B_SPLINE      0x00000000
#define SCALER_MODE_NOTCH         0x00200000
#define SCALER_MODE_CATMULL_ROM   0x00400000
#define SCALER_MODE_MITCHELL      0x00600000


typedef struct
{
  int32_t x0,y0,x1,y1;
} rect_tt;



typedef struct
{
  uint32_t  version; // 0 = initial

  uint32_t  four_cc;

  uint32_t  width;
  uint32_t  height;
  int32_t   stride[4];
  uint8_t*  plane[4];
} ext1_tt;


#define SCALER_OPT_BLEND_OVER     0x00000100 //internal blend source over destination
#define SCALER_OPT_BLEND_UNDER    0x00000300 //internal blend destination over source
#define SCALER_OPT_BLEND          0x0000FF00 //external blending

#define SCALER_OPT_EXT_PRECISION  0x00000001
//Five bits in „opt_Flags“ parameter (from the 2nd to the 6th) is used to set the number of threads.
//If the bits is set 0 then number of threads is equal to the core count. The number of threads can't be more than 16.
#define SCALER_OPT_SINGLE_THREAD  0x00000002
#define SCALER_OPT_INP_RECT       SCALER_OPT_EXT_PRECISION
#define SCALER_OPT_OUT_RECT       0x00010000

#define SUB_PEL_BITS (8)
/*
{
 x0 = (SrcBeginX<<SUB_PEL_BITS); y0 = (SrcBeginY<<SUB_PEL_BITS);
 x1 = (SrcEndX<<SUB_PEL_BITS);   y1 = (SrcEndY<<SUB_PEL_BITS);
}
*/


// NOTE: The old convenience functions SRScaler_FourCC_BGRA() and SRScaler_BGRA_FourCC() have been removed.
// Since Codec SDK 8.0 they are not supported anymore due to redundancy, at the same time when the
// old scaler/boosterengine was replaced by trans_video_imagescaler.
//
// Use the general SRScaler() API function instead, the parameter replacement is straight-forward:
//   - SRScaler_FourCC_BGRA( optFlags, FourCC, SrcPtr, SrcRealLineSize, SrcWidth, SrcHeight, DstPtr, DstRealLineSize, DstWidth, DstHeight, ColorTable, mode_flags )
//     becomes
//     SRScaler( optFlags, FourCC, SrcPtr, SrcRealLineSize, SrcWidth, SrcHeight, PIXEL_LAYOUT_BGRA, DstPtr, DstRealLineSize, DstWidth, DstHeight, ColorTable, mode_flags )
//   - SRScaler_BGRA_FourCC( optFlags, FourCC, SrcPtr, SrcRealLineSize, SrcWidth, SrcHeight, DstPtr, DstRealLineSize, DstWidth, DstHeight, ColorTable, mode_flags )
//     becomes
//     SRScaler( optFlags, PIXEL_LAYOUT_BGRA, SrcPtr, SrcRealLineSize, SrcWidth, SrcHeight, FourCC, DstPtr, DstRealLineSize, DstWidth, DstHeight, ColorTable, mode_flags )
//   - If the passed FourCC happens to be PIXEL_LAYOUT_RGB3, use PIXEL_LAYOUT_BGR3 instead in both cases.
//

#ifdef __cplusplus
extern "C" {
#endif

int MC_EXPORT_API SRScaler(             uint32_t  optFlags,
                          uint32_t  SrcLayout,    uint8_t*  SrcPtr,     int32_t SrcRealLineSize,
                          uint32_t  SrcWidth,     uint32_t  SrcHeight,
                          uint32_t  DstLayout,    uint8_t*  DstPtr,     int32_t DstRealLineSize,
                          uint32_t  DstWidth,     uint32_t  DstHeight,
                          uint8_t*  ColorTable,   uint32_t  mode_flags
 #ifdef __cplusplus
                          ,rect_tt* SrcRect=0, void* reserved=0
 #else
                          ,rect_tt* SrcRect, void* reserved
 #endif
            );


APIEXTFUNC MC_EXPORT_API SRScaler_GetAPIExt(uint32_t func);

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------

#endif // #if !defined (__IMAGE_SCALER_API_INCLUDED__)

