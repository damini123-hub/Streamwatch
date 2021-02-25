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
#if _MSC_VER == 1700
#define VCVERSION_SUFFIX "_vc2012"
#endif
#if _MSC_VER == 1800
#define VCVERSION_SUFFIX "_vc2013"
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
class CCustomInfo;
class CStringBuilder;

enum DefaultableBoolean
{

	DB_Default,
	DB_True,
	DB_False,
};


enum FloatingHeartBeatResult
{
	FHBR_Failed,
    FHBR_Succeeded,
};

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
	LS_SerialCodeInvalid = LS_NotValidated << 1, // 2
	LS_SignatureInvalid = LS_SerialCodeInvalid << 1, // 4
	LS_MachineCodeInvalid = LS_SignatureInvalid << 1, // 8
	LS_Expired = LS_MachineCodeInvalid << 1, // 16
	LS_UsageModeInvalid = LS_Expired << 1, // 32
	LS_ActivationFailed = LS_UsageModeInvalid << 1, // 64
	LS_UsageDaysExceeded = LS_ActivationFailed << 1, // 128
	LS_UniqueUsageDaysExceeded = LS_UsageDaysExceeded << 1, // 256
	LS_ExecutionsExceeded = LS_UniqueUsageDaysExceeded << 1, // 512
	LS_EvaluationTampered = LS_ExecutionsExceeded << 1, // 1024
	LS_GenericFailure = LS_EvaluationTampered << 1, // 2048
	LS_InstancesExceeded = LS_GenericFailure << 1, // 4096
	LS_RunTimeExceeded = LS_InstancesExceeded << 1, // 8192
	LS_CumulativeRunTimeExceeded = LS_RunTimeExceeded << 1, // 16384
	LS_ServiceNotificationFailed = LS_CumulativeRunTimeExceeded << 1, // 16384
	LS_Deactivated = LS_ServiceNotificationFailed << 1, // 65536
	LS_DebuggerDetected = LS_Deactivated << 1, // 131072
    LS_DateRollbackDetected = LS_DebuggerDetected << 1, // 262144
    LS_LocalTimeInvalid = LS_DateRollbackDetected << 1, // 524288
	LS_RemoteSessionDetected = LS_LocalTimeInvalid << 1, // 1048576
	LS_LicenseServerMachineCodeInvalid = LS_RemoteSessionDetected << 1,
	LS_EvaluationDataLoadSaveFailed = LS_LicenseServerMachineCodeInvalid << 1,
		
	LS_InValid = (LS_NotValidated | LS_GenericFailure | LS_SignatureInvalid | LS_MachineCodeInvalid | LS_Expired | LS_UsageModeInvalid
	| LS_ActivationFailed | LS_UsageDaysExceeded | LS_UniqueUsageDaysExceeded | LS_ExecutionsExceeded
	| LS_EvaluationTampered | LS_InstancesExceeded | LS_RunTimeExceeded | LS_CumulativeRunTimeExceeded | LS_ServiceNotificationFailed
	| LS_Deactivated | LS_DebuggerDetected | LS_DateRollbackDetected | LS_LocalTimeInvalid | LS_RemoteSessionDetected | LS_LicenseServerMachineCodeInvalid | LS_EvaluationDataLoadSaveFailed),

	LS_EvaluationExpired = LS_UsageDaysExceeded | LS_UniqueUsageDaysExceeded | LS_ExecutionsExceeded
	| LS_RunTimeExceeded | LS_CumulativeRunTimeExceeded | LS_EvaluationTampered,
};

enum SerialValidationResult
{
	SVR_Success,
	SVR_Failed,
	SVR_NotASerial,
};

class CFloatingHeartBeatEventArgs
{
public:

	CFloatingHeartBeatEventArgs();
	
	BOOL exceptionOccurred;
	FloatingHeartBeatResult result;

	FILETIME timeLastFailed;
	FILETIME timeLastSucceeded;

	
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

	BSTR GetLicenseServiceURL();
	void SetLicenseServiceURL(LPCTSTR value);

	BSTR GetLicenseServiceSettingsFilePath();
	void SetLicenseServiceSettingsFilePath(LPCTSTR value);

	BSTR GetValidationKey();
	void SetValidationKey(LPCTSTR value);

	BSTR GetLicenseCode();
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

#ifndef UNDER_CE
	int GetMaxRuntime();
	BOOL GetHasMaxRuntime();

	int GetMaxCumulativeRuntime();
	BOOL GetHasMaxCumulativeRuntime();

	short GetCurrentCumulativeRuntime();
	void SetCurrentCumulativeRuntime(short value);

	short GetRemainingCumulativeRuntime();
#endif

	BOOL GetEnableTamperChecking();

	BOOL GetEnableAntiDebuggerProtection();

// 	BOOL GetValidAtDesignTime();
// 	BOOL GetValidAtRunTime();
// 	BOOL GetHasSeparateRuntimeLicense();

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

	BSTR GetUserDataFieldValue(LPCTSTR fieldName, LPCTSTR separator);

	BYTE* GetMachineCode();
	BOOL GetHasMachineCode();
	BSTR GetMachineCodeAsString();

	virtual BYTE* GetLocalMachineCode(int& machineCodeLength);
	BSTR GetLocalMachineCodeAsString();

	virtual BOOL IsMachineCodeValid();

	LicenseStatus GetStatus();

	void CopySettingsFrom(CCryptoLicense* src);
	
	void Dispose();

	BOOL IsFeaturePresent(LicenseFeatures feature,BOOL considerHigherFeatures);
	BOOL IsEvaluationLicense();
	BOOL IsEvaluationExpired();

	BSTR QueryWebService(LPCTSTR methodName,LPCTSTR paramNames[],LPCTSTR paramValues[],int length);

	virtual BOOL Load();
	virtual BOOL Save();
	virtual BOOL Remove();

	LicenseStorageMode GetStorageMode();
	void SetStorageMode(LicenseStorageMode value);

	BSTR GetFileStoragePath();
	void SetFileStoragePath(LPCTSTR value);

	BSTR GetRegistryStoragePath();
	void SetRegistryStoragePath(LPCTSTR value);

	BSTR EncryptString(LPCTSTR str);
	BSTR DecryptString(LPCTSTR str);

	void ActivateDeregisterFloatingLicense();

	virtual void OnRuntimeExceeded();
	virtual void OnCumulativeRuntimeExceeded();

	BOOL GetLicenseFromProfile(LPCTSTR profileName, LPCTSTR userData);
	BSTR GetSerialFromProfile(LPCTSTR profileName);
	BOOL RenewLicense(LPCTSTR userData);
	BOOL UpdateLicense(LPCTSTR userData);

	BOOL EncodeToBase64String(BYTE* data,int dataLength,BSTR* str);
	BYTE* DecodeFromBase64String(LPCTSTR str,int* dataLength) ;

	BOOL EncodeToBase32String(BYTE* data,int dataLength,BSTR* str);
	BYTE* DecodeFromBase32String(LPCTSTR str,int* dataLength) ;

	BOOL Base32StringToBase64String(LPCTSTR base32String,BSTR* base64String);
	BOOL Base64StringToBase32String(LPCTSTR base64String,BSTR* base32String);


	void ResetEvaluationInfo();
	BOOL IsEvaluationTampered();

	BOOL IsLicenseDisabledInService();
	BSTR Deactivate();
	BSTR DeactivateLocally();

	BOOL ValidateSignature();

	BOOL IsSerial(LPCTSTR str);
	SerialValidationResult GetLicenseFromSerial(LPCTSTR serial, LPCTSTR userData);

	BOOL GetUseHashedMachineCode();
	void SetUseHashedMachineCode(BOOL value);

	BOOL IsFeaturePresentEx(int bitNum, BOOL considerHigherFeatures);

	BOOL GetDetectDateRollback();
	BOOL GetVerifyLocalTimeInService();

	void ResetStatus();

	BSTR PingLicenseService();
	BOOL EnableInLicenseService();
	BOOL DisableInLicenseService();

	virtual void SaveEvaluationInfo(BYTE* data,int dataLength);
	virtual BYTE* LoadEvaluationInfo(int& dataLength);

	int GetCurrentActivationCount();
	int GetRemainingActivationCount();

	CTimeSpan GetFloatingLeasePeriod();
	BOOL GetHasFloatingLeasePeriod();

	void RemoveCustomInfo(LPCTSTR key);
	BOOL TryGetCustomInfo(LPCTSTR key, BSTR* value);
	void SetCustomInfo(LPCTSTR key, LPCTSTR value);
	void ResetCustomInfo();

	virtual void OnFloatingHeartBeat(CFloatingHeartBeatEventArgs& e);
	int GetFloatingHeartBeatInterval();
	BOOL GetHasFloatingHeartBeatInterval();

	BOOL GetReleaseFloatingHeartbeatLicenseSlotOnDispose();
	void SetReleaseFloatingHeartbeatLicenseSlotOnDispose(BOOL value);

	BOOL IsCurrentMachineActivated();

	BOOL GetDisallowInRemoteSession();

	virtual void OnLeaseExpired();
	FILETIME GetLeaseExpires();
	BOOL GetHasLeaseExpires();

	//virtual BOOL IsDeactivationRecordPresent(BSTR recordID);
	//virtual BOOL CreateDeactivationRecord(BSTR recordID);
	//virtual BOOL DeleteDeactivationRecord(BSTR recordID);
	//BOOL DeleteDeactivationRecord();

	BOOL GetHasLicenseServerMachineCode();
	BYTE* GetLicenseServerMachineCode();
	BSTR GetLicenseServerMachineCodeAsString();

	BSTR GetLicenseStatusAsString();


	BSTR GetActivationContext();
	void SetActivationContext(LPCTSTR value);

	BSTR GetStatusExceptionMessage(int statusCode);
	BSTR GetAllStatusExceptionsAsString();

	BSTR GetRecordGUID(LPCTSTR recordKey);

#ifndef UNDER_CE
	short GetCurrentRuntime();
	short GetRemainingRuntime();
#endif



private:


	void symbol_1(LPCTSTR str);
	CString symbol_2(CException* ex);
	void symbol_3(int status,CException* ex);
	void symbol_4(int status,CString& str);

	CString symbol_5(int status) ;

	void symbol_6(CCryptoLicense* src);


	void symbol_7();
	BOOL symbol_8();
	BOOL symbol_9();
	void symbol_10();
	LicenseStatus symbol_11();

	CBuffer* symbol_12();
	void symbol_13(CBuffer* data);


	void symbol_14(CCryptoLicense* dst);


	void symbol_15();
	void symbol_16();
	void symbol_17();
	void symbol_18();
	void symbol_19(BOOL release);

	void symbol_20();
	void symbol_21();
	void symbol_22();

	BOOL symbol_23(HKEY& key, CString& value,CString regStoragePath,BOOL readwrite);
	CString symbol_24();
	CString symbol_25();
	BOOL symbol_26(CString& regStoragePath);

	BOOL symbol_27();
	BOOL symbol_28();
	void symbol_29();

	CString symbol_30();

	BOOL symbol_31();

	BOOL symbol_32();

	HKEY symbol_33();

	void symbol_34(BYTE* data,int dataLength);
	BYTE* symbol_35(int& dataLength);
	HKEY symbol_36();

	CString symbol_37(LPCTSTR str);

	BOOL symbol_38();

	void* symbol_39;
	void* symbol_40;
	void* symbol_41;

	CString symbol_42;
	CBuffer* symbol_43;
	int symbol_44;

	CBuffer* symbol_45;
	UINT symbol_46;
	UINT symbol_47;
	UINT symbol_48;


	int symbol_49;
	short symbol_50;
	short symbol_51;
	short symbol_52;
	short symbol_53;
	short symbol_54;
	short symbol_55;
	short symbol_56;
	short symbol_57;
	int symbol_58;
	LONGLONG symbol_59;
	LONGLONG symbol_60;
	LONGLONG symbol_61;
	LPWSTR symbol_62;
	CBuffer* symbol_63;
	CBuffer* symbol_64;

	int symbol_65;
	CEvaluationInfo* symbol_66;
	CCustomInfo* symbol_67;

	int symbol_68;
	CString symbol_69;
	CString symbol_70;
	CString symbol_71;
	CString symbol_72;
	CString symbol_73;
	CString symbol_74;

	BOOL symbol_75;

	CString symbol_76;

	CBuffer* symbol_77;
	CBuffer* symbol_78;

	CTimeSpan symbol_79;

	BOOL symbol_80;

	HANDLE symbol_81;
	HANDLE symbol_82;
	int symbol_83;
	BOOL symbol_84();
	void symbol_85();
	
	friend DWORD __stdcall symbol_86( LPVOID pParam );

	HANDLE symbol_87;
	HANDLE symbol_88;

	void symbol_89(BOOL join);
	BOOL symbol_90();

	friend DWORD __stdcall symbol_91( LPVOID pParam );

	CString symbol_92;
	CString symbol_93;
	BOOL symbol_94;
	CMapStringToString symbol_95;

	BYTE* symbol_96;
	int symbol_97;
	BOOL symbol_98;
	BYTE* symbol_99(int& machineCodeLength);
	void symbol_100();

	int symbol_101;
	BOOL symbol_102;

};

#ifndef UNDER_CE

class CUsageReportFeature 
{
public:
	void AddCustomData(LPCTSTR key,LPCTSTR value);

private:
	CString symbol_103;
	LONGLONG symbol_104;
	LONGLONG symbol_105;

	CUsageReportFeature(LPCTSTR name,LONGLONG startTime,LONGLONG endTime);
	CMapStringToString symbol_106;

	friend class CUsageReport;

};

class CUsageReport
{
public:
	CUsageReport();
	~CUsageReport();

	LONGLONG StartTime;
	LONGLONG EndTime;
	BSTR UserID;
	CCryptoLicense* AssociatedLicense;
	int SystemMemory;
	BSTR ScreenResolution;
	BSTR AppName;
	float AppVersion;
	BSTR OS;

	BOOL SendToService();
	BOOL SendToServiceFromBuffer(BYTE* buffer,DWORD length);
	BOOL SendToServiceFromFile(LPCTSTR filePath);
	void SendToServiceFromFolder(LPCTSTR folderPath,LPCTSTR reportFilesExtension,int* totalSent,int* totalFound);

	BYTE* SaveToBuffer(DWORD *length);
	BOOL SaveToFile(LPCTSTR filePath);

	void AddCustomData(LPCTSTR key,LPCTSTR value);

	CUsageReportFeature* RecordFeature(LPCTSTR featureName);
	CUsageReportFeature* RecordFeatureStart(LPCTSTR featureName);
	CUsageReportFeature* RecordFeatureEnd(LPCTSTR featureName);


private:

	CPtrList symbol_107;
	CMapStringToString symbol_108;
	CString symbol_109;

	void symbol_110();
	CStringBuilder* symbol_111();
	BOOL symbol_112(BYTE* buffer,DWORD length);
	void symbol_113();
	CUsageReportFeature* symbol_114(LPCTSTR name,int type);

};


#endif

#endif // !defined(AFX_LICENSE_H__02D560F1_10AB_4049_A422_D3DBB64F13F2__INCLUDED_)
