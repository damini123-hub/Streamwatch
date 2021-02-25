/* ----------------------------------------------------------------------------
* File: sample_net_common_ds.h
*
* Desc: DirectShow samples common definitions
*
* Copyright (c) 2007-2009, MainConcept GmbH. All rights reserved.
*
* This software is the confidential and proprietary information of
* MainConcept GmbH and may be used only in accordance with the terms of
* your license from MainConcept GmbH.
* ----------------------------------------------------------------------------
*/

#if !defined (__MC_SAMPLE_NET_COMMON_DS_H__)
#define __MC_SAMPLE_NET_COMMON_DS_H__

#define WIN32_LEAN_AND_MEAN

#include "sample_net_common.h"

#include <winsock2.h>
#include <iptypes.h>

#include <wtypes.h>
#include <atlbase.h>
#include <initguid.h>
#include <objbase.h>

#include <wmsdk.h>
#include <streams.h>

#include <ks.h>
#include <ksmedia.h>
#include <bdatypes.h>
#include <bdamedia.h>

#include <dvdmedia.h>

#include <tchar.h>

#include "ModuleConfig.h"
#include "common_mc.h"

#define SAFE_RELEASE(_p) {if((_p) != NULL) { (_p)->Release(); (_p) = NULL; }} 
                  
#endif //__MC_SAMPLE_NET_COMMON_DS_H__
