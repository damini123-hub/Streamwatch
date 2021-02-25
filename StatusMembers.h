#pragma once

//#include "global.h"
#include "stdafx.h"
#include "PictureCtrl.h"

class CStatusMembers
{
public:
	CPictureCtrl m_pic_Prime_Stream[MAX_STREAM];
	CPictureCtrl m_pic_Second_Stream[MAX_STREAM];
	CPictureCtrl m_pic_Output_Stream[MAX_STREAM];
	CString m_strStatusChannel_Name[MAX_STREAM];
};