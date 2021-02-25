/* ----------------------------------------------------------------------------
* File: sample_net_ref_counter.cpp
*
* Desc: Sample reference counter implementation
*
* Copyright (c) 2007-2011, MainConcept GmbH. All rights reserved.
*
* This software is the confidential and proprietary information of
* MainConcept GmbH and may be used only in accordance with the terms of
* your license from MainConcept GmbH.
* ----------------------------------------------------------------------------
*/

#include "sample_net_ref_counter.h"

CRefCounter::CRefCounter() : m_iRefCount(1) 
{

}

CRefCounter::~CRefCounter() 
{

}

int32_t CRefCounter::BaseAddRef() 
{
    m_iRefCount++;
    return m_iRefCount;
}

int32_t CRefCounter::BaseRelease() 
{
    m_iRefCount--;
    if ( m_iRefCount == 0 ) {

		// Protect from double deletion in case AddRef+Release called in destructor
        m_iRefCount++;

        delete this;
        return 0;
    }

    return m_iRefCount;
}