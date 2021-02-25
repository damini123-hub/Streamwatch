#pragma once
#include "IP Fail Over.h"
//#include "ColorEdit.h"
//#include "GdipButton.h"
//#include "ColorStatic.h"

// CLicenseExpiryDlg dialog

class CLicenseExpiryDlg : public CDialog
{
	DECLARE_DYNAMIC(CLicenseExpiryDlg)

public:
	BOOL m_bIsValid;
	BOOL m_bIsExpired;

	HICON m_hIcon;
	CBrush m_brushG;	
//	CColorStatic m_lblEnterActivationMsg, m_lblRegisterProductMsg, m_lblClientNameHeading, m_lblClientEmailIDHeading;
	/*CColorEdit m_tbxActivationKey, m_tbxClientName, m_tbxClientEmailID;
	CGdipButton m_btnSubmit, m_btnBuy, m_btnRequestForPurchase, m_btnMinimize, m_btnClose, m_btnExtend, m_btnRequestForExtend;
	CColorStatic m_lblEvaluationHeading, m_lblClientDetailsHeading, m_lblWaitMessage, m_lblExtendEvaluationMsg;
	CArray<CGdipButton,CGdipButton&> m_btnVersionType;*/
	//TCHAR m_AppPath[MAX_PATH];	
	CLicenseExpiryDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLicenseExpiryDlg();

// Dialog Data
//	enum { IDD = IDD_DIALOG_LICENSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtnexpirysubmit();
	afx_msg void OnBnClickedMinimizeBtn();
	afx_msg void OnBnClickedCloseBtn();
	afx_msg void OnBnClickedbtnbuy();
	afx_msg void OnBnClickedbtnrequestforpurchase();
	afx_msg void OnBnClickedbtnextend();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedbtnrequestforextend();
	void ShowClientDetailControls();
	void SendClientRequest(int nSendMode);

};
