.*==============================================================*\
.*                                                              *
.* Main.ipf - Information Tag Language file for the main        *
.*            application's help manager                        *
.*                                                              *
.*==============================================================*/
:userdoc.

.*--------------------------------------------------------------*\
.*  Main window extended help panel                             *
.*      res = PANEL_MAIN                                        *
.*--------------------------------------------------------------*/
:h1 res=2100 name=PANEL_MAIN.About PMVDesk
:i1 id=aboutMain.About PMVDesk
:p.PMVDesk is a Virtual Desktop utility for the OS/2 2.x Workplace Shell.  
When you start the program you will see a 9 button grid. Each button 
represents a screen (of your current videoadapter resolution). The 
current screen is marked with an "X".  
:p.When you press one of the buttons (move the cursor over it and click 
mouse button 1) you move to that virtual location. (In this version 
you can not see your open windows in the PMVDesk application, ...but 
just wait for the future versions). Your Workplace Shell Desktop 
(icons) will always be visible. This way you can easily get hold of those
objects (where ever you are).


.*--------------------------------------------------------------*/
:h1 res=2150 name=PANEL_ENVIRONMENT.Environment variables
:i1 id=environment.Environment variables
:p.PMVDesk uses a environment variable: PMVDESK_INI. This variable
specifies the filename (including full path) that PMVDesk should use
for storing its options. If this environment variable is not set,
PMVDesk will create the file PMVDESK.INI in the current directory
and store its information there. I recommend that you put the line:
:ul.
:li.SET PMVDESK_INI=C&colon.\OS2\PMVDESK.INI
:eul.
:p.in your CONFIG.SYS (Of course, you can use any other path or filename
that you want). By doing this you avoid multiple copies of PMVDESK.INI
scattered around your hard drive.
:note.You will have to reboot before this line will take effect.
:p.PMVDesk will "remember" it's last position and virtual screen. If you 
put a PMVDesk Program Object in your startup folder, it will start where 
you last left it. You may also "Shutdown" your system without closing 
PMVDesk (because "Shutdown" closes it for you). Remember to put the 
line:
:ul.
:li.SET RESTARTOBJECTS=STARTUPFOLDERSONLY
:eul.
:p.in your CONFIG.SYS file before SET AUTOSTART=PROGRAMS, TASKLIST,
FOLDERS, CONNECTIONS.


.*--------------------------------------------------------------*\
.*  Help menu Keys command help panel                           *
.*      res = PANEL_HELPKEYS                                    *
.*--------------------------------------------------------------*/
:h1 res=2950 name=PANEL_HELPKEYS.Key Assignments
:i1 id=help.Key Assignments
:p.When two key names are joined by a plus sign (+),
use these two keys together. Hold down the first key
and press the second key.
:parml tsize=15 break=none.
:hp2.
:pt.ALT + F
:ehp2.
:pd.Frame controls
:hp2.
:pt.ALT + T
:ehp2.
:pd.Float on top
:hp2.
:pt.SHIFT + F10
:ehp2.
:pd.Popup menu
:hp2.
:pt.ALT + F4
:ehp2.
:pd.Close window and exit application
:hp2.
:pt.ALT + F5
:ehp2.
:pd.Restore window
:hp2.
:pt.ALT + F7
:ehp2.
:pd.Move window
:hp2.
:pt.ALT + F8
:ehp2.
:pd.Size window
:hp2.
:pt.ALT + F9
:ehp2.
:pd.Minimize window
:hp2.
:pt.ALT + F10
:ehp2.
:pd.Maximize window
:hp2.
:pt.ALT + F11
:ehp2.
:pd.Hide window
:hp2.
:pt.CTRL + ESC
:ehp2.
:pd.Workplace Shell "Window List"
:hp2.
:pt.F1
:ehp2.
:pd.Help
:eparml.


.*-- Import the Options menu help file --*
.im options.ipf

.*-- Import the popup menu help file --*
.im popup.ipf

.*-- Import the tutorial menu help file --*
.im tutorial.ipf

.*-- Import the Help menu help file --*
.*-- im help.ipf --*

.*-- Import the Application Menu help file --*
.*-- im menu.ipf --*

.*-- Import the Dialog boxes help file --*
.*-- im dlg.ipf --*

:euserdoc.
