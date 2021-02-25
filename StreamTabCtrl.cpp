#include "stdafx.h"
#include "StreamWatch.h"
#include "StreamTabCtrl.h"

#include "Stream_Watch_Tab1.h"
#include "Stream_Watch_Tab2.h"
#include "Stream_Watch_Tab3.h"
#include "Stream_Watch_Tab4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStreamTabCtrl

CStreamTabCtrl::CStreamTabCtrl()
{
	m_nNumberOfPages = 4;
}

CStreamTabCtrl::~CStreamTabCtrl()
{
	for(int nCount = 0; nCount < m_nNumberOfPages; nCount++)
	{
		delete m_tabPages[nCount];
	}
}

void CStreamTabCtrl::Init()
{
	m_tabCurrent=0;

	m_tabPages[0]->Create(IDD_STREAM_WATCH_TAB1, this);
	m_tabPages[1]->Create(IDD_STREAM_WATCH_TAB2, this);
	m_tabPages[2]->Create(IDD_STREAM_WATCH_TAB3, this);
	m_tabPages[3]->Create(IDD_STREAM_WATCH_TAB4, this);

	m_tabPages[0]->ShowWindow(SW_SHOW);
	m_tabPages[1]->ShowWindow(SW_HIDE);
	m_tabPages[2]->ShowWindow(SW_HIDE);
	m_tabPages[3]->ShowWindow(SW_HIDE);

	SetRectangle();
}

void CStreamTabCtrl::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	nX=itemRect.left;
	nY=itemRect.bottom + 1;
	nXc=tabRect.right - itemRect.left - 1;
	nYc=tabRect.bottom - nY - 1;

	m_tabPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	for(int nCount = 1; nCount < m_nNumberOfPages; nCount++)
	{
		m_tabPages[nCount]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	}
}

BEGIN_MESSAGE_MAP(CStreamTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CStreamTabCtrl)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStreamTabCtrl message handlers

void CStreamTabCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CTabCtrl::OnLButtonDown(nFlags, point);

	if(m_tabCurrent != GetCurFocus())
	{
		m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
		m_tabCurrent = GetCurFocus();
		m_tabPages[m_tabCurrent]->ShowWindow(SW_SHOW);
		m_tabPages[m_tabCurrent]->SetFocus();
	}
}
