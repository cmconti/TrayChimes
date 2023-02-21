// MessageDlg.cpp : implementation file
//

#include "pch.h"
#include "traychimes.h"
#include "MessageDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessageDlg dialog

CMessageDlg::CMessageDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MESSAGE, pParent)
{
    m_Message = L"";
}

void CMessageDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_MESSAGE, m_Message);
}

BEGIN_MESSAGE_MAP(CMessageDlg, CDialogEx)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageDlg message handlers
