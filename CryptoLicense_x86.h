// License.h: interface for the CLicense class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LICENSE_H__02D560F1_10AB_4049_A422_D3DBB64F13F2__INCLUDED_)
#define AFX_LICENSE_H__02D560F1_10AB_4049_A422_D3DBB64F13F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#ifndef CRYPTOLICENSING_SOURCE_PROJECT

#if _MSC_VER < 1300
#define VCVERSION_SUFFIX "_vc60"
#endif
#if _MSC_VER == 1300
#define VCVERSION_SUFFIX "_vc70"
#endif
#if _MSC_VER == 1310
#define VCVERSION_SUFFIX "_vc71"
#endif
#if _MSC_VER == 1400
#define VCVERSION_SUFFIX "_vc80"
#endif
#if _MSC_VER == 1500
#define VCVERSION_SUFFIX "_vc90"
#endif
#if _MSC_VER == 1600
#define VCVERSION_SUFFIX "_vc2010"
#endif

#ifndef VCVERSION_SUFFIX
#error "Error : This version of Visual Studio is not supported"
#endif

#ifdef _AFXDLL
#ifdef _UNICODE
#ifdef _DEBUG
#define EZNSE_LIBNAME "cryptolicensing_dud"
#else
#define EZNSE_LIBNAME "cryptolicensing_rud"
#endif
#else
#ifdef _DEBUG
#define EZNSE_LIBNAME "cryptolicensing_dad"
#else
#define EZNSE_LIBNAME "cryptolicensing_rad"
#endif
#endif
#else
#ifdef _UNICODE
#ifdef _DEBUG
#define EZNSE_LIBNAME "cryptolicensing_dus"
#else
#define EZNSE_LIBNAME "cryptolicensing_rus"
#endif
#else
#ifdef _DEBUG
#define EZNSE_LIBNAME "cryptolicensing_das"
#else
#define EZNSE_LIBNAME "cryptolicensing_ras"
#endif
#endif
#endif

#ifdef _WIN64
#define PLATFORM_SUFFIX "_x64"
#else
#define PLATFORM_SUFFIX ""
#endif

#ifdef UNDER_CE
#define CE_SUFFIX "_ce"
#else
#define CE_SUFFIX ""
#endif


#pragma message("Linking with library file " EZNSE_LIBNAME VCVERSION_SUFFIX PLATFORM_SUFFIX CE_SUFFIX ".lib")
#pragma comment(lib, EZNSE_LIBNAME VCVERSION_SUFFIX PLATFORM_SUFFIX CE_SUFFIX ".lib")

#endif



class CBuffer;
class CEvaluationInfo;



enum LicenseStorageMode
{
	LS_None=0,
	LS_File,
	LS_Registry,
};

enum LicenseFeatures
{
	LF_DoesNotMatter = 0,
	LF_Feature1 = 1,
	LF_Feature2 = LF_Feature1 << 1,
	LF_Feature3 = LF_Feature2 << 1,
	LF_Feature4 = LF_Feature3 << 1,
	LF_Feature5 = LF_Feature4 << 1,
	LF_Feature6 = LF_Feature5 << 1,
	LF_Feature7 = LF_Feature6 << 1,
	LF_Feature8 = LF_Feature7 << 1,
	LF_Feature9 = LF_Feature8 << 1,
	LF_Feature10 = LF_Feature9 << 1,
	LF_Feature11 = LF_Feature10 << 1,
	LF_Feature12 = LF_Feature11 << 1,
	LF_Feature13 = LF_Feature12 << 1,
	LF_Feature14 = LF_Feature13 << 1,
	LF_Feature15 = LF_Feature14 << 1,
	LF_Feature16 = LF_Feature15 << 1,
	LF_Feature17 = LF_Feature16 << 1,
	LF_Feature18 = LF_Feature17 << 1,
	LF_Feature19 = LF_Feature18 << 1,
	LF_Feature20 = LF_Feature19 << 1,
	LF_Feature21 = LF_Feature20 << 1,
	LF_Feature22 = LF_Feature21 << 1,
	LF_Feature23 = LF_Feature22 << 1,
	LF_Feature24 = LF_Feature23 << 1,
	LF_Feature25 = LF_Feature24 << 1,
	LF_Feature26 = LF_Feature25 << 1,
	LF_Feature27 = LF_Feature26 << 1,
	LF_Feature28 = LF_Feature27 << 1,
	LF_Feature29 = LF_Feature28 << 1,
	LF_Feature30 = LF_Feature29 << 1,
	LF_Feature31 = LF_Feature30 << 1,
};

enum LicenseStatus
{
	LS_Valid = 0,
	LS_NotValidated = 1,
	LS_SerialCodeInvalid = LS_NotValidated << 1,
	LS_SignatureInvalid = LS_SerialCodeInvalid << 1,
	LS_MachineCodeInvalid = LS_SignatureInvalid << 1,
	LS_Expired = LS_MachineCodeInvalid << 1,
	LS_UsageModeInvalid = LS_Expired << 1,
	LS_ActivationFailed = LS_UsageModeInvalid << 1,
	LS_UsageDaysExceeded = LS_ActivationFailed << 1,
	LS_UniqueUsageDaysExceeded = LS_UsageDaysExceeded << 1,
	LS_ExecutionsExceeded = LS_UniqueUsageDaysExceeded << 1,
	LS_EvaluationTampered = LS_ExecutionsExceeded << 1,
	LS_GenericFailure = LS_EvaluationTampered << 1,
	LS_InstancesExceeded = LS_GenericFailure << 1,
	LS_RunTimeExceeded = LS_InstancesExceeded << 1,
	LS_CumulativeRunTimeExceeded = LS_RunTimeExceeded << 1,
	LS_ServiceNotificationFailed = LS_CumulativeRunTimeExceeded << 1,
	LS_Deactivated = LS_ServiceNotificationFailed << 1,
	LS_DebuggerDetected = LS_Deactivated << 1,
    LS_DateRollbackDetected = LS_DebuggerDetected << 1,
    LS_LocalTimeInvalid = LS_DateRollbackDetected << 1,
		
	LS_InValid = (LS_NotValidated | LS_GenericFailure | LS_SignatureInvalid | LS_MachineCodeInvalid | LS_Expired | LS_UsageModeInvalid
	| LS_ActivationFailed | LS_UsageDaysExceeded | LS_UniqueUsageDaysExceeded | LS_ExecutionsExceeded
	| LS_EvaluationTampered | LS_InstancesExceeded | LS_RunTimeExceeded | LS_CumulativeRunTimeExceeded | LS_ServiceNotificationFailed
	| LS_Deactivated | LS_DebuggerDetected | LS_DateRollbackDetected | LS_LocalTimeInvalid),

	LS_EvaluationExpired = LS_UsageDaysExceeded | LS_UniqueUsageDaysExceeded | LS_ExecutionsExceeded
	| LS_RunTimeExceeded | LS_CumulativeRunTimeExceeded | LS_EvaluationTampered,
};

enum SerialValidationResult
{
	SVR_Success,
	SVR_Failed,
	SVR_NotASerial,
};


class CCryptoLicense  
{

public:
	CCryptoLicense();
	CCryptoLicense(LPCTSTR validationKey);
	CCryptoLicense(LPCTSTR licenseCode,LPCTSTR validationKey);
	CCryptoLicense(LicenseStorageMode storageMode,LPCTSTR validationKey);
	CCryptoLicense(LicenseStorageMode storageMode,LPCTSTR storagePath,LPCTSTR validationKey);
	~CCryptoLicense();

	LONGLONG GetID();

	CString GetLicenseServiceURL();
	void SetLicenseServiceURL(LPCTSTR value);

	CString GetLicenseServiceSettingsFilePath();
	void SetLicenseServiceSettingsFilePath(LPCTSTR value);

	CString GetValidationKey();
	void SetValidationKey(LPCTSTR value);

	CString GetLicenseCode();
	void SetLicenseCode(LPCTSTR value);

	short GetNumberOfUsers();
	BOOL GetHasNumberOfUsers();

	short GetMaxActivations();
	BOOL GetHasMaxActivations();

	short GetMaxUsageDays();
	BOOL GetHasMaxUsageDays();

	short GetCurrentUsageDays();
	void SetCurrentUsageDays(short value);

	short GetRemainingUsageDays();

	short GetMaxUniqueUsageDays();
	BOOL GetHasMaxUniqueUsageDays();

	short GetCurrentUniqueUsageDays();
	void SetCurrentUniqueUsageDays(short value);

	short GetRemainingUniqueUsageDays();

	short GetMaxExecutions();
	BOOL GetHasMaxExecutions();

	short GetCurrentExecutions();
	void SetCurrentExecutions(short value);

	short GetRemainingExecutions();

	short GetMaxInstances();
	BOOL GetHasMaxInstances();

	int GetMaxRuntime();
	BOOL GetHasMaxRuntime();

	int GetMaxCumulativeRuntime();
	BOOL GetHasMaxCumulativeRuntime();

	short GetCurrentCumulativeRuntime();
	void SetCurrentCumulativeRuntime(short value);

	short GetRemainingCumulativeRuntime();

	BOOL GetEnableTamperChecking();

	BOOL GetEnableAntiDebuggerProtection();

	BOOL GetValidAtDesignTime();
	BOOL GetValidAtRunTime();
	BOOL GetHasSeparateRuntimeLicense();

	BOOL GetActivationsAreFloating();

	BOOL GetNotifyServiceOnValidation();

	LicenseFeatures GetFeatures();
	BOOL GetHasFeatures();

	FILETIME GetDateGenerated();

	FILETIME GetDateExpires();
	BOOL GetHasDateExpires();

	FILETIME GetDateLastUsed();

	LPWSTR GetUserData();
	BOOL GetHasUserData();

	BYTE* GetMachineCode();
	BOOL GetHasMachineCode();
	CString GetMachineCodeAsString();

	virtual BYTE* GetLocalMachineCode(int& machineCodeLength);
	CString GetLocalMachineCodeAsString();

	virtual BOOL IsMachineCodeValid();

	LicenseStatus GetStatus();

	void CopySettingsFrom(CCryptoLicense* src);
	
	void Dispose();

	BOOL IsFeaturePresent(LicenseFeatures feature,BOOL considerHigherFeatures);
	BOOL IsEvaluationLicense();
	BOOL IsEvaluationExpired();

	CString QueryWebService(CString methodName,CString paramNames[],CString paramValues[],int length);

	virtual BOOL Load();
	virtual BOOL Save();
	virtual BOOL Remove();

	LicenseStorageMode GetStorageMode();
	void SetStorageMode(LicenseStorageMode value);

	CString GetFileStoragePath();
	void SetFileStoragePath(LPCTSTR value);

	CString GetRegistryStoragePath();
	void SetRegistryStoragePath(LPCTSTR value);

	CString EncryptString(LPCTSTR str);
	CString DecryptString(LPCTSTR str);

	void ActivateDeregisterFloatingLicense();

	virtual void OnRuntimeExceeded();
	virtual void OnCumulativeRuntimeExceeded();

	BOOL GetLicenseFromProfile(CString profileName, CString userData);

	BOOL EncodeToBase64String(BYTE* data,int dataLength,CString& str);
	BYTE* DecodeFromBase64String(LPCTSTR str,int* dataLength) ;

	BOOL EncodeToBase32String(BYTE* data,int dataLength,CString& str);
	BYTE* DecodeFromBase32String(LPCTSTR str,int* dataLength) ;

	BOOL Base32StringToBase64String(LPCTSTR base32String,CString& base64String);
	BOOL Base64StringToBase32String(LPCTSTR base64String,CString& base32String);


	void ResetEvaluationInfo();
	BOOL IsEvaluationTampered();

	BOOL IsLicenseDisabledInService();
	CString Deactivate();
	CString DeactivateLocally();

	BOOL ValidateSignature();

	BOOL IsSerial(LPCTSTR str);
	SerialValidationResult GetLicenseFromSerial(LPCTSTR serial, CString userData);

	BOOL GetUseHashedMachineCode();
	void SetUseHashedMachineCode(BOOL value);

	BOOL IsFeaturePresentEx(int bitNum, BOOL considerHigherFeatures);

	BOOL GetDetectDateRollback();
	BOOL GetVerifyLocalTimeInService();

	void ResetStatus();

	CString PingLicenseService();
	BOOL EnableInLicenseService();
	BOOL DisableInLicenseService();

	virtual void SaveEvaluationInfo(BYTE* data,int dataLength);
	virtual BYTE* LoadEvaluationInfo(int& dataLength);

	int GetCurrentActivationCount();

	CTimeSpan GetFloatingLeasePeriod();
	BOOL GetHasFloatingLeasePeriod();




private:


	void symbol_1(CCryptoLicense* src);


	void symbol_2();
	BOOL symbol_3();
	BOOL symbol_4();
	void symbol_5();
	LicenseStatus symbol_6();

	CBuffer* symbol_7();
	void symbol_8(CBuffer* data);


	void symbol_9(CCryptoLicense* dst);


	void symbol_10();
	void symbol_11();
	void symbol_12();
	void symbol_13();
	void symbol_14(BOOL release);

	void symbol_15();
	void symbol_16();
	void symbol_17();

	BOOL symbol_18(HKEY& key, CString& value,CString regStoragePath,BOOL readwrite);
	CString symbol_19();
	CString symbol_20();
	BOOL symbol_21(CString& regStoragePath);

	BOOL symbol_22();
	BOOL symbol_23();
	BOOL symbol_24();
	void symbol_25();

	CString symbol_26();

	BOOL symbol_27();

	BOOL symbol_28();

	HKEY symbol_29();

	void* symbol_30;
	void* symbol_31;
	void* symbol_32;

	CString symbol_33;
	CBuffer* symbol_34;
	int symbol_35;

	CBuffer* symbol_36;
	int symbol_37;

	int symbol_38;
	short symbol_39;
	short symbol_40;
	short symbol_41;
	short symbol_42;
	short symbol_43;
	short symbol_44;
	short symbol_45;
	short symbol_46;
	int symbol_47;
	LONGLONG symbol_48;
	LONGLONG symbol_49;
	LPWSTR symbol_50;
	CBuffer* symbol_51;

	int symbol_52;
	CEvaluationInfo* symbol_53;

	int symbol_54;
	CString symbol_55;
	CString symbol_56;
	CString symbol_57;
	CString symbol_58;
	CString symbol_59;

	BOOL symbol_60;

	CString symbol_61;

	CBuffer* symbol_62;
	CBuffer* symbol_63;

	CTimeSpan symbol_64;

	BOOL symbol_65;


};

#endif // !defined(AFX_LICENSE_H__02D560F1_10AB_4049_A422_D3DBB64F13F2__INCLUDED_)
