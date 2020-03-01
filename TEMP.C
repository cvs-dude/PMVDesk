LONG FindFrameWindows(HWND hwndMain, HWND hwndParent, BOOL fNullhandle, PSWP pswp, LONG lPosX, LONG lPosY, LONG i)
{
 HWND	hWin; 
 HENUM	hEnum;
 CHAR	szClassBuffer[20];
 CHAR	szWindowTitle[32];	

 hEnum = WinBeginEnumWindows(hwndParent);
 while (hWin = WinGetNextWindow(hEnum))
 {
	WinQueryClassName(hWin, sizeof(szClassBuffer), szClassBuffer);
	if (strcmp(szClassBuffer, "#1")) continue;
	if (hWin == hwndMain) continue;
	if (fNullhandle){
		if (NULLHANDLE != WinQueryWindow(hWin, QW_FRAMEOWNER)) continue;
	} else {
		if (NULLHANDLE == WinQueryWindow(hWin, QW_FRAMEOWNER)) continue;
	}
	WinQueryWindowText(hWin, sizeof(szWindowTitle), szWindowTitle);
	if (!strcmp(szWindowTitle, "Desktop")) continue;
	if (!strcmp(szWindowTitle, "Window List")) continue;
	i = FindFrameWindows(hwndMain, hWin, fNullhandle, pswp, lPosX, lPosY, i);
	WinQueryWindowPos(hWin, pswp);
	if (pswp->fl & SWP_MINIMIZE) continue;
	if (pswp->fl & SWP_HIDE) continue;
/*	WinQueryWindowText(hWin, sizeof(szWindowTitle), szWindowTitle);
	WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
		szWindowTitle,
		(PSZ)NULL,
		0,
		MB_OK | MB_ERROR);
*/	pswp->x += lPosX;
	pswp->y += lPosY;
	pswp->fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
	pswp++;
	i++;
 }
 WinEndEnumWindows(hEnum);
 return i;
} /* FindFrameWindows */

/****************************************************************\
 *  Name: MoveWindows
 *  Purpose: Move windows to their virtual screen
\****************************************************************/
VOID MoveWindows(HWND hwnd, LONG lX, LONG lY)
{
 SWP	swp[100];	/* array of SWP structures. (max number of windows) */
 SWP	swpSub[30];
 PSWP	pswp;	/* pointer to SWP array */
 PSWP	pswpSub;
 LONG	i = 0, j = 0, z;
 LONG	lPosX, lPosY;
 HWND	hwndMain, hwndParent; 
 HAB	hab;
 LONG	lXScreen,
 	lYScreen;

 for (z=0; z < 100; z++){
	swp[z].hwnd = NULLHANDLE;
 }

 /* screen size */
 lXScreen = WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN);
 lYScreen = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN);
 lPosX = (cp.currentAtx - lX)*(1 + lXScreen);
 lPosY = (cp.currentAty - lY)*(1 + lYScreen);
 pswp = &swp[0];
 hwndMain = WinQueryWindow(hwnd, QW_PARENT);
 hab = WinQueryAnchorBlock(hwndMain);
 hwndParent = WinQueryDesktopWindow(hab, NULLHANDLE);
 i = FindFrameWindows(hwndMain, hwndParent, TRUE, pswp, lPosX, lPosY, i);
 pswpSub = &swpSub[0];
 j = FindFrameWindows(hwndMain, hwndParent, FALSE, pswpSub, lPosX, lPosY, j);
 WinSetMultWindowPos(hab, pswp, i-1);
 WinSetMultWindowPos(hab, pswpSub, j-1);
} /* end MoveWindows */



LONG FindFrameWindows(HWND hwndMain, HWND hwndParent, BOOL fNullhandle, PSWP pswp, LONG lPosX, LONG lPosY, LONG i)
{
 HWND	hWin; 
 HENUM	hEnum;
 CHAR	szClassBuffer[20];
 CHAR	szWindowTitle[32];	

 hEnum = WinBeginEnumWindows(hwndParent);
 while (hWin = WinGetNextWindow(hEnum))
 {
	WinQueryClassName(hWin, sizeof(szClassBuffer), szClassBuffer);
	if (strcmp(szClassBuffer, "#1")) continue;
	if (hWin == hwndMain) continue;
	if (fNullhandle){
		if (NULLHANDLE != WinQueryWindow(hWin, QW_FRAMEOWNER)) continue;
	} else {
		if (NULLHANDLE == WinQueryWindow(hWin, QW_FRAMEOWNER)) continue;
	}
	WinQueryWindowText(hWin, sizeof(szWindowTitle), szWindowTitle);
	if (!strcmp(szWindowTitle, "Desktop")) continue;
	if (!strcmp(szWindowTitle, "Window List")) continue;
	i = FindFrameWindows(hwndMain, hWin, fNullhandle, pswp, lPosX, lPosY, i);
	WinQueryWindowPos(hWin, pswp);
	if (pswp->fl & SWP_MINIMIZE) continue;
	if (pswp->fl & SWP_HIDE) continue;
/*	WinQueryWindowText(hWin, sizeof(szWindowTitle), szWindowTitle);
	WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
		szWindowTitle,
		(PSZ)NULL,
		0,
		MB_OK | MB_ERROR);
*/	pswp->x += lPosX;
	pswp->y += lPosY;
	pswp->fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
	pswp++;
	i++;
 }
 WinEndEnumWindows(hEnum);
 return i;
} /* FindFrameWindows */

/****************************************************************\
 *  Name: MoveWindows
 *  Purpose: Move windows to their virtual screen
\****************************************************************/
VOID MoveWindows(HWND hwnd, LONG lX, LONG lY)
{
 SWP	swp[100];	/* array of SWP structures. (max number of windows) */
 PSWP	pswp;	/* pointer to SWP array */
 PSWP	pswpSub;
 LONG	i = 0, j = 0, z;
 LONG	lPosX, lPosY;
 HWND	hwndMain, hwndParent; 
 HAB	hab;
 LONG	lXScreen,
 	lYScreen;

 for (z=0; z < 100; z++){
	swp[z].hwnd = NULLHANDLE;
 }

 /* screen size */
 lXScreen = WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN);
 lYScreen = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN);
 lPosX = (cp.currentAtx - lX)*(1 + lXScreen);
 lPosY = (cp.currentAty - lY)*(1 + lYScreen);
 pswp = &swp[0];
 hwndMain = WinQueryWindow(hwnd, QW_PARENT);
 hab = WinQueryAnchorBlock(hwndMain);
 hwndParent = WinQueryDesktopWindow(hab, NULLHANDLE);
 i = FindFrameWindows(hwndMain, hwndParent, TRUE, pswp, lPosX, lPosY, i);
 pswpSub = &swp[i];
 j = FindFrameWindows(hwndMain, hwndParent, FALSE, pswpSub, lPosX, lPosY, j);
 WinSetMultWindowPos(hab, pswp, i-1);
 WinSetMultWindowPos(hab, pswpSub, j-1);
} /* end MoveWindows */



LONG FindFrameWindows(HWND hwndMain, HWND hwndParent, PSWP pswp, LONG lPosX, LONG lPosY, LONG i)
{
 HWND	hWin; 
 HENUM	hEnum;
 CHAR	szClassBuffer[20];
 CHAR	szWindowTitle[32];	

 hEnum = WinBeginEnumWindows(hwndParent);
 while (hWin = WinGetNextWindow(hEnum))
 {
	WinQueryClassName(hWin, sizeof(szClassBuffer), szClassBuffer);
	if (strcmp(szClassBuffer, "#1")) continue;
	if (hWin == hwndMain) continue;
	WinQueryWindowText(hWin, sizeof(szWindowTitle), szWindowTitle);
	if (!strcmp(szWindowTitle, "Desktop")) continue;
	if (!strcmp(szWindowTitle, "Window List")) continue;
	i = FindFrameWindows(hwndMain, hWin, pswp, lPosX, lPosY, i);
	WinQueryWindowPos(hWin, pswp);
	if (pswp->fl & SWP_MINIMIZE) continue;
	if (pswp->fl & SWP_HIDE) continue;
	WinQueryWindowText(hWin, sizeof(szWindowTitle), szWindowTitle);
	WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
		szWindowTitle,
		(PSZ)NULL,
		0,
		MB_OK | MB_ERROR);
	pswp->x += lPosX;
	pswp->y += lPosY;
	pswp->fl |= (SWP_NOADJUST | SWP_MOVE);
	pswp++;
	i++;
 }
 WinEndEnumWindows(hEnum);
 return i;
} /* FindFrameWindows */

/****************************************************************\
 *  Name: MoveWindows
 *  Purpose: Move windows to their virtual screen
\****************************************************************/
VOID MoveWindows(HWND hwnd, LONG lX, LONG lY)
{
 SWP	swp[100];	/* array of SWP structures. (max number of windows) */
 PSWP	pswp;	/* pointer to SWP array */
 LONG	i = 0;
 LONG	lPosX, lPosY;
 HWND	hwndMain, hwndParent; 
 HAB	hab;
 LONG	lXScreen,
 	lYScreen;

 /* screen size */
 lXScreen = WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN);
 lYScreen = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN);
 lPosX = (cp.currentAtx - lX)*(1 + lXScreen);
 lPosY = (cp.currentAty - lY)*(1 + lYScreen);
 pswp = &swp[0];
// for (z=0; z < 100; z++){
// 	swp[z].hwnd = NULLHANDLE;
// }
 hwndMain = WinQueryWindow(hwnd, QW_PARENT);
 hab = WinQueryAnchorBlock(hwndMain);
 hwndParent = WinQueryDesktopWindow(hab, NULLHANDLE);
 i = FindFrameWindows(hwndMain, hwndParent, pswp, lPosX, lPosY, i);
 WinSetMultWindowPos(hab, pswp, i-1);
} /* end MoveWindows */



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
				swp[i].x += posX;
				swp[i].y += posY;
				swp[i++].fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
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
							swp[i].x += posX;
							swp[i].y += posY;
							swp[i++].fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
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
					swp2[z].x += posX;
					swp2[z].y += posY;
					swp2[z].fl |= (SWP_NOADJUST | SWP_MOVE | SWP_ZORDER);
				}
			}
		}
		WinSetMultWindowPos(hab, pswp2, j);
	}

} /* end MoveWindows */
