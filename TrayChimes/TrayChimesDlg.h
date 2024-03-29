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
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP();

private:
    Gdiplus::Bitmap* m_pBitmap;
};

// CTrayChimesDlg dialog
class CTrayChimesDlg : public CDialogEx
{
    // Construction
public:
    CTrayChimesDlg(CWnd* pParent = nullptr); // standard constructor

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_TRAYCHIMES_DIALOG };
#endif

    void Snooze();

private:
    CMFCEditBrowseCtrl m_edit00Chime;
    CButton m_btnPlay00;
    BOOL m_bChimeAt00;
    CString m_str00Chime;

    CMFCEditBrowseCtrl m_editHourChime;
    CButton m_btnPlayHour;
    BOOL m_bChimeHourCount;
    CString m_strHourChime;

    CMFCEditBrowseCtrl m_edit15Chime;
    CButton m_btnPlay15;
    BOOL m_bChimeAt15;
    CString m_str15Chime;

    CMFCEditBrowseCtrl m_edit30Chime;
    CButton m_btnPlay30;
    BOOL m_bChimeAt30;
    CString m_str30Chime;

    CMFCEditBrowseCtrl m_edit45Chime;
    CButton m_btnPlay45;
    BOOL m_bChimeAt45;
    CString m_str45Chime;

    CMFCEditBrowseCtrl m_editAlarmChime;
    CButton m_btnPlayAlarm;
    BOOL m_bAlarmSet;
    CString m_strAlarmChime;
    CDateTimeCtrl m_TimeSelection;
    COleDateTime m_timeAlarm;
    BOOL m_bPlayAlarmOnce;

    BOOL m_bDisplayMessage;
    CString m_Message;

    BOOL m_bRunOnStartup;

    void LoadDataFromRegistry();
    void SaveDataToRegistry();

protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

    // Implementation
protected:
    HICON m_hIcon;
    HICON m_hTrayIcon;

    HICON m_hIconSpeaker;

    int m_nTimerID;
    UINT m_uTrayID;

    BOOL m_bAllowShowWindow;

    CAboutDlg m_dlgAbout;
    CAlarmDlg m_dlgAlarm;

    CTime m_tNextChime;
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

    void PlayAudio(CEdit& editControl, bool bAsync = true);
    void PlayAudio(CString& audioFile, bool bAsync = true);
    void BrowseForSound(CEdit& editControl);

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnTimer(UINT nIDEvent);
    virtual void OnOK();
    afx_msg void OnClose();
    virtual void OnCancel();
    afx_msg void OnTimeChange();
    afx_msg void OnPlay00();
    afx_msg void OnPlay15();
    afx_msg void OnPlay30();
    afx_msg void OnPlay45();
    afx_msg void OnPlayHour();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
    afx_msg void OnPlayAlarm();
    afx_msg void OnDestroy();
    afx_msg void OnEditMessage();
    afx_msg LRESULT OnTrayNotification(WPARAM wp, LPARAM lp);
    afx_msg void OnPopupAbout();
    afx_msg void OnPopupExit();
    afx_msg void OnPopupProperties();
    afx_msg LRESULT OnAddToTray(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnMciNotify(WPARAM wParam, LPARAM lParam);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    DECLARE_MESSAGE_MAP();
};
