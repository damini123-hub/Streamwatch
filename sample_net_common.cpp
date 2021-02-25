/* ----------------------------------------------------------------------------
* File: sample_net_common.cpp
*
* Desc: Common network samples implementations
*
* Copyright (c) 2007-2011, MainConcept GmbH. All rights reserved.
*
* This software is the confidential and proprietary information of
* MainConcept GmbH and may be used only in accordance with the terms of
* your license from MainConcept GmbH.
* ----------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "sample_net_common.h"

void SleepMilli( int32_t iMilliseconds )
{
	if ( iMilliseconds > 0 ) {
#if defined(__linux__) || defined(__APPLE__)
		usleep(iMilliseconds*1000);
#else
		Sleep(iMilliseconds);
#endif
	}
}

uint32_t GetMyPid()
{
#if defined(__linux__) || defined(__APPLE__)
	return (uint64_t)pthread_self()&0xFFFFFFFF;
#else
	return (uint32_t)GetCurrentThreadId();
#endif
}

int64_t GetCurrentTimeInMilliseconds()
{
	int64_t i64CurrentTime;

#if defined(__linux__) || defined(__APPLE__)
	timeval tv;
	gettimeofday(&tv, NULL);

	i64CurrentTime = (tv.tv_sec * 1000 + tv.tv_usec/1000);
#elif !defined (UNDER_CE)
	struct _timeb tstruct;
	_ftime_s( &tstruct );
	i64CurrentTime = tstruct.time * 1000 + tstruct.millitm;
#else
	i64CurrentTime = timeGetTime();
#endif

	return i64CurrentTime;
}
          
void ConvertMillisecondsToUpnpTime(int64_t iTimeInMilliseconds, int8_t *pOutTimeString)
{
    if ( !pOutTimeString ) {
        return;
    }

    int32_t iHours = (int32_t)(iTimeInMilliseconds / 3600000);
    int32_t iMinutes = (int32_t)((iTimeInMilliseconds - iHours*3600000) / 60000);
    int32_t iSeconds = (int32_t)((iTimeInMilliseconds - iHours*3600000 - iMinutes*60000) / 1000);
    int32_t iMilliSeconds = (int32_t)(iTimeInMilliseconds % 1000);

    sprintf((char*)pOutTimeString, "%d:%02d:%02d.0", iHours, iMinutes, iSeconds);

}

void LOG(const char *fmt, ...)
{
#if defined (DEBUG) || defined (_DEBUG)
#if defined(WIN32)
	char buf[2048];

	va_list vl;

	sprintf_s(buf,2048, FORMATI64": ", GetCurrentTimeInMilliseconds());

	va_start(vl,fmt);
	vsprintf_s(&buf[strlen(buf)],2048, fmt, vl);
	va_end(vl);

	OutputDebugStringA(buf);
#endif // defined(WIN32)
#endif
}

static char *pInputKeys = 0;
static uint32_t *pInputIntervals = 0;
bool ParseCmdQueue(char *arg)
{
	int count = 0;
	for (int i = 0; arg[i]; i++)
		if (arg[i] == ':') count++;
	if (count & 1) {
		pInputKeys = (char*)malloc(count+1);
		pInputIntervals = (uint32_t*)malloc(count*sizeof(uint32_t));
		int j = 0;
		for (char *pch = strtok(arg, ":"); pch; pch = strtok(NULL, ":")) {
			pInputIntervals[j] = atoi(pch);
			pch = strtok(NULL, ":");
			if (pch[0] == '%')
				pInputKeys[j] = (char)strtol(pch+1, NULL, 16);
			else
				pInputKeys[j] = pch[0];
			j++;
		}
		pInputKeys[j] = 0;
		return true;
	}
	return false;
}

static int8_t GetKeyFromCmdQueue( uint32_t uiMilliseconds )
{
	static int currIndex = 0;
	static uint32_t timeElapsed = 0;
	
	SleepMilli(uiMilliseconds);
	if (pInputKeys[currIndex]) {
		timeElapsed += uiMilliseconds;
		if (timeElapsed >= pInputIntervals[currIndex]) {
			timeElapsed = 0;
			return pInputKeys[currIndex++];
		}
	}
	return 0;
}

int8_t WaitForUserInput( uint32_t uiMilliseconds )
{
	if (pInputKeys)
		return GetKeyFromCmdQueue(uiMilliseconds);
#if defined(__linux__) || defined(__APPLE__)
	struct timeval tv;
	fd_set fds;

	tv.tv_sec = uiMilliseconds/1000;
	tv.tv_usec = (uiMilliseconds % 1000) * 1000;

	int fd = fileno(stdin);

	FD_ZERO(&fds);
	FD_SET(fd, &fds);

	int res = select(fd+1, &fds, NULL, NULL, uiMilliseconds != MCNET_THREAD_WAIT_INFINITE ? &tv : NULL);
	if ( res > 0 && FD_ISSET(fd, &fds) ) {
		return getchar();
	}
	return 0;
#else
    HANDLE stdIn = GetStdHandle(STD_INPUT_HANDLE);
     DWORD mode = 0;
    const bool isPipe = !GetConsoleMode(stdIn, &mode);
    const int64_t iStartLoop = GetCurrentTimeInMilliseconds();
    const int64_t iStopLoop = iStartLoop + uiMilliseconds;

    if (isPipe) {
        do {
            DWORD bytes_left = 0;
            PeekNamedPipe(stdIn, NULL, 0, NULL, &bytes_left, NULL);
            if(bytes_left > 0) {
                DWORD iRead = 0;
                char ch = 0;
                BOOL r = ReadFile(stdIn, &ch, 1, &iRead, NULL);
                if ( iRead == 1 ) {
                    return ch;
                }
            }
            Sleep(1);
        }while (iStopLoop > GetCurrentTimeInMilliseconds());
    } else {
         while ( iStopLoop > GetCurrentTimeInMilliseconds() &&
            WaitForSingleObject(stdIn, uiMilliseconds != MCNET_THREAD_WAIT_INFINITE ? uiMilliseconds : INFINITE) == WAIT_OBJECT_0 ) {

                DWORD cNumRead = 0;
                INPUT_RECORD irInBuf;

                if ( ReadConsoleInput( stdIn, &irInBuf, 1, &cNumRead) ) {

                    if ( cNumRead && irInBuf.EventType == KEY_EVENT && irInBuf.Event.KeyEvent.bKeyDown &&
                        irInBuf.Event.KeyEvent.uChar.AsciiChar != 0) {
                            return irInBuf.Event.KeyEvent.uChar.AsciiChar;
                    }
                }
        };
    }
    return 0;
#endif
}


#if defined(__APPLE__)
int wcsncasecmp(const wchar_t *str1, const wchar_t *str2, size_t MaxCount)
{
	for ( size_t index = 0; index < MaxCount; index++ ) {

		if ( towlower(str1[index]) > towlower(str2[index]) ) {
			return 1;
		} else if ( towlower(str1[index]) < towlower(str2[index]) ) {
			return -1;
		}
	}

	return 0;
}

int wcscasecmp(const wchar_t *str1, const wchar_t *str2)
{
	size_t strLen1 = wcslen(str1);
	size_t strLen2 = wcslen(str2);

	if ( strLen1 > strLen2 ) {
		return 1;
	} else if ( strLen1 < strLen2 ) {
		return -1;
	}

	return wcsncasecmp(str1, str2, strLen1);
}
#endif
