

#ifndef WINDOW_FINDER_H
  #define WINDOW_FINDER_H

#include <windows.h>

long StartSearchWindowDialog (HWND hwndMain);

BOOL CheckWindowValidity (HWND hwndDialog, HWND hwndToCheck);

long DoMouseMove 
(
  HWND hwndDialog, 
  UINT message, 
  WPARAM wParam, 
  LPARAM lParam
);

long DoMouseUp
(
  HWND hwndDialog, 
  UINT message, 
  WPARAM wParam, 
  LPARAM lParam
);

BOOL SetFinderToolImage (HWND hwndDialog, BOOL bSet);

BOOL MoveCursorPositionToBullsEye (HWND hwndDialog);

long SearchWindow (HWND hwndDialog);

long DisplayInfoOnFoundWindow (HWND hwndDialog, HWND hwndFoundWindow);

long RefreshWindow (HWND hwndWindowToBeRefreshed);

long HighlightFoundWindow (HWND hwndDialog, HWND hwndFoundWindow);

BOOL CALLBACK SearchWindowDialogProc
(
  HWND hwndDlg, // handle to dialog box 
  UINT uMsg, // message 
  WPARAM wParam, // first message parameter 
  LPARAM lParam // second message parameter 
); 

#endif


