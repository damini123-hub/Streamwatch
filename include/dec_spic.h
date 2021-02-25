/**
 @file  dec_spic.h
 @brief MPEG Subpicture Decoder API

 @verbatim
 File: dec_spic.h

 Desc: MPEG Subpicture Decoder API

 Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

 MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
 This software is protected by copyright law and international treaties.  Unauthorized
 reproduction or distribution of any portion is prohibited by law.
 @endverbatim
 **/

#ifndef _DEC_SPIC_H_
#define _DEC_SPIC_H_

#include "bufstrm.h"
#include "mcdefs.h"


#ifdef __cplusplus
extern "C" {
#endif

bufstream_tt* MC_EXPORT_API open_MPEGin_Subpic_stream(void *(MC_EXPORT_API *get_rc)(const char* name), int palFlag, long reserved1);

#ifdef __cplusplus
}
#endif

#endif // _DEC_SPIC_H_
