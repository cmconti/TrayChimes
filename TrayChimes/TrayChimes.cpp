// TrayChimes.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TrayChimes.h"
#include "TrayChimesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrayChimesApp

BEGIN_MESSAGE_MAP(CTrayChimesApp, CWinApp)
	//{{AFX_MSG_MAP(CTrayChimesApp)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrayChimesApp construction

CTrayChimesApp::CTrayChimesApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTrayChimesApp object

HANDLE m_hMapObject = NULL;
static LPVOID m_lpvMem = NULL;

CTrayChimesApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTrayChimesApp initialization

BOOL CTrayChimesApp::InitInstance()
{
    // Create a named file mapping object.
    m_hMapObject = CreateFileMapping( 
                            (HANDLE) 0xFFFFFFFF, // use paging file
                            NULL,                // no security attributes
                            PAGE_READWRITE,      // read/write access
                            0,                   // size: high 32-bits
                            sizeof(HWND),           // size: low 32-bits
                            "myuniquename");    // name of map object

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
        memset(m_lpvMem, '\0', sizeof(HWND));

    // otherwise,  get the HWND, show the window and exit
    else
    {
        HWND hWnd = GetSharedHWND();
        if (hWnd != NULL)
        {
            // if so, does it have any popups?
            HWND hWndChild = ::GetLastActivePopup(hWnd) ;

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

    // Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

    SetRegistryKey("PretzelSoft");

	CTrayChimesDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();

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
