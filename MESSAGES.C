/**************************************************************************
 *  File name  :  messages.c
 *
 *  Description:  
 *
 *	This source file contains the following functions:
 *
 *	UserWndProc(hwnd, msg, mp1, mp2)
 *	UserCommand()
 *************************************************************************/

/*
 *  Include files, macros, defined constants, and externs
 */
#define INCL_WINHELP
#define INCL_WINFRAMEMGR
#define INCL_WINTRACKRECT
#define INCL_WININPUT

#include <os2.h>
#include "strings.h"
#include "pmvd.h"
#include "xtrn.h"

/*
 *  Global variables
 */

/*
 *  Entry point declarations
 */

/**************************************************************************
 *  Name       : UserCommand(mp1, mp2)
 *
 *  Description: Process any WM_COMMAND messages sent to hwndMain
 *               that are not processed by MainCommand.
 *
 *  Concepts:    Routine is called for each WM_COMMAND that is
 *               not posted by a standard menu item.
 *               A switch statement branches control based upon
 *               the id of the control which posted the message.
 *
 *  Parameters :  mp1  = first message parameter
 *                mp2  = second message parameter
 *
 *  Return     :  [none]
 *************************************************************************/
VOID UserCommand(HWND hwnd, MPARAM mp1, MPARAM mp2)
{
 HWND	hwndMain;
 SWP	swp;
 LONG	x, y;
 static LONG	lXMoved=0, lYMoved=0;

 switch (SHORT1FROMMP(mp1))
 {
	case IDM_BUTTON0:
	case IDM_BUTTON1:
	case IDM_BUTTON2:
	case IDM_BUTTON3:
	case IDM_BUTTON4:
	case IDM_BUTTON5:
	case IDM_BUTTON6:
	case IDM_BUTTON7:
	case IDM_BUTTON8:
	case IDM_BUTTON9:
	case IDM_BUTTON10:
	case IDM_BUTTON11:
	case IDM_BUTTON12:
	case IDM_BUTTON13:
	case IDM_BUTTON14:
	case IDM_BUTTON15:
		if (SHORT1FROMMP(mp1) != (cp.currentAty*4+cp.currentAtx)){
			WinSetWindowText(WinWindowFromID(hwnd, (cp.currentAty*4+cp.currentAtx)), "");
			if (SHORT1FROMMP(mp1) < 4) {
				y = 0;
				x = SHORT1FROMMP(mp1);
			} else if (SHORT1FROMMP(mp1) < 8) {
				y = 1;
				x = SHORT1FROMMP(mp1) - 4;
			} else if (SHORT1FROMMP(mp1) < 12) {
				y = 2;
				x = SHORT1FROMMP(mp1) - 8;
			} else {
				y = 3;
				x = SHORT1FROMMP(mp1) - 12;
			}
			WinSetWindowText(WinWindowFromID(hwnd, (y*4+x)), "X");
			MoveWindows(hwnd, x, y);
			cp.currentAty = y;
			cp.currentAtx = x;
		}
		break;

	case IDM_TOGGLECONTROLS :
		hwndMain = WinQueryWindow(hwnd, QW_PARENT);
		FrameControls(hwndMain, cp.fControlsHidden, FALSE);
		break ;

	case IDM_FLOAT :
		if (cp.floatOnTop) {
			cp.floatOnTop = FALSE;
		} else {
			cp.floatOnTop = TRUE;
		}
		break;

	case IDM_RETRIEVE :
		if (cp.retrieveWindows) {
			cp.retrieveWindows = FALSE;
		} else {
			cp.retrieveWindows = TRUE;
		}
		break;

	case IDM_VISIBLE :
		MoveWindowsVisible(hwnd);
		break;

	case IDM_2X2 :
		if (cp.nrButtons != 2){
			mp2 = MPFROM2SHORT(0, 0);
			MainSize(hwnd, mp2);
			cp.nrButtons = 2;
			WinSetWindowText(WinWindowFromID(hwnd, (cp.currentAty*4+cp.currentAtx)), "");
			if (cp.currentAtx > 1){
				lXMoved += cp.currentAtx - 1;
				cp.currentAtx = 1;
			}
			if (cp.currentAty > 1){
				lYMoved += cp.currentAty - 1;
				cp.currentAty = 1;
			}
			WinQueryWindowPos(hwnd, (PSWP)&cp.swp);
			mp2 = MPFROM2SHORT(cp.swp.cx, cp.swp.cy);
			MainSize(hwnd, mp2);
		}
		break ;

	case IDM_3X3 :
		if (cp.nrButtons != 3){
			mp2 = MPFROM2SHORT(0, 0);
			MainSize(hwnd, mp2);
			cp.nrButtons = 3;
			WinSetWindowText(WinWindowFromID(hwnd, (cp.currentAty*4+cp.currentAtx)), "");
			if (cp.currentAtx > 2){
				lXMoved = 1;
				cp.currentAtx = 2;
			}
			if (cp.currentAty > 2){
				lYMoved = 1;
				cp.currentAty = 2;
			}
			if ((cp.currentAtx < 2) && (lXMoved > 0)){
				cp.currentAtx += 1;
				lXMoved -= 1;
			}
			if ((cp.currentAty < 2) && (lYMoved > 0)){
				cp.currentAty += 1;
				lYMoved -= 1;
			}
			if ((cp.currentAtx < 1) && (lXMoved > 0)){
				cp.currentAtx += 1;
				lXMoved -= 1;
			}
			if ((cp.currentAty < 1) && (lYMoved > 0)){
				cp.currentAty += 1;
				lYMoved -= 1;
			}
			WinQueryWindowPos(hwnd, (PSWP)&cp.swp);
			mp2 = MPFROM2SHORT(cp.swp.cx, cp.swp.cy);
			MainSize(hwnd, mp2);
		}
		break ;

	case IDM_4X4 :
		if (cp.nrButtons != 4){
			mp2 = MPFROM2SHORT(0, 0);
			MainSize(hwnd, mp2);
			cp.nrButtons = 4;
			WinSetWindowText(WinWindowFromID(hwnd, (cp.currentAty*4+cp.currentAtx)), "");
			if ((cp.currentAtx < 3) && (lXMoved > 0)){
				cp.currentAtx += lXMoved;
				lXMoved = 0;
			}
			if ((cp.currentAty < 3) && (lYMoved > 0)){
				cp.currentAty += lYMoved;
				lYMoved = 0;
			}
			WinQueryWindowPos(hwnd, (PSWP)&cp.swp);
			mp2 = MPFROM2SHORT(cp.swp.cx, cp.swp.cy);
			MainSize(hwnd, mp2);
		}
		break ;

	case IDM_POPUPMENU :
		WinPostMsg(hwndButton[0][0], WM_CONTEXTMENU, MPVOID, MPVOID);
		break;

   case IDM_HELPINDEX:
      HelpIndex();
      break;

   case IDM_HELPGENERAL:
      HelpGeneral();
      break;

   case IDM_HELPUSINGHELP:
      HelpUsingHelp();
      break;

   case IDM_HELPKEYS:
      HelpKeys();
      break;

	case IDM_HELPPRODUCTINFO:
		hwndMain = WinQueryWindow(hwnd, QW_PARENT);
		HelpProductInfo(hwndMain);
		break;

	case SC_HIDE:
                /*
                 * Get the restore position that SetMultWindowPos will use.
                 */
		hwndMain = WinQueryWindow(hwnd, QW_PARENT);
		WinQueryWindowPos(hwndMain, &swp);
		swp.fl = SWP_HIDE;
		WinSetMultWindowPos(NULLHANDLE, (PSWP)&swp, 1);
		break;

	case SC_SIZE:
		hwndMain = WinQueryWindow(hwnd, QW_PARENT);
		WinSendMsg(hwndMain, WM_TRACKFRAME,
			MPFROMSHORT(TF_SETPOINTERPOS), MPVOID );
		break;

	case SC_MOVE:
		hwndMain = WinQueryWindow(hwnd, QW_PARENT);
		WinSendMsg (hwndMain, WM_TRACKFRAME,
		MPFROMSHORT(TF_SETPOINTERPOS | TF_MOVE), MPVOID );
		break;

	case SC_CLOSE:
		hwndMain = WinQueryWindow(hwnd, QW_PARENT);
		WinPostMsg (hwndMain, WM_CLOSE, MPVOID, MPVOID );
		break;

	default:
		break;
 }

} /* End of UserCommand  */
