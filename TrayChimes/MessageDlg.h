// MessageDlg.h : header file
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CMessageDlg dialog

class CMessageDlg : public CDialogEx
{
    // Construction
public:
    CMessageDlg(CWnd* pParent = nullptr);   // standard constructor

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MESSAGE };
#endif
    CString	m_Message;

    // Overrides
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Implementation
protected:

    // Generated message map functions
    DECLARE_MESSAGE_MAP();
};
