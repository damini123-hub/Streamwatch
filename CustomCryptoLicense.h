#pragma once


// CCustomCryptoLicense

#include "stdafx.h"

#ifdef _WIN64
	#include "CryptoLicense.h"
#else
	#include "CryptoLicense_x86.h"
#endif

class CCustomCryptoLicense : public CWnd, public CCryptoLicense
{
	DECLARE_DYNAMIC(CCustomCryptoLicense)

public:
	CCustomCryptoLicense();
	virtual ~CCustomCryptoLicense();
	 // Return the MAC address as the machine code 
	virtual BYTE* GetLocalMachineCode(int& machineCodeLength);

	UINT nCurrentAdaptor;
	
protected:
	DECLARE_MESSAGE_MAP()
};


