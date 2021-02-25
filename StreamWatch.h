// New_Version_Stream_Watch.h : main header file for the PROJECT_NAME application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "LicenseValidator.h"

// CStream_WatchApp:
// See New_Version_Stream_Watch.cpp for the implementation of this class
//

class CStream_WatchApp : public CWinApp
{
public:
	CLicenseValidator m_LicenseValidator;
	HANDLE m_hAppMutex;
	TCHAR m_strAppDataPath[MAX_PATH];

public:
	CStream_WatchApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CStream_WatchApp theApp;