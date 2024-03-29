// TrayChimesDlg.cpp : implementation file
//

#include "pch.h"
#include "TrayChimes.h"
#include "TrayChimesDlg.h"
#include "MessageDlg.h"
#include "afxdialogex.h"

#include <mmsystem.h>
#include <shellapi.h>

#include "GDIPlusHelpers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const UINT WM_TASKBARCREATED =
::RegisterWindowMessage(L"TaskbarCreated");

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX), m_pBitmap(nullptr)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BOOL CAboutDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    //TODO, get dlg height, less margins
    m_pBitmap = LoadPNGBitmap(180);

    return FALSE;
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
    ON_WM_CTLCOLOR()
    ON_WM_PAINT()
END_MESSAGE_MAP()

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    return (HBRUSH)GetStockObject(WHITE_BRUSH);
}

void CAboutDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    Gdiplus::Graphics graphics(dc);
    graphics.DrawImage(m_pBitmap, Gdiplus::Point(0, 0));
}

/////////////////////////////////////////////////////////////////////////////
// CTrayChimesDlg dialog

CTrayChimesDlg::CTrayChimesDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_TRAYCHIMES_DIALOG, pParent)
    , m_tspan15(0, 0, 15, 0)
    , m_tspan30(0, 0, 30, 0)
    , m_tspan45(0, 0, 45, 0)
    , m_tspan00(0, 1, 0, 0)
    , m_tspan24Hours(1, 0, 0, 0)
    , m_timeAlarm(COleDateTime::GetCurrentTime())
{
    m_bChimeAt15 = TRUE;
    m_bChimeAt30 = TRUE;
    m_bChimeAt45 = TRUE;
    m_bChimeHourCount = TRUE;
    m_bChimeAt00 = TRUE;
    m_str15Chime = L"";
    m_str30Chime = L"";
    m_str45Chime = L"";
    m_str00Chime = L"";
    m_strHourChime = L"";
    m_strAlarmChime = L"";
    m_bAlarmSet = FALSE;
    m_bDisplayMessage = FALSE;
    m_bPlayAlarmOnce = FALSE;
    m_bRunOnStartup = FALSE;

    m_uTrayID = IDR_TRAY_CONTEXT;
    m_bAllowShowWindow = TRUE;

    SetNextChime();
    TRACE(L"reset Alarm 1\n");
    int nAlarmHour = m_nActualAlarmHour = CTime::GetCurrentTime().GetHour();
    int nAlarmMinute = m_nActualAlarmMinute = CTime::GetCurrentTime().GetMinute();
    m_timeAlarm.SetTime(m_nActualAlarmHour, m_nActualAlarmMinute, 0);
    m_bAlarmPlayed = FALSE;

    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_hTrayIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_hIconSpeaker = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SPEAKER),
        IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
}

void CTrayChimesDlg::LoadDataFromRegistry()
{
    m_bChimeAt15 = AfxGetApp()->GetProfileInt(L"ChimeSelection", L"Chime15", TRUE);
    m_bChimeAt30 = AfxGetApp()->GetProfileInt(L"ChimeSelection", L"Chime30", TRUE);
    m_bChimeAt45 = AfxGetApp()->GetProfileInt(L"ChimeSelection", L"Chime45", TRUE);
    m_bChimeAt00 = AfxGetApp()->GetProfileInt(L"ChimeSelection", L"Chime00", TRUE);
    m_bChimeHourCount = AfxGetApp()->GetProfileInt(L"ChimeSelection", L"ChimeHour", TRUE);
    m_bAlarmSet = AfxGetApp()->GetProfileInt(L"ChimeSelection", L"AlarmSet", FALSE);

    TRACE(L"reset Alarm 2\n");
    int nAlarmHour = m_nActualAlarmHour = AfxGetApp()->GetProfileInt(L"ChimeSelection", L"AlarmHour", 12);
    int nAlarmMinute = m_nActualAlarmMinute = AfxGetApp()->GetProfileInt(L"ChimeSelection", L"AlarmMinute", 0);
    m_timeAlarm.SetTime(m_nActualAlarmHour, m_nActualAlarmMinute, 0);

    m_Message = AfxGetApp()->GetProfileString(L"ChimeSelection", L"Message", L"");
    m_bDisplayMessage = AfxGetApp()->GetProfileInt(L"ChimeSelection", L"DisplayMessage", FALSE);
    m_bPlayAlarmOnce = AfxGetApp()->GetProfileInt(L"ChimeSelection", L"PlayAlarmOnce", FALSE);

    m_str15Chime = AfxGetApp()->GetProfileString(L"ChimeSounds", L"Chime15", L"HourChime.mp3");
    m_str30Chime = AfxGetApp()->GetProfileString(L"ChimeSounds", L"Chime30", L"HourChime.mp3");
    m_str45Chime = AfxGetApp()->GetProfileString(L"ChimeSounds", L"Chime45", L"HourChime.mp3");
    m_str00Chime = AfxGetApp()->GetProfileString(L"ChimeSounds", L"Chime00", L"Westminster.mp3");
    m_strHourChime = AfxGetApp()->GetProfileString(L"ChimeSounds", L"ChimeHour", L"HourChime.mp3");
    m_strAlarmChime = AfxGetApp()->GetProfileString(L"ChimeSounds", L"AlarmChime", L"Alarm.mp3");

    m_bRunOnStartup = FALSE;
    CRegKey keyHKCURun;
    if (ERROR_SUCCESS == keyHKCURun.Open(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"))
    {
        ULONG nChars = 0;
        m_bRunOnStartup = (ERROR_SUCCESS == keyHKCURun.QueryStringValue(L"TrayChimes", NULL, &nChars));
    }
}

void CTrayChimesDlg::SaveDataToRegistry()
{
    AfxGetApp()->WriteProfileInt(L"ChimeSelection", L"Chime15", m_bChimeAt15);
    AfxGetApp()->WriteProfileInt(L"ChimeSelection", L"Chime30", m_bChimeAt30);
    AfxGetApp()->WriteProfileInt(L"ChimeSelection", L"Chime45", m_bChimeAt45);
    AfxGetApp()->WriteProfileInt(L"ChimeSelection", L"Chime00", m_bChimeAt00);
    AfxGetApp()->WriteProfileInt(L"ChimeSelection", L"ChimeHour", m_bChimeHourCount);
    AfxGetApp()->WriteProfileInt(L"ChimeSelection", L"AlarmSet", m_bAlarmSet);
    AfxGetApp()->WriteProfileInt(L"ChimeSelection", L"AlarmHour", m_timeAlarm.GetHour());
    AfxGetApp()->WriteProfileInt(L"ChimeSelection", L"AlarmMinute", m_timeAlarm.GetMinute());

    AfxGetApp()->WriteProfileString(L"ChimeSelection", L"Message", m_Message);
    AfxGetApp()->WriteProfileInt(L"ChimeSelection", L"DisplayMessage", m_bDisplayMessage);
    AfxGetApp()->WriteProfileInt(L"ChimeSelection", L"PlayAlarmOnce", m_bPlayAlarmOnce);

    AfxGetApp()->WriteProfileString(L"ChimeSounds", L"Chime15", m_str15Chime);
    AfxGetApp()->WriteProfileString(L"ChimeSounds", L"Chime30", m_str30Chime);
    AfxGetApp()->WriteProfileString(L"ChimeSounds", L"Chime45", m_str45Chime);
    AfxGetApp()->WriteProfileString(L"ChimeSounds", L"Chime00", m_str00Chime);
    AfxGetApp()->WriteProfileString(L"ChimeSounds", L"ChimeHour", m_strHourChime);
    AfxGetApp()->WriteProfileString(L"ChimeSounds", L"AlarmChime", m_strAlarmChime);

    CRegKey keyHKCURun;
    if (ERROR_SUCCESS == keyHKCURun.Open(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"))
    {
        if (m_bRunOnStartup)
        {
            HMODULE hmod = GetModuleHandle(NULL);

            CString fullPath;
            DWORD pathLen = ::GetModuleFileName(hmod, fullPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH); // hmod of zero gets the main EXE
            fullPath.ReleaseBuffer(pathLen);

            keyHKCURun.SetStringValue(L"TrayChimes", fullPath);
        }
        else
        {
            keyHKCURun.DeleteValue(L"TrayChimes");
        }
    }
}

void CTrayChimesDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_HOUR_CHIME, m_edit00Chime);
    DDX_Control(pDX, IDC_PLAY_00, m_btnPlay00);
    DDX_Check(pDX, IDC_CHIME_HOUR, m_bChimeAt00);
    DDX_Text(pDX, IDC_HOUR_CHIME, m_str00Chime);

    DDX_Control(pDX, IDC_HOUR_BONG, m_editHourChime);
    DDX_Control(pDX, IDC_PLAY_HOUR, m_btnPlayHour);
    DDX_Check(pDX, IDC_CHIME_COUNT, m_bChimeHourCount);
    DDX_Text(pDX, IDC_HOUR_BONG, m_strHourChime);

    DDX_Control(pDX, IDC_15_CHIME, m_edit15Chime);
    DDX_Control(pDX, IDC_PLAY_15, m_btnPlay15);
    DDX_Check(pDX, IDC_CHIME_15, m_bChimeAt15);
    DDX_Text(pDX, IDC_15_CHIME, m_str15Chime);

    DDX_Control(pDX, IDC_30_CHIME, m_edit30Chime);
    DDX_Control(pDX, IDC_PLAY_30, m_btnPlay30);
    DDX_Check(pDX, IDC_CHIME_30, m_bChimeAt30);
    DDX_Text(pDX, IDC_30_CHIME, m_str30Chime);

    DDX_Control(pDX, IDC_45_CHIME, m_edit45Chime);
    DDX_Control(pDX, IDC_PLAY_45, m_btnPlay45);
    DDX_Check(pDX, IDC_CHIME_45, m_bChimeAt45);
    DDX_Text(pDX, IDC_45_CHIME, m_str45Chime);

    DDX_Control(pDX, IDC_ALARM_CHIME, m_editAlarmChime);
    DDX_Control(pDX, IDC_PLAY_ALARM, m_btnPlayAlarm);
    DDX_Check(pDX, IDC_ALARM_SET, m_bAlarmSet);
    DDX_Text(pDX, IDC_ALARM_CHIME, m_strAlarmChime);
    DDX_Control(pDX, IDC_TIME_EDIT, m_TimeSelection);
    DDX_DateTimeCtrl(pDX, IDC_TIME_EDIT, m_timeAlarm);
    DDX_Check(pDX, IDC_ALARM_ONCE, m_bPlayAlarmOnce);

    DDX_Check(pDX, IDC_DISPLAY_MESSAGE, m_bDisplayMessage);

    DDX_Check(pDX, IDC_RUN_ON_STARTUP, m_bRunOnStartup);

    if (pDX->m_bSaveAndValidate)
    {
        CTime time;
        m_TimeSelection.GetTime(time);

        //get alarm time from control
        if ((m_timeAlarm.GetMinute() != time.GetMinute()) || m_timeAlarm.GetHour() != time.GetHour())
        {
            //alarm has changed, so reset snooze
            TRACE(L"reset Alarm 3\n");
            m_nActualAlarmHour = time.GetHour();
            m_nActualAlarmMinute = time.GetMinute();
            SetTipText();
        }
    }
}

BEGIN_MESSAGE_MAP(CTrayChimesDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    ON_WM_TIMECHANGE()
    ON_BN_CLICKED(IDC_PLAY_00, OnPlay00)
    ON_BN_CLICKED(IDC_PLAY_15, OnPlay15)
    ON_BN_CLICKED(IDC_PLAY_30, OnPlay30)
    ON_BN_CLICKED(IDC_PLAY_45, OnPlay45)
    ON_BN_CLICKED(IDC_PLAY_HOUR, OnPlayHour)
    ON_WM_SHOWWINDOW()
    ON_WM_WINDOWPOSCHANGING()
    ON_BN_CLICKED(IDC_PLAY_ALARM, OnPlayAlarm)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_EDIT_MESSAGE, OnEditMessage)
    ON_MESSAGE(CHIMES_NOTIFYICON, OnTrayNotification)
    ON_COMMAND(ID_POPUP_ABOUT, OnPopupAbout)
    ON_COMMAND(ID_POPUP_EXIT, OnPopupExit)
    ON_COMMAND(ID_POPUP_PROPERTIES, OnPopupProperties)
    ON_MESSAGE(WM_PSADDTOTRAY, OnAddToTray)
    ON_REGISTERED_MESSAGE(WM_TASKBARCREATED, OnTaskBarCreated)
    ON_BN_CLICKED(ID_CLOSE, &CTrayChimesDlg::OnClose)
    ON_MESSAGE(MM_MCINOTIFY, OnMciNotify)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrayChimesDlg message handlers
LRESULT CTrayChimesDlg::OnTaskBarCreated(WPARAM wp, LPARAM lp)
{
    //make sure that the original instance is in the tray
    ::PostMessage(GetSafeHwnd(), WM_PSADDTOTRAY, 0, 0);
    return 0;
}

BOOL CTrayChimesDlg::TrayMessage(DWORD dwMessage, UINT uID, HICON hIcon, PTSTR pszTip)
{
    BOOL res;

    NOTIFYICONDATA tnd;

    tnd.cbSize = sizeof(NOTIFYICONDATA);
    tnd.hWnd = m_hWnd;
    tnd.uID = uID;

    tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    tnd.uCallbackMessage = CHIMES_NOTIFYICON;
    tnd.hIcon = hIcon;
    if (pszTip)
    {
        lstrcpyn(tnd.szTip, pszTip, sizeof(tnd.szTip));
    }
    else
    {
        tnd.szTip[0] = L'\0';
    }

    res = Shell_NotifyIcon(dwMessage, &tnd);

    return res;
}

BOOL CTrayChimesDlg::TrayTip(DWORD dwMessage, UINT uID, PTSTR pszTip)
{
    BOOL res;

    NOTIFYICONDATA tnd;

    tnd.cbSize = sizeof(NOTIFYICONDATA);
    tnd.hWnd = m_hWnd;
    tnd.uID = uID;

    tnd.uFlags = NIF_TIP;
    if (pszTip)
    {
        lstrcpyn(tnd.szTip, pszTip, sizeof(tnd.szTip));
    }
    else
    {
        tnd.szTip[0] = L'\0';
    }

    res = Shell_NotifyIcon(dwMessage, &tnd);

    return res;
}

BOOL CTrayChimesDlg::OnInitDialog()
{
    ((CTrayChimesApp*)AfxGetApp())->SetSharedHWND(m_hWnd);

    LoadDataFromRegistry();

    CDialogEx::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);   // Set big icon
    SetIcon(m_hIcon, FALSE);  // Set small icon

    CString defaultExtension(L".wav; .mp3");
    CString extensionFilter(L"Sound Files (*.wav;*.mp3)|*.wav; *.mp3|All Files (*.*)|*.*||");
    m_edit00Chime.EnableFileBrowseButton(defaultExtension, extensionFilter);
    m_editHourChime.EnableFileBrowseButton(defaultExtension, extensionFilter);
    m_edit15Chime.EnableFileBrowseButton(defaultExtension, extensionFilter);
    m_edit30Chime.EnableFileBrowseButton(defaultExtension, extensionFilter);
    m_edit45Chime.EnableFileBrowseButton(defaultExtension, extensionFilter);
    m_editAlarmChime.EnableFileBrowseButton(defaultExtension, extensionFilter);

    m_btnPlay00.SetIcon(m_hIconSpeaker);
    m_btnPlay15.SetIcon(m_hIconSpeaker);
    m_btnPlay30.SetIcon(m_hIconSpeaker);
    m_btnPlay45.SetIcon(m_hIconSpeaker);
    m_btnPlayHour.SetIcon(m_hIconSpeaker);
    m_btnPlayAlarm.SetIcon(m_hIconSpeaker);

    SetTipText(TRUE);

    wchar_t buf[1000];

    int x = GetLocaleInfoEx(
        LOCALE_NAME_USER_DEFAULT,
        LOCALE_SSHORTTIME,
        buf,
        _countof(buf)
    );

    m_TimeSelection.SetFormat(buf);

    m_nTimerID = SetTimer(1, 1000, NULL);

    m_bAllowShowWindow = FALSE; // don't allow window to be shown initially

    return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTrayChimesDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTrayChimesDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CTrayChimesDlg::Snooze()
{
    //First, stop alarm sound.
    ::sndPlaySound(NULL, SND_ASYNC);

    //bump alarm to 10 minutes from now.
    CTime time = CTime::GetCurrentTime();

    time_t t = 600; //ten minutes

    CTimeSpan span(t);
    time += span;

    TRACE(L"reset Alarm 4\n");
    TRACE(L"SNOOZE: Changing alarm from %02d:%02d", m_nActualAlarmHour, m_nActualAlarmMinute);
    m_nActualAlarmHour = time.GetHour();
    m_nActualAlarmMinute = time.GetMinute();
    TRACE(L"to %02d:%02d\n", m_nActualAlarmHour, m_nActualAlarmMinute);

    SetTipText();
}

UINT AlarmDialogThread(LPVOID pParam)
{
    //pParam is the address of a CAlarmDlg
    if (pParam != NULL)
    {
        CAlarmDlg* pAlarm = (CAlarmDlg*)(pParam);
        if (pAlarm != NULL)
        {
            pAlarm->DoModal();
        }
    }

    return 0;
}

void CTrayChimesDlg::OnTimer(UINT nIDEvent)
{
    CTime time = CTime::GetCurrentTime();

    if (m_bAlarmSet)
    {
        if ((time.GetHour() == m_nActualAlarmHour) && (time.GetMinute() == m_nActualAlarmMinute))
        {
            if (!m_bAlarmPlayed) //check to see if alarm has been played once this minute already
            {
                m_bAlarmPlayed = TRUE;
                PlayAudio(m_strAlarmChime);

                if ((!::IsWindow(m_dlgAlarm.GetSafeHwnd())) && m_bDisplayMessage)
                {
                    m_dlgAlarm.m_strMessage = m_Message;
                    AfxBeginThread(&AlarmDialogThread, (LPVOID)(&m_dlgAlarm));
                }
                else if (m_bDisplayMessage)
                {
                    m_dlgAlarm.BringWindowToTop();
                }

                if (m_bPlayAlarmOnce)
                {
                    m_bAlarmSet = FALSE; //turn off alarm and save settings
                    CButton* pCheck = (CButton*)GetDlgItem(IDC_ALARM_SET);
                    if (pCheck != NULL)
                    {
                        pCheck->SetCheck(FALSE);
                    }
                    AfxGetApp()->WriteProfileInt(L"ChimeSelection", L"AlarmSet", m_bAlarmSet);
                }

                //reset m_nActualAlarmHour and m_nActualAlarmMinute;
                TRACE(L"reset Alarm 5\n");
                m_nActualAlarmHour = m_timeAlarm.GetHour();
                m_nActualAlarmMinute = m_timeAlarm.GetMinute();
                SetTipText();
            }
        }
        else
        {
            m_bAlarmPlayed = FALSE;
        }
    }

    if (time >= m_tNextChime)
    {
        TRACE(L"Play Chimes");
        PlayNextChime();
        SetNextChime();
        SetTipText();
    }

    CDialogEx::OnTimer(nIDEvent);
}

void CTrayChimesDlg::SetTipText(BOOL bAdd /*=FALSE*/)
{
    CString strNextChime = m_tNextChime.Format(L"Next Chime: %I:%M");
    CString TipText;
    if (m_bAlarmSet)
    {
        TipText.Format(L"%s  Next Alarm: %02d:%02d", strNextChime, (m_nActualAlarmHour % 12), m_nActualAlarmMinute);
    }
    else
    {
        TipText = strNextChime;
    }

    if (bAdd)
    {
        TrayMessage(NIM_ADD, m_uTrayID, m_hTrayIcon, TipText.GetBuffer(0));
    }
    else
    {
        TrayTip(NIM_MODIFY, m_uTrayID, TipText.GetBuffer(0));
    }
    TipText.ReleaseBuffer();
}

void CTrayChimesDlg::OnDestroy()
{
    TRACE(L"Destroy\n");
    KillTimer(m_nTimerID);
    TrayMessage(NIM_DELETE, m_uTrayID, NULL, NULL);

    CDialogEx::OnDestroy();
}

BOOL CTrayChimesDlg::IsChimesSet()
{
    return (m_bChimeAt15 | m_bChimeAt30 | m_bChimeAt45 | m_bChimeAt00 | m_bChimeHourCount);
}

void CTrayChimesDlg::SetNextChime()
{
    if (IsChimesSet())
    {
        //Get current time
        CTime time = CTime::GetCurrentTime();
        int Year = time.GetYear();
        int Month = time.GetMonth();
        int Day = time.GetDay();
        int Hour = time.GetHour();
        int CurrentMinute = time.GetMinute();
        int Second = 0;

        CTime timeQuarter(Year, Month, Day, Hour, CurrentMinute, Second);

        //figure out the next quarter hour
        int MinutesToNextQuarter = 15 - (CurrentMinute % 15);
        CTimeSpan tspanNextQuarter(0, 0, MinutesToNextQuarter, 0);

        timeQuarter += tspanNextQuarter;

        int NextQuarterMinute = timeQuarter.GetMinute();
        if (NextQuarterMinute == CurrentMinute)
        {
            timeQuarter += m_tspan15;
            NextQuarterMinute = timeQuarter.GetMinute();
        }

        if (NextQuarterMinute == 0)
        {
            if (m_bChimeAt00 | m_bChimeHourCount)
            {
                m_tNextChime = timeQuarter;
            }
            else if (m_bChimeAt15)
            {
                timeQuarter += m_tspan15;
                m_tNextChime = timeQuarter;
            }
            else if (m_bChimeAt30)
            {
                timeQuarter += m_tspan30;
                m_tNextChime = timeQuarter;
            }
            else
            {
                timeQuarter += m_tspan45;
                m_tNextChime = timeQuarter;
            }
        }
        else if (NextQuarterMinute == 15)
        {
            if (m_bChimeAt15)
            {
                m_tNextChime = timeQuarter;
            }
            else if (m_bChimeAt30)
            {
                timeQuarter += m_tspan15;
                m_tNextChime = timeQuarter;
            }
            else if (m_bChimeAt45)
            {
                timeQuarter += m_tspan30;
                m_tNextChime = timeQuarter;
            }
            else
            {
                timeQuarter += m_tspan45;
                m_tNextChime = timeQuarter;
            }
        }
        else if (NextQuarterMinute == 30)
        {
            if (m_bChimeAt30)
            {
                m_tNextChime = timeQuarter;
            }
            else if (m_bChimeAt45)
            {
                timeQuarter += m_tspan15;
                m_tNextChime = timeQuarter;
            }
            else if (m_bChimeAt00 | m_bChimeHourCount)
            {
                timeQuarter += m_tspan30;
                m_tNextChime = timeQuarter;
            }
            else
            {
                timeQuarter += m_tspan45;
                m_tNextChime = timeQuarter;
            }
        }
        else
        {
            if (m_bChimeAt45)
            {
                m_tNextChime = timeQuarter;
            }
            else if (m_bChimeAt00 | m_bChimeHourCount)
            {
                timeQuarter += m_tspan15;
                m_tNextChime = timeQuarter;
            }
            else if (m_bChimeAt15)
            {
                timeQuarter += m_tspan30;
                m_tNextChime = timeQuarter;
            }
            else
            {
                timeQuarter += m_tspan45;
                m_tNextChime = timeQuarter;
            }
        }
    }
}

void CTrayChimesDlg::PlayNextChime()
{
    int minute = m_tNextChime.GetMinute();

    if (minute == 15)
    {
        PlayAudio(m_str15Chime, false);
    }
    else if (minute == 30)
    {
        PlayAudio(m_str30Chime, false);
    }
    else  if (minute == 45)
    {
        PlayAudio(m_str45Chime, false);
    }
    else
    {
        if (m_bChimeAt00)
        {
            PlayAudio(m_str00Chime);
        }

        if (m_bChimeHourCount)
        {
            int hour = m_tNextChime.GetHour();
            hour = hour % 12;
            if (hour == 0)
            {
                hour = 12;
            }

            for (int ii = 0; ii < hour; ii++)
            {
                PlayAudio(m_strHourChime, false);
            }
        }
    }
}

void CTrayChimesDlg::OnOK()
{
    UpdateData();
    SaveDataToRegistry();
    SetNextChime();
    SetTipText();
    ShowWindow(SW_HIDE);
}

//The cancel button
void CTrayChimesDlg::OnClose()
{
    UpdateData(FALSE);
    ShowWindow(SW_HIDE);
}

//The exit button
void CTrayChimesDlg::OnCancel()
{
    UpdateData(FALSE);
    CDialogEx::OnCancel();
}

//////////////////
// Handle notification from tray icon: display a message.
//
LRESULT CTrayChimesDlg::OnTrayNotification(WPARAM uID, LPARAM lEvent)
{
    if (/*m_bShowTrayNotifications*/TRUE) {
        static LPCWSTR MouseMessages[] = { L"WM_MOUSEMOVE",
           L"WM_LBUTTONDOWN", L"WM_LBUTTONUP", L"WM_LBUTTONDBLCLK",
           L"WM_RBUTTONDOWN", L"WM_RBUTTONUP", L"WM_RBUTTONDBLCLK",
           L"WM_MBUTTONDOWN", L"WM_MBUTTONUP", L"WM_MBUTTONDBLCLK" };

        CString s;
        s.Format(L"Tray notification: ID=%d, lEvent=0x%04x %s\r\n",
            uID, lEvent, WM_MOUSEFIRST <= lEvent && lEvent <= WM_MOUSELAST ?
            MouseMessages[lEvent - WM_MOUSEFIRST] : L"(Unknown)");
    }

    if (uID != m_uTrayID || (lEvent != WM_RBUTTONUP && lEvent != WM_LBUTTONDBLCLK))
        return 0;

    // If there's a resource menu with the same ID as the icon, use it as
    // the right-button popup menu. CTrayIcon will interprets the first
    // item in the menu as the default command for WM_LBUTTONDBLCLK
    //
    CMenu menu;
    if (!menu.LoadMenu(m_uTrayID))
        return 0;
    CMenu* pSubMenu = menu.GetSubMenu(0);
    if (!pSubMenu)
        return 0;

    if (lEvent == WM_RBUTTONUP)
    {
        // Make first menu item the default (bold font)
        ::SetMenuDefaultItem(pSubMenu->m_hMenu, 0, TRUE);

        // Display the menu at the current mouse location. There's a "bug"
        // (Microsoft calls it a feature) in Windows 95 that requires calling
        // SetForegroundWindow. To find out more, search for Q135788 in MSDN.
        //
        CPoint mouse;
        GetCursorPos(&mouse);
        ::SetForegroundWindow(m_hWnd);
        ::TrackPopupMenu(pSubMenu->m_hMenu, 0, mouse.x, mouse.y, 0, m_hWnd, NULL);
    }
    else  // double click: execute first menu item
    {
        ::SendMessage(m_hWnd, WM_COMMAND, pSubMenu->GetMenuItemID(0), 0);
    }

    return 1; // handled
}

void CTrayChimesDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        OnPopupAbout();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

void CTrayChimesDlg::OnPopupAbout()
{
    if (m_dlgAbout.GetSafeHwnd() == NULL)
        m_dlgAbout.DoModal();
    else
        m_dlgAbout.BringWindowToTop();
}

void CTrayChimesDlg::OnPopupExit()
{
    TRACE(L"Exit\n");
    EndDialog(IDOK);
}

void CTrayChimesDlg::OnPopupProperties()
{
    m_bAllowShowWindow = TRUE; //reset value
    UpdateData(FALSE);
    ShowWindow(SW_SHOW);
    SetForegroundWindow();
}

void CTrayChimesDlg::OnTimeChange()
{
    TRACE(L"Time Change\n");
    SetNextChime();
    SetTipText();
    CDialogEx::OnTimeChange();
}

void CTrayChimesDlg::PlayAudio(CEdit& editControl, bool bAsync /*= true*/)
{
    CString strPath;
    editControl.GetWindowText(strPath);
    PlayAudio(strPath, bAsync);
}

void  CTrayChimesDlg::PlayAudio(CString& audioFile, bool bAsync /*= true*/)
{
    ::sndPlaySound(NULL, SND_ASYNC);

    if (audioFile.Right(4).CompareNoCase(L".wav") == 0)
    {
        UINT fuSound = bAsync ? SND_ASYNC : (SND_SYNC | SND_NOSTOP);
        ::sndPlaySound(audioFile, fuSound);
        return;
    }

    static bool guidSet = false;
    CString songAlias(L"tc_sound");
    if (!guidSet)
    {
        RPC_WSTR guidStr;
        GUID guid;
        HRESULT hr = CoCreateGuid(&guid);
        if (hr == S_OK)
        {
            if (UuidToString(&guid, &guidStr) == RPC_S_OK)
            {
                songAlias.Format(L"tc_%ls", guidStr);
                RpcStringFree(&guidStr);
            }
        }
        guidSet = true;
    }

    CString openCmd;
    openCmd.Format(L"open \"%ls\" type mpegvideo alias %ls", audioFile, songAlias);

    LPCWSTR pszWait = bAsync ? L"notify" : L"wait";
    HWND hCallback = bAsync ? m_hWnd : nullptr;
    CString playCmd;
    playCmd.Format(L"play %ls %ls", songAlias, pszWait);

    CString closeCmd;
    closeCmd.Format(L"close %ls", songAlias);

    MCIERROR me = mciSendString(openCmd, NULL, 0, 0);

    if (me == 0)
    {
        me = mciSendString(playCmd, NULL, 0, hCallback);
        if (!bAsync)
        {
            mciSendString(closeCmd, NULL, 0, 0);
        }
    }
}

void CTrayChimesDlg::OnPlay00()
{
    PlayAudio(m_edit00Chime);
}

void CTrayChimesDlg::OnPlay15()
{
    PlayAudio(m_edit15Chime);
}

void CTrayChimesDlg::OnPlay30()
{
    PlayAudio(m_edit30Chime);
}

void CTrayChimesDlg::OnPlay45()
{
    PlayAudio(m_edit45Chime);
}

void CTrayChimesDlg::OnPlayHour()
{
    PlayAudio(m_editHourChime);
}

void CTrayChimesDlg::OnPlayAlarm()
{
    PlayAudio(m_editAlarmChime);
}

void CTrayChimesDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    if (m_bAllowShowWindow)
        CDialogEx::OnShowWindow(bShow, nStatus);
}

void CTrayChimesDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
    CDialogEx::OnWindowPosChanging(lpwndpos);

    if (!m_bAllowShowWindow)
    {
        if ((lpwndpos->flags & SWP_SHOWWINDOW) == SWP_SHOWWINDOW)
        {
            lpwndpos->flags &= ~SWP_SHOWWINDOW;
        }
    }
}

void CTrayChimesDlg::OnEditMessage()
{
    CMessageDlg dlg;

    dlg.m_Message = m_Message;
    if (dlg.DoModal() == IDOK)
    {
        m_Message = dlg.m_Message;
    }
}

LRESULT CTrayChimesDlg::OnAddToTray(WPARAM wParam, LPARAM lParam)
{
    SetTipText(TRUE);
    return 0;
}

LRESULT CTrayChimesDlg::OnMciNotify(WPARAM wFlags, LPARAM lDeviceID)
{
    //switch (wFlags)
    //{
    //case MCI_NOTIFY_SUCCESSFUL:
    //    break;
    //case MCI_NOTIFY_FAILURE:
    //    break;
    //case MCI_NOTIFY_SUPERSEDED:
    //    break;
    //}
    mciSendCommand(lDeviceID, MCI_CLOSE, 0, NULL);

    return 0L;
}

HBRUSH CTrayChimesDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    return (HBRUSH)GetStockObject(WHITE_BRUSH);
}
