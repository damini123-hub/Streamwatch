/* ----------------------------------------------------------------------------
* File: sample_net_ref_counter.h
*
* Desc: Sample reference counter declaration
*
* Copyright (c) 2007-2011, MainConcept GmbH. All rights reserved.
*
* This software is the confidential and proprietary information of
* MainConcept GmbH and may be used only in accordance with the terms of
* your license from MainConcept GmbH.
* ----------------------------------------------------------------------------
*/
#if !defined (__MC_SAMPLE_NET_REF_COUNTER_H__)
#define __MC_SAMPLE_NET_REF_COUNTER_H__

#include "mctypes.h"
#include "net_common.h"

#define DECLARE_INETREFCOUNTED		\
    virtual int32_t AddRef() {				\
    return BaseAddRef();		\
    };								\
    virtual int32_t Release() {				\
    return BaseRelease();		\
    };

class CRefCounter : public MainConcept::NetworkStreaming::INetRefCounted 
{
public:
    CRefCounter();
    virtual ~CRefCounter();

protected:
    int32_t BaseAddRef();
    int32_t BaseRelease();
    
protected:
    int32_t m_iRefCount;
};

#define SAFE_RELEASE(_p) {if((_p) != NULL) { (_p)->Release(); (_p) = NULL; }} 

#endif //__MC_SAMPLE_NET_REF_COUNTER_H__