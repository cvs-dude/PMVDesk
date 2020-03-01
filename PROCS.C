/*========================================================\
 *      FILE: pmvdproc.c 
 *  
 *      DESCRIPTION: functions for virtual desktop
 * 
 *	This source file contains the following functions:
 *	InitMainWindow
 *	SetCascadeDefault
 *	HideFrameControls
 *	ShowFrameControls
 *	SaveApplication
 *	ButtonPressed
 *	MoveWindows
 *	MoveWindowsVisible
 *	MoveActiveWindow
 *
 \*=======================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs
\*--------------------------------------------------------------*/

#define INCL_DEV
#define INCL_WIN
#define INCL_GPI
#define INCL_DOS

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include "xtrn.h"
#include "main.h"

/*--------------------------------------------------------------*\
 *  Global variables and definitions for this file
\*--------------------------------------------------------------*/
SWP swp[100];	/* array of SWP structures. (max number of windows) */

/*-------------------------------------------------------------*\
 *  Entry point declarations
\*-------------------------------------------------------------*/
MRESULT InitMainWindow(HWND, MPARAM, MPARAM)
BOOL SetCascadeDefault(HWND,USHORT,USHORT)
VOID HideFrameControls (HWND);
VOID ShowFrameControls (HWND);
VOID FAR SaveApplication (HWND);
VOID MoveWindowsVisible();
VOID MoveActiveWindows();
VOID SetWindowPosition(HWND);

/**************************************************************************
 *  Name       : InitMainWindow(hwnd, mp1, mp2)
 *
 *  Description: Performs initialization functions required
 *               when the main window is created.
 *
 *  Concepts:    Called once during the WM_CREATE processing when
 *               the main window is created.
 *
 *  Parameters :  hwnd = window handle
 *                mp1  = first message parameter
 *                mp2  = second message parameter
 *
 *  Return     :   value to be returned from the WM_CREATE message:
 *                TRUE =  window creation should stop
 *                FALSE = window creation should continue
 *************************************************************************/
MRESULT InitMainWindow(HWND hwnd, MPARAM mp1, MPARAM mp2)
{
    LONG xLeft , yBottom ;      /* frame window location */
    LONG cyHeight, cxWidth, x, y;
    ULONG cbBuf;
    HINI hini;
    PSZ iniFileName;
    APIRET rc;

/*    hwndMainFrame = WinQueryWindow(hwnd , QW_PARENT); */
    hwndMainFrame = ((PCREATESTRUCT)PVOIDFROMMP(mp2))->hwndParent;
    hwndTitleBar = WinWindowFromID(hwndMainFrame, FID_TITLEBAR);
    hwndSysMenu = WinWindowFromID(hwndMainFrame, FID_SYSMENU);
    hwndMinMax = WinWindowFromID(hwndMainFrame, FID_MINMAX);
    hwndMenu = WinWindowFromID(hwndMainFrame, FID_MENU);
    hwndPopupMenu = WinLoadMenu(HWND_OBJECT, NULLHANDLE, IDM_POPUPMENU);

    SetCascadeDefault(hwndPopupMenu, IDM_HELP, IDM_HELPINDEX);
    UpdateTitleText(hwndMainFrame);

    /* screen size */
    cxScreen = WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN);
    cyScreen = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN);

    cbBuf = sizeof(cp);
    if (rc=DosScanEnv("PMVDESKB_INI", &iniFileName)){
	iniFileName = "PMVDESKB.INI";
    }
    hini = PrfOpenProfile(hab, iniFileName);
    if (!PrfQueryProfileData(hini, SZ_APPNAME, SZ_KEYNAME, &cp, &cbBuf))
    {
        cp.fControlsHidden = FALSE;
        cp.floatOnTop = FALSE;
	cp.retrieveWindows = TRUE;
        cp.currentAtx = cp.currentAty = 1;
        cp.nrButtons = 3;				/* number of buttons in row and column*/
        /*
        * Calculate an initial window position and size.
        */
        xLeft = cxScreen / 20;
        yBottom = cyScreen / 20;
        cxWidth = cxScreen / 6;
        cyHeight = cyScreen / 5;
        /* position the window and make it visible */
        WinSetWindowPos (hwndMainFrame, NULLHANDLE, xLeft, yBottom,
                       cxWidth, cyHeight,
                       SWP_SIZE | SWP_MOVE | SWP_ACTIVATE);
        WinQueryWindowPos(hwndMainFrame, &cp.swp);
    }
    else
    { /*Protect against garbage swp*/
        cp.swp.hwnd = hwndMainFrame;
        cp.swp.hwndInsertBehind = HWND_TOP;
        cp.swp.fl = (cp.swp.fl & (SWP_MINIMIZE | SWP_MAXIMIZE)) | SWP_SIZE | SWP_MOVE | SWP_ACTIVATE;
        WinSetMultWindowPos(hab, &cp.swp, 1);
    }
    PrfCloseProfile(hini);

/*    WinShowWindow(hwndMainFrame, TRUE); */

    for(idTimer=0; idTimer<=TID_USERMAX; idTimer++)
        if(WinStartTimer (hab, hwnd, idTimer, 750)) break; /* If we found the first timer break */
    if(idTimer > TID_USERMAX){ /* Inform user if we can't find a timer */
               MessageBox(HWND_DESKTOP,
                  IDMSG_TIMERINITFAILED,
                  MB_OK | MB_ERROR,
                  TRUE);
    }

    hwndCurrent = WinQueryActiveWindow(HWND_DESKTOP);

    if (cp.fControlsHidden){
        HideFrameControls (hwndMainFrame);
    } else {
	CheckMenuItem(hwndMenu, IDM_TOGGLECONTROLS, TRUE);
	CheckMenuItem(hwndPopupMenu, IDM_TOGGLECONTROLS, TRUE);
    }

    if (cp.floatOnTop){
	CheckMenuItem(hwndMenu, IDM_FLOAT, TRUE);
	CheckMenuItem(hwndPopupMenu, IDM_FLOAT, TRUE);
    }

    if (cp.retrieveWindows){
	CheckMenuItem(hwndMenu, IDM_RETRIEVE, TRUE);
	CheckMenuItem(hwndPopupMenu, IDM_RETRIEVE, TRUE);
    }

    /* create button grid */
    for (y=0; y < cp.nrButtons ; ++y) {
	for (x=0; x < cp.nrButtons ; ++x) {
		hwndButton[x][y] = WinCreateWindow(hwnd, WC_BUTTON, NULL, WS_VISIBLE |
					BS_DEFAULT | BS_NOPOINTERFOCUS, 0, 0, 0, 0,
					hwnd, HWND_BOTTOM,
					(y*cp.nrButtons+x), NULL, NULL);
	}
    }
    MainSize(hwnd);

   /* return FALSE to continue window creation, TRUE to abort it */
   return (MRESULT)FALSE;

}   /* End of InitMainWindow   */

/**************************************************************************
 *  Name       : SetCascadeDefault(hwndMenu, usSubmenu, usDefault)
 *
 *  Description: This function sets the default menuitem for the
 *               specified CCM and checks the menuitem.
 *
 *  Parameters :  hwndMenu, usSubmenu, usDefault
 *                hwndMenu - specifies the menu window handle.
 *                usSubmenu - specifies the id of the cascade menu.
 *                usDefault - specifies the id of the default menuitem.
 *
 *  Return     :  BOOL value. TRUE if successful, FALSE otherwise.
 *************************************************************************/
 BOOL SetCascadeDefault(HWND hwnd,USHORT usSubmenu,USHORT usDefault)
 {
    MENUITEM miItem;
    ULONG ulStyle;
 
    WinSendMsg(hwnd,
               MM_QUERYITEM,
               MPFROM2SHORT(usSubmenu,TRUE),
               MPFROMP(&miItem));
    ulStyle=WinQueryWindowULong(miItem.hwndSubMenu,QWL_STYLE);
    ulStyle|=MS_CONDITIONALCASCADE;
    WinSetWindowULong(miItem.hwndSubMenu,QWL_STYLE,ulStyle);
 
    WinSendMsg(miItem.hwndSubMenu,
               MM_SETDEFAULTITEMID,
               MPFROM2SHORT(usDefault,FALSE),
               0L);
    WinSendMsg(miItem.hwndSubMenu,
               MM_SETITEMATTR,
               MPFROM2SHORT(usDefault,FALSE),
               MPFROM2SHORT(MIA_CHECKED,MIA_CHECKED));
 
    return TRUE;
 }   /* End of SetCascadeDefault */

/****************************************************************\
 *  Name:HideFrameControls
 *
 *  Purpose:Hide the title bar and associted controls
\****************************************************************/
VOID HideFrameControls ( HWND hwnd )
{
    WinSetParent ( hwndTitleBar , HWND_OBJECT, FALSE ) ;
    WinSetParent ( hwndSysMenu , HWND_OBJECT, FALSE ) ;
    WinSetParent ( hwndMinMax , HWND_OBJECT , FALSE ) ;
    WinSetParent ( hwndMenu , HWND_OBJECT , FALSE ) ;
    WinSendMsg ( hwndMainFrame , WM_UPDATEFRAME ,
                 MPFROMLONG( FCF_TITLEBAR | FCF_SYSMENU | FCF_MINMAX | FCF_MENU ) ,
                 MPVOID) ;
    cp.fControlsHidden = TRUE ;
}

/****************************************************************\
 *  Name:ShowFrameControls
 *
 *  Purpose:Show the title bar and associated contols
 *  Returns:
 *          VOID
 *
\****************************************************************/
VOID ShowFrameControls ( HWND hwnd )
{
    WinSetParent ( hwndTitleBar , hwndMainFrame , FALSE ) ;
    WinSetParent ( hwndSysMenu , hwndMainFrame , FALSE ) ;
    WinSetParent ( hwndMinMax , hwndMainFrame , FALSE ) ;
    WinSetParent ( hwndMenu , hwndMainFrame , FALSE ) ;
    WinSendMsg ( hwndMainFrame , WM_UPDATEFRAME ,
                MPFROMLONG( FCF_TITLEBAR | FCF_SYSMENU | FCF_MINMAX | FCF_MENU ) ,
                MPVOID) ;
    WinInvalidateRect ( hwndMainFrame , NULL , TRUE ) ;
    cp.fControlsHidden = FALSE ;
}

/****************************************************************\
 *  Name:SaveApplication()
 *
 *  Purpose:Handle the WM_SAVEAPPLICATION message from the desktop
 *  Returns: VOID
\****************************************************************/
VOID FAR SaveApplication( HWND hwnd )
{
	HINI hini;
	PSZ iniFileName;
	APIRET rc;

	WinQueryWindowPos( hwnd, (PSWP)&cp.swp);
    if (rc=DosScanEnv("PMVDESKB_INI", &iniFileName)){
	iniFileName = "PMVDESKB.INI";
    }
	hini = PrfOpenProfile(hab, iniFileName);
	PrfWriteProfileData(hini, SZ_APPNAME, SZ_KEYNAME, &cp, sizeof(cp));
	PrfCloseProfile(hini);

}

/****************************************************************\
 *  Name: MoveWindowsVisible
 *  Purpose: Move windows to the visible area
\****************************************************************/
VOID MoveWindowsVisible()
{
 SWP swp2[30];	/* array of SWP structures. (max number of windows) */
 SWP tempSwp;
 PSWP	pswp;			/* pointer to SWP array */
 PSWP	pswp2;
 LONG	windowCount, i = 0, j = 0, z;
 HWND	ownerhwnd, tophwnd; 
 BOOL	fNotMoved;

	pswp = &swp[0];
	pswp2 = &swp2[0];
	for (z=0; z < 100; z++){
		swp[z].hwnd = NULLHANDLE;
	}
	for (z=0; z < 30; z++){
		swp2[z].hwnd = NULLHANDLE;
	}
	tophwnd = hwndMainFrame;

	while(((tophwnd = WinQueryWindow(tophwnd, QW_NEXTTOP)) != hwndMainFrame) && i < 100) {
		fNotMoved = TRUE;
		WinQueryWindowPos(tophwnd, (PSWP)&tempSwp);
		if ((WinQueryWindow(tophwnd, QW_FRAMEOWNER) == NULLHANDLE) && ((!WinIsWindowShowing(tophwnd)) || ((tempSwp.x+tempSwp.cx >= 0) && (tempSwp.x+tempSwp.cx <= xBorder)) || ((tempSwp.y+tempSwp.cy >= 0) && (tempSwp.y+tempSwp.cy <= yBorder)) || ((tempSwp.x<=cxScreen) && (tempSwp.x >= cxScreen-xBorder)) || ((tempSwp.y <= cyScreen) && (tempSwp.y >= cyScreen-yBorder)))) {
			for (z=0; z <= i; z++){
				if (tophwnd == swp[z].hwnd){
					fNotMoved = FALSE;
					break;
				}
			}
			if (fNotMoved == FALSE) continue;
			WinQueryWindowPos(tophwnd, (PSWP)&swp[i]);
			swp[i].x = 0;
			swp[i].y = 0;
			swp[i++].fl |= (SWP_NOADJUST | SWP_HIDE | SWP_ZORDER);
		} else if ((!WinIsWindowShowing(tophwnd)) || ((tempSwp.x+tempSwp.cx >= 0) && (tempSwp.x+tempSwp.cx <= xBorder)) || ((tempSwp.y+tempSwp.cy >= 0) && (tempSwp.y+tempSwp.cy <= yBorder)) || ((tempSwp.x<=cxScreen) && (tempSwp.x >= cxScreen-xBorder)) || ((tempSwp.y <= cyScreen) && (tempSwp.y >= cyScreen-yBorder))) {
			if (WinQueryWindow(tophwnd, QW_FRAMEOWNER) == hwndMainFrame) continue;
			WinQueryWindowPos(tophwnd, (PSWP)&swp2[j++]);
			ownerhwnd = tophwnd;
			while((ownerhwnd = WinQueryWindow(ownerhwnd, QW_FRAMEOWNER)) != NULLHANDLE) {
				if (ownerhwnd != hwndMainFrame){
					WinQueryWindowPos(ownerhwnd, (PSWP)&tempSwp);
					if ((WinQueryWindow(ownerhwnd, QW_FRAMEOWNER) == NULLHANDLE) && ((!WinIsWindowShowing(ownerhwnd)) || ((tempSwp.x+tempSwp.cx >= 0) && (tempSwp.x+tempSwp.cx <= xBorder)) || ((tempSwp.y+tempSwp.cy >= 0) && (tempSwp.y+tempSwp.cy <= yBorder)) || ((tempSwp.x<=cxScreen) && (tempSwp.x >= cxScreen-xBorder)) || ((tempSwp.y <= cyScreen) && (tempSwp.y >= cyScreen-yBorder)))) {
						for (z=0; z < i; z++){
							if (ownerhwnd == swp[z].hwnd){
								swp[z].fl |= (SWP_NOADJUST | SWP_SHOW | SWP_MOVE | SWP_ZORDER);
								fNotMoved = FALSE;
								break;
							}
						}
						if (fNotMoved == FALSE) continue;
						WinQueryWindowPos(ownerhwnd, (PSWP)&swp[i]);
						swp[i].x = 0;
						swp[i].y = 0;
						swp[i++].fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
					} else {
						WinQueryWindowPos(ownerhwnd, (PSWP)&swp2[j++]);
					}
				}
			}
		}
	}

	WinSetMultWindowPos(hab, pswp, i);

	if (j > 0){
		for (z=0; z <= j; z++){
			if (WinQueryWindow(swp2[z].hwnd, QW_FRAMEOWNER) != NULLHANDLE){
				swp2[z].x = 0;
				swp2[z].y = 0;
				swp2[z].fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
			}
		}
		WinSetMultWindowPos(hab, pswp2, j);
	}

} /* end MoveWindowsVisible */

/****************************************************************\
 *  Name: MoveActiveWindows
 *  Purpose: Move active window (if not visible) to visible area
\****************************************************************/
VOID MoveActiveWindows()
{
   HWND hwndOwner, whwnd;
   HENUM henum;

	hwndOwner = WinQueryWindow(hwndCurrent, QW_OWNER);
	if ((hwndOwner != NULLHANDLE) && (hwndOwner != (WinQueryDesktopWindow(hab, hdcMain)))){
		SetWindowPosition(hwndOwner);
	} else {
		SetWindowPosition(hwndCurrent);
	}

} /* end MoveActiveWindows */

/****************************************************************\
 *  Name: SetWindowPosition
 *  Purpose: Move windows to their virtual screen
\****************************************************************/
VOID SetWindowPosition(HWND whwnd)
{
   SWP swp;
   INT xTimes=cp.nrButtons-1;
   INT yTimes=cp.nrButtons-1;
   LONG x, y, midpointX, midpointY;

	WinQueryWindowPos(whwnd, (PSWP)&swp);
	if ((!WinIsWindowShowing(whwnd)) || ((swp.x+swp.cx >= 0) && (swp.x+swp.cx <= xBorder+1)) || ((swp.y+swp.cy >= 0) && (swp.y+swp.cy <= yBorder+1)) || ((swp.x<=cxScreen) && (swp.x >= cxScreen-xBorder-1)) || ((swp.y <= cyScreen) && (swp.y >= cyScreen-yBorder-1))){  
		do {
			x = swp.x + xTimes*(1+cxScreen);
			midpointX = x + (swp.cx / 2);
			xTimes--;
		}while ((midpointX < 0) || (midpointX > cxScreen));
		do {
			y = swp.y + yTimes*(1+cyScreen);
			midpointY = y + (swp.cy / 2);
			yTimes--;
		}while ((midpointY < 0) || (midpointY > cyScreen));
		WinSetWindowPos (whwnd, HWND_TOP, x, y, swp.cx, swp.cy, swp.fl | SWP_MOVE); /*  | SWP_NOADJUST SWP_ZORDER | SWP_SHOW */
	} /* endif */

} /* end SetWindowPosition */

/*--------------------------------------------------------------*\
 *  End of file: pmvdproc.c
\*--------------------------------------------------------------*/
