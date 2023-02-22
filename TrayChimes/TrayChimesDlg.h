// TrayChimesDlg.h : header file
//

#pragma once

#include "AlarmDlg.h"

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

    // Implementation
protected:
    DECLARE_MESSAGE_MAP();

private:
    CPngImage m_RegulatorImg;
    CStatic m_staticImage;
};

// CTrayChimesDlg dialog
class CTrayChimesDlg : public CDialogEx
{
    // Construction
public:
    CTrayChimesDlg(CWnd* pParent = nullptr);	// standard constructor

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_TRAYCHIMES_DIALOG };
#endif

    CButton	m_btnPlayAlarm;
    CButton	m_btnPlayHour;
    CButton	m_btnPlay45;
    CButton	m_btnPlay30;
    CButton	m_btnPlay15;
    CButton	m_btnPlay00;
    BOOL	m_bChimeAt15;
    BOOL	m_bChimeAt30;
    BOOL	m_bChimeAt45;
    BOOL	m_bChimeHourCount;
    BOOL	m_bChimeAt00;
    CString	m_str15Chime;
    CString	m_str30Chime;
    CString	m_str45Chime;
    CString	m_str00Chime;
    CString	m_strHourChime;
    CString	m_strAlarmChime;
    BOOL	m_bAlarmSet;
    BOOL	m_bDisplayMessage;
    BOOL	m_bPlayAlarmOnce;

    CString m_Message;

    void Snooze();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

    // Implementation
protected:
    HICON m_hIcon;
    HICON m_hTrayIcon;

    HICON m_hIconSpeaker;

    int m_nTimerID;
    UINT m_uTrayID;

    BOOL m_AccessRegistry;
    BOOL m_bAllowShowWindow;

    CAboutDlg m_dlgAbout;
    CDateTimeCtrl m_TimeSelection;

    CAlarmDlg m_AlarmDlg;

    CTime m_tNextChime;
    int m_nAlarmHour;
    int m_nAlarmMinute;
    int m_nActualAlarmHour;     //in case snooze was pressed
    int m_nActualAlarmMinute;   //in case snooze was pressed
    BOOL m_bAlarmPlayed;

    CTimeSpan m_tspan15;
    CTimeSpan m_tspan30;
    CTimeSpan m_tspan45;
    CTimeSpan m_tspan00;
    CTimeSpan m_tspan24Hours;

    LRESULT OnTaskBarCreated(WPARAM wp, LPARAM lp);
    BOOL TrayMessage(DWORD dwMessage, UINT uID, HICON hIcon, PTSTR pszTip);
    BOOL TrayTip(DWORD dwMessage, UINT uID, PTSTR pszTip);
    BOOL IsChimesSet();
    void SetNextChime();
    void PlayNextChime();
    void SetTipText(BOOL bAdd = FALSE);

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnTimer(UINT nIDEvent);
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnTimeChange();
    afx_msg void OnPlay00();
    afx_msg void OnPlay15();
    afx_msg void OnPlay30();
    afx_msg void OnPlay45();
    afx_msg void OnPlayHour();
    afx_msg void On15Browse();
    afx_msg void On30Browse();
    afx_msg void On45Browse();
    afx_msg void OnBongBrowse();
    afx_msg void OnHourBrowse();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
    afx_msg void OnAlarmBrowse();
    afx_msg void OnPlayAlarm();
    afx_msg void OnDestroy();
    afx_msg void OnEditMessage();
    afx_msg LRESULT OnTrayNotification(WPARAM wp, LPARAM lp);
    afx_msg void OnPopupAbout();
    afx_msg void OnPopupExit();
    afx_msg void OnPopupProperties();
    afx_msg LRESULT OnAddToTray(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP();
};
