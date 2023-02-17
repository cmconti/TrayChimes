// MessageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "traychimes.h"
#include "MessageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessageDlg dialog


CMessageDlg::CMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMessageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMessageDlg)
	m_Message = _T("");
	//}}AFX_DATA_INIT
}


void CMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMessageDlg)
	DDX_Text(pDX, IDC_MESSAGE, m_Message);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMessageDlg, CDialog)
	//{{AFX_MSG_MAP(CMessageDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageDlg message handlers
