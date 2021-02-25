/* ----------------------------------------------------------------------------
* File: autolock.h
*
* Desc: Autolock class
*
* Copyright (c) 2007-2009, MainConcept GmbH. All rights reserved.
*
* This software is the confidential and proprietary information of
* MainConcept GmbH and may be used only in accordance with the terms of
* your license from MainConcept GmbH.
* ----------------------------------------------------------------------------
*/

#if !defined (__MC_SAMPLE_AUTOLOCK_H__)
#define __MC_SAMPLE_AUTOLOCK_H__

class CAutoLock
{
public:
    CAutoLock(MCNetCriticalSection p)  
    {
        m_CritSec = p;
        MCNetCriticalSectionEnter(m_CritSec);
    }

    ~CAutoLock() 
    {
        MCNetCriticalSectionLeave(m_CritSec);
    }

private:
    MCNetCriticalSection m_CritSec;
};


#endif //__MC_AUTOLOCK_H__
