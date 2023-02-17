// AlarmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "traychimes.h"
//#include "AlarmDlg.h"
#include "TrayChimesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlarmDlg dialog


CAlarmDlg::CAlarmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAlarmDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlarmDlg)
	m_strMessage = _T("");
	//}}AFX_DATA_INIT
}


void CAlarmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlarmDlg)
	DDX_Control(pDX, IDC_MESSAGE, m_Message);
	DDX_Text(pDX, IDC_MESSAGE, m_strMessage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlarmDlg, CDialog)
	//{{AFX_MSG_MAP(CAlarmDlg)
	ON_BN_CLICKED(IDC_SNOOZE, OnSnooze)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlarmDlg message handlers

void CAlarmDlg::OnCancel() 
{
    CDialog::OnCancel();
}

void CAlarmDlg::OnOK() 
{
    CDialog::OnOK();
}

BOOL CAlarmDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
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
