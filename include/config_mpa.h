/* ----------------------------------------------------------------------------
 * File: config_mpa.h
 *
 * Desc: MPEG-1/2 Audio Encoder Configuration API
 *
 * Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.
 *
 * MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 * This software is protected by copyright law and international treaties.  Unauthorized
 * reproduction or distribution of any portion is prohibited by law.
 * ----------------------------------------------------------------------------
 */

#if !defined (__MPA_CONFIG_API_INCLUDED__)
#define __MPA_CONFIG_API_INCLUDED__

#include "mcapiext.h"

#include "enc_mpa.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The API Extension control.
 * @param func one of the following API function identifiers:
 *    MCAPI_LoadParamSet
 *    MCAPI_SaveParamSet
 *    MCAPI_ShowPages
 * @return pointer to the requested API function or NULL, if not supported.
 */
APIEXTFUNC MC_EXPORT_API MPAConfigGetAPIExt(uint32_t func);

#ifdef __cplusplus
}
#endif

/**
 * @struct mpa_param_set
 * This structure is used as parameter set for all available API functions.
 */
struct mpa_param_set
{
  struct mpeg_a_settings params;
  int32_t                audio_type;
  int32_t                audio_frequency;
  int32_t                reserved[1022];
};

/**
 * @name GUI Options
 * These options are supported by ShowPages API.
 * @{
 */
#define MPA_PAGES_BASIC               0x00000001  /**< display the basic settings page */
#define MPA_PAGES_ALL                 0x00000001  /**< display all of the above pages */

#define MPA_PAGES_LOADSAVE            0x00000010  /**< display the load/save buttons */
#define MPA_PAGES_NOINITCHECK         0x00000020  /**< do not check the validity of the setting at function start */
#define MPA_PAGES_REDWARNINGS         0x00000040  /**< display any validation warnings & errors */

#define MPA_PAGES_BANNER_OFF          0x00001000  /**< do not display codec banner */
#define MPA_PAGES_DISABLE_PRESET      0x00002000  /**< disable preset gui (used e.g. in Encoder Application) */

#define MPA_PAGES_QT_APPLICATION      0x10000000  /**< caller application is QT (not MFC) */
#define MPA_PAGES_USE_DLG_SET_STRUCT  0x20000000  /**< pass @ref mpa_dialog_set via @ref opt_ptr parameter in @ref showMPAEncoderPages function */
/** @} */

#endif // __MPA_CONFIG_API_INCLUDED__

