
#ifndef MAIN_H
  #define MAIN_H

#include <windows.h>

extern HINSTANCE	g_hInst;
extern HWND			g_hwndMainWnd;
extern HANDLE		g_hApplicationMutex;
extern DWORD		g_dwLastError;
extern BOOL			g_bStartSearchWindow;
extern HCURSOR		g_hCursorSearchWindow;
extern HCURSOR		g_hCursorPrevious;
extern HBITMAP		g_hBitmapFinderToolFilled;
extern HBITMAP		g_hBitmapFinderToolEmpty;
extern HWND			g_hwndFoundWindow;
extern HPEN			g_hRectanglePen;

#define WINDOW_FINDER_MAIN_WINDOW_TITLE	"Window Finder"
#define WINDOW_FINDER_APP_MUTEX_NAME	"WINDOWFINDERMUTEX"
#define WM_START_SEARCH_WINDOW			WM_USER + 100
#define WINDOW_FINDER_TOOL_TIP			"Window Finder"
#define ABOUT_WINDOW_FINDER				"Window Finder\r\n(C) 2001 Lim Bio Liong."

BOOL InitializeApplication
(
  HINSTANCE hThisInst, 
  HINSTANCE hPrevInst, 
  LPTSTR lpszArgs, 
  int nWinMode
);

BOOL UninitializeApplication ();

BOOL InitialiseResources();

BOOL UninitialiseResources();

int APIENTRY WinMain
(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR     lpCmdLine,
  int       nCmdShow
);

LRESULT CALLBACK MainWndProc
(
  HWND hwnd, 
  UINT message, 
  WPARAM wParam, 
  LPARAM lParam
);

#endif

