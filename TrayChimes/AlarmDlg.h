// AlarmDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlarmDlg dialog

class CAlarmDlg : public CDialog
{
// Construction
public:
	CAlarmDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlarmDlg)
	enum { IDD = IDD_DISPLAY_MESSAGE };
	CStatic	m_Message;
	CString	m_strMessage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlarmDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlarmDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSnooze();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
