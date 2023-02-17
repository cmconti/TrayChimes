// TrayChimes.h : main header file for the TRAYCHIMES application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTrayChimesApp:
// See TrayChimes.cpp for the implementation of this class
//

class CTrayChimesApp : public CWinApp
{
public:
	CTrayChimesApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrayChimesApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
    void SetSharedHWND(HWND hWnd);
    HWND GetSharedHWND();

	//{{AFX_MSG(CTrayChimesApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
