/* ----------------------------------------------------------------------------
* File: sample_net_common.h
*
* Desc: General network samples defines
*
* Copyright (c) 2007-2011, MainConcept GmbH. All rights reserved.
*
* This software is the confidential and proprietary information of
* MainConcept GmbH and may be used only in accordance with the terms of
* your license from MainConcept GmbH.
* ----------------------------------------------------------------------------
*/

#if !defined (__MC_SAMPLE_NET_COMMON_H__)
#define __MC_SAMPLE_NET_COMMON_H__

#if !defined(__linux__) && !defined(__APPLE__)

	// Modify the following defines if you have to target a platform prior to the ones specified below.
	// Refer to MSDN for the latest info on corresponding values for different platforms.
	#ifndef WINVER				// Allow use of features specific to Windows XP or later.
	#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
	#endif

	#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
	#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
	#endif						

	#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
	#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
	#endif

	#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
	#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
	#endif

	#include <ws2tcpip.h>
	#include <windows.h>
	#include <conio.h>
	#include <sys/timeb.h>
	#include <stdio.h>
	#include <sys/stat.h>
	#include <time.h>
    #include <math.h>
    #include <signal.h>

    #include <iostream>
    #include <errno.h>

	#define ATOI64 _atoi64
	#define FORMATI64	"%I64d"
	#define FORMATI64U	"%I64u"
#else
	#include <unistd.h>
	#include <stdlib.h>
	#include <string.h>
	#include <ctype.h>
	#include <dlfcn.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <stdio.h>
	#include <wchar.h>
	#include <termios.h>
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <pthread.h>
	#include <stdarg.h>
	#include <time.h>
	#include <sys/time.h>
    #include <limits.h>
    #include <iostream>
    #include <errno.h>
	#include <math.h>

	#define _stricmp strcasecmp
	#define _strnicmp strncasecmp
	#define _putch putchar
       	              
	#define wcsnicmp wcsncasecmp
	#define wcsicmp wcscasecmp

	#define _stat stat

	typedef int SOCKET;

	typedef void* HMODULE;

	#define LoadLibrary(x)	dlopen(x, RTLD_NOW);

	#define GetProcAddress	dlsym

	#define	ATOI64 atoll
	#define	FORMATI64	"%lld"
	#define	FORMATI64U	"%llu"

#if defined(__APPLE__)
	int wcsncasecmp(const wchar_t *str1, const wchar_t *str2, size_t MaxCount);
	int wcscasecmp(const wchar_t *str1, const wchar_t *str2);
#endif // #if defined(__APPLE__)

#endif

#include "stdafx.h"
#include "nutil_api.h"
#include "tools.h"
#include "net_threads.h"
#include <locale.h>
#include <vector>
#include <list>

#define SAFE_DELETE(_p) {if(_p != NULL) { delete _p; _p = NULL; }}
#define SAFE_DELETE_ARRAY(_p) {if(_p != NULL) { delete [] _p; _p = NULL; }}
#define SAFE_COPY_STRING(_dst, _src, _srclen) { SAFE_DELETE_ARRAY(_dst); _dst = new int8_t[_srclen + 1]; memset(_dst, 0, _srclen + 1); memcpy(_dst, _src, _srclen);}
#define SAFE_COPY_STRING_SHORT(_dst, _src) { SAFE_DELETE_ARRAY(_dst); _dst = new int8_t[strlen((const char*)_src) + 1]; strcpy( (char*)_dst, (const char*)_src);}
#define SAFE_FREE(_p) {if(_p != NULL) { free(_p); _p = NULL; }}

#define SAFE_CONVERT_STRING_WIDE_TO_MBS(_dst, _src) { size_t iwLen = wcslen(_src); size_t icLen = iwLen * sizeof(int8_t); _dst = new int8_t[icLen + 1]; wcstombs((char*)_dst, _src, (icLen + 1) * sizeof (wchar_t));};
#define SAFE_CONVERT_STRING_MBS_TO_WIDE(_dst, _src) { size_t icLen = strlen(_src); size_t iwLen = icLen * sizeof(wchar_t); _dst = new wchar_t[iwLen + 1]; mbstowcs(_dst, (char*)_src, iwLen + 1);};

#define WIDE(txt)	_WIDE(txt)
#define _WIDE(txt)	L##txt

#define TIMEOUT_DELIVERYWAIT 50

uint32_t GetMyPid();
int64_t GetCurrentTimeInMilliseconds();
void LOG(const char *fmt, ...);
bool ParseCmdQueue(char *arg);
int8_t WaitForUserInput( uint32_t uiMilliseconds );
void SleepMilli( int32_t iMilliseconds );
void ConvertMillisecondsToUpnpTime(int64_t iTimeInMilliseconds, int8_t *pOutTimeString);
#endif //__MC_SAMPLE_NET_COMMON_H__
