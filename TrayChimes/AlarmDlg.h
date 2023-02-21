// AlarmDlg.h : header file
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CAlarmDlg dialog

class CAlarmDlg : public CDialogEx
{
    // Construction
public:
    CAlarmDlg(CWnd* pParent = nullptr);   // standard constructor

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DISPLAY_MESSAGE };
#endif
    CStatic	m_Message;
    CString	m_strMessage;

    // Overrides
        // ClassWizard generated virtual function overrides
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Implementation
protected:

    // Generated message map functions
    virtual void OnCancel();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnSnooze();
    DECLARE_MESSAGE_MAP();
};
