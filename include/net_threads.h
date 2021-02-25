/**
@file net_threads.h
@brief Thread management functions
@verbatim
  File: net_threads.h

  Desc: Thread management functions

  Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

  MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
  This software is protected by copyright law and international treaties.  Unauthorized
  reproduction or distribution of any portion is prohibited by law.
@endverbatim
 */

#if !defined(__NET_THREADS_H__)
#define __NET_THREADS_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _tagMCNetEvent *MCNetEvent;
typedef struct _tagMCNetSemaphore *MCNetSemaphore;
typedef struct _tagMCNetThread *MCNetThread;
typedef struct _tagMCNetCriticalSection *MCNetCriticalSection;

#define MCNetThreadFunc(fn,a) int fn(void* a)
#define MCNetThreadFuncArg(a) int (*a)(void*)

typedef int ( *MCNET_PTHREAD_START_ROUTINE)(void * a);

#if defined(__APPLE__) || defined(__linux__)

#include <pthread.h>
#include <errno.h>

#define MCNET_THREAD_WAIT_TIMEDOUT    ETIMEDOUT
#define MCNET_THREAD_WAIT_SIGNALED    EINTR
#define MCNET_THREAD_WAIT_INFINITE    -1

#else

#include <windows.h>
#include <winbase.h>

#define MCNET_THREAD_WAIT_TIMEDOUT    WAIT_TIMEOUT
#define MCNET_THREAD_WAIT_SIGNALED    WAIT_OBJECT_0
#define MCNET_THREAD_WAIT_INFINITE    INFINITE

#endif // #if defined(__APPLE__) || defined(__linux__)

// Event Handling
signed long MCNetEventCreate(MCNetEvent* e, int manualReset, int initialState, void *(MC_EXPORT_API *get_rc)(const char*));
void MCNetEventPost(MCNetEvent e);
int MCNetEventWait(MCNetEvent e, signed long timeout);
void MCNetEventReset(MCNetEvent e);
void MCNetEventDestroy(MCNetEvent e);

// Semaphore Handling
signed long MCNetSemaphoreCreate(MCNetSemaphore* s,long value,long max, void *(MC_EXPORT_API *get_rc)(const char*));
int MCNetSemaphoreWait(MCNetSemaphore s,signed long timeout);
void MCNetSemaphoreRelease(MCNetSemaphore s,long* prev_count);
void MCNetSemaphoreDestroy(MCNetSemaphore s);

// Thread handling
signed long MCNetThreadCreate(MCNetThread* t,MCNetThreadFuncArg(tf),void* ta, void *(MC_EXPORT_API *get_rc)(const char*));
signed long MCNetThreadWait(MCNetThread t,long timeout);
signed long MCNetThreadDestroy(MCNetThread t);
void MCNetThreadDetach(MCNetThread t);
// WARNING: Destructors will NOT be called when thread is terminated with this function
int MCNetThreadReturn();

// Critical section handling
signed long MCNetCriticalSectionCreate(MCNetCriticalSection* cs, void *(MC_EXPORT_API *get_rc)(const char*));
void MCNetCriticalSectionDestroy(MCNetCriticalSection cs);
void MCNetCriticalSectionEnter(MCNetCriticalSection cs);
void MCNetCriticalSectionLeave(MCNetCriticalSection cs);

#ifdef __cplusplus
}
#endif

#endif //__NET_THREADS_H__
