/**************************************************************************
 *  File name  :  help.c
 *
 *  Description:  This module contains all the routines for interfacing
 *                the IPF Help Manager.
 *
 *                This source file contains the following functions:
 *
 *                InitHelp()
 *                HelpIndex()
 *                HelpGeneral()
 *                HelpUsingHelp()
 *                HelpKeys()
 *                HelpTutorial()
 *                HelpProductInfo()
 *                DisplayHelpPanel(idPanel)
 *                DestroyHelpInstance()
 *************************************************************************/

/*
 *  Include files, macros, defined constants, and externs
 */

#define  INCL_WINHELP

#include <os2.h>
#include <string.h>
#include "pmvd.h"
#include "strings.h"
#include "xtrn.h"

/* If DEBUG is defined, then the help panels will display their
 *  id values on their title bar.  This is useful for determining
 *  which help panels are being shown for each dialog item.  When
 *  the DEBUG directive is not defined, then the panel ids are not
 *  displayed.
 */
/*
#define  DEBUG
*/

/*
 *  Global variables
 */
BOOL fHelpEnabled;
static HWND hwndHelpInstance;

/*
 *  Entry point declarations
 */

/**************************************************************************
 *  Name       : InitHelp()
 *
 *  Description:  Initializes the IPF help facility
 *
 *  Concepts:     Called once during initialization of the program
 *
 *                Initializes the HELPINIT structure and creates the
 *                help instance.  If successful, the help instance
 *                is associated with the main window.
 *************************************************************************/
VOID InitHelp(HAB hab, HWND hwndMain)
{
   HELPINIT hini;

   /* if we return because of an error, Help will be disabled */
   fHelpEnabled = FALSE;

   /* initialize help init structure */
   hini.cb = sizeof(HELPINIT);
   hini.ulReturnCode = 0;

   hini.pszTutorialName = (PSZ)NULL;   /* if tutorial added, add name here */

   hini.phtHelpTable = (PHELPTABLE)MAKELONG(TEMPLATE_HELP_TABLE, 0xFFFF);
   hini.hmodHelpTableModule = 0;
   hini.hmodAccelActionBarModule = 0;
   hini.idAccelTable = 0;
   hini.idActionBar = 0;

   hini.pszHelpWindowTitle = (PSZ)SZ_HELPWINDOWTITLE;

   /* if debugging, show panel ids, else don't */
#ifdef DEBUG
   hini.fShowPanelId = CMIC_SHOW_PANEL_ID;
#else
   hini.fShowPanelId = CMIC_HIDE_PANEL_ID;
#endif

   hini.pszHelpLibraryName = (PSZ)SZ_HELPLIBRARYNAME;

   /* creating help instance */
   hwndHelpInstance = WinCreateHelpInstance(hab, &hini);

   if(hwndHelpInstance == NULLHANDLE || hini.ulReturnCode)
   {
	WinAlarm(HWND_DESKTOP, WA_ERROR);
	WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
		SZ_HELPLOADERROR,
		(PSZ)NULL,
		0,
		MB_OK | MB_ERROR);
	return;
   }

   /* associate help instance with main frame */
   if(!WinAssociateHelpInstance(hwndHelpInstance, hwndMain))
   {
	WinAlarm(HWND_DESKTOP, WA_ERROR);
	WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
		SZ_HELPLOADERROR,
		(PSZ)NULL,
		0,
		MB_OK | MB_ERROR);
	return;
   }

   /* help manager is successfully initialized so set flag to TRUE */
   fHelpEnabled = TRUE;
}   /* End of InitHelp   */

/**************************************************************************
 *  Name       : HelpGeneral()
 *
 *  Description: Processes the WM_COMMAND message posted by the
 *               General Help item of the Help menu.
 *
 *  Concepts:    Called from UserCommand when the General Help
 *               menu item is selected.
 *
 *               Sends an HM_EXT_HELP message to the help
 *               instance so that the default Extended Help is
 *               displayed.
 *************************************************************************/
VOID  HelpGeneral(VOID)
{
    /* this just displays the system extended help panel */
   if(fHelpEnabled)
      if(NULL != WinSendMsg(hwndHelpInstance, HM_EXT_HELP,
                            (MPARAM)NULL, (MPARAM)NULL))
	WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
		SZ_HELPDISPLAYERROR,
		(PSZ)NULL,
		0,
		MB_OK | MB_ERROR);
}   /* End of HelpGeneral  */

/**************************************************************************
 *  Name       : HelpUsingHelp()
 *
 *  Description: Processes the WM_COMMAND message posted by the
 *               Using Help item of the Help menu.
 *
 *  Concepts:    Called from UserCommand when the Using Help
 *               menu item is selected.
 *
 *               Sends an HM_DISPLAY_HELP message to the help
 *               instance so that the default Using Help is
 *               displayed.
 *************************************************************************/
VOID  HelpUsingHelp(VOID)
{
   /* this just displays the system help for help panel */
   if(fHelpEnabled)
      if(NULL != WinSendMsg(hwndHelpInstance, HM_DISPLAY_HELP,
                            (MPARAM)NULL, (MPARAM)NULL))
	WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
		SZ_HELPDISPLAYERROR,
		(PSZ)NULL,
		0,
		MB_OK | MB_ERROR);
}   /* End of HelpUsingHelp   */

/**************************************************************************
 *  Name       : HelpKeys()
 *
 *  Description: Processes the WM_COMMAND message posted by the
 *               Keys Help item of the Help menu.
 *
 *  Concepts:    Called from UserCommand when the Keys Help
 *               menu item is selected.
 *
 *               Sends an HM_KEYS_HELP message to the help
 *               instance so that the default Keys Help is
 *               displayed.
 *************************************************************************/
VOID  HelpKeys(VOID)
{
   /* this just displays the system keys help panel */
   if(fHelpEnabled)
      if(NULL != WinSendMsg(hwndHelpInstance, HM_KEYS_HELP,
                            (MPARAM)NULL, (MPARAM)NULL))
	WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
		SZ_HELPDISPLAYERROR,
		(PSZ)NULL,
		0,
		MB_OK | MB_ERROR);
}   /* End of HelpKeys   */


/**************************************************************************
 *  Name       : HelpIndex()
 *
 *  Description: Processes the WM_COMMAND message posted by the
 *               Help Index item of the Help menu.
 *
 *  Concepts:    Called from UserCommand when the Help Index
 *               menu item is selected.
 *
 *               Sends an HM_INDEX_HELP message to the help
 *               instance so that the default Help Index is
 *               displayed.
 *************************************************************************/
VOID  HelpIndex(VOID)
{
   /* this just displays the system help index panel */
   if(fHelpEnabled)
      if(NULL != WinSendMsg(hwndHelpInstance, HM_HELP_INDEX,
                             (MPARAM)NULL, (MPARAM)NULL))
	WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
		SZ_HELPDISPLAYERROR,
		(PSZ)NULL,
		0,
		MB_OK | MB_ERROR);
}   /* End of HelpIndex() */

/**************************************************************************
 *  Name       : HelpTutorial()
 *
 *  Description: Processes the WM_COMMAND message posted by the
 *                Tutorial Help item of the Help menu.  While the
 *                standard template application does not include a
 *                Tutorial menu item, you can add one if your
 *                application has a tutorial.
 *
 *  Concepts:    Called from UserCommand when the Tutorial Help
 *               menu item is selected.
 *************************************************************************/
VOID  HelpTutorial(VOID)
{
   /* this just displays the help index panel */
   if(fHelpEnabled)
      if(NULL != WinSendMsg(hwndHelpInstance, HM_HELP_INDEX,
                             (MPARAM)NULL, (MPARAM)NULL))
	WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
		SZ_HELPDISPLAYERROR,
		(PSZ)NULL,
		0,
		MB_OK | MB_ERROR);
}   /* End of HelpTutorial   */

/**************************************************************************
 *  Name       : HelpProductInfo()
 *
 *  Description: Processes the WM_COMMAND message posted by the
 *               Product information item of the Help Menu.
 *
 *  Concepts:    Called from UserCommand when the Product information
 *               menu item is selected
 *
 *               Calls WinDlgBox to display the Product information dialog.
 *************************************************************************/
VOID  HelpProductInfo(HWND hwndMain)
{
   /* display the Product Information dialog. */
   WinDlgBox(HWND_DESKTOP,
             hwndMain,
             (PFNWP)ProductInfoDlgProc,
             0,
             IDD_PRODUCTINFO,
             (PVOID)NULL);
}   /* End of HelpProductInfo() */

/**************************************************************************
 *  Name       : DisplayHelpPanel(idPanel)
 *
 *  Description: Displays the help panel whose id is given
 *
 *  Concepts:    Called whenever a help panel is desired to be
 *               displayed, usually from the WM_HELP processing
 *               of the dialog boxes.
 *
 *               Sends HM_DISPLAY_HELP message to the help instance.
 *************************************************************************/
VOID DisplayHelpPanel(ULONG idPanel)
{
   if(fHelpEnabled)
      if(NULL != WinSendMsg(hwndHelpInstance,
                            HM_DISPLAY_HELP,
                            MPFROMLONG(idPanel),
                            MPFROMSHORT(HM_RESOURCEID)))
	WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
		SZ_HELPDISPLAYERROR,
		(PSZ)NULL,
		0,
		MB_OK | MB_ERROR);
}   /* End of DisplayHelpPanel   */

/**************************************************************************
 *  Name       : DestroyHelpInstance()
 *
 *  Description: Destroys the help instance for the application
 *
 *  Concepts:    Called after exit from message loop.
 *
 *               Calls WinDestroyHelpInstance() to destroy the
 *               help instance.
 *************************************************************************/
VOID DestroyHelpInstance(VOID)
{
   if(hwndHelpInstance != NULLHANDLE)
      WinDestroyHelpInstance(hwndHelpInstance);
}   /* End of DestroyHelpInstance   */
/***************************  End of help.c  ****************************/
