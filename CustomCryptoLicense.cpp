// CustomCryptoLicense.cpp : implementation file
//

#include "stdafx.h"
#include "StreamWatch.h"
#include "CustomCryptoLicense.h"
//#include <winsock2.h>
//#include <iphlpapi.h>
//#include <stdio.h>
//#include <stdlib.h>
//#pragma comment(lib, "IPHLPAPI.lib")

// CCustomCryptoLicense

IMPLEMENT_DYNAMIC(CCustomCryptoLicense, CWnd)

CCustomCryptoLicense::CCustomCryptoLicense()
{
	nCurrentAdaptor = 0;
}

CCustomCryptoLicense::~CCustomCryptoLicense()
{
}

BEGIN_MESSAGE_MAP(CCustomCryptoLicense, CWnd)
END_MESSAGE_MAP()

// CCustomCryptoLicense message handlers

BYTE* CCustomCryptoLicense::GetLocalMachineCode(int& machineCodeLength)
{
	machineCodeLength = 0; 
	BYTE* ret = NULL; 

	DWORD dwRetVal = 0;
		
	ULONG flags = GAA_FLAG_INCLUDE_PREFIX;// Set the flags to pass to GetAdaptersAddresses
	ULONG family = AF_UNSPEC;			  // default to unspecified address family (both)
	ULONG outBufLen = 0;
	ULONG Iterations = 0;
	
	PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
	PIP_ADAPTER_ADDRESSES pAddresses = NULL;

	outBufLen = 10 * sizeof(IP_ADAPTER_ADDRESSES);// Allocate a 10 KB buffer to start with.

	do {
		
		pAddresses = new IP_ADAPTER_ADDRESSES[outBufLen];
		
		if (pAddresses == NULL) 
		{
			dwRetVal = ERROR_NOT_ENOUGH_MEMORY;
			break;
		}

		dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen); // For Windows XP and later

		if (dwRetVal == ERROR_BUFFER_OVERFLOW) 
		{
			SAFE_DELETE_ARRAY(pAddresses);
		}
		else
			break;

	} while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (++Iterations < 3));

	if (dwRetVal != NO_ERROR)
	{
		SAFE_DELETE_ARRAY(pAddresses);
		return ret;
	}

	unsigned int i = 0;
	pCurrAddresses = pAddresses;
	while(i++ < nCurrentAdaptor)
	{
		if(pCurrAddresses->Next)//if(pAddresses->Next)
			pCurrAddresses = pCurrAddresses->Next;
		else
			break;
	}
	
	if(pCurrAddresses == NULL)
	{
		SAFE_DELETE_ARRAY(pAddresses);
		return ret;
	}

	unsigned int nLength = 0;
	if (pCurrAddresses->PhysicalAddressLength <= 10 && pCurrAddresses->PhysicalAddressLength > 0) // EUI-48 = 6 and EUI-64 = 8
		nLength = (unsigned int)pCurrAddresses->PhysicalAddressLength * 2;

	// Convert Hex value to String
	char* HexString = NULL;
	HexString = new char[nLength+1];
	memset(HexString, 0, nLength);
	for(unsigned int i = 0; i < (unsigned int)pCurrAddresses->PhysicalAddressLength; i++)
		sprintf_s(HexString,nLength+1,"%s%02X",HexString, pCurrAddresses->PhysicalAddress[i]);
	
	// Copy the actual MAC value Globle Memory
	machineCodeLength = nLength;
	ret = (BYTE*)CoTaskMemAlloc(machineCodeLength);
	CopyMemory(ret, HexString, machineCodeLength);

	// deallocate the buffer. 
	SAFE_DELETE_ARRAY(HexString);
	SAFE_DELETE_ARRAY(pAddresses);

		
	// deallocate the buffer.
/*	SAFE_DELETE_ARRAY(pAddresses);//SAFE_DELETE_ARRAY(pBuffer);


    //for retrieving Hard Disk ID

	DWORD dwVolNameSize= 0, dwSerialNumber = 0;
	TCHAR strVolume[MAX_PATH]; 
	GetVolumeInformation(_T("\\\\.\\PHYSICALDRIVE0"), strVolume, MAX_PATH-1, &dwSerialNumber, NULL, NULL, NULL, NULL);
	int nHDD_String_Length = 0;

	char *hdID;
	char* ConcatedString = NULL;
	int nCount=0;
	
	string line = "";
	ifstream IPFile;
	        
	system("wmic diskdrive get serialnumber > serial.txt");

	IPFile.open("serial.txt"); 
	if(IPFile.is_open())
	{
		getline(IPFile,line);
		getline(IPFile,line);
		hdID = (char*)line.data();
		IPFile.close();
	}

	for(int i = 1 ; i < line.length()-6 ; i++)
	{			
		if(line.at(i) != '\0')
			{
				hardDiskID[nCount++] = line.at(i);
			}
	}
	
	nHDD_String_Length = nCount;
	
	int j=0;
	int ntotal_length = nLength + nHDD_String_Length;
	ConcatedString = new char[ntotal_length+1];
	memset(ConcatedString, 0, sizeof(char) * (ntotal_length + 1));
	for(int i=0;i<nLength;i++)
		ConcatedString[i] = HexString[i];
	for(i=nLength,j=0;i<ntotal_length;i++,j++)
		ConcatedString[i] = hardDiskID[j];

	
	// Copy the actual MAC value + Hard disk ID to Globle Memory

	machineCodeLength = ntotal_length;
	ret = (BYTE*)CoTaskMemAlloc(machineCodeLength);
	CopyMemory(ret, ConcatedString, machineCodeLength);

	TCHAR* SerialNumber = new TCHAR[ntotal_length + 1];
	memset(SerialNumber, 0 , sizeof(TCHAR) * (ntotal_length + 1));
	for(int i = 0; i <= ntotal_length; i++)
	{
		SerialNumber[i] = (wchar_t)ConcatedString[i];
	}

	TCHAR tempStr[MAX_PATH];
	memset(tempStr, 0, MAX_PATH);
	::GetModuleFileName(0, tempStr, MAX_PATH);
	unsigned int loc = (unsigned int)(_tcsrchr(tempStr, _T('\\')) - tempStr);
	tempStr[loc] = NULL;

	CString strBrowsedPath;
	strBrowsedPath.Format(_T("%s\\serial.txt"),tempStr);
	CFile SerialNumFile;
	CFileException excFile;
	OutputDebugString(SerialNumber);
	try
	{
		if(!bwriteDone)
		{
			if(SerialNumFile.Open(strBrowsedPath,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeBinary, &excFile))
			{
				SerialNumFile.Write(SerialNumber, _tcslen(SerialNumber)*sizeof(TCHAR));
				SerialNumFile.Close();
			}
			bwriteDone = true;
		}
	}
	catch(...)
	{
		OutputDebugString(_T("Failed to open serial.txt"));
	}

	SAFE_DELETE_ARRAY(HexString);
	SAFE_DELETE_ARRAY(ConcatedString);
	//delete[] SerialNumber;
*/
	return ret;
}