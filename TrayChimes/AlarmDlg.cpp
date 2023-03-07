// AlarmDlg.cpp : implementation file
//

#include "pch.h"
#include "traychimes.h"
//#include "AlarmDlg.h"
#include "TrayChimesDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlarmDlg dialog

CAlarmDlg::CAlarmDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DISPLAY_MESSAGE, pParent)
{
    m_strMessage = L"";
}

void CAlarmDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MESSAGE, m_Message);
    DDX_Text(pDX, IDC_MESSAGE, m_strMessage);
}

BEGIN_MESSAGE_MAP(CAlarmDlg, CDialogEx)
    ON_BN_CLICKED(IDC_SNOOZE, OnSnooze)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlarmDlg message handlers

void CAlarmDlg::OnCancel()
{
    CDialogEx::OnCancel();
}

void CAlarmDlg::OnOK()
{
    CDialogEx::OnOK();
}

BOOL CAlarmDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CAlarmDlg::OnSnooze()
{
    CTrayChimesDlg* pParent = (CTrayChimesDlg*)m_pParentWnd;

    pParent->Snooze();

    OnOK();
}

HBRUSH CAlarmDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    return (HBRUSH)GetStockObject(WHITE_BRUSH);
}
