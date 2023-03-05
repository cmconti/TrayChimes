// TrayChimes.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "TrayChimes.h"
#include "TrayChimesDlg.h"

#include <filesystem>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTrayChimesApp

BEGIN_MESSAGE_MAP(CTrayChimesApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// CTrayChimesApp construction

CTrayChimesApp::CTrayChimesApp()
{
    // support Restart Manager
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

// The one and only CTrayChimesApp object

HANDLE m_hMapObject = NULL;
static LPVOID m_lpvMem = NULL;

CTrayChimesApp theApp;

// CTrayChimesApp initialization

BOOL CTrayChimesApp::InitInstance()
{
    // Create a named file mapping object.
    m_hMapObject = CreateFileMapping(
        (HANDLE)0xFFFFFFFF, // use paging file
        NULL,                // no security attributes
        PAGE_READWRITE,      // read/write access
        0,                   // size: high 32-bits
        sizeof(HWND),           // size: low 32-bits
        L"TrayChimesHWND");    // name of map object

    if (m_hMapObject == NULL)
        return FALSE;

    // The first process to attach initializes memory.
    BOOL bFirstProcess = (GetLastError() != ERROR_ALREADY_EXISTS);

    // Get a pointer to the file-mapped shared memory.
    m_lpvMem = MapViewOfFile(
        m_hMapObject,     // object to map view of
        FILE_MAP_WRITE, // read/write access
        0,              // high offset:  map from
        0,              // low offset:   beginning
        0);             // default: map entire file
    if (m_lpvMem == NULL)
        return FALSE;

    // Initialize memory if this is the first process.
    if (bFirstProcess)
        memset(m_lpvMem, L'\0', sizeof(HWND));

    // otherwise,  get the HWND, show the window and exit
    else
    {
        HWND hWnd = GetSharedHWND();
        if (hWnd != NULL)
        {
            // if so, does it have any popups?
            HWND hWndChild = ::GetLastActivePopup(hWnd);

            // If iconic, restore the main window
            if (::IsIconic(hWnd))
                ::ShowWindow(hWnd, SW_RESTORE);

            // Bring the main window or its popup to
            // the foreground
            ::SetForegroundWindow(hWndChild);

            //make sure that the original instance is in the tray
            ::PostMessage(hWnd, WM_PSADDTOTRAY, 0, 0);
        }

        return FALSE;
    }

    // InitCommonControlsEx() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // Set this to include all the common control classes you want to use
    // in your application.
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();

    AfxEnableControlContainer();

    // Create the shell manager, in case the dialog contains
    // any shell tree view or shell list view controls.
    CShellManager* pShellManager = new CShellManager;

    // Activate "Windows Native" visual manager for enabling themes in MFC controls
    CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.
    SetRegistryKey(L"PretzelSoft");

    // Get full executable path
    wchar_t buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, _countof(buffer));

    // Get executable directory
    std::filesystem::path path(buffer);
    path = path.parent_path();

    // Set current path to that directory
    std::filesystem::current_path(path);

    CTrayChimesDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();

    if (nResponse == -1)
    {
        TRACE(traceAppMsg, 0, L"Warning: dialog creation failed, so application is terminating unexpectedly.\n");
        TRACE(traceAppMsg, 0, L"Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
    }

    // Delete the shell manager created above.
    if (pShellManager != nullptr)
    {
        delete pShellManager;
    }

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
    ControlBarCleanUp();
#endif

    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    return TRUE;
}

int CTrayChimesApp::ExitInstance()
{
    // Unmap shared memory from the process's address space.
    ::UnmapViewOfFile(m_lpvMem);
    // Close the process's handle to the file-mapping object.
    ::CloseHandle(m_hMapObject);

    return CWinApp::ExitInstance();
}

void CTrayChimesApp::SetSharedHWND(HWND hWnd)
{
    HWND* pHWND;

    // Get the address of the shared memory block.
    pHWND = (HWND*)m_lpvMem;

    // Copy the HWND into shared memory.
    *pHWND = hWnd;
}

HWND CTrayChimesApp::GetSharedHWND()
{
    HWND* pHWND;

    // Get the address of the shared memory block.
    pHWND = (HWND*)m_lpvMem;

    // return the shared HWND.
    return *pHWND;
}
