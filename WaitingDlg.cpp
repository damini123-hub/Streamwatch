// WaitingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StreamWatch.h"
#include "WaitingDlg.h"


// WaitingDlg dialog

IMPLEMENT_DYNAMIC(WaitingDlg, CDialog)

WaitingDlg::WaitingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(WaitingDlg::IDD, pParent)
{
}

WaitingDlg::~WaitingDlg()
{
}

BOOL WaitingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	/*CRect ParentDlgPosition;
	m_pParentWnd->GetClientRect(&ParentDlgPosition);
	m_pParentWnd->ClientToScreen(&ParentDlgPosition);

	CRect WaitingDlgRect;
	GetClientRect(&WaitingDlgRect);
	ClientToScreen(&WaitingDlgRect);

	CRect ScreenRect;
    int MAX_HEIGHT = GetSystemMetrics(SM_CYSCREEN);
	int MAX_WIDTH = GetSystemMetrics(SM_CXSCREEN);

	int nLeft = ParentDlgPosition.left + ParentDlgPosition.Width()/2 - WaitingDlgRect.Width()/2;
	int nTop = ParentDlgPosition.top + ParentDlgPosition.Height()/2 - WaitingDlgRect.Height()/2;
	
	if(nLeft + WaitingDlgRect.Width() >= MAX_WIDTH)
		nLeft = ParentDlgPosition.left;

	if(nTop + WaitingDlgRect.Height() >= MAX_HEIGHT)
		nTop = ParentDlgPosition.top + 10;*/

	SetWindowPos(&CWnd::wndTop, 725, 350, 0, 0, SWP_NOSIZE);

	if (m_Picture.Load(MAKEINTRESOURCE(IDR_LOADING),_T("GIF")))
		m_Picture.Draw();

	//OutputDebugString(_T("\n\nWaitingDlg::OnInitDialog() - Finish\n\n"));

	return TRUE;
}

void WaitingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, 2008, m_Picture);
}


BEGIN_MESSAGE_MAP(WaitingDlg, CDialog)
END_MESSAGE_MAP()


// WaitingDlg message handlers
void WaitingDlg::OnClose()
{
	EndDialog(0);
}