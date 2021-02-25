/* ----------------------------------------------------------------------------
 * File: config_gui_mp2m.h
 *
 * Desc: MPEG-1/2 Muxer GUI
 *
 * Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.
 *
 * MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.  
 * This software is protected by copyright law and international treaties.  Unauthorized 
 * reproduction or distribution of any portion is prohibited by law.
 * ----------------------------------------------------------------------------
 */

#if !defined (__MP2MUXER_PAGES_INCLUDED__)
#define __MP2MUXER_PAGES_INCLUDED__

#include "config_mp2m.h"


struct mp2m_dialog_set
{
    uint32_t options;
    void *parent_ptr; // HWND handle or pointer to QApplication
    char *title;
    char *banner_filename;

    int32_t reserved_int[400 - 1];
    void *reserved_ptr[112 - 3];
};


#ifdef __cplusplus
extern "C" {
#endif

/** 
 * Displays settings dialog.
 * @param set current settings being displayed in dialog.
 * @param options any combination of above flags.
 * @param option_ptr should be a HWND handle for MFC applications or pointer to QApplication if using MP2M_QT_APPLICATION option.
 * @return MCAPI_ACCEPTED if accepted, else MCAPI_REJECTED.
 */
int32_t showMP2MuxerPages(struct mp2m_param_set *set,
                          int32_t                options,
                          void                  *option_ptr);

#ifdef __cplusplus
    }
#endif

#endif // __MP2MUXER_PAGES_INCLUDED__
