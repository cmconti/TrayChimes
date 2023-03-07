// TrayChimes.h : main header file for the TRAYCHIMES application
//

#pragma once

#ifndef __AFXWIN_H__
#error include 'pch.h' before including this file for PCH
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
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    // Implementation
    void SetSharedHWND(HWND hWnd);
    HWND GetSharedHWND();

    DECLARE_MESSAGE_MAP();

private:
    ULONG_PTR m_gdiplusToken;
};

/////////////////////////////////////////////////////////////////////////////
