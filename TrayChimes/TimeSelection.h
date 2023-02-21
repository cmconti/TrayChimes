// TimeSelection.h : header file
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CNumEdit window

class CNumEdit : public CEdit
{
    // Construction
public:
    CNumEdit();

    // Attributes
public:

    void SetRange(int min, int max) { m_nMinValue = min; m_nMaxValue = max; }

    int GetValue() { return m_nNumber; }
    void SetValue(int nVal);

    // Operations
public:
    virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID, BOOL bLeadingZeros);

    // Overrides

    // Implementation
public:
    virtual ~CNumEdit();

    // Generated message map functions
protected:
    afx_msg void OnUpdate();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP();

    CString m_strLastValue;

    BOOL m_bLeadingZeros;
    int m_nNumber;
    int m_nMinValue;
    int m_nMaxValue;

    BOOL m_bKeyInput;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CTimeSelection window

class CTimeSelection : public CWnd
{
    // Construction
public:
    CTimeSelection();

    // Attributes
public:

    // Operations
public:
    virtual BOOL Create(LPCTSTR lpszWindowName, const RECT& rect, CWnd* pParentWnd, UINT nID);

    int GetHour(); //0 - 23
    int GetMinute(); //0 - 59

    void SetHour(int Hour);
    void SetMinute(int Minute);

    // Overrides
        // ClassWizard generated virtual function overrides
protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

    // Implementation
public:
    virtual ~CTimeSelection();

    // Generated message map functions
protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSetfocusHourEdit();
    afx_msg void OnSetfocusMinuteEdit();
    afx_msg void OnSetfocusAMPMList();
    afx_msg void OnDeltaposSpinAMPM(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnPaint();
    afx_msg UINT OnGetDlgCode();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    DECLARE_MESSAGE_MAP();

    CFont m_Font;
    CBrush m_brStaticBackground;

    CNumEdit m_ctlHour;
    CStatic m_ctlHourSeparator;
    CNumEdit m_ctlminute;
    CListBox m_ctlAMPM;

    CSpinButtonCtrl	m_ctrlSpinMinute;
    CSpinButtonCtrl	m_ctrlSpinHour;
    CSpinButtonCtrl	m_ctrlSpinAMPM;

    CRect m_rectTime;
    int m_nInitialHour;
    int m_nInitialMinute;
};

/////////////////////////////////////////////////////////////////////////////
