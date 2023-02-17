// TrayChimesDlg.h : header file
//
#include "TimeSelection.h"
#include "AlarmDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CTrayChimesDlg dialog

class CTrayChimesDlg : public CDialog
{
// Construction
public:
	CTrayChimesDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTrayChimesDlg)
	enum { IDD = IDD_TRAYCHIMES_DIALOG };
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
	//}}AFX_DATA
    CString m_Message;

    void Snooze();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrayChimesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

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
    CTimeSelection m_TimeSelection;

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
    BOOL TrayMessage(DWORD dwMessage, UINT uID,
        HICON hIcon, PSTR pszTip);
    BOOL TrayTip(DWORD dwMessage, UINT uID, PSTR pszTip);
    BOOL IsChimesSet();
    void SetNextChime();
    void PlayNextChime();
    void SetTipText(BOOL bAdd = FALSE);

	// Generated message map functions
	//{{AFX_MSG(CTrayChimesDlg)
	virtual BOOL OnInitDialog();
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
	//}}AFX_MSG
    afx_msg LRESULT OnTrayNotification(WPARAM wp, LPARAM lp);
	afx_msg void OnPopupAbout();
	afx_msg void OnPopupExit();
	afx_msg void OnPopupProperties();
    afx_msg LRESULT OnAddToTray(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
