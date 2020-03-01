/**************************************************************************
 *  File name  :  main.c
 *
 *  Description:  
 *
 *	This source file contains the following functions:
 *
 *	main()
 *	MainWndProc(hwnd, msg, mp1, mp2)
 *************************************************************************/

/*
 *  Include files, macros, defined constants, and externs
 */
#define INCL_DOSERRORS
#define INCL_DOSMISC
#define INCL_DOSSEMAPHORES
#define INCL_WINFRAMEMGR
#define INCL_WINHELP
#define INCL_WINMENUS
#define INCL_WINSHELLDATA
#define INCL_WINSWITCHLIST
#define INCL_WINSYS
#define INCL_WINTIMER
#define INCL_WINTRACKRECT
#define INCL_WINWINDOWMGR

#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include "strings.h"
#include "pmvd.h"
#include "xtrn.h"

/*
 *  Global variables
 */
PMVDPREFS	cp;
PFNWP	OldFrameProc;

/*
 *  Entry point declarations
 */
MRESULT EXPENTRY MainWndProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY SubFrameProc(HWND, ULONG, MPARAM, MPARAM);
VOID FAR SaveSettings(HWND);

/**************************************************************************
 *  Name       : main()
 *
 *  Description: Initializes the PM environment, calls the
 *               initialization routine, creates the main
 *               window,  and polls the message queue
 *
 *  Concepts:    - obtains anchor block handle and creates message
 *                    queue
 *               - calls the initialization routine
 *               - creates the main frame window which creates the
 *                    main client window
 *               - polls the message queue via Get/Dispatch Msg loop
 *               - upon exiting the loop, exits
 *
 *  Return     :  0 - if successful execution completed
 *                1 - if error
 *************************************************************************/
INT main(VOID)
{
 HAB	hab;
 HMQ	hmq;
 HWND	hwndFrame,
	hwndClient;
 QMSG	qmsg;
 ULONG	flFrameFlags;
 SWCNTRL	PgmEntry;
 HMTX	hmtx;

 hab = WinInitialize(0);
 hmq = WinCreateMsgQueue(hab, 0);

/* check if program allready running */
if (DosCreateMutexSem(SZ_MULT_CPY_MUTEX, &hmtx, 0, TRUE) == ERROR_DUPLICATE_NAME)
{
	DosOpenMutexSem(SZ_MULT_CPY_MUTEX, &hmtx);
	WinAlarm(HWND_DESKTOP, WA_ERROR);
	WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
		SZ_RUNNING,
		(PSZ)NULL,
		0,
		MB_OK | MB_ERROR);
	WinDestroyMsgQueue(hmq);
	WinTerminate(hab);
	return(1);
}

 WinRegisterClass(hab, SZ_TITLE, MainWndProc, CS_SIZEREDRAW, 0);

 flFrameFlags = FCF_TITLEBAR | FCF_SYSMENU | FCF_MENU |
	FCF_SIZEBORDER | FCF_MINMAX | FCF_ACCELTABLE |
	FCF_ICON;
 hwndFrame = WinCreateStdWindow(HWND_DESKTOP, WS_ANIMATE, &flFrameFlags,
	SZ_TITLE, SZ_TITLE, 0L,
	(HMODULE)NULL, ID_MAINWND,
	&hwndClient);

 /* Subclassing is moved to LoadSettings */
 /* Subclass default frame window procedure */
// OldFrameProc = WinSubclassWindow(hwndFrame, (PFNWP)SubFrameProc);

 /* Add to window list */
 PgmEntry.hwnd		= hwndFrame;
 PgmEntry.hwndIcon	= (HWND)NULL;
 PgmEntry.hprog	= (HPROGRAM)NULL;
 PgmEntry.idProcess	= (PID)NULL;
 PgmEntry.idSession	= (ULONG)NULL;
 PgmEntry.uchVisibility	= SWL_VISIBLE;
 PgmEntry.fbJump	= SWL_JUMPABLE;
 strcpy(PgmEntry.szSwtitle, SZ_TITLE);

 WinAddSwitchEntry(&PgmEntry);

 while (WinGetMsg(hab, &qmsg, (HWND)NULL, 0, 0))
	WinDispatchMsg(hab, &qmsg);

 /* destroy the help instance */
 DestroyHelpInstance();
 /* Remove from window list and clean up */
 WinRemoveSwitchEntry(WinQuerySwitchHandle(hwndFrame, 0));
 WinDestroyWindow(hwndFrame);
 DosCloseMutexSem(hmtx);
 WinDestroyMsgQueue(hmq);
 WinTerminate(hab);
 return 0;
} /* End of main */

/**************************************************************************
 *  Name:         MainWndProc(hwnd, msg, mp1, mp2)
 *
 *  Description:  Processes the messages sent to the main client
 *                window.  This routine processes the basic
 *                messages all client windows should process.
 *
 *  Concepts:     Called for each message placed in the main
 *                window's message queue
 *                A switch statement branches to the routines to be
 *                performed for each message processed.
 *
 *  Parameters:   hwnd = window handle
 *                msg  = message i.d.
 *                mp1  = first message parameter
 *                mp2  = second message parameter
 *
 *  Return:       value is determined by each message
 *************************************************************************/
MRESULT EXPENTRY MainWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
 HAB	hab;
 HWND	hwndMain;
 static ULONG idTimer;
 SWP	swp;

 switch (msg)
 {
	case WM_COMMAND:
		UserCommand(hwnd, mp1, mp2);
		break;

	case WM_DESTROY:
		hwndMain = WinQueryWindow(hwnd, QW_PARENT);
		WinAssociateHelpInstance(NULLHANDLE, hwndMain);
		break;

	case WM_ERASEBACKGROUND:
		return (MRESULT)TRUE;

	case WM_SAVEAPPLICATION:
		SaveSettings(hwnd);
		break;

	case HM_QUERY_KEYS_HELP:
		/* return id of key help panel */
		return (MRESULT)PANEL_HELPKEYS;

	case WM_CREATE:
		InitMainWindow(hwnd, mp1, mp2);
		hwndMain = WinQueryWindow(hwnd, QW_PARENT);
		hab = WinQueryAnchorBlock(hwndMain);
		for(idTimer=0; idTimer<=TID_USERMAX; idTimer++)
			/* If we found the first timer break */
			if(WinStartTimer (hab, hwnd, idTimer, 750)) break;
		if(idTimer > TID_USERMAX){
			/* Inform user if we can't find a timer */
			WinAlarm(HWND_DESKTOP, WA_ERROR);
			WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
				SZ_NOTIMER,
				(PSZ)NULL,
				0,
				MB_OK | MB_ERROR);
		}
		break;

	case WM_CLOSE:
		if (cp.retrieveWindows){
	 		MoveWindowsVisible(hwnd);
		}
		hwndMain = WinQueryWindow(hwnd, QW_PARENT);
		hab = WinQueryAnchorBlock(hwndMain);
		WinStopTimer (hab, hwnd, idTimer);
		break;

	case WM_PAINT:
		{
		HPS hps;
		RECTL rcl;

		hps = WinBeginPaint(hwnd, (HPS)NULL, &rcl);
		WinFillRect(hps, &rcl, SYSCLR_WINDOW);
		WinEndPaint(hps);
		}
		break;

	case WM_SIZE:
		MainSize(hwnd, mp2);
		break;

	case WM_INITMENU:
		InitMenu(mp1, mp2);
		break;

	case WM_TIMER:
		{
		HWND	hwndActive;
		static HWND	hwndCurrent;

		if (cp.floatOnTop) {
			hwndMain = WinQueryWindow(hwnd, QW_PARENT);
			WinQueryWindowPos(hwndMain, (PSWP)&swp);
			if (swp.hwndInsertBehind != HWND_TOP) {
				WinSetWindowPos (hwndMain, HWND_TOP, swp.x, swp.y, swp.cx, swp.cy, swp.fl | SWP_ZORDER);
			} /* endif */
		}
		hwndActive = WinQueryActiveWindow(HWND_DESKTOP);
		if (hwndActive != hwndCurrent) {
			hwndCurrent = hwndActive;
		if (hwndCurrent != NULLHANDLE)
			MoveActiveWindows(hwnd, hwndCurrent);
		}
		}
		break;

 /*
  * Add case statements for message ids you wish to process.
  */
 }
 return WinDefWindowProc(hwnd, msg, mp1, mp2);
} /* End of MainWndProc */

/**************************************************************************
 *  Name:         SubFrameProc(hwnd, msg, mp1, mp2)
 *
 *  Description:  When frame window procedure is subclassed, this
 *                procedure first handles the messages before sending
 *                them to the (default) old frame window procedure.
 *                Processes the messages sent to the main frame
 *                window.  This routine processes the basic
 *                messages the frame window should process
 *                and passes all others onto OldFrameProc.
 *
 *  Concepts:     Called for each message placed in the main
 *                window's message queue
 *                A switch statement branches to the routines to be
 *                performed for each message processed.
 *
 *  Parameters:   hwnd = window handle
 *                msg  = message i.d.
 *                mp1  = first message parameter
 *                mp2  = second message parameter
 *
 *  Return:       value is determined by each message
 *************************************************************************/
MRESULT EXPENTRY SubFrameProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
 PTRACKINFO	ptrack;
 LONG	cxWindowSize,
	cyWindowSize;
 SWP	swp;

 switch(msg)
 {
	case WM_QUERYTRACKINFO:
		/* default frame window procedure first updates */
		/* tracking rectangle to new position */
		OldFrameProc(hwnd, msg, mp1, mp2);
		WinQueryWindowPos(hwnd, (PSWP)&swp);
		if (cp.fControlsHidden){
			cxWindowSize = 32;
			cyWindowSize = 32;
		} else if (swp.fl & SWP_MINIMIZE){
			return OldFrameProc(hwnd, msg, mp1, mp2);
		} else {
			cxWindowSize = WinQuerySysValue(HWND_DESKTOP, SV_CXMINMAXBUTTON)*2 + 2*WinQuerySysValue(HWND_DESKTOP, SV_CXSIZEBORDER);
			cyWindowSize = WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR) + WinQuerySysValue(HWND_DESKTOP, SV_CYMENU) + 2*WinQuerySysValue(HWND_DESKTOP, SV_CYSIZEBORDER);
		}
		ptrack = (PTRACKINFO)mp2;
		ptrack->cxGrid = 2;
		ptrack->cyGrid = 2; /* smooth tracking with mouse */
		ptrack->cxKeyboard = 1;
		ptrack->cyKeyboard = 1; /* smooth tracking using cursor keys */
		/* set smallest allowed size of rectangle */
		ptrack->ptlMinTrackSize.x = cxWindowSize;
		ptrack->ptlMinTrackSize.y = cyWindowSize;
		/* set largest allowed size of rectangle */
//		ptrack->ptlMaxTrackSize.x = 200;
//		ptrack->ptlMaxTrackSize.y = 200;
		ptrack->fs |= TF_GRID;
		return((MRESULT)TRUE);
 }

 /* pass all other messages to the default procedure */
 return OldFrameProc(hwnd, msg, mp1, mp2);
} /* End of SubFrameProc */

/****************************************************************\
 *  Name:	SaveSettings()
 *
 *  Purpose:	Handle the WM_SAVEAPPLICATION message from the desktop.
 *	Save initial settings to INI-file
 *  Returns:	VOID
\****************************************************************/
VOID FAR SaveSettings(HWND hwnd)
{
 HAB	hab;
 HINI	hini;
 HWND	hwndFrame;
 PSZ	iniFileName;

 hwndFrame = WinQueryWindow(hwnd, QW_PARENT);
 WinQueryWindowPos(hwndFrame, (PSWP)&cp.swp);
 if (DosScanEnv((PSZ)SZ_ENVIRON_VAR, (const CHAR **)&iniFileName)){
 	iniFileName = SZ_INI_FILE;
 }
 hab = WinQueryAnchorBlock(hwndFrame);
 hini = PrfOpenProfile(hab, iniFileName);
 PrfWriteProfileData(hini, SZ_APPNAME, SZ_KEYNAME, &cp, sizeof(cp));
 PrfCloseProfile(hini);
} /* End of SaveSettings */
