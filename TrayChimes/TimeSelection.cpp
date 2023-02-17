// TimeSelection.cpp : implementation file
//

#include "stdafx.h"
#include "TimeSelection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT  IDC_HOUR = WM_APP + 100;
UINT  IDC_MINUTE = WM_APP + 101;
UINT  IDC_SEPARATOR = WM_APP + 102;
UINT  IDC_AMPM = WM_APP + 103;
UINT  IDC_SPIN_HOUR = WM_APP + 104;
UINT  IDC_SPIN_MINUTE = WM_APP + 105;
UINT  IDC_SPIN_AMPM = WM_APP + 106;

/////////////////////////////////////////////////////////////////////////////
// CTimeSelection

CTimeSelection::CTimeSelection()
{
    m_nInitialHour = CTime::GetCurrentTime().GetHour();
    m_nInitialMinute = CTime::GetCurrentTime().GetMinute();
}

CTimeSelection::~CTimeSelection()
{
}


BEGIN_MESSAGE_MAP(CTimeSelection, CWnd)
	//{{AFX_MSG_MAP(CTimeSelection)
	ON_WM_CREATE()
	ON_EN_SETFOCUS(IDC_HOUR, OnSetfocusHourEdit)
	ON_EN_SETFOCUS(IDC_MINUTE, OnSetfocusMinuteEdit)
	ON_LBN_SETFOCUS(IDC_AMPM, OnSetfocusAMPMList)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_AMPM, OnDeltaposSpinAMPM)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_GETDLGCODE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimeSelection message handlers

BOOL CTimeSelection::Create(LPCTSTR lpszWindowName, const RECT& rect, CWnd* pParentWnd, UINT nID) 
{

    DWORD dwStyle = WS_VISIBLE | WS_BORDER | WS_CHILD;

	return CWnd::Create(NULL, lpszWindowName, dwStyle, rect, pParentWnd, nID, NULL);
}

BOOL CTimeSelection::PreCreateWindow(CREATESTRUCT& cs) 
{
    cs.cx = 75;
    cs.cy = 17;

    cs.style = WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_CLIPCHILDREN;
    cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	
	return CWnd::PreCreateWindow(cs);
}

int CTimeSelection::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

    m_Font.CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE,
        FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS,
        PROOF_QUALITY, DEFAULT_PITCH | TMPF_TRUETYPE | FF_MODERN, "Arial" );

    LOGFONT lf;
    m_Font.GetLogFont(&lf);
    int Height = lf.lfHeight + 2;
    int Width = 8;

    m_brStaticBackground.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));

    int Hour = m_nInitialHour % 12;
    if (Hour == 0)
        Hour = 12;
    int Minute = m_nInitialMinute;

    m_ctlHour.SetRange(1,12);
    m_ctlHour.SetValue(Hour);

    m_ctlminute.SetRange(0,59);
    m_ctlminute.SetValue(Minute);

    CRect rectHour(2, 2, Width*2 + 2, Height + 1);
    CRect rectSeparator(Width*2 + 2, 2, Width*2 + 6, Height + 1);
    CRect rectMinute(Width*2 + 6, 2, Width*4 + 6, Height + 1);
    CRect rectAMPM(Width*4 + 6, 2, Width*6 + 10, Height + 1);
    CRect rectSpin(Width*6 + 12, -1, Width*6 + 22, Height + 1);

    m_rectTime.SetRect(0,0,Width*6 + 12,Height+3);

    SetWindowPos(NULL, 0, 0, Width*6+32/*28*/, Height + 8/*4*/, SWP_NOMOVE | SWP_NOZORDER);

    m_ctlHour.Create(rectHour, this, IDC_HOUR, FALSE);
    m_ctrlSpinHour.Create(WS_CHILD | WS_VISIBLE | UDS_WRAP | 
                    UDS_SETBUDDYINT | UDS_AUTOBUDDY | UDS_ARROWKEYS | 
                    UDS_NOTHOUSANDS, rectSpin, this, IDC_SPIN_HOUR);

    m_ctlHourSeparator.Create(":", WS_CHILD | WS_VISIBLE | SS_CENTER, rectSeparator,
        this, IDC_SEPARATOR);

    m_ctlminute.Create(rectMinute, this, IDC_MINUTE, TRUE);
    m_ctrlSpinMinute.Create(WS_CHILD | UDS_WRAP | 
                    UDS_SETBUDDYINT | UDS_AUTOBUDDY | UDS_ARROWKEYS | 
                    UDS_NOTHOUSANDS, rectSpin, this, IDC_SPIN_MINUTE);

    m_ctlAMPM.Create(WS_VISIBLE | WS_CHILD | LBS_NOINTEGRALHEIGHT | LBS_NOSEL,
                    rectAMPM, this, IDC_AMPM);
    m_ctrlSpinAMPM.Create(WS_CHILD | UDS_WRAP | 
                    UDS_AUTOBUDDY | UDS_ARROWKEYS,
                    rectSpin, this, IDC_SPIN_AMPM);

    m_ctlHour.SetFont(&m_Font);
    m_ctlHourSeparator.SetFont(&m_Font);
    m_ctlminute.SetFont(&m_Font);
    m_ctlAMPM.SetFont(&m_Font);

    m_ctlAMPM.AddString("AM");
    m_ctlAMPM.AddString("PM");

    if (m_nInitialHour < 12)
        m_ctlAMPM.SetCurSel(0);
    else
        m_ctlAMPM.SetCurSel(1);

    m_ctrlSpinHour.SetRange(1,12);
    m_ctrlSpinHour.SetBase(10);
    m_ctrlSpinHour.SetPos(Hour);

    m_ctrlSpinMinute.SetRange(0,59);
    m_ctrlSpinMinute.SetBase(10);
    m_ctrlSpinMinute.SetPos(Minute);

    m_ctrlSpinAMPM.SetRange(0,1);
    m_ctrlSpinAMPM.SetBase(10);
    m_ctrlSpinAMPM.SetPos(m_ctlAMPM.GetCurSel());

    return 0;
}
void CTimeSelection::OnPaint() 
{
    TRACE ("Paint\n");
	CPaintDC dc(this); // device context for painting

    CRect rect(m_rectTime);
    rect.InflateRect(1,1,0,0);
	dc.DrawEdge(rect, EDGE_SUNKEN, BF_RECT);

}
void CTimeSelection::OnSetfocusHourEdit() 
{
	m_ctrlSpinHour.ShowWindow(SW_SHOW);	
	m_ctrlSpinMinute.ShowWindow(SW_HIDE);	
	m_ctrlSpinAMPM.ShowWindow(SW_HIDE);	
}

void CTimeSelection::OnSetfocusMinuteEdit() 
{
	m_ctrlSpinMinute.ShowWindow(SW_SHOW);	
	m_ctrlSpinHour.ShowWindow(SW_HIDE);	
	m_ctrlSpinAMPM.ShowWindow(SW_HIDE);	
}

void CTimeSelection::OnSetfocusAMPMList() 
{
	m_ctrlSpinAMPM.ShowWindow(SW_SHOW);	
	m_ctrlSpinMinute.ShowWindow(SW_HIDE);	
	m_ctrlSpinHour.ShowWindow(SW_HIDE);	
}

void CTimeSelection::OnDeltaposSpinAMPM(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    int NewValue = (m_ctlAMPM.GetCurSel() == 0) ? 1 : 0;
    m_ctlAMPM.SetCurSel(NewValue);
	
	*pResult = 0;
}

int CTimeSelection::GetHour()
{
	CString strText;
    m_ctlHour.GetWindowText(strText);

    int nValue = atoi(strText);
    int nAMPM = m_ctlAMPM.GetCurSel();

    if (nValue == 12)
        nValue = 0;

    nValue = nValue + (12 * nAMPM);

    return nValue;
}

int CTimeSelection::GetMinute()
{
	CString strText;
    m_ctlminute.GetWindowText(strText);

    int nValue = atoi(strText);

    return nValue;
}

void CTimeSelection::SetHour(int Hour)
{
    m_nInitialHour = Hour; //in case ctrl not created yet

    if (m_ctlAMPM.GetSafeHwnd() != NULL)
    {
        if (Hour > 11)
        {
            m_ctlAMPM.SetCurSel(1); //PM
        }
        else
        {
            m_ctlAMPM.SetCurSel(0); //AM
        }
    }

    Hour = Hour % 12;
    if (Hour == 0)
        Hour = 12;

    m_ctlHour.SetValue(Hour);
}

void CTimeSelection::SetMinute(int Minute)
{
    m_nInitialMinute = Minute; //in case ctrl not created yet
    m_ctlminute.SetValue(Minute);
}

HBRUSH CTimeSelection::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC)
    {
        hbr = m_brStaticBackground;
        pDC->SetBkMode(OPAQUE);
        pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
    }

	return hbr;
}

UINT CTimeSelection::OnGetDlgCode() 
{
	UINT uRet = CWnd::OnGetDlgCode();
    uRet |= DLGC_WANTTAB; //so that TAB key will stop here 
	
	return uRet;
}

void CTimeSelection::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	m_ctlHour.SetFocus();	
}

/////////////////////////////////////////////////////////////////////////////
// CNumEdit

CNumEdit::CNumEdit()
{
    m_nNumber = -1;
    m_bLeadingZeros = FALSE;
    m_bKeyInput = FALSE;
}

CNumEdit::~CNumEdit()
{
}


BEGIN_MESSAGE_MAP(CNumEdit, CEdit)
	//{{AFX_MSG_MAP(CNumEdit)
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_SETTEXT, OnSetText)

END_MESSAGE_MAP()

LRESULT CNumEdit::OnSetText(WPARAM wParam, LPARAM lParam)
{
    TRACE("Set Text\n");
    Default();
    OnUpdate();
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CNumEdit message handlers
void CNumEdit::OnUpdate() 
{
    TRACE("On Update\n");
	CString strText;
    GetWindowText(strText);

    int nValue = atoi(strText);
    if (nValue != m_nNumber)
    {
        if ((nValue < m_nMinValue || nValue > m_nMaxValue) && !strText.IsEmpty())
        {
            SetWindowText(m_strLastValue);
        }
        else
        {
            m_nNumber = nValue;
            if (m_bLeadingZeros)
            {
                if (!m_bKeyInput)
                {
                    ASSERT(!strText.IsEmpty());
                    m_bKeyInput = FALSE;
                    if ((nValue == 0) && (strText != "00"))
                    {
                        SetWindowText("00");
                    }
                    else if ((nValue < 10) && (strText[0] != '0'))
                    {
                        strText.Format("%02d", m_nNumber);
                        SetWindowText(strText);
                    }
                }
            }
        }
    }
    else if (m_nNumber == 0 && m_bLeadingZeros) //extra check because of initialization
    {
        if (!m_bKeyInput)
        {
            ASSERT(!strText.IsEmpty());
            m_bKeyInput = FALSE;
            if ((nValue == 0) && (strText != "00"))
            {
                SetWindowText("00");
            }
            else if ((nValue < 10) && (strText[0] != '0'))
            {
                strText.Format("%02d", m_nNumber);
                SetWindowText(strText);
            }
        }
    }
}

BOOL CNumEdit::Create(const RECT& rect, CWnd* pParentWnd, UINT nID, BOOL bLeadingZeros) 
{
    m_bLeadingZeros = bLeadingZeros;

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_TABSTOP |
                    ES_MULTILINE | ES_RIGHT | ES_NUMBER;
	
	return CEdit::Create(dwStyle, rect, pParentWnd, nID);
}

int CNumEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    CString str;
    if (m_bLeadingZeros)
    {
        if (m_nNumber == 0)
        {
            SetWindowText("00");
        }
        else if (m_nNumber < 10)
        {
            str.Format("%02d", m_nNumber);
            SetWindowText(str);
        }
    }
    else
    {
        str.Format("%d", m_nNumber);
        SetWindowText(str);
    }

	return 0;
}

void CNumEdit::SetValue( int nVal )
{
    if (nVal >= m_nMinValue && nVal <= m_nMaxValue)
    {
        m_nNumber = nVal;

        if (GetSafeHwnd() != NULL)
        {
            CString strValue;
            if (m_bLeadingZeros && (m_nNumber != 0))
            {
                strValue.Format("%02d", m_nNumber);
                SetWindowText(strValue);
            }
            if (m_bLeadingZeros && (m_nNumber == 0))
            {
                SetWindowText("00");
            }
            else
            {
                strValue.Format("%d", m_nNumber);
                SetWindowText(strValue);
            }
        }
    }
}

void CNumEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    GetWindowText(m_strLastValue); //save current text in case we need to revert

    TRACE("Key Down\n");
    m_bKeyInput = TRUE;
  	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);

    if (nChar == VK_TAB)
    {
        CDialog* pDlg = NULL;
        CWnd* pParent = GetParent();
        if (pParent != NULL)
            pDlg = (CDialog*)pParent->GetParent();

        if (pDlg != NULL)
        {
            //Is the shift key pressed also? 
            if (GetKeyState(VK_SHIFT) & 0x8000) 
                pDlg->PrevDlgCtrl();
            else 
                pDlg->NextDlgCtrl();
        }
    }
}

BOOL CTimeSelection::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	UINT idEditCtrl = (UINT) LOWORD(wParam); // identifier of edit control 
    UINT nCmd = (UINT) HIWORD(wParam);
    HWND hwndEditCtrl = (HWND) lParam;      // handle of edit control 

    if (nCmd == EN_UPDATE)
        TRACE("update recieved\n");

	
	return CWnd::OnCommand(wParam, lParam);
}

