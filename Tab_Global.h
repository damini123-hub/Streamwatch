#pragma once

#include "stdafx.h"
//#include "global.h"
#include "PictureCtrl.h"

class CTabGeneral
{
public:
	CInput m;
	CComboBox m_cmbSourceAdapter[MAX_STREAM/3][MAX_RECEIVER];
	CString m_strNic_Source[MAX_STREAM/3][MAX_RECEIVER];
	CString m_strIp_Source[MAX_STREAM/3][MAX_RECEIVER];
	int m_nPort_Source[MAX_STREAM/3][MAX_RECEIVER];

	CComboBox m_cmbDestAdapter[MAX_STREAM/3];
	CString m_strNic_Dest[MAX_STREAM/3];
	CString m_strIp_Dest[MAX_STREAM/3];
	int m_nPort_Dest[MAX_STREAM/3];
	int m_m_nDetection_Time[MAX_STREAM/3];
	int m_nPresentDetection_Time[MAX_STREAM/3];
	CString m_strChannel_Name[MAX_STREAM/3];

	CPictureCtrl m_PriStatus[MAX_STREAM/3];
	CPictureCtrl m_SecondaryStatus[MAX_STREAM/3];
	CPictureCtrl m_OtputStatus[MAX_STREAM/3];

	CComboBox m_cmbPrgNo[MAX_STREAM/3];
};