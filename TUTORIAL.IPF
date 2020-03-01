.*==============================================================*\
.*                                                              *
.* Tutorial.ipf - Information Tag Language file for the Tutorial*
.*            menu help panels.                                 *
.*                                                              *
.*==============================================================*/

.*--------------------------------------------------------------*\
.*  Main Tutorial menu                                          *
.*      res = PANEL_HELPTUTORIAL                                *
.*--------------------------------------------------------------*/
:h1 res=2910 name=PANEL_HELPTUTORIAL.Tutorial
:i1 id=Tutorial.Tutorial
:p.
:hp2.
Beginners guide using virtual desktop applications.
:ehp2.
:p.Usually you work with several windows at the same time,
but often find that it would be nice to have a bigger screen,
so you could see all your windows. Then you have a look in
your wallet, and find no many, and no credit for your credit
card. This is when you need a cheap (read: free) virtual
desktop to emulate a big screen.
:p.My implementation is not perfect (and not the final version), but
gives you nine screens where you can open different windows, and
easily jump between these screens, just by pressing one of the
buttons.
:p.Example:
:ol.
:li.Start PMVDesk (you see a X on the middle button, so this
represents the current screen).
:li.Open a window, or a program (it opens normally).
:li.Press the lower left button on PMVDesk (your open windows
seems to disappear, and you have a clean screen. Do not worry,
your windows are still there, just press the middle button and
you are back where you started).
:li.In this new screen you may open new windows and programs.
:li.To quickly return back just press that button on PMVDesk that
represents the screen where you want to go to.
:eol.

:p.So, this is how you use a virtual desktop. There are different
kinds of virtual desktop applications, with fancy miniature windows
representing your windows, and that might be my final goal when I
continue developing my application.

.*--------------------------------------------------------------*\
.*  Troubleshooting menu                                          *
.*      res = PANEL_TROUBLESHOOTING                                *
.*--------------------------------------------------------------*/
:h1 res=2980 name=PANEL_TROUBLESHOOTING.Troubleshooting
:i1 id=Troubleshooting.Troubleshooting
:p. Recovering "lost" windows: 
:ul.
:li.Choose "Retrieve windows" from PMVDesk "Settings" menu to
bring all windows to the lover left corner of the visible area.
:note.Retrieved windows are hidden. To activate: select application
from Workplace Shell "Window List". Only windows with child windows
(ex. dialog windows) are retrieved visible.
:li.You may choose the "lost" program from Workplace Shell "Window List"
and cascade the program back to the visible area (press mouse button
2, choose "Cascade").
:eul.

