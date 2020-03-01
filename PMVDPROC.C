/**************************************************************************
 *      FILE: pmvdproc.c 
 *  
 *      DESCRIPTION: functions for virtual desktop
 * 
 *	This source file contains the following functions:
 *	InitMainWindow
/**************************************************************************
/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs
\*--------------------------------------------------------------*/
#define INCL_DEV
#define INCL_DOSMISC
#define INCL_GPIBITMAPS
#define INCL_WINBUTTONS
#define INCL_WINFRAMEMGR
#define INCL_WININPUT
#define INCL_WINMENUS
#define INCL_WINPOINTERS
#define INCL_WINSHELLDATA
#define INCL_WINSYS
#define INCL_WINTRACKRECT
#define INCL_WINWINDOWMGR

#include <os2.h>
#include "pmvd.h"
#include "xtrn.h"
#include "strings.h"

/*--------------------------------------------------------------*\
 *  Global variables and definitions for this file
\*--------------------------------------------------------------*/
HWND	hwndButton[4][4];
PFNWP	OldButtonProc;

/*-------------------------------------------------------------*\
 *  Entry point declarations
\*-------------------------------------------------------------*/
MRESULT InitMainWindow(HWND, MPARAM, MPARAM);
BOOL SetCascadeDefault(HWND,USHORT,USHORT);
VOID FrameControls (HWND, BOOL, BOOL);
VOID InitMenu(MPARAM, MPARAM);
VOID EnableMenuItem(HWND, USHORT, BOOL);
VOID CheckMenuItem(HWND, SHORT, BOOL);
VOID MainSize(HWND, MPARAM);
MRESULT EXPENTRY SubButtonProc(HWND, ULONG, MPARAM, MPARAM);
HBITMAP CopyScreenToBitmap (HWND);
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
 HAB	hab;
 HWND	hwndMain;
 LONG	x, y;
 HINI	hini;
 PSZ	iniFileName;
 LONG	lXScreen,
 	lYScreen;
 ULONG	cbBuf;

 hwndMain = WinQueryWindow(hwnd, QW_PARENT);
 hab = WinQueryAnchorBlock(hwndMain);
 InitHelp(hab, hwndMain);

 /* Subclass default frame window procedure */
 OldFrameProc = WinSubclassWindow(hwndMain, (PFNWP)SubFrameProc);

 /* screen size */
 lXScreen = WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN);
 lYScreen = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN);

 /* init frame controls */
 FrameControls(hwndMain, FALSE, TRUE);

 /* Get initial settings from INI-file */
 cbBuf = sizeof(cp);
 if (DosScanEnv((PSZ)SZ_ENVIRON_VAR, (const CHAR **)&iniFileName)){
 	iniFileName = SZ_INI_FILE;
 }
 hini = PrfOpenProfile(hab, iniFileName);
 if (!PrfQueryProfileData(hini, SZ_APPNAME, SZ_KEYNAME, &cp, &cbBuf)){
 	cp.fControlsHidden = FALSE;
 	cp.floatOnTop = FALSE;
 	cp.retrieveWindows = TRUE;
 	cp.currentAtx = cp.currentAty = 1;
 	cp.nrButtons = 3;	/* number of buttons in row and column*/
 	/*
 	* Calculate an initial window position and size.
 	*/
 	cp.swp.x = lXScreen / 20;
 	cp.swp.y = lYScreen / 20;
 	cp.swp.cx = lXScreen / 6;
 	cp.swp.cy = lYScreen / 5;
 } else {
	if (cp.fControlsHidden) {
 		WinSetWindowPos (hwndMain, HWND_TOP, 0, 0, 0, 0,
			SWP_SIZE | SWP_MOVE | SWP_ACTIVATE);
		FrameControls(hwndMain, !cp.fControlsHidden, FALSE);
	}
 }
 PrfCloseProfile(hini);

    /* create button grid */
    for (y=0; y < 4 ; ++y) {
	for (x=0; x < 4 ; ++x) {
		hwndButton[x][y] = WinCreateWindow(hwnd,
				WC_BUTTON,
				NULL,
				WS_VISIBLE |
				BS_DEFAULT |
				BS_NOPOINTERFOCUS,
				0, 0, 0, 0,
				hwndMain,
				HWND_TOP,
				(y*4+x),
				NULL, NULL);
 /* Subclass button window procedure */
 OldButtonProc = WinSubclassWindow(hwndButton[x][y], (PFNWP)SubButtonProc);
	}
    }
 WinSendMsg(hwndButton[0][0], UM_INIT_POPUP_MENU, 0, 0);

 /* position the window and make it visible */
 WinSetWindowPos (hwndMain, HWND_TOP, cp.swp.x, cp.swp.y, cp.swp.cx, cp.swp.cy,
	SWP_SIZE | SWP_MOVE | SWP_SHOW | SWP_ACTIVATE);
 /*Protect against garbage swp*/
 WinQueryWindowPos(hwndMain, &cp.swp);

 /* return FALSE to continue window creation, TRUE to abort it */
 return (MRESULT)FALSE;
} /* End of InitMainWindow   */

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
 *  Name:FrameControls
 *
 *  Purpose:Hide the title bar and associted controls
\****************************************************************/
VOID FrameControls (HWND hwndMain, BOOL fShow, BOOL fInit)
{
 static HWND	hwndTitleBar,
	hwndSysMenu,
	hwndMinMax,
	hwndMenu;
 SWP	swp;
 LONG	cxWindowSize,
	cyWindowSize;

 WinQueryWindowPos(hwndMain, (PSWP)&swp);
 if (fInit){
	hwndTitleBar = WinWindowFromID(hwndMain, FID_TITLEBAR);
	hwndSysMenu = WinWindowFromID(hwndMain, FID_SYSMENU);
	hwndMinMax = WinWindowFromID(hwndMain, FID_MINMAX);
	hwndMenu = WinWindowFromID(hwndMain, FID_MENU);
 } else if (!(swp.fl & SWP_MINIMIZE)){
	if (fShow){
		WinSetParent(hwndTitleBar, hwndMain, TRUE);
		WinSetParent(hwndSysMenu, hwndMain, TRUE);
		WinSetParent(hwndMinMax, hwndMain, TRUE);
		WinSetParent(hwndMenu, hwndMain, TRUE);
		cp.fControlsHidden = FALSE;
		cxWindowSize = WinQuerySysValue(HWND_DESKTOP, SV_CXMINMAXBUTTON)*2 + 2*WinQuerySysValue(HWND_DESKTOP, SV_CXSIZEBORDER);
		cyWindowSize = WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR) + WinQuerySysValue(HWND_DESKTOP, SV_CYMENU) + 2*WinQuerySysValue(HWND_DESKTOP, SV_CYSIZEBORDER);
		if (swp.cx < cxWindowSize)
			swp.cx = cxWindowSize;
		if (swp.cy < cyWindowSize)
			swp.cy = cyWindowSize;
		WinSetWindowPos (hwndMain, swp.hwndInsertBehind,
			swp.x, swp.y, swp.cx, swp.cy, SWP_SIZE | SWP_MOVE);
	} else {
		WinSetParent(hwndTitleBar, HWND_OBJECT, FALSE);
		WinSetParent(hwndSysMenu, HWND_OBJECT, FALSE);
		WinSetParent(hwndMinMax, HWND_OBJECT, FALSE);
		WinSetParent(hwndMenu, HWND_OBJECT, FALSE);
		cp.fControlsHidden = TRUE;
	}
	WinSendMsg(hwndMain, WM_UPDATEFRAME,
		MPFROMLONG(FCF_TITLEBAR | FCF_SYSMENU |
		FCF_MINMAX | FCF_MENU), 0);
 }
} /* End of FrameControls */

/**************************************************************************
 *  Name       : InitMenu()
 *
 *  Description: Processes the WM_INITMENU message for the main window,
 *               disabling any menus that are not active.
 *
 *  Concepts:    Routine is called each time a menu is dropped.
 *
 *               A switch statement branches control based upon
 *               the id of the menu that is being displayed.
 *************************************************************************/
VOID InitMenu(MPARAM mp1, MPARAM mp2)
{
 HWND	hwndMenu;

 /* define a shorthand way of denoting the menu handle */
 hwndMenu = HWNDFROMMP(mp2);

   switch(SHORT1FROMMP(mp1))
   {
   case IDM_HELP:
	/*
	* Enable or disable the Help menu depending upon whether the
	* help manager has been enabled
	*/
	EnableMenuItem(hwndMenu, IDM_HELPUSINGHELP, fHelpEnabled);
	EnableMenuItem(hwndMenu, IDM_HELPGENERAL, fHelpEnabled);
	EnableMenuItem(hwndMenu, IDM_HELPKEYS, fHelpEnabled);
	EnableMenuItem(hwndMenu, IDM_HELPINDEX, fHelpEnabled);
	/*  REMEMBER: add a case for IDM_HELPTUTORIAL if you include
	*  the Tutorial menu item.
	*/
	break;

   case IDM_OPTIONS:
	CheckMenuItem(hwndMenu, IDM_TOGGLECONTROLS, !cp.fControlsHidden);
	CheckMenuItem(hwndMenu, IDM_FLOAT, cp.floatOnTop);
	CheckMenuItem(hwndMenu, IDM_RETRIEVE, cp.retrieveWindows);
	break;

   case IDM_BUTTONS:
	CheckMenuItem(hwndMenu, IDM_2X2, (IDM_BUTTONS + cp.nrButtons == IDM_2X2));
	CheckMenuItem(hwndMenu, IDM_3X3, (IDM_BUTTONS + cp.nrButtons == IDM_3X3));
	CheckMenuItem(hwndMenu, IDM_4X4, (IDM_BUTTONS + cp.nrButtons == IDM_4X4));
	break;

    default:
	/* Message came from popupmenu */
	CheckMenuItem(hwndMenu, IDM_TOGGLECONTROLS, !cp.fControlsHidden);
	CheckMenuItem(hwndMenu, IDM_FLOAT, cp.floatOnTop);
	CheckMenuItem(hwndMenu, IDM_RETRIEVE, cp.retrieveWindows);
	break;
    }
}   /* End of InitMenu   */

/**************************************************************************
 *  Name       : EnableMenuItem(hwndMenu, idItem, fEnable)
 *
 *  Description: Enables or disables the menu item
 *
 *  Concepts:    Called whenever a menu item is to be enabled or
 *               disabled
 *
 *               Sends a MM_SETITEMATTR to the menu with the
 *               given item id.  Sets the MIA_DISABLED attribute
 *               flag if the item is to be disabled, clears the flag
 *               if enabling.
 *
 *  Parameters :  hwndmenu = menu window handle
 *                idItem   = menu item i.d.
 *                fEnable  = enable (yes) or disable (no)
 *************************************************************************/
VOID EnableMenuItem(HWND hwndMenu, USHORT idItem, BOOL fEnable)
{
   SHORT fsFlag;

   if(fEnable)
      fsFlag = 0;
   else
      fsFlag = MIA_DISABLED;

   WinSendMsg(hwndMenu,
              MM_SETITEMATTR,
              MPFROM2SHORT(idItem, TRUE),
              MPFROM2SHORT(MIA_DISABLED, fsFlag));

}   /* End of EnableMenuItem() */

/****************************************************************\
 *  Name:   CheckMenuItem(hwndMenu, idItem, fEnable)
 *
 *  Checks/UnChecks the menu item of the given menu
 *
 *  Method: Sends a MM_SETITEMATTR to the menu with the
 *          given item id.
\****************************************************************/
VOID CheckMenuItem(HWND hwndMenu, SHORT idItem, BOOL fEnable)
{
    SHORT fsFlag;

    if(fEnable)
        fsFlag = MIA_CHECKED;
    else
        fsFlag = 0;

    WinSendMsg(hwndMenu,
               MM_SETITEMATTR,
               MPFROM2SHORT(idItem, TRUE),
               MPFROM2SHORT(MIA_CHECKED, fsFlag));
}   /* CheckMenuItem() */

/**************************************************************************
 *  Name       : MainSize(hwnd)
 *
 *  Description: Sizes the main client window objects.
 *
 *  Concepts:  Routine is called whenver the client window
 *             procedure receives a WM_SIZE message
 *************************************************************************/
VOID MainSize(HWND hwnd, MPARAM mp2)
{
	HAB	hab;
	LONG	buttonLength, buttonHeight;
	INT	x, y, i=0;
	SWP	Swp[25];

	buttonLength = (SHORT1FROMMP(mp2)) / cp.nrButtons;
	buttonHeight = (SHORT2FROMMP(mp2)) / cp.nrButtons;
	for (y=0; y < cp.nrButtons ; ++y) {
		for (x=0; x < cp.nrButtons ; ++x) {
			Swp[i].hwnd = hwndButton[x][y];
			Swp[i].hwndInsertBehind = HWND_TOP;
			Swp[i].fl = SWP_MOVE | SWP_SIZE;
			Swp[i].x = x * buttonLength;
			Swp[i].y = y * buttonHeight;
			Swp[i].cx = buttonLength;
			Swp[i].cy = buttonHeight;
			if (x == cp.currentAtx && y == cp.currentAty) {
				WinSetWindowText(hwndButton[x][y], "X");
			}
			i++;
		} /* endfor */
	} /* endfor */
	hab = WinQueryAnchorBlock(hwnd);
	WinSetMultWindowPos(hab, Swp, i);
}   /* End of MainSize   */

/**************************************************************************
 *  Name:         SubButtonProc(hwnd, msg, mp1, mp2)
 *
 *  Description:  When Button window procedure is subclassed, this
 *                procedure first handles the messages before sending
 *                them to the (default) old frame window procedure.
 *                Processes the messages sent to the Button
 *                window.  This routine processes the basic
 *                messages the Button window should process
 *                and passes all others onto OldButtonProc.
 *
 *  Concepts:     Called for each message placed in the Button
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
MRESULT EXPENTRY SubButtonProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
 static HWND	hwndPopupMenu;
 HWND	hwndMain,
	hwndClient;

 hwndClient = WinQueryWindow(hwnd, QW_PARENT);
 hwndMain = WinQueryWindow(hwndClient, QW_PARENT);
 switch(msg)
 {
	case WM_CONTEXTMENU:
	{
		POINTL	point;

		WinQueryPointerPos(HWND_DESKTOP, &point);
		WinMapWindowPoints(HWND_DESKTOP, hwnd, &point,1);
//		WinSendMsg(hwndMain, WM_INITMENU, MPFROMSHORT(IDM_POPUPMENU), MPFROMHWND(hwndPopupMenu));
		WinPopupMenu(hwnd, hwndMain, hwndPopupMenu,
				point.x, point.y, 0, PU_NONE | PU_MOUSEBUTTON1
				 | PU_MOUSEBUTTON2 | PU_KEYBOARD |
				PU_HCONSTRAIN | PU_VCONSTRAIN );
	}
	return (MRESULT)TRUE;

	case UM_INIT_POPUP_MENU:
		hwndPopupMenu = WinLoadMenu(HWND_OBJECT, NULLHANDLE, IDM_POPUPMENU);
		SetCascadeDefault(hwndPopupMenu, IDM_HELP, IDM_HELPINDEX);
		break;
 }

 /* pass all other messages to the default procedure */
 return OldButtonProc(hwnd, msg, mp1, mp2);
} /* End of SubButtonProc */

/****************************************************************\
 *  Name: MoveWindows
 *  Purpose: Move windows to their virtual screen
\****************************************************************/
VOID MoveWindows(HWND hwnd, LONG x, LONG y)
{
 SWP	swp2[30];	/* array of SWP structures. (max number of windows) */
 SWP	swp[100];
 PSWP	pswp;	/* pointer to SWP array */
 PSWP	pswp2;
 LONG	i = 0, j = 0, z;
 LONG	posX, posY;
 HWND	hwndMain, ownerhwnd, tophwnd; 
 BOOL	fNotMoved;
 HAB	hab;
 LONG	lXScreen,
 	lYScreen;

 /* screen size */
 lXScreen = WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN);
 lYScreen = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN);
 hwndMain = WinQueryWindow(hwnd, QW_PARENT);

 pswp = &swp[0];
 pswp2 = &swp2[0];
 for (z=0; z < 100; z++){
	swp[z].hwnd = NULLHANDLE;
 }
 for (z=0; z < 30; z++){
	swp2[z].hwnd = NULLHANDLE;
 }
 posX = (cp.currentAtx - x)*(1 + lXScreen);
 posY = (cp.currentAty - y)*(1 + lYScreen);
 hwndMain = WinQueryWindow(hwnd, QW_PARENT);
 hab = WinQueryAnchorBlock(hwndMain);
 tophwnd = hwndMain;

 while(((tophwnd = WinQueryWindow(tophwnd, QW_NEXTTOP)) != hwndMain) && i < 100) {
	fNotMoved = TRUE;
	if (WinQueryWindow(tophwnd, QW_FRAMEOWNER) == NULLHANDLE) {
		for (z=0; z <= i; z++){
			if (tophwnd == swp[z].hwnd){
				fNotMoved = FALSE;
				break;
			}
		}
		if (fNotMoved == FALSE) continue;
		WinQueryWindowPos(tophwnd, (PSWP)&swp[i]);
		if (!(swp[i].fl & SWP_MINIMIZE)){
			if (!(swp[i].fl & SWP_HIDE)){
				swp[i].x += posX;
				swp[i].y += posY;
				swp[i++].fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
			}
		}
	} else {
		if (WinQueryWindow(tophwnd, QW_FRAMEOWNER) == hwndMain) continue;
		WinQueryWindowPos(tophwnd, (PSWP)&swp2[j++]);
		ownerhwnd = tophwnd;
		while((ownerhwnd = WinQueryWindow(ownerhwnd, QW_FRAMEOWNER)) != NULLHANDLE) {
			if (ownerhwnd != hwndMain){
				if (WinQueryWindow(ownerhwnd, QW_FRAMEOWNER) == NULLHANDLE){
					for (z=0; z < i; z++){
						if (ownerhwnd == swp[z].hwnd){
							fNotMoved = FALSE;
							break;
						}
					}
					if (fNotMoved == FALSE) continue;
					WinQueryWindowPos(ownerhwnd, (PSWP)&swp[i]);
					if (!(swp[i].fl & SWP_MINIMIZE)){
						if (!(swp[i].fl & SWP_HIDE)){
							swp[i].x += posX;
							swp[i].y += posY;
							swp[i++].fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
						}
					}
				} else {
					WinQueryWindowPos(ownerhwnd, (PSWP)&swp2[j++]);
				}
			}
		}
	}
 }

 WinSetMultWindowPos(hab, pswp, i-1);

 if (j > 0){
	for (z=0; z <= j; z++){
		if (WinQueryWindow(swp2[z].hwnd, QW_FRAMEOWNER) != NULLHANDLE){
			if (!(swp2[z].fl & SWP_MINIMIZE)){
				if (!(swp2[z].fl & SWP_HIDE)){
					swp2[z].x += posX;
					swp2[z].y += posY;
					swp2[z].fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
				}
			}
		}
	}
	WinSetMultWindowPos(hab, pswp2, j);
 }

} /* end MoveWindows */

/****************************************************************\
 *  Name: MoveWindowsVisible
 *  Purpose: Move windows to the visible area
\****************************************************************/
VOID MoveWindowsVisible(HWND hwnd)
{
 SWP	swp2[30];	/* array of SWP structures. (max number of windows) */
 SWP	swp[100];
 SWP	tempSwp;
 PSWP	pswp;	/* pointer to SWP array */
 PSWP	pswp2;
 LONG	i = 0, j = 0, z;
 HWND	ownerhwnd, tophwnd, hwndMain; 
 BOOL	fMoved;
 HAB	hab;
 LONG	lXScreen,
	lXBorder,
 	lYScreen,
	lYBorder;

 /* screen size */
 lXScreen = WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN);
 lYScreen = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN);
 lXBorder = WinQuerySysValue(HWND_DESKTOP, SV_CXSIZEBORDER);
 lYBorder = WinQuerySysValue(HWND_DESKTOP, SV_CYSIZEBORDER);

 pswp = &swp[0];
 pswp2 = &swp2[0];
 for (z=0; z < 100; z++){
	swp[z].hwnd = NULLHANDLE;
 }
 for (z=0; z < 30; z++){
	swp2[z].hwnd = NULLHANDLE;
 }
 hwndMain = WinQueryWindow(hwnd, QW_PARENT);
 hab = WinQueryAnchorBlock(hwndMain);
 tophwnd = hwndMain;

 while(((tophwnd = WinQueryWindow(tophwnd, QW_NEXTTOP)) != hwndMain) && i < 100) {
	fMoved = FALSE;
	WinQueryWindowPos(tophwnd, (PSWP)&tempSwp);
	if ((WinQueryWindow(tophwnd, QW_FRAMEOWNER) == NULLHANDLE) && ((!WinIsWindowShowing(tophwnd)) || ((tempSwp.x+tempSwp.cx >= 0) && (tempSwp.x+tempSwp.cx <= lXBorder)) || ((tempSwp.y+tempSwp.cy >= 0) && (tempSwp.y+tempSwp.cy <= lYBorder)) || ((tempSwp.x<=lXScreen) && (tempSwp.x >= lXScreen-lXBorder)) || ((tempSwp.y <= lYScreen) && (tempSwp.y >= lYScreen-lYBorder)))) {
		for (z=0; z <= i; z++){
			if (tophwnd == swp[z].hwnd){
				fMoved = TRUE;
				break;
			}
		}
		if (fMoved) continue;
		WinQueryWindowPos(tophwnd, (PSWP)&swp[i]);
		swp[i].x = 0;
		swp[i].y = 0;
		swp[i++].fl |= (SWP_NOADJUST | SWP_HIDE | SWP_ZORDER);
	} else if ((!WinIsWindowShowing(tophwnd)) || ((tempSwp.x+tempSwp.cx >= 0) && (tempSwp.x+tempSwp.cx <= lXBorder)) || ((tempSwp.y+tempSwp.cy >= 0) && (tempSwp.y+tempSwp.cy <= lYBorder)) || ((tempSwp.x<=lXScreen) && (tempSwp.x >= lXScreen-lXBorder)) || ((tempSwp.y <= lYScreen) && (tempSwp.y >= lYScreen-lYBorder))) {
		if (WinQueryWindow(tophwnd, QW_FRAMEOWNER) == hwndMain) continue;
		WinQueryWindowPos(tophwnd, (PSWP)&swp2[j++]);
		ownerhwnd = tophwnd;
		while((ownerhwnd = WinQueryWindow(ownerhwnd, QW_FRAMEOWNER)) != NULLHANDLE) {
			if (ownerhwnd != hwndMain){
				WinQueryWindowPos(ownerhwnd, (PSWP)&tempSwp);
				if ((WinQueryWindow(ownerhwnd, QW_FRAMEOWNER) == NULLHANDLE) && ((!WinIsWindowShowing(ownerhwnd)) || ((tempSwp.x+tempSwp.cx >= 0) && (tempSwp.x+tempSwp.cx <= lXBorder)) || ((tempSwp.y+tempSwp.cy >= 0) && (tempSwp.y+tempSwp.cy <= lYBorder)) || ((tempSwp.x<=lXScreen) && (tempSwp.x >= lXScreen-lXBorder)) || ((tempSwp.y <= lYScreen) && (tempSwp.y >= lYScreen-lYBorder)))) {
					for (z=0; z < i; z++){
						if (ownerhwnd == swp[z].hwnd){
							swp[z].fl |= (SWP_NOADJUST | SWP_SHOW | SWP_MOVE | SWP_ZORDER);
							fMoved = TRUE;
							break;
						}
					}
					if (fMoved) continue;
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
VOID MoveActiveWindows(HWND hwnd, HWND hwndCurrent)
{
   HWND hwndOwner, hwndMain;
   HDC hdcMain;
   HAB hab;

	hwndMain = WinQueryWindow(hwnd, QW_PARENT);
	hab = WinQueryAnchorBlock(hwndMain);
	hdcMain = WinOpenWindowDC(hwndMain);
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
 SWP	swp;
 INT	xTimes=cp.nrButtons-1;
 INT	yTimes=cp.nrButtons-1;
 LONG	x, y, midpointX, midpointY;
 LONG	lXScreen,
	lXBorder,
 	lYScreen,
	lYBorder;

 /* screen size */
 lXScreen = WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN);
 lYScreen = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN);
 lXBorder = WinQuerySysValue(HWND_DESKTOP, SV_CXSIZEBORDER);
 lYBorder = WinQuerySysValue(HWND_DESKTOP, SV_CYSIZEBORDER);

 WinQueryWindowPos(whwnd, (PSWP)&swp);
 if ((!WinIsWindowShowing(whwnd)) || ((swp.x+swp.cx >= 0) && (swp.x+swp.cx <= lXBorder+1)) || ((swp.y+swp.cy >= 0) && (swp.y+swp.cy <= lYBorder+1)) || ((swp.x<=lXScreen) && (swp.x >= lXScreen-lXBorder-1)) || ((swp.y <= lYScreen) && (swp.y >= lYScreen-lYBorder-1))){  
	do {
		x = swp.x + xTimes*(1+lXScreen);
		midpointX = x + (swp.cx / 2);
		xTimes--;
	}while ((midpointX < 0) || (midpointX > lXScreen));
	do {
		y = swp.y + yTimes*(1+lYScreen);
		midpointY = y + (swp.cy / 2);
		yTimes--;
	}while ((midpointY < 0) || (midpointY > lYScreen));
	WinSetWindowPos (whwnd, HWND_TOP, x, y, swp.cx, swp.cy, swp.fl | SWP_MOVE); /*  | SWP_NOADJUST SWP_ZORDER | SWP_SHOW */
 } /* endif */

} /* end SetWindowPosition */
