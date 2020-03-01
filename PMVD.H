/**************************************************************************
 *  File name  :	pmvd.h
 *
 *  Description:	This header file contains the constants
 *		and global external variables.
 *************************************************************************/

typedef struct _PMVDPREFS
{
	BOOL fControlsHidden;
	BOOL floatOnTop;
	BOOL retrieveWindows;
	LONG currentAtx, currentAty, nrButtons;
	SWP swp;
} PMVDPREFS;

extern PMVDPREFS	cp;
extern BOOL		fHelpEnabled;
extern HWND		hwndButton[4][4];
extern PFNWP		OldFrameProc;

#define UM_INIT_POPUP_MENU		WM_USER

#define ID_MAINWND		1000
#define ID_MSGBOX		1100

#define TEMPLATE_HELP_TABLE		2000
#define SUBTABLE_MAIN		2100

#define IDD_PRODUCTINFO     1010	/*<-@UNI->*/
#define IDC_OK		1
#define IDC_CANCEL	2
#define IDC_HELP	3
#define IDC_ICON	4
#define IDC_BITMAP	5	/*<-@UNI  */
#define IDR_BITMAP	6	/*  @UNI->*/

/*
 *  Other constants
 */
#define MESSAGELEN		50 /* maximum length for messages */

/*
 *  Ids of messages
 */
#define ID_COPY_SCREEN		10
#define ID_CURRENT_SCREEN		11

/*
 *  Ids of messages used in menus and buttons
 */
#define IDM_BUTTON0		0
#define IDM_BUTTON1		1
#define IDM_BUTTON2		2
#define IDM_BUTTON3		3
#define IDM_BUTTON4		4
#define IDM_BUTTON5		5
#define IDM_BUTTON6		6
#define IDM_BUTTON7		7
#define IDM_BUTTON8		8
#define IDM_BUTTON9		9
#define IDM_BUTTON10		10
#define IDM_BUTTON11		11
#define IDM_BUTTON12		12
#define IDM_BUTTON13		13
#define IDM_BUTTON14		14
#define IDM_BUTTON15		15

#define IDM_EDIT		2000
#define IDM_EDITUNDO		2100
#define IDM_EDITCUT		2200
#define IDM_EDITCOPY		2300
#define IDM_EDITPASTE		2400

#define IDM_OPTIONS		3100
#define IDM_TOGGLECONTROLS		3200
#define IDM_FLOAT		3300
#define IDM_RETRIEVE		3400
#define IDM_UTIL		3500
#define IDM_VISIBLE		3510
#define IDM_BUTTONS		3600
#define IDM_2X2		3602
#define IDM_3X3		3603
#define IDM_4X4		3604

#define IDM_POPUPMENU		5100
#define IDM_WINDOW		5110
#define IDM_CANCEL		5120

#define IDM_HELP		9000
#define IDM_HELPUSINGHELP		9100
#define IDM_HELPGENERAL		9200
#define IDM_HELPKEYS		9300
#define IDM_HELPINDEX		9400
#define IDM_HELPTUTORIAL		9500
#define IDM_HELPPRODUCTINFO		9600

/*
 *  Main window help panels
 */
#define PANEL_MAIN                          2100
#define PANEL_ENVIRONMENT                   2150
#define PANEL_PRODUCTINFODLG                3100

#define PANEL_POPUP                         2200
#define PANEL_POPUPHELP                     2210
#define PANEL_WINDOW                        2220
#define PANEL_UTIL                          2230
#define PANEL_FRAMECONTROLS                 2240
#define PANEL_FLOATONTOP                    2250
#define PANEL_RETRIEVE                      2260
#define PANEL_CANCEL                        2270
#define PANEL_BUTTONS                       2280
#define PANEL_EDIT                          2310
#define PANEL_EDITUNDO                      2320
#define PANEL_EDITCUT                       2330
#define PANEL_EDITCOPY                      2340
#define PANEL_EDITPASTE                     2350
#define PANEL_EDITCLEAR                     2360
#define PANEL_HELP                          2910
#define PANEL_HELPINDEX                     2920
#define PANEL_HELPGENERAL                   2930
#define PANEL_HELPUSINGHELP                 2940
#define PANEL_HELPKEYS                      2950
#define PANEL_HELPTUTORIAL                  2960
#define PANEL_HELPPRODUCTINFO               2970
#define PANEL_TROUBLESHOOTING               2980
