/* ----------------------------------------------------------------------------
 * File: config_mp4v.h
 *
 * Desc: MPEG-4 part 2 Video Encoder Configuration API
 * Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.
 *
 * MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 * This software is protected by copyright law and international treaties.  Unauthorized
 * reproduction or distribution of any portion is prohibited by law.
 * ----------------------------------------------------------------------------
 */

#if !defined (__MP4V_CONFIG_API_INCLUDED__)
#define __MP4V_CONFIG_API_INCLUDED__

#include "mcapiext.h"

#include "enc_mp4v.h"
#include "enc_mp4v_def.h"

// dialog pages available
#define MP4V_PAGES_BASIC                0x00000001  // display the basic settings page
#define MP4V_PAGES_ADVANCED             0x00000002  // display the advanced settings page
#define MP4V_PAGES_MISC                 0x00000004  // display the miscellaneous page
#define MP4V_PAGES_ALL                  0x00000007  // display all of the above pages

// dialog options
#define MP4V_PAGES_LOADSAVE             0x00000010  // display the load/save buttons
#define MP4V_PAGES_NOINITCHECK          0x00000020  // do not check the validity of the setting at function start
#define MP4V_PAGES_REDWARNINGS          0x00000040  // display any validation warnings & errors

#define MP4V_PAGES_BANNER_OFF           0x00001000  // do not display codec banner
#define MP4V_PAGES_DISABLE_PRESET       0x00002000  // disable preset gui (used e.g. in Encoder Application)
#define MP4V_PAGES_DISABLE_FRAMESIZE    0x00004000  // disable framesize gui (used e.g. in Encoder Application)
#define MP4V_PAGES_DISABLE_FRAMERATE    0x00008000  // disable framerate gui (used e.g. in Encoder Application)
#define MP4V_PAGES_DISABLE_PIXELASPECT  0x00010000  // disable pixel aspect ratio gui

#define MP4V_PAGES_QT_APPLICATION       0x10000000  // caller application is QT (not MFC)
#define MP4V_PAGES_USE_DLG_SET_STRUCT   0x20000000  /**< pass @ref mp4v_dialog_set via @ref opt_ptr parameter in @ref showMP4VEncoderPages function */

struct mp4v_param_set
{
  struct m4_v_settings params;
  int32_t reserved[1024];
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The API Extension control.
 * @param func one of the defined API function identifiers.
 * @return pointer to the requested API function or NULL, if not supported.
 */
APIEXTFUNC MC_EXPORT_API MP4VConfigGetAPIExt(uint32_t func);

#ifdef __cplusplus
}
#endif

#endif // __MP4V_CONFIG_API_INCLUDED__
