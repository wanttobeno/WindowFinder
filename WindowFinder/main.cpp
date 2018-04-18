
#include "main.h"
#include "WindowFinder.h"
#include "resource.h"





char		szMainWindowClassName[] = "WindowFinderMainWindow"; // Name of the Main Window class.
HINSTANCE	g_hInst = NULL;
HWND		g_hwndMainWnd = NULL;
HANDLE		g_hApplicationMutex = NULL;
DWORD		g_dwLastError = 0;
BOOL		g_bStartSearchWindow = FALSE;
HCURSOR		g_hCursorSearchWindow = NULL;
HCURSOR		g_hCursorPrevious = NULL;
HBITMAP		g_hBitmapFinderToolFilled;
HBITMAP		g_hBitmapFinderToolEmpty;
HWND		g_hwndFoundWindow = NULL;
HPEN		g_hRectanglePen = NULL;





BOOL InitializeApplication
(
  HINSTANCE hThisInst, 
  HINSTANCE hPrevInst, 
  LPTSTR lpszArgs, 
  int nWinMode
)
{ 
  WNDCLASS wc; 
  BOOL bRetTemp = FALSE;
  long lRetTemp = 0;
  BOOL bRet = FALSE;

  // Create the application mutex so that no two instances of this app can run.
  g_hApplicationMutex = CreateMutex
  (
    (LPSECURITY_ATTRIBUTES)NULL, // pointer to security attributes 
    (BOOL)TRUE, // flag for initial ownership 
    (LPCTSTR)WINDOW_FINDER_APP_MUTEX_NAME // pointer to mutex-object name 
  );

  g_dwLastError = GetLastError ();

  // If cannot create Mutex, exit.
  if (g_hApplicationMutex == NULL)
  {
    bRet = FALSE;
	goto InitializeApplication_0;
  }

  // If Mutex already existed, exit.
  if (g_dwLastError == ERROR_ALREADY_EXISTS)
  {
    CloseHandle (g_hApplicationMutex);
	g_hApplicationMutex = NULL;
    bRet = FALSE;
	goto InitializeApplication_0;
  }

  // Register the frame window class. 
  memset (&wc, 0, sizeof(WNDCLASS));

  // Register the main window class. 
  wc.style			= CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc	= (WNDPROC)MainWndProc; 
  wc.hInstance		= hThisInst;
  wc.hIcon			= LoadIcon((HINSTANCE)NULL, IDI_WINLOGO); 
  wc.hCursor		= LoadCursor ((HINSTANCE)NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH); 
  wc.lpszMenuName  = (LPCTSTR) MAKEINTRESOURCE(IDR_MENU_MAIN); 
  wc.lpszClassName = szMainWindowClassName; 
 
  if (!RegisterClass(&wc)) 
  {
    bRet = FALSE; 
	goto InitializeApplication_0;
  }

  // All went well, return a TRUE.
  bRet = TRUE;

InitializeApplication_0:
 
  return bRet; 
}





BOOL UninitializeApplication ()
{
  BOOL bRet = FALSE;

  bRet = UnregisterClass
  (
    (LPCTSTR)szMainWindowClassName, // address of class name string 
    (HINSTANCE)g_hInst // handle of application instance 
  );

  if (g_hApplicationMutex)
  {
    ReleaseMutex (g_hApplicationMutex);
    CloseHandle (g_hApplicationMutex);
	g_hApplicationMutex = NULL;
  }

  return bRet;
}





BOOL InitialiseResources()
{
  BOOL bRet = FALSE;

  g_hCursorSearchWindow = LoadCursor (g_hInst, MAKEINTRESOURCE(IDC_CURSOR_SEARCH_WINDOW));
  if (g_hCursorSearchWindow == NULL)
  {
    bRet = FALSE;
	goto InitialiseResources_0;
  }

  g_hRectanglePen = CreatePen (PS_SOLID, 3, RGB(256, 0, 0));
  if (g_hRectanglePen == NULL)
  {
    bRet = FALSE;
	goto InitialiseResources_0;
  }

  g_hBitmapFinderToolFilled = LoadBitmap (g_hInst, MAKEINTRESOURCE(IDB_BITMAP_FINDER_FILLED));
  if (g_hBitmapFinderToolFilled == NULL)
  {
    bRet = FALSE;
	goto InitialiseResources_0;
  }

  g_hBitmapFinderToolEmpty = LoadBitmap (g_hInst, MAKEINTRESOURCE(IDB_BITMAP_FINDER_EMPTY));
  if (g_hBitmapFinderToolEmpty == NULL)
  {
    bRet = FALSE;
	goto InitialiseResources_0;
  }

  // All went well. Return TRUE.
  bRet = TRUE;

InitialiseResources_0:

  return bRet;
}





BOOL UninitialiseResources()
{
  BOOL bRet = TRUE;

  if (g_hCursorSearchWindow)
  {
    // No need to destroy g_hCursorSearchWindow. It was not created using 
	// CreateCursor().
  }

  if (g_hRectanglePen)
  {
    bRet = DeleteObject (g_hRectanglePen);
	g_hRectanglePen = NULL;
  }

  if (g_hBitmapFinderToolFilled)
  {
	DeleteObject (g_hBitmapFinderToolFilled);
	g_hBitmapFinderToolFilled = NULL;
  }

  if (g_hBitmapFinderToolEmpty)
  {
	DeleteObject (g_hBitmapFinderToolEmpty);
	g_hBitmapFinderToolEmpty = NULL;
  }
    
  return bRet;
}





int APIENTRY WinMain
(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR     lpCmdLine,
  int       nCmdShow
)
{
 	// TODO: Place code here.
  MSG		msg;
  BOOL		bRet = FALSE;
  int		iRet = 0;
  DWORD		dwStyle = 0;

  g_hInst = hInstance;

  bRet = InitializeApplication (hInstance, hPrevInstance, lpCmdLine, nCmdShow);
  if (bRet == FALSE)
  {
    // Failure to initialise application.
	// Exit immediately.
	iRet = 0;
    goto WinMain_0;
  }

  bRet = InitialiseResources();
  if (bRet == FALSE)
  {
    // Failure to initialise resources
	// Exit immediately.
	iRet = 0;
    goto WinMain_0;
  }


  dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

  // Create the main window.
  g_hwndMainWnd = CreateWindow
  (
    szMainWindowClassName,     // name of window class 
    WINDOW_FINDER_MAIN_WINDOW_TITLE,  // title 
    dwStyle, // window style - normal 
    CW_USEDEFAULT, // X coordinate - let Windows decide 
    CW_USEDEFAULT, // Y coordinate - let Windows decide 
    CW_USEDEFAULT, // width - let Windows decide 
    CW_USEDEFAULT, // height - let Windows decide
    NULL,          // no parent window 
    NULL,          // no override of class menu 
    hInstance,     // handle for this instance
    NULL           // no additional arguments 
  );

  if (g_hwndMainWnd == NULL)
  {
    // Failure to create Main Frame Window.
	// Exit immediately.
	iRet = 0;
    goto WinMain_0;
  }

  // Display the window. 
  ShowWindow(g_hwndMainWnd, SW_SHOWNORMAL);
  UpdateWindow(g_hwndMainWnd);

  // Create the message loop. 
  while (GetMessage(&msg, (HWND)NULL, 0, 0)) 
  { 
    if (!TranslateAccelerator(g_hwndMainWnd, NULL, &msg))
    { 
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    } 
  }

  iRet = msg.wParam;

WinMain_0:

  UninitializeApplication();

  UninitialiseResources();

  return iRet;
}





// The window procedure.
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  BOOL bCallDefWndProc = FALSE;
  long lRet = 0;

  switch(message) 
  {
	case WM_COMMAND:
	{
	  WORD wNotifyCode = HIWORD(wParam); // notification code 
      WORD wID = LOWORD(wParam);         // item, control, or accelerator identifier 
      HWND hwndCtl = (HWND)lParam;      // handle of control 

	  if (wNotifyCode == 0)
	  {
	    // Message is from a menu.
		if (wID == IDM_ABOUT)
		{
		  MessageBox (NULL, ABOUT_WINDOW_FINDER, "Window Finder", MB_OK);
		}

		if (wID == IDM_FIND_WINDOW)
		{
		  PostMessage (hwnd, WM_START_SEARCH_WINDOW, 0, 0);
		}

		if (wID == IDM_EXIT)
		{
		  PostMessage (hwnd, WM_CLOSE, 0, 0);
		}

        lRet = 0;
		bCallDefWndProc = FALSE;
	  }
	  else
	  {
	    bCallDefWndProc = TRUE;
	  }
 
	  break;
	}

    case WM_DESTROY: // terminate the program 
	{
      lRet = 0;
	  bCallDefWndProc = FALSE;
      PostQuitMessage(0);
      break;
	}

	case WM_START_SEARCH_WINDOW :
	{
      lRet = 0;
	  bCallDefWndProc = FALSE;

	  StartSearchWindowDialog (hwnd);

	  break;
	}

	default:
	{
      // Let Windows process any messages not specified in the preceding switch statement.
	  bCallDefWndProc = TRUE;
	  break;
	}
  }

  if (bCallDefWndProc)
  {
      return DefWindowProc
	  (
	    (HWND)hwnd, // handle to main frame window 
        (UINT)message, // message 
        (WPARAM)wParam, // first message parameter 
        (LPARAM)lParam // second message parameter 
      ); 
  }
  else
  {
    return lRet;
  }
}

