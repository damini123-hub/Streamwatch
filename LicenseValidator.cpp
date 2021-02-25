// LicenseValidator.cpp : implementation file
//

#include "stdafx.h"
#include "StreamWatch.h"
#include "LicenseValidator.h"
#include "CustomCryptoLicense.h"

#define APPLICATION_NAME _T("StreamWatch")
#define VALIDATION_KEY  _T("AQABAANZTW+41UbO4kzmYVGZh8ao8UD5S7sm307BsN/wfbFXzLjIeK5o+hpYboXmHAMj2ZMAMADAAA==")

// LicenseValidator

IMPLEMENT_DYNAMIC(CLicenseValidator, CWnd)

CLicenseValidator::CLicenseValidator()
{
	m_bAppStartLicenseCheck = false;
	m_nMax_LicensedStream = 0;
}

CLicenseValidator::~CLicenseValidator()
{
}

BEGIN_MESSAGE_MAP(CLicenseValidator, CWnd)
END_MESSAGE_MAP()

int CLicenseValidator::IsLicenseValid()
{
	int ValidStatus = 2;		//2 = .ini file is not present
								//1 = key is expired
								//0 = valid license
		
//	TCHAR strAppDataPath[MAX_PATH];
	TCHAR strINI_Path[MAX_PATH];

// for geting the current location of project
	TCHAR tempPath[MAX_PATH];
	memset(tempPath, 0, MAX_PATH);
	::GetModuleFileName(0, tempPath, MAX_PATH);
	unsigned int loc = (unsigned int)(_tcsrchr(tempPath, _T('\\')) - tempPath);
	tempPath[loc] = NULL;
		//end
	memset(strINI_Path, 0, sizeof(strINI_Path));
	_stprintf_s(strINI_Path,MAX_PATH, _T("%s\\EvaluationLicense.ini"), tempPath);
	FILE *fpLicenseFile = NULL;
	_tfopen_s(&fpLicenseFile,strINI_Path, _T("r"));
	
	if(fpLicenseFile != NULL)
	{
		TCHAR strLicenseKey[MAX_KEY_LENGTH];
		memset(strLicenseKey, 0, sizeof(TCHAR) * MAX_KEY_LENGTH);
		
		_ftscanf_s(fpLicenseFile, _T("%s"), strLicenseKey, _countof(strLicenseKey));
		fclose(fpLicenseFile);

		ValidStatus = 1;
		for(int i = 0; i < 6; i++)
		{
			AppEvalLicense.SetValidationKey(VALIDATION_KEY);							
			AppEvalLicense.SetLicenseCode(strLicenseKey);
			AppEvalLicense.nCurrentAdaptor = i;
			if(AppEvalLicense.GetStatus() == LS_Valid)
			{
				ValidStatus = 0;
				if(m_bAppStartLicenseCheck == true)
				{
					const int SIZE = 200;
					TCHAR msg[SIZE];
					memset(msg, 0, sizeof(msg));

					int EvalDaysRemaining = AppEvalLicense.GetMaxUsageDays() - AppEvalLicense.GetCurrentUsageDays();
					if(EvalDaysRemaining > 0 && EvalDaysRemaining <= 60)
						_stprintf_s(msg,SIZE, _T("%d days of Professional Period Remaining !"), EvalDaysRemaining + 1);
					else if(EvalDaysRemaining <= 0)
						_stprintf_s(msg,SIZE, _T("Today is the last day of Professional Period !"));
					
					//int x = MessageBox(msg, "Ad Serter", MB_ICONINFORMATION | MB_OK  | MB_TOPMOST | MB_TOPMOST);
					if(0 != _tcscmp(msg, _T("")))
					{
						int bMsg = AfxMessageBox(msg, MB_OKCANCEL|MB_ICONINFORMATION);

						if(bMsg == IDOK)
							ValidStatus = 0;
						else
							ValidStatus = 3;
					}
				}
				// Feature Enable-Disable
			
				BOOL bIsUserData = AppEvalLicense.GetHasUserData();
				if(bIsUserData)
				{
					CString strUserData = AppEvalLicense.GetUserData();

					int nCount = strUserData.Replace(_T("#"), _T("~"));

					int nStart = 0;
					int nEnd = 0;
					CString strVal = _T("");
					for(int i = 0; i <= nCount; i++)
					{
						nEnd = strUserData.Find(_T('~'), nStart);
						if(i == nCount)
							strVal = strUserData.Mid(nStart);
						else
							strVal = strUserData.Mid(nStart, nEnd - nStart);
						nStart = nEnd + 1;

						int nPos = -1;
						if(-1 < (nPos = strVal.Find(_T("Max_LicensedStream="))))
						{
							int nLen = _tcslen(_T("Max_LicensedStream="));
							m_nMax_LicensedStream = _ttoi(strVal.Mid(nPos + nLen));
						}
					}
				}
				break;
			}
		}
	}
	return ValidStatus;
		//return 0;
}
