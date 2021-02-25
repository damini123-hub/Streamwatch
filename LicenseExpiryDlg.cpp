
#include "stdafx.h"
#include "LicenseExpiryDlg.h"

#include "IP Fail Over.h"

#define VALIDATION_KEY  _T("AQABAAOpvRLHm9Yg7N3/sF3116mKgzRhfe3EjJfVGKAx567fd+yQM/TYpKyTwWoxE/U4UacAMADAAA==")

IMPLEMENT_DYNAMIC(CLicenseExpiryDlg, CDialog)

CLicenseExpiryDlg::CLicenseExpiryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLicenseExpiryDlg::IDD, pParent)
{
	m_brushG.CreateSolidBrush(RGB(100,100,100));
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bIsValid = false;
	m_bIsExpired = false;
}

CLicenseExpiryDlg::~CLicenseExpiryDlg()
{
}

void CLicenseExpiryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	/*DDX_Control(pDX, IDC_btnExpirySubmit, m_btnSubmit);
	DDX_Control(pDX, IDC_btnBuy, m_btnBuy);
	DDX_Control(pDX, IDC_btnExtend, m_btnExtend);	
	DDX_Control(pDX, IDC_btnRequestForPurchase, m_btnRequestForPurchase);*/
	DDX_Control(pDX, IDC_btnRequestForExtend, m_btnRequestForExtend);	
	DDX_Control(pDX, IDC_Minimize_Btn, m_btnMinimize);
	DDX_Control(pDX, IDC_Close_Btn, m_btnClose);
}

BEGIN_MESSAGE_MAP(CLicenseExpiryDlg, CDialog)
	/*ON_BN_CLICKED(IDC_btnExpirySubmit, &CLicenseExpiryDlg::OnBnClickedbtnexpirysubmit)*/
	ON_BN_CLICKED(IDC_Minimize_Btn, &CLicenseExpiryDlg::OnBnClickedMinimizeBtn)
	ON_BN_CLICKED(IDC_Close_Btn, &CLicenseExpiryDlg::OnBnClickedCloseBtn)
	/*ON_BN_CLICKED(IDC_btnBuy, &CLicenseExpiryDlg::OnBnClickedbtnbuy)
	ON_BN_CLICKED(IDC_btnRequestForPurchase, &CLicenseExpiryDlg::OnBnClickedbtnrequestforpurchase)
	ON_BN_CLICKED(IDC_btnExtend, &CLicenseExpiryDlg::OnBnClickedbtnextend)
	ON_WM_CTLCOLOR()*/
	ON_BN_CLICKED(IDC_btnRequestForExtend, &CLicenseExpiryDlg::OnBnClickedbtnrequestforextend)
END_MESSAGE_MAP()

void CLicenseExpiryDlg::OnBnClickedbtnexpirysubmit()
{
	CString strActivationKey;
	m_tbxActivationKey.GetWindowText(strActivationKey);

	if(_tcscmp(strActivationKey, _T("")))
	{
		CString LicenseKey = CString(strActivationKey);
		LicenseKey = LicenseKey.Trim();

		LicenseStatus nResult = LS_NotValidated;		
		for(int i = 0; i < 6; i++)
		{
			CCustomCryptoLicense AppEvalLicense;
			AppEvalLicense.SetValidationKey(VALIDATION_KEY);		
			AppEvalLicense.SetLicenseCode(LicenseKey);			
			AppEvalLicense.nCurrentAdaptor = i;
			if(AppEvalLicense.GetStatus() == LS_Valid)
			{
				nResult = LS_Valid;
				break;
			}
		}

		if(nResult == LS_Valid)
		{
			FILE *outputfile = NULL;
			
			TCHAR strFilePath[300];
			//_stprintf(strFilePath,_T("%s\\Ad Serter 1\\EvaluationLicense.ini"),strAppFolderPath);*/
			_stprintf(strFilePath, _T("%s\\EvaluationLicense.ini"), ((CDvbTestBedApp*)AfxGetApp())->m_strAppDataPath);

			CFile::Remove(strFilePath);
			outputfile = _tfsopen(strFilePath, _T("a"), _SH_DENYWR );

			if(outputfile)
			{
				//fwrite(strActivationKey, _tcslen(strActivationKey), sizeof(TCHAR), outputfile);
				#ifdef UNICODE
					char strKey[MAX_KEY_LENGTH];
					memset(strKey, 0, sizeof(strKey));

					wcstombs(strKey, strActivationKey, strActivationKey.GetLength());
					fwrite(strKey, strlen(strKey), sizeof(char), outputfile);
				#else
					fwrite(strActivationKey, _tcslen(strActivationKey), sizeof(TCHAR), outputfile);
				#endif
				fclose(outputfile);

				outputfile = NULL;			
				m_bIsValid = true;
				OnCancel();
				AfxMessageBox(_T("License has been successfully registered !"));							
			}
			else
			{				
				MessageBox(_T("System Information could not be saved !!\nPlease try later or contact at info@wisedv.com"), _T("Ad Serter"), MB_ICONERROR | MB_TOPMOST);
			}
		}
		else
			AfxMessageBox(_T("Invalid Activation Key !"));
	}
	else
		AfxMessageBox(_T("Activation Key cannot be kept Empty"));
}

void CLicenseExpiryDlg::OnBnClickedMinimizeBtn()
{
	ShowWindow(SW_MINIMIZE);
}

void CLicenseExpiryDlg::OnBnClickedCloseBtn()
{
	OnCancel();
}

void CLicenseExpiryDlg::OnBnClickedbtnbuy()
{
	ShowClientDetailControls();
	m_btnRequestForPurchase.ShowWindow(1);
	m_btnRequestForExtend.ShowWindow(0);
}

void CLicenseExpiryDlg::ShowClientDetailControls()
{
	m_lblEvaluationHeading.ShowWindow(0);
	m_lblEnterActivationMsg.ShowWindow(0);	
	m_tbxActivationKey.ShowWindow(0);
	m_btnSubmit.ShowWindow(0);
	m_lblRegisterProductMsg.ShowWindow(0);
	m_btnBuy.ShowWindow(0);
	m_btnExtend.ShowWindow(0);
	m_lblExtendEvaluationMsg.ShowWindow(0);

	m_lblClientDetailsHeading.ShowWindow(1);
	m_lblClientNameHeading.ShowWindow(1);
	m_tbxClientName.ShowWindow(1);
	m_lblClientEmailIDHeading.ShowWindow(1);
	m_tbxClientEmailID.ShowWindow(1);
	m_lblWaitMessage.ShowWindow(1);
}



void CLicenseExpiryDlg::OnBnClickedbtnrequestforpurchase()
{
	SendClientRequest(2);
}

void CLicenseExpiryDlg::OnBnClickedbtnextend()
{
	ShowClientDetailControls();
	m_btnRequestForPurchase.ShowWindow(0);
	m_btnRequestForExtend.ShowWindow(1);
}

BOOL CLicenseExpiryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);

	SetWindowText(_T("Ad Serter"));

	m_btnSubmit.LoadStdImage(IDB_pngSubmit, _T("PNG"));
	m_btnBuy.LoadStdImage(IDB_pngBuy, _T("PNG"));
	m_btnExtend.LoadStdImage(IDB_pngExtend, _T("PNG"));
	m_btnRequestForPurchase.LoadStdImage(IDB_pngReqeuestForPurchase, _T("PNG"));
	m_btnRequestForPurchase.SetToolTipText(_T("To send request for Payment"), 1);
	m_btnRequestForExtend.LoadStdImage(IDB_pngRequestForExtend, _T("PNG"));
	m_btnRequestForExtend.SetToolTipText(_T("To send request for Payment"), 1);	
	m_btnClose.LoadStdImage(IDB_pngClose, _T("PNG"));
	m_btnMinimize.LoadStdImage(IDB_pngMinimize, _T("PNG"));
	m_btnSubmit.SetToolTipText(_T("Submit Activation Key"), 1);
	
	m_btnBuy.SetToolTipText(_T("To Buy Ad Serter"), 1);
	m_btnExtend.SetToolTipText(_T("To Extend Evaluation Period of Ad Serter"), 1);
	m_btnMinimize.SetToolTipText(_T("Minimize Ad Serter"), 1);
	m_btnClose.SetToolTipText(_T("Close Ad Serter"), 1);

	int CamControlID = 3001;
	int Control_Left_Position = 32;
	int Control_Top_Position = 20;
	int Control_Right_Position = Control_Left_Position + 220;
	int Control_Bottom_Position = Control_Top_Position + 18;

	TCHAR strVerExp[100];
	_stprintf(strVerExp, _T("Ad Serter Version has Expired"));
	m_lblEvaluationHeading.Create(strVerExp, WS_VISIBLE|WS_CHILD,CRect(Control_Left_Position,Control_Top_Position,Control_Right_Position,Control_Bottom_Position),this,(CamControlID));
	m_lblEvaluationHeading.SetBkColor(RGB(100,100,100));
	m_lblEvaluationHeading.SetTextColor(RGB(255,255,255));
	m_lblEvaluationHeading.ShowWindow(m_bIsExpired);
	
	Control_Top_Position = Control_Bottom_Position + 20;
	Control_Right_Position = Control_Left_Position + 220;
	Control_Bottom_Position = Control_Top_Position + 18;
	CamControlID++;

	m_lblEnterActivationMsg.Create(_T("Please enter Activation Key"), WS_VISIBLE|WS_CHILD,CRect(Control_Left_Position,Control_Top_Position,Control_Right_Position,Control_Bottom_Position),this,(CamControlID));
	m_lblEnterActivationMsg.SetBkColor(RGB(100,100,100));
	m_lblEnterActivationMsg.SetTextColor(RGB(203,121,21));
	m_lblEnterActivationMsg.ShowWindow(1);	

	Control_Top_Position = Control_Bottom_Position + 15;
	Control_Right_Position = Control_Left_Position + 280;
	Control_Bottom_Position = Control_Top_Position + 18;
	CamControlID++;

	m_tbxActivationKey.Create(WS_VISIBLE|WS_CHILD|ES_AUTOHSCROLL|WS_TABSTOP,CRect(Control_Left_Position,Control_Top_Position,Control_Right_Position,Control_Bottom_Position),this,(CamControlID));
	m_tbxActivationKey.SetLimitText(MAX_KEY_LENGTH);
	m_tbxActivationKey.SetBkColor(RGB(255,255,255));
	m_tbxActivationKey.SetTextColor(RGB(36,36,36));
	m_tbxActivationKey.ShowWindow(1);

	Control_Top_Position = Control_Bottom_Position + 15;
	Control_Right_Position = Control_Left_Position + 100;
	Control_Bottom_Position = Control_Top_Position + 35;
	CamControlID++;
	
	Control_Top_Position = Control_Bottom_Position + 23;
	Control_Right_Position = Control_Left_Position + 190;
	Control_Bottom_Position = Control_Top_Position + 18;
	CamControlID++;

	m_lblRegisterProductMsg.Create(_T("To Get Registered Key"), WS_VISIBLE|WS_CHILD,CRect(Control_Left_Position,Control_Top_Position,Control_Right_Position,Control_Bottom_Position),this,(CamControlID));\
	m_lblRegisterProductMsg.SetBkColor(RGB(100,100,100));
	m_lblRegisterProductMsg.SetTextColor(RGB(203,121,21));
	m_lblRegisterProductMsg.ShowWindow(1);

	Control_Top_Position = Control_Bottom_Position + 22;
	Control_Right_Position = Control_Left_Position + 190;
	Control_Bottom_Position = Control_Top_Position + 18;
	CamControlID++;

	m_lblExtendEvaluationMsg.Create(_T("To Extend Evaluation Period"), WS_VISIBLE|WS_CHILD,CRect(Control_Left_Position,Control_Top_Position,Control_Right_Position,Control_Bottom_Position),this,(CamControlID));
	m_lblExtendEvaluationMsg.SetBkColor(RGB(100,100,100));
	m_lblExtendEvaluationMsg.SetTextColor(RGB(203,121,21));
	m_lblExtendEvaluationMsg.ShowWindow(1);
	CamControlID++;	

	Control_Left_Position = 32;
	Control_Top_Position = 35;
	Control_Right_Position = Control_Left_Position + 220;
	Control_Bottom_Position = Control_Top_Position + 18;

	m_lblClientDetailsHeading.Create(_T("Please Enter following details:"), WS_VISIBLE|WS_CHILD,CRect(Control_Left_Position,Control_Top_Position,Control_Right_Position,Control_Bottom_Position),this,(CamControlID));
	m_lblClientDetailsHeading.SetBkColor(RGB(100,100,100));
	m_lblClientDetailsHeading.SetTextColor(RGB(255,255,255));
	m_lblClientDetailsHeading.ShowWindow(0);

	Control_Top_Position = Control_Bottom_Position + 15;
	Control_Right_Position = Control_Left_Position + 220;
	Control_Bottom_Position = Control_Top_Position + 18;
	CamControlID++;

	m_lblClientNameHeading.Create(_T("Your Name"), WS_VISIBLE|WS_CHILD,CRect(Control_Left_Position,Control_Top_Position,Control_Right_Position,Control_Bottom_Position),this,(CamControlID));
	m_lblClientNameHeading.SetBkColor(RGB(100,100,100));
	m_lblClientNameHeading.SetTextColor(RGB(203,121,21));
	m_lblClientNameHeading.ShowWindow(0);

	Control_Top_Position = Control_Bottom_Position + 5;
	Control_Right_Position = Control_Left_Position + 280;
	Control_Bottom_Position = Control_Top_Position + 18;
	CamControlID++;

	m_tbxClientName.Create(WS_VISIBLE|WS_CHILD|ES_AUTOHSCROLL|WS_TABSTOP,CRect(Control_Left_Position,Control_Top_Position,Control_Right_Position,Control_Bottom_Position),this,(CamControlID));
	m_tbxClientName.SetLimitText(20);
	m_tbxClientName.ShowWindow(0);

	Control_Top_Position = Control_Bottom_Position + 15;
	Control_Right_Position = Control_Left_Position + 220;
	Control_Bottom_Position = Control_Top_Position + 18;
	CamControlID++;

	m_lblClientEmailIDHeading.Create(_T("Your Email-ID"), WS_VISIBLE|WS_CHILD,CRect(Control_Left_Position,Control_Top_Position,Control_Right_Position,Control_Bottom_Position),this,(CamControlID));
	m_lblClientEmailIDHeading.SetBkColor(RGB(100,100,100));
	m_lblClientEmailIDHeading.SetTextColor(RGB(203,121,21));
	m_lblClientEmailIDHeading.ShowWindow(0);

	Control_Top_Position = Control_Bottom_Position + 5;
	Control_Right_Position = Control_Left_Position + 280;
	Control_Bottom_Position = Control_Top_Position + 18;
	CamControlID++;

	m_tbxClientEmailID.Create(WS_VISIBLE|WS_CHILD|ES_AUTOHSCROLL|WS_TABSTOP,CRect(Control_Left_Position,Control_Top_Position,Control_Right_Position,Control_Bottom_Position),this,(CamControlID));
	m_tbxClientEmailID.SetLimitText(50);
	m_tbxClientEmailID.ShowWindow(0);

	Control_Top_Position = Control_Bottom_Position + 75;
	Control_Right_Position = Control_Left_Position + 280;
	Control_Bottom_Position = Control_Top_Position + 18;
	CamControlID++;

	m_lblWaitMessage.Create(_T(""), WS_VISIBLE|WS_CHILD,CRect(Control_Left_Position,Control_Top_Position,Control_Right_Position,Control_Bottom_Position),this,(CamControlID));
	m_lblWaitMessage.SetBkColor(RGB(100,100,100));
	m_lblWaitMessage.SetTextColor(RGB(255,255,255));
	m_lblWaitMessage.ShowWindow(0);

	return TRUE;  
}

HBRUSH CLicenseExpiryDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	switch (nCtlColor)
	{
		case CTLCOLOR_DLG:		pDC->SetBkMode(TRANSPARENT);
								pDC->SetTextColor(RGB(255,255,255));
								pDC->SetBkColor(RGB(0,0,0));			
								return (HBRUSH)(m_brushG.GetSafeHandle());
								break;
	}
	return hbr;
}

void CLicenseExpiryDlg::OnBnClickedbtnrequestforextend()
{
	SendClientRequest(1);
}

void CLicenseExpiryDlg::SendClientRequest(int nSendMode)
{
	TCHAR* strProductName = _T("Ad Serter");

	CString strName = _T(""), strEmailID = _T("");
	bool bNameEmpty = false;
	UINT bEmailIDEmpty = 0;

	m_tbxClientName.GetWindowText(strName);
	if(strName.IsEmpty())
		bNameEmpty = true;

	m_tbxClientEmailID.GetWindowText(strEmailID);
	if(strEmailID.IsEmpty())
		bEmailIDEmpty = 1;
	else
	{
		// w@w.w is allowed ; minimum of this will not be allowed
		unsigned int nLength = strEmailID.GetLength();
		int nAtLocation  = strEmailID.Find(_T('@'));
		int nLastDotLocation = strEmailID.ReverseFind(_T('.')); // retrieve last . in string
		
		if( nAtLocation < 1						|| // @ should not be -1 or 0
			nAtLocation >= nLength - 3			|| // w.w is length 3
			nLastDotLocation < 1				|| // . should not be -1 or 0
			nLastDotLocation >= nLength - 1		|| // after last . there should be one letter
			nAtLocation + 1 == nLastDotLocation || // @. string is not allowed
			nAtLocation > nLastDotLocation)		   // last . should be come after @

			bEmailIDEmpty = 2;
	}

	if(!bNameEmpty && !bEmailIDEmpty)
	{
		m_lblWaitMessage.SetWindowText(_T("               This will take few minutes"));

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

			dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

			if (dwRetVal == ERROR_BUFFER_OVERFLOW) 
			{
				SAFE_DELETE_ARRAY(pAddresses);
			}
			else
				break;

		} while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (++Iterations < 3));

		if (dwRetVal != NO_ERROR)
		{
			MessageBox(_T("System Information could not be retrieved !!\nInternet is required to proceed next stage.\nPlease try later or contact at info@wisedv.com"),strProductName, MB_ICONERROR | MB_TOPMOST);
			SAFE_DELETE_ARRAY(pAddresses);
			return;
		}
		
		unsigned int nLength = 0;
		if (pAddresses->PhysicalAddressLength <= 10 && pAddresses->PhysicalAddressLength > 0) // EUI-48 = 6 and EUI-64 = 8
			nLength = (unsigned int)pAddresses->PhysicalAddressLength * 2;

		// Convert Hex value to String
		TCHAR* HexString = NULL;
		HexString = new TCHAR[nLength+1];
		memset(HexString, 0, nLength);
		
		for(unsigned int i = 0; i < (unsigned int)pAddresses->PhysicalAddressLength; i++)
			_stprintf(HexString,_T("%s%02X"),HexString, pAddresses->PhysicalAddress[i]);

		//Cretae .ini file contain Customer Name, Customer Email-ID & MAC Address
		TCHAR strINI_Path[MAX_PATH];
		TCHAR strAppDataPath[MAX_PATH];
		
		memset(strAppDataPath, 0, sizeof(strAppDataPath));
		_stprintf(strAppDataPath, _T("%s"), ((CDvbTestBedApp*)AfxGetApp())->m_strAppDataPath);
		
		switch(nSendMode)
		{
			case 1: _stprintf(strINI_Path, _T("%s\\%s%s.ini"),strAppDataPath,_T("Extend_") ,strEmailID);
					break;
			case 2:	_stprintf(strINI_Path, _T("%s\\%s%s.ini"),strAppDataPath,_T("Buy_") ,strEmailID);
					break;
		}		
		
		FILE *outputfile = NULL;
		outputfile = _tfsopen(strINI_Path, _T("w"), _SH_DENYWR );
		if(outputfile)
		{
			fwrite(_T("~"), sizeof(TCHAR), 1, outputfile);								// start code
			fwrite(strName, sizeof(TCHAR), strName.GetLength(), outputfile);			// write customer name
			fwrite(_T("~$~"), sizeof(TCHAR), 3, outputfile);								// separation code
			fwrite(strEmailID, sizeof(TCHAR), strEmailID.GetLength(), outputfile);	// write customer email address
			fwrite(_T("~$~"), sizeof(TCHAR), 3, outputfile);								// separation code
			fwrite(HexString, sizeof(TCHAR), nLength, outputfile);					// write customer MAC Address
			fwrite(_T("~$~"), sizeof(TCHAR), 3, outputfile);								// separation code
			switch(nSendMode)
			{
				case 1: fwrite(_T("1"), sizeof(TCHAR), 1, outputfile);					// write request code 1 - Request for Extending Evaluation
						break;
				case 2:	fwrite(_T("2"), sizeof(TCHAR), 1, outputfile);					// write request code 2 - Request for Purchaseing
						break;
			}
			fwrite(_T("~$~"), sizeof(TCHAR), 3, outputfile);								// separation code
			fwrite(strProductName, sizeof(TCHAR), _tcslen(strProductName), outputfile);
			fwrite(_T("~"), sizeof(TCHAR), 1, outputfile);								// end code
			fclose(outputfile);
			outputfile = NULL;
		}
		else
		{
			MessageBox(_T("System Information could not be saved !!\nInternet is required to proceed next stage.\nPlease try later or contact at info@wisedv.com"), strProductName, MB_ICONERROR | MB_TOPMOST);
			
			SAFE_DELETE_ARRAY(pAddresses);
			SAFE_DELETE_ARRAY(HexString);
			return;
		}

		// Create FTP connection to put file
		CInternetSession FTPsess(_T("Ad Serter/1.0"));
		CFtpConnection* pFTPConnect = NULL;

		try
		{
			pFTPConnect = FTPsess.GetFtpConnection(_T("ftp.wisedv.com"), _T("WiseCastLicense"), _T("EasyCastLicense1112*"), 21, TRUE);
			
			BOOL nResult = FALSE;
			DWORD dwError = 0;
			strEmailID += _T(".ini");
			TCHAR strUploadFileName[100];
			switch(nSendMode)
			{
				case 1: _stprintf(strUploadFileName, _T("Extend_%s"),strEmailID);
						break;
				case 2:	_stprintf(strUploadFileName, _T("Buy_%s"),strEmailID);
						break;
			}		
			
			nResult = pFTPConnect->PutFile(strINI_Path, strUploadFileName, FTP_TRANSFER_TYPE_ASCII, 1);
			dwError = GetLastError();
			if(!nResult)
				MessageBox(_T("Server may be down. Sorry for inconvenience.\nPlease try later or contact at info@wisedv.com"), strProductName, MB_ICONWARNING | MB_OK  | MB_TOPMOST | MB_TOPMOST);
			else
			{	
				OnCancel();
				MessageBox(_T("THANK YOU !!\n\nWiseDV will contact you as early as possible.\n\nFor any query, please contact at info@wisedv.com"), strProductName, MB_ICONINFORMATION | MB_OK  | MB_TOPMOST | MB_TOPMOST);
			}
		}
		catch (CInternetException* pEx)
		{
			TCHAR sz[1024];
			pEx->GetErrorMessage((LPTSTR)sz, 1024);			
			pEx->Delete();
			
			OnCancel();
			MessageBox(_T("There is some problem with Internet Conncetion.\nPlease try later or contact at info@wisedv.com"), strProductName, MB_ICONERROR | MB_TOPMOST | MB_OK  | MB_TOPMOST);

			SAFE_DELETE_ARRAY(HexString);
			SAFE_DELETE_ARRAY(pAddresses);//SAFE_DELETE_ARRAY(pBuffer);
			return;
		}

		// if the connection is open, close it
		if (pFTPConnect != NULL) 
		{
			pFTPConnect->Close();
			SAFE_DELETE(pFTPConnect);			
		}

		DeleteFile(strINI_Path);
		SAFE_DELETE_ARRAY(HexString);
		SAFE_DELETE_ARRAY(pAddresses);//SAFE_DELETE_ARRAY(pBuffer);
	}
	else
	{
		TCHAR strmsg[100];
		memset(strmsg, 0, sizeof(strmsg));
		//sprintf(strmsg, "Following Fields are invalid\n\n");
		if(bNameEmpty)
			_stprintf(strmsg, _T("%sName  - cannot be Empty\n"), strmsg);
		
		if(bEmailIDEmpty == 1)
			_stprintf(strmsg, _T("%sEmailID - cannot be Empty\n"), strmsg);
		else if(bEmailIDEmpty == 2)
			_stprintf(strmsg, _T("%sEmailID - is not valid\n"), strmsg);

		MessageBox(strmsg, strProductName, MB_ICONERROR | MB_TOPMOST);
	}
}