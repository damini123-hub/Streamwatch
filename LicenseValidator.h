#pragma once

#include "CustomCryptoLicense.h"
#define MAX_KEY_LENGTH 500
// LicenseValidator

class CLicenseValidator : public CWnd
{
	DECLARE_DYNAMIC(CLicenseValidator)

public:
	CLicenseValidator();
	int IsLicenseValid();
	//TCHAR m_AppPath[MAX_PATH];	
	virtual ~CLicenseValidator();
	bool m_bAppStartLicenseCheck;


	UINT m_nMax_LicensedStream;
	//bool m_bIP;
	//bool m_bSDI;
	//bool m_bASI;
	//bool m_bHD;
	//bool m_bH264;
	//bool m_bMPEG;
	//UINT m_nMaxInstance;
	//bool m_bSCTE35;
	//bool m_bEAS;
	//bool m_bAnalog_Cue_Tone;
	//bool m_bWeb_Cue_Tone;
	//bool m_bPlayout;

	//bool m_bForDeveloper;
	CCustomCryptoLicense AppEvalLicense;
	
protected:
	DECLARE_MESSAGE_MAP()
};


