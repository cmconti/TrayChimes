// TrayChimesDlg.cpp : implementation file
//

#include "pch.h"
#include "TrayChimes.h"
#include "TrayChimesDlg.h"
#include "MessageDlg.h"
#include "afxdialogex.h"

#include <mmsystem.h>
#include <shellapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const UINT WM_TASKBARCREATED =
::RegisterWindowMessage(L"TaskbarCreated");

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_REGULATOR, m_staticImage);
}

//https://stackoverflow.com/questions/2770855/how-do-you-scale-a-cbitmap-object
void ResizeBitmap(CBitmap& bmp_src, CBitmap& bmp_dst, int dstW, int dstH)
{
    BITMAP bm = { 0 };
    bmp_src.GetBitmap(&bm);
    auto size = CSize(bm.bmWidth, bm.bmHeight);
    CWindowDC wndDC(NULL);
    CDC srcDC;
    srcDC.CreateCompatibleDC(&wndDC);
    auto oldSrcBmp = srcDC.SelectObject(&bmp_src);

    CDC destDC;
    destDC.CreateCompatibleDC(&wndDC);
    bmp_dst.CreateCompatibleBitmap(&wndDC, dstW, dstH);
    auto oldDestBmp = destDC.SelectObject(&bmp_dst);

    destDC.StretchBlt(0, 0, dstW, dstH, &srcDC, 0, 0, size.cx, size.cy, SRCCOPY);
}

BOOL CAboutDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    VERIFY(m_RegulatorImg.Load(IDB_REGULATOR_PNG));

    BITMAP bm = { 0 };
    m_RegulatorImg.GetBitmap(&bm);
    auto imgSize = CSize(bm.bmWidth, bm.bmHeight);

    //CRect clientRect;
    //m_staticImage.GetClientRect(&clientRect);

    //auto ctrlHeight = clientRect.Height();
    auto ctrlHeight = 120;

    auto imgAspectRatio = (double)imgSize.cx / (double)imgSize.cy;

    CBitmap resizedImg;
    ResizeBitmap(m_RegulatorImg, resizedImg, (int)((double)ctrlHeight * imgAspectRatio), ctrlHeight);
    m_staticImage.SetBitmap(resizedImg);

    return FALSE;
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrayChimesDlg dialog

CTrayChimesDlg::CTrayChimesDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_TRAYCHIMES_DIALOG, pParent),
    m_tspan15(0, 0, 15, 0),
    m_tspan30(0, 0, 30, 0),
    m_tspan45(0, 0, 45, 0),
    m_tspan00(0, 1, 0, 0),
    m_tspan24Hours(1, 0, 0, 0)
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
    m_AccessRegistry = TRUE;
    m_bAllowShowWindow = TRUE;

    SetNextChime();
    TRACE(L"reset Alarm 1\n");
    m_nActualAlarmHour = m_nAlarmHour = CTime::GetCurrentTime().GetHour();
    m_nActualAlarmMinute = m_nAlarmMinute = CTime::GetCurrentTime().GetMinute();
    m_bAlarmPlayed = FALSE;

    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_hTrayIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_hIconSpeaker = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SPEAKER),
        IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
}

void CTrayChimesDlg::DoDataExchange(CDataExchange* pDX)
{
    if (!pDX->m_bSaveAndValidate && m_AccessRegistry)  //retrieving
    {
        m_bChimeAt15 = AfxGetApp()->GetProfileInt(L"ChimeSelection", L"Chime15", TRUE);
        m_bChimeAt30 = AfxGetApp()->GetProfileInt(L"ChimeSelection", L"Chime30", TRUE);
        m_bChimeAt45 = AfxGetApp()->GetProfileInt(L"ChimeSelection", L"Chime45", TRUE);
        m_bChimeAt00 = AfxGetApp()->GetProfileInt(L"ChimeSelection", L"Chime00", TRUE);
        m_bChimeHourCount = AfxGetApp()->GetProfileInt(L"ChimeSelection", L"ChimeHour", TRUE);
        m_bAlarmSet = AfxGetApp()->GetProfileInt(L"ChimeSelection", L"AlarmSet", FALSE);
        TRACE(L"reset Alarm 2\n");
        m_nActualAlarmHour = m_nAlarmHour = AfxGetApp()->GetProfileInt(L"ChimeSelection", L"AlarmHour", 12);
        m_nActualAlarmMinute = m_nAlarmMinute = AfxGetApp()->GetProfileInt(L"ChimeSelection", L"AlarmMinute", 0);

        m_Message = AfxGetApp()->GetProfileString(L"ChimeSelection", L"Message", L"");
        m_bDisplayMessage = AfxGetApp()->GetProfileInt(L"ChimeSelection", L"DisplayMessage", FALSE);
        m_bPlayAlarmOnce = AfxGetApp()->GetProfileInt(L"ChimeSelection", L"PlayAlarmOnce", FALSE);

        m_str15Chime = AfxGetApp()->GetProfileString(L"ChimeSounds", L"Chime15", L"HourChime.wav");
        m_str30Chime = AfxGetApp()->GetProfileString(L"ChimeSounds", L"Chime30", L"HourChime.wav");
        m_str45Chime = AfxGetApp()->GetProfileString(L"ChimeSounds", L"Chime45", L"HourChime.wav");
        m_str00Chime = AfxGetApp()->GetProfileString(L"ChimeSounds", L"Chime00", L"Westminster.wav");
        m_strHourChime = AfxGetApp()->GetProfileString(L"ChimeSounds", L"ChimeHour", L"HourChime.wav");
        m_strAlarmChime = AfxGetApp()->GetProfileString(L"ChimeSounds", L"AlarmChime", L"Alarm.wav");

        m_bRunOnStartup = FALSE;
        CRegKey keyHKCURun;
        if (ERROR_SUCCESS == keyHKCURun.Open(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"))
        {
            ULONG nChars = 0;
            m_bRunOnStartup = (ERROR_MORE_DATA == keyHKCURun.QueryStringValue(L"TrayChimes", NULL, &nChars));
        }
    }

    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PLAY_ALARM, m_btnPlayAlarm);
    DDX_Control(pDX, IDC_PLAY_HOUR, m_btnPlayHour);
    DDX_Control(pDX, IDC_PLAY_45, m_btnPlay45);
    DDX_Control(pDX, IDC_PLAY_30, m_btnPlay30);
    DDX_Control(pDX, IDC_PLAY_15, m_btnPlay15);
    DDX_Control(pDX, IDC_PLAY_00, m_btnPlay00);
    DDX_Check(pDX, IDC_CHIME_15, m_bChimeAt15);
    DDX_Check(pDX, IDC_CHIME_30, m_bChimeAt30);
    DDX_Check(pDX, IDC_CHIME_45, m_bChimeAt45);
    DDX_Check(pDX, IDC_CHIME_COUNT, m_bChimeHourCount);
    DDX_Check(pDX, IDC_CHIME_HOUR, m_bChimeAt00);
    DDX_Text(pDX, IDC_15_CHIME, m_str15Chime);
    DDX_Text(pDX, IDC_30_CHIME, m_str30Chime);
    DDX_Text(pDX, IDC_45_CHIME, m_str45Chime);
    DDX_Text(pDX, IDC_HOUR_CHIME, m_str00Chime);
    DDX_Text(pDX, IDC_HOUR_BONG, m_strHourChime);
    DDX_Text(pDX, IDC_ALARM_CHIME, m_strAlarmChime);
    DDX_Check(pDX, IDC_ALARM_SET, m_bAlarmSet);
    DDX_Check(pDX, IDC_DISPLAY_MESSAGE, m_bDisplayMessage);
    DDX_Check(pDX, IDC_ALARM_ONCE, m_bPlayAlarmOnce);
    DDX_Control(pDX, IDC_TIME_EDIT, m_TimeSelection);
    DDX_Check(pDX, IDC_RUN_ON_STARTUP, m_bRunOnStartup);

    if (!pDX->m_bSaveAndValidate)
    {
        //set alarm time in control
        CTime time(2005, 1, 1, m_nAlarmHour, m_nAlarmMinute, 0);
        m_TimeSelection.SetTime(&time);
    }

    if (pDX->m_bSaveAndValidate)
    {
        CTime time;
        m_TimeSelection.GetTime(time);

        //get alarm time from control
        if ((m_nAlarmMinute != time.GetMinute()) || (m_nAlarmHour != time.GetHour()))
        {
            //alarm has changed, so reset snooze
            TRACE(L"reset Alarm 3\n");
            m_nActualAlarmHour = m_nAlarmHour = time.GetHour();
            m_nActualAlarmMinute = m_nAlarmMinute = time.GetMinute();
            SetTipText();
        }
    }

    if (pDX->m_bSaveAndValidate && m_AccessRegistry)  //storing
    {
        AfxGetApp()->WriteProfileInt(L"ChimeSelection", L"Chime15", m_bChimeAt15);
        AfxGetApp()->WriteProfileInt(L"ChimeSelection", L"Chime30", m_bChimeAt30);
        AfxGetApp()->WriteProfileInt(L"ChimeSelection", L"Chime45", m_bChimeAt45);
        AfxGetApp()->WriteProfileInt(L"ChimeSelection", L"Chime00", m_bChimeAt00);
        AfxGetApp()->WriteProfileInt(L"ChimeSelection", L"ChimeHour", m_bChimeHourCount);
        AfxGetApp()->WriteProfileInt(L"ChimeSelection", L"AlarmSet", m_bAlarmSet);
        AfxGetApp()->WriteProfileInt(L"ChimeSelection", L"AlarmHour", m_nAlarmHour);
        AfxGetApp()->WriteProfileInt(L"ChimeSelection", L"AlarmMinute", m_nAlarmMinute);

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
    ON_BN_CLICKED(IDC_15_BROWSE, On15Browse)
    ON_BN_CLICKED(IDC_30_BROWSE, On30Browse)
    ON_BN_CLICKED(IDC_45_BROWSE, On45Browse)
    ON_BN_CLICKED(IDC_BONG_BROWSE, OnBongBrowse)
    ON_BN_CLICKED(IDC_HOUR_BROWSE, OnHourBrowse)
    ON_WM_SHOWWINDOW()
    ON_WM_WINDOWPOSCHANGING()
    ON_BN_CLICKED(IDC_ALARM_BROWSE, OnAlarmBrowse)
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
                ::sndPlaySound(m_strAlarmChime, SND_ASYNC);

                if ((!::IsWindow(m_AlarmDlg.GetSafeHwnd())) && m_bDisplayMessage)
                {
                    m_AlarmDlg.m_strMessage = m_Message;
                    AfxBeginThread(&AlarmDialogThread, (LPVOID)(&m_AlarmDlg));
                }
                else if (m_bDisplayMessage)
                {
                    m_AlarmDlg.BringWindowToTop();
                }

                if (m_bPlayAlarmOnce)
                {
                    m_bAlarmSet = FALSE; //turn off alarm and save settings
                    CButton* pCheck = (CButton*)GetDlgItem(IDC_ALARM_SET);
                    if (pCheck != NULL)
                    {
                        pCheck->SetCheck(FALSE);    //turn off check because DDX will reset variable
                    }
                    m_AccessRegistry = TRUE;
                    UpdateData(TRUE);
                }
                //reset m_nActualAlarmHour and m_nActualAlarmMinute;
                TRACE(L"reset Alarm 5\n");
                m_nActualAlarmHour = m_nAlarmHour;
                m_nActualAlarmMinute = m_nAlarmMinute;
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
        ::sndPlaySound(m_str15Chime, SND_SYNC | SND_NOSTOP);
    }
    else if (minute == 30)
    {
        ::sndPlaySound(m_str30Chime, SND_SYNC | SND_NOSTOP);
    }
    else  if (minute == 45)
    {
        ::sndPlaySound(m_str45Chime, SND_SYNC | SND_NOSTOP);
    }
    else
    {
        if (m_bChimeAt00)
            ::sndPlaySound(m_str00Chime, SND_SYNC | SND_NOSTOP);

        if (m_bChimeHourCount)
        {
            int hour = m_tNextChime.GetHour();
            hour = hour % 12;
            if (hour == 0)
                hour = 12;

            for (int ii = 0; ii < hour; ii++)
                ::sndPlaySound(m_strHourChime, SND_SYNC | SND_NOSTOP);
        }
    }
}

void CTrayChimesDlg::OnOK()
{
    UpdateData();
    ShowWindow(SW_HIDE);
}

void CTrayChimesDlg::OnClose()
{
    UpdateData(FALSE);
    ShowWindow(SW_HIDE);
}

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
        ::SendMessage(m_hWnd, WM_COMMAND, pSubMenu->GetMenuItemID(0), 0);

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

void CTrayChimesDlg::OnPlay00()
{
    ::sndPlaySound(m_str00Chime, SND_ASYNC);
}

void CTrayChimesDlg::OnPlay15()
{
    ::sndPlaySound(m_str15Chime, SND_ASYNC);
}

void CTrayChimesDlg::OnPlay30()
{
    ::sndPlaySound(m_str30Chime, SND_ASYNC);
}

void CTrayChimesDlg::OnPlay45()
{
    ::sndPlaySound(m_str45Chime, SND_ASYNC);
}

void CTrayChimesDlg::OnPlayHour()
{
    ::sndPlaySound(m_strHourChime, SND_ASYNC);
}

void CTrayChimesDlg::OnPlayAlarm()
{
    ::sndPlaySound(m_strAlarmChime, SND_ASYNC);
}

void CTrayChimesDlg::On15Browse()
{
    CFileDialog dlg(TRUE, L"*.wav", m_str15Chime, OFN_FILEMUSTEXIST,
        L"Sound Files (*.wav)|*.wav", this);
    if (dlg.DoModal() == IDOK)
    {
        m_str15Chime = dlg.GetPathName();
        m_AccessRegistry = FALSE;
        UpdateData(FALSE);
        m_AccessRegistry = TRUE;
    }
}

void CTrayChimesDlg::On30Browse()
{
    CFileDialog dlg(TRUE, L"*.wav", m_str30Chime, OFN_FILEMUSTEXIST,
        L"Sound Files (*.wav)|*.wav", this);
    if (dlg.DoModal() == IDOK)
    {
        m_str30Chime = dlg.GetPathName();
        m_AccessRegistry = FALSE;
        UpdateData(FALSE);
        m_AccessRegistry = TRUE;
    }
}

void CTrayChimesDlg::On45Browse()
{
    CFileDialog dlg(TRUE, L"*.wav", m_str45Chime, OFN_FILEMUSTEXIST,
        L"Sound Files (*.wav)|*.wav", this);
    if (dlg.DoModal() == IDOK)
    {
        m_str45Chime = dlg.GetPathName();
        m_AccessRegistry = FALSE;
        UpdateData(FALSE);
        m_AccessRegistry = TRUE;
    }
}

void CTrayChimesDlg::OnBongBrowse()
{
    CFileDialog dlg(TRUE, L"*.wav", m_strHourChime, OFN_FILEMUSTEXIST,
        L"Sound Files (*.wav)|*.wav", this);
    if (dlg.DoModal() == IDOK)
    {
        m_strHourChime = dlg.GetPathName();
        m_AccessRegistry = FALSE;
        UpdateData(FALSE);
        m_AccessRegistry = TRUE;
    }
}

void CTrayChimesDlg::OnHourBrowse()
{
    CFileDialog dlg(TRUE, L"*.wav", m_str00Chime, OFN_FILEMUSTEXIST,
        L"Sound Files (*.wav)|*.wav", this);
    if (dlg.DoModal() == IDOK)
    {
        m_str00Chime = dlg.GetPathName();
        m_AccessRegistry = FALSE;
        UpdateData(FALSE);
        m_AccessRegistry = TRUE;
    }
}

void CTrayChimesDlg::OnAlarmBrowse()
{
    CFileDialog dlg(TRUE, L"*.wav", m_strAlarmChime, OFN_FILEMUSTEXIST,
        L"Sound Files (*.wav)|*.wav", this);
    if (dlg.DoModal() == IDOK)
    {
        m_strAlarmChime = dlg.GetPathName();
        m_AccessRegistry = FALSE;
        UpdateData(FALSE);
        m_AccessRegistry = TRUE;
    }
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
