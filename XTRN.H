/**************************************************************************
 *  File name  :  xtrn.h
 *
 *  Description:  This header file contains the application-wide function
 *                declarations
 *************************************************************************/
/* from dlg.c */
MRESULT EXPENTRY ProductInfoDlgProc(HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2);
VOID SetSysMenu( HWND hDlg );

/* from help.c */
VOID InitHelp(HAB hab, HWND hwndMain);
VOID HelpIndex(VOID);
VOID HelpGeneral(VOID);
VOID HelpUsingHelp(VOID);
VOID HelpKeys(VOID);
VOID HelpTutorial(VOID);
VOID HelpProductInfo(HWND hwndMain);
VOID DisplayHelpPanel(ULONG idPanel);
VOID DestroyHelpInstance(VOID);

/* from main.c */
MRESULT EXPENTRY SubFrameProc(HWND, ULONG, MPARAM, MPARAM);

/* from messages.c */
VOID UserCommand(HWND hwnd, MPARAM mp1, MPARAM mp2);

/* from pmvdprocs.c */
MRESULT InitMainWindow(HWND hwnd, MPARAM mp1, MPARAM mp2);
BOOL SetCascadeDefault(HWND hwnd,USHORT usSubmenu,USHORT usDefault);
VOID FrameControls (HWND hwndMain, BOOL fHide, BOOL fInit);
VOID HideFrameControls (HWND hwndMain);
VOID ShowFrameControls (HWND hwndMain);
VOID InitMenu(MPARAM mp1, MPARAM mp2);
VOID EnableMenuItem(HWND hwndMenu, USHORT idItem, BOOL fEnable);
VOID CheckMenuItem(HWND hwndMenu, SHORT idItem, BOOL fEnable);
VOID MainSize(HWND hwnd, MPARAM mp2);
VOID MoveWindows(HWND hwnd, LONG x, LONG y);
VOID MoveActiveWindows(HWND hwnd, HWND hwndCurrent);
VOID MoveWindowsVisible(HWND hwnd);
